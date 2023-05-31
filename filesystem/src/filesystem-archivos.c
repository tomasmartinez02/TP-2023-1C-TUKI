#include <filesystem-archivos.h>

// Variables globales
t_bitarray *bitmap;
t_superbloque *superbloque;

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
    int inicializacionCorrecta = config_init(superbloque, pathSuperbloque, filesystemLogger, __inicializar_superbloque);
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

t_bitarray *abrir_bitmap (char* pathBitmap, uint32_t blockCount)
{
    int fileDescriptor = open(pathBitmap, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fileDescriptor == -1) {
        // error al abrir el archivo
    }

    uint32_t tamanioBitmap = (blockCount / 8);
    if (ftruncate(fileDescriptor,tamanioBitmap) == -1) {
        // error al truncar el archivo
    }

    char* bitmap = mmap(NULL, tamanioBitmap, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor,0);
    if (bitmap == MAP_FAILED) {
        // error
    }

    t_bitarray *bitarray = bitarray_create_with_mode(bitmap,tamanioBitmap,LSB_FIRST);

    close (fileDescriptor);
    munmap(bitmap,tamanioBitmap);
    return bitarray;
}

void crear_bitmap(char *pathBitmap, uint32_t blockCount)
{
    bitmap = abrir_bitmap(pathBitmap, blockCount);
}

// ARCHIVO DE BLOQUES



void inicializar_estructuras(void)
{
    char *pathSuperbloque = filesystem_config_get_path_superbloque(filesystemConfig);
    t_superbloque *superbloque = crear_superbloque(pathSuperbloque);
    free(pathSuperbloque);

    char *pathBitmap = filesystem_config_get_path_bitmap(filesystemConfig);
    uint32_t blockCount = get_superbloque_block_count(superbloque);
    crear_bitmap(pathBitmap,blockCount);
    free(pathBitmap);
}
