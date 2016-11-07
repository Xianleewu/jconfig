#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include "cjson/cJSON.h"

typedef struct _configManager
{
	cJSON *config;
	pthread_rwlock_t rwlock;
}configManager;

extern configManager *gConfigManager;

configManager *initConfigManager(void);
configManager *getConfigManager(void);
int delConfigManager(configManager *manager);
int saveConfigFiles(configManager *manager);

#endif