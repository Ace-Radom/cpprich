#include"cpprich.hpp"

const char* wait_char[] = { "⡇" , "⠏" , "⠛" , "⠹" , "⢸" , "⣰" , "⣤" , "⣆" };

void rstatus( const std::string __text , const std::string __status_display_sytle ){
    static int count = 0;
    rprint( "[%s]%s[/] %s" , __status_display_sytle.c_str() , wait_char[count] , __text.c_str() );
    std::cout << std::flush;
    count == 7 ? count = 0 
               : count++;
    return;
}