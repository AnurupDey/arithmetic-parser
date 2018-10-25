#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>


enum TokenType{
    TTNONE,
    TTNUMBER,
    TTOPERATOR,
    TTBRACKET
};

/*-----------------------------------------------------------------------------
Representation of a token. A token is the smallest individual unit of 
information that carries any meaning. These in this case are either numbers,
operators or brackets.
-----------------------------------------------------------------------------*/
struct Token{
    Token(TokenType _type, std::string _value){
        type = _type;
        value = _value;
    }

    TokenType type;
    std::string value;
};

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

    return tokens;
} 

/*-----------------------------------------------------------------------------
Program driver. 
-----------------------------------------------------------------------------*/
int main(){
    std::ifstream in("../../data/input.txt");
    if(!in){
        std::cerr << "Error opening file\n";
        return 1;
    }

    char buffer[1024];
    //load line from file
    in.getline(buffer,1024,';');
    std::string input = buffer;
    std::cout << "Buffer: " << buffer << std::endl;
    
    //generate tokens
    std::vector<Token> tokens = tokenize(input);

    for(auto& token : tokens){
        std::cout << '[' << token.type << ',' << token.value  << ']' << std::endl;
    }

    //time to parse.

    in.close();
    return 0;
}