
#pragma once
#ifndef CTH_CRT_H_
#define CTH_CRT_H_

// User entry point.
int cth_main(Arena* arena, int argc, str8_c argv[]);

void* CDECL memset(void* ptr, int value, size_t byteCount);
void* CDECL memcpy(void* dst, const void* src, size_t byteCount);

#endif // CTH_CRT_H_

