
#pragma once
#ifndef CTH_H_
#define CTH_H_

typedef struct _Arena			Arena;

typedef int8_t					s8;
typedef int16_t					s16;
typedef int32_t					s32;
typedef int64_t					s64;

typedef uint8_t					u8;
typedef uint16_t				u16;
typedef uint32_t				u32;
typedef uint64_t				u64;

#define EXIT_SUCCESS			0
#define EXIT_FAILURE			1
#define EXIT_CODE_ERROR_FATAL	100
#define EXIT_CODE_ASSERT		101

#ifndef __cplusplus
	#define alignas _Alignas
	#define alignof _Alignof
#endif

#define CDECL					__cdecl
#define STDCALL					__stdcall

#define NORETURN				__declspec(noreturn)

// __fastfail breaks in a debugger, or immediately terminates the calling process with minimum overhead.
#define DEBUG_BREAK_OR_TERMINATE(_ExitCode) __fastfail(_ExitCode)

#define IS_POWER_OF_TWO(_X)				((_X) && !((_X) & ((_X) - 1)))
#define ALIGN_DOWN_TO_POW2(_X, _Pow2)	((_X) & ~((_Pow2) - 1))
#define ALIGN_UP_TO_POW2(_X, _Pow2)		ALIGN_DOWN_TO_POW2((_X) + (_Pow2) - 1, (_Pow2))
#define MOD_POW2(_X, _Pow2)				((_X) & ((_Pow2) - 1))

#define KILOBYTES(_X)			((_X) * 1024LL)
#define MEGABYTES(_X)			(KILOBYTES(_X) * 1024LL)
#define GIGABYTES(_X)			(MEGABYTES(_X) * 1024LL)
#define TERABYTES(_X)			(GIGABYTES(_X) * 1024LL)

#endif // CTH_H_

