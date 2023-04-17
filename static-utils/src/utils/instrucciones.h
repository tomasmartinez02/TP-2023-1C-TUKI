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

    INSTRUCCION_set,
    INSTRUCCION_movin,
    INSTRUCCION_movout,
    INSTRUCCION_io,
    INSTRUCCION_fopen,
    INSTRUCCION_fclose,
    INSTRUCCION_fseek,
    INSTRUCCION_fread,
    INSTRUCCION_fwrite,
    INSTRUCCION_ftruncate,
    INSTRUCCION_wait,
    INSTRUCCION_signal,
    INSTRUCCION_create_segment,
    INSTRUCCION_delete_segment,
    INSTRUCCION_yield,
    INSTRUCCION_exit,
    INSTRUCCION_error
    
};
typedef enum tipo_instruccion t_tipo_instruccion;

enum registro {

    REGISTRO_ax,
    REGISTRO_bx,
    REGISTRO_cx,
    REGISTRO_dx,
    REGISTRO_eax,
    REGISTRO_ebx,
    REGISTRO_ecx,
    REGISTRO_edx,
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
    char *dispositivoIo;
    char *nombreArchivo;
    char *toString;

};
typedef struct instruccion t_instruccion;

typedef struct{

    uint32_t operando1;
    uint32_t operando2;
    uint32_t operando3;
    t_registro registro1;
    t_registro registro2;
    char *dispositivoIo;
    char *nombreArchivo;
    
} t_info_instruccion;

struct registros_cpu{

    uint32_t registroAx;
    uint32_t registroBx;
    uint32_t registroCx;
    uint32_t registroDx;
    uint32_t registroEax;
    uint32_t registroEbx;
    uint32_t registroEcx;
    uint32_t registroEdx;
    uint32_t registroRax;
    uint32_t registroRbx;
    uint32_t registroRcx;
    uint32_t registroRdx;

};
typedef struct registros_cpu t_registros_cpu;