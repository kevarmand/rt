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
    double a = vlen(vsub(B,C));
    double b = vlen(vsub(C,A));
    double c = vlen(vsub(A,B));
    double per = a + b + c;
    if (per <= 1e-12){ *center = A; *radius = 0.0; return; }
    *center = vscale(A, a/per);
    *center = vadd(*center, vscale(B, b/per));
    *center = vadd(*center, vscale(C, c/per));
    double area = triangle_area(A,B,C);
    *radius = (per > 0.0) ? (2.0 * area / per) : 0.0;
}

// Circumcenter (weights by squared side lengths) and R = a*b*c / (4*Area)
static void circum_sphere(vec3 A, vec3 B, vec3 C, vec3 *center, double *radius){
    double a = vlen(vsub(B,C));
    double b = vlen(vsub(C,A));
    double c = vlen(vsub(A,B));
    double a2 = a*a, b2 = b*b, c2 = c*c;
    double s = a2 + b2 + c2;
    if (s <= 1e-18){ *center = A; *radius = 0.0; return; }
    *center = vscale(A, a2/s);
    *center = vadd(*center, vscale(B, b2/s));
    *center = vadd(*center, vscale(C, c2/s));
    double area = triangle_area(A,B,C);
    if (area <= 1e-18){ *radius = 0.0; }
    else *radius = (a * b * c) / (4.0 * area);
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
		rin*=2.0; // Écrire les diamètres
        // Écrire les sphères (rayon, pas diamètre)
        // Format: sp x,y,z rayon R,G,B
        fprintf(fins,  "sp %.6f,%.6f,%.6f %.6f %d,%d,%d\n",
                cin.x,cin.y,cin.z, rin, R,G,Bc);
        fprintf(fcirc, "sp %.6f,%.6f,%.6f %.6f %d,%d,%d\n",
                ccir.x,ccir.y,ccir.z, rcir, R,G,Bc);
    }

    fclose(fin); fclose(fcirc); fclose(fins);
    return 0;
}
