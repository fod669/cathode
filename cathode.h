
#pragma once
#ifndef CATHODE_H_
#define CATHODE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>

// Make sure cth_defines.h is always first.
#include "include/cth_defines.h"

#if CTH_C_VER < CTH_C11
	#error Need to compile with c11 or higher!
#endif

// Third party libs used by cathode go here.
#include "third_party/stb/stb_sprintf.h"

// All other cathode lib includes go here.
#include "include/cth.h"
#include "include/cth_string.h"
#include "include/cth_arena.h"
#include "include/cth_log.h"
#include "include/cth_os.h"

#endif // CATHODE_H_

