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