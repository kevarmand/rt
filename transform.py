import trimesh
import numpy as np

# -----------------------------
# CONFIGURATION
# -----------------------------
obj_file = "model.obj"       # input OBJ
output_file = "scene.txt"    # output scene file

resolution = (1500, 1100)    # R width height
ambient_intensity = 0.1
ambient_color = (255, 255, 255)

# Default light setup
lights = [
    {"pos": (700, 900, 600), "intensity": 0.412456, "color": (255,255,255)},
    {"pos": (400, 200, 0), "intensity": 0.412456, "color": (255,0,255)},
    {"pos": (200, 500, 300), "intensity": 0.412456, "color": (0,255,255)},
    {"pos": (1000, 300, 400), "intensity": 0.412456, "color": (255,255,0)},
]

# Default triangle color if OBJ has no materials
default_color = (255, 127, 0)

# -----------------------------
# LOAD OBJ
# -----------------------------
mesh = trimesh.load(obj_file, process=False)

# Flatten multiple meshes if needed
if isinstance(mesh, trimesh.Scene):
    mesh = trimesh.util.concatenate([g for g in mesh.geometry.values()])

# -----------------------------
# COMPUTE CAMERA
# -----------------------------
# Compute bounding box
bbox_min, bbox_max = mesh.bounds
center = (bbox_min + bbox_max) / 2
size = bbox_max - bbox_min
max_dim = np.max(size)

# Place camera along +Z axis in front of the object, centered
# Assume FOV is 60 degrees (vertical)
fov = 60
fov_rad = np.radians(fov)
# distance so that object fits in view
distance = max_dim / (2 * np.tan(fov_rad / 2))
camera_pos = center + np.array([0, 0, distance])
camera_dir = center - camera_pos
camera_dir /= np.linalg.norm(camera_dir)

# -----------------------------
# WRITE SCENE FILE
# -----------------------------
with open(output_file, "w") as f:
    # Resolution
    f.write(f"R\t{resolution[0]} {resolution[1]}\n")

    # Ambient light
    f.write(f"A\t{ambient_intensity}\t{','.join(map(str, ambient_color))}\n")

    # Camera
    f.write(f"c\t{','.join(f'{v:.6f}' for v in camera_pos)}\t"
            f"{','.join(f'{v:.6f}' for v in camera_dir)}\t{fov}\n")

    # Lights
    for light in lights:
        pos_str = ",".join(str(x) for x in light["pos"])
        color_str = ",".join(str(x) for x in light["color"])
        f.write(f"l\t{pos_str}\t{light['intensity']}\t{color_str}\n")

    # Triangles
    for face in mesh.faces:
        v0, v1, v2 = mesh.vertices[face]
        v0_str = ",".join(f"{coord:.6f}" for coord in v0)
        v1_str = ",".join(f"{coord:.6f}" for coord in v1)
        v2_str = ",".join(f"{coord:.6f}" for coord in v2)
        color_str = ",".join(str(c) for c in default_color)
        f.write(f"tr\t{v0_str}\t{v1_str}\t{v2_str}\t{color_str}\n")
