#include "stdio.h"
#include "stdlib.h"
#include "malloc.h"
#include "cjson/cJSON.h"
#include "jconfig.h"

configManager gConfigManager;
cJSON *loadLocalConf(char *filename);
cJSON *loadDefConfig(void);
int saveConfig(cJSON *json, char *filename);

int  main()
{
	cJSON *root = NULL;
	
	printf("reading config info from %s \n", CONFIG_FILE_PATH);
	
	if((root = loadDefConfig()) == NULL)
	{
		printf("load default config failed");
		return -1;
	}
	
	saveConfig(root, "demo.json");
	
	cJSON_Delete(root);
	
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