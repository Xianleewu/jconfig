#ifndef _JCONFIG_H_
#define _JCONFIG_H_

#include "stdbool.h"
#include "sys/file.h"
#include "cjson/cJSON.h"

#define DEBUG

#ifndef DEBUG

#define CONFIG_FILE_PATH		("/data/config/config.json")
#define CONFIG_FILE_PATH_BACK	("/data/config/config.json~")

#else

#define CONFIG_FILE_PATH        ("config.json")
#define CONFIG_FILE_PATH_BACK	("config.json~")

#endif

#define TYPE_STRING 0
#define TYPE_NUMBER 1
#define TYPE_BOOL 2

typedef struct _configItem
{
	char *root;		//parent node
	char *name;		//obj name
	char type;		//value type:number char bool
	char *string;	//string value
	int  value;		//number and bool value depends "type"
}configItem;

/*
You can add your own config items below.
And this array must end of with {NULL, NULL, -1, NULL, false, 0} 
*/

configItem itemList[] =
{
    {"video", "width", TYPE_NUMBER, NULL, 1280},
    {"video", "height", TYPE_NUMBER, NULL, 720},
    {"video", "quality", TYPE_STRING, "720P", 0},
    {"video", "fps", TYPE_NUMBER, NULL, 30},
    {"video", "format", TYPE_STRING, "H264", 0},
	{"video", "bitrate", TYPE_NUMBER, NULL, 1024},

    {"audio", "bitrate", TYPE_NUMBER, NULL, 1024},
    {"audio", "format", TYPE_STRING, "AVC", 0},

	{NULL, "motion", TYPE_BOOL, NULL, false},

    {NULL, NULL, -1, NULL, false}
};

typedef struct _configManager
{
	FILE *fd;
	cJSON *config;
}configManager;

extern configManager gConfigManager;

configManager *loadConfigFile(char *filename);
configManager *initConfigManager(void);
configManager *getConfigManager(void);
int delConfigManager(configManager *manager);
int savConfigFiles(configManager *manager, char *filename);

int getConfigItemValue(char *item);
int setConfigItemValue(char *item, void *value);






#endif
