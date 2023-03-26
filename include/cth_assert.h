
#pragma once
#ifndef CTH_ASSERT_H_
#define CTH_ASSERT_H_

/*

Proper ASSERT() usage:
----------------------
- Use asserts to check invariants. Meaning the asserted statement absolutely HAS to be true for the code to function correctly.
	- For example, if a function expects a parameter to be within a certain range, you can assert that range at the top of the function.
	- This way the function doesn't have to waste time (in release builds) by checking the range itself, it can just assume it's within range.
	- It's up to the caller to ensure they respect the invariants of the function, by passing parameters within range etc.
- If the assert fails, then the code is being used incorrectly and must be fixed.
	- This might occur if the invariants change, but any callers are not updated to respect them. In which case the asserts are useful because
	  they alert any callers that code needs to be updated.
	- It might also occur if a caller has its code changed in an unexpected way, producing invalid parameters.
	- The asserts are there to catch these cases.
- Do NOT use asserts to check for runtime errors (or to assert that an error didn't occur).
	- A code path that may cause an unrecoverable error should be dealth with by an error popup and perhaps a subsequent crashing (or clean exiting)
	  of the program.
	- A code path that may cause a recoverable error should simply be dealt with by checking for the error and taking the appropriate action to recover.
- It should be safe to assume that all assert statements are true in a release version of the program, which is why we remove them for non debug builds,
  and why we should never use them for runtime error checking.

*/

#define STATIC_ASSERT_MSG(_Exp, _Msg)	_Static_assert(_Exp, _Msg)
#define STATIC_ASSERT(_Exp)				STATIC_ASSERT_MSG(_Exp, #_Exp)

// TODO: Have some sort of option to not show message boxes (if a console app only).

#if CTH_ENABLE_ASSERTS
	#define ASSERT(_Exp)																																				\
		do																																								\
		{																																								\
			if (!(_Exp))																																				\
			{																																							\
				if (g_crt != NULL)																																		\
				{																																						\
					u8* _arenaPos = arena_get_pos(g_crt->arena);																										\
					log_critical("Assert failed!\n[%u] %s(%d): \"%s\"\n\n", os_thread_get_ID(), __FILE__, __LINE__, #_Exp);												\
					str8 _msg = str8_printf(g_crt->arena, "File: %s - line: %d\nThread ID: %u\n\nExpression: \"%s\"", __FILE__, __LINE__, os_thread_get_ID(), #_Exp);	\
					switch (os_message_box("Assert failed!", _msg.str, "Abort", "Debug", "Continue", DMBB_TWO))															\
					{																																					\
						case 0: FAST_FAIL(EXIT_CODE_ASSERT); break;																										\
						case 1: DEBUG_BREAK(); break;																													\
						default: break;																																	\
					}																																					\
					arena_pop_to(g_crt->arena, _arenaPos, false);																										\
				}																																						\
				else																																					\
				{																																						\
					os_output_debug_string("Assert failed!\n\"");																										\
					os_output_debug_string(#_Exp);																														\
					os_output_debug_string("\"\n");																														\
					DEBUG_BREAK();																																		\
				}																																						\
			}																																							\
		}																																								\
		while (0)
#else
	#define ASSERT(_Exp) do { (void)0; } while (0)
#endif

#define ERROR_FATAL(_Format, ...)																											\
	do																																		\
	{																																		\
		ASSERT(g_crt != NULL);																												\
		u8* _arenaPos = arena_get_pos(g_crt->arena);																						\
		str8 _msg = str8_printf(g_crt->arena, "Fatal error in function: %s\n\nError message:\n\"" _Format "\"", __func__, ##__VA_ARGS__);	\
		log_critical("%P\n\n", _msg);																										\
		os_message_box("Fatal error!", _msg.str, "Abort", NULL, NULL, DMBB_ONE);															\
		arena_pop_to(g_crt->arena, _arenaPos, false);																						\
		FAST_FAIL(EXIT_CODE_ERROR_FATAL);																									\
	}																																		\
	while (0)

#endif // CTH_ASSERT_H_

