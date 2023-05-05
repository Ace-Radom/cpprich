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

void markdown::pre_format(){
    std::istringstream istrstream( this -> raw );
    std::ostringstream ostrstream;
    std::string this_line;
    while ( getline( istrstream , this_line ) )
    {
        if ( this_line.empty() )
        {
            ostrstream << std::endl;
            continue;
        }
        size_t bpos = this_line.find_first_not_of( ' ' );
        size_t epos = this_line.find_last_not_of( ' ' );
        if ( epos != std::string::npos )
            this_line = this_line.substr( bpos , epos - bpos + 1 );
        this_line.insert( 0 , bpos / 4 * 4 , ' ' );
        ostrstream << this_line << std::endl;
    }
    this -> raw = ostrstream.str();
    return;
}

int markdown::parse(){
    if ( this -> raw.empty() )
        return -1;
    // raw not set

    this -> pre_format();
    this -> parsed.clear();

    std::istringstream raw_stream( this -> raw );
    std::string this_line;
    std::wstring this_block;

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
            this -> parsed.push_back( std::make_tuple( BLOCK_TITLE , this_line.substr( title_level + 1 ) , title_level , 0 ) );
            continue;
        } // this line is a title

#pragma endregion TITLE

#pragma region CODE_BLOCK

        if ( this_line.substr( 0 , 3 ) == "```" || this_line.substr( 0 , 3 ) == "~~~" )
        {
            this -> parsed.push_back( std::make_tuple( BLOCK_TEXT , "\n" , 0 , 0 ) );
            this -> parsed.push_back( std::make_tuple( BLOCK_CODEB , "" , 0 , 0 ) );
            // empty code block begin line
            while ( std::getline( raw_stream , this_line ) )
            {
                if ( this_line.substr( 0 , 3 ) == "```" || this_line.substr( 0 , 3 ) == "~~~" )
                    break;
                this -> parsed.push_back( std::make_tuple( BLOCK_CODEB , this_line , 0 , 0 ) );
            } // get all codes in code block
            this -> parsed.push_back( std::make_tuple( BLOCK_CODEB , "" , 0 , 0 ) );
            // empty code block end line
            continue;
        } // code block start

#pragma endregion CODE_BLOCK

        unsigned int this_line_sytle = BLOCK_TEXT; // line style, for those have consistent style but lots of blocks
        int this_line_extra = 0;                   // line style extra

#pragma region QUOTE

        if ( this_line[0] == '>' )
        {
            int count = 0;
            while ( this_line[count] == '>' )
                count++;

            if ( this -> parsed.empty() )
                this -> parsed.push_back( std::make_tuple( BLOCK_QUOTE , "" , -count , 0 ) );
            // this is the first block
            else if ( ( std::get<0>( this -> parsed.back() ) & BLOCK_QUOTE ) && std::get<1>( this -> parsed.back() ) == "\n" )
                this -> parsed.push_back( std::make_tuple( BLOCK_QUOTE , "" , -count , 0 ) );
            // last one is also quote block, and one line ">\n" given
            else if ( ( std::get<0>( this -> parsed.back() ) & BLOCK_QUOTE ) && std::get<2>( this -> parsed.back() ) != count )
            {
                this -> parsed.push_back( std::make_tuple( BLOCK_QUOTE , "\n" , count , 0 ) );
                this -> parsed.push_back( std::make_tuple( BLOCK_QUOTE , "" , -count , 0 ) );
            } // last one is also quote block, one line ">\n" not given, but in different levels
            else if ( !( std::get<0>( this -> parsed.back() ) & BLOCK_QUOTE ) )
            {
                this -> parsed.push_back( std::make_tuple( BLOCK_QUOTE , "\n" , count , 0 ) );
                this -> parsed.push_back( std::make_tuple( BLOCK_QUOTE , "" , -count , 0 ) );
            } // last one isn't quote block
            this_line_sytle = BLOCK_QUOTE;
            this_line_extra = count;
            this_line = this_line.erase( 0 , count );
            // delete quote start sign
            if ( this_line.find_first_not_of( ' ' ) != std::string::npos )
                this_line = this_line.substr( this_line.find_first_not_of( ' ' ) );
            // earse possible space left
        } // quote

#pragma endregion QUOTE

        if ( this_line.empty() )
        {
            this -> parsed.push_back( std::make_tuple( this_line_sytle , "\n" , this_line_extra , 0 ) );
            continue;
        }

        if ( this_line_sytle == BLOCK_TEXT )
        {
            if ( !( std::get<0>( this -> parsed.back() ) & BLOCK_TEXT ) )
                this -> parsed.push_back( std::make_tuple( this_line_sytle , "\n" , this_line_extra , 0 ) );
            // last block isn't text
            if ( std::get<0>( this -> parsed.back() ) & BLOCK_TEXT && std::get<1>( this -> parsed.back() ) == "\n" && this -> parsed.size() > 1 && !( std::get<0>( this -> parsed[this->parsed.size()-2] ) & BLOCK_TEXT ) )
                this -> parsed.push_back( std::make_tuple( this_line_sytle , "\n" , this_line_extra , 0 ) );
            // \n given, but before \n isn't text
        }
            
        // empty line between text and others

        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        std::wstring wthis_line = conv.from_bytes( this_line );
        // change to wide string

        this_block.clear();

        for ( int i = 0 ; i < wthis_line.size() ; i++ )
        {
            if ( wthis_line[i] == L' ' )
            {
                if ( i != 0 && wthis_line[i-1] != ' ' )
                {
                    this_block.append( L" " );
                    this -> parsed.push_back( std::make_tuple( this_line_sytle , conv.to_bytes( this_block ) , this_line_extra , 0 ) );
                    this_block.clear();
                }
                continue;
            } // match space

            if ( wthis_line[i] == L'`' )
            {
                if ( i != 0 && wthis_line[i-1] == L'\\' )
                {
                    this_block.pop_back();
                    // delete '\'
                    this_block += wthis_line[i];
                    continue;
                } // Transliteration
                if ( !this_block.empty() )
                {
                    this -> parsed.push_back( std::make_tuple( this_line_sytle , conv.to_bytes( this_block ) , this_line_extra , 0 ) );
                    this_block.clear();
                } // push first
                if ( wthis_line.find( L'`' , i + 1 ) == i + 1 )
                {
                    size_t code_end_index = wthis_line.find( L"``" , i + 2 );
                    if ( code_end_index != std::wstring::npos )
                    {
                        this_block = wthis_line.substr( i + 2 , code_end_index - i - 2 );
                        this -> parsed.push_back( std::make_tuple( this_line_sytle | STYLE_CODE , conv.to_bytes( this_block ) , this_line_extra , 0 ) );
                        this_block.clear();
                        i = code_end_index + 1;
                        continue;
                    }
                } // double '`'
                else if ( wthis_line.find( L'`' , i + 1 ) != std::wstring::npos )
                {
                    size_t code_end_index = wthis_line.find( L'`' , i + 1 );
                    this_block = wthis_line.substr( i + 1 , code_end_index - i - 1 );
                    this -> parsed.push_back( std::make_tuple( this_line_sytle | STYLE_CODE , conv.to_bytes( this_block ) , this_line_extra , 0 ) );
                    this_block.clear();
                    i = code_end_index;
                    continue;
                } // one '`'
            } // code

            this_block += wthis_line[i];
        }

        this -> parsed.push_back( std::make_tuple( this_line_sytle , conv.to_bytes( this_block ) , this_line_extra , 0 ) );
    }
    return 0;
}

void markdown::print(){
    struct winsize size;
    ioctl( STDOUT_FILENO , TIOCGWINSZ , &size );
    this -> terminal_row = size.ws_row;
    this -> terminal_col = size.ws_col;
    // get terminal size

    size_t index = -1;

    for ( auto block : this -> parsed )
    {
        index++;
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
            std::cout << "\r " << std::get<1>( block ) << "\033[0m";
            continue;
        } // print code block
        else if ( std::get<0>( block ) & BLOCK_QUOTE )
        {
            std::cout << "\033[95m";
            if ( std::get<2>( block ) < 0 )
            {
                for ( int i = 0 ; i < abs( std::get<2>( block ) ) ; i++ )
                    std::cout << "▌ ";
                // quote line begin block has a space at beginning
                continue;
            } // print quote begin, continue
        }

        if ( std::get<0>( block ) & STYLE_CODE )
            std::cout << "\033[40m" << "\033[96m";

        std::cout << std::get<1>( block ) << "\033[0m";
    }
    return;
}