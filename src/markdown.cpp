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
        if ( bpos == std::string::npos && epos == std::string::npos )
            continue;
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

        unsigned int this_line_style = BLOCK_TEXT; // line style, for those have consistent style but lots of blocks
        int this_line_extra = 0;                   // line style extra

#pragma region QUOTE

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
            this_line_style = BLOCK_QUOTE;
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
            this -> parsed.push_back( std::make_tuple( this_line_style , "\n" , this_line_extra ) );
            continue;
        }

        if ( this_line_style == BLOCK_TEXT )
        {
            if ( !( std::get<0>( this -> parsed.back() ) & BLOCK_TEXT ) )
                this -> parsed.push_back( std::make_tuple( this_line_style , "\n" , this_line_extra ) );
            // last block isn't text
            if ( std::get<0>( this -> parsed.back() ) & BLOCK_TEXT && std::get<1>( this -> parsed.back() ) == "\n" && this -> parsed.size() > 1 && !( std::get<0>( this -> parsed[this->parsed.size()-2] ) & BLOCK_TEXT ) )
                this -> parsed.push_back( std::make_tuple( this_line_style , "\n" , this_line_extra ) );
            // \n given, but before \n isn't text
            size_t space_num = this_line.find_first_not_of( ' ' );
            if ( space_num == std::string::npos )
                continue;
            // only contains spaces, continue (but it acutally shouldn't happen)
            if ( space_num != 0 )
            {
                std::string spaces;
                spaces.append( space_num , ' ' );
                this -> parsed.push_back( std::make_tuple( this_line_style , spaces , this_line_extra ) );
                this_line.erase( 0 , space_num );
            } // push spaces to parsed, delete them in original this_line
        } // empty line between text and others

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
                    this -> parsed.push_back( std::make_tuple( this_line_style , conv.to_bytes( this_block ) , this_line_extra ) );
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
                } // Blackslash Transliteration
                if ( !this_block.empty() )
                {
                    this -> parsed.push_back( std::make_tuple( this_line_style , conv.to_bytes( this_block ) , this_line_extra ) );
                    this_block.clear();
                } // push first
                if ( wthis_line.find( L'`' , i + 1 ) == i + 1 )
                {
                    size_t code_end_index = wthis_line.find( L"``" , i + 2 );
                    if ( code_end_index != std::wstring::npos )
                    {
                        this_block = wthis_line.substr( i + 2 , code_end_index - i - 2 );
                        this -> parsed.push_back( std::make_tuple( this_line_style | STYLE_CODE , conv.to_bytes( this_block ) , this_line_extra ) );
                        this_block.clear();
                        i = code_end_index + 1;
                        continue;
                    }
                } // double '`'
                else if ( wthis_line.find( L'`' , i + 1 ) != std::wstring::npos )
                {
                    size_t code_end_index = wthis_line.find( L'`' , i + 1 );
                    this_block = wthis_line.substr( i + 1 , code_end_index - i - 1 );
                    this -> parsed.push_back( std::make_tuple( this_line_style | STYLE_CODE , conv.to_bytes( this_block ) , this_line_extra ) );
                    this_block.clear();
                    i = code_end_index;
                    continue;
                } // one '`'
            } // code

            if ( wthis_line[i] == L'*' )
            {
                if ( i != 00 && wthis_line[i-1] == L'\\' )
                {
                    this_block.pop_back();
                    // delete '\'
                    this_block += wthis_line[i];
                    continue;
                } // Blachslash Transliteration
                if ( !this_block.empty() )
                {
                    this -> parsed.push_back( std::make_tuple( this_line_style , conv.to_bytes( this_block ) , this_line_extra ) );
                    this_block.clear();
                } // push first

                unsigned int this_block_style = STYLE_ITALIC;
                std::wstring this_block_end_sign;
                this_block_end_sign = L"*";
                if ( wthis_line.find( L'*' , i + 1 ) == i + 1 )
                {
                    this_block_style = STYLE_BOLD;
                    this_block_end_sign = L"**";
                    if ( wthis_line.find( L'*' , i + 2 ) == i + 2 )
                    {
                        this_block_style |= STYLE_ITALIC;
                        this_block_end_sign = L"***";
                    }
                } // judge bold / bold + italic style
                size_t style_end_index = wthis_line.find( this_block_end_sign , i + 1 );
                std::cout << style_end_index << std::endl;
                if ( style_end_index != std::wstring::npos )
                {
                    this_block = wthis_line.substr( i + this_block_end_sign.size() , style_end_index - i - this_block_end_sign.size() );
                    std::cout << conv.to_bytes( this_block ) << std::endl;
                    this -> parsed.push_back( std::make_tuple( this_line_style | this_block_style , conv.to_bytes( this_block ) , this_line_extra ) );
                    this_block.clear();
                    i = style_end_index + this_block_end_sign.size() - 1;
                    continue;
                }
            } // bold, italic, bold + italic

            this_block += wthis_line[i];
        }

        this -> parsed.push_back( std::make_tuple( this_line_style , conv.to_bytes( this_block ) , this_line_extra ) );
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

        if ( std::get<0>( block ) & STYLE_BOLD )
            std::cout << "\033[1m";
        
        if ( std::get<0>( block ) & STYLE_ITALIC )
            std::cout << "\033[3m";

        std::cout << std::get<1>( block ) << "\033[0m";
    }
    return;
}