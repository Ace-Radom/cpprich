#ifndef _CPPRICH_HPP_
#define _CPPRICH_HPP_

#include<iostream>
#include<string>
#include<cstring>
#include<sstream>
#include<cstdarg>
#include<stack>
#include<codecvt>
#include<wchar.h>
#include<locale>
#include<cmath>
#include<unistd.h>
#include<sys/ioctl.h>

#include"colorlist.h"
// #include"markdown.hpp"

#define RPRINT_MAX_LENGTH 32767

const char* parse_tag( const char* __tag );

void rprint( const std::string __format , ... );
void rprint( const std::string __format , va_list args );
void rprintf( std::string __msg );

void rstatus( const std::string __text , const std::string __status_display_sytle );

void rpanel( const std::string __title , const std::string __subtitle , const unsigned int __width , const std::string __style , const unsigned int __line_num , ... );
void rpanel( const std::string __title , const std::string __subtitle , const unsigned int __width , const std::string __style , const unsigned int __line_num , va_list lines );

size_t get_wstring_column_width( std::string __str );

void rtitle( const std::string __title , const unsigned char __level );

#endif