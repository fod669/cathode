
#include "cathode.h"

int cth_main(Arena* arena, int argc, str8* argv)
{
	log_info("Hello world!\n");
	for (int i = 0; i < argc; ++i)
	{
		log_info("Args:\n");
		log_info("[%d] \"%s\"\n", i, argv[i].str);
	}
	return EXIT_SUCCESS;
}

