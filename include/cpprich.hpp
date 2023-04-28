#ifndef _CPPRICH_HPP_
#define _CPPRICH_HPP_

#include<iostream>
#include<string>
#include<cstring>
#include<sstream>
#include<cstdarg>
#include<stack>

#include"colorlist.h"

#define RPRINT_MAX_LENGTH 32767

const char* parse_tag( const char* __tag );

void rprint( const std::string __format , ... );
void rprint( const std::string __format , va_list args );
void rprintf( std::string __msg );

#endif