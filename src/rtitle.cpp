#include"cpprich.hpp"

void rtitle1( const std::string __title , const size_t __col ){
    size_t col = round( ( __col - get_wstring_column_width( __title ) ) / 2.0 );
    std::cout << "\033[1m" << "┏";
    for ( int i = 0 ; i < __col - 2 ; i++ )
        std::cout << "━";
    std::cout << "┓" << std::endl;
    printf( "┃\033[%dG%s\033[%dG┃\n" , col , __title.c_str() , __col );
    std::cout << "┗";
    for ( int i = 0 ; i < __col - 2 ; i++ )
        std::cout << "━";
    std::cout << "┛" << "\033[0m" << std::endl << std::endl << std::endl;
    return;
}

void rtitle2( const std::string __title , const size_t __col ){
    size_t col = round( ( __col - get_wstring_column_width( __title ) ) / 2.0 );
    printf( "\033[1m\033[4m\033[%dG%s\033[0m\n\n" , col , __title.c_str() );
    return;
}

void rtitle3( const std::string __title , const size_t __col ){
    size_t col = round( ( __col - get_wstring_column_width( __title ) ) / 2.0 );
    printf( "\033[1m\033[%dG%s\033[0m\n\n" , col , __title.c_str() );
    return;
}

void rtitle4( const std::string __title , const size_t __col ){
    size_t col = round( ( __col - get_wstring_column_width( __title ) ) / 2.0 );
    printf( "\033[2m\033[1m\033[%dG%s\033[0m\n\n" , col , __title.c_str() );
    return;
}

void rtitle5( const std::string __title , const size_t __col ){
    size_t col = round( ( __col - get_wstring_column_width( __title ) ) / 2.0 );
    printf( "\033[4m\033[%dG%s\033[0m\n\n" , col , __title.c_str() );
    return;
}

void rtitle6( const std::string __title , const size_t __col ){
    size_t col = round( ( __col - get_wstring_column_width( __title ) ) / 2.0 );
    printf( "\033[3m\033[%dG%s\033[0m\n\n" , col , __title.c_str() );
    return;
}

void rtitle( const std::string __title , const unsigned char __level ){
    struct winsize size;
    ioctl( STDOUT_FILENO , TIOCGWINSZ , &size );
    size_t col = size.ws_col;
    switch ( __level )
    {
        case 1:
            rtitle1( __title , col );
            break;
        case 2:
            rtitle2( __title , col );
            break;
        case 3:
            rtitle3( __title , col );
            break;
        case 4:
            rtitle4( __title , col );
            break;
        case 5:
            rtitle5( __title , col );
            break;
        case 6:
            rtitle6( __title , col );
            break;
        default:
            return;
    }
    return;
}