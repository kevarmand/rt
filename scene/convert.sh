#!/bin/sh
set -eu

# Texture herbe existante (ambientCG, Grass001, licence CC0).
# Source couleur 2K -> XPM 2K avec 4096 couleurs.
mkdir -p texture
zip_file="$(mktemp /tmp/grass001.XXXXXX.zip)"
jpg_file="$(mktemp /tmp/grass001.XXXXXX.jpg)"
trap 'rm -f "$zip_file" "$jpg_file"' EXIT HUP INT TERM
curl -fL 'https://ambientcg.com/get?file=Grass001_2K-JPG.zip' -o "$zip_file"
unzip -p "$zip_file" 'Grass001_2K-JPG_Color.jpg' > "$jpg_file"
magick "$jpg_file" -colors 4096 'XPM:texture/meadow_grass_photo.xpm'
printf '%s\n' 'Texture creee : texture/meadow_grass_photo.xpm'
