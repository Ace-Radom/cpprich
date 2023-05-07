#include"crich.h"

#define JUDGE_NULL( str ) ( str == NULL ) ? "" : str

markdown md( "" );

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
    rstatus( std::string( JUDGE_NULL( __text ) ) , std::string( JUDGE_NULL( __status_display_sytle ) ) );
    return;
}

void crpanel( const char* __title , const char* __subtitle , const unsigned int __width , const char* __sytle , const unsigned int __line_num , ... ){
    va_list lines;
    va_start( lines , __line_num );
    rpanel( std::string( JUDGE_NULL( __title ) ) , std::string( JUDGE_NULL( __subtitle ) ) , __width , std::string( JUDGE_NULL( __sytle ) ) , __line_num , lines );
    va_end( lines );
    return;
}

void crtitle( const char* __title , const unsigned char __level ){
    rtitle( std::string( JUDGE_NULL( __title ) ) , __level );
    return;
}

void md_set( const char* __raw ){
    md.set( std::string( JUDGE_NULL( __raw ) ) );
    return;
}

void md_parse(){
    md.parse();
    return;
}

void md_print(){
    md.print();
    return;
}