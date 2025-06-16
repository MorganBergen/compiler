/**
 * @file       parser.h
 * @date       sun june 15 2025
 * @brief
 */

#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"

class Parser {
    public:
        void ConsumeAllInput();

    public:
        LexicalAnalyzer lexer;
        void syntax_error();
        Token expect(TokenType expected_type);
};

#endif
