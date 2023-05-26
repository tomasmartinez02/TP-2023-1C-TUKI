#include <kernel-archivos.h>

// Set y Get nombre

char* archivo_get_nombre(t_archivo* archivo)
{
    return archivo->nombre;
}

void archivo_set_nombre(t_archivo* archivo, char* nombre)
{
    archivo->nombre = strdup(nombre);
}

// Set y Get estado
bool archivo_get_estado(t_archivo* archivo)
{
    return archivo->abierto;
}

void archivo_set_estado(t_archivo* archivo, bool estado)
{
    archivo->abierto = estado;
}

// Get mutex Archivo
pthread_mutex_t* archivo_get_mutex(t_archivo* archivo)
{
    return archivo->mutex;
}

// Set y get puntero
uint32_t archivo_get_posicionPuntero(t_archivo* archivo)
{
    return archivo->posicionPuntero;
}

void archivo_set_posicionPuntero(t_archivo* archivo, uint32_t posicion)
{
    archivo->posicionPuntero = posicion;
}

// Crear y destruir archivo
t_archivo *crear_archivo(char* nombre)
{
    t_archivo *archivo = malloc(sizeof(*archivo));
    
    archivo->nombre = strdup(nombre);
    archivo->abierto = false;
    archivo->posicionPuntero = 0; 

    pthread_mutex_t *mutex = malloc(sizeof(*(archivo->mutex)));
    pthread_mutex_init(mutex, NULL);
    archivo->mutex = mutex;
    
    return archivo;
}

void destruir_archivo(t_archivo* archivo) 
{
    pthread_mutex_lock(archivo_get_mutex(archivo));
     char *nombreArchivo = archivo->nombre;
    if (nombreArchivo != NULL) {
        free(nombreArchivo);
    }
    pthread_mutex_unlock(archivo_get_mutex(archivo));

    pthread_mutex_destroy(archivo->mutex);
    free(archivo->mutex);

    free(archivo);
}

