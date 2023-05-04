#include"markdown.hpp"

markdown::markdown( std::string __raw ){
    this -> set( __raw );
    return;
}

void markdown::set( std::string __raw ){
    this -> raw = __raw;
    this -> parsed.clear();
    return;
}

size_t markdown::get_wstring_col_width( std::string __str ){
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    std::wstring ws = conv.from_bytes( __str );
    int width = 0;
    for ( auto c : ws )
        width += wcwidth( c );
    return width;
}

int markdown::parse(){
    if ( this -> raw.empty() )
        return -1;
    // raw not set

    this -> parsed.clear();

    std::istringstream raw_stream( this -> raw );
    std::string this_line;
    std::string this_block;

    while ( std::getline( raw_stream , this_line ) )
    {

#pragma region TITLE

        unsigned char title_level = 0;
        for ( int i = 0 ; i < 6 && i < this_line.size() ; i++ )
        {
            if ( this_line[i] == '#' )
                title_level++;
            else
                break;
        } // get title level (if this line is a title)
        if ( title_level > 0 )
        {
            this -> parsed.push_back( std::make_tuple( BLOCK_TITLE , this_line.substr( title_level + 1 ) , title_level ) );
            continue;
        } // this line is a title

#pragma endregion TITLE

#pragma region CODE_BLOCK

        if ( this_line.substr( 0 , 3 ) == "```" || this_line.substr( 0 , 3 ) == "~~~" )
        {
            this -> parsed.push_back( std::make_tuple( BLOCK_TEXT , "\n" , 0 ) );
            this -> parsed.push_back( std::make_tuple( BLOCK_CODEB , "" , 0 ) );
            // empty code block begin line
            while ( std::getline( raw_stream , this_line ) )
            {
                if ( this_line.substr( 0 , 3 ) == "```" || this_line.substr( 0 , 3 ) == "~~~" )
                    break;
                this -> parsed.push_back( std::make_tuple( BLOCK_CODEB , this_line , 0 ) );
            } // get all codes in code block
            this -> parsed.push_back( std::make_tuple( BLOCK_CODEB , "" , 0 ) );
            // empty code block end line
            continue;
        } // code block start

#pragma endregion CODE_BLOCK

        unsigned int this_line_sytle = BLOCK_TEXT; // line style, for those have consistent style but lots of blocks
        int this_line_extra = 0;                   // line style extra

        if ( this_line[0] == '>' )
        {
            int count = 0;
            while ( this_line[count] == '>' )
                count++;

            if ( this -> parsed.empty() )
                this -> parsed.push_back( std::make_tuple( BLOCK_QUOTE , "" , -count ) );
            // this is the first block
            else if ( ( std::get<0>( this -> parsed.back() ) & BLOCK_QUOTE ) && std::get<1>( this -> parsed.back() ) == "\n" )
                this -> parsed.push_back( std::make_tuple( BLOCK_QUOTE , "" , -count ) );
            // last one is also quote block, and one line ">\n" given
            else if ( ( std::get<0>( this -> parsed.back() ) & BLOCK_QUOTE ) && std::get<2>( this -> parsed.back() ) != count )
            {
                this -> parsed.push_back( std::make_tuple( BLOCK_QUOTE , "\n" , count ) );
                this -> parsed.push_back( std::make_tuple( BLOCK_QUOTE , "" , -count ) );
            } // last one is also quote block, one line ">\n" not given, but in different levels
            else if ( !( std::get<0>( this -> parsed.back() ) & BLOCK_QUOTE ) )
            {
                this -> parsed.push_back( std::make_tuple( BLOCK_QUOTE , "\n" , count ) );
                this -> parsed.push_back( std::make_tuple( BLOCK_QUOTE , "" , -count ) );
            } // last one isn't quote block
            this_line_sytle = BLOCK_QUOTE;
            this_line_extra = count;
            this_line = this_line.erase( 0 , count );
        } // quote

        if ( this_line.empty() )
            this_line = "\n";

        this -> parsed.push_back( std::make_tuple( this_line_sytle , this_line , this_line_extra ) );
    }
}

void markdown::print(){
    struct winsize size;
    ioctl( STDOUT_FILENO , TIOCGWINSZ , &size );
    this -> terminal_row = size.ws_row;
    this -> terminal_col = size.ws_col;
    // get terminal size

    for ( auto block : this -> parsed )
    {
        if ( std::get<0>( block ) == BLOCK_TITLE )
        {
            std::cout << this -> parse_title( std::get<1>( block ) , std::get<2>( block ) );
            continue;
        } // print title
        else if ( std::get<0>( block ) == BLOCK_CODEB )
        {
            std::cout << "\033[48;2;39;40;34m";
            for ( int i = 0 ; i < this -> terminal_col ; i++ )
                std::cout << " ";
            std::cout << "\r " << std::get<1>( block ) << "\033[0m" << std::endl;
            continue;
        } // print code block
        else if ( std::get<0>( block ) & BLOCK_QUOTE )
        {
            std::cout << "\033[95m";
            if ( std::get<2>( block ) < 0 )
            {
                std::cout << "▌";
                for ( int i = 1 ; i < abs( std::get<2>( block ) ) ; i++ )
                    std::cout << " ▌";
                // quote line begin block has a space at beginning
                continue;
            } // print quote begin, continue
        }
        std::cout << std::get<1>( block ) << "\033[0m";
    }
}