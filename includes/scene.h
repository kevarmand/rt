#ifndef SCENE_H
# define SCENE_H
/* ============================================================
 * scene_type.h — Représentation runtime immuable pour le rendu
 * (Aucun lien MLX/UI ici; parsing et build font la validation)
 * ============================================================ */

/* Indice uniforme pour référencer les éléments de la scène. */
typedef int t_index;

typedef enum e_scene_id {
	SCENE_ID_NONE = -1
}	t_scene_id;

/* --------- Vecteurs de base ---------
 * t_vec3f = base mathématique float32
 * (Si tu as déjà un vec3 centralisé, garde ce typedef identique)
 */
typedef struct s_vec3f {
	float x;
	float y;
	float z;
}	t_vec3f;

/* AABB utilitaire (min,max) — utilisé par les nœuds BVH et les objets. */
typedef t_vec3f t_aabb[2];

/* --------- Types de primitives géométriques --------- */
typedef enum e_primtype
{
	PRIM_TRIANGLE = 0,
	PRIM_SPHERE   = 1,
	PRIM_CYLINDER = 2,
	PRIM_TORUS    = 3
	/* PRIM_PLANE non listé ici: plans traités hors BVH. */
}	t_primtype;

/* Triangle sous forme “Möller-ready”:
 * v0, edge1 = v1 - v0, edge2 = v2 - v0
 */
typedef struct s_triangle
{
	t_vec3f v0;
	t_vec3f edge1;
	t_vec3f edge2;
}	t_triangle;

typedef struct s_sphere
{
	t_vec3f center;
	float   radius;
}	t_sphere;

typedef struct s_cylinder
{
	t_vec3f p0;     /* base de l’axe */
	t_vec3f p1;     /* sommet de l’axe */
	float   radius;
}	t_cylinder;

typedef struct s_torus
{
	t_vec3f center;
	float   R;      /* grand rayon (centre → tube) */
	float   r;      /* petit rayon (rayon du tube) */
}	t_torus;

/* Primitive finale (runtime): compacte, sans AABB/centroid.
 * Les AABB/centroids existent côté builder (structures “info”) pour le BVH,
 * pas ici. Intersection ne nécessite pas ces champs.
 */
typedef struct s_primitive
{
	t_primtype type;
	union {
		t_triangle tr;
		t_sphere   sp;
		t_cylinder cy;
		t_torus    to;
	} geo;
	t_index     object_id;    /* SCENE_ID_NONE si non applicable */
	t_index     material_id;  /* SCENE_ID_NONE si défaut global */
	t_index     surface_id;   /* Submesh/material slot si besoin, sinon NONE */
}	t_primitive;

/* --------- Objets logiques (groupent des primitives contiguës) --------- */
typedef struct s_object
{
	const char *name;     /* optionnel, peut être NULL */
	t_index     first_prim;
	int         prim_count;
	t_aabb      bounds;   /* AABB agrégée de l’objet (min,max) */
}	t_object;

/* --------- Plans “infini” (hors BVH) ---------
 * Motif fréquent: peu de plans par scène, non découpables proprement.
 * On les teste systématiquement après la traversée BVH.
 */
typedef struct s_plane
{
	t_vec3f normal; /* unitaire */
	float   d;      /* plan: dot(normal, X) + d = 0 */
}	t_plane;

/* --------- Matériaux / Textures (simplifiés, extensibles) --------- */
typedef struct s_texture
{
	const char *path;     /* chemin normalisé; pixels résolus au build */
	int         width;
	int         height;
	int         channels;
	void       *pixels;   /* peut rester NULL selon backend */
}	t_texture;

typedef struct s_material
{
	float ambient;     /* ka */
	float diffuse;     /* kd */
	float specular;    /* ks */
	float shininess;   /* n */
	float reflection;  /* [0..1] */
	float refraction;  /* [0..1] */
	float ior;         /* indice optique */
	t_index texture_albedo_id; /* SCENE_ID_NONE si absent */
	t_index texture_normal_id; /* SCENE_ID_NONE si absent */
}	t_material;

/* --------- Lumières (point pour l’instant) --------- */
typedef struct s_light
{
	t_vec3f position;
	t_vec3f color;     /* linéaire 0..1 */
	float   intensity; /* puissance relative */
}	t_light;

/* --------- Caméra prête pour ray(x,y) ---------
 * p0 = centre du pixel (0,0); dx,dy = incréments monde par pixel
 * Convention: dy va vers le BAS de l’écran.
 */
typedef struct s_camera
{
	t_vec3f origin;    /* O */
	t_vec3f forward;   /* F unitaire */
	t_vec3f right;     /* R unitaire */
	t_vec3f up;        /* U unitaire */
	t_vec3f p0;        /* centre du pixel (0,0) en monde */
	t_vec3f dx;        /* +1 px X (droite) */
	t_vec3f dy;        /* +1 px Y (bas) */
	float   fov_deg;   /* vertical */
	float   aspect;    /* width / height */
}	t_camera;

/* --------- BVH (runtime) ---------
 * Nœuds immuables; feuilles pointent sur une plage contiguë de primitives.
 */
typedef struct s_bvh_node
{
	t_aabb  bounds;        /* AABB du nœud (min,max) */
	int     is_leaf;       /* 0/1 */
	t_index left_child;    /* index dans nodes[], inutilisé si feuille */
	t_index right_child;   /* index dans nodes[], inutilisé si feuille */
	t_index first_prim;    /* utilisé si feuille */
	int     prim_count;    /* utilisé si feuille */
}	t_bvh_node;

/* --------- Sélection courante (indices; pas de pointeurs) --------- */
typedef struct s_select
{
	t_index active_camera_id;
	t_index hovered_object_id;
	t_index hovered_primitive_id;
	t_index hovered_light_id;
}	t_select;

/* --------- Scène finale immuable --------- */
typedef struct s_scene
{
	/* Globaux */
	int     resolution_width;
	int     resolution_height;
	t_vec3f ambient_color;     /* 0..1 linéaire */
	float   ambient_brightness;

	/* Primitives & Objets (BVH) */
	t_primitive *primitives;
	int          primitive_count;

	t_object    *objects;
	int          object_count;

	t_bvh_node  *bvh_nodes;
	int          bvh_node_count;
	t_index      bvh_root_id; /* SCENE_ID_NONE si vide */

	/* Plans infinis (hors BVH) */
	t_plane     *planes;
	int          plane_count;

	/* Matériaux / Textures */
	t_material  *materials;
	int          material_count;

	t_texture   *textures;
	int          texture_count;

	/* Lumières */
	t_light     *lights;
	int          light_count;

	/* Caméras */
	t_camera    *cameras;
	int          camera_count;

	/* Sélection (UI/inspection) */
	t_select     select_state;
}	t_scene;

/* --------- Notes de design ---------
 * - Les préconditions (normales unitaires, FOV valides, etc.) sont garanties
 *   par le parse/init; on ne les re-teste pas ici (pas de checks runtime).
 * - Les AABB/centroids par primitive n’existent que côté builder (structures
 *   temporaires) pour la construction/réordonnancement BVH. Pas dans t_primitive.
 * - Les plans sont exclus du BVH (typique: peu nombreux, indécoupables).
 */

#endif