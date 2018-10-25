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
struct Token{
    Token(TokenType _type, std::string _value){
        type = _type;
        value = _value;
    }

    TokenType type;
    std::string value;
};

std::string getNumberValue(char** curChar){
    std::string number = "";
    while((**curChar)!='\0' && (isdigit(**curChar) || (**curChar) == '.')){
        number += **curChar;
        (*curChar)++;
    }
    (*curChar)--;
    return number;
}

std::vector<Token> tokenize(std::string input){
    std::vector<Token> tokens;
    LexState state = LSNONE;

    if(input.length() == 0 ) return tokens;
    
    for(char* curChar = &input[0]; *curChar != '\0'; curChar++){
        if(isdigit(*curChar)){
            tokens.emplace_back(TTNUMBER,getNumberValue(&curChar));
        } else if (*curChar == '+' || *curChar == '-' || *curChar == '*' || *curChar == '/'){
            tokens.emplace_back(TTOPERATOR,std::string("")+(*curChar));
        } else if (*curChar == '(' || *curChar == ')'){
            tokens.emplace_back(TTBRACKET,std::string("")+(*curChar));
        }
    }

    return tokens;
} 


int main(){
    std::ifstream in("../../data/input.txt");
    if(!in){
        std::cerr << "Error opening file\n";
        return 1;
    }

    char buffer[1024];
    in.getline(buffer,1024,';');
    std::string input = buffer;
    std::cout << "Buffer: " << buffer << std::endl;
    std::vector<Token> tokens = tokenize(input);
    for(auto& token : tokens){
        std::cout << '[' << token.type << ',' << token.value  << ']' << std::endl;
    }

    in.close();
    return 0;
}