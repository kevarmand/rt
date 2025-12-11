#!/bin/bash

# Couleur cible (blanc légèrement bleuté pour Nivia)
TARGET="200,220,255"

while IFS= read -r line; do
    # Si la ligne commence par tr ET finit par 255,127,0
    if [[ "$line" == tr* && "$line" == *"255,127,0" ]]; then
        # Remplacement de la couleur uniquement à la fin
        newline="${line%255,127,0}$TARGET"
        echo "$newline"
    else
        echo "$line"
    fi
done < "${1:-/dev/stdin}"