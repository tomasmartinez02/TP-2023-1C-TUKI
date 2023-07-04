#include <memoria-config.h>

// Funciones privadas

// Funcion utilizada para la instanciacion de la configuracion de la memoria
static void __memoria_config_initializer(void *moduleConfig, t_config *tempCfg) 
{
    t_memoria_config *memoriaConfig = (t_memoria_config *) moduleConfig;

    memoriaConfig->IP_ESCUCHA = strdup(config_get_string_value(tempCfg, "IP_ESCUCHA"));
    memoriaConfig->PUERTO_ESCUCHA = strdup(config_get_string_value(tempCfg, "PUERTO_ESCUCHA"));
    memoriaConfig->TAM_MEMORIA = (uint32_t) config_get_int_value(tempCfg, "TAM_MEMORIA");
    memoriaConfig->TAM_SEGMENTO_0 = (uint32_t) config_get_int_value(tempCfg, "TAM_SEGMENTO_0");
    memoriaConfig->CANT_SEGMENTOS = (uint32_t) config_get_int_value(tempCfg, "CANT_SEGMENTOS");
    memoriaConfig->RETARDO_MEMORIA = (uint32_t) config_get_int_value(tempCfg, "RETARDO_MEMORIA");
    memoriaConfig->RETARDO_COMPACTACION = (uint32_t) config_get_int_value(tempCfg, "RETARDO_COMPACTACION");
    memoriaConfig->ALGORITMO_ASIGNACION = strdup(config_get_string_value(tempCfg, "ALGORITMO_ASIGNACION"));

    return;
}

// Funciones publicas
t_memoria_config *memoria_config_create(char *memoriaConfigPath, t_log *memoriaLogger) 
{
    t_memoria_config *self = malloc(sizeof(*self));
    
    //Chequeo que la inicializacion de la configuracion de la memoria sea correcta
    int inicializacionCorrecta = config_init(self, memoriaConfigPath, memoriaLogger, __memoria_config_initializer);
    if(inicializacionCorrecta == -1){
        exit(EXIT_FAILURE);
    }
    
    return self;
}

void memoria_config_destroy(t_memoria_config *self) 
{
    free(self->IP_ESCUCHA);
    free(self->PUERTO_ESCUCHA);
    free(self->ALGORITMO_ASIGNACION);
    free(self);

    return;
}

char *memoria_config_get_ip_escucha(t_memoria_config *self) 
{
    return self->IP_ESCUCHA;
}

char *memoria_config_get_puerto_escucha(t_memoria_config *self) 
{
    return self->PUERTO_ESCUCHA;
}

uint32_t memoria_config_get_tamanio_memoria(t_memoria_config *self)
{
    return self->TAM_MEMORIA;
}

uint32_t memoria_config_get_tamanio_segmento_cero(t_memoria_config *self)
{
    return self->TAM_SEGMENTO_0;
}

uint32_t memoria_config_get_cantidad_segmentos(t_memoria_config *self)
{
    return self->CANT_SEGMENTOS;
}

uint32_t memoria_config_get_retardo_memoria(t_memoria_config *self)
{
    return self->RETARDO_MEMORIA;
}

uint32_t memoria_config_get_retardo_compactacion(t_memoria_config *self)
{
    return self->RETARDO_COMPACTACION;
}

char *memoria_config_get_algoritmo_asignacion(t_memoria_config *self)
{
    return self->ALGORITMO_ASIGNACION;
}

int memoria_config_get_socket_cpu(t_memoria_config *self)
{
    return self->SOCKET_CPU;
}

int memoria_config_get_socket_kernel(t_memoria_config *self)
{
    return self->SOCKET_KERNEL;
}

int memoria_config_get_socket_filesystem(t_memoria_config *self)
{
    return self->SOCKET_FILESYSTEM;
}

void memoria_config_set_socket_cpu(t_memoria_config *self, int socketCpu)
{
    self->SOCKET_CPU = socketCpu;
}

void memoria_config_set_socket_kernel(t_memoria_config *self, int socketKernel)
{
    self->SOCKET_KERNEL = socketKernel;
}

void memoria_config_set_socket_filesystem(t_memoria_config *self, int socketFilesystem)
{
    self->SOCKET_FILESYSTEM = socketFilesystem;
}
