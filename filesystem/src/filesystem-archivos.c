#include <filesystem-archivos.h>

// Variables globales
t_bitmap *bitmap;
t_superbloque *superbloque;
t_fcb *fcb;

// SUPERBLOQUE

static void __inicializar_superbloque (void *module, t_config *tempSuperbloque)
{
    superbloque = (t_superbloque *) module;

    superbloque->BLOCK_SIZE = (uint32_t) config_get_int_value(tempSuperbloque, "BLOCK_SIZE");
    superbloque->BLOCK_COUNT = (uint32_t) config_get_int_value(tempSuperbloque, "BLOCK_COUNT");

    return;
}

t_superbloque *crear_superbloque (char *pathSuperbloque)
{
    t_superbloque *superbloque = malloc(sizeof(*superbloque));

    // Checkear que se haya inicializado bien
    int32_t inicializacionCorrecta = config_init(superbloque, pathSuperbloque, filesystemLogger, __inicializar_superbloque);
    if (inicializacionCorrecta == -1) {
        exit(EXIT_FAILURE);
    }

    return superbloque;
}

void destruir_superbloque (t_superbloque *superbloque)
{
    free(superbloque);

    return;
}

uint32_t get_superbloque_block_size (t_superbloque *superbloque)
{
    return superbloque->BLOCK_SIZE;
}

uint32_t get_superbloque_block_count(t_superbloque *superbloque)
{
    return superbloque->BLOCK_COUNT;
}

// BITMAP

void abrir_bitmap (char* pathBitmap, uint32_t blockCount)
{
    bitmap = malloc(sizeof(t_bitmap));

    uint32_t fileDescriptor = open(pathBitmap, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fileDescriptor == -1) {
        // error al abrir el archivo
    }

    bitmap->tamanio = (blockCount / 8);
    if (ftruncate(fileDescriptor, bitmap->tamanio) == -1) {
        // error al truncar el archivo
        //ponele unos logs xfis<3
    }

    bitmap->direccion = mmap(NULL, bitmap->tamanio, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor,0);
    if (bitmap->direccion == MAP_FAILED) {
        // error
    }

    bitmap->bitarray = bitarray_create_with_mode(bitmap->direccion, bitmap->tamanio, LSB_FIRST);

    close (fileDescriptor);
}

void crear_bitmap(char *pathBitmap, uint32_t blockCount)
{
    abrir_bitmap(pathBitmap, blockCount);
}

void destruir_bitmap()
{
    munmap(bitmap->direccion, bitmap->tamanio);
    bitarray_destroy(bitmap->bitarray);
}

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
        }
    }
     // Si no encuentra un bloque libre, retorna -1
    return -1;
}

void bitmap_marcar_bloque_libre(uint32_t numeroBloque) // 0 --> libre
{
     bitarray_clean_bit(bitmap->bitarray, numeroBloque);
     return;
}

void bitmap_marcar_bloque_ocupado(uint32_t numeroBloque) // 1 --> ocupado
{
     bitarray_set_bit(bitmap->bitarray, numeroBloque);
     return;
}


// ARCHIVO DE BLOQUES --- falta terminar ---

void abrir_archivo_de_bloques (char *pathArchivoDeBloques, uint32_t blockCount, uint32_t blockSize)
{
    uint32_t fileDescriptor = open(pathArchivoDeBloques, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fileDescriptor == -1) {
        // error al abrir el archivo
    }

    uint32_t tamanioArchivoDeBloques = (blockCount * blockSize);
    if (ftruncate(fileDescriptor, tamanioArchivoDeBloques) == -1) {
        // error al truncar el archivo
    }

    // no se si vamos a usar mmap o fseek, fread y fwrite
    char* direccion = mmap(NULL, tamanioArchivoDeBloques, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor,0);
    if (direccion == MAP_FAILED) {
        // error
    }

    close (fileDescriptor);
    //munmap(direccion, tamanioArchivoDeBloques);
}

void crear_archivo_de_bloques(char *pathArchivoDeBloques, uint32_t blockCount, uint32_t blockSize)
{
    abrir_archivo_de_bloques(pathArchivoDeBloques, blockCount, blockSize);
}

/*
archivo_de_bloques_buscar_bloque(uint32_t bloqueBuscado)
{
 // TODO
}

*/

void inicializar_estructuras(void)
{
    char *pathSuperbloque = filesystem_config_get_path_superbloque(filesystemConfig);
    superbloque = crear_superbloque(pathSuperbloque);
    free(pathSuperbloque);

    char *pathBitmap = filesystem_config_get_path_bitmap(filesystemConfig);
    uint32_t blockCount = get_superbloque_block_count(superbloque);
    crear_bitmap(pathBitmap,blockCount);
    free(pathBitmap);

    char *pathArchivoDeBloques = filesystem_config_get_path_bloques(filesystemConfig);
    uint32_t blockSize = get_superbloque_block_size(superbloque);
    crear_archivo_de_bloques(pathArchivoDeBloques,blockCount,blockSize);
    free(pathArchivoDeBloques);

    char *directorioFcbs = filesystem_config_get_path_fcb(filesystemConfig);
    recorrer_directorio_fcbs(directorioFcbs);
    free(directorioFcbs);
}