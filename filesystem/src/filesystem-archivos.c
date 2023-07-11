#include <filesystem-archivos.h>

// Variables globales
t_bitmap *bitmap;
t_superbloque *superbloque;
t_fcb *fcb;
FILE* archivoDeBloques;
uint32_t tamanioBloques;
char *directorioFcbs;
char *pathArchivoDeBloques;

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
        log_error(filesystemLogger,"Error al abrir el archivo Bitmap");
    }

    bitmap->tamanio = (blockCount / 8);
    if (ftruncate(fileDescriptor, bitmap->tamanio) == -1) {
        log_error(filesystemLogger,"Error al truncar el archivo Bitmap");
    }

    bitmap->direccion = mmap(NULL, bitmap->tamanio, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor,0);
    if (bitmap->direccion == MAP_FAILED) {
        log_error(filesystemLogger,"Error al mapear el Bitmap");
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

// ARCHIVO DE BLOQUES

FILE *abrir_archivo_de_bloques()
{
    archivoDeBloques = fopen(pathArchivoDeBloques, "r+b");

    if (archivoDeBloques == NULL) {
        log_error(filesystemLogger, "No se pudo abrir el archivo.");
    }
    return archivoDeBloques;
}

void crear_archivo_de_bloques(uint32_t blockCount, uint32_t blockSize)
{
    uint32_t fileDescriptor = open(pathArchivoDeBloques, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fileDescriptor == -1) {
        log_error(filesystemLogger,"Error al abrir el Archivo de Bloques");
    }

    uint32_t tamanioArchivoDeBloques = (blockCount * blockSize);
    if (ftruncate(fileDescriptor, tamanioArchivoDeBloques) == -1) {
        log_error(filesystemLogger,"Error al truncar el Archivo de Bloques");
    }

    close (fileDescriptor);
}

void inicializar_estructuras(void)
{
    char *pathSuperbloque = filesystem_config_get_path_superbloque(filesystemConfig);
    superbloque = crear_superbloque(pathSuperbloque);
    free(pathSuperbloque);

    char *pathBitmap = filesystem_config_get_path_bitmap(filesystemConfig);
    uint32_t blockCount = get_superbloque_block_count(superbloque);
    crear_bitmap(pathBitmap,blockCount);
    free(pathBitmap);

    pathArchivoDeBloques = filesystem_config_get_path_bloques(filesystemConfig);
    tamanioBloques = get_superbloque_block_size(superbloque);
    crear_archivo_de_bloques(blockCount,tamanioBloques);

    directorioFcbs = filesystem_config_get_path_fcb(filesystemConfig);
    recorrer_directorio_fcbs();
}