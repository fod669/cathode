
#include "cathode.h"

int cth_main(Arena* arena, int argc, str8* argv)
{
	logi("Hello world!\n");
	for (int i = 0; i < argc; ++i)
	{
		logi("Args:\n");
		logi("[%d] \"%s\"\n", i, argv[i].str);
	}
	return EXIT_SUCCESS;
}

