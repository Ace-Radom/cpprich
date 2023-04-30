#include"crich.h"

void cpprich_init( cpprich_colortype_t __colortype ){
    color_type = __colortype;
    return;
}

void crprint( const char* __format , ... ){
    va_list args;
    va_start( args , __format );
    rprint( __format , args );
    va_end( args );
    return;
}

void crstatus( const char* __text , const char* __status_display_sytle ){
    rstatus( std::string( __text ) , std::string( __status_display_sytle ) );
    return;
}

void crpanel( const char* __title , const char* __subtitle , const unsigned int __width , const char* __sytle , const unsigned int __line_num , ... ){
    va_list lines;
    va_start( lines , __line_num );
    rpanel( std::string(  __title ) , std::string( __subtitle ) , __width , std::string( __sytle ) , __line_num , lines );
    va_end( lines );
    return;
}