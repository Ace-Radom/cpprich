#ifndef _MARKDOWN_HPP_
#define _MARKDOWN_HPP_

#include<iostream>
#include<string>
#include<sstream>
#include<stack>
#include<vector>
#include<cstdio>
#include<codecvt>
#include<cmath>
#include<locale>
#include<unistd.h>
#include<sys/ioctl.h>

#define BLOCK_TITLE  0b100000000000
#define BLOCK_TEXT   0b010000000000
#define BLOCK_CODEB  0b001000000000
#define BLOCK_QUOTE  0b000100000000
#define BLOCK_UL     0b000010000000 // unordered list
#define BLOCK_OL     0b000001000000 // ordered list
#define STYLE_STRONG 0b000000000001
#define STYLE_ITALIC 0b000000000010
#define STYLE_CODE   0b000000000100

class markdown {
    public:
        markdown( std::string __raw );
        void set( std::string __raw );
        void append( std::string __raw );
        int parse();
        void print();

    private:
        std::string raw;
        std::vector<std::tuple<unsigned int,std::string,int>> parsed;
        // <block type / style , block data , extra info (for title, quote, ul and ol)>
        size_t terminal_col;
        size_t terminal_row;

        size_t get_wstring_col_width( std::string __str );

        void pre_format();

        std::string parse_title( std::string __title , const unsigned char __level );

};

#endif