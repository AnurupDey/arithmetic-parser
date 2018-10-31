#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>

#include "ParseTree.h"

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


#define CRASH() {std::cout << "Controlled Crash\n"; int* ptr = nullptr; *ptr = 0;};

ParseNode parsePrimary(std::vector<Token>::iterator& tokenIt);

ParseNode parseTerm(std::vector<Token>::iterator& tokenIt){
    ParseNode node;
    node.name = "Term";
    node.subNodes.push_back(parsePrimary(tokenIt));
    if(tokenIt->type != TTEND){
        if(tokenIt->type==TTBRACKET && tokenIt->value == ")"){
            tokenIt++;
        } else if(tokenIt->type == TTOPERATOR){
            if(tokenIt->value == "*" || tokenIt->value == "/"){
                node.tokens.push_back(*tokenIt);
                tokenIt++;
                node.subNodes.push_back(parseTerm(tokenIt));
            } else CRASH();
        } else CRASH();
    }
    return node;
}

ParseNode parseExpression(std::vector<Token>::iterator& tokenIt){
    ParseNode node;
    node.name = "Expression";
    node.subNodes.push_back(parseTerm(tokenIt));
    if(tokenIt->type != TTEND && (tokenIt->type!=TTBRACKET && tokenIt->value != ")")){
        if(tokenIt->type == TTOPERATOR){
            if(tokenIt->value == "+" || tokenIt->value == "-"){
                node.tokens.push_back(*tokenIt);
                tokenIt++;
                node.subNodes.push_back(parseExpression(tokenIt));
            } else CRASH();
        } else CRASH();
    }
    return node;
}

ParseNode parsePrimary(std::vector<Token>::iterator& tokenIt){
    ParseNode node;
    node.name = "Primary";
    if(tokenIt->type == TTNUMBER){
        node.tokens.push_back(*tokenIt);
        tokenIt++;
    } else if(tokenIt->type == TTOPERATOR){
        if(tokenIt->value == "+" || tokenIt->value == "-"){
            node.tokens.push_back(*tokenIt);
            tokenIt++;
            if(tokenIt->type == TTNUMBER){
                node.tokens.push_back(*tokenIt);
                tokenIt++;
            } else CRASH();
        }
    } else if(tokenIt->type == TTBRACKET && tokenIt->value == "("){
        tokenIt++;
        node.subNodes.push_back(parseExpression(tokenIt));
        tokenIt++; //skip the closing bracket.
    } else CRASH();
    return node;
}

ParseNode parse(std::vector<Token>::iterator tokenIt){
    ParseNode curNode = parseExpression(tokenIt);
    return curNode;
}


void DispNode(ParseNode& node, int l = 0){
    for(int i = 0; i < l; i++) std::cout << "\t";
    std::cout << "Node: \n";
    for(int i = 0; i < l; i++) std::cout << "\t";
    std::cout << "Name: " << node.name << std::endl;

    if(node.tokens.size()){
        for(int i = 0; i < l; i++) std::cout << "\t";
        std::cout << "Tokens: \n";
        for(auto& token : node.tokens){
            for(int i = 0; i < l; i++) std::cout << "\t";
            std::cout << '[' << token.type << ',' << token.value  << ']' << std::endl;
        }
    }

    if(node.subNodes.size()){
        for(int i = 0; i < l; i++) std::cout << "\t";
        std::cout << "SubNodes: \n";
        for(auto& subnode : node.subNodes){
            DispNode(subnode,l+1);
        }
    }
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
    ParseNode exp = parse(tokens.begin());
    DispNode(exp);
    in.close();
    return 0;
}