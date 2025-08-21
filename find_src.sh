#!/usr/bin/env bash


set -euo pipefail





usage() {


    cat <<EOF 1>&2


Usage: $(basename "$0") [SRC_PATH] [FORMAT] [VAR] [MAKEFILE]


VAR defaults to SRC


FORMAT defaults to *.c


MAKEFILE defaults to Makefile in current directory


This script finds all files in [SRC_PATH] matching [FORMAT]


and puts them in [VAR] in [MAKEFILE].


EOF


}





if (( $# == 0 )); then


    usage


    exit 1


fi





SRC_PATH="$1"


FORMAT="${2:-*.c}"


VAR="${3:-SRC}"


MAKEFILE="${4:-Makefile}"





# Remove trailing slash from SRC_PATH if present


SRC_PATH="${SRC_PATH%/}"





# Use GNU find's -printf to get relative paths, safer than sed


SRC=$(find "$SRC_PATH" -type f -name "$FORMAT" -printf '%P\n' | tr '\n' ' ')





# Escape VAR for use in sed (escape regex special chars)


escaped_VAR=$(printf '%s\n' "$VAR" | sed 's/[][\.*^$(){}?+|/]/\\&/g')





# Escape replacement string for sed (& and \)


escaped_SRC=$(printf '%s\n' "$SRC" | sed -e 's/[&/\]/\\&/g')





# Replace the first occurrence of VAR assignment line in MAKEFILE


sed -i "0,/^\s*${escaped_VAR}\s*=/s|^\(\s*${escaped_VAR}\s*=\s*\).*|\1${escaped_SRC}|" "$MAKEFILE"

