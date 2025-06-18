/**
 * @file        parser.cc
 * @date        sun june 15 2025
 * @brief       simple polynomial language parser
 */

#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
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

/**
 * @
 */
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
        poly_map[p.name] = polys.size() - 1;
        current_poly_vars.clear();
        current_poly_line = 0;
        t = lexer.peek(1);
    }
}

void Parser::parse_execute_section() {
    expect(EXECUTE);
    Token t = lexer.peek(1);
    if (!(t.token_type == INPUT || t.token_type == OUTPUT || t.token_type == ID)) {
        syntax_error();
    }
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
    if (t1.token_type == NUM) {
        Token num = expect(NUM);
        Expr* node = new Expr(Expr::NUM);
        node->value = to_int(num.lexeme);
        node->line_no = num.line_no;
        return node;
    }
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
            Polynomial *p = &polys[poly_map[e->name]];
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
            Polynomial *p = &polys[poly_map[e.name]];
            int expected = p->params.empty() ? 1 : (int)p->params.size();
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

// ----- warning checks -----
static void collect_uninit(Expr* e, const std::set<std::string>& defined,
                           std::vector<int>& lines) {
    if (!e) return;
    if (e->type == Expr::VAR) {
        if (defined.find(e->name) == defined.end()) {
            lines.push_back(e->line_no);
        }
    }
    for (Expr* c : e->args) {
        collect_uninit(c, defined, lines);
    }
}

void Parser::check_warning1() {
    std::set<std::string> defined;
    for (const Statement &s : stmts) {
        if (s.type == Statement::INPUT_STMT) {
            defined.insert(s.var);
        } else if (s.type == Statement::ASSIGN_STMT) {
            collect_uninit(s.expr, defined, warn1_lines);
            defined.insert(s.var);
        }
    }
    if (!warn1_lines.empty()) {
        sort(warn1_lines.begin(), warn1_lines.end());
        cout << "Warning Code 1:";
        for (int ln : warn1_lines) cout << " " << ln;
        cout << "\n";
    }
}

static void collect_used(Expr* e, std::set<std::string>& used) {
    if (!e) return;
    if (e->type == Expr::VAR) {
        used.insert(e->name);
    }
    for (Expr* c : e->args) {
        collect_used(c, used);
    }
}

void Parser::check_warning2() {
    std::set<std::string> live;
    for (int i = (int)stmts.size() - 1; i >= 0; i--) {
        const Statement &s = stmts[i];
        if (s.type == Statement::OUTPUT_STMT) {
            live.insert(s.var);
        } else if (s.type == Statement::ASSIGN_STMT) {
            if (live.find(s.var) == live.end()) {
                warn2_lines.push_back(s.line_no);
            }
            live.erase(s.var);
            collect_used(s.expr, live);
        } else if (s.type == Statement::INPUT_STMT) {
            live.erase(s.var);
        }
    }
    if (!warn2_lines.empty()) {
        sort(warn2_lines.begin(), warn2_lines.end());
        cout << "Warning Code 2:";
        for (int ln : warn2_lines) cout << " " << ln;
        cout << "\n";
    }
}

// ----- degree computation -----
int Parser::degree_expr(Expr* e, std::map<std::string,int>& env) {
    switch (e->type) {
        case Expr::NUM:
            return 0;
        case Expr::VAR: {
            auto it = env.find(e->name);
            if (it != env.end()) return it->second;
            return 1;
        }
        case Expr::ADD:
        case Expr::SUB:
            return std::max(degree_expr(e->args[0], env),
                            degree_expr(e->args[1], env));
        case Expr::MUL:
            return degree_expr(e->args[0], env) +
                   degree_expr(e->args[1], env);
        case Expr::POW:
            return degree_expr(e->args[0], env) * e->value;
        case Expr::CALL: {
            if (poly_map.find(e->name) == poly_map.end()) return 0;
            Polynomial *p = &polys[poly_map[e->name]];
            std::map<std::string,int> new_env;
            if (p->params.empty()) {
                int arg_deg = degree_expr(e->args[0], env);
                new_env["x"] = arg_deg;
            } else {
                for (size_t i = 0; i < p->params.size() && i < e->args.size(); i++) {
                    int arg_deg = degree_expr(e->args[i], env);
                    new_env[p->params[i]] = arg_deg;
                }
            }
            return degree_expr(p->body, new_env);
        }
        default:
            return 0;
    }
}

int Parser::degree_expr(Expr* e) {
    std::map<std::string,int> env;
    return degree_expr(e, env);
}

void Parser::print_degrees() {
    for (const Polynomial &p : polys) {
        cout << p.name << ": " << degree_expr(p.body) << "\n";
    }
}

void Parser::Run() {

    //  parse each section of input
    parse_tasks_section();
    parse_poly_section();
    parse_execute_section();
    parse_inputs_section();

    //  perform semantic checks and exit on error
    check_semantics();

    //  determine which tasks to execute based on teh parsed 'tasks' list
    bool t2=false, t3=false, t4=false, t5=false;
    for (int t : tasks) {
        if (t==2) t2=true;  //  task 2 - execute program
        if (t==3) t3=true;  //  task 3 - check warning 1 (unitialized variables)
        if (t==4) t4=true;  //  task 4 - check warning 2 (unused variables)
        if (t==5) t5=true;  //  task 5 - print polynomial degrees
    }
    if (t2) execute_program();
    if (t3) check_warning1();
    if (t4) check_warning2();
    if (t5) print_degrees();
}

int main() {
    Parser parser;
    parser.Run();
    return 0;
}
