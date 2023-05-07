#ifndef _CRICH_H_
#define _CRICH_H_

#ifdef __cplusplus
#include"cpprich.hpp"
#include"markdown.hpp"
extern "C" {
#endif

#include<stdarg.h>
#include"colorlist.h"

void cpprich_init( cpprich_colortype_t __colortype );
void crprint( const char* __format , ... );

void crstatus( const char* __text , const char* __status_display_sytle );

void crpanel( const char* __title , const char* __subtitle , const unsigned int __width , const char* __sytle , const unsigned int __line_num , ... );

void crtitle( const char* __title , const unsigned char __level );

void md_set( const char* __raw );
void md_append( const char* __raw );
void md_parse();
void md_print();

#ifdef __cplusplus
}
#endif

#endif