// Bibliotecas usada para utilidades de uso comun de los modulos
#ifndef COMMON_UTILS_H_
#define COMMON_UTILS_H_

//Biblitoecas estandar
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
//Bibliotecas commons
#include <commons/collections/list.h>
#include <commons/log.h>

// Prototipos

/**
 * @brief Linear search algorithm to find an element in a list that satisfies the given cutting condition
 * 
 * @param list: elements list
 * @param cutting_condition: boolean condition that satisfies the element we are searching 
 * @param target: element we are looking for 
 * @return int: index of element found or -1 if not found 
 */
int list_get_index(t_list *list, bool (*cutting_condition)(void *temp, void *target), void *target);

/**
 * @brief Tamanio de una array de enteros con caracter centinela 0
 * 
 * @param arrayEnteros: array de enteros distintos de 0 con caracter centinela 0
 * @return uint32_t: tamanio del array 
 */
uint32_t tamanio_array_enteros(uint32_t *arrayEnteros);

/**
 * @brief Abre un archivo chequeando que se haya abierto correctamente
 * 
 * @param pathArchivo: path al archivo que hay que abrir desde el directorio del ejecutable 
 * @param mode: modo de apertura del archivo 
 * @param moduloLogger: logger del modulo que este abriendo el archivo 
 * @return FILE*: descriptor del archivo 
 */
FILE *abrir_archivo(const char *pathArchivo, const char *mode, t_log *moduloLogger);

/**
 * @brief 
 * 
 * @param duracionEnMilisegundos 
 */
void intervalo_de_pausa(uint32_t duracionEnMilisegundos);

/**
 * @brief 
 * 
 * @param moduleConfig 
 * @param config_destroyer 
 * @param moduleMinimalLogger 
 * @param moduleLogger 
 */
void module_destroy(void *moduleConfig, void (*config_destroyer)(void *moduleConfig), t_log *moduleLogger, t_log *moduleMinimalLogger);

/**
 * @brief 
 * 
 * @param argc 
 * @param errorMessage 
 * @param moduleLogger 
 */
void check_arguments(int argc, int numberOfArgsRequired, char *errorMessage, t_log* moduleLogger);

/**
 * @brief 
 * 
 * @param vectorStrings 
 * @return int 
 */
int cantidad_argumentos_vector_strings(char **vectorStrings);

/**
 * @brief 
 * 
 * @param vectorStrings 
 */
void destruir_vector_strings(char **vectorStrings);

#endif