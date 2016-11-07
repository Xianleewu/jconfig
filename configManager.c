#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <fcntl.h>  
#include "malloc.h"
#include "cjson/cJSON.h"
#include "jconfig.h"
#include "configManager.h"

configManager *gConfigManager = NULL;

configManager *initConfigManager()
{
	configManager *manager;
	
	if(gConfigManager == NULL)
	{
		manager = (configManager*) malloc(sizeof(configManager));
		
		if(manager == NULL)
		{
			return NULL;
		}
		
		if(access(CONFIG_FILE_PATH_BACK, F_OK) != -1)
		{
			manager->config = loadLocalConf(CONFIG_FILE_PATH_BACK);
		}
		else if(access(CONFIG_FILE_PATH, F_OK) != -1)
		{
			manager->config = loadLocalConf(CONFIG_FILE_PATH);
		}
		else
		{
			manager->config = loadDefConfig();
		}
		
		gConfigManager = manager;
	}

	return gConfigManager;	
}

configManager *getConfigManager()
{
	return initConfigManager();
}

int delConfigManager(configManager *manager)
{
	if(manager == NULL)
	{
		return -1;
	}
	
	if(manager->config)
	{
		cJSON_Delete(manager->config);
	}
	
	free(manager);
	
	return 0;
}

int saveConfigFiles(configManager *manager)
{
	int ret = -1;
	
	if(manager == NULL)
	{
		return ret;
	}
	
	pthread_rwlock_wrlock(&manager->rwlock);
	
	ret = saveConfig(manager->config, CONFIG_FILE_PATH_BACK);
	ret = saveConfig(manager->config, CONFIG_FILE_PATH);
	ret = remove(CONFIG_FILE_PATH_BACK);
	
	pthread_rwlock_unlock(&manager->rwlock);
	
	return ret;	
}

int getItemValue(char *root, char *name, configManager *manager)
{
	return getConfigValue(root, name, manager->config);
}

char *getItemStr(char *root, char *name, configManager *manager)
{
	return getConfigStr(root, name, manager->config);
}

int setItemValue(char *root, char *name, configManager *manager, int value)
{
	return setConfigValue(root, name, manager->config, value);
}

int setItemStr(char *root, char *name, configManager *manager, char *str)
{
	return setConfigStr(root, name, manager->config, str);
}
