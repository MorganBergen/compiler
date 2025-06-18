#  input format

a context free grammar defines how input to your compiler is structured.  it clearly specifies how valid programs can be written.

key terms

1.  **non terminals** - symbols on the left side of the rules. they are "expanded" into terminals and/or other non terminals.
2.  **terminals** -  actual tokens that are recognized by the lexical anlyzer (e.g. `NUM`, `ID`, `PLUS`)
3.  **rules / productions** -  define how non terminals expand

given the grammar snippet

```
poly_decl -> poly_header EQUAL poly_body SEMICOLON
poly_header -> poly_name
poly_header -> poly_name LPAREN id_list RPAREN
```

`poly_decl`, `poly_header`, `poly_body`, `id_list` are non terminals
`EQUAL`, `SEMICOLON`, `LPAREN`, `RPAREN`, are terminals (actual tokens)
the grammar clearly defines how a polynomial declaration must appear

how cfg connects to the recrusive descent parser

a recursive descent parser implements a cfg by having one parsing function per non terminal in your grammar

example implementation

the function to parser a `poly_decl` might look like this

```c++
void parse_poly_decl() {
  parse_poly_header();    //  calls the parser for poly_header
  expect(EQUAL);          //  expects an '=' token
  parse_poly_body();      //  calls the parser for poly_body
  expect(SEMICOLON);      //expects a ';' token
}
```

each grammar rule corresponds to such a parsing function
the parser checks syntax correctness by ensuring tokens match expected patterns

`lexer`

defines `LexicalAnalyzer` class with methods

`GetToken()` returns next token
`peek()` looks ahead without consuming tokens
`expect()` checks and consumes expected token

`inputbuf`

handles input buffering and tokenization details

`parser`

skeleton to implement your recursive descent parser.  it calls lexer functions to read tokens

cfg rules -> guide implementation in `parser.cc`

lexer functions `GetToken()`, `peek()`, `expect()` -> used by your parser functions to recognize terminals

###  project implementation overview

**parsing**

-  create a parsing function for each non terminal in your context free grammar
-  use provided lexer methods `peek`, `expect` to check token types

**semantic checking**

-  check polynomial declarations for semantic correctness
-  duplicate polynomial names
-  undeclared variables
-  incorrect polynomial evaluations (wrong number of arguments, undeclared polynomials)

**semantic warnings**

-  warnings for uninitialized variables
-  warnings for useless assignments

**execution**

-  represent polynomials in a data structure
-  allocate memory for variables
-  execute the program represented in your data structure

###  data structures needed

1.  **symbol table**  -  maps variables / polynomials to memory locations
2.  **ast** **(abstract syntax tree)** -  represents parsed program statements (especially polynomials and assignments) to execute later
2.  **vector/list** **for INPUTS** -  holds integers read from the `INPUTS` section

###  example to understand parsing flow

consider an input

```
TASKS
  1 2
POLY
  F = x^2 + 1;
EXECUTE
  INPUT X;
  Y = F(X);
  OUTPUT Y;
INPUTS
  3
```

parsing flow

```
parse_program()
    parse_tasts_section()
        expect(TASKS), parse_num_list()
    parse_poly_section()
        expect(POLY), parse_poly_decl_list()
            parse_poly_decl()
                parse_poly_header()
                expect(EQUAL)
                parse_poly_body()
                expect(SEMICOLON)
    parse_execute_section()
        expect(EXECUTE), parse_statement_list()
            parse_input_statement()
            parse_assign_statement()
            parse_output_statement()
    parse_inputs_section()
        expect(INPUTS), parse_num_list()
```

####  grammar explained step by step


```
 1 -> | TASKS             |
 2    |   1 2             |
 3    | POLY              |
 4    |   F = x^2 + 1;    |
 5    |   G = x + 1;      |
 6    | EXECUTE           |
 7    |   X = F(4);       |
 8    |   Y = G(2);       |
 9    |   OUTPUT X;       |
10    |   OUTPUT Y;       |
11    | INPUTS            |
12    |   1 2 3 18 19     |
```

####  grammar

the input of the program is specified by the following context free grammar

```
program -> tasks_sectionpoly_sectionexecute_sectioninputs_section
tasks_section -> TASKSnum_list
num_list -> NUM
poly_section -> POLYpoly_decl_list
poly_dec_list -> poly_decl
poly_decl -> poly_headerEQUALpoly_bodySEMICOLON
poly_header -> poly_name
poly_header -> poly_nameLPARENid_listRPAREN
id_list -> ID
id_list -> IDCOMMAid_list
poly_name -> ID
poly_body -> term_list
term_list -> terms
term_list -> termadd_operatorterm_list
term -> monomial_list
term -> coefficientmonomial_list
term -> coefficient
monomial_list -> monomial
monomial_list -> monomialmonomial_list
monomial -> primary
monomial -> primaryexponent
primary -> ID
primary -> LPARENterm_listRPAREN
exponent -> POWERNUM
add_operator -> PLUS
add_operator -> MINUS
coefficient -> NUM
execute_section -> EXECUTEstatement_list
statement_list -> statement
statement_list -> statementstatement_list
statement -> input_statement
statement -> output_statement
input_statement -> INPUTIDSEMICOLON
output_statement -> OUTPUTIDSEMICOLON
assign_statement -> IDEQUALpoly_evaluationSEMICOLON
poly_evaluation -> poly_nameLPARENargument_listRPAREN
argument_list -> argument
argument_list -> argumentCOMMAlargument_list
argument -> ID
argument -> poly_evaluation
inputs_section -> INPUTSnum_list
```
```
program → tasks_sectionpoly_sectionexecute_sectioninputs_section
tasks_section → TASKSnum_list
num_list → NUM
num_list → NUMnum_list
poly_section → POLYpoly_decl_list
poly_dec_list → poly_decl
poly_dec_list → poly_declpoly_dec_list
poly_decl → poly_headerEQUALpoly_bodySEMICOLON
poly_header → poly_name
poly_header → poly_nameLPARENid_listRPAREN
id_list → ID
id_list → IDCOMMAid_list
poly_name → ID
poly_body → term_list
term_list → term
term_list → termadd_operatorterm_list
term → monomial_list
term → coefficientmonomial_list
term → coefficient
monomial_list → monomial
monomial_list → monomialmonomial_list
monomial → primary
monomial → primaryexponent
primary → ID
primary → LPARENterm_listRPAREN
exponent → POWERNUM
add_operator → PLUS
add_operator → MINUS
coefficient → NUM
execute_section → EXECUTEstatement_list
statement_list → statement
statement_list → statementstatement_list
statement → input_statement
statement → output_statement
statement → assign_statement
input_statement → INPUTIDSEMICOLON
output_statement → OUTPUTIDSEMICOLON
assign_statement → IDEQUALpoly_evaluationSEMICOLON
poly_evaluation → poly_nameLPARENargument_listRPAREN
argument_list → argument
argument_list → argumentCOMMAargument_list
argument → ID
argument → NUM
argument → poly_evaluation
inputs_section → INPUTSnum_list
```

####  tokens

the following is a definition of tokens

```
char        = a | b | ... | z | A | B | ... | Z | 0 | 1 | ... | 192
letter      = a | b | ... | z | A | B | ... | z
pdigit      = 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
digit       = 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

SEMICOLON   = ;
COMMA       = ,
PLUS        = +
MINUX       = -
POWER       = ^
EQUAL       = =
LPAREN      = (
RPAREN      = )
TASKS       = (T).(A).(S).(K).(S)
POLY        = (P).(O).(L).(Y)
EXECUTE     = (E).(X).(E).(C).(U).(T).(E)
INPUT       = (I).(N).(P).(U).(T)
OUTPUT      = (O).(U).(T).(P).(U).(T)
INPUTS      = (I).(N).(P).(U).(T).(S)
NUM         = 0 | pdigit . digit*
ID          = letter . char*
```
