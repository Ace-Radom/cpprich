#include"markdown.hpp"

std::string markdown::parse_title( std::string __title , const unsigned char __level ){
    std::string title;
    size_t col = round( ( this -> terminal_col - this -> get_wstring_col_width( __title ) ) / 2.0 );
    switch ( __level )
    {
        case 1:
            title.append( "\033[1m┏" );
            for ( size_t i = 0 ; i < this -> terminal_col - 2 ; i++ )
                title.append( "━" );
            title.append( "┓\n" );
            title.append( "┃\033[" ).append( std::to_string( col ) ).append( "G" ).append( __title ).append( "\033[" ).append( std::to_string( this -> terminal_col ) ).append( "G┃\n" );
            title.append( "┗" );
            for ( size_t i = 0 ; i < this -> terminal_col - 2 ; i++ )
                title.append( "━" );
            title.append( "┛\033[0m\n\n\n" );
            break;
        case 2:
            title.append( "\033[1m\033[4m\033[" ).append( std::to_string( col ) ).append( "G" ).append( __title ).append( "\033[0m\n\n" );
            break;
        case 3:
            title.append( "\033[1m\033[" ).append( std::to_string( col ) ).append( "G" ).append( __title ).append( "\033[0m\n\n" );
            break;
        case 4:
            title.append( "\033[2m\033[1m\033[" ).append( std::to_string( col ) ).append( "G" ).append( __title ).append( "\033[0m\n\n" );
            break;
        case 5:
            title.append( "\033[4m\033[" ).append( std::to_string( col ) ).append( "G" ).append( __title ).append( "\033[0m\n\n" );
            break;
        case 6:
            title.append( "\033[3m\033[" ).append( std::to_string( col ) ).append( "G" ).append( __title ).append( "\033[0m\n\n" );
            break;
    }
    return title;
}