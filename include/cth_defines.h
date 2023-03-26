
#pragma once
#ifndef CTH_DEFINES_H_
#define CTH_DEFINES_H_

/*

Summary of default context defines in the cathode libs.
All of these options are either set on the command line as a compiler option,
or detected by the compiler itself automatically.
===============================================================================

Build type. One is set to 1, the rest are 0.
Exactly one of these must be set as a compiler option: eg -DCTH_DEBUG=1
- CTH_DEBUG						0 or 1
- CTH_RELEASE					0 or 1
- CTH_DIST						0 or 1

Console app. Set as a compiler option. Defaults to 0 if not set.
- CTH_CONSOLE					0 or 1

The app should show UI elements. Set as a compiler option. Defaults to 1 if not set.
- CTH_UI						0 or 1

Compiler. One is set to 1, the rest are 0 (auto detected).
- CTH_COMPILER_CLANG			0 or 1
- CTH_COMPILER_GCC				0 or 1
- CTH_COMPILER_CL				0 or 1

Language versions. Used by CTH_C/CPP_VER below.
- CTH_C99						199901L
- CTH_C11						201112L
- CTH_C17						201710L
- CTH_CPP98						199711L
- CTH_CPP11						201103L
- CTH_CPP14						201402L
- CTH_CPP17						201703L

Set to one of the above values, or 0L (auto detected).
- CTH_C_VER						__STDC_VERSION__ or 0L
- CTH_CPP_VER					__cplusplus or 0L

Operating system. One is set to 1, the rest are 0 (auto detected).
- CTH_OS_WINDOWS				0 or 1
- CTH_OS_APPLE					0 or 1
- CTH_OS_ANDROID				0 or 1
- CTH_OS_LINUX					0 or 1

Architecture. One is set to 1, the other 0 (auto detected).
- CTH_ARCH_32_BIT				0 or 1
- CTH_ARCH_64_BIT				0 or 1

Asserts.
Defaults to 1 in debug builds, otherwise it's 0.
Can be overridden by setting it as a compiler option: eg -DCTH_ENABLE_ASSERTS=1
- CTH_ENABLE_ASSERTS			0 or 1

Logging line information.
Defaults to 1 in debug builds, otherwise it's 0.
Can be overridden by setting it as a compiler option: eg -DCTH_LOG_LINE_INFO=1
- CTH_LOG_LINE_INFO				0 or 1

*/

// Build type, and console app:
// NOTE: These are defined in the build script.
// ============================================================================
#if !defined(CTH_DEBUG)
	#define CTH_DEBUG 0
#endif
#if !defined(CTH_RELEASE)
	#define CTH_RELEASE 0
#endif
#if !defined(CTH_DIST)
	#define CTH_DIST 0
#endif

#if (CTH_DEBUG + CTH_RELEASE + CTH_DIST) != 1
	#error One of CTH_DEBUG, CTH_RELEASE, or CTH_DIST must be set on the command line.
#endif

#if !defined(CTH_CONSOLE)
	#define CTH_CONSOLE 0
#endif

#if !defined(CTH_UI)
	#define CTH_UI 1
#endif

// Compiler detection:
// ============================================================================
#if defined(__clang__)
	#define CTH_COMPILER_CLANG 1
#elif defined(__GNUCTH__)
	#define CTH_COMPILER_GCC 1
#elif defined(_MSCTH_VER)
	#define CTH_COMPILER_CL 1
#else
	#error Unknown compiler!
#endif

#if !defined(CTH_COMPILER_CLANG)
	#define CTH_COMPILER_CLANG 0
#endif
#if !defined(CTH_COMPILER_GCC)
	#define CTH_COMPILER_GCC 0
#endif
#if !defined(CTH_COMPILER_CL)
	#define CTH_COMPILER_CL 0
#endif

// Language detection:
// ============================================================================
#define CTH_C99			199901L
#define CTH_C11			201112L
#define CTH_C18			201710L
#define CTH_CPP98		199711L
#define CTH_CPP11		201103L
#define CTH_CPP14		201402L
#define CTH_CPP17		201703L

#if defined(__STDC_VERSION__)
	#define CTH_C_VER __STDC_VERSION__
#elif defined(__cplusplus)
	#define CTH_CPP_VER __cplusplus
#else
	#error Unknown language!
#endif

#if !defined(CTH_C_VER)
	#define CTH_C_VER 0L
#endif
#if !defined(CTH_CPP_VER)
	#define CTH_CPP_VER 0L
#endif

// Operating system detection:
// ============================================================================
#if defined(_WIN32)
	#define CTH_OS_WINDOWS 1
#elif defined(__APPLE__)
	#define CTH_OS_APPLE 1
#elif defined(__ANDROID__)
	#define CTH_OS_ANDROID 1
#elif defined(__linux__)
	#define CTH_OS_LINUX 1
#else
	#error Unknown operating system!
#endif

#if !defined(CTH_OS_WINDOWS)
	#define CTH_OS_WINDOWS 0
#endif
#if !defined(CTH_OS_APPLE)
	#define CTH_OS_APPLE 0
#endif
#if !defined(CTH_OS_ANDROID)
	#define CTH_OS_ANDROID 0
#endif
#if !defined(CTH_OS_LINUX)
	#define CTH_OS_LINUX 0
#endif

// Architecture detection:
// ============================================================================
#if UINTPTR_MAX == 0xFFFFFFFF
	#define CTH_ARCH_32_BIT 1
#elif UINTPTR_MAX == 0xFFFFFFFFFFFFFFFF
	#define CTH_ARCH_64_BIT 1
#else
	#error Unknown architecture!
#endif

#if !defined(CTH_ARCH_32_BIT)
	#define CTH_ARCH_32_BIT 0
#endif
#if !defined(CTH_ARCH_64_BIT)
	#define CTH_ARCH_64_BIT 0
#endif

// Asserts:
// ============================================================================
#if !defined(CTH_ENABLE_ASSERTS)
	#if CTH_DEBUG
		#define CTH_ENABLE_ASSERTS 1
	#else
		#define CTH_ENABLE_ASSERTS 0
	#endif
#endif

// Logging:
// ============================================================================
#if !defined(CTH_LOG_LINE_INFO)
	#if CTH_DEBUG
		#define CTH_LOG_LINE_INFO 1
	#else
		#define CTH_LOG_LINE_INFO 0
	#endif
#endif

#endif // CTH_DEFINES_H_

