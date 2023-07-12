// Bibliotecas usada para utilidades de uso comun de los modulos
#ifndef COMMON_UTILS_H_
#define COMMON_UTILS_H_

//Biblitoecas estandar
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
//Bibliotecas commons
#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/string.h>

// Prototipos

/**
 * @brief Linear search algorithm to find an element in a list that satisfies the given cutting condition
 * 
 * @param list: elements list
 * @param cutting_condition: boolean condition that satisfies the element we are searching 
 * @param target: element we are looking for 
 * @return int: index of element found or -1 if not found
 * 
 * @example int indice = list_get_index(listaStrings, string_es, stringHola);
 */
int list_get_index(t_list *list, bool (*cutting_condition)(void *temp, void *target), void *target);

/**
 * @brief Tamanio de una array de enteros con caracter centinela 0
 * 
 * @param arrayEnteros: array de enteros distintos de 0 con caracter centinela 0
 * @return uint32_t: tamanio del array
 * 
 * @example uint32_t tamanioArray = tamanio_array_enteros(arrayEnteros);
 */
uint32_t tamanio_array_enteros(uint32_t *arrayEnteros);

/**
 * @brief Abre un archivo chequeando que se haya abierto correctamente
 * 
 * @param pathArchivo: path al archivo que hay que abrir desde el directorio del ejecutable 
 * @param mode: modo de apertura del archivo 
 * @param moduloLogger: logger del modulo que este abriendo el archivo 
 * @return FILE*: descriptor del archivo
 * 
 * @example FILE *archivo = abrir_archivo("../miarchivo.txt", "r", loggerModulo); 
 */
FILE *abrir_archivo(const char *pathArchivo, const char *mode, t_log *moduloLogger);

/**
 * @brief Duerme a un hilo o proceso por una cierta cantidad de milisegundos
 * 
 * @param duracionEnMilisegundos: Duracion de la pausa para el hilo o proceso
 * 
 * @example intervalo_de_pausa(tiempoEnMilisegundosDePausa); 
 */
void intervalo_de_pausa(uint32_t duracionEnMilisegundos);

/**
 * @brief Frees up and destroys module main structures
 * 
 * @param moduleConfig 
 * @param config_destroyer 
 * @param moduleMinimalLogger 
 * @param moduleLogger 
 */
void module_destroy(void *moduleConfig, void (*config_destroyer)(void *moduleConfig), t_log *moduleLogger, t_log *moduleMinimalLogger);

/**
 * @brief Checks if a module receives the correct number of arguments
 * 
 * @param argc: argc main function variable, arg count 
 * @param errorMessage: Message to display to user. Ex: "Argumentos: <configPath>" 
 * @param moduleLogger: Logger del modulo
 * 
 * @example check_arguments(argc, 2, "Argumentos: <configPath>");
 */
void check_arguments(int argc, int numberOfArgsRequired, char *errorMessage, t_log *moduleLogger);

/**
 * @brief Chequea la cantidad de elementos que posee un vector de strings, en este caso una instruccion
 * 
 * @param vectorStrings: Vector de strings finalizado en NULL
 * @return int: Cantidad de argumentos de la instruccion 
 * 
 * @example int tamanioVectorStrings = cantidad_argumentos_vector_strings(vectorStrings);
 */
int cantidad_argumentos_vector_strings(char **vectorStrings);

/**
 * @brief Destruye o libera un vector de strings finalizado en NULL
 * 
 * @param vectorStrings 
 * 
 * @example destruir_vector_strings(vectorStrings);
 */
void destruir_vector_strings(char **vectorStrings);

/**
 * @brief Transforma un numero en un string con el numero amarillo
 * 
 * @param number: Numero del que quiero obtener el string amarillo 
 * @return char*: string amarillo del numero 
 */
char *int_to_yellow_string(uint32_t number);

/**
 * @brief Transforma cualquier string en un string amarillo
 * 
 * @param string: String sin color
 * @return char*: String amarillo 
 */
char *string_to_yellow_string(char *string);

char* agregarCaracterNulo(void* data, uint32_t length); 

#endif