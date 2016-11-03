#include "stdio.h"
#include "stdlib.h"
#include "malloc.h"
#include "cjson/cJSON.h"
#include "jconfig.h"

configManager gConfigManager;
void doit(char *text);
void dofile(char *filename);
int savefile(char *filename);

int  main()
{
	printf("reading config info from %s \n", CONFIG_FILE_PATH);	
	
	dofile(CONFIG_FILE_PATH);
	
	savefile("demo.json");
	
	return 0;
}

/* Parse text to JSON, then render back to text, and print! */
void doit(char *text)
{
    char *out;
    cJSON *json;

    json = cJSON_Parse(text);
    if (!json)
    {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    }
    else
    {
        out = cJSON_Print(json);
        cJSON_Delete(json);
        printf("%s\n", out);
        free(out);
    }
}


/* Read a file, parse, render back, etc. */
void dofile(char *filename)
{
    FILE *f;
    long len;
    char *data;

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

    doit(data);
    free(data);
}

int savefile(char *filename)
{
	FILE *f = NULL;
	char *data = NULL;
	unsigned int i = 0;	

	cJSON *root,*fmt;

	
	//f = fopen(filename, "w+");
	
	root  = cJSON_CreateObject();

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
				return -1;
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
                printf("waring:Could not recognize value: %d \n",itemList[i].type);
            }
		}

		i++;
	}
	
	data = cJSON_Print(root);
	cJSON_Delete(root);
	printf("%s\n",data);
	free(data);

	return 0;
}
