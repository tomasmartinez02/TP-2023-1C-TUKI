#include <cpu-config.h>

// Funciones privadas

// Función utilizada para la instanciación de la configuración de la consola
static void __cpu_config_initializer(void *moduleConfig, t_config *tempCfg)
{
    t_cpu_config *cpuConfig = (t_cpu_config *) moduleConfig;

    cpuConfig->RETARDO_INSTRUCCION = (uint32_t) config_get_int_value(tempCfg, "RETARDO_INSTRUCCION");
    cpuConfig->IP_MEMORIA = strdup(config_get_string_value(tempCfg, "IP_MEMORIA"));
    cpuConfig->IP_ESCUCHA = strdup(config_get_string_value(tempCfg, "IP_ESCUCHA"));
    cpuConfig->PUERTO_MEMORIA = strdup(config_get_string_value(tempCfg, "PUERTO_MEMORIA"));
    cpuConfig->PUERTO_ESCUCHA = strdup(config_get_string_value(tempCfg, "PUERTO_ESCUCHA"));
    cpuConfig->TAM_MAX_SEGMENTO = (uint32_t) config_get_int_value(tempCfg, "TAM_MAX_SEGMENTO");
    cpuConfig->SOCKET_MEMORIA = -1;
    cpuConfig->SOCKET_KERNEL = -1;

    return;
}

// Funciones públicas
t_cpu_config *cpu_config_create(char *cpuConfigPath, t_log *cpuLogger)
{
    t_cpu_config *self = malloc(sizeof(*self));

    int inicializacionCorrecta = config_init(self, cpuConfigPath, cpuLogger, __cpu_config_initializer);
    if (inicializacionCorrecta == -1) {
        exit(EXIT_FAILURE);
    }

    return self;

}

void cpu_config_destroy(t_cpu_config *self)
{   
    free(self->IP_MEMORIA);
    free(self->IP_ESCUCHA);
    free(self->PUERTO_MEMORIA);
    free(self->PUERTO_ESCUCHA);

    close(self->SOCKET_MEMORIA);
    close(self->SOCKET_KERNEL);

    free(self);

    return;
}

uint32_t cpu_config_get_retardo_instruccion(t_cpu_config *self)
{
    return self->RETARDO_INSTRUCCION;
}

char *cpu_config_get_ip_memoria(t_cpu_config *self)
{
    return self->IP_MEMORIA;
}

char *cpu_config_get_ip_escucha(t_cpu_config *self)
{
    return self->IP_ESCUCHA;
}

char *cpu_config_get_puerto_escucha(t_cpu_config *self)
{
    return self->PUERTO_ESCUCHA;
}

char *cpu_config_get_puerto_memoria(t_cpu_config *self)
{
    return self->PUERTO_MEMORIA;
}

uint32_t cpu_config_get_tam_max_segmento(t_cpu_config *self)
{
    return self->TAM_MAX_SEGMENTO;
}

int cpu_config_get_socket_memoria(t_cpu_config *self)
{
    return self->SOCKET_MEMORIA;
}

int cpu_config_get_socket_kernel(t_cpu_config *self)
{
    return self->SOCKET_KERNEL;
}

void cpu_config_set_socket_memoria(t_cpu_config *self, int socketMemoria)
{
    self->SOCKET_MEMORIA = socketMemoria;
}

void cpu_config_set_socket_kernel(t_cpu_config *self, int socketKernel)
{
    self->SOCKET_KERNEL = socketKernel;
}