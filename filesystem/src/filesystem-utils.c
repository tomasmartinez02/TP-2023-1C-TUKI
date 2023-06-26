#include <filesystem-utils.h>

// Funciones privadas

// Adapta la funcion para destruir el config
static void __config_destroyer(void *moduleConfig)
{
    t_filesystem_config *filesystemConfigTemp = (t_filesystem_config *) moduleConfig;
    filesystem_config_destroy(filesystemConfigTemp);

    return;
}

// Funciones publicas
void filesystem_destroy(t_filesystem_config *filesystemConfig, t_log *filesystemLogger, t_log *filesystemDebuggingLogger)
{
    module_destroy((void *) filesystemConfig, __config_destroyer, filesystemLogger, filesystemDebuggingLogger);

    return;
}
void log_existe_archivo(char *nombreArchivo)
{   
    log_info(filesystemLogger, "El Archivo con nombre <%s> existe", nombreArchivo);
    log_info(filesystemDebuggingLogger, "El Archivo con nombre <%s> existe", nombreArchivo);
    return;   
}

void log_no_existe_archivo(char *nombreArchivo)
{   
    log_info(filesystemLogger, "El Archivo con nombre <%s> no existe", nombreArchivo);
    log_info(filesystemDebuggingLogger, "El Archivo con nombre <%s> no existe", nombreArchivo);
    return;   
}

void log_crear_archivo(char *nombreArchivo)
{   
    log_info(filesystemLogger, "Crear Archivo: <%s>", nombreArchivo);
    log_info(filesystemDebuggingLogger, "Crear Archivo: <%s>", nombreArchivo);
    return;   
}

void log_abrir_archivo(char *nombreArchivo)
{   
    log_info(filesystemLogger, "Abrir Archivo: <%s>", nombreArchivo);
    log_info(filesystemDebuggingLogger, "Abrir Archivo: <%s>", nombreArchivo);
    return;   
}

void log_truncar_archivo(char *nombreArchivo, uint32_t tamanio)
{   
    log_info(filesystemLogger, "Truncar Archivo: <%s> - Tamaño: <%d>", nombreArchivo, tamanio);
    log_info(filesystemDebuggingLogger, "Truncar Archivo: <%s> - Tamaño: <%d>", nombreArchivo, tamanio);
    return;   
}

void log_acceso_bitmap(uint32_t numeroBloque, uint32_t estadoBloque)
{   
    log_info(filesystemLogger, "Acceso a Bitmap - Bloque: <%d> - Estado: <%d>", numeroBloque, estadoBloque);
    log_info(filesystemDebuggingLogger, "Acceso a Bitmap - Bloque: <%d> - Estado: <%d>", numeroBloque, estadoBloque);
    return;   
}

void log_lectura_archivo(char *nombreArchivo, uint32_t puntero, uint32_t direccionMemoria, uint32_t tamanio)
{   
    log_info(filesystemLogger, "Leer Archivo: <%s> - Puntero: <%d> - Memoria: <%d> - Tamaño: <%d>", nombreArchivo, puntero, direccionMemoria, tamanio);
    log_info(filesystemDebuggingLogger, "Leer Archivo: <%s> - Puntero: <%d> - Memoria: <%d> - Tamaño: <%d>", nombreArchivo, puntero, direccionMemoria, tamanio);
    return;   
}

void log_escritura_archivo(char *nombreArchivo, uint32_t puntero, uint32_t direccionMemoria, uint32_t tamanio)
{   
    log_info(filesystemLogger, "Escribir Archivo: <%s> - Puntero: <%d> - Memoria: <%d> - Tamaño: <%d>", nombreArchivo, puntero, direccionMemoria, tamanio);
    log_info(filesystemDebuggingLogger, "Escribir Archivo: <%s> - Puntero: <%d> - Memoria: <%d> - Tamaño: <%d>", nombreArchivo, puntero, direccionMemoria, tamanio);
    return;   
}

void log_acceso_bloque(char *nombreArchivo, uint32_t numeroBloqueArchivo, uint32_t numeroBloqueFs)
{   
    log_info(filesystemLogger, "Acceso Bloque - Archivo: <%s> - Bloque Archivo: <%d> - Bloque File System <%d>", nombreArchivo, numeroBloqueArchivo, numeroBloqueFs);
    log_info(filesystemDebuggingLogger, "Acceso Bloque - Archivo: <%s> - Bloque Archivo: <%d> - Bloque File System <%d>", nombreArchivo, numeroBloqueArchivo, numeroBloqueFs);
    return;   
}

void log_acceso_bloque_punteros(char *nombreArchivo, uint32_t numeroBloqueFs)
{   
    log_info(filesystemLogger, "Acceso Bloque de Punteros - Archivo: <%s> - Bloque Archivo: <1> - Bloque File System <%d>", nombreArchivo, numeroBloqueFs);
    log_info(filesystemDebuggingLogger, "Acceso Bloque de Punteros - Archivo: <%s> - Bloque Archivo: <1> - Bloque File System <%d>", nombreArchivo, numeroBloqueFs);
    return;   
}

void log_bloque_asignado(char *nombreArchivo, uint32_t bloqueDatos)
{   
    log_info(filesystemLogger, "Bloque <%d> asignado al Archivo: <%s>", bloqueDatos, nombreArchivo);
    log_info(filesystemDebuggingLogger, "Bloque <%d> asignado al Archivo: <%s>", bloqueDatos, nombreArchivo);
    return;   
}

void log_bloque_desasignado(char *nombreArchivo, uint32_t bloqueDatos)
{   
    log_info(filesystemLogger, "Bloque <%d> desasignado al Archivo: <%s>", bloqueDatos, nombreArchivo);
    log_info(filesystemDebuggingLogger, "Bloque <%d> desasignado al Archivo: <%s>", bloqueDatos, nombreArchivo);
    return;   
}

uint32_t redondearHaciaArriba(uint32_t nuevoTamanio, uint32_t tamanio)
{
    uint32_t resultado = (nuevoTamanio / tamanio);
    if ((nuevoTamanio % tamanio) == 0)
    {
        return resultado;
        log_info(filesystemLogger,"el resultado es: %u",resultado);
    }
    return (resultado+1);
}