#ifndef CONFIG_FS_H_
#define CONFIG_FS_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <commons/string.h>
#include <commons/config.h>

typedef struct ConfigType{
	uint32_t PUERTO_FILESYSTEM;
	char* PUNTO_MONTAJE;
	uint32_t CANTCONEXIONES;
} Type_Config;



Type_Config load_config(char* path);
void print_config(Type_Config auxConfig);


#endif /* CONFIG_FS_H_ */
