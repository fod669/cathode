
#include "cathode.h"

int cth_main(Arena* arena, int argc, str8_const argv[])
{
	float stuff = 123.456f;
	str8 s = str8_printf(g_crt->arena, "%.2f", stuff);

	log_success("Hello world! - %P\n", s);
	log_info("Hello world! - %P\n", s);
	log_warning("Hello world!\n");
	log_error("Hello world!\n");
	log_critical("Hello world!\n");

	//ASSERT(!"Blah");
	//ERROR_FATAL("blah! num: %f", stuff);

	log_info("Args:\n");
	for (int i = 0; i < argc; ++i)
	{
		log_info("[%d] [%P]\n", i, argv[i]);
	}
	return EXIT_SUCCESS;
}

