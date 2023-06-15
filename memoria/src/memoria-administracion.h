// Biblioteca para guardar las estructuras y variables globales del kernel
#ifndef MEMORIA_ADMINISTRACION_H_
#define MEMORIA_ADMINISTRACION_H_

// Bibliotecas commons
#include <commons/log.h>

// Bibliotecas internas
#include <memoria-utils.h>
#include <memoria-estructuras.h>
#include <utils/tablas-pcb.h>

// Prototipos

void inicializar_memoria(void);
void actualizar_lista_huecos_libres (t_info_segmentos *segmento);
bool verificar_memoria_suficiente (uint32_t tamanioSolicitado);
bool verificar_memoria_contigua (uint32_t tamanioSolicitado);
uint32_t crear_segmento(t_info_segmentos* segmento, uint32_t pid);
void eliminar_segmento(uint32_t idSegmento, uint32_t pid);

#endif