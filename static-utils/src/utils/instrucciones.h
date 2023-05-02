// Biblioteca usada para usar instrucciones y registros cpu
#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

#define TAMANIO_STRING_VACIO 0
#define TAMANIO_REG_N 4
#define TAMANIO_REG_E 8
#define TAMANIO_REG_R 16


// Bibliotecas Estandar
#include <stdlib.h>
//Bibliotecas Commons
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/log.h>
// Bibliotecas internas
#include <serializacion/buffer.h>
#include <conexiones/conexiones.h>

// Enums y Estructuras
enum registro {

    // Registros 4 bytes
    REGISTRO_ax,
    REGISTRO_bx,
    REGISTRO_cx,
    REGISTRO_dx,
    // Registros 8 bytes
    REGISTRO_eax,
    REGISTRO_ebx,
    REGISTRO_ecx,
    REGISTRO_edx,
    // Registros 16 bytes
    REGISTRO_rax,
    REGISTRO_rbx,
    REGISTRO_rcx,
    REGISTRO_rdx,
    REGISTRO_null

};

typedef enum registro t_registro;

enum tipo_instruccion {

    // Registros/memoria
    INSTRUCCION_set,
    INSTRUCCION_movin,
    INSTRUCCION_movout,
    // Dispositivos io
    INSTRUCCION_io,
    // Archivos
    INSTRUCCION_fopen,
    INSTRUCCION_fclose,
    INSTRUCCION_fseek,
    INSTRUCCION_fread,
    INSTRUCCION_fwrite,
    INSTRUCCION_ftruncate,
    // Semaforos
    INSTRUCCION_wait,
    INSTRUCCION_signal,
    // Memoria
    INSTRUCCION_create_segment,
    INSTRUCCION_delete_segment,
    // Desalojo voluntario y exit
    INSTRUCCION_yield,
    INSTRUCCION_exit,
    INSTRUCCION_error
    
};
typedef enum tipo_instruccion t_tipo_instruccion;

struct instruccion {
    
    t_tipo_instruccion tipoInstruccion;
    uint32_t operando1;
    uint32_t operando2;
    uint32_t operando3;
    t_registro registro1;
    t_registro registro2;
    char *valorSet;
    char *dispositivoIo;
    char *nombreArchivo;
    char *toString;

};
typedef struct instruccion t_instruccion;

struct info_instruccion {

    uint32_t operando1;
    uint32_t operando2;
    uint32_t operando3;
    t_registro registro1;
    t_registro registro2;
    char *valorSet;
    char *dispositivoIo;
    char *nombreArchivo;
    
};
typedef struct info_instruccion t_info_instruccion;

// Registros de la cpu y pcb
struct registros_cpu{

    // Registros 4 bytes
    char *registroAx;
    char *registroBx;
    char *registroCx;
    char *registroDx;
    // Registros 8 bytes
    char *registroEax;
    char *registroEbx;
    char *registroEcx;
    char *registroEdx;
    // Registros 16 bytes
    char *registroRax;
    char *registroRbx;
    char *registroRcx;
    char *registroRdx;

};
typedef struct registros_cpu t_registros_cpu;

// Prototipos

// Creates y destroys de estructuras

char *t_registro_to_string(t_registro registro);

/**
 * @brief Crea una instruccion
 * 
 * @param tipoInstruccion: Identificador de la instruccion
 * @param infoInstruccion: Estructura auxiliar que para cargar la instruccion
 * @return t_instruccion*: Instruccion creada e instanciada 
 */
t_instruccion *instruccion_create(t_tipo_instruccion tipoInstruccion, t_info_instruccion *infoInstruccion);

/**
 * @brief Crea la estructura para guardar la informacion de la instruccion
 * 
 * @return t_info_instruccion*: Estructura de la info instruccion 
 */
t_info_instruccion *info_instruccion_create(void);

/**
 * @brief Libera la memoria de la estructura para guardar la info de la instruccion
 * 
 * @param self: Info instruccion a destruir 
 */
void info_instruccion_destroy(t_info_instruccion *self);

/**
 * @brief Crea la estructura para guardar los registros en cpu y en los pcb
 * 
 * @return t_registros_cpu*: Estructura que contiene los registros de cpu o pcb 
 */
t_registros_cpu *registros_cpu_create(void);

/**
 * @brief Libera la memoria de la estructura que guarda los registros de cpu o pcb
 * 
 * @param self: Estructura de registros cpu o pcb 
 */
void registros_cpu_destroy(t_registros_cpu *self);

// Empaquetado y desempaquetado de registros cpu

void empaquetar_registros(t_buffer *bufferAEnviar, t_registros_cpu *registrosCpu);
t_registros_cpu *desempaquetar_registros(t_buffer *bufferRecibido);

// Create y destroy de la lista de instrucciones

/**
 * @brief Crea una lista de instruccion a partir de un buffer de instrucciones
 * 
 * @param bufferConInstrucciones: Buffer que contiene instrucciones serializadas 
 * @param loggerModulo: Logger del modulo que lo llama 
 * @return t_list*: Lista de instrucciones 
 */
t_list *instruccion_list_create_from_buffer(t_buffer *bufferConInstrucciones, t_log *loggerModulo);

/**
 * @brief Destruye la lista de instrucciones
 * 
 * @param listaInstrucciones: Lista de instrucciones 
 */
void destroy_lista_instrucciones(t_list* listaInstrucciones);

// Getters instruccion

t_tipo_instruccion instruccion_get_tipo_instruccion(t_instruccion *self);
uint32_t instruccion_get_operando1(t_instruccion *self);
uint32_t instruccion_get_operando2(t_instruccion *self);
uint32_t instruccion_get_operando3(t_instruccion *self);
t_registro instruccion_get_registro1(t_instruccion *self);
t_registro instruccion_get_registro2(t_instruccion *self);
char *instruccion_get_valor_set(t_instruccion *self);
char *instruccion_get_dispositivo_io(t_instruccion *self);
char *instruccion_get_nombre_archivo (t_instruccion *self);
char *instruccion_get_to_string(t_instruccion *self);

// Getters info instruccion

uint32_t info_instruccion_get_operando1(t_info_instruccion *self);
uint32_t info_instruccion_get_operando2(t_info_instruccion *self);
uint32_t info_instruccion_get_operando3(t_info_instruccion *self);
t_registro info_instruccion_get_registro1(t_info_instruccion *self);
t_registro info_instruccion_get_registro2(t_info_instruccion *self);
char *info_instruccion_get_valor_set(t_info_instruccion *self);
char *info_instruccion_get_dispositivo_io(t_info_instruccion *self);
char *info_instruccion_get_nombre_archivo (t_info_instruccion *self);

// Setters info instruccion

void info_instruccion_set_operando1(t_info_instruccion* self, uint32_t operando1); 
void info_instruccion_set_operando2(t_info_instruccion* self, uint32_t operando2); 
void info_instruccion_set_operando3(t_info_instruccion *self, uint32_t operando3);
void info_instruccion_set_registro1(t_info_instruccion* self, t_registro registro1); 
void info_instruccion_set_registro2(t_info_instruccion* self, t_registro registro2); 
void info_instruccion_set_valor_set(t_info_instruccion *self, char *valorSet);
void info_instruccion_set_dispositivo_io(t_info_instruccion *self, char *dispositivoIo); 
void info_instruccion_set_nombre_archivo(t_info_instruccion *self, char *nombreArchivo);

// Getters registros cpu

char *registros_cpu_get_registro_ax(t_registros_cpu *self);
char *registros_cpu_get_registro_bx(t_registros_cpu *self);
char *registros_cpu_get_registro_cx(t_registros_cpu *self);
char *registros_cpu_get_registro_dx(t_registros_cpu *self);
char *registros_cpu_get_registro_eax(t_registros_cpu *self);
char *registros_cpu_get_registro_ebx(t_registros_cpu *self);
char *registros_cpu_get_registro_ecx(t_registros_cpu *self);
char *registros_cpu_get_registro_edx(t_registros_cpu *self);
char *registros_cpu_get_registro_rax(t_registros_cpu *self);
char *registros_cpu_get_registro_rbx(t_registros_cpu *self);
char *registros_cpu_get_registro_rcx(t_registros_cpu *self);
char *registros_cpu_get_registro_rdx(t_registros_cpu *self);

// Setters registros cpu

void registros_cpu_set_registro_ax(t_registros_cpu *self, char *valor);
void registros_cpu_set_registro_bx(t_registros_cpu *self, char *valor);
void registros_cpu_set_registro_cx(t_registros_cpu *self, char *valor);
void registros_cpu_set_registro_dx(t_registros_cpu *self, char *valor);
void registros_cpu_set_registro_eax(t_registros_cpu *self, char *valor);
void registros_cpu_set_registro_ebx(t_registros_cpu *self, char *valor);
void registros_cpu_set_registro_ecx(t_registros_cpu *self, char *valor);
void registros_cpu_set_registro_edx(t_registros_cpu *self, char *valor);
void registros_cpu_set_registro_rax(t_registros_cpu *self, char *valor);
void registros_cpu_set_registro_rbx(t_registros_cpu *self, char *valor);
void registros_cpu_set_registro_rcx(t_registros_cpu *self, char *valor);
void registros_cpu_set_registro_rdx(t_registros_cpu *self, char *valor);



#endif