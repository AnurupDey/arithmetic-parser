#include <iostream>
#include <fstream>
#include <vector>

#include "Lexer.h"
#include "Parser.h"

/*-----------------------------------------------------------------------------
Helper function to display the subtree represented by node. 
PARAMETERS:
[IN]    node    -   node representing the subtree to display
[IN]    l       -   level of indentation to display at
-----------------------------------------------------------------------------*/
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

/*-----------------------------------------------------------------------------
Solves the expression represented by the given expression Parsed Tree
PARAMETRS:
[IN]    node    -   root node to the expression tree to solve
RETURNS:
        the result of the arithmetic expression.
-----------------------------------------------------------------------------*/
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