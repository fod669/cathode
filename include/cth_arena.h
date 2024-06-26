
#pragma once
#ifndef CTH_ARENA_H_
#define CTH_ARENA_H_

#if CTH_DEBUG
	#define ARENA_THREAD_DEBUG 1
#else
	#define ARENA_THREAD_DEBUG 0
#endif

#if ARENA_THREAD_DEBUG
	// TODO: Test that this works.
	#define ARENA_THREAD_CHECK(_Arena)																																	\
		do																																								\
		{																																								\
			if (_Arena->threadId != os_thread_get_ID())																													\
			{																																							\
				ERROR_FATAL("Arena function called from a thread that didn't create the arena!\nArena name: %P\nCreation thread Id: %u\nCalling thread Id: %u\n",		\
					_Arena->name,																																		\
					_Arena->threadId,																																	\
					os_thread_get_ID());																																\
			}																																							\
		}																																								\
		while (0)
#else
	#define ARENA_THREAD_CHECK(_Arena) do { (void)0; } while (0)
#endif

typedef struct Arena
{
	u8*			reserveBegin;			// Pointer to the beginning of the reserved virtual memory region.
	u8*			currentPos;				// Pointer to (one past) the end of the memory pushed so far.
	u8*			commitEnd;				// Pointer to (one past) the committed region of virtual memory.
	u8*			reserveEnd;				// Pointer to (one past) the end of the reserved virtual memory region.
	u8*			userMemoryStartPos;		// Pointer to the start of where user memory will be allocated (pushed) from in this
										// arena (because the Arena object and name string are pushed to the very beginning of the memory region).
	size_t		minBlockSizeBytes;		// Memory commits will always be rounded up to this boundary. Will always be a multiple of the OS page size.
	str8		name;					// The name of the arena. Memory for this string is allocated at the beginning of the arena itself.

#if ARENA_THREAD_DEBUG
	u32			threadId;				// Arenas are not thread safe. So ensure that every time a function is called on it, it's from the same thread that created it.
#endif

} Arena;

// Create and destroy arenas.
// NOTE: arena_create() can return NULL if it fails to reserve the memory.
// NOTE: The Arena itself is placed at the beginning of the allocated memory, so you get sizeof(Arena) fewer useable bytes
//       than what you set in maxSizeBytes.
Arena*			arena_create(const char* name, size_t maxSizeBytes, size_t minBlockSizeBytes, void* baseAddress);
void			arena_destroy(Arena* arena);

void*			arena_push(Arena* arena, size_t byteCount, size_t byteAlignment);
void			arena_pop(Arena* arena, size_t byteCount, bool decommit);
void			arena_pop_to(Arena* arena, u8* pos, bool decommit);
void			arena_reset(Arena* arena, bool decommit);

u8*				arena_get_pos(Arena* arena);
size_t			arena_get_pushed_total(Arena* arena);
size_t			arena_get_committed_total(Arena* arena);
size_t			arena_get_committed_remaining(Arena* arena);
size_t			arena_get_reserved_total(Arena* arena);
size_t			arena_get_reserved_remaining(Arena* arena);

void			arena_print(Arena* arena);

// Use these macros to allocate and deallocate memory from arenas.
// - Arenas act like a LIFO stack, where memory can be pushed onto the end or popped from the end.
// - Memory pushed will always be cleared to zero.
#define PUSH_BYTES(_Arena, _ByteCount, _Align)					arena_push(_Arena, _ByteCount, _Align)
#define PUSH_TYPE(_Arena, _Type)								(_Type*)PUSH_BYTES(_Arena, sizeof(_Type), alignof(_Type))
#define PUSH_TYPE_ARRAY(_Arena, _Type, _ElementCount)			(_Type*)PUSH_BYTES(_Arena, sizeof(_Type) * (_ElementCount), alignof(_Type))

#define POP_BYTES(_Arena, _ByteCount, _Decommit)				arena_pop(_Arena, _ByteCount, _Decommit)
#define POP_TYPE(_Arena, _Type, _Decommit)						POP_BYTES(_Arena, sizeof(_Type), _Decommit)
#define POP_TYPE_ARRAY(_Arena, _Type, _ElementCount, _Decommit)	POP_BYTES(_Arena, sizeof(_Type) * (_ElementCount), _Decommit)

#endif // CTH_ARENA_H_

