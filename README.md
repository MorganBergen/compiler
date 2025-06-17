#  simple compiler

####  contents

[introduction](#introduction)

###  introduction

a simple compiler using a recursive descent parser along with simple data structures for semantic checking and program execution.

**compiler input**

1.  `TASKS` section - contains a list of one or more numbers of tasks to be executed by the compiler
2.  `POLY` section -  contains a list of polynomial declarations
3.  `EXECUTE` section - contains a sequence of `INPUT`, `OUTPUT`, and assignment statements
4.  `INPUTS` section - contains a sequence of integers used as inputs for the `EXECUTE` section

**compiler behavior**

1.  parse input
2.  if syntax error exists, produce synatx error message
3.  if syntax is correct, perform semantic analysis
4.  if semantic errors exist, produce semantic error messages
5.  if syntax and semantics are correct, performa additional semantic analysis based on specified tasks
6.  execute the statements in the `EXECUTE` section
7.  generate output for `OUTPUT` statements

###  input format

**grammar and tokens**

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

**definition of tokens**

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

the code provided has a class `LexicalAnalyzer` with methods `GetToken()`, `peek()`, and `expect()`.

the parser will use the functions provided to `peek()` at tokens or `expect()` tokens as needed.  do not change any functions provided in `inputbuf.cc`, `inputbuf.h`, `lexer.cc`, `lexer.h`

to use the provided methods, first instantiate only one lexer object of the class `LexicalAnalyzer` and call the methods on this instance.

what you need to do is write a parser to parse the input according to the context free grammar and produce a syntax error

###  output if syntax or semantic erros exist


###  output if syntax or semantics are correct

###  requirements for solution
