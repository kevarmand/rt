#ifndef SCENE_H
# define SCENE_H
/* ============================================================
 * scene_type.h — Représentation runtime immuable pour le rendu
 * (Aucun lien MLX/UI ici; parsing et build font la validation)
 * ============================================================ */


# include "vector.h"
# include <stdint.h>
/* Indice uniforme pour référencer les éléments de la scène. */
typedef int t_index;

typedef enum e_scene_id {
	SCENE_ID_NONE = -1
}	t_scene_id;

/* AABB utilitaire (min,max) — utilisé par les nœuds BVH et les objets. */
// typedef t_vec3f t_aabb[2];
typedef struct s_aabb
{
	t_vec3f	b[2];
}	t_aabb;

/* --------- Types de primitives géométriques --------- */
typedef enum e_primtype
{
	PRIM_TRIANGLE,
	PRIM_PLANE,
	PRIM_SPHERE,
	PRIM_CYLINDER,
	PRIM_TORUS,
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
	float	r_squared;
}	t_sphere;

typedef struct s_cylinder
{
	t_vec3f	base;       // point de départ de l’axe
	t_vec3f	axis;       // unitaire
	float	height;     // longueur
	float	radius;     // rayon
	float	r_squared;  // rayon^2
}	t_cylinder;

typedef struct s_torus
{
	t_vec3f center;
	t_vec3f normal; /* unitaire */
	float   R;      /* grand rayon (centre → tube) */
	float   r;      /* petit rayon (rayon du tube) */
	float	r_square;
	float	R_square;
}	t_torus;

/* --------- Plans “infini” (hors BVH) ---------
 * Motif fréquent: peu de plans par scène, non découpables proprement.
 * On les teste systématiquement après la traversée BVH.
 */
typedef struct s_plane
{
	t_vec3f normal; /* unitaire */
	float   d;      /* plan: dot(normal, X) + d = 0 */
}	t_plane;

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
		t_plane    pl;
	};
	t_index     material_id;  /* SCENE_ID_NONE si défaut global */
	t_index     surface_id;   /* Submesh/material slot si besoin, sinon NONE */
}	t_primitive;

typedef struct s_priminfo
{
	t_aabb			bounds;
	t_vec3f			centroid;
}	t_priminfo;

/* --------- Objets logiques (groupent des primitives contiguës) --------- */
typedef struct s_object
{
	const char *name;     /* optionnel, peut être NULL */
	t_index     first_prim;
	int         prim_count;
	t_aabb      bounds;   /* AABB agrégée de l’objet (min,max) */
}	t_object;



/* --------- Matériaux / Textures (simplifiés, extensibles) --------- */
typedef struct s_texture
{
	char	*path;     /* chemin normalisé; pixels résolus au build */
	int		width;
	int		height;
	int		channels;
	void	*pixels;   /* peut rester NULL selon backend */
}	t_texture;

typedef struct s_surface
{
	//pas d index ixi je veux le mapping UV, la couleur de base, etc
	float		map_uv[6]; /* u0,v0,u1,v1,u2,v2 OU px py pz, rx ry rz*/
	int			uv_mod;
	float		scale_u;
	float		scale_v;
	t_vec3f		color;    /* 0..1 linéaire */
	t_vec3f		normal;   /* unitaire */
	// float		w2o[16];  /* world to object matrix */
	// float		o2w[16];  /* object to world matrix */
}	t_surface;

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
	float   intensity;
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
	float   fov_deg;   /* horizontal */
}	t_camera;

/* --------- BVH (runtime) ---------
 * Nœuds immuables; feuilles pointent sur une plage contiguë de primitives.
 */
typedef struct s_bvhnode
{
	t_aabb	bounds;
	uint8_t	is_leaf;
	union
	{
		struct s_node
		{
			uint32_t	left;
			uint32_t	right;
		}	node;
		struct s_leaf
		{
			uint32_t	start;
			uint32_t	count;
		}	leaf;
	};
}	t_bvhnode;


typedef struct s_skybox
{
	t_index		texture_id; /* SCENE_ID_NONE si pas de skybox */
	int			mode;       /* e_skybox_mode */
	t_vec3f		intensity; /* multiplicateur de couleur */
}	t_skybox;


/* --------- Scène finale immuable --------- */
typedef struct s_scene
{
	/* Globaux */
	int     resolution_width;
	int     resolution_height;
	t_vec3f ambient_color;     /* 0..1 linéaire */

	/* Skybox */
	t_skybox	skybox;

	/* Primitives & Objets (BVH) */
	t_primitive *primitives;
	int          primitive_count;

	t_bvhnode	*bvh_nodes;
	int          bvh_node_count;
	t_index      bvh_root_id; /* SCENE_ID_NONE si vide */

	/* Plans infinis (hors BVH) */
	t_primitive     *planes;
	int          plane_count;

	/* Matériaux / Textures */
	t_material  *materials;
	int          material_count;

	/* Surfaces */
	t_surface   *surfaces;
	int          surface_count;

	t_texture   *textures;
	int          texture_count;

	/* Lumières */
	t_light     *lights;
	int          light_count;

	/* Caméras */
	t_camera    *cameras;
	int          camera_count;

}	t_scene;

/* --------- Notes de design ---------
 * - Les préconditions (normales unitaires, FOV valides, etc.) sont garanties
 *   par le parse/init; on ne les re-teste pas ici (pas de checks runtime).
 * - Les AABB/centroids par primitive n’existent que côté builder (structures
 *   temporaires) pour la construction/réordonnancement BVH. Pas dans t_primitive.
 * - Les plans sont exclus du BVH (typique: peu nombreux, indécoupables).
 */

#endif
