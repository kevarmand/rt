// tri2spheres.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct { double x,y,z; } vec3;

static vec3 vsub(vec3 a, vec3 b){ return (vec3){a.x-b.x,a.y-b.y,a.z-b.z}; }
static vec3 vadd(vec3 a, vec3 b){ return (vec3){a.x+b.x,a.y+b.y,a.z+b.z}; }
static vec3 vscale(vec3 a, double s){ return (vec3){a.x*s,a.y*s,a.z*s}; }
static double vdot(vec3 a, vec3 b){ return a.x*b.x + a.y*b.y + a.z*b.z; }
static vec3 vcross(vec3 a, vec3 b){
    return (vec3){a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}
static double vlen(vec3 a){ return sqrt(vdot(a,a)); }




static int parse_triangle_line(const char *line,
                               vec3 *A, vec3 *B, vec3 *C,
                               int *R, int *G, int *Bc)
{
    // Expected: tr ax,ay,az bx,by,bz cx,cy,cz R,G,B
    // Tabs/spaces tolerated between groups
    float ax,ay,az,bx,by,bz,cx,cy,cz;
    int r,g,b;
    int n = sscanf(line,
        " tr %f,%f,%f %f,%f,%f %f,%f,%f %d,%d,%d",
        &ax,&ay,&az, &bx,&by,&bz, &cx,&cy,&cz, &r,&g,&b);
    if (n == 12){
        *A = (vec3){ax,ay,az};
        *B = (vec3){bx,by,bz};
        *C = (vec3){cx,cy,cz};
        *R = r; *G = g; *Bc = b;
        return 1;
    }
    // Also accept no leading space after 'tr'
    n = sscanf(line,
        "tr %f,%f,%f %f,%f,%f %f,%f,%f %d,%d,%d",
        &ax,&ay,&az, &bx,&by,&bz, &cx,&cy,&cz, &r,&g,&b);
    if (n == 12){
        *A = (vec3){ax,ay,az};
        *B = (vec3){bx,by,bz};
        *C = (vec3){cx,cy,cz};
        *R = r; *G = g; *Bc = b;
        return 1;
    }
    return 0;
}

static double triangle_area(vec3 A, vec3 B, vec3 C){
    vec3 AB = vsub(B,A), AC = vsub(C,A);
    return 0.5 * vlen(vcross(AB,AC));
}

// Incenter (weights by side lengths) and inradius r = 2*Area / (a+b+c)
static void in_sphere(vec3 A, vec3 B, vec3 C, vec3 *center, double *radius){
    vec3 AB = vsub(B,A), AC = vsub(C,A);
    double a = vlen(vsub(B,C)); // |BC|
    double b = vlen(vsub(C,A)); // |CA|
    double c = vlen(vsub(A,B)); // |AB|
    double per = a + b + c;

    if (per <= 1e-15){
        *center = A;
        *radius = 0.0;
        return;
    }

    // Incentre (poids = longueurs opposées)
    vec3 num = vadd( vscale(A,a), vadd( vscale(B,b), vscale(C,c) ) );
    *center = vscale(num, 1.0 / per);

    // Inrayon r = 2*Area / perimeter
    double area2 = vlen( vcross(AB, AC) ); // = 2*Area
    *radius = area2 / per;
}


// Centre et rayon de la sphère circonscrite du triangle ABC.
// Renvoie centre=A et rayon=0 si triangle dégénéré.
static void circum_sphere(vec3 A, vec3 B, vec3 C, vec3 *center, double *radius){
    vec3 u = vsub(B, A);
    vec3 v = vsub(C, A);
    vec3 n = vcross(u, v);

    double n2 = vdot(n, n);                // ||n||^2
    if (n2 <= 1e-18){                      // triangle quasi-colinéaire
        *center = A;
        *radius = 0.0;
        return;
    }

    double u2 = vdot(u, u);                // ||u||^2
    double v2 = vdot(v, v);                // ||v||^2

    // term1 = ||u||^2 * (v x n)
    vec3 term1 = vcross(v, n);
    term1 = vscale(term1, u2);

    // term2 = ||v||^2 * (n x u)
    vec3 term2 = vcross(n, u);
    term2 = vscale(term2, v2);

    vec3 num = vadd(term1, term2);
    vec3 offs = vscale(num, 1.0 / (2.0 * n2));
    *center = vadd(A, offs);

    // Rayon via a*b*c / (4A)
    double a = vlen(vsub(B, C));
    double b = vlen(vsub(C, A));
    double c = vlen(vsub(A, B));
    double area2 = vlen(n);                // = 2*A
    *radius = (a * b * c) / (2.0 * area2); // car 4A = 2*|n|
}


static void make_output_names(const char *in,
                              char *out_circ, size_t cap_circ,
                              char *out_in, size_t cap_in)
{
    size_t len = strlen(in);
    const char *dot = (len >= 3 && strcmp(in+len-3, ".rt")==0) ? in+len-3 : NULL;
    if (dot){
        size_t base = (size_t)(dot - in);
        snprintf(out_circ, cap_circ, "%.*s_circonscrit.rt", (int)base, in);
        snprintf(out_in, cap_in, "%.*s_inscrit.rt", (int)base, in);
    }else{
        snprintf(out_circ, cap_circ, "%s_circonscrit.rt", in);
        snprintf(out_in, cap_in, "%s_inscrit.rt", in);
    }
}

int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "Usage: %s input.rt\n", argv[0]);
        return 1;
    }
    const char *inpath = argv[1];
    FILE *fin = fopen(inpath, "r");
    if (!fin){ perror("open input"); return 1; }

    char out_circ[1024], out_in[1024];
    make_output_names(inpath, out_circ, sizeof(out_circ), out_in, sizeof(out_in));

    FILE *fcirc = fopen(out_circ, "w");
    if (!fcirc){ perror("open output circum"); fclose(fin); return 1; }
    FILE *fins = fopen(out_in, "w");
    if (!fins){ perror("open output in"); fclose(fin); fclose(fcirc); return 1; }

    char line[4096];
    while (fgets(line, sizeof(line), fin)){
        // Si ce n'est pas une ligne 'tr', recopier telle quelle
        if (!(line[0]=='t' && line[1]=='r') && !(line[0]==' ' && line[1]=='t' && line[2]=='r')){
            fputs(line, fcirc);
            fputs(line, fins);
            continue;
        }

        vec3 A,B,C; int R,G,Bc;
        if (!parse_triangle_line(line, &A,&B,&C, &R,&G,&Bc)){
            // Pas au format attendu => recopier pour ne rien casser
            fputs(line, fcirc);
            fputs(line, fins);
            continue;
        }

        // Sécurité dégénérescence : si aire ~ 0, recopier
        if (triangle_area(A,B,C) <= 1e-12){
            fputs(line, fcirc);
            fputs(line, fins);
            continue;
        }

        vec3 cin;  double rin;
        vec3 ccir; double rcir;
        in_sphere(A,B,C, &cin, &rin);
        circum_sphere(A,B,C, &ccir, &rcir);
		rin*=3; // Écrire les diamètres
        // Écrire les sphères (rayon, pas diamètre)
        // Format: sp x,y,z rayon R,G,B
        fprintf(fins,  "sp %.2f,%.2f,%.2f %.2f %d,%d,%d\n",
                cin.x,cin.y,cin.z, rin, R,G,Bc);
        fprintf(fcirc, "sp %.2f,%.2f,%.2f %.2f %d,%d,%d\n",
                ccir.x,ccir.y,ccir.z, rcir, R,G,Bc);
    }

    fclose(fin); fclose(fcirc); fclose(fins);
    return 0;
}
