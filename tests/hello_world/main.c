
#include "cathode.h"

int cth_main(Arena* arena, int argc, str8_c argv[])
{
	log_info("Hello world!\n");
	log_warning("Hello world!\n");
	log_error("Hello world!\n");
	log_critical("Hello world!\n");

	log_info("Args:\n");
	for (int i = 0; i < argc; ++i)
	{
		log_info("[%d] [%P]\n", i, argv[i]);
	}
	return EXIT_SUCCESS;
}

