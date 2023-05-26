#ifndef KERNEL_ARCHIVOS_H_
#define KERNEL_ARCHIVOS_H_

//Standard Libraries
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

// Bibliotecas internas
#include <kernel-estructuras.h>


// Prototipos

// Builders y destroyers de estructuras
t_archivo *crear_archivo(char* nombre);
void destruir_archivo(t_archivo* archivo);

// Setters y getters
char* archivo_get_nombre(t_archivo* archivo);
void archivo_set_nombre(t_archivo* archivo, char* nombre);
bool archivo_get_estado(t_archivo* archivo);
void archivo_set_estado(t_archivo* archivo, bool estado);
pthread_mutex_t* archivo_get_mutex(t_archivo* archivo);
uint32_t archivo_get_posicionPuntero(t_archivo* archivo);
void archivo_set_posicionPuntero(t_archivo* archivo, uint32_t posicion);

#endif