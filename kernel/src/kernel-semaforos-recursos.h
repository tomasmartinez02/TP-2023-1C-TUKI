#ifndef KERNEL_SEMAFOROS_RECURSOS_H_
#define KERNEL_SEMAFOROS_RECURSOS_H_

// Bibliotecas estandar
// Bibliotecas commons
#include <commons/collections/dictionary.h>
// Bibliotecas static utils
// Bibliotecas internas
#include <kernel-estructuras.h>
#include <kernel-estados.h>

// Prototipos

// Interfaz de semaforo_recurso

// Getters y setters semaforo recurso
int32_t semaforo_recurso_get_instancias(t_semaforo_recurso *self);
t_estado *semaforo_recurso_get_estado_recurso(t_semaforo_recurso *self);
void semaforo_recurso_wait(t_semaforo_recurso *self);
void semaforo_recurso_post(t_semaforo_recurso *self);
bool semaforo_recurso_puedo_utilizar_recurso(t_semaforo_recurso *self);
bool semaforo_recurso_hay_procesos_bloqueados(t_semaforo_recurso *self);
void semaforo_recurso_bloquear_proceso(t_semaforo_recurso *self, t_pcb *pcbBloqueado);
t_pcb *semaforo_recurso_desbloquear_primer_proceso_bloqueado(t_semaforo_recurso *self);
bool semaforo_recurso_debe_bloquear_proceso(t_semaforo_recurso *self);

// Interfaz de diccionario semaforos recursos

t_dictionary *crear_diccionario_semaforos_recursos(char **recursos, char **instanciasRecursos);
void destruir_diccionario_semaforos_recursos(t_dictionary *diccionarioSemaforosRecursos);
bool diccionario_semaforos_recursos_existe_recurso(t_dictionary *diccionarioSemaforosRecursos, char *nombreRecurso);
t_semaforo_recurso *diccionario_semaforos_recursos_get_semaforo_recurso(t_dictionary *diccionarioSemaforosRecursos, char *nombreRecurso);
bool semaforo_recurso_debe_desbloquear_recurso(t_semaforo_recurso *self);

#endif