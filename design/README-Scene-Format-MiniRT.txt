README — MiniRT : format .rt, objets, shading & paramètres (V1)

But : décrire comment on parse les objets, comment on les “shade”, et quels paramètres sont supportés.
Objectif : rendu “waouh” simple = HDR + tone mapping + Lambert + Phong normalisé + réflexion/réfraction (Fresnel).

───────────────────────────────────────────────────────────────────────────────
1) Pipeline de rendu (ordre des opérations)
───────────────────────────────────────────────────────────────────────────────
- Tout est calculé en linéaire HDR (float).
- On accumule : Diffuse (Lambert) + Spéculaire (Phong normalisé) + Réflexion/Réfraction (Fresnel).
- Ensuite seulement : Exposure -> Tone mapping (ACES/Reinhard) -> Gamma (≈2.2).
- Pas de clamp avant le tone mapping. Profondeur de rebond bornée (max_bounce).

───────────────────────────────────────────────────────────────────────────────
2) Format du fichier .rt
───────────────────────────────────────────────────────────────────────────────
- Une entrée par ligne. Espaces libres. Les options sont sous forme `cle=valeur` après les champs obligatoires.
- Commentaires : tout ce qui suit `#` sur la ligne est ignoré.
- Couleurs : R,G,B en 0..255 sRGB (converties en linéaire au chargement).
- Vecteurs/points : `x,y,z` (floats).

2.1) Paramètres GLOBAUX (MVP)
  HDR=on|off           # def: on
  exposure=<float>     # def: 1.0
  tonemap=aces|reinhard|none  # def: aces
  gamma=2.2            # appliqué après tone mapping
  max_bounce=<int>     # def: 2 (profondeur réflections/réfractions)
  background=R,G,B     # def: 0,0,0 si pas d'envmap
  envmap=path.hdr      # optionnel

Exemple :
  HDR=on
  exposure=1.2
  tonemap=aces
  max_bounce=2
  background=0,0,0
  # envmap=assets/studio.hdr

2.2) Objets du SUJET (+ options V1)
- Sphère :
  sp  cx,cy,cz   radius   R,G,B   [options...]
- Plan :
  pl  px,py,pz   nx,ny,nz R,G,B   [options...]
- Cylindre :
  cy  cx,cy,cz   ax,ay,az diameter height  R,G,B  [options...]

Options V1 par objet (6 clés) :
  shin=<float>      # “shininess” Phong (taille du highlight). def: 64
  metal=0|1         # 1 => matériau métallique (pas de diffuse). def: 0
  ior=<float>       # indice réfraction (utile si trans>0 ; sert à F0 pour diélectriques). def: 1.0
  trans=<float>     # [0..1] fraction transmissive (verre/eau). def: 0.0
  F0=R,G,B          # override de la réflectance à incidence normale. Si absent: déduit de ior (diélectriques).
  tex=path.png      # (optionnel) texture albedo/base color (sRGB -> linéaire).

Option V1 “bump” (facultative) :
  bump=path.png         # height map (données non-color)
  bump_strength=<float> # def: 1.0

Remarques :
- Normal mapping (normal=...) : prévu en V2 (non requis pour V1).
- Wrap/repeat/clamp : non requis en V1 (repeat par défaut si besoin).

2.3) Caméra & lumières (sujet)
  A  ratio  R,G,B
  C  cx,cy,cz   nx,ny,nz   fov
  L  lx,ly,lz   brightness   R,G,B
(Interprétation standard ; en V1 on reste en “point lights”.)

───────────────────────────────────────────────────────────────────────────────
3) Shading : modèles utilisés et mélange d’énergie
───────────────────────────────────────────────────────────────────────────────
3.1) Diffuse (Lambert)
  L_d = (albedo / π) * Σ_lights [ max(0, N · L) * atténuation ]
- albedo vient de la couleur (et/ou tex). Couleurs converties en linéaire.

3.2) Spéculaire (Phong normalisé)
  L_p = k_s * ((s + 2) / (2π)) * max(0, R · V)^s
- s = shininess (plus grand => lobe plus étroit).
- k_s ≈ 1.0 en V1 (on peut l’exposer plus tard si besoin).
- On normalise pour conserver l’énergie quand s varie (important en HDR).

3.3) Fresnel (Schlick) et répartition
- Fresnel (Schlick) : F(θ) = F0 + (1 - F0) * (1 - cosθ)^5.
- F0 :
  - Diélectriques : si F0 non donné, on le calcule via ior : F0 ≈ ((n - 1) / (n + 1))^2 (≈ 0.02–0.08).
  - Métaux : F0 coloré donné par l’artiste (F0=R,G,B).
- Types de matériaux (mélanges simples, cohérents) :
  A) Diélectrique opaque (metal=0, trans=0) :
     C = (1 - F) * (L_d + L_p) + F * R
  B) Métal opaque (metal=1, trans=0) :
     (pas de diffuse)   C = (1 - F) * L_p(coloré) + F * R
  C) Transmissif (trans>0) :
     C = F * R + (1 - F) * T   # T via Snell, gère TIR
- R = contribution de la réflexion raytracée (rebond reflet), T = contribution réfractée.
- Profondeur bornée par max_bounce.

3.4) Réfraction (Snell) et TIR
- Utiliser l’ior pour calculer la direction réfractée.
- Gérer la réflexion totale interne (sinon NaN).

───────────────────────────────────────────────────────────────────────────────
4) Normales : sources et perturbations
───────────────────────────────────────────────────────────────────────────────
- Si le mesh/OBJ fournit des normales sommets : on interpole barycentriquement, puis normalize → N_shade.
- Construction du repère TBN au point (T, B, N_shade) via UV (orthonormalisation rapide).
- Bump mapping (si bump=...):
  * Échantillonner la height h(u,v) (non-color).
  * Approcher le gradient (différences finies) : h_u, h_v.
  * Dans l’espace tangent : Δ_t = (-s*h_u, -s*h_v, 0). N_t' = normalize( (0,0,1) + Δ_t ).
  * Revenir en monde : N' = normalize(T*N_t'.x + B*N_t'.y + N_shade*N_t'.z).
- Normal mapping (V2) :
  * Lire la normale tangent depuis la normal map (non-color), la normaliser, transformer via TBN.

Politique V1 si un jour normal ET bump existent :
  - Simple : normal map > bump (priorité à la normal map).
  - Avancé : combiner en tangent puis normaliser (option V2).

───────────────────────────────────────────────────────────────────────────────
5) Valeurs par défaut (V1)
───────────────────────────────────────────────────────────────────────────────
- shin = 64
- metal = 0
- ior = 1.0
- trans = 0.0
- F0 = (déduit de ior pour diélectriques ; requis/attendu pour métaux si rendu fidèle)
- tex = none
- bump = none ; bump_strength = 1.0
- Couleurs : 0..255 sRGB, converties en linéaire
- max_bounce = 2 ; HDR=on ; tonemap=aces ; exposure=1.0 ; gamma=2.2

───────────────────────────────────────────────────────────────────────────────
6) Exemples (V1)
───────────────────────────────────────────────────────────────────────────────
# Globaux
HDR=on
exposure=1.2
tonemap=aces
max_bounce=2
background=0,0,0
# envmap=assets/studio.hdr

# Caméra, lumière, ambiance (exemple standard du sujet)
C  0,1,6   0,0,-1   60
A  0.1  255,255,255
L  5,5,5  2.0  255,244,234

# 1) Plastique brillant (diélectrique opaque)
sp  0,0,6   2   200,80,60   shin=120   metal=0

# 2) Chrome (métal opaque, pas de diffuse)
sp  3,0,6   2   255,255,255  metal=1  shin=800  F0=230,230,230

# 3) Eau (transmissif + réfraction ; F0 déduit de ior)
sp -3,0,6   2   180,180,255  trans=1.0  ior=1.33  shin=200

# 4) Plan texturé + bump léger
pl  0,-2,0   0,1,0   255,255,255  tex=assets/tiles_albedo.png  bump=assets/tiles_height.png  bump_strength=0.7  shin=50

# 5) Cylindre métallique brossé (highlight large)
cy  0,0,0   0,1,0   1.2  4.0  180,180,180  metal=1  shin=20

───────────────────────────────────────────────────────────────────────────────
7) Roadmap V2 (idées futures, non requises V1)
───────────────────────────────────────────────────────────────────────────────
- normal=path.png (normal mapping, tangent space)
- wrap=repeat|clamp, uvscale, uvoffset, uvrot
- emissive=R,G,B (auto-illumination de surface)
- soft shadows (lights avec radius), DOF (aperture/focus)
- presets matériaux (mat=chrome/or/cuivre/verre…)
- Beer-Lambert pour l’absorption en transmission
- Réflexion glossy (roughness -> rayons cône/importance sampling)
- Modeles micro-facettes (GGX/Beckmann) en PBR léger

Fin du README V1.
