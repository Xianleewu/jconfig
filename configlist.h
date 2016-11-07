#include "stdbool.h"
#include "jconfig.h"
/*
You can add your own config items below.
And this array must end of with {NULL, NULL, -1, NULL, false, 0}
*/

#define ROOT_NAME_VIDEO	"video"
#define ROOT_NAME_AUDIO	"audio"

const configItem itemList[] =
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
