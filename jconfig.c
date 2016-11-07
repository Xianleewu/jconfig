#include "stdio.h"
#include "stdlib.h"
#include "malloc.h"
#include "cjson/cJSON.h"
#include "jconfig.h"

configManager *gConfigManager = NULL;
cJSON *loadLocalConf(char *filename);
cJSON *loadDefConfig(void);
int saveConfig(cJSON *json, char *filename);

int getConfigValue(char *root, char *name, cJSON *json);
char *getConfigStr(char *root, char *name, cJSON *json);

int setConfigValue(char *root, char *name, cJSON *json, int value);
int setConfigStr(char *root, char *name, cJSON *json, char *str);

int  main()
{
	cJSON *root = NULL;
	char *tmp = NULL;
	configManager *manager = NULL;
	
	manager = initConfigManager();
	
	printf("config manager init ok \n");
	
	saveConfigFiles(manager);
	
	printf("save config manager ok \n");
	
	delConfigManager(manager);
	
	return 0;
}

/* Read a file, parse, render back, etc. */
cJSON *loadLocalConf(char *filename)
{
	FILE *f;
	long len;
	char *data;
	cJSON *json;

	/* open in read binary mode */
	f = fopen(filename,"rb");
	/* get the length */
	fseek(f, 0, SEEK_END);
	len = ftell(f);
	fseek(f, 0, SEEK_SET);

	data = (char*)malloc(len + 1);

	fread(data, 1, len, f);
	data[len] = '\0';
	fclose(f);

	json = cJSON_Parse(data);
	free(data);

	return json;
}

cJSON *loadDefConfig(void)
{
	unsigned int i = 0;

	cJSON *root;

	root  = cJSON_CreateObject();
	/*
	Read objects from config array until NULL
	*/
	while(itemList[i].root || itemList[i].name)
	{
		if(itemList[i].root && itemList[i].name)
		{
			cJSON *tmpJSON = NULL;
			tmpJSON = cJSON_GetObjectItem(root, itemList[i].root);
			if(!tmpJSON)
			{
				cJSON_AddItemToObject(root, itemList[i].root, tmpJSON = cJSON_CreateObject());
			}

			if(!tmpJSON)
			{
				printf("Error: creat json object failed \n");
				return NULL;
			}

			if(itemList[i].type == TYPE_STRING)
			{
				cJSON_AddStringToObject(tmpJSON, itemList[i].name, itemList[i].string);
			}
			else if(itemList[i].type == TYPE_NUMBER)
			{
				cJSON_AddNumberToObject(tmpJSON, itemList[i].name, itemList[i].value);
			}
			else if(itemList[i].type == TYPE_BOOL)
			{
				cJSON_AddBoolToObject(tmpJSON, itemList[i].name, itemList[i].value);
			}
			else
			{
				printf("waring:Could not recognize value: %d \n",itemList[i].type);
			}
		}
		else
		{
			if(itemList[i].type == TYPE_STRING)
			{
				cJSON_AddStringToObject(root, itemList[i].name, itemList[i].string);
			}
			else if(itemList[i].type == TYPE_NUMBER)
			{
				cJSON_AddNumberToObject(root, itemList[i].name, itemList[i].value);
			}
			else if(itemList[i].type == TYPE_BOOL)
			{
				cJSON_AddBoolToObject(root, itemList[i].name, itemList[i].value);
			}
			else
			{
				printf("waring:Could not recognize value type: %d \n",itemList[i].type);
			}
		}
		i++;
	}

	return root;
}

int saveConfig(cJSON *json, char *filename)
{
	FILE *f = NULL;
	char *data = NULL;
	unsigned int i = 0;

	f = fopen(filename, "w+");

	if(f == NULL)
	{
		printf("can not open file %s \n", filename);
		return -1;
	}

	data = cJSON_Print(json);
	printf("%s\n", data);
	fprintf(f, "%s", data);
	free(data);
	fclose(f);

	return 0;
}

int getConfigValue(char *root, char *name, cJSON *json)
{
	cJSON *tmpJSON = json;

	if(root != NULL)
	{
		tmpJSON = cJSON_GetObjectItem(json, root);
	}

	if(tmpJSON == NULL)
	{
		printf("no valuable json \n");

		return -1;
	}

	return cJSON_GetObjectItem(tmpJSON, name)->valueint;
}

char *getConfigStr(char *root, char *name, cJSON *json)
{
	cJSON *tmpJSON = json;

	if(root != NULL)
	{
		tmpJSON = cJSON_GetObjectItem(json, root);
	}

	if(tmpJSON == NULL)
	{
		printf("no valuable json \n");

		return NULL;
	}

	return cJSON_GetObjectItem(tmpJSON, name)->valuestring;
}

int setConfigValue(char *root, char *name, cJSON *json, int value)
{
	cJSON *tmpJSON = json;

	if(root != NULL)
	{
		tmpJSON = cJSON_GetObjectItem(json, root);
	}

	if(tmpJSON == NULL)
	{
		printf("no valuable json \n");

		return -1;
	}

	if(cJSON_GetObjectItem(tmpJSON, name)->type == cJSON_Number)
	{
		cJSON_ReplaceItemInObject(tmpJSON, name, cJSON_CreateNumber(value));
	}
	else
	{
		cJSON_ReplaceItemInObject(tmpJSON, name, cJSON_CreateBool(value));
	}

	return 0;
}

int setConfigStr(char *root, char *name, cJSON *json, char *str)
{
	cJSON *tmpJSON = json;

	if(root != NULL)
	{
		tmpJSON = cJSON_GetObjectItem(json, root);
	}

	if(tmpJSON == NULL)
	{
		printf("no valuable json \n");

		return -1;
	}

	cJSON_ReplaceItemInObject(tmpJSON, name, cJSON_CreateString(str));

	return 0;
}

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