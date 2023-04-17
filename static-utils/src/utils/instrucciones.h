#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

// Bibliotecas Estandar
#include <stdlib.h>
//Bibliotecas Commons
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/log.h>
// Bibliotecas internas
#include <serializacion/buffer.h>
#include <conexiones/conexiones.h>

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

t_registros_cpu *registros_cpu_create(void);
void registros_cpu_destroy(t_registros_cpu *registrosCpu);
char *instruccion_to_string(t_instruccion *self);
t_instruccion *instruccion_create(t_tipo_instruccion tipoInstruccion, t_info_instruccion *infoInstruccion);
void instruccion_destroy(void *selfVoid);
t_info_instruccion *info_instruccion_create(void);
void info_instruccion_destroy(t_info_instruccion *self);
t_list* instruccion_list_create_from_buffer(t_buffer* bufferConInstrucciones, t_log* logger);
void destroy_instructions_list(t_list* instructionsList);
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
uint32_t info_instruccion_get_operando1(t_info_instruccion *self);
uint32_t info_instruccion_get_operando2(t_info_instruccion *self);
uint32_t info_instruccion_get_operando3(t_info_instruccion *self);
t_registro info_instruccion_get_registro1(t_info_instruccion *self);
t_registro info_instruccion_get_registro2(t_info_instruccion *self);
char *info_instruccion_get_valor_set(t_info_instruccion *self);
char *info_instruccion_get_dispositivo_io(t_info_instruccion *self);
char *info_instruccion_get_nombre_archivo (t_info_instruccion *self);
void info_instruccion_set_operando1(t_info_instruccion* self, uint32_t operando1); 
void info_instruccion_set_operando2(t_info_instruccion* self, uint32_t operando2); 
void info_instruccion_set_operando3(t_info_instruccion *self, uint32_t operando3);
void info_instruccion_set_registro1(t_info_instruccion* self, t_registro registro1); 
void info_instruccion_set_registro2(t_info_instruccion* self, t_registro registro2); 
void info_instruccion_set_valor_set(t_info_instruccion *self, char *valorSet);
void info_instruccion_set_dispositivo_io(t_info_instruccion *self, char *dispositivoIo); 
void info_instruccion_set_nombre_archivo(t_info_instruccion *self, char *nombreArchivo);

#endif