#! /bin/bash
FILENAME="$1"
shift
# KEY="$2"
# echo "${FILENAME%.*}_${KEY}.txt"

for KEY in "$@"; do
    grep $KEY < $FILENAME | awk 'NF>1{print $NF}' > "${FILENAME%.*}_${KEY}.txt"
done

