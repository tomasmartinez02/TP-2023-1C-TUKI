#!/bin/bash

CWD=$PWD
COMMONS="so-commons-library"
echo "\n\t\t¡Dale que se aprueba SO!"
cd ./consola/
echo "\nCreando carpetas en: ${PWD}"
mkdir bin
mkdir obj
mkdir logs
cd ../cpu/
echo "\nCreando carpetas en: ${PWD}"
mkdir bin
mkdir obj
mkdir logs
cd ../kernel/
echo "\nCreando carpetas en: ${PWD}"
mkdir bin
mkdir obj
mkdir logs
cd ../memoria/
echo "\nCreando carpetas en: ${PWD}"
mkdir bin
mkdir obj
mkdir logs
cd ../filesystem/
echo "\nCreando carpetas en: ${PWD}"
mkdir bin
mkdir obj
cd ../static-utils/
echo "\nCreando carpetas en: ${PWD}"
mkdir bin
mkdir obj
cd $CD
echo "\nClonando commons..."
git clone "https://github.com/sisoputnfrba/${COMMONS}.git" $COMMONS
cd $COMMONS
echo "\nInstalando commons..."
sudo make install
clear
echo "\n\t\tQUE COMIENCE EL JUEGO\n"
