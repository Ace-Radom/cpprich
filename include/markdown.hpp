#ifndef _MARKDOWN_HPP_
#define _MARKDOWN_HPP_

#include<iostream>
#include<string>
#include<stack>
#include<vector>
#include<cstdio>
#include<unistd.h>
#include<sys/ioctl.h>

class markdown {
    public:
        markdown( std::string __raw );
        void set( std::string __raw );
        void append( std::string __raw );
        int parse();
        void print();

    private:
        std::string raw;
        std::string parsed;
        size_t terminal_col;
        size_t terminal_row;
};

#endif