#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>
/*-----------------------------------------------------------------------------
Representation of a token. A token is the smallest individual unit of 
information that carries any meaning. These in this case are either numbers,
operators or brackets.
-----------------------------------------------------------------------------*/
enum TokenType{
    TTNONE,
    TTNUMBER,
    TTOPERATOR,
    TTBRACKET,
    TTEND = 0
};

struct Token{
    Token(TokenType _type, std::string _value){
        type = _type;
        value = _value;
    }

    TokenType type;
    std::string value;
};

#endif