#include <utils/common-utils.h>

int list_get_index(t_list *list, bool (*cutting_condition)(void *temp, void *target), void* target) 
{
    // Linear search algorithm to find an item with a given condition
    for (int i = 0; i < list_size(list); i++) {
        void *temp = list_get(list, i);
        
        if (cutting_condition(temp, target)) {
            return i;
        }
    }
    
    return -1;
}

uint32_t tamanio_array_enteros(uint32_t *arrayEnteros)
{
    uint32_t tamanioArrayEnteros = 0;

    // Tamanio hasta caracter centinela 0
    while (arrayEnteros[tamanioArrayEnteros] != 0){
        
        tamanioArrayEnteros++;
    }
    
    return tamanioArrayEnteros;
}

FILE *abrir_archivo(const char *pathArchivo, const char *mode, t_log *moduleLogger)
{
    FILE* tempFilePointer = fopen(pathArchivo, mode);

    // Chequeo que el archivo se haya abierto correctamente
    if(tempFilePointer == NULL){
        log_error(moduleLogger, "No se pudo abrir el archivo en el path %s especificado.", pathArchivo);
    }

    return tempFilePointer;
}

void intervalo_de_pausa(uint32_t duracionEnMilisegundos) 
{
    const uint32_t SECS_MILISECS = 1000;
    const uint32_t MILISECS_NANOSECS = 1000000;
    struct timespec timeSpec;
    
    // Time in seconds and nano seconds calculation
    timeSpec.tv_sec = duracionEnMilisegundos / SECS_MILISECS;
    timeSpec.tv_nsec = (duracionEnMilisegundos % SECS_MILISECS) * MILISECS_NANOSECS;
    
    nanosleep(&timeSpec, &timeSpec);

    return;
}

void module_destroy(void *moduleConfig, void (*config_destroyer)(void *moduleConfig), t_log *moduleLogger, t_log *moduleMinimalLogger)
{
    config_destroyer(moduleConfig);
    log_destroy(moduleLogger);
    log_destroy(moduleMinimalLogger);

    return;
}

void check_arguments(int argc, int numberOfArgsRequired, char *errorMessage, t_log *moduleLogger)
{
    if (argc != numberOfArgsRequired) {
        
        log_error(moduleLogger, "Cantidad de argumentos inválida. %s", errorMessage);
        exit(EXIT_FAILURE);
    }

    return;
}

int cantidad_argumentos_vector_strings(char **vectorStrings)
{
    int cantidadArgumentos = 0;

    for(int i = 0; vectorStrings[i] != NULL; i++){

        cantidadArgumentos++;
    }

    return cantidadArgumentos - 1;
}

void destruir_vector_strings(char **vectorStrings)
{
    int cantidadArgumentos = cantidad_argumentos_vector_strings(vectorStrings);

    for(int i = 0; i < cantidadArgumentos + 1; i++){
        	free(vectorStrings[i]);
    }
    free(vectorStrings);
}

// Transforma un int en un string con color amarillo
char *int_to_yellow_string(uint32_t number)
{
    char* yellowStringNumber = string_from_format("\e[1;93m%d\e[0m", number);
    return yellowStringNumber;
}

// Transforma cualquier string en un string de color amarillo

char *string_to_yellow_string(char *string)
{
    char* yellowString = string_from_format("\e[1;93m%s\e[0m", string);
    return yellowString;
}

char* agregarCaracterNulo(void* data, uint32_t length)
{
    char* charData = (char*)data;

    char* str = (char*)malloc((length + 1) * sizeof(char));
    if (str == NULL)
    {
        return NULL;
    }

    // Copiar los caracteres al nuevo char*
    for (uint32_t i = 0; i < length; i++)
    {
        str[i] = charData[i];
    }
    
    // Agregar el carácter nulo al final
    str[length] = '\0';

    return str;
}