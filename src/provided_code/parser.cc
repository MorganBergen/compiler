/**
 * @file        parser.cc
 * @date        sun june 15 2025
 * @brief
 */

#include <iostream>
#include <cstdlib>
#include "parser.h"

void Parser::syntax_error() {
    std::cout << "SYNTAX ERROR !!!!!&%!!\n";
    exit(1);
}

//  this function gets a token and checks if it is
// of the expected type.  if it is, the token is
// returned, otherwise, syntax_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule
// written by mohsen zohrevandi
Token Parser::expect(TokenType expected_type) {
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// parsing
// this function is simply to illustrate the GetToken() function
// you will not need it for your project and you can delete it
// the function also illustrates the use of peek()
void Parser::ConsumeAllInput() {
    Token token;
    int i = 1;
    token = lexer.peek(1);
    token.Print();

    while (token.token_type != END_OF_FILE) {
        i = i + 1;
        token = lexer.peek(i);
        token.Print();
    }

    token = lexer.GetToken();
    token.Print();

    while (token.token_type != END_OF_FILE) {
        token = lexer.GetToken();
        token.Print();
    }
}

int main() {
    // note:  the parser class has a lexer object instantiated in it
    // you should not be declaring a separate lexer object.
    // you can access the lexer object in the parser functions as shown in the
    // example method Parser:ConsumeAllInput
    // if you declare another lexer object, lexical analysis will not work correctly
    Parser parser;

    parser.ConsumeAllInput();
}
