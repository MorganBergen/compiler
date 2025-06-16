/**
 * @file        parser.cc
 * @date        sun june 15 2025
 * @brief       Simple polynomial language parser and evaluator
 */

#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include "parser.h"

using namespace std;

// ----- utility structures -----
struct Expr {
    enum Type { NUM, VAR, ADD, SUB, MUL, POW, CALL } type;
    int value = 0;                 // for NUM or exponent for POW
    string name;                   // for VAR or CALL
    vector<Expr*> args;            // children / arguments
    int line_no = 0;               // for error reporting
    Expr(Type t) : type(t) {}
};

struct Polynomial {
    string name;
    vector<string> params;
    Expr* body = nullptr;
    int line_no = 0;
};

struct Statement {
    enum Type { INPUT_STMT, OUTPUT_STMT, ASSIGN_STMT } type;
    string var;
    Expr* expr = nullptr;          // for assignment
    int line_no = 0;
};

struct EvalInfo {                  // for semantic checks on calls
    string name;
    int arg_count = 0;
    int line_no = 0;
};

// ----- parser implementation -----
void Parser::syntax_error() {
    cout << "SYNTAX ERROR !!!!!&%!!\n";
    exit(1);
}

Token Parser::expect(TokenType expected_type) {
    Token t = lexer.GetToken();
    if (t.token_type != expected_type) {
        syntax_error();
    }
    return t;
}

int Parser::to_int(const string &s) {
    return stoi(s);
}

// forward declarations are provided in parser.h

// ----- parsing helpers -----
void Parser::parse_tasks_section() {
    expect(TASKS);
    Token t = expect(NUM);
    tasks.push_back(to_int(t.lexeme));
    while (lexer.peek(1).token_type == NUM) {
        t = expect(NUM);
        tasks.push_back(to_int(t.lexeme));
    }
}

void Parser::parse_poly_section() {
    expect(POLY);
    Token t = lexer.peek(1);
    while (t.token_type == ID) {
        Polynomial p;
        Token nameTok = expect(ID);
        p.name = nameTok.lexeme;
        p.line_no = nameTok.line_no;
        if (poly_map.count(p.name)) {
            error1_lines.push_back(nameTok.line_no);
        }
        Token next = lexer.peek(1);
        if (next.token_type == LPAREN) {
            expect(LPAREN);
            Token idTok = expect(ID);
            p.params.push_back(idTok.lexeme);
            while (lexer.peek(1).token_type == COMMA) {
                expect(COMMA);
                idTok = expect(ID);
                p.params.push_back(idTok.lexeme);
            }
            expect(RPAREN);
        }
        expect(EQUAL);
        current_poly_vars = p.params; // track for invalid variable check
        current_poly_line = nameTok.line_no;
        p.body = term_list();
        expect(SEMICOLON);
        polys.push_back(p);
        poly_map[p.name] = &polys.back();
        current_poly_vars.clear();
        current_poly_line = 0;
        t = lexer.peek(1);
    }
}

void Parser::parse_execute_section() {
    expect(EXECUTE);
    Token t = lexer.peek(1);
    while (t.token_type == INPUT || t.token_type == OUTPUT || t.token_type == ID) {
        Statement s;
        if (t.token_type == INPUT) {
            lexer.GetToken();
            Token id = expect(ID);
            expect(SEMICOLON);
            s.type = Statement::INPUT_STMT;
            s.var = id.lexeme;
            s.line_no = id.line_no;
        } else if (t.token_type == OUTPUT) {
            lexer.GetToken();
            Token id = expect(ID);
            expect(SEMICOLON);
            s.type = Statement::OUTPUT_STMT;
            s.var = id.lexeme;
            s.line_no = id.line_no;
        } else {
            Token id = expect(ID);
            expect(EQUAL);
            s.type = Statement::ASSIGN_STMT;
            s.var = id.lexeme;
            s.expr = poly_evaluation();
            expect(SEMICOLON);
            s.line_no = id.line_no;
        }
        stmts.push_back(s);
        t = lexer.peek(1);
    }
}

void Parser::parse_inputs_section() {
    expect(INPUTS);
    Token t = expect(NUM);
    inputs.push_back(to_int(t.lexeme));
    while (lexer.peek(1).token_type == NUM) {
        t = expect(NUM);
        inputs.push_back(to_int(t.lexeme));
    }
    expect(END_OF_FILE);
}

// ----- grammar functions for expressions -----
Expr* Parser::term_list() {
    Expr* left = term();
    Token t = lexer.peek(1);
    if (t.token_type == PLUS || t.token_type == MINUS) {
        Token op = lexer.GetToken();
        Expr* right = term_list();
        Expr* node = new Expr(op.token_type == PLUS ? Expr::ADD : Expr::SUB);
        node->line_no = op.line_no;
        node->args.push_back(left);
        node->args.push_back(right);
        return node;
    } else {
        return left;
    }
}

Expr* Parser::term() {
    Token t = lexer.peek(1);
    if (t.token_type == NUM) {
        Token coeffTok = expect(NUM);
        t = lexer.peek(1);
        if (t.token_type == ID || t.token_type == LPAREN) {
            Expr* mono = monomial_list();
            Expr* cnode = new Expr(Expr::NUM);
            cnode->value = to_int(coeffTok.lexeme);
            cnode->line_no = coeffTok.line_no;
            Expr* mul = new Expr(Expr::MUL);
            mul->args.push_back(cnode);
            mul->args.push_back(mono);
            mul->line_no = coeffTok.line_no;
            return mul;
        } else {
            Expr* node = new Expr(Expr::NUM);
            node->value = to_int(coeffTok.lexeme);
            node->line_no = coeffTok.line_no;
            return node;
        }
    } else {
        return monomial_list();
    }
}

Expr* Parser::monomial_list() {
    Expr* first = monomial();
    Token t = lexer.peek(1);
    while (t.token_type == ID || t.token_type == LPAREN) {
        Expr* next = monomial();
        Expr* mul = new Expr(Expr::MUL);
        mul->args.push_back(first);
        mul->args.push_back(next);
        mul->line_no = next->line_no;
        first = mul;
        t = lexer.peek(1);
    }
    return first;
}

Expr* Parser::monomial() {
    Expr* base = nullptr;
    Token t = lexer.peek(1);
    if (t.token_type == ID || t.token_type == LPAREN) {
        base = primary(current_poly_vars, current_poly_line);
    } else {
        syntax_error();
    }
    t = lexer.peek(1);
    if (t.token_type == POWER) {
        Token powTok = lexer.GetToken();
        Token numTok = expect(NUM);
        Expr* node = new Expr(Expr::POW);
        node->args.push_back(base);
        node->value = to_int(numTok.lexeme);
        node->line_no = powTok.line_no;
        return node;
    } else {
        return base;
    }
}

Expr* Parser::primary(const vector<string>& vars, int pline) {
    Token t = lexer.peek(1);
    if (t.token_type == ID) {
        Token id = expect(ID);
        Expr* node = new Expr(Expr::VAR);
        node->name = id.lexeme;
        node->line_no = id.line_no;
        bool ok = false;
        for (const string &v : vars) if (v == id.lexeme) ok = true;
        if (vars.empty() && id.lexeme == "x") ok = true;
        if (!ok) error2_lines.push_back(id.line_no);
        return node;
    } else if (t.token_type == LPAREN) {
        expect(LPAREN);
        Expr* e = term_list();
        expect(RPAREN);
        return e;
    } else {
        syntax_error();
        return nullptr;
    }
}

Expr* Parser::poly_evaluation() {
    Token nameTok = expect(ID);
    expect(LPAREN);
    vector<Expr*> args;
    args.push_back(argument());
    while (lexer.peek(1).token_type == COMMA) {
        expect(COMMA);
        args.push_back(argument());
    }
    expect(RPAREN);
    EvalInfo info; info.name = nameTok.lexeme; info.arg_count = (int)args.size(); info.line_no = nameTok.line_no;
    eval_calls.push_back(info);
    Expr* node = new Expr(Expr::CALL);
    node->name = nameTok.lexeme;
    node->args = args;
    node->line_no = nameTok.line_no;
    return node;
}

Expr* Parser::argument() {
    Token t1 = lexer.peek(1);
    if (t1.token_type != ID) {
        syntax_error();
    }
    Token t2 = lexer.peek(2);
    if (t2.token_type == LPAREN) {
        return poly_evaluation();
    } else {
        Token id = expect(ID);
        Expr* node = new Expr(Expr::VAR);
        node->name = id.lexeme;
        node->line_no = id.line_no;
        return node;
    }
}

// ----- evaluation functions -----
int Parser::eval_expr(Expr* e, map<string,int>& env) {
    switch (e->type) {
        case Expr::NUM:
            return e->value;
        case Expr::VAR:
            return env[e->name];
        case Expr::ADD:
            return eval_expr(e->args[0], env) + eval_expr(e->args[1], env);
        case Expr::SUB:
            return eval_expr(e->args[0], env) - eval_expr(e->args[1], env);
        case Expr::MUL:
            return eval_expr(e->args[0], env) * eval_expr(e->args[1], env);
        case Expr::POW: {
            int base = eval_expr(e->args[0], env);
            int res = 1;
            for (int i = 0; i < e->value; i++) res *= base;
            return res;
        }
        case Expr::CALL: {
            if (poly_map.find(e->name) == poly_map.end()) return 0;
            Polynomial *p = poly_map[e->name];
            map<string,int> newEnv;
            if (p->params.empty()) {
                newEnv["x"] = eval_expr(e->args[0], env);
            } else {
                for (size_t i = 0; i < p->params.size() && i < e->args.size(); i++) {
                    newEnv[p->params[i]] = eval_expr(e->args[i], env);
                }
            }
            return eval_expr(p->body, newEnv);
        }
    }
    return 0;
}

void Parser::execute_program() {
    size_t input_pos = 0;
    map<string,int> mem;
    for (const Statement &s : stmts) {
        if (s.type == Statement::INPUT_STMT) {
            int val = (input_pos < inputs.size()) ? inputs[input_pos] : 0;
            input_pos++;
            mem[s.var] = val;
        } else if (s.type == Statement::OUTPUT_STMT) {
            cout << mem[s.var] << "\n";
        } else {
            map<string,int> env;
            for (const auto &p : mem) env[p.first] = p.second;
            int val = eval_expr(s.expr, env);
            mem[s.var] = val;
        }
    }
}

// ----- semantic checks -----
void Parser::check_semantics() {
    if (!error1_lines.empty()) {
        sort(error1_lines.begin(), error1_lines.end());
        cout << "Semantic Error Code 1:";
        for (int ln : error1_lines) cout << " " << ln;
        cout << "\n";
        exit(0);
    }
    if (!error2_lines.empty()) {
        sort(error2_lines.begin(), error2_lines.end());
        cout << "Semantic Error Code 2:";
        for (int ln : error2_lines) cout << " " << ln;
        cout << "\n";
        exit(0);
    }
    vector<int> error3, error4;
    for (const EvalInfo &e : eval_calls) {
        if (poly_map.find(e.name) == poly_map.end()) {
            error3.push_back(e.line_no);
        } else {
            int expected = poly_map[e.name]->params.empty() ? 1 : (int)poly_map[e.name]->params.size();
            if (e.arg_count != expected) error4.push_back(e.line_no);
        }
    }
    if (!error3.empty()) {
        sort(error3.begin(), error3.end());
        cout << "Semantic Error Code 3:";
        for (int ln : error3) cout << " " << ln;
        cout << "\n";
        exit(0);
    }
    if (!error4.empty()) {
        sort(error4.begin(), error4.end());
        cout << "Semantic Error Code 4:";
        for (int ln : error4) cout << " " << ln;
        cout << "\n";
        exit(0);
    }
}

// ----- driver -----
void Parser::Run() {
    parse_tasks_section();
    parse_poly_section();
    parse_execute_section();
    parse_inputs_section();
    check_semantics();
    // run task 2 if requested
    bool t2 = false;
    for (int t : tasks) if (t == 2) t2 = true;
    if (t2) {
        execute_program();
    }
}

int main() {
    Parser parser;
    parser.Run();
    return 0;
}
