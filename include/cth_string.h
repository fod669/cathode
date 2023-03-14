
#pragma once
#ifndef CTH_STRING_H_
#define CTH_STRING_H_

typedef struct _str8
{
	char*	str;
	u64		len;
} str8;

#define STR8(_X) str8_from_cstring(_X)

str8 str8_from_cstring(char* s);

#endif // CTH_STRING_H_

