
// minBlockSizeBytes will be rounded up to a multiple of the OS page size.
// maxSizeBytes will then be rounded up to a multiple of the minBlockSizeBytes.
// Therefore this function can allocate a minimum of OS page size bytes.
// NOTE: Can return NULL if the OS fails to reserve the memory.
// NOTE: The Arena object itself and the name string (if not NULL) will be pushed to the beginning of the arena memory.
Arena* arena_create(const char* name, size_t maxSizeBytes, size_t minBlockSizeBytes, void* baseAddress)
{
	ASSERT(maxSizeBytes != 0);
	Arena* newArena = NULL;

	if ((baseAddress == NULL) || (MOD_POW2((size_t)baseAddress, os_mem_get_base_granularity()) == 0))
	{
		const size_t pageSize = os_mem_get_page_size();
		ASSERT(IS_POWER_OF_TWO(pageSize));

		minBlockSizeBytes = (minBlockSizeBytes == 0) ? pageSize : ALIGN_UP_TO_POW2(minBlockSizeBytes, pageSize);
		maxSizeBytes = ALIGN_UP_TO_POW2(maxSizeBytes, minBlockSizeBytes);

		u8* mem = os_mem_reserve(maxSizeBytes, baseAddress);
		if (mem == NULL)
		{
			log_error("Failed to reserve memory!\n");
			log_error("Name:                 %s\n", name);
			log_error("Max Size Bytes:       %zu\n", maxSizeBytes);
			log_error("Min Block Size Bytes: %zu\n", minBlockSizeBytes);
			log_error("Base Address:         %p\n", baseAddress);
		}
		else
		{
			// Create a temp Arena on the stack, which we can use to push memory to bootstrap our actual arena.
			Arena tmpArena =
			{
				.reserveBegin			= mem,
				.currentPos				= mem,
				.commitEnd				= mem,
				.reserveEnd				= mem + maxSizeBytes,
				.userMemoryStartPos		= NULL,
				.minBlockSizeBytes		= minBlockSizeBytes,
				.name					= { .str = NULL, .len = 0 },
				
				#if ARENA_THREAD_DEBUG
				.threadId				= os_thread_get_ID(),
				#endif
			};

			// Allocate the memory for the Arena using the temp Arena, then copy the temp Arena into the allocated memory.
			newArena = PUSH_TYPE(&tmpArena, Arena);
			*newArena = tmpArena;

			if (name != NULL)
			{
				newArena->name = str8_push_cstring(newArena, name);
			}

			newArena->userMemoryStartPos = newArena->currentPos;
		}
	}
	else
	{
		log_error("Failed to reserve memory!\n");
		log_error("The base address must be a multiple of the base address granularity.\n");
		log_error("Base Address:         %p\n", baseAddress);
		log_error("Granularity:          %zu\n", os_mem_get_base_granularity());
	}

	return newArena;
}

void arena_destroy(Arena* arena)
{
	ASSERT(arena != NULL);
	ARENA_THREAD_CHECK(arena);
	ASSERT(arena->reserveBegin != NULL);

	// Delete the Arena.
	if (os_mem_release(arena->reserveBegin) == false)
	{
		ERROR_FATAL("Failed to release memory!\n");
	}
}

void* arena_push(Arena* arena, size_t byteCount, size_t byteAlignment)
{
	ASSERT(arena != NULL);
	ARENA_THREAD_CHECK(arena);
	ASSERT(IS_POWER_OF_TWO(byteAlignment));

	u8* mem = (u8*)ALIGN_UP_TO_POW2((size_t)arena->currentPos, byteAlignment);
	u8* end = mem + byteCount;

	// Do we need to commit more memory?
	if (end > arena->commitEnd)
	{
		// Align the end to the next block boundary.
		u8* newCommitEnd = (u8*)ALIGN_UP_TO_POW2((size_t)end, arena->minBlockSizeBytes);

		// If this goes over the reserved end, we're toast.
		if (newCommitEnd > arena->reserveEnd)
		{
			// NOTE: We could reserve another memory region directly after the currently reserved region.
			// This would keep the memory contiguous, but then we'd have to set the min block size to the
			// os allocation granularity. And we'd also have to do some trickery when deallocating. And
			// it also may not work if the memory region afterwards has already been reserved. I didn't
			// bother with this because it seems as though you should really just ensure that your reserved
			// memory region is always large enough anyway. But it should theoretically be possible to
			// continue pushing memory until you run out of virtual address space by doing this.
			ERROR_FATAL("Tried to allocate more memory than was reserved!\n");
		}

		void* result = os_mem_commit(arena->commitEnd, newCommitEnd - arena->commitEnd);
		if (result == NULL)
		{
			ERROR_FATAL("Failed to commit memory!\n");
		}
		ASSERT(result == arena->commitEnd);
		arena->commitEnd = newCommitEnd;
	}

	arena->currentPos = end;
	return mem;
}

void arena_pop(Arena* arena, size_t byteCount, bool decommit)
{
	ASSERT(arena != NULL);
	ARENA_THREAD_CHECK(arena);

	u8* newPos = arena->currentPos - byteCount;
	if (newPos < arena->userMemoryStartPos)
	{
		newPos = arena->userMemoryStartPos;
	}

	if (decommit)
	{
		u8* blockBoundary = (u8*)ALIGN_UP_TO_POW2((size_t)newPos, arena->minBlockSizeBytes);
		if (blockBoundary < arena->commitEnd)
		{
			if (os_mem_decommit(blockBoundary, arena->commitEnd - blockBoundary) == false)
			{
				ERROR_FATAL("Failed to decommit memory!\n");
			}
			arena->commitEnd = blockBoundary;
			arena->currentPos = blockBoundary;
		}
	}

	os_mem_zero(newPos, arena->currentPos - newPos);
	arena->currentPos = newPos;
}

void arena_pop_to(Arena* arena, u8* pos, bool decommit)
{
	ASSERT(arena != NULL);
	ASSERT(pos < arena->currentPos);
	ASSERT(pos >= arena->userMemoryStartPos);
	arena_pop(arena, (size_t)(arena->currentPos - pos), decommit);
}

void arena_print(Arena* arena)
{
	ASSERT(arena != NULL);

	size_t pushedTotal			= arena_get_pushed_total(arena);
	size_t committedTotal		= arena_get_committed_total(arena);
	size_t committedRemaining	= arena_get_committed_remaining(arena);
	size_t reservedTotal		= arena_get_reserved_total(arena);
	size_t reservedRemaining	= arena_get_reserved_remaining(arena);

	log_info("Arena: %p\n", (void*)arena);
	log_info("  Name:                      \"%P\"\n", arena->name);
	#if ARENA_THREAD_DEBUG
	log_info("  Thread Id:                 %u\n", arena->threadId);
	#endif

	log_info("  Min Block Size:            %zu\n", arena->minBlockSizeBytes);

	log_info("  Pushed Total Bytes:        %zu\n", pushedTotal);

	log_info("  Committed Total Bytes:     %zu\n", committedTotal);
	log_info("  Committed Remaining Bytes: %zu\n", committedRemaining);

	log_info("  Reserved Total Bytes:      %zu\n", reservedTotal);
	log_info("  Reserved Remaining Bytes:  %zu\n", reservedRemaining);
}

