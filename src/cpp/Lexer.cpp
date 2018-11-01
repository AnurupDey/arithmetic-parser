#include "Lexer.h"
#include <cctype>

/*-----------------------------------------------------------------------------
Helper function to extract numbers from an input string. Will Extract decimal 
numbers with a decimal point. EX: /*[0-9].*[0-9]/
PARAMETERS:
[IN]    curChar -   pointer to the location in the string the number starts.
                    Note that this pointer gets modified.
RETURNS:
        number  -   a string containing just the number extracted.
    Also, curChar will point to the last digit of the number once the function
    returns.
-----------------------------------------------------------------------------*/
std::string getNumberValue(char** curChar){
    std::string number = "";
    while((**curChar)!='\0' && (isdigit(**curChar) || (**curChar) == '.')){
        number += **curChar;
        (*curChar)++;
    }
    (*curChar)--;
    return number;
}

/*-----------------------------------------------------------------------------
This is the whole Lexer. Converts a stream of characters representing the input
expression into a sequence of tokens.
PARAMETERS:
[IN]    input   -   string containing an arithmetic expression as input.
RETURNS:
        tokens  -   the stream of tokenized input.
Each Token contains information about what type of token that is and what the 
contents/value of the token is.
-----------------------------------------------------------------------------*/
std::vector<Token> tokenize(std::string input){
    std::vector<Token> tokens;

    if(input.length() == 0 ) return tokens;
    
    for(char* curChar = &input[0]; *curChar != '\0'; curChar++){
        if(isdigit(*curChar))
        {
            tokens.emplace_back(TTNUMBER,getNumberValue(&curChar));
        } 
        else if (*curChar == '+' || *curChar == '-' || 
                 *curChar == '*' || *curChar == '/')
        {
            tokens.emplace_back(TTOPERATOR,std::string("")+(*curChar));
        } 
        else if (*curChar == '(' || *curChar == ')')
        {
            tokens.emplace_back(TTBRACKET,std::string("")+(*curChar));
        }
    }

    tokens.emplace_back(TTEND,"");
    return tokens;
} 