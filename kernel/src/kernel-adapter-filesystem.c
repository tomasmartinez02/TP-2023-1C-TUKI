#include <kernel-adapter-filesystem.h>


bool condicionBusqueda(void* elemento, void* elementoBuscado) {
    const char* elementoActual = (const char*)elemento;
    const char* elementoBuscadoStr = (const char*)elementoBuscado;
    return strcmp(elementoActual, elementoBuscadoStr) == 0;
}

bool archivo_esta_abierto(char *nombreArchivo)
{   
    return list_any_satisfy(tablaArchivosAbiertos, (void*)condicionBusqueda, nombreArchivo);
}