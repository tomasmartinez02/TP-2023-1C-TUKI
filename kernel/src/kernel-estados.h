#ifndef KERNEL_ESTADOS_H
#define KERNEL_ESTADOS_H

#include <kernel-estructuras.h>

/**
 * @brief Crea el estado 
 *
 * @param nombre: Nombre del estado a crear
 * 
 * @return t_estado*: Estructura del estado
 * 
 * @example t_estado* new = crear_estado(NEW);
 */
t_estado* crear_estado(t_nombre_estado nombre);

/**
 * @brief Destruye la estructura en memoria del estado 
 * 
 * @param estado: Estado a destruir
 * 
 * @example destruir_estado(new); 
 */
void destruir_estado(t_estado* estado); 



#endif