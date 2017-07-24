#!/bin/bash

if [ ! -f build/Converter ]; then
    echo "[!] Building VTK_To_VTP ..."
    cd build
    cmake ..
    make
fi

echo "[!] Converting files in the folder VTK-Files to VTP ..."
cd VTK-Files
FILES=($(find *.vtk))
cd ..
for FILE in ${FILES[@]}; do
    SIZE=${#FILE}
    OUT_FILE=$(echo "${FILE:0:SIZE-1}p")
    build/Converter VTK-Files/$FILE VTP-Files/$OUT_FILE  
done
echo "[+] Done"