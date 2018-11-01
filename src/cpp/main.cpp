#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>

#include "Parser.h"

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

void DispNode(ParseNode& node, int l = 0){
    for(int i = 0; i < l; i++) std::cout << "\t";
    std::cout << "{\n";
    for(int i = 0; i < l+1; i++) std::cout << "\t";
    std::cout << node.name << std::endl;

    if(node.tokens.size()){
        //
        for(auto& token : node.tokens){
            for(int i = 0; i < l+1; i++) std::cout << "\t";
            std::cout << '[' << token.type << ',' << token.value  << ']' << std::endl;
        }
    }

    if(node.subNodes.size()){
        for(int i = 0; i < l+1; i++) std::cout << "\t";
        std::cout << "--\n";
        for(auto& subnode : node.subNodes){
            DispNode(subnode,l+1);
        }
    }

    for(int i = 0; i < l; i++) std::cout << "\t";
    std::cout << "}\n";
}

double solve(ParseNode& node){
    if(node.name == "Expression"){
        double sum = solve(node.subNodes[0]); 
        //expressions are expected to have at least one term
        for(int i=1;i<node.subNodes.size();i++){
            if(node.tokens[i-1].value == "+")
                sum += solve(node.subNodes[i]);
            else if(node.tokens[i-1].value == "-"){
                sum -= solve(node.subNodes[i]);
            }
        }
        return sum;
    } else if (node.name == "Term"){
        double product = solve(node.subNodes[0]); 
        //Terms are expected to have at least one factor
        for(int i=1;i<node.subNodes.size();i++){
            if(node.tokens[i-1].value == "*")
                product *= solve(node.subNodes[i]);
            else if(node.tokens[i-1].value == "/"){
                product /= solve(node.subNodes[i]);
            }
        }
        return product;
    } else if (node.name == "Factor"){
        if(node.tokens.size() == 0){
            //we have one expression inside this factor
            return solve(node.subNodes[0]);
        } else if(node.tokens.size() == 1){
            //its a single number.
            return atof(node.tokens[0].value.c_str());
        } else if(node.tokens.size() == 2){
            //we have a sign followed by a number
            if(node.tokens[0].value == "-"){
                return -atof(node.tokens[1].value.c_str());
            } else {
                return atof(node.tokens[1].value.c_str());
            }
            //NOTE: skipping error check here.
        } else {
            //wuh? TODO: Error handling
        }
    } else {
        //wuh? TODO: Error handling
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

    std::cout << std::endl << std::endl;
    std::cout << "Tokens: \n";
    for(auto& token : tokens){
        std::cout << '[' << token.type << ',' << token.value  << ']' << std::endl;
    }

    //time to parse.
    Parser parser;
    ParseNode exp = parser.parse(tokens.begin());

    std::cout << std::endl << std::endl << "Parsed Tree:\n";
    DispNode(exp);

    //lets solve!
    double answer = solve(exp);
    std::cout << "\n\nAnswer = " << answer << std::endl;
    in.close();
    return 0;
}