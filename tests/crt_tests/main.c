
#include "cathode.h"

u32 other_thread(void* userData)
{
	log_info("other thread!\n");
	os_thread_sleep(3000);
	return 123;
}

int cth_main(int argc, str8_const argv[])
{
	log_info("\nArgs:\n");
	for (int i = 0; i < argc; ++i)
	{
		log_info("[%d] [%P]\n", i, argv[i]);
	}

	s8				tmp_s8		= -1;
	s16				tmp_s16		= -2;
	s32				tmp_s32		= -3;
	s64				tmp_s64		= -4;
	u8				tmp_u8		= 1;
	u16				tmp_u16		= 2;
	u32				tmp_u32		= 3;
	u64				tmp_u64		= 4;
	f32				tmp_f32		= 3.1415f;
	f64				tmp_f64		= 2.7182;
	int				tmp_int		= -10;
	unsigned int	tmp_uint	= 10;
	size_t			tmp_size_t	= 20;
	ptrdiff_t		tmp_ptrdiff_t = &tmp_s32 - &tmp_int;

	log_info("\nprintf tests:\n");
	log_info("s8 = %d\n", tmp_s8);
	log_info("s16 = %d\n", tmp_s16);
	log_info("s32 = %d\n", tmp_s32);
	log_info("s64 = %lld\n", tmp_s64);
	log_info("u8 = %u\n", tmp_u8);
	log_info("u16 = %u\n", tmp_u16);
	log_info("u32 = %u\n", tmp_u32);
	log_info("u64 = %llu\n", tmp_u64);
	log_info("f32 = %f\n", tmp_f32);
	log_info("f64 = %f\n", tmp_f64);
	log_info("int = %d\n", tmp_int);
	log_info("unsigned int = %d\n", tmp_uint);
	log_info("size_t = %zu\n", tmp_size_t);
	log_info("ptrdiff_t = %td\n", tmp_ptrdiff_t);

	arena_print(g_crt->arena);

	ThreadLocalStorage* tls = os_tls();
	arena_print(tls->arena);

	ThreadHandle th = os_thread_create(other_thread, NULL);
	u32 retVal = 0;
	os_thread_join(&th, &retVal);
	log_warning("thread returned %u\n", retVal);

	return EXIT_SUCCESS;
}

