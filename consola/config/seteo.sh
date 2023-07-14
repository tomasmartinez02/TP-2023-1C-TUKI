#!/bin/bash

echo "\n\t\t¡Dale que se aprueba SO!\n"

read -p "Ingrese nueva IP_KERNEL: " IP_KERNEL

grep -RiIl 'IP_KERNEL' | xargs sed -i 's|\(IP_KERNEL\s*=\).*|\1'$IP_KERNEL'|'

echo "\nLos .config han sido modificados correctamente\n"