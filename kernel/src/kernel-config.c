#include <kernel-config.h>

// Funciones privadas

// Funcion utilizada para la instanciacion de la configuracion del kernel
static void __kernel_config_initializer(void *moduleConfig, t_config *tempCfg) 
{
    t_kernel_config *kernelConfig = (t_kernel_config *) moduleConfig;

    kernelConfig->IP_MEMORIA = strdup(config_get_string_value(tempCfg, "IP_MEMORIA"));
    kernelConfig->PUERTO_MEMORIA = strdup(config_get_string_value(tempCfg, "PUERTO_MEMORIA"));
    kernelConfig->IP_FILESYSTEM = strdup(config_get_string_value(tempCfg, "IP_FILESYSTEM"));
    kernelConfig->PUERTO_FILESYSTEM = strdup(config_get_string_value(tempCfg, "PUERTO_FILESYSTEM"));
    kernelConfig->IP_CPU = strdup(config_get_string_value(tempCfg, "IP_CPU"));
    kernelConfig->PUERTO_CPU = strdup(config_get_string_value(tempCfg, "PUERTO_CPU"));
    kernelConfig->PUERTO_ESCUCHA = strdup(config_get_string_value(tempCfg, "PUERTO_ESCUCHA"));
    kernelConfig->ALGORITMO_PLANIFICACION = strdup(config_get_string_value(tempCfg, "ALGORITMO_PLANIFICACION"));
    kernelConfig->ESTIMACION_INICIAL = (uint32_t) config_get_int_value(tempCfg, "ESTIMACION_INICIAL");
    kernelConfig->HRRN_ALFA = config_get_double_value(tempCfg, "HRRN_ALFA");
    kernelConfig->GRADO_MAX_MULTIPROGRAMACION = (uint32_t) config_get_int_value(tempCfg, "GRADO_MAX_MULTIPROGRAMACION");
    kernelConfig->RECURSOS = config_get_array_value(tempCfg, "RECURSOS");
    kernelConfig->INSTANCIAS_RECURSOS = config_get_array_value(tempCfg, "INSTANCIAS_RECURSOS");

    return;
}

//Funciones publicas
t_kernel_config *kernel_config_create(char *kernelConfigPath, t_log *kernelLogger)
{
    t_kernel_config *self = malloc(sizeof(*self));
    
    //Chequeo que la inicializacion de la configuracion del kernel sea correcta
    int inicializacionCorrecta = config_init(self, kernelConfigPath, kernelLogger, __kernel_config_initializer);
    if(inicializacionCorrecta == -1) {
        exit(EXIT_FAILURE);
    }
    
    return self;
}

void kernel_config_destroy(t_kernel_config *self) 
{
    free(self->IP_MEMORIA);
    free(self->PUERTO_MEMORIA);
    free(self->IP_FILESYSTEM);
    free(self->PUERTO_FILESYSTEM);
    free(self->IP_CPU);
    free(self->PUERTO_CPU);
    free(self->PUERTO_ESCUCHA);
    free(self->ALGORITMO_PLANIFICACION);
    // free(self-> ESTIMACION_INICIAL);       Estos 3 no los liberamos porque son ints
    // free(self-> HRRN_ALFA);
    // free(self-> GRADO_MAX_MULTIPROGRAMACION);
    string_array_destroy(self->RECURSOS);
    string_array_destroy(self->INSTANCIAS_RECURSOS);
    free(self);

    return;
}

char *kernel_config_get_ip_memoria(t_kernel_config *self) 
{
    return self->IP_MEMORIA;
}

char *kernel_config_get_puerto_memoria(t_kernel_config *self)
{
    return self->PUERTO_MEMORIA;
}


char *kernel_config_get_ip_filesystem(t_kernel_config *self)
{
    return self->IP_FILESYSTEM;
}


char *kernel_config_get_puerto_filesystem(t_kernel_config *self)
{
    return self->PUERTO_FILESYSTEM;
}


char *kernel_config_get_ip_cpu(t_kernel_config *self)
{
    return self->IP_CPU;
}


char *kernel_config_get_puerto_cpu(t_kernel_config *self)
{
    return self->PUERTO_CPU;
}

char *kernel_config_get_puerto_escucha(t_kernel_config *self)
{
    return self->PUERTO_ESCUCHA;
}

char *kernel_config_get_algoritmo_planificacion(t_kernel_config *self)
{
    return self->ALGORITMO_PLANIFICACION;
}


uint32_t kernel_config_get_estimacion_inicial(t_kernel_config *self)
{
    return self->ESTIMACION_INICIAL;
}

double kernel_config_get_hrrn_alfa(t_kernel_config *self)
{
    return self->HRRN_ALFA;
}

uint32_t kernel_config_get_grado_max_multiprogramacion(t_kernel_config *self)
{
    return self->GRADO_MAX_MULTIPROGRAMACION;
}

char **kernel_config_get_recursos(t_kernel_config *self)
{
    return self->RECURSOS;
}

char **kernel_config_get_instancias_recursos(t_kernel_config *self)
{
    return self->INSTANCIAS_RECURSOS;
}

int kernel_config_get_socket_memoria(t_kernel_config *self)
{
    return self->SOCKET_MEMORIA;
}

int kernel_config_get_socket_cpu(t_kernel_config *self)
{
    return self->SOCKET_CPU;
}

int kernel_config_get_socket_filesystem(t_kernel_config *self)
{
    return self->SOCKET_FILESYSTEM;
}

void kernel_config_set_socket_memoria(t_kernel_config *self, int socketMemoria)
{
    self->SOCKET_MEMORIA = socketMemoria;
}

void kernel_config_set_socket_cpu(t_kernel_config *self, int socketCpu)
{
    self->SOCKET_CPU = socketCpu;
}

void kernel_config_set_socket_filesystem(t_kernel_config *self, int socketFilesystem)
{
    self->SOCKET_FILESYSTEM = socketFilesystem;
}
