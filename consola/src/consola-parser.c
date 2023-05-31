#include <consola-parser.h>

// Funciones privadas

static bool __es_instruccion_con_tres_argumentos(char *instruccion)
{
    return !strcmp(instruccion, "F_READ")
        || !strcmp(instruccion, "F_WRITE"); 
}

static bool __es_instruccion_con_dos_argumentos(char *instruccion)
{
    return !strcmp(instruccion, "SET") 
        || !strcmp(instruccion, "MOV_IN")
        || !strcmp(instruccion, "MOV_OUT")
        || !strcmp(instruccion, "F_SEEK")
        || !strcmp(instruccion, "F_TRUNCATE")
        || !strcmp(instruccion, "CREATE_SEGMENT");
}

static bool __es_instruccion_con_un_argumento(char *instruccion)
{
    return !strcmp(instruccion, "I/O") 
        || !strcmp(instruccion, "F_OPEN")
        || !strcmp(instruccion, "F_CLOSE")
        || !strcmp(instruccion, "WAIT")
        || !strcmp(instruccion, "SIGNAL")
        || !strcmp(instruccion, "DELETE_SEGMENT");
}

static bool __es_instruccion_valida(char *instruccion)
{
    return __es_instruccion_con_tres_argumentos(instruccion)
        || __es_instruccion_con_dos_argumentos(instruccion)
        || __es_instruccion_con_un_argumento(instruccion)
        || !strcmp(instruccion, "YIELD")
        || !strcmp(instruccion, "EXIT");
}
static t_tipo_instruccion __obtener_tipo_instruccion(char* instruccion)
{
    if(!strcmp(instruccion, "SET")) return INSTRUCCION_set;
    else if(!strcmp(instruccion, "MOV_IN")) return INSTRUCCION_movin;
    else if(!strcmp(instruccion, "MOV_OUT")) return INSTRUCCION_movout;
    else if(!strcmp(instruccion, "I/O")) return INSTRUCCION_io;
    else if(!strcmp(instruccion, "F_OPEN")) return INSTRUCCION_fopen;
    else if(!strcmp(instruccion, "F_CLOSE")) return INSTRUCCION_fclose;
    else if(!strcmp(instruccion, "F_SEEK")) return INSTRUCCION_fseek;
    else if(!strcmp(instruccion, "F_READ")) return INSTRUCCION_fread;
    else if(!strcmp(instruccion, "F_WRITE")) return INSTRUCCION_fwrite;
    else if(!strcmp(instruccion, "F_TRUNCATE")) return INSTRUCCION_ftruncate;
    else if(!strcmp(instruccion, "WAIT")) return INSTRUCCION_wait;
    else if(!strcmp(instruccion, "SIGNAL")) return INSTRUCCION_signal;
    else if(!strcmp(instruccion, "CREATE_SEGMENT")) return INSTRUCCION_create_segment;
    else if(!strcmp(instruccion, "DELETE_SEGMENT")) return INSTRUCCION_delete_segment;
    else if(!strcmp(instruccion, "YIELD")) return INSTRUCCION_yield;
    else if(!strcmp(instruccion, "EXIT")) return INSTRUCCION_exit;
    else return INSTRUCCION_error;
}

static t_registro __obtener_registro(char* registro)
{
    if(!strcmp(registro, "AX")) return REGISTRO_ax;
    else if(!strcmp(registro, "BX")) return REGISTRO_bx;
    else if(!strcmp(registro, "CX")) return REGISTRO_cx;
    else if(!strcmp(registro, "DX")) return REGISTRO_dx;
    else if(!strcmp(registro, "EAX")) return REGISTRO_eax;
    else if(!strcmp(registro, "EBX")) return REGISTRO_ebx;
    else if(!strcmp(registro, "ECX")) return REGISTRO_ecx;
    else if(!strcmp(registro, "EDX")) return REGISTRO_edx;
    else if(!strcmp(registro, "RAX")) return REGISTRO_rax;
    else if(!strcmp(registro, "RBX")) return REGISTRO_rbx;
    else if(!strcmp(registro, "RCX")) return REGISTRO_rcx;
    else if(!strcmp(registro, "RDX")) return REGISTRO_rdx;
    else return REGISTRO_null;
}

//Funciones publicas

bool consola_parser_parse_instrucciones(t_buffer *buffer, const char *pathInstrucciones) 
{
    bool parseSuccess = false;
    FILE *archivoInstrucciones = abrir_archivo(pathInstrucciones, "r", consolaLogger);
    if(archivoInstrucciones == NULL) return parseSuccess;
    
    for (;;) {
        
        char *instruccion = malloc(MAX_LENGTH_INSTRUCTION);

        // Verifico que se haya leido correctamente el archivo
        if (fgets(instruccion, MAX_LENGTH_INSTRUCTION, archivoInstrucciones) != NULL) {
            
            // Verifico que la linea leida no sea un salto de linea (linea vacia)
            if (string_equals_ignore_case(instruccion, "\n")) {

                free(instruccion);
                continue;
            }
            
            // Utilizo una copia de la instruccion para evitar errores con el buffer devuelto por fgets
            char *auxInstruccion = string_duplicate(instruccion);
            free(instruccion);

            // Separo la instruccion en identificador y parametros
            string_trim(&auxInstruccion);
            char** vectorStringsInstruccion = string_split(auxInstruccion, " ");
            char *identificadorInstruccion = string_duplicate(vectorStringsInstruccion[0]);
            string_trim(&identificadorInstruccion);
            
            // Verifico que la instruccion este dentro del set de instrucciones
            if(__es_instruccion_valida(identificadorInstruccion)){
                
                t_tipo_instruccion tipoInstruccion = __obtener_tipo_instruccion(identificadorInstruccion);
                // Verifico que sea instruccion con tres argumentos
                if (__es_instruccion_con_tres_argumentos(identificadorInstruccion)) {
                    
                    // Verifico que efectivamente tenga tres argumentos
                    if (string_array_size(vectorStringsInstruccion) != 4) {
                        log_error(consolaLogger, "La instruccion %s debe contener tres argumentos. Uso: %s <argumento1> <argumento2> <argumento3>", identificadorInstruccion, identificadorInstruccion);
                        log_error(consolaLogger, "La instruccion %s debe contener tres argumentos. Uso: %s <argumento1> <argumento2> <argumento3>", identificadorInstruccion, identificadorInstruccion);
                        
                        free(auxInstruccion);
                        string_array_destroy(vectorStringsInstruccion);
                        free(identificadorInstruccion);
                        
                        parseSuccess = false;
                        break;
                    }
                    
                    // Saco primer argumento (nombre archivo)
                    char *nombreArchivo = string_duplicate(vectorStringsInstruccion[1]);
                    string_trim(&nombreArchivo);
                    
                    // Saco segundo argumento (direccion logica)
                    char *direccionLogicaString = string_duplicate(vectorStringsInstruccion[2]);
                    string_trim(&direccionLogicaString);
                    uint32_t direccionLogica = (uint32_t) atoi(direccionLogicaString);
                    free(direccionLogicaString);

                    // Saco tercer argumento (cantidad de bytes)
                    char *cantidadBytesString = string_duplicate(vectorStringsInstruccion[3]);
                    string_trim(&cantidadBytesString);
                    uint32_t cantidadBytes = (uint32_t) atoi(cantidadBytesString);
                    free(cantidadBytesString);

                    // Empaqueto instruccion en buffer
                    consola_serializer_pack_three_args(buffer, tipoInstruccion, (void *) nombreArchivo, (void *) &direccionLogica, (void *) &cantidadBytes);
                    
                    log_info(consolaLogger, "Se empaqueta instruccion: %s con operandos %s, %d y %d", identificadorInstruccion, nombreArchivo, direccionLogica, cantidadBytes);
                    log_info(consolaDebuggingLogger, "Se empaqueta instruccion: %s con operandos %s, %d y %d", identificadorInstruccion, nombreArchivo, direccionLogica, cantidadBytes);

                    free(auxInstruccion);
                    string_array_destroy(vectorStringsInstruccion);
                    free(identificadorInstruccion);
                    free(nombreArchivo);
                    continue;
                }
                // Verifico que sea instruccion con dos argumentos
                else if (__es_instruccion_con_dos_argumentos(identificadorInstruccion)) {
                    
                    // Verifico que efectivamente tenga dos argumentos
                    if (string_array_size(vectorStringsInstruccion) != 3) {
                        log_error(consolaLogger, "La instruccion %s debe contener dos argumentos. Uso: %s <argumento1> <argumento2>", identificadorInstruccion, identificadorInstruccion);
                        log_error(consolaLogger, "La instruccion %s debe contener dos argumentos. Uso: %s <argumento1> <argumento2>", identificadorInstruccion, identificadorInstruccion);
                        
                        free(auxInstruccion);
                        string_array_destroy(vectorStringsInstruccion);
                        free(identificadorInstruccion);
                        
                        parseSuccess = false;
                        break;
                    }

                    // Chequeo que instruccion de dos argumentos es
                    switch (tipoInstruccion)
                    {
                        case INSTRUCCION_set:
                        {
                            // Saco primer argumento (registro)
                            char *registroString = string_duplicate(vectorStringsInstruccion[1]);
                            string_trim(&registroString);
                            t_registro registro1 = __obtener_registro(registroString);
                            free(registroString);

                            // Saco segundo argumento (valor)
                            char *valor = string_duplicate(vectorStringsInstruccion[2]);
                            string_trim(&valor);
                            
                            consola_serializer_pack_two_args(buffer, tipoInstruccion, (void *) &registro1, (void *) valor);
                            
                            free(valor);
                            break;
                        }
                        case INSTRUCCION_movin:
                        {
                            // Saco primer argumento (registro)
                            char *registroString = string_duplicate(vectorStringsInstruccion[1]);
                            string_trim(&registroString);
                            t_registro registro1 = __obtener_registro(registroString);
                            free(registroString);

                            // Saco segundo argumento (direccion logica)
                            char *direccionLogicaString = string_duplicate(vectorStringsInstruccion[2]);
                            string_trim(&direccionLogicaString);
                            uint32_t direccionLogica = (uint32_t) atoi(direccionLogicaString);
                            free(direccionLogicaString);

                            consola_serializer_pack_two_args(buffer, tipoInstruccion, (void *) &registro1, (void *) &direccionLogica);
                            break;
                        }
                        case INSTRUCCION_movout:
                        {
                            // Saco primer argumento (direccion logica)
                            char *direccionLogicaString = string_duplicate(vectorStringsInstruccion[1]);
                            string_trim(&direccionLogicaString);
                            uint32_t direccionLogica = (uint32_t) atoi(direccionLogicaString);
                            free(direccionLogicaString);
                            
                            // Saco segundo argumento (registro)
                            char *registroString = string_duplicate(vectorStringsInstruccion[2]);
                            string_trim(&registroString);
                            t_registro registro2 = __obtener_registro(registroString);
                            free(registroString);

                            consola_serializer_pack_two_args(buffer, tipoInstruccion, (void *) &direccionLogica, (void *) &registro2);
                            break;
                        }
                        case INSTRUCCION_fseek:
                        case INSTRUCCION_ftruncate:
                        {
                            // Saco primer argumento (nombreArchivo)
                            char *nombreArchivo = string_duplicate(vectorStringsInstruccion[1]);
                            string_trim(&nombreArchivo);
                            
                            // Saco segundo argumento (posicion o tamanio)
                            char *posicionOTamanioString = string_duplicate(vectorStringsInstruccion[2]);
                            string_trim(&posicionOTamanioString);
                            uint32_t posicionOTamanio = (uint32_t) atoi(posicionOTamanioString); 
                            free(posicionOTamanioString);

                            consola_serializer_pack_two_args(buffer, tipoInstruccion, (void *) nombreArchivo, (void *) &posicionOTamanio);
                            
                            free(nombreArchivo);
                            break;
                        }
                        case INSTRUCCION_create_segment:
                        {
                            // Saco primer argumento (id del segmento)
                            char *idSegmentoString = string_duplicate(vectorStringsInstruccion[1]);
                            string_trim(&idSegmentoString);
                            uint32_t idSegmento = (uint32_t) atoi(idSegmentoString);
                            free(idSegmentoString);
                            
                            // Saco segundo argumento (posicion o tamanio)
                            char *tamanioString = string_duplicate(vectorStringsInstruccion[2]);
                            string_trim(&tamanioString);
                            t_registro tamanio = (uint32_t) atoi(tamanioString); 
                            free(tamanioString);

                            consola_serializer_pack_two_args(buffer, tipoInstruccion, (void *) &idSegmento, (void *) &tamanio);
                            break;
                        }
                        default:
                            log_error(consolaLogger, "La instruccion %s no existe", identificadorInstruccion);
                            log_error(consolaDebuggingLogger, "La instruccion %s no existe", identificadorInstruccion);
                            
                            parseSuccess = false;
                            return parseSuccess;
                            break;
                    }

                    char *argumento1 = string_duplicate(vectorStringsInstruccion[1]);
                    char *argumento2 = string_duplicate(vectorStringsInstruccion[2]);
                    log_info(consolaLogger, "Se empaqueta instruccion: %s con operandos %s y %s", identificadorInstruccion, argumento1, argumento2);
                    log_info(consolaDebuggingLogger, "Se empaqueta instruccion: %s con operandos %s y %s", identificadorInstruccion, argumento1, argumento2);
                    
                    free(argumento1);
                    free(argumento2);
                    free(auxInstruccion);
                    string_array_destroy(vectorStringsInstruccion);
                    free(identificadorInstruccion);
                    continue;
                }
                // Verifico que sea instruccion con un argumento
                else if (__es_instruccion_con_un_argumento(identificadorInstruccion)) {
                    
                    // Verifico que efectivamente tenga un argumento
                    if (string_array_size(vectorStringsInstruccion) != 2) {
                        log_error(consolaLogger, "La instruccion %s debe contener dos argumentos. Uso: %s <argumento1>", identificadorInstruccion, identificadorInstruccion);
                        log_error(consolaLogger, "La instruccion %s debe contener dos argumentos. Uso: %s <argumento1>", identificadorInstruccion, identificadorInstruccion);
                        
                        free(auxInstruccion);
                        string_array_destroy(vectorStringsInstruccion);
                        free(identificadorInstruccion);
                        
                        parseSuccess = false;
                        break;
                    }

                    // Chequeo que instruccion de dos argumentos es
                    switch (tipoInstruccion)
                    {
                        case INSTRUCCION_io:
                        {
                            // Saco primer argumento (tiempo)
                            char *tiempoString = string_duplicate(vectorStringsInstruccion[1]);
                            string_trim(&tiempoString);
                            uint32_t tiempo = (uint32_t) atoi(tiempoString);
                            free(tiempoString);
                            
                            consola_serializer_pack_one_integer_args(buffer, tipoInstruccion, tiempo);
                            break;
                        }
                        case INSTRUCCION_fopen:
                        case INSTRUCCION_fclose:
                        {
                            // Saco primer argumento (nombre archivo)
                            char *nombreArchivo = string_duplicate(vectorStringsInstruccion[1]);
                            string_trim(&nombreArchivo);

                            consola_serializer_pack_one_string_args(buffer, tipoInstruccion, nombreArchivo);

                            free(nombreArchivo);
                            break;
                        }
                        case INSTRUCCION_wait:
                        case INSTRUCCION_signal:
                        {
                            // Saco primer argumento (recurso)
                            char *recurso = string_duplicate(vectorStringsInstruccion[1]);
                            string_trim(&recurso);

                            consola_serializer_pack_one_string_args(buffer, tipoInstruccion, recurso);

                            free(recurso);
                            break;
                        }
                        case INSTRUCCION_delete_segment:
                        {
                            // Saco primer argumento (id segmento)
                            char *idSegmentoString = string_duplicate(vectorStringsInstruccion[1]);
                            string_trim(&idSegmentoString);
                            uint32_t idSegmento = (uint32_t) atoi(idSegmentoString);
                            free(idSegmentoString);
                            
                            consola_serializer_pack_one_integer_args(buffer, tipoInstruccion, idSegmento);
                            break;
                        }
                        default:
                            log_error(consolaLogger, "La instruccion %s no existe", identificadorInstruccion);
                            log_error(consolaDebuggingLogger, "La instruccion %s no existe", identificadorInstruccion);
                            
                            parseSuccess = false;
                            return parseSuccess;
                            break;
                    }

                    char *argumento1 = string_duplicate(vectorStringsInstruccion[1]);
                    log_info(consolaLogger, "Se empaqueta instruccion: %s con operando %s", identificadorInstruccion, argumento1);
                    log_info(consolaDebuggingLogger, "Se empaqueta instruccion: %s con operando %s", identificadorInstruccion, argumento1);
                    
                    free(argumento1);
                    free(auxInstruccion);
                    string_array_destroy(vectorStringsInstruccion);
                    free(identificadorInstruccion);
                    continue;
                }
                // Instruccion sin argumentos
                else {
                    
                    // Verifico que efectivamente tenga 0 argumentos
                    if(string_array_size(vectorStringsInstruccion) > 1) {
                        log_error(consolaLogger, "La instruccion %s no debe contener ningun argumento. Uso: %s", identificadorInstruccion, identificadorInstruccion);
                        log_error(consolaDebuggingLogger, "La instruccion %s no debe contener ningun argumento. Uso: %s", identificadorInstruccion, identificadorInstruccion);
                        
                        free(auxInstruccion);
                        string_array_destroy(vectorStringsInstruccion);
                        free(identificadorInstruccion);
                        
                        parseSuccess = false;
                        break;
                    }

                    consola_serializer_pack_no_args(buffer, tipoInstruccion);
                    log_info(consolaLogger, "Se empaqueta la instruccion: %s", identificadorInstruccion);
                    
                    free(auxInstruccion);
                    string_array_destroy(vectorStringsInstruccion);
                    free(identificadorInstruccion);
                    
                    if (tipoInstruccion == INSTRUCCION_yield) continue;
                    
                    parseSuccess = true;
                    break;

                }
            }
            else{
                // Error por instruccion no valida (fuera del set de instrucciones)
                log_error(consolaLogger, "Instruccion %s no reconocida. Fuera del set de instrucciones", identificadorInstruccion);
                log_error(consolaDebuggingLogger, "Instruccion %s no reconocida. Fuera del set de instrucciones", identificadorInstruccion);
                free(auxInstruccion);
                string_array_destroy(vectorStringsInstruccion);
                free(identificadorInstruccion);
                parseSuccess = false;
                break;
            }
        }
        else {
            // Error de lectura de archivo instrucciones
            log_error(consolaLogger, "Error al leer el archivo %s de instrucciones: %s", pathInstrucciones, strerror(errno));
            log_error(consolaDebuggingLogger, "Error al leer el archivo %s de instrucciones: %s", pathInstrucciones, strerror(errno));
            free(instruccion);
            parseSuccess = false;
            break;
        }

    }

    fclose(archivoInstrucciones);
    return parseSuccess;
}

/*
switch (obtener_tipo_instruccion(identificadorInstruccion))
                    {
                        case INSTRUCCION_set:
                        case INSTRUCCION_movin:

                            arg1 = string_duplicate(vectorStringsInstruccion[1]);
                            string_trim(&arg1);
                            auxRegistro1 = obtener_registro(arg1);
                            arg2 = string_duplicate(vectorStringsInstruccion[2]);
                            string_trim(&arg2);
                            op2 = (uint32_t) atoi(arg2);
                            consola_serializer_pack_two_args(buffer, obtener_tipo_instruccion(identificadorInstruccion), (void*)&auxRegistro1, (void*)&op2);
                            break;
                        
                        case INSTRUCCION_add:
                            
                            arg1 = string_duplicate(vectorStringsInstruccion[1]);
                            string_trim(&arg1);
                            auxRegistro1 = obtener_registro(arg1);
                            arg2 = string_duplicate(vectorStringsInstruccion[2]);
                            string_trim(&arg2);
                            auxRegistro2 = obtener_registro(arg2);
                            consola_serializer_pack_two_args(buffer, obtener_tipo_instruccion(identificadorInstruccion), (void*)&auxRegistro1, (void*)&auxRegistro2);
                            break;

                        case INSTRUCCION_movout:
                            
                            arg1 = string_duplicate(vectorStringsInstruccion[1]);
                            string_trim(&arg1);
                            op1 = (uint32_t) atoi(arg1);
                            arg2 = string_duplicate(vectorStringsInstruccion[2]);
                            string_trim(&arg2);
                            auxRegistro2 = obtener_registro(arg2);
                            consola_serializer_pack_two_args(buffer, obtener_tipo_instruccion(identificadorInstruccion), (void*)&op1, (void*)&auxRegistro2);
                            break;
                        
                        case INSTRUCCION_io:
                            
                            if(instruccion_io_es_pantalla_o_teclado(identificadorInstruccion)) {

                                arg1 = string_duplicate(vectorStringsInstruccion[1]);
                                string_trim(&arg1);
                                arg2 = string_duplicate(vectorStringsInstruccion[2]);
                                string_trim(&arg2);
                                auxRegistro2 = obtener_registro(arg2);
                                consola_serializer_pack_two_args(buffer, obtener_tipo_instruccion(identificadorInstruccion), (void*)arg1, (void*)&auxRegistro2);
                            }
                            else {

                                arg1 = string_duplicate(vectorStringsInstruccion[1]);
                                string_trim(&arg1);
                                arg2 = string_duplicate(vectorStringsInstruccion[2]);
                                string_trim(&arg2);
                                op2 = (uint32_t) atoi(arg2);
                                consola_serializer_pack_two_args(buffer, obtener_tipo_instruccion(identificadorInstruccion), (void*)arg1, (void*)&op2);
                            }
                            break;
                        
                        default:
                            break;
                        }
*/