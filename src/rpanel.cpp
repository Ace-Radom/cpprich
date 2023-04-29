#include"cpprich.hpp"

size_t get_wstring_column_width( std::string __str ){
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    std::wstring ws = conv.from_bytes( __str );
    int width = 0;
    for ( auto c : ws )
        width += wcwidth( c );
    return width;
}

void rpanel( const std::string __title , const unsigned int __width , const std::string __style , const unsigned int __line_num , ... ){
    va_list lines;
    va_start( lines , __line_num );
    rpanel( __title , __width , __style , __line_num , lines );
    va_end( lines );
    return;
}

void rpanel( const std::string __title , const unsigned int __width , const std::string __style , const unsigned int __line_num , va_list lines ){
    std::string begin_line( __title );
    if ( begin_line.empty() )
    {
        for ( int i = 0 ; i < __width - 2 ; i++ )
            begin_line.append( "─" );
    } // no title 
    else
    {
        int straight_still_needed_num = __width - get_wstring_column_width( begin_line ) - 5;
        begin_line = "─ " + begin_line + " ";
        for ( int i = 0 ; i < straight_still_needed_num ; i++ )
            begin_line.append( "─" );
    }
    rprint( "[%s]╭%s╮\n" , __style.c_str() , begin_line.c_str() );
    for ( int i = 0 ; i < __line_num ; i++ )
    {
        const char* thisline_msg = va_arg( lines , const char* );
        std::string thisline( thisline_msg );
        int space_still_needed_num = __width - get_wstring_column_width( thisline ) - 3;
        thisline.append( space_still_needed_num , ' ' );
        rprint( "[%s]│ %s│\n" , __style.c_str() , thisline.c_str() );
    }
    std::string end_line;
    for ( int i = 0 ; i < __width - 2 ; i++ )
        end_line.append( "─" );
    rprint( "[%s]╰%s╯\n" , __style.c_str() , end_line.c_str() );
    return;
}