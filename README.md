# compiler

1.  introduction

the goal of this project is to implement a simple compiler for a simple programming language.
to implement this simple compiler, you will write a recusive-decent parser and use some simple data structures to implement semantic checking and execute the program.

the input to your compiler has four parts

1.  `TASKS` section -  it contains a list of one or more numbers of tasks to be executed by the compiler
2.  `POLY` section -  it contains a list of polynomial declarations
3.  `EXECUTE` section -  it contains a secquence of `INPUT`, `OUTPUT` and assignment statements
4.  `INPUTS` section - it contains a sequence of integers that will be used as the input to `INPUT` statement in the `EXECUTE` section.

your compiler will parse the input and produces a syntax error message if there is a syntax error.  if there is no syntax error, your compiler will analyze semantic errors.  if there are no synatx and no semantic errors, your compiler will perform other semantic analyses if so specified by the tasks numbers in the `TASKS` section.  if required, it will also execute the `EXECUTE` section and produces the output that should be produces by the `OUTPUT` statement.

```
input to compiler -> part 1:  tasks to be executed                        -> compiler -> -10 -192
                     part 2:  polynomials definitions                     ->
                     part 3:  statements to execute                       ->
                     part 4:  inputs to be used by the INPUTS statements  ->

part 1: tasks to be executed
TASKS
  1

part 2:  polynomials definitions
POLY
  F = x^2 + x + 1;
  G(x, y) = 2(x + y)(x - y);

part 3: statement to execute
EXECUTE
  INPUT w;
  INPUT z;
  z = G(w, z);
  w = G(w, z);
  OUTPUT w;
  OUTPUT z;

part 4:  inputs to be used by the INPUTS statements
INPUTS
  2 3
```

the remainder of this readme is organized as follows

1.  the second section describes the input format
2.  the third section describes the expected output when the syntax or semantics are not correct
3.  the fourth section describes the output when the program syntax and semantics are correct
4.  the fifth section describes the requirements for your solution


2.  input format

2.1  grammar and tokens

the input of your program is specified by the following context-free grammar

```grammar
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

the code provided has a class `LexicalAnalyzer` with methods `GetToken()` and `peek()`.  also, an `expect()` function is provided.  your parser will use teh functions provide to `peek()` at tokens or `expect()` tokens as needed.  you must not change the provided functions.  do not modify files `inputbuf.cc`, `inputbuff.h`, `lexer.cc`, `lexer.h`.

to use the provided methods, you should first instantiate a lexer object of the class `LexicalAnalyzer` and call the methods on this instance.  you should only instantiate one lexer object.  if you try to instantiate more than one, this will result in errors.

the definitions of the tokens is given below for completeness

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

what you need to do is write a parser to parse the input according to the grammar and produce a synatx error message if tehre is a syntax error.  your program will also check for semantic errors, and depending on the tasks list, will execute more semantic checks.  to achieve that, your parser will store the program in approriate data structures that facilitate semantic analysis and allow your compiler to execute the statement list in the `execute_section`.

2.2  examples

the following are examples of input to your compiler with corresponding outputs.  the output will be explained in more detail in later sections.  each of these examples has task numbers 1 and 2 listed in the `tasks_section`.  they have the following meanings:

-  the number 1 listed means that your program should perform syntax and semantic checking
-  the number 2 listed means that your program should produce the output of the output statements if there are no syntax and no semantic error.

EXAMPLE 1

```
TASKS
  1 2
POLY
  F = x^2 + 1;
  G = x + 1;
EXECUTE
  X = F(4); Y =
  G(2);
  OUTPUT X;
  OUTPUT Y;
INPUTS
  1 2 3 18 19
```

this example shows two polynomial declarations and a EXECUTE section section in which the polynomials are evaulated with arguments 4 and 2 respectively.  the output of the program will be

17
3

this example shows two polynomial declarations and a `EXECUTE` section in which the polynomials are evaulated with arguments 4 and 2 respectively.  the output of the program will be

17
3

the sequence of numbers at the end of the (in the input_section) is ignored because there are no `INPUT` statements.










.
