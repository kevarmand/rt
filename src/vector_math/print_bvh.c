// bvh_print.c — rendu BVH équiréparti + split (propre & modulaire)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/ioctl.h>
#include <unistd.h>

/* === Types === */
typedef struct s_vec3f { float x, y, z; } t_vec3f;
typedef t_vec3f t_aabb[2];

typedef struct s_bvhnode {
	t_aabb  bounds;
	int     start, count;
	int     left, right, parent;
} t_bvhnode;

/* === Couleurs & UTF-8 === */
#define COLOR_RESET   "\033[0m"
#define COLOR_NODE    "\033[1;36m"
#define COLOR_LEAF    "\033[1;32m"
#define COLOR_TEXT    "\033[1;37m"
#define COLOR_BAR     "\033[1;36m"

#define U_VLINE     "\xE2\x94\x82"  /* │ */
#define U_HLINE     "\xE2\x94\x80"  /* ─ */
#define U_TJUNC     "\xE2\x94\xB4"  /* ┴ */
#define U_CORNER_L  "\xE2\x94\x8C"  /* ┌ */
#define U_CORNER_R  "\xE2\x94\x90"  /* ┐ */
#define U_CORNER_BL "\xE2\x94\x94"  /* └ */
#define U_CORNER_BR "\xE2\x94\x98"  /* ┘ */
#define U_VLINE_DASH "\xE2\x94\x86"  /* ┆*/

#define COLOR_CHILD_L "\033[38;5;214m"  /* orange */
#define COLOR_CHILD_R "\033[1;32m"      /* vert   */

#define BLOCK_W 11
#define HALF_W  (BLOCK_W/2)

/* === Utils simples === */
static int is_leaf(const t_bvhnode *n){
	if (!n) return 0;
	if (n->left < 0 && n->right < 0) return 1;
	if (n->count > 0) return 1;
	return 0;
}

static int visual_length(const char *s){
	int len=0;
	for (int i=0; s[i];){
		if (s[i]=='\033' && s[i+1]=='['){
			i+=2; while (s[i] && s[i]!='m') i++; if (s[i]=='m') i++;
		} else { len++; i++; }
	}
	return len;
}

static void print_until(int *cursor, int target){
	while (*cursor < target){ putchar(' '); (*cursor)++; }
}
static void print_str_advance(const char *s, int *cursor){
	fputs(s, stdout); *cursor += visual_length(s);
}

/* === Centres équirépartis pour un niveau (n éléments) ===
centres[i] = round( (i+1) * term_w / (n+1) ) */
// static void equispaced_centers(int n, int term_w, int *centers){
// 	for (int i=0;i<n;i++){
// 		double c = (double)(i+1) * (double)term_w / (double)(n+1);
// 		centers[i] = (int)lround(c);
// 	}
// }

/* === capacité max d’un niveau sans recouvrement en placement équiréparti ===
condition: spacing >= BLOCK_W  =>  term_w/(n+1) >= BLOCK_W  =>
n <= term_w/BLOCK_W - 1                                         */
static int max_equispaced_nodes_that_fit(int term_w){
	int m = term_w / BLOCK_W - 1;
	return (m < 1) ? 1 : m;
}

/* === format 4 caractères (k/M) === */
static void fmt4_block(char out[5], float v){
	int neg = (v < 0.0f); float a = neg ? -v : v;
	if (a < 1000.0f){ int n=(int)lroundf(a); char b[8];
		snprintf(b,sizeof(b),"%s%3d",neg?"-":" ",n);
		if (n==0 && neg) snprintf(b,sizeof(b),"   0");
		memcpy(out,b,4); out[4]='\0'; return; }
	if (a < 10000.0f){ int k=(int)(a/1000.0f); if (k>99) k=99;
		int d=(int)lroundf((a-k*1000.0f)/100.0f); if (d>9) d=9;
		out[0]=neg?'-':(k>=10? (char)('0'+(k/10)%10):' ');
		out[1]=(char)('0'+(k%10)); out[2]='k'; out[3]=(char)('0'+d); out[4]='\0'; return; }
	if (a < 1000000.0f){ int k=(int)lroundf(a/1000.0f); if (k>999) k=999; char b[8];
		if (neg) snprintf(b,sizeof(b),"-%2dk",k); else snprintf(b,sizeof(b),"%3dk",k);
		memcpy(out,b,4); out[4]='\0'; return; }
	if (a < 10000000.0f){ int M=(int)(a/1000000.0f); if (M>99) M=99;
		int d=(int)lroundf((a-M*1000000.0f)/100000.0f); if (d>9) d=9;
		out[0]=neg?'-':(M>=10?(char)('0'+(M/10)%10):' ');
		out[1]=(char)('0'+(M%10)); out[2]='M'; out[3]=(char)('0'+d); out[4]='\0'; return; }
	if (a < 1000000000.0f){ int M=(int)lroundf(a/1000000.0f); if (M>999) M=999; char b[8];
		if (neg) snprintf(b,sizeof(b),"-%2dM",M); else snprintf(b,sizeof(b),"%3dM",M);
		memcpy(out,b,4); out[4]='\0'; return; }
	memcpy(out,"inf ",4); out[4]='\0';
}

/* === assemblage d’un bloc 11×5 === */
typedef struct {
    int   idx, has_left, has_right;
    int   left_idx, right_idx;          /* NEW */
    int   start_x, center_x;
    char  l0[64], l1[64], l2[64], l3[64], l4[64], l5[64];  /* NEW l5 */
} RNode;

static void fmt4_child(char out[5], int idx){
    if (idx < 0) { memcpy(out, "    ", 4); out[4]='\0'; return; }
    char tmp[16]; snprintf(tmp, sizeof(tmp), "#%d", idx);
    int len = (int)strlen(tmp);
    if (len >= 4) { memcpy(out, tmp + (len-4), 4); out[4]='\0'; return; }
    /* right-align in width 4 */
    int pad = 4 - len;
    for (int i=0; i<pad; ++i) out[i] = ' ';
    memcpy(out+pad, tmp, len);
    out[4]='\0';
}


static void build_block_11(RNode *rn, const t_bvhnode *nd, int index){
	char xmin[5],xmax[5],ymin[5],ymax[5],zmin[5],zmax[5];
	fmt4_block(xmin, nd->bounds[0].x); fmt4_block(xmax, nd->bounds[1].x);
	fmt4_block(ymin, nd->bounds[0].y); fmt4_block(ymax, nd->bounds[1].y);
	fmt4_block(zmin, nd->bounds[0].z); fmt4_block(zmax, nd->bounds[1].z);

	const char *LB = COLOR_BAR U_VLINE COLOR_TEXT;
	const char *RB = COLOR_BAR U_VLINE COLOR_RESET;

	snprintf(rn->l1, sizeof(rn->l1), "%s%s %s%s", LB, xmin, xmax, RB);
	snprintf(rn->l2, sizeof(rn->l2), "%s%s %s%s", LB, ymin, ymax, RB);
	snprintf(rn->l3, sizeof(rn->l3), "%s%s %s%s", LB, zmin, zmax, RB);

	const char *col = is_leaf(nd) ? COLOR_LEAF : COLOR_NODE;
	char idbuf[24]; snprintf(idbuf,sizeof(idbuf),"[#%d]",index);
	int idw=(int)strlen(idbuf);
	int freew = BLOCK_W - 2 - idw; if (freew < 0) freew = 0;
	int left_h = freew/2, right_h = freew - left_h;

	int off=0;
	off += snprintf(rn->l0+off,sizeof(rn->l0)-off,"%s%s",col,U_CORNER_L);
	for(int i=0;i<left_h;i++) off += snprintf(rn->l0+off,sizeof(rn->l0)-off,"%s",U_HLINE);
	off += snprintf(rn->l0+off,sizeof(rn->l0)-off,"%s",idbuf);
	for(int i=0;i<right_h;i++) off += snprintf(rn->l0+off,sizeof(rn->l0)-off,"%s",U_HLINE);
	off += snprintf(rn->l0+off,sizeof(rn->l0)-off,"%s%s",U_CORNER_R,COLOR_RESET);

	off=0;
	off += snprintf(rn->l4+off,sizeof(rn->l4)-off,"%s%s",col,U_CORNER_BL);
	for(int i=0;i<BLOCK_W-2;i++) off += snprintf(rn->l4+off,sizeof(rn->l4)-off,"%s",U_HLINE);
	off += snprintf(rn->l4+off,sizeof(rn->l4)-off,"%s%s",U_CORNER_BR,COLOR_RESET);
}


/* centres équirépartis sur la largeur du terminal */
static void centers_for_width(int n, int term_w, int *centers)
{
	for (int i = 0; i < n; ++i) {
		double c = (double)(i + 1) * (double)term_w / (double)(n + 1);
		centers[i] = (int)lround(c);
	}
}

/* Prépare la “géométrie” d’un niveau : start_x/center_x/flags, aucun I/O */
static RNode *layout_level(
	const t_bvhnode *nodes,
	const int *cur_idx, int n_cur,
	int term_w, const int *centers /* n_cur */,
	int *out_n_cur /* optionnel, peut être NULL */)
{
	RNode *level = (RNode*)calloc(n_cur, sizeof(RNode));
	if (!level) return NULL;

	for (int i = 0; i < n_cur; ++i) {
		const t_bvhnode *nd = &nodes[cur_idx[i]];
		RNode *rn = &level[i];

		rn->idx = cur_idx[i];
		rn->has_left  = (nd->left  >= 0);
		rn->has_right = (nd->right >= 0);

		build_block_11(rn, nd, cur_idx[i]);

		int start = centers[i] - HALF_W;
		if (start < 0) start = 0;
		if (start > term_w - BLOCK_W) start = term_w - BLOCK_W;
		rn->start_x  = start;
		rn->center_x = start + HALF_W;
		
		char L[5], R[5];
		rn->left_idx  = nd->left;   // <-- manquait
		rn->right_idx = nd->right;  // <-- manquait

		fmt4_child(L, rn->left_idx);
		fmt4_child(R, rn->right_idx);
		/* | #### #### |  -> 1 + 4 + 1 + 4 + 1 = 11 colonnes */
		snprintf(rn->l5, sizeof(rn->l5),
				COLOR_BAR U_VLINE COLOR_RESET
				COLOR_CHILD_L "%s" COLOR_RESET " "
				COLOR_CHILD_R "%s" COLOR_RESET
				COLOR_BAR U_VLINE COLOR_RESET,
				L, R);
	}

	if (out_n_cur) *out_n_cur = n_cur;
	return level;
}

/* Collecte des enfants du niveau courant (ordre gauche puis droite) */
static void collect_children(
	const t_bvhnode *nodes, const RNode *level, int n_cur,
	int **out_next_idx, int *out_n_next)
{
	int cap = n_cur * 2;
	int *next = cap ? (int*)malloc(sizeof(int)*cap) : NULL;
	int nn = 0;

	for (int i = 0; i < n_cur; ++i) {
		const t_bvhnode *nd = &nodes[level[i].idx];
		if (nd->left  >= 0) next[nn++] = nd->left;
		if (nd->right >= 0) next[nn++] = nd->right;
	}
	*out_next_idx = next;
	*out_n_next   = nn;
}

/* Imprime les 5 lignes de blocs d’un niveau déjà “layouté” */
static void draw_level_blocks(const RNode *level, int n_cur)
{
	int cursor;

	/* Ligne 0 */
	cursor = 0;
	for (int i = 0; i < n_cur; i++) {
		print_until(&cursor, level[i].start_x);
		fputs(level[i].l0, stdout);
		cursor += BLOCK_W;
	}
	putchar('\n');
	/* Ligne 1 */
	cursor = 0;
	for (int i = 0; i < n_cur; i++) {
		print_until(&cursor, level[i].start_x);
		fputs(level[i].l1, stdout);
		cursor += BLOCK_W;
	}
	putchar('\n');

	/* Ligne 2 */
	cursor = 0;
	for (int i = 0; i < n_cur; i++) {
		print_until(&cursor, level[i].start_x);
		fputs(level[i].l2, stdout);
		cursor += BLOCK_W;
	}
	putchar('\n');

	/* Ligne 3 */
	cursor = 0;
	for (int i = 0; i < n_cur; i++) {
		print_until(&cursor, level[i].start_x);
		fputs(level[i].l3, stdout);
		cursor += BLOCK_W;
	}
	putchar('\n');

	/* Ligne 5 (enfants) */
	cursor = 0;
	for (int i = 0; i < n_cur; i++) {
		print_until(&cursor, level[i].start_x);
		fputs(level[i].l5, stdout);
		cursor += BLOCK_W;
	}
	putchar('\n');

	/* Ligne 4 */
	cursor = 0;
	for (int i = 0; i < n_cur; i++) {
		print_until(&cursor, level[i].start_x);
		fputs(level[i].l4, stdout);
		cursor += BLOCK_W;
	}
	putchar('\n');



}


/* Imprime la ligne verticale “│” sous les parents qui ont des enfants */
static void draw_level_stems(const RNode *level, int n_cur)
{
	int cursor = 0;
	for (int i=0;i<n_cur;i++){
		if (!(level[i].has_left || level[i].has_right)) continue;
		print_until(&cursor, level[i].center_x);
		fputs(U_VLINE, stdout);
		cursor++;
	}
	putchar('\n');
}

/* helpers pour tracer des segments en largeur fixe (1 col / glyphe) */
static inline void put1(const char *g, int *cursor) {
    fputs(g, stdout);
    (*cursor)++;                 /* chaque glyphe de box-drawing = 1 colonne */
}
static void draw_hspan(int from_x, int to_x, int *cursor) {
    /* trace des ─ de from_x (inclus) à to_x (exclu) */
    for (int x = from_x; x < to_x; ++x) {
        print_until(cursor, x);
        put1(U_HLINE, cursor);
    }
}

/* Imprime les barres horizontales entre parents et enfants (centers enfants fournis) */
static void draw_connectors(
    const t_bvhnode *nodes, const RNode *level, int n_cur,
    const int *next_centers, int n_next)
{
    int cursor = 0;
    int k = 0; /* index dans next_centers */

    for (int i = 0; i < n_cur; ++i) {
        const t_bvhnode *nd = &nodes[level[i].idx];
        const int px   = level[i].center_x;
        const int hasL = (nd->left  >= 0);
        const int hasR = (nd->right >= 0);

        int lx = -1, rx = -1;
        if (hasL && k < n_next) lx = next_centers[k++];
        if (hasR && k < n_next) rx = next_centers[k++];

        if (hasL && hasR) {
            /* ┌───…───┴───…───┐ */
            print_until(&cursor, lx);  put1(U_CORNER_L, &cursor);
            draw_hspan(lx + 1, px, &cursor);
            print_until(&cursor, px);  put1(U_TJUNC, &cursor);
            draw_hspan(px + 1, rx, &cursor);
            print_until(&cursor, rx);  put1(U_CORNER_R, &cursor);

        } else if (hasL) {
            /* ┌───…───┴ */
            print_until(&cursor, lx);  put1(U_CORNER_L, &cursor);
            draw_hspan(lx + 1, px, &cursor);
            print_until(&cursor, px);  put1(U_TJUNC, &cursor);

        } else if (hasR) {
            /* ┴───…───┐ */
            print_until(&cursor, px);  put1(U_TJUNC, &cursor);
            draw_hspan(px + 1, rx, &cursor);
            print_until(&cursor, rx);  put1(U_CORNER_R, &cursor);
        }
        /* si aucun enfant: rien à tracer */
    }
    putchar('\n');
}



/* === impression d’un niveau + calcul du prochain (layout séparé du draw) === */
static int render_level(
    const t_bvhnode *nodes,
    const int *cur_idx, int n_cur,
    int term_w,
    const int *centers_in,                 /* NEW: centres imposés pour CE niveau (peut être NULL) */
    int **out_next_idx, int *out_n_next,
    int **out_next_centers)
{
	if (n_cur <= 0) { *out_next_idx=NULL; *out_n_next=0; *out_next_centers=NULL; return 0; }

	/* 1) centres du niveau courant */
	int *centers = (int*)malloc(sizeof(int)*n_cur);
	if (!centers){ *out_next_idx=NULL; *out_n_next=0; *out_next_centers=NULL; return 0; }
	if (centers_in) {
		memcpy(centers, centers_in, sizeof(int)*n_cur);
	} else {
		centers_for_width(n_cur, term_w, centers);
	}

	/* 2) layout du niveau (aucun print) */
	RNode *level = layout_level(nodes, cur_idx, n_cur, term_w, centers, NULL);
	free(centers);
	if (!level){ *out_next_idx=NULL; *out_n_next=0; *out_next_centers=NULL; return 0; }

	/* 3) draw des blocs puis tiges verticales */
	draw_level_blocks(level, n_cur);
	draw_level_stems(level, n_cur);

	/* 4) collecte enfants + centres du prochain niveau */
	int *next_idx = NULL, n_next = 0;
	collect_children(nodes, level, n_cur, &next_idx, &n_next);

	int *next_centers = NULL;
	if (n_next > 0) {
		next_centers = (int*)malloc(sizeof(int) * n_next);
		if (!next_centers) { free(level); free(next_idx);
			*out_next_idx=NULL; *out_n_next=0; *out_next_centers=NULL; return 0; }

		int k = 0; /* remplit dans l'ordre compact: left puis right si présents */
		for (int i = 0; i < n_cur; ++i) {
			const int hasL = level[i].has_left;
			const int hasR = level[i].has_right;
			if (!(hasL || hasR)) continue;

			/* territoire horizontal du parent i : bornes entre milieux avec voisins */
			int lb = (i == 0)
				? 0
				: (level[i-1].center_x + level[i].center_x) / 2;
			int rb = (i == n_cur - 1)
				? (term_w - 1)
				: (level[i].center_x + level[i+1].center_x) / 2;
			if (rb < lb) { int t = lb; lb = rb; rb = t; }

			const int px = level[i].center_x;

			/* borne globale pour un bloc entier dans [lb, rb] */
			int g_min = lb;
			int g_max = rb - (BLOCK_W - 1);
			if (g_max < g_min) g_max = g_min;

			if (hasL) {
				/* moitié gauche : [lb .. px-1] → centre = milieu de cette moitié */
				int target = lb + (px - lb) / 2;
				int s = target - HALF_W;

				/* force "à gauche du parent" pour le bloc complet */
				int max_L = (px - 1) - HALF_W;      /* centre ≤ px-1 ⇒ start ≤ px-1-HALF_W */
				int min_L = g_min;
				if (max_L < min_L) max_L = min_L;   /* pas la place → on colle */

				if (s < min_L) s = min_L;
				if (s > max_L) s = max_L;

				/* clamp global de sécurité */
				if (s < 0) s = 0;
				if (s > term_w - BLOCK_W) s = term_w - BLOCK_W;

				next_centers[k++] = s + HALF_W;
			}

			if (hasR) {
				/* moitié droite : [px+1 .. rb] → centre = milieu de cette moitié */
				int target = px + (rb - px) / 2;
				int s = target - HALF_W;

				/* force "à droite du parent" pour le bloc complet */
				int min_R = (px + 1) - HALF_W;      /* centre ≥ px+1 ⇒ start ≥ px+1-HALF_W */
				int max_R = g_max;
				if (min_R > max_R) min_R = max_R;   /* pas la place → on colle */

				if (s < min_R) s = min_R;
				if (s > max_R) s = max_R;

				/* clamp global de sécurité */
				if (s < 0) s = 0;
				if (s > term_w - BLOCK_W) s = term_w - BLOCK_W;

				next_centers[k++] = s + HALF_W;
			}
		}
}



	/* 5) draw des connecteurs en utilisant next_centers (si présents) */
	if (next_centers) draw_connectors(nodes, level, n_cur, next_centers, n_next);
	else putchar('\n');

	free(level);

	*out_next_idx     = next_idx;
	*out_n_next       = n_next;
	*out_next_centers = next_centers;
	return n_next;
}

/* Imprime des IDs exactement aux centres donnés (sans recalc global) */
static void print_ids_at_centers(const int *ids, const int *centers, int n, int term_w)
{
    int cursor = 0;
    int prev_end = -1;

    for (int i = 0; i < n; ++i) {
        char out[32];
        snprintf(out, sizeof(out), COLOR_NODE "[#%d]" COLOR_RESET, ids[i]);
        int w = visual_length(out);

        /* start centré sur centers[i] */
        int start = centers[i] - w / 2;

        /* éviter chevauchement avec le précédent */
        if (start < prev_end + 1) start = prev_end + 1;

        /* clamp global */
        if (start < 0) start = 0;
        if (start > term_w - w) start = term_w - w;

        print_until(&cursor, start);
        fputs(out, stdout);
        prev_end = start + w - 1;
        cursor   = prev_end + 1;
    }
    putchar('\n');
}

/* === rendu récursif avec split (équiréparti propre) === */
static void render_tree_split(const t_bvhnode *nodes, int root_idx, int term_w, int dashed)
{
	/* si on est dans un sous-arbre (dashed!=0), poser un repère ┆ centré */
	if (dashed) {

		//afficher une ligne continue de U-HLINE
		for (int j = 0; j < term_w; j++) {
			fputs(U_HLINE, stdout);
		}
		putchar('\n');

		int cursor = 0;
		int cx = term_w / 2 + 1;          /* le root d'un sous-arbre est centré */
		print_until(&cursor, cx);
		fputs(COLOR_BAR, stdout);
		fputs(U_VLINE_DASH, stdout);
		putchar('\n');
		cursor = 0;
		print_until(&cursor, cx);
		fputs(U_VLINE_DASH, stdout);
		putchar('\n');
		fputs(COLOR_RESET, stdout);
	}

	/* Niveau courant (on part de la racine) */
	int *cur = malloc(sizeof(int));
	if (!cur) return;
	cur[0] = root_idx;
	int n_cur = 1;

	int *centers_in = NULL;  /* NEW: centres imposés pour le niveau courant */

	int n_slots = 1;
	while (n_cur > 0)
	{
		int *next = NULL;
		int *next_centers = NULL;
		int n_next = 0;

		render_level(nodes, cur, n_cur, term_w,
					centers_in,                 /* NEW */
					&next, &n_next, &next_centers);

		free(cur);        cur = NULL;
		free(centers_in); centers_in = NULL;     /* NEW: ceux qu'on vient d'utiliser */

		if (n_next == 0) { free(next_centers); free(next); break; }

		int nmax = max_equispaced_nodes_that_fit(term_w);

		n_slots *= 2;
		if (n_slots <= nmax)               /* ✅ on teste la CAPACITÉ, pas le réel */
		{
			cur = next; n_cur = n_next;
			centers_in = next_centers;     /* on impose ces centres au niveau suivant */
			continue;
		}
		/* split visuel */
		print_ids_at_centers(next, next_centers, n_next, term_w);
		putchar('\n');
		for (int i = 0; i < n_next; i++) {
			render_tree_split(nodes, next[i], term_w, 1);
			putchar('\n');
		}
		free(next_centers);
		free(next);
		break;
	}
	free(centers_in); /* sécurité */
	free(cur);

}


/* === API === */
void bvh_print_tree(const t_bvhnode *nodes, int root_index, int node_count){
	(void)node_count;
	if (!nodes || root_index < 0){
		printf("BVH invalide.\n"); return;
	}
	struct winsize w; ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int term_w = (w.ws_col > 0 ? w.ws_col : 120);
	render_tree_split(nodes, root_index, term_w, 0);
}
