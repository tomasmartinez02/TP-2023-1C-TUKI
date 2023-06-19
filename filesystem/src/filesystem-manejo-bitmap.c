#include <filesystem-manejo-bitmap.h>

int32_t bitmap_encontrar_bloque_libre()
{   
    // false = 0 --> libre
    // true = 1 --> ocupado
    uint32_t i;
    bool bloqueOcupado;
    for (i=0; i < bitmap->tamanio ; i++)
    {
        bloqueOcupado  = bitarray_test_bit(bitmap->bitarray, i);
        // Si encuentra un bloque que esté en 0 devuelve la posición de ese bloque
        if(!bloqueOcupado)
        {
            return i;
            break;
        }
    }
     // Si no encuentra un bloque libre, retorna -1
    return -1;
}

void bitmap_marcar_bloque_libre(uint32_t numeroBloque) // 0 --> libre
{
    bitarray_clean_bit(bitmap->bitarray, numeroBloque);
    // Sincronizar los cambios en el archivo y verificar que se haga de forma correcta
    if (msync(bitmap->direccion, bitmap->tamanio, MS_SYNC) == -1) {
        log_error(filesystemLogger,"Error al sincronizar los cambios en el Bitmap");
    }
    return;
}

void bitmap_marcar_bloque_ocupado(uint32_t numeroBloque) // 1 --> ocupado
{
    bitarray_set_bit(bitmap->bitarray, numeroBloque);
    // Sincronizar los cambios en el archivo y verificar que se haga de forma correcta
    if (msync(bitmap->direccion, bitmap->tamanio, MS_SYNC) == -1) {
        log_error(filesystemLogger,"Error al sincronizar los cambios en el Bitmap");
    }
    return;
}