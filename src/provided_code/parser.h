/**
 * @file       parser.h
 * @date       sun june 15 2025
 * @brief
 */

#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <vector>
#include <map>
#include "lexer.h"

struct Expr;
struct Polynomial;
struct Statement;
struct EvalInfo;

class Parser {
    public:
        void Run();
        LexicalAnalyzer lexer;

    private:
        void syntax_error();
        Token expect(TokenType expected_type);
        int to_int(const std::string &s);

        // parsing sections
        void parse_tasks_section();
        void parse_poly_section();
        void parse_execute_section();
        void parse_inputs_section();

        // expression parsing
        Expr* term_list();
        Expr* term();
        Expr* monomial_list();
        Expr* monomial();
        Expr* primary(const std::vector<std::string>& vars, int line);
        Expr* poly_evaluation();
        Expr* argument();

        // evaluation and semantics
        int eval_expr(Expr* e, std::map<std::string,int>& env);
        void execute_program();
        void check_semantics();
        void check_warning1();
        void check_warning2();
        int degree_expr(Expr* e);
        void print_degrees();


        // program data
        std::vector<int> tasks;
        std::vector<Polynomial> polys;
        std::vector<Statement> stmts;
        std::vector<int> inputs;


        std::map<std::string, size_t> poly_map;

        std::map<std::string, Polynomial*> poly_map;

        std::vector<int> error1_lines;
        std::vector<int> error2_lines;
        std::vector<EvalInfo> eval_calls;


        std::vector<int> warn1_lines;
        std::vector<int> warn2_lines;

        std::vector<std::string> current_poly_vars;
        int current_poly_line = 0;
};

#endif
