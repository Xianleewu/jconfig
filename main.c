#include <stdio.h>
#include <stdlib.h>
#include "configManager.h"

int  main()
{
	configManager *manager = NULL;
	
	manager = initConfigManager();
	
	printf("config manager init ok \n");
	
	saveConfigFiles(manager);
	
	printf("save config manager ok \n");
	
	delConfigManager(manager);
	
	return 0;
}
