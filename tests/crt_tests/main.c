
#include "cathode.h"

int cth_main(int argc, str8_const argv[])
{
	log_success("Success :)\n");
	log_info("Info...\n");
	log_warning("Warning :O\n");
	log_error("Error :(\n");
	log_critical("Critical! >_<\n");

	log_info("Args:\n");
	for (int i = 0; i < argc; ++i)
	{
		log_info("[%d] [%P]\n", i, argv[i]);
	}
	return EXIT_SUCCESS;
}

