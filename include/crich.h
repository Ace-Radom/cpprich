#ifndef _CRICH_H_
#define _CRICH_H_

#ifdef __cplusplus
#include<cpprich.hpp>
extern "C" {
#endif

#include<stdarg.h>
#include"colorlist.h"

void cpprich_init( cpprich_colortype_t __colortype );
void crprint( const char* __format , ... );

void crstatus( const char* __text , const char* __status_display_sytle );

#ifdef __cplusplus
}
#endif

#endif