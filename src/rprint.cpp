#include"cpprich.hpp"

const char* parse_tag( const char* __tag ){
    for ( int i = 0 ; i < DISPLAY_SET_LIST_LENGTH ; i++ )
    {
        if ( strcmp( __tag , Display_Set_List[i].tag ) == 0 )
        {
            return Display_Set_List[i].ANSI;
        }
    }
    return NULL;
}

void rprint( const std::string __format , ... ){
    va_list args;
    va_start( args , __format );
    rprint( __format , args );
    va_end( args );
    return;
}

void rprint( const std::string __format , va_list args ){
    char* buf = new char[RPRINT_MAX_LENGTH];
    vsnprintf( buf , RPRINT_MAX_LENGTH , __format.c_str() , args );
    std::string fullstr( buf );
    delete[] buf;
    rprintf( fullstr );
    return;
}

void rprintf( std::string __msg ){
    std::stack<const char*> ANSI_codes;
    std::string outstr;
    outstr.clear();
    for ( int i = 0 ; i < __msg.size() ; i++ )
    {
        if ( __msg[i] == '[' )
        {
            if ( i > 0 && __msg[i-1] == '\\' )
            {
                outstr.pop_back();
                // remove '\'
                outstr += __msg[i];
                continue;
            } // \[] appears
            int rb_index = __msg.find( ']' , i );
            // find next right bracket
            if ( rb_index == i + 1 )
            {
                i = rb_index;
                continue;
            } // [] occurs
            std::string this_tag = __msg.substr( i + 1 , rb_index - i - 1 );
            const char* this_ANSI_code;
            if ( strcmp( this_tag.c_str() , "/" ) == 0 )
            {
                if ( ANSI_codes.empty() )
                {
                    outstr += __msg[i];
                    continue;
                } // empty stack, no display to reset
                outstr += "\033[0m";
                ANSI_codes.pop();
                std::stack<const char*> ANSI_codes_copy( ANSI_codes );
                while ( !ANSI_codes_copy.empty() )
                {
                    outstr += ANSI_codes_copy.top();
                    ANSI_codes_copy.pop();
                } // copy display set codes and write them to outstr
                i = rb_index;
                continue;
            } // display reset
            if ( ( this_ANSI_code = parse_tag( this_tag.c_str() ) ) == NULL )
            {
                outstr += __msg[i];
                continue;
            } // tag not found, add the whole tag part to outstr
            outstr += this_ANSI_code;
            ANSI_codes.push( this_ANSI_code );
            i = rb_index;
            // link ANSI code, push it to stack; skip whole tag part
        }
        else
            outstr += __msg[i];
    }
    outstr += "\033[0m";
    std::cout << outstr;
}