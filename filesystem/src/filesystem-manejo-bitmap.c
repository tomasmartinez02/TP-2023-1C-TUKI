#include <filesystem-manejo-bitmap.h>

int32_t bitmap_encontrar_bloque_libre()
{   
    // false = 0 --> libre
    // true = 1 --> ocupado
    uint32_t i;
    bool bloqueOcupado;
    for (i=0; i < (bitmap->tamanio * 8); i++)
    {   
        bloqueOcupado  = bitarray_test_bit(bitmap->bitarray, i);
        log_acceso_bitmap(i, bloqueOcupado);
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

// Para pruebas
void bitmap_mostrar_por_pantalla()
{   
    // false = 0 --> libre
    // true = 1 --> ocupado
    uint32_t i;
    bool bloqueLeido;
    for (i=0; i < (bitmap->tamanio * 8); i++)
    {
        bloqueLeido  = bitarray_test_bit(bitmap->bitarray, i);
        // Si encuentra un bloque que esté en 0 devuelve la posición de ese bloque
        if(bloqueLeido == 0)
        {
            log_info(filesystemLogger, "Bloque %u: 0", i);
        }
        else
        {
            log_info(filesystemLogger, "Bloque %u: 1", i);
        }
    }
    return;
}

void bitmap_marcar_bloque_libre(uint32_t numeroBloque) // 0 --> libre
{
    bitarray_clean_bit(bitmap->bitarray, numeroBloque);
    // Sincronizar los cambios en el archivo y verificar que se haga de forma correcta
    if (msync(bitmap->direccion, bitmap->tamanio, MS_SYNC) == -1) {
        log_error(filesystemLogger,"Error al sincronizar los cambios en el Bitmap");
    }
    log_acceso_bitmap(numeroBloque, 0);
    return;
}

void bitmap_marcar_bloque_ocupado(uint32_t numeroBloque) // 1 --> ocupado
{
    bitarray_set_bit(bitmap->bitarray, numeroBloque);
    // Sincronizar los cambios en el archivo y verificar que se haga de forma correcta
    if (msync(bitmap->direccion, bitmap->tamanio, MS_SYNC) == -1) {
        log_error(filesystemLogger,"Error al sincronizar los cambios en el Bitmap");
    }
    log_acceso_bitmap(numeroBloque, 1);
    return;
}