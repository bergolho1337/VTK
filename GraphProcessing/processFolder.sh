#!/bin/bash

if [ ! -f build/GraphProcessing ]; then
    echo "[!] Building GraphProcessing ..."
    cd build
    cmake ..
    make
    cd ..
fi

echo "[!] Processing files in the folder VTP-Files ..."
cd VTP-Files
FILES=($(ls -t | sort))
cd ..
for FILE in ${FILES[@]}; do
    build/GraphProcessing VTP-Files/$FILE Output-Files/$FILE  
done
echo "[+] Done"