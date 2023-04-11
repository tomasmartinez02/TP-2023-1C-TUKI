#include <cpu-config.h>

// Funciones privadas

// Función utilizada para la instanciación de la configuración de la consola
static void __cpu_config_initializer(void *moduleConfig, t_config *tempCfg)
{
    t_cpu_config *cpuConfig = (t_cpu_config *) moduleConfig;

    cpuConfig->RETARDO_INSTRUCCION = config_get_int_value(tempCfg, "RETARDO_INSTRUCCION");
    cpuConfig->IP_MEMORIA = strdup(config_get_string_value(tempCfg, "IP_MEMORIA"));
    cpuConfig->PUERTO_MEMORIA = config_get_int_value(tempCfg, "PUERTO_MEMORIA");
    cpuConfig->PUERTO_ESCUCHA = config_get_int_value(tempCfg, "PUERTO_ESCUCHA");
    cpuConfig->TAM_MAX_SEGMENTO = config_get_int_value(tempCfg, "TAM_MAX_SEGMENTO");

    return;
}

// Funciones públicas
t_cpu_config *cpu_config_create(char *cpuConfigPath, t_log *cpuLogger)
{
    t_cpu_config *self = malloc(sizeof(*self));

    int inicializacionCorrecta = config_init(self, cpuConfigPath, cpuLogger, __cpu_config_initializer);
    if (inicializacionCorrecta == -1) {
        exit(EXIT_FAILURE);
        //no habria q loggear algo aca? idk
    }

    return self;

}

void consola_config_destroy(t_cpu_config *self)
{   
    free(self->IP_MEMORIA);
    free(self);

    return;
}

int32_t cpu_config_get_retardo_instruccion(t_cpu_config *self)
{
    return self->RETARDO_INSTRUCCION;
}

char* cpu_config_get_ip_memoria(t_cpu_config *self)
{
    return self->IP_MEMORIA;
}

int32_t cpu_config_get_puerto_escucha(t_cpu_config *self)
{
    return self->PUERTO_ESCUCHA;
}

int32_t cpu_config_get_puerto_memoria(t_cpu_config *self)
{
    return self->PUERTO_MEMORIA;
}

int32_t cpu_config_get_tam_max_segmento(t_cpu_config *self)
{
    return self->TAM_MAX_SEGMENTO;
}