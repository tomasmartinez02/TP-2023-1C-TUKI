#include <filesystem-manejo-bloques.h>

void asignar_puntero_directo(t_fcb *fcbArchivo)
{
    uint32_t bloque = bitmap_encontrar_bloque_libre();
    fcbArchivo->PUNTERO_DIRECTO = bloque; // mapear el fcb y hacer un msync aca
    bitmap_marcar_bloque_ocupado(bloque);
}

void asignar_puntero_indirecto(t_fcb *fcbArchivo,uint32_t cantidadPunteros)
{
    uint32_t bloque = bitmap_encontrar_bloque_libre();
    fcbArchivo->PUNTERO_INDIRECTO = bloque; // mapear el fcb y hacer un msync aca
    bitmap_marcar_bloque_ocupado(bloque);

    // asignar los punteros al bloque de punteros segun la cantidad pasada por parametro
}

// esta implementacion es solo para archivos nuevos o archivos con tamaño 0 y sin punteros
void asignar_bloques_archivo_vacio(t_fcb *fcbArchivo,uint32_t tamanioNuevo)
{
    uint32_t tamanioBloques = get_superbloque_block_size(superbloque);

    // Si el tamanio del bloque alcanza, se le asigna solo el puntero directo
    if (tamanioNuevo <= tamanioBloques) {
        asignar_puntero_directo(fcbArchivo);
    }
    else {
        // cantidad de punteros que deberia haber en el bloque de punteros
        uint32_t cantidadPunteros = tamanioNuevo / tamanioBloques - 1;

        asignar_puntero_directo(fcbArchivo);
        asignar_puntero_indirecto(fcbArchivo,cantidadPunteros);
    }
}

// esta implementacion es para archivos que ya tienen punteros asignados
/*void asignar_bloques_archivo_no_vacio(fcbArchivo,tamanioNuevo)
{
    asignar_puntero_indirecto(t_fcb *fcbArchivo);
}*/

void desasignarBloque(t_fcb* fcbArchivo)
{   
    // desasignar el bloque!!
    //bitmap_marcar_bloque_libre(x);
}

/*
archivo_de_bloques_buscar_bloque(uint32_t bloqueBuscado)
{
 // TODO
}

*/