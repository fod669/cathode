
inline void arena_reset(Arena* arena, bool decommit)
{
	arena_pop_to(arena, arena->userMemoryStartPos, decommit);
}

inline u8*		arena_get_pos(Arena* arena)						{ return arena->currentPos; }
inline size_t	arena_get_pushed_total(Arena* arena)			{ ASSERT(arena->currentPos >= arena->reserveBegin);		return (size_t)(arena->currentPos - arena->reserveBegin); }
inline size_t	arena_get_committed_total(Arena* arena)			{ ASSERT(arena->commitEnd >= arena->reserveBegin);		return (size_t)(arena->commitEnd - arena->reserveBegin); }
inline size_t	arena_get_committed_remaining(Arena* arena)		{ ASSERT(arena->commitEnd >= arena->currentPos);		return (size_t)(arena->commitEnd - arena->currentPos); }
inline size_t	arena_get_reserved_total(Arena* arena)			{ ASSERT(arena->reserveEnd >= arena->reserveBegin);		return (size_t)(arena->reserveEnd - arena->reserveBegin); }
inline size_t	arena_get_reserved_remaining(Arena* arena)		{ ASSERT(arena->reserveEnd >= arena->currentPos);		return (size_t)(arena->reserveEnd - arena->currentPos); }

