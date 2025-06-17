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

--------------------------------------------




EXAMPLE 2

```
TASKS
  1 2
POLY
  F = x^2 + 1;
  G = x + 1;
EXECUTE
  INPUT X; INPUT
  Y;
  X = F(X); Y =
  G(Y);
  OUTPUT X;
INPUTS
  1 2 3 18 19
```

This is similar to the previous example, but here we have two INPUT statements. The first INPUT statement
reads a value for X from the sequence of numbers and X gets the value 1. The second INPUT statement reads a
value for Y which gets the value 2. Here the output will be 2
Note that the values 3, 18 and 19 are not read and do not affect the execution of the program.



EXAMPLE 3

```
1: TASKS
2:    1 2
3: POLY
4:    F = x^2 + 1;
5:    G = x + 1;
6: EXECUTE
7:    INPUT X;
8:    INPUT Y;
9:    X = F(X);
10:   Y = G(Y);
11:   OUTPUT X;
12: INPUTS:
13:   1 2 3 18 19
```

Note that there are line numbers added to this example. These line numbers are not part of the input and are added only to refer to specific lines of the program. In this example, which looks almost the same as the previous example, there is a syntax error because there is a missing semicolon on line 4. The output of the program should be SYNTAX ERROR !!!!!&%!!

EXAMPLE 4

```
1: TASKS
2:    1 2
3: POLY
4:    F = x^2 + 1;
5:    G(X,Y) = X Y^2 + X Y;
6: EXECUTE
7:    INPUT Z;
8:    INPUT W;
9:    X = F(Z);
10:   Y = G(Z,W);
11:   OUTPUT X;
12:   OUTPUT Y;
12: INPUTS
13:    1 2 3 18 19
```

In this example, the polynomial G has two variables which are given explicitly (in the absence of explicitly named variables, the variable is lower case x by default). The output is

```
2
6
```

EXAMPLE 5

```
1: TASKS
2:    1 2
3: POLY
4:    F = x^2 + 1;
5:    G(X,Y) = X Y^2 + X Z;
6: EXECUTE
7:    INPUT Z;
8:    INPUT W;
9:    X = F(Z);
10:   Y = G(Z,W);
11:   OUTPUT X;
12:   OUTPUT Y;
12: INPUTS
13:   1 2 3 18 19
```

This example is similar to the previous one but it has a problem. The polynomial G is declared with two variables X and Y but its equation (called poly_body in the grammar) has Z which is different from X and Y . The output captures this error (see below for error codes and their format) Semantic Error Code 2: 5

3 Tasks and their priorities
The task numbers specify what your program should do with the input program. Task 1 is one of the larger tasks and,
but it is not graded as one big task. Task 1 has the following functionalities:
1. Syntax checking
2. Semantic error checkings
The other tasks, 2, 3, 4, 5 and 6 have the following functionalities:
• Task 2 – Output: Task 2 requires your compiler to produce the output that should be produced by the output
statements of the program. .
• Task 3 – Variable used but not explicitly initialized: Task 3 requires your compiler to produce a warning about
uninitialized variables. A variable is uninitialized when a variable appears on the right-hand side of an
assignment statement without having previously appeared on the left-hand side of an assignment statement
or in an INPUT statement. This will result in a warning message. However, it is not considered a semantic error.
The execution can proceed assuming the variable is initially zero.
• Task 4 – Useless assignments: This happens when a variable value is calculated, but the variable is not used
later in the right-hand side of an assignment or in an OUTPUT statement.
• Task 5 – Polynomial degree: This task requires that the degree of all the polynomials in the polynomial sections
are calculated and outputted.
Detailed descriptions of these tasks and what the output should be for each of them is given in the sections that
follow. The remainder of this section explains what the output of your program should be when multiple task
numbers are listed in the tasks_section.
If task 1 is listed in the tasks_section, then task 1 should be executed. Remember that task 1 performs syntax error
checking and semantic error checking. If the execution of task 1 results in an error, and task 1 is listed in the
tasks_section, then your program should only output the error messages (as described below) and exits. If task 1
results in an error (syntax or semantic) no other tasks will be executed even if they are listed in the tasks_section. If
task 1 is listed in the tasks_section and does not result in an error message, then task 1 produces no output. In that
case, the outputs of the other tasks that are listed in tasks_section should be produced by the program. The order of
these outputs should be according to the task numbers. So, first the output of task 2 is produced (if task 2 is listed in
tasks_section), then the output of task 3 is produced (if task 3 is listed in tasks_section) and so on.
If task 1 is not listed in the tasks_section, task 1 still needs to be executed. If task 1's execution results in an error,
then your program should output nothing in this case. If task 1 is not listed and task 1's execution does not result in
an error, then the outputs of the other tasks that are listed in tasks_section should be produced by the program. The
order of these outputs should be according to the task numbers. So, first the output of task 2 is produced, then the
output of task 3 is produced (if task 3 is listed in tasks_section) and so on.
You should keep in mind that tasks are not necessarily listed in order in the tasks_section and they can even be
repeated. For instance, we can have the following TASKS section: TASKS
1 3 4 1 2 3
In this example, some tasks are listed more than once. Later occurrences are ignored. So, the tasks_section above is
equivalent to TASKS 1 2 3 4
In the implementation guide, I explain a simple way to read the list and sort the task numbers using a boolean array.

4 Task 1 – Syntax and Semantic Checking
For task 1, your solution should detect syntax and semantic errors in the input program as specified in this section.
4.1 Syntax Checking
If the input is not correct syntactically, your program should output SYNTAX ERROR
!!!!!&%!!
If there is syntax error, the output of your program should exactly match the output given above. No other output
should be produced in this case, and your program should exit after producing the syntax error message. The provided
parser.* skeleton files already have a function that produces the message above and exits the program.
4.2 Semantic Checking
Semantic checking also checks for invalid input. Unlike syntax checking, semantic checking requires knowledge of the
specific lexemes and does not simply look at the input as a sequence of tokens (token types). I start by explaining the
rules for semantic checking. I also provide some examples to illustrate these rules.
• Polynomial declared more than once – Semantic Error Code 1. If the same polynomial_name is used in two or
more different polynomial_header's, then we have the error polynomial declared more
than once. The output in this case should be of the form
Semantic Error Code 1: <line no 1> <line no 2> ... <line no k>
where <line no 1> through <line no k> are the numbers of each of the lines in which a duplicate
polynomial_name appears in a polynomial header. The numbers should be sorted from smallest to largest. For example, if the input is (recall that line numbers are not part of the input and are just for reference):

```
1: TASKS
2:    1 3 4
3: POLY
4:    F1 = x^2 + 1;
5:
6:    F2 = x^2 + 1;
7:    F1 = x^2 + 1;
8:    F3 = x^2 + 1;
9:    G = x^2 + 1;
10:   F1 = x^2 + 1;
11:   G(X,Y) = X Y^2 + X Y;
12: EXECUTE
13:   INPUT Z;
14:   INPUT W;
15:   X = F1(Z);
16:   Y = G(W);
17:   OUTPUT X;
18:   OUTPUT Y;
19: INPUTS
20: 1 2 3 18 19
```

then the output should be

```
Semantic Error Code 1: 7 10 11
```

because on each of these lines the name of the polynomial in question has a duplicate declaration. Note that
only the line numbers for the duplicates are listed. The line number for the first occurrence of a name is not
listed.
• Invalid monomial name – Semantic Error Code 2. There are two kinds of polynomials headers. In the first kind,
only the polynomial name (ID) is given and no parameter list (id_list in the header) is given. In the second kinds,
the header has the form polynomial_nameLPARENid_listRPAREN. In a polynomial with the first kind of header,
the polynomial should be univariate (one variable) and the variable name should be lower case "x". In a
polynomials with the second kind of header, the id_list is the list variables that can appear in the polynomial
body. An ID that appears in the body of a polynomial (in primary) should be equal to one of the variables of the
polynomial. If that is not the case, we say that we have an invalid monomial name error and the output in this
case should be of the form:
Semantic Error Code 2: <line no 1> <line no 2> ... <line no k> where <line no 1> through <line no k> are the
numbers of lines in which an invalid monomial name appears with one number printed per occurrence of an
invalid monomial name. If there are multiple occurrences of an invalid monomial name on a line, the line
number should be printed multiple times. The line numbers should be sorted from smallest to largest.
• Attempted evaluation of undeclared polynomial – Semantic Error Code 3. If there is no polynomial declaration
with polynomial name which is the same as a polynomial name used in a polynomial evaluation, then we have
attempted evaluation of undeclared polynomial error. In this case, the output should be of the form
Semantic Error Code 3: <line no 1> <line no 2> ... <line no k>
where <line no 1> through <line no k> are the numbers of each of the lines in which a
polynomial_name appears in a polynomial_evaluation but for which there is no polynomial_declaration
with the same name. The line numbers should be listed from the smallest to the largest. For example if
the input is:

```
1: TASKS
2:    1 3 4
3: POLY
4:    F1 = x^2 + 1;
5:    F2 = x^2 + 1;
6:    F3 = x^2 + 1;
7:    F4 = x^2 + 1;
8:    G1 = x^2 + 1;
9:    F5 = x^2 + 1;
10:   G2(X,Y) = X Y^2 + X Y;
11: EXECUTE
12:   INPUT Z;
13:   INPUT W;
14:   X = G(Z);
15:   Y = G2(Z,W);
16:   X = F(Z);
17:   Y = G2(Z,W);
18: INPUTS
19:   1 2 3 18 19
```

then the output should be

```
Semantic Error Code 3: 14 16
```

Because on line 14, there is an evaluation of polynomial G but there is no declaration for polynomial G and on line 16, there is an evaluation of polynomial F but there is no declaration of polynomial F.

Wrong number of arguments – Semantic Error Code 4. If the number of arguments in a polynomial evaluation is different from the number of parameters in the polynomial declaration, then we say that we have wrong

number of arguments error and the output should be of the form: Semantic Error Code 4: <line no 1> <line no 2> ... <line no k> where <line no 1> through <line no k> are the numbers of each of the lines in which polynomial_name appears in a polynomial_evaluation but the number of arguments in the polynomial evaluation is different from the number of parameters in the corresponding polynomial declaration. The line numbers should be listed from the smallest to the largest. For example if the input is:

```
1: TASKS
2:    1 3 4
3: POLY
4:    F1 = x^2 + 1;
5:    F2 = x^2 + 1;
6:    F3 = x^2 + 1;
7:    F4 = x^2 + 1;
8:    G1 = x^2 + 1;
9:    F5 = x^2 + 1;
10:   G2(X,Y) = X Y^2 + X Y;
11: EXECUTE
12:   INPUT Z;
13:   INPUT W;
14:   X = G2(X,Y, Z);
15:   Y = G2(Z,W);
16:   X = F1(Z);
17:   Y = F5(Z,Z);
18:   Y = F5(Z,Z,W);
19: INPUTS
20:   1 2 3 18 19
```

then the output should be

```
Semantic Error Code 4: 14 17 18
```

You can assume that an input program will have only one kind of semantic errors. So, for example, if a test case has Semantic Error Code 2, it will not have any other kind of semantic errors.

5 Task 2 – Program Output

For task 2, your program should output the results of all the polynomial evaluations in the propram. In this section I give a precise definition of the meaning of the input and the output that your compiler should generate. In a separate document that I will upload a little later, I will give an implementation guide that will help you plan your solution. You do not need to wait for the implementation guide to write the parser!

5.1 Variables and Locations

The program uses names to refer to variables in the EXECUTE section. For each variable name, we associate a unique locations that will hold the value of the variable. This association between a variable name and its location is assumed to be implemented with a function location that takes a string as input and returns an integer value. We assume that there is a variable mem which is an array with each entry corresponding to one variable. All variables should be initialized to 0 (zero).

To allocate mem entries to variables, you can have a simple table or map (which I will call the location table) that associates a variable name with a location. As your parser parses the input program, if it encounters a variable name in an input_statement, it needs to determine if this name has been previously encountered or not by looking it up in the location table. If the name is a new variable name, a new location needs to be associated with it, and the mapping from the variable name to the location needs to be added to the location table. To associate a location with a variable, you can simply keep a counter that tells you how many locations have been used (associated with variable names). Initially, the counter is 0. The first variable will have location 0 associated with it (will be stored in mem[0]), and the counter is incremented to become 1. The next variable will have location 1 associated with it (will be stored in mem[1]), and the counter is incremented to become 2 and so on.

For example, if the input program is

```
1: TASKS
2:    1 2
3: POLY
4:    F1 = x^2 + 1;
5:    F2(x,y,z) = x^2 + y + z + 1;
6:    F3(y) = y^2 + 1;
7:    F4(x,y) = x^2 + y^2;
8:    G1 = x^2 + 1;
9:    F5 = x^2 + 1;
10:   G2(X,Y,Z,W) = X Y^2 + X Z + W + 1;
11: EXECUTE
12:   INPUT X;
13:   INPUT Z;
14:   Y = F1(Z);
15:   W = F2(X,Z,Z);
16:   OUTPUT W;
17:   OUTPUT Y;
18:   INPUT X;
19:   INPUT Y;
20:   INPUT Z;
21:   Y = F3(X);
22:   W = F4(X,Y);
23:   OUTPUT W;
24:   OUTPUT Y;
25:   INPUT X;
26:   INPUT Z;
27:   INPUT W;
28:   W = G2(X,Z,W,Z);
29:
30: INPUTS
31:   1 2 3 18 19 22 33 12 11 16

Then the locations of variables will be
```
X 0 Z
1 Y 2
W 3
```

5.2 Statements
We explain the semantics of the four kinds of statements in the program.
5.2.1 Input statements
Input statements get their input from the sequence of inputs. We refer to i'th value that appears in inputs as i'th
input. The i'th input statement in the program of the form INPUT X is equivalent to:
mem[location("X")] = i'th input

5.2.2 Output statements
Output statements have the form OUTPUT ID where the lexeme of the token ID is a variable name. This is the output
variable of the output statement. Output statements print the values of their OUTPUT variables. If the output
statement has the form OUTPUT X; , its effect is equivalent to:
cout << mem[location("X")] << endl;
Note that each output statement produces its output on a separate line.
5.2.3 Assignment statements
Assignment statements have the form:
ID EQUAL poly_evaluation SEMICOLON
If the lexeme of the ID token of an assign_statement is "X", and the poly_evaluation of the assign_statement has value
v (see below for the value of a polynomial evaluation), then the assign statement execution will have an effect
equivalent to mem[location("X")] = v;
5.2.4 Polynomial Evaluation
The polynomial evaluation depends on the evaluation of arguments and the correspondence between the arguments
in polynomial evaluation and the parameters in the polynomial declaration.
Argument Evaluation. The value of a variable X at a given point in the program is equal to the last value assigned
to X before that point (a variable is assigned a value either in an input_statement statement or in an
assign_statement) . If there is no prior assignment to X before that point, then the value of X is
0 (zero). The definition of what an argument evaluates to depends on the definition of what a polynomial evaluates
to because an argument can be a polynomial evaluation. An argument is evaluated as follows:
• If the argument is an ID whose lexeme is "X", then it evaluates to the value of "X" at that point.
• If the argument is a polynomial evaluation, then it evaluates to what the polynomial evaluates to (see below).
Correspondence Between Arguments and Parameters. In a polynomial declaration, the list of parameters is given by
the id_list in the header or if the header has no id_list, then the parameter is the unique variable "x" (lower case). In
a polynomial evaluation, the argument list is given by the argument_list.
We say that the i'th argument in a polynomial evaluation corresponds to the i'th parameter of the polynomial
declaration.
Evaluation of a coefficient. A coefficient whose lexeme is L evaluates to the integer represented by L.
Evaluation of an exponent. An exponent whose lexeme is L evaluates to the integer represented by L.
Evaluation of a monomial. There are a number of cases to consider
• A monomial of the form ID whose lexeme is "X" evaluates to the argument corresponding to "X"
• A monomial of the form IDexponent where the lexeme of the ID is "X" evaluates to ve where e is the value that
the exponent evaluate to and v is the value that the argument corresponding to "X" evaluates to.

Evaluation of a monomial_list . A monomial_list of the form monomial evaluates to the value that monomial
evaluates to. A monomial_list of the form monomialmonomial_list' evaluates to the product of v (the value that
monomial evaluates to) and v0 (the value that monomial_list' evaluates to).
Evaluation of a term. A term of the form coefficientmonomial_list, where coefficient evaluates to c and
monomial_list evaluates to v, evaluates to c × v (the product of c and v).
Evaluation of a term_list. A term_list of the form term evaluate to the value that term evaluates to. A term_list of the
form termadd_operatorterm_list' , where term evaluates to v and term_list' evaluates to v', evaluates to v + v0 if the
add_operator is PLUS and to v − v0 if the add_operator is MINUS.
Evaluation of a polynomial_body. evaluates to.
A polynomial_body of the form term_list evaluates to the value that the term_list
Evaluation of a polynomial. A polynomial evaluates to the value that its polynomial_body evaluates to.
5.3 Assumptions
You can assume that the following semantic errors are not going to be tested
1. You can assume that if there is a polynomial declaration with a given polynomial name, then there is no variable
with the same name in the program.
2. If you want to use an array for the mem variable, you can use an array of size 1000 which should be enough for
all test cases, but make sure that your code handles overflow (more than 1000 variables in the program)
because that is good programming practice.

6 Task 3 – Variable used before being initialized
Uninitialized argument. Warning Code 1. If an argument in an argument_list in a polynomial_evaluation does not
appear in an input_statement nor on the lefthand side of an assignment statement before the polynomial evaluation,
then we say that we have an uninitialized argument
warning and the output should be of the form:
Warning Code 1: <line no 1> <line no 2> ... <line no k> where <line no 1> through <line no k> are the numbers of
each of the lines in which an argument appears in a polynomial_evaluation but for which: (1) which there is no
previous assign_statement in which the argument appears on the lefthand side of the EQUAL sign and (2) there is no
previous input_statement in which the argument appears. The line numbers should be listed from the smallest to the
largest. For example,

if the input is:

```
1: TASKS
2:    1 2 3
3: POLY
4:    F1 = x^2 + 1;
5:    F2(x,y,z) = x^2 + y + z + 1;
6:    F3(y) = y^2 + 1;
7:    F4(x,y) = x^2 + y^2;
8:    G1 = x^2 + 1;
9:    F5 = x^2 + 1;
10:   G2(X,Y,Z,W) = X Y^2 + X Z + W + 1;
11: EXECUTE
12:   INPUT X;
13:   INPUT Z;
14:   Y = F1(Z);
15:   W = F2(W,Z, W);
16:   INPUT Y;
17:   INPUT Z;
18:   Y = F4(X,Q);
19:   W = F4(X,Y);
20:   W = F2(W, P, P);
21:   OUTPUT Y;
22:   INPUT X;
23:   W = G2(X,Z,W,Z);
24:
25: INPUTS
26:   5 2 3 18 19 22 33 12 11 16
```

then the output will be

```
25
Warning Code 1: 15 15 18 20 20
```

We first note that there is a 25 printed before the warning message. This is the output of the program (produced in line 21). Notice how the calculation is performed using value 5 for X (which is read from INPUTS on line 12) and value 0 for Q ( initial value 0 for Q because Q is not initialized). The output of the program should be produced in this case because there are no syntax and no semantic errors, and 2 appears in the TASKS section, so Task 2 should be executed.

Now, we explain the warning message. Notice that line 15 is repeated in the output because both W's on line 15 are not initialized before they appear in the polynomial evaluation F2(W,Z,W). Line 18 is also listed in the warning message because in line 18, Q is used in the evaluation of F4(X,Q) without previously being initialized. Line 20 is repeated in the output because P which is used twice in the calculation of F2(W,P,P) without being previously initialized. Finally, note that there is no warning for lines 23 and 24 because all arguments are previously initialized: X on line 22, Z on line 17 and W on line 20.


--------------------


7 Task 4 – Useless assignments
A useless assignment occurs if a variable is assigned a value in an assignment statement, but the variable is not used later on the righthand side of an assignment in an assign_statement or in an output_statement.

The definition requires some explanation. Consider the following program:

```
1: TASKS
2:    1 4
3: POLY
4:    F = 2x + 1;
5: EXECUTE
6:    X = F(5);
7:    X = F(6);
8:    OUTPUT X;
9:    X = F(7);
10:   X = F(X);
11:   INPUT X;
12:   OUTPUT X;
13: INPUTS
14:   2 4 6
```

We examine each assignment statement in the program.

Line 6: X = F(5); The value calculated in this statement is not used later in an output_statement or in an assign_statement because the next statement on line 7 overwrites the calculated value. Line 7: X = F(6); The value calculated in this statement is used by the output statement that appears on line 8. Line 9: X = F(7); The value calculated in this statement appears on the right-hand side of the assign_statement on line 10, so the assignment is not useless. Line 10: X = F(X); The value calculated in this statement is not used later in an output_statement or in an assign_statement because it is immediately followed by INPUT X; which overwrites the value of X.

In general, we can define useless assignments as follows. We say that a statement defines a variable x if it is of the form INPUT x; or if it is of the form x = poly_evaluations ; We say that a statement uses a variable x if the statement is
of the form OUTPUT x; or if the statement is of the form ID = poly_evaluation ; and x appears in poly_evaluation as
an argument.
Given the statement list stmt1 stmt2 ... stmtk, if stmti has the form x = poly_evaluations ; , we say that stmti is a useless
assignment statement if stmti is the last statement in the list, or stmti+1 defines x, or stmti+1 does not use x and stmti is
useless in the sequence stmt1 stmt2 ... stmti stmti+2 ... stmtk

If the program has useless assignments, then its output should be of the form: Warning Code

2: <line no 1> <line no 2> ... <line no k> where <line no 1> through <line no k> are the numbers of each of the lines that contains a useless assignment statement as defined above. The line numbers should be listed from the smallest to the largest, and you can assume that useless assignment statements are not split across multiple lines.

8 Task 5 – Polynomial degree

This tasks requires that the degree of polynomials in the polynomial sections be calculated and outputted. The degree of a polynomial is defined as follows:

1. 2. 3. 4. 5. 6. 7. 8. 9.

The degree of a primary of the form ID is 1.
The degree of a primary of the form LPARENterm_listRPAREN is equal to the degree of term_list.
The degree of a monomial of the form primary is equal to the degree of primary.
The degree of a monomial of the form primaryexponent is equal to the degree of primary times the value of the exponent.
The value of an exponent of the form POWERNUM is equal to the value of NUM, which is the integer that corresponds to the lexeme of NUM.
The degree of a monomial list of the form monomial is equal to the degree of the monomial.
The degree of a monomial list of the form monomialmonomial_list is equal to the sum of the degree of the monomial and the degree of monomial_list.
The degree of a term of the form coefficient is equal to 0 (zero).
The degree of a term which is of the form coefficientmonomial_list is equal to the degree of the monomial_list.
10. The degree of a term which is of the form monomial_list is equal to the degree of the monomial_list.
14
11. The degree of a term list of the form term is equal to the degree of term.
12. The degree of a term list of the form termadd_operatorterm_list is equal to the maximum of the degree of
term and the degree of term_list.
If there are no syntax and no semantic errors, the output for this task should have the form
<poly 1>: <degree 1> <poly 2>:
<degree 2>
<poly 3>: <degree 3> ...
<poly k>: <degree k> where <poly i> is the name of the i'th polynomial and <degree i> is the degree of the
i'th polynomial.

For example, if the input is:

```
1: TASKS
2:    1 2 5
3: POLY
4:    F1 = x^2x + x^3x^1;
5:    F2(x,y,z) = x^2 + y + z + 1;
6:    F3(y) = y^2 + 1;
7:    F4(x,y) = x^2 + y^2;
8:    G1(x,y,z,w) = x^2 y z w + x^2 y^2 z w + 1;
9:    F5 = x^2 + 1;
10:   G2(X,Y,Z,W) = X Y^2 + X Z + W + 1;
11: EXECUTE
12:   INPUT X;
13:   INPUT Z;
14:   Y = F1(Z);
15:   W = F2(W,Z, W);
16:   INPUT Y;
17:   INPUT Z;
18:   Y = F4(X,Q);
19:   W = F4(X,Y);
20:   W = F2(W, P, P);
21:   OUTPUT Y;
22:   INPUT X;
23:   W = G2(X,Z,W,Z);
25: INPUTS
26:   5 2 3 18 19 22 33 12 11 16
```

The output will be:
```
25
F1: 4 F2: 2
F3: 2 F4: 2
G1: 6 F5:
2 G2: 3
```

By now, you should know why the output has 25 on the first line.

9 Requirements
You should write a program to generate the correct output for a given input as described above. You should start by
writing the parser and make sure that it correctly parses the input before attempting to implement the rest of the
project.

You will be provided with a number of example test cases. These test cases are not meant to be complete or even close to complete. They are only provided as examples to complement the project description. It is still your responsibility to make sure that your implementation satisfies the requirements given in this document, and you should develop your own test cases to do so.

10 Instructions

Follow these steps:

• Read this document carefully.

• When the implementation guide is posted, read it carefully. It has detailed explanations on how to approach the implementation.

• Download the lexer.cc, lexer.h, inputbuf.cc and inputbuf.h files accompanying this project description and familiarize yourself with the provided functions.

• Design a solution before you start coding. It is really very important to have a clear overall picture of what the project entails before you start coding. Deciding on data structures and how you will use them is crucial. One possible exception is the parser, which you can and should write first before the rest of the solution.

• Write your code and make sure to compile your code using GCC (4:11.2.0) on Ubuntu 22.04 (Ubuntu).

If you want to test your code on your personal machine, you should install a virtual machine with Ubuntu 22.04 and the correct version of GCC on it. You will need to use the g++ command to compile your code in a terminal window. See section 12 for more details on how to compile using GCC. You are required to compile and test your code on Ubuntu using the GCC compiler, but you are free to use any IDE or text editor on any platform while developing your code as long as you compile it and test it on Ubuntu/GCC before submitting it.

• Test your code to see if it passes the provided test cases. You will need to extract the test cases from the zip file and run the provided test script test1.sh. See section 12 for more details.

• Develop your own test cases and test your program on them.

• Submit your code through gradescope.

• You are allowed an unlimited number of submissions, but it is your responsibility to activate the submission that should count for your grade. If have a submission with a grade of 90 and another submission with a grade of 80. You should make sure to activate the older submission if you want it to count. Activating older submissions is also allowed during the late submission period. You can continue on working to improve your grade, but you can revert to an earlier grade if you want by activating an earlier submission.

Keep in mind that
• You should use C++11, no other programming languages or versions of C++ are allowed.
• All programming assignments in this course are individual assignments. Students must complete the assignments on their own.
• You should submit your code through gradescope; no other mode of submission will be accepted.
• You should familiarize yourself with the Ubuntu environment and the GCC compiler. Programming assignments in this course are different from assignments in other classes.

11 Evaluation

Submissions are evaluated using automated test cases on gradescope. To receive credit for parsing, your submission should pass ALL the parsing test cases. For other categories, your grade will be proportional to the number of test cases that your submission passes.If your code does not compile on gradescope, you will not receive any points even if it runs correctly in a different environment (Windows/Visual Studio for example). Here is the grade breakdown for the various categories:

1. Task 1 – Parsing: 30%. There is no partial credit for parsing. Your program should pass ALL the parsing test cases to get credit for parsing, otherwise no credit will be given for parsing.

2. Task 1 – Semantic Error Code 1: 7.5%. The grade for this category will be proportional to the number of test cases that are correctly handled by your program.

3. Task 1 – Semantic Error Code 2: 7.5%. The grade for this category will be proportional to the number of test cases that are correctly handled by your program.

4. Task 1 – Semantic Error Code 3: 7.5%. The grade for this category will be proportional to the number of test cases that are correctly handled by your program.

5. Task 1 – Semantic Error Code 4: 7.5%. The grade for this category will be proportional to the number of test cases that are correctly handled by your program.

6. Task 2 – Program Output: 20%. The grade for this category will be proportional to the number of test cases that are correctly handled by your program.

7. Task 3 – Variable used before being initialized: 10%. The grade for this category will be proportional to the number of test cases that are correctly handled by your program.

8. Task 4 – Useless assignments: 10%. The grade for this category will be proportional to the number of test cases that are correctly handled by your program.

9. Task 5 – Polynomial degree: 10%. The grade for this category will be proportional to the number of test cases that are correctly handled by your program.

12 General instructions for all programming assignments

NOTE: This section applies to all programming assignments.

You should use the instructions in the following sections to compile and test your programs for all programming assignments in this course.

12.1 Compiling your code with GCC

You should compile your programs with the GCC compilers. GCC is a collection of compilers for many programming languages. There are separate commands for compiling C and C++ programs. Use the g++ command to compile C++ programs

Here is an example of how to compile a simple C++ program:

$ g++ test_program.cpp

If the compilation is successful, it will generate an executable file named a.out in the same directory (folder) as the program. You can change the executable file name by using the -o option. For example, if you want the executable name to be hello.out, you can execute

$ g++ test_program.cpp -o hello.out

To enable C++11, with g++, which you should do for projects in this class, use the -std=c++11 option:

$ g++ -std=c++11 test_program.cpp -o hello.out

The following table summarizes some useful compiler options for g++:

Option Description
-o path Change the filename of the generated artifact
-g Generate debugging information
-ggdb Generate debugging information for use by GDB
-Wall Enable most warning messages
-std=c++11 Compile C++ code using 2011 C++ standard

Compiling projects with multiple files

If your program is written in multiple source files that should be linked together, you can compile and link all files together with one command:

$ g++ file1.cpp file2.cpp file3.cpp

Or you can compile them separately and then link:

$ g++ -c file1.cpp $ g++ -c file2.cpp $ g++ -c file3.cpp
$ g++ file1.o file2.o file3.o

The files with the .o extension are object files but are not executable. They are linked together with the last statement

(g++ file1.o file2.o file3.o) and the final executable will be a.out.

12.2 Testing your code on Ubuntu

Your programs should not explicitly open any file. You can only use the standard input and standard output in C++.

The provided lexical analyzer already reads the input from standard input and you should not modify it. In C++, standard input is std::cin and standard output is std::cout. In C++, any output that your program produces should be done with cout. To read input from a file or produce output to a file, we use IO redirection outside the program. The following illustrates the concept.

Suppose we have an executable program a.out, we can run it by issuing the following command in a terminal (the dollar sign is not part of the command):

$ ./a.out

If the program expects any input, it waits for it to be typed on the keyboard and any output generated by the program will be displayed on the terminal screen.

To get the input to the program from a file, we can redirect the standard input to a file:

$ ./a.out < input_data.txt

Now, the program will not wait for keyboard input, but rather read its input from the specified file as if the file input_data.txt is standard input. We can redirect the output of the program as well:

$ ./a.out > output_file.txt

In this way, no output will be shown in the terminal window, but rather it will be saved to the specified file1.

Finally, it's possible to do redirection for standard input and standard output simultaneously. For example,

$ ./a.out < input_data.txt > output_file.txt will read standard input from input_data.txt and produces standard output to output_file.txt.

Now that we know how to use standard IO redirection, we are ready to test the program with test cases.

1 Programs have access to another standard stream which is called standard error e.g. std::cerr in C++. Any such output is still displayed on the terminal screen. It is possible to redirect standard error to a file as well, but we will not discuss that here

Test Cases

For a given input to your program, there is an expected output which is the correct output that should be produced for the given input. So, a test case is represented by two files:

- test_name.txt
- test_name.txt.expected

The input is given in test_name.txt and the expected output is given in test_name.txt.expected.

To test a program against a single test case, first we execute the program with the test input data:

$ ./a.out < test_name.txt > program_output.txt

With this command, the output generated by the program will be stored in program_output.txt. To see if the program generated the correct expected output, we need to compare program_output.txt and test_name.txt.expected. We do that using the diff command which is a command to determine differences between two files:

$ diff -Bw program_output.txt test_name.txt.expected

If the two files are the same, there should be no difference between them. The options -Bw tell diff to ignore whitespace differences between the two files. If the files are the same (ignoring the whitespace differences), we should see no output from diff, otherwise, diff will produce a report showing the differences between the two files. We consider that the test passed if diff could not find any differences, otherwise we consider that the test failed.

Our grading system uses this method to test your submissions against multiple test cases. In order to avoid having to type the commands shown above for running and comparing outputs for each test case manually, we provide you with a script that automates this process. The script name is test1.sh. test1.sh will make your life easier by allowing you to test your code against multiple test cases with one command.

Here is how to use test1.sh to test your program:
• Store the provided test cases zip file in the same directory as your project source files
• Open a terminal window and navigate to your project directory
• Unzip the test archive using the unzip command: bash $ unzip tests.zip
This will create a directory called tests
• Store the test1.sh script in your project directory as well
• Make the script executable: bash $ chmod +x test1.sh
• Compile your program. The test script assumes your executable is called a.out
• Run the script to test your code: bash $ ./test1.sh

The output of the script should be self explanatory. To test your code after you make changes, you will just perform the last two steps (compile and run test1.sh).


----------------


# guide

PROJECT 1

IMPLEMENTATION GUIDE
CSE 340
Rida Bazzi
This document is provided to help you with the implementation
The first step is to write your parser. You should finish writing the parser
completely and you should test it thoroughly before attempting to implement
anything else. I cannot emphasize this strongly enough. I have seen many
projects in the past that suffered because the parser was not written first.
The second step is to write the semantic error checking functionality: do not work
on the “semantic warning” functionality and the “execution” functionality before
you are done with semantic error checking!
The third step is to write the semantic warning functionality: do not work on
the “execution” functionality before you are done with semantic warnings
functionality!
The last step is to work on the execution functionality
That said, you should have a clear idea about what the various parts entail
before you start coding.
You should read this document at least a couple of times to get a good
understanding.
You should treat all code fragments in this document as pseudocode and not as
code. You should not cut and paste code fragments from this document into your
program. I did not compile the code fragments!
FIRST STEP
In case you missed it on the previous page, The first step is to write your
parser. You should finish writing the parser completely and systematically and
test thoroughly before attempting to write anything else. I cannot emphasize this
strongly enough.
The first step is to write your parser
COMPLETELY and TEST IT thoroughly.
I have seen many projects in the past that suffered because the parser was not
properly written.
NOTE: You should read this document at least a couple of times to get a good
understanding.
NOTE: You should treat all code fragments in this document as pseudocode. You
should not cut and paste code fragments from this document into your program. I
did not compile the code fragments!
Content
• Major components 1 page
• Parsing 2 pages
• Semantic Checking 3 pages
• Program execution 1 page
• Memory allocation for variables 1 page
• Storing inputs 1 page
• Representing polynomial declarations 4 pages
• Representing the program as a linked list 1 page
• Representing polynomials for evaluation 2 pages
• Generating a statement list 1 page
• Executing the program 1 page
• Evaluating polynomials 1 page
Major Components
2. 2. The major components of the project are the following (note that the total is 110%, so there are 10%
bonus points)
1. Error Checking
1. Syntax checking (30%): You should finish writing the parser before attempting to do anything
else. I cannot emphasize this strongly enough. I have seen many projects in the past that
suffered because the parser was not written carefully as a first step.
Semantic error checking (30%): This part has quite a few grade points allocated to it, but it
is not the most involved part. If you understand what needs to be done and you read the
suggestions on how to do it, you should be able to finish it in a relatively short time. That
said, start early!
Program Execution (20%): To implement the program execution functionality, you need some
supporting functionality:
2.1 Memory Allocation. After writing the parser, you should implement a function to allocate
memory to variables. This will be useful for other functionality as described later.
2.2 Storing Inputs. You should implement functionality to store the inputs internally in a data
structure that can be accessed later when the program is executed.
2.3 Polynomial Declaration. Implement code that parses a polynomial declaration and stores a
representation of the polynomial in a data structure that can be executed later.
2.4 INPUT Statements. Implement the code that will transform an INPUT statement into an
intermediate representation that can be executed later.
2.5 OUTPUT Statements. implement the code that will transform an OUTPUT statement into an
intermediate representation that can be executed later
2.6 Assignment Statements. implement the code that will transform an assignment statement
into an intermediate representation that can be executed later.
2.7 Statement List. Implement the code that will transform a list of statements into a linked
list that can be executed later.
2.8 Execution. implement an execute_program() function that takes the data structure
generated in 2.7 as an argument to simulate the execution of the program and generate the
output of the program
3. Semantic warnings (30%): There are three semantic warning tasks. Two of them are relatively
straightforward, like semantic error checking. One of them is a bit more involved and I do not
describe how to do it (those are the 10% bonus points).
NOTE You should read this document at least a couple of times to get a good understanding.
NOTE You should consider all code fragments in this document as pseudocode and not as code that you
can cut and paste into your program. I did not compile the code fragments!
Parsing (1/2)
The easiest way to write the parser is to write one parsing function for each non-terminal and
make sure that when that function is called, it consumes part of the input that corresponds to the
non-terminal. This was covered in the lectures. I repeat it here by showing how to write the parse
functions for some of the non-terminals of the project’s grammar.
Remember that code in this document has not been compiled. You should not treat it as code that
you can cut and paste into your program.
void parse_input()
{ // the input consists of a program followed by nothing
parse_program();
expect(END_OF_FILE);
}
void parse_program()
{ // program -> tasks_section poly_section execute_section inputs_section
parse_tasks_section();
parse_poly_section()
parse_execute_section();
parse_inputs_section();
}
program has only one rule. To parse the program, we need to consume a sequence of tokens that
corresponds to program. Remember that we are writing our parsing functions so that when
parse_X() is called, it consumes part of the input that corresponds to X. The rule for program is of
the form
A –> B C D E
For such a rule, the sequence of tokens corresponding to A would consist of a section corresponding
to B followed by a sequence corresponding to C followed by a sequence corresponding to D followed
by a sequence corresponding to E. So, to parse A, we need to consume a sequence corresponding to
B, then consume a sequence corresponding to C, then consume a sequence corresponding to D and
finally consume a sequence corresponding to E. This is achieved by calling parse_B(); parse_C();
parse_D(); parse_E(); This is the same situation we have for the non-terminal program
Let us look at a couple more example on the next page.
Parsing (2/2)
Next, we will consider the rules for the non-terminal poly_decl
void parse_poly_decl()
{ // poly_decl -> poly_header parse_poly_header();
expect(EQUAL);
parse_poly_body();
expect(SEMICOLON);
EQUAL poly_body SEMICOLON
}
This is similar to the case of program. We have only one rule for poly_decl, so to parse
poly_decl, we need to consume a sequence of tokens corresponding to the right-hand side of the
single rule for poly_decl. Unlike the example of program, here the right-hand side has terminals
(tokens). We have already seen that to consume part of the input corresponding to a non-terminal X,
we call parse_X(). To consume a particular terminal (token), we use the expect()function
(provided). The code for parse_poly_decl() is color coded and shows how each call
corresponds to the right-hand side of the rule for poly_decl.
Note: you must have noticed that I include the rules for the non-terminals as a comment in the code for
the non-terminal. This is not done only to explain how parsing is done. This is done to avoid mistakes and is
good practice in general. I recommend that you start by writing one parse function for each non-terminal
and include in it the rules for that non-terminal. Then you start writing the bodies of these functions one
by one by following the rules.
I am going to look at one more example before finishing the discussion of parsing. We will consider the
non-terminal primary. This non-terminals has two rules. To decide which rule to follow to do the parsing,
we call peek(1) which returns the upcoming token but does not consume it. In general, peek(k) will
return the k’th token ahead.
void parse_primary()
{ // primary -> ID
// primary -> LPAREN term_list RPAREN
Token t = lexer.peek(1);
if (t.token_type == ID)
expect(ID);
else if (t.token_type == LPAREN) {
expect(LPAREN);
parse_term_list();
expect(RPAREN);
} else
syntax_error();
}
Notice how this code proceeds. First, it peeks at the next token. If it is ID, then it determines that the
rule for primary must be primary -> ID and it parses accordingly by consuming ID using the expect()
function. If peek(1) returns LPAREN, then it determines that we must parse primary according to the
rule primary -> LPAREN term_list RPAREN. If peek(1) returns neither ID nor LPAREN, then
syntax_error() is called.
Note. In general, we might need to peek for two tokens ahead to determine which right-hand side to
parse. For this project’s grammar, this is the case for some non-terminals.
Semantic Error Checking (1/3)
The semantic error checking functionality asks you to determine if
1. 2. 3. 4. A polynomial is declared more than once
A name (ID) appears in the body of a polynomial declaration but not in the list of parameters to the
polynomial.
A polynomial evaluation uses a name of a polynomial that was not declared
A polynomial evaluation does not have the correct number of arguments.
For the first case, we have conflicting declarations. For cases 2,3 and 4, we have uses of names that do not
match declarations of names. To support these functionalities, we need to store information about the
declarations, as they are parsed and, when a particular name is parsed, we check it against the stored
information. I will explain in detail how to check for polynomial declared more than once and give a high-level
explanation on how to check for the other semantic errors.
Let us consider the rules for the polynomial declarations section (decl in the names stands for declaration)
poly_section -> POLY poly_decl_list
poly_dec_list -> poly_decl
poly_dec_list -> poly_decl poly_dec_list
poly_decl -> poly_header EQUAL poly_body SEMICOL
poly_header -> poly_name
poly_header -> poly_name LPAREN id_list RPAREN
id_list -> ID
id_list -> Id COMMA id_list
The code for parse_poly_decl_list() looks as follows
void parse_poly_decl_list()
{ // poly_dec_list -> poly_decl
// poly_dec_list -> poly_decl poly_dec_list
parse_poly_decl(); // parses one declaration
Token t = lexer.peek(1);
if (t.token_type == ID) // ID is the start of a new poly_decl_list
{
parse_poly_decl_list();
} else if (t.token_type == EXECUTE) // polynomial declaration list
return; // is followed by EXECUTE
else
syntax_error();
}
Note that the part highlighted in yellow can be omitted for this function because after the call to
parse_poly_section() returns, parse_execute_section() will be called by parse_program() and
the first call that parse_execute() makes is expect(EXECUTE).
You should be able to see that the polynomials are listed one after another in the poly_section. When a
particular call to parse_poly_decl() is active, it must be handling the next polynomial in the list.
I continue the explanation on the next page.
Semantic Error Checking (2/3)
Now, let us consider the rules for poly_decl and poly_header
poly_decl -> poly_header EQUAL poly_body SEMICOLON
poly_header -> poly_name
poly_header -> poly_name LPAREN id_list RPAREN
The code for parse_poly_decl looks as follows
void parse_poly_decl ()
{ //poly_decl -> poly_header EQUAL poly_body SEMICOLON
parse_poly_header();
expect(EQUAL);
parse_poly_body();
expect(SEMICOLON);
}
This does not look different from what we have seen so far, and it is not enough. We need to store the polynomial header information for the list of polynomials. So, we need parse_poly_header() to return a
data structure that represents the header. The following data structure could be used
struct poly_header_t {
Token name;
vector<Token> id_list;
}
This representation is somewhat redundant, but it will do (we don’t really need the token_type field in the
Token struct or even the line_no field). parse_poly_header() will look like this
struct poly_header_t parse_poly_header()
{
struct poly_header_t header;
header.name = parse_poly_name();
Token t = lexer.peek(1);
if (t.token_type == LPAREN)
{ expect(LPAREN);
header.id_list = parse_id_list();
expect(RPAREN);
} else
header.id_list = make_list(“x”);
return header;
}
Now, we can write
struct poly_decl_t parse_poly_decl ()
{ //poly_decl -> poly_header EQUAL poly_body SEMICOLON
struct poly_decl_t decl;
decl.header = parse_poly_header();
expect(EQUAL);
decl.body = parse_poly_body();
expect(SEMICOLON);
return decl;
}
Semantic Error Checking (3/3)
The code on the previous page used poly_decl_t struct, but did not declare it. The struct will contain a
header which is of type poly_header_t and a poly_body which we will discuss later.
Going back to parse_poly_decl_list(), we have
void parse_poly_decl_list()
{ // poly_dec_list -> poly_decl
// poly_dec_list -> poly_decl poly_dec_list
struct poly_dec = parse_poly_decl(); // parses one declaration
. . . ? . . .
Token t = lexer.peek(1);
if (t.token_type == ID) // ID is the start of a new poly_decl_list
{
parse_poly_decl_list();
. . . ? . . .
}
}
We need to store these declarations in one list or vector of polynomial declarations. In general, when we are
constructing a data structure recursively with a recursive descent parser, we need to create the data structure dynamically. That will be needed for creating the representation of the polynomial body for
execution, but it is not really needed for the sequence of polynomial declarations. Since the declarations are
listed one after the other and there are no nested declarations, we can have a global vector to store all the
declarations. The code can be:
vector<poly_decl_t> polynomials; // global data structure
void parse_poly_decl_list()
{ // poly_dec_list -> poly_decl
// poly_dec_list -> poly_decl poly_dec_list
struct poly_decl = parse_poly_decl(); // parses one declaration
polynomials.push_back(poly_decl);
Token t = lexer.peek(1);
if (t.token_type == ID) // ID is the start of a new poly_decl_list
{
parse_poly_decl_list();
}
}
The highlighted code pushes the declaration into the global polynomials vector. Actually, this single line of
highlighted code will have the effect of pushing ALL the declarations onto the polynomials vector! You
should try to trace the code by hand to get a better understanding of what is happening. If you don’t have a
good understanding, you will not be able to do the other parts of the project.
Pushing the the polynomial declaration to the vector easily allows us to check for duplicate polynomial names.
Before adding poly_decl to the vector, we can check if the polynomial name in the poly_header of the
poly_decl struct is already in the vector polynomials. You will need to write the code to check for
duplicates.
The code as written would not be adequate to support “undeclared monomial” error because when
parse_poly_body() is called from parse_poly_decl() (see previous page), the poly_decl struct is
not available in the global vector. One way to get around this is to pass the header (see previous page) to
parse_poly_body() or to push the header info to the global vector directly in parse_poly_decl()
(pushing a poly_decl with only the header part filled and with the poly_body field empty).
The data structures discussed here should also be enough to check for undeclared polynomial name and for
wrong number of arguments (the number of arguments would need to be calculated when parse_id_list()
is called, and the number stored in the poly_decl struct.
Program Execution
Supporting the program execution functionality (Task 2) is more involved than semantic error
checking and semantic warnings. You will need to:
1. 2. 3. 4. Represent the polynomial body in a data structure that can be used later for execution
Allocate locations to variables (in the EXECUTE section) so that each variable has a fixed
locations to store its value (for INPUT or assignment statements) and to read its value (for
OUTPUT statements).
Represent the EXECUTE section in a data structure (linked list of statements)
Call an execute_program() function that will execute the data structure from item 3
above
In what follows, I explain how to support various aspects of the execution functionality.
Allocating memory for variables
All variables in the EXECUTE section need memory to be allocated to them. In this section I explain how
memory allocation should be done.
Let us consider the parse function for INPUT statements. The function will return a data structure which
is a representation of the statement that can be “executed” later.
struct stmt_t * parse_input_statement()
{ Token t;
t = lexer.getToken();
struct stmt_t * st = new stmt_t;
expect(INPUT);
t = expect(ID);
expect(SEMICOLON);
// at this point t.lexeme contains the name of a variable.
//
// There are two possibilities
//
// // 1. the name has previously appeared during parsing in which case memory has been
allocated to the variable, or
// 2. this is the first time we see the name in which case we need to allocate memory for it.
//
// so, we need to lookup the name of the variable. If we find it, nothing needs to be done
// as far as memory allocation is concerned. If we don’t find the name, memory needs to
// be reserved for the variable. Remember that the actual name of the variable is in
// t.lexeme, so, your program will lookup if t.lexeme is already in the symbol table
// (see next page for details on the symbol table) if loc is the location associated with
// t.lexeme, we will have:
//
// st->stmt_type = INPUT; and
// st->op1 = loc;
//
// st is returned by the function:
return st;
Memory allocation is not only done in INPUT statements. memory allocation is done any time a variable
name appears for the first time in the EXECUTE section. This can be on the left-hand side of an
assignment, as an argument to a polynomial evaluation or even in an OUTPUT statement.
So, how do we allocate memory to variables? See next page!
Allocating memory for variables
Symbol Table memory
“X” , 0
0
TASKS 1 2
POLY F(x,y) = x+y;
EXECUTE
INPUT X;
INPUT Y;
X = F(X,Y);
INPUT Z;
INPUT Y;
Y = F(X,W);
INPUT X;
INPUT W;
W = F(X,W);
OUTPUT W;
INPUTS 1 4 17 18 19 13 14
”Y” , 1
0
0
1
“Z” , 2
0
2
“W” , 3
0
0
3
4
5
3
5
6
0
7
The sample program above has 4 variables. The illustration on the right shows variables x, y,
z, and w with locations 0, 1, 2, and 3 allocated to them. The allocation is noted in the symbol
table (can be implemented as a map in C++).
Notice that the entries are initialized to zero.
Now, that we know what should go in the symbol table, the question is: where is that
functionality implemented in the parser? We have already seen where that functionality
should go for an input_statement (see previous page). You should also have that
functionality in assign_statement and in input_statement.
How to do allocation? The allocation itself is straightforward. You keep a global counter
variable in your parser called next_available. When you need to allocate space you insert
(t.lexeme , next_available) in the symbol table and you increment next_available so that the
next allocation will be to the next location in memory.
Note. There is a difference between program variables that have memory allocated to them
and between polynomial parameters that do not have memory allocated to them.
Storing inputs
Symbol Table memory
TASKS 1 2
POLY F(x,y) = x+y;
EXECUTE
INPUT X;
INPUT Y;
X = F(X,Y);
INPUT Z;
INPUT Y;
Y = F(X,W);
INPUT X;
INPUT W;
W = F(X,W);
OUTPUT W;
INPUTS 1 4 17 18 19 13 14
“X” , 0
0
”Y” , 1
0
“Z” , 2
0
“W” , 3
0
0
5
3
0
On the previous page, we have shown how to allocate memory to variables.
We also need to store the sequence of input values
in an array or vector so that they can be accessed later
when the program is “executed”
This can be achieved by storing the input values successively
in a vector as shown to the right
The C++ atoi() function (lookup its documentation ) is used to obtain
the integer values that will be stored in the vector
inputs
1
4
17
18
19
13
14
0
1
2
3
4
5
6
7
Representing Polynomial Declarations (1/3)
As your parser parses a polynomial declaration, it needs to represent it internally as a data structure that can be used later to help in evaluating the polynomial. In what follows, I explain how this data structure can be build piece by piece. The approach will be as follows:
1. When a polynomial header is parsed, the polynomial name and a list containing the names of the parameters is
returned
2. 3. 4. When a coefficient is parsed, the integer value for the coefficient is returned
When an exponent is parsed, an integer representing the exponent is returned
When a primary which is an ID is parsed, a struct consisting of two fields is returned: the first field indicates
that the primary is an ID, and the second field is the order of the ID in the list of variables for the polynomial.
5. When a primary which is a (term_list) is parsed, a struct consisting of two fields is returned: the first field
indicates that the primary is a TERM_LIST, and the second field is a pointer to the representation of the
term_list.
Note: to support 4 and 5, when we parse a primary, we return a data structure with 3 fields, the first field indicates the type of the primary (VAR or TERM_LIST), the second field is an integer that is equal to the
index of the primary when the primary is a VAR and the third field is a pointer to a term_list_t data
structure when the primary is a term_list
6. When a monomial is parsed, a data structure consisting of two fields is returned. The first field is a pointer to a data structure that represent the primary and the second field is the power to which it is raised
7. 8. When a monomial list is parsed, a list containing the information of each monomial in the list is returned
When a term is parsed, the coefficient and the monomial list (if any) is returned in a struct
9. When a polynomial body is parsed, a data structure containing information about each term and the operator of the polynomial (PLUS/MINUS) is returned
int parse_coefficient()
{ Token t;
t = expect(NUM);
return atoi(t.lexeme); // you need to lookup information
// about atoi() and how to use it
}
{
struct primary_t * parse_primary()
// A primary is either a variable name or a term_list
// struct primary_t {
// int kind; // VAR or TERM_LIST
// int var; // if the primary is a VAR, this is
// // index of the variable in the list of
// // parameters of the polynomial
// struct term_t * t_list; // if the primary is a TERM_LIST
// }
}
Representing Polynomial Declarations (2/3)
struct monomial * parse_monomial()
{
// A monomial consists of a primary and an optional exponent
//
// struct monomial_t {
// struct primary_t * primary;
// int exponent; // // exponent
// }
// equal to 1 if there is no specified
}
struct monomial_list * parse_monomial_list()
// parse the monomial list and return pointer to the list
{
}
{
struct term * parse_term()
// parse the term and return a structure containing the coefficient and
// the monomial list
}
{
struct polynomial_body * parse_polynomial_body()
// parse the polynomial body and return a list representing the polynomial and
// its operators.
}
Representing Polynomial Declarations (3/3)
At the top level, we need to maintain a list of polynomial. The list can be kept in a table (vector) in which
each entry will have the polynomial name and the list of parameters (the polynomial header) and the
representation of the polynomial body
Initially, the first thing that is parsed is the polynomial header: polynomial name and parameter list. When the header is parsed, information about it is added to a polynomial declaration table. The table is a simple
vector to which you should keep track of the last added entry. This information can be used by the
parse_monomial() function to determine the order in which a variable appears in the list of parameters
of the polynomial.
These ideas are best illustrated by an example. Consider the declarations
Name “F”
Parameters
Body
“x”
F = x^2 + 5;
G(x,y,z) = 2 y^3 x^4 + 3 x z^2;
When F is parsed, we have the following in the polynomial table
Notice how the list of parameters is given as “x”. This is the default when there is no explicit parameter
list given. This is just an illustration. In the implementation, you will store the line numbers (full tokens)
and as discussed earlier, you can have a separate structures for the header which will contain the
polynomial name and the parameter information.
After x^2 + 1 is parsed, the new polynomial representation becomes the following
Name “F”
Parameters
Body
“x”
( 5 , NULL) ? NULL
(1, ) +
( ,2)
( VAR ,1, NULL )
This requires some explanation
• (1, [( ,2)] , +, ): The 1 is the coefficient. The monomial list has only one element representing the primary “x”. The primary “x” is represented with ( VAR , 1 , NULL )
: the VAR indicates that this
primary is only a variable (“x”) and the 1 is the order of “x” in the list of parameters since “x” is
the first parameter. The NULL is for the term_list field of the primary which is not used when the
primary is a VAR. The 2 is the exponent of the primary “x”. The + represents the + symbol in the
+ term_list for this polynomial and the is a pointer to a term list which represents the term
list representing the constant 5 in the polynomial (the constant 5 is a term list containing one
term which is only a coefficient).
• (5, [], ?, NULL ): The 5 is the coefficient. Since there is no monomial list, the monomial list is
empty. The ? represents the value of the operator field which is not set because there is no next
term list (next term list is NULL)
After parsing G(x,yz), the representation becomes as follows
“x”
Name “F”
Parameters
Body
(1, ) + ( 5 , NULL) ? NULL
( ,2)
Note: this representation assumes
that variables are counted starting
from index 1. for instance, ”x” is the
first parameter and is represented
with 1. In your implementation, you
can start from index 0.
Name “G”
Parameters
Body
( VAR ,1, NULL )
“x” “y” “z”
Representing Polynomial Declarations
We continue with our example:
F = x^2 + 5;
G(x,y,z) = 2 y^3 x^4 - 3 x z^2;
After parsing the body of G(x,y,z), the representation becomes as follows
“x”
Name “F”
Parameters
Body
(1, ) + ( 5 , NULL) ? NULL
( ,2)
Name “G”
Parameters
Body
( VAR ,1, NULL )
“x” “y” “z”
( ,3)
2 -
(2, ) -
y3
( VAR , 2, NULL )
x4
3
x1
( VAR ,1 , NULL )
(3, ) ? NULL
( ,1,
)
( ,4)
( VAR ,1, NULL ) z2
( VAR ,3, NULL )
(3,2)
Let us examine the first term of the body
• (2, [( ,3),( ,4)], - , ): The 2 is the coefficient in front of y^3 x^4. The first element in
the list of monomials of the first term is ( ,3) which represents y^3. The arrow points
to (VAR,2) which represents y because y is the second parameter in the list of parameters of the polynomial G. The 3 represents the power to which y is raised. The
second element in the list of monomials of the first term ( ,4) represents x^4. The
arrow points to (VAR,1) which represents x because x is the first parameter in the list
of parameters of the polynomial G. the 4 represents the power to which x is raised.
The – represents the minus sign in – term_list for this polynomial.
• Work out the second part and convince yourself that it represents 3 x z^2.
The representation above does not show the field term_list pointer in the primary data
structure because in the example we are only dealing with primarys that are ID. If a primary
is a term_list, then the data structure will have TERM_LIST instead of VAR and will point to
a term_list struct (recursive representation).
Note: this representation assumes that variables are counted starting from index 1. for instance, ”x” is the first
parameter and is represented with the number 1, “y” is the second parameter and is represented with the number 2
and so on. In your implementation, you can start from index 0.
Representing the program as a linked list
The goal of this step is to represent the program as a data structure that can be easily
“executed” later. I show what the data structure looks like for an example program
TASKS 1 2
stmt_type the statement type is an integer that indicates the type of the statement. It can be
POLY
INPUT, OUTPUT or ASSIGN (you can use whatever
F = x^2 + 5;
names you want as long as you distinguish between
the cases)
G(x,y,z) = 2 y^3 x^4 - 3 x z^2;
LHS This field is only relevant for assignment
EXECUTE
statements. It is an integer which is equal to the index of the location associated with the variable on
INPUT X;
the LHS of an assignment statement.
INPUT Y;
poly_eval This field is relevant only for if the
statement type is an assignment statement. The value is a pointer to a data structure that
Y = F(X,W);
represents the polynomial evaluation statement.
W = F(X,W);
var This field is relevant for INPUT and OUTPUT
OUTPUT W;
statements. It is an integer which is equal to the index of the location associated with the variable in
INPUT and OUTPUT statements.
INPUTS 1 4 17
data structure for a statement
int stmt_type
int LHS
struct poly_eval_t *
int var
struct stmt * next;
You should declare the data structures for statements yourself in your program. It is
not provided with the provided code.
Now that we know how a statement is represented, we can look on the next page at
the representation of the program above
Representing Polynomial for Evaluation
Polynomial evaluation statements are the most involved data structures in the program, but they
are not really hard to work with. A polynomial evaluation needs the following information
1. 2. A reference to the polynomial. This can be the index of the polynomial entry in the
polynomial declaration table
The list of arguments. An argument to a polynomial evaluation can be either a NUM, or
an ID or a polynomial evaluation. We discuss each case
1. NUM. If the argument is NUM, then the actual value of the argument needs to
be computed at compile time and stored in the polynomial evaluation statement
2. ID. If the argument is the name of a variable, then the index of the locations
associated with the variable must be stored in the argument
3. Polynomial Evaluation. If the argument is a polynomial evaluation, then we need a
pointer to a polynomial evaluation data structure (recursive representation).
It should be clear that, in order to distinguish between the various kinds of arguments, we will
need a flag that specifies the argument type. I will assume that the flag takes the values NUM,
ID, and POLYEVAL (you need to define them). I illustrate these ideas with an example program
on the next page for the following program
TASKS 1 2
POLY
F = x^2 + 5;
G(x,y,z) = 2 y^3 x^4 - 3 x z^2;
EXECUTE
INPUT X;
INPUT Y;
Y = F(1);
W = G( G(4,X,Y) , 2 , 3)
OUTPUT W;
INPUTS 1 4 17
A poly_eval_t struct can be declared as follows
struct poly_eval_t {
int kind; int value; int index; struct poly_eval_t * pe; // NUM, ID, or POLYEVAL
// Value of NUM
// index of ID
// pointer to another
// poly_eval_t struct
}
“x”
Name “F”
Parameters
Body
(1, ) + ( 5 , NULL) ? NULL
( ,2)
Name “G”
Parameters
Body
( VAR ,1, NULL )
Representing Polynomial for Evaluation: Example
poly_declarations
Consider the following program
TASKS 1 2
POLY
F = x^2 + 5;
G(x,y,z) = 2 y^3 x^4 - 3 x z^2;
EXECUTE
INPUT X;
INPUT Y;
Y = F(1);
W = G(G(4,X,Y),2,3)
OUTPUT W;
INPUTS 1 4 17
This program will be represented as follows
INPUT X;
X
Y
“x” “y” “z”
(2, ) -
(3, ) ? NULL
( ,3)
( ,1,
)
( VAR , 2, NULL )
( VAR ,1 , NULL )
stmt_type INPUT
LHS -1
poly_eval NULL
var 0
( ,4)
(3,2)
INPUT Y;
stmt_type INPUT
LHS -1
poly_eval NULL
var 1
( VAR ,1, NULL ) ( VAR ,3, NULL )
polynomial F
argument is
constant 1
input
Y = F(1);
1
Y
stmt_type ASSIGN
LHS 1
poly_eval
var -1
Poly 0
arguments
Type NUM
value 1
index
poly_eval NULL
2
4
8
16
W = G(G(4,X,Y),2,3)
W
polynomial G
(G(4,X,Y),2,3)
stmt_type ASSIGN
LHS 2
poly_eval
var -1
polynomial G
Poly 1
arguments
OUTPUT W;
Type POLYEVAL
value
index
poly_eval
(4,X,Y)
Poly 1
arguments
W
stmt_type OUTPUT
LHS -1
poly_eval NULL
var 2
Type NUM
value 2
index
poly_eval NULL
Type NUM
value 4
index
poly_eval NULL
Type NUM
value 3
index
poly_eval NULL
Type ID
value
index 0
poly_eval NULL
Type ID
value
index 1
poly_eval NULL
4
X
Y
Generating a statement list
So far, we discussed how to generate the representation of individual statements.
One issue that remains is how to generate the list of statements. This is explained
in the following code fragment
struct stmt* parse_statement_list()
{
struct stmt * st;
struct stmt * stl;
...
st = parse_stmt();
// if there no more input for the statement list
return st;
// if there is more input
stl = parse_statement_list();
// append stl to st
// return st
}
Executing the program
So far, we have discussed what the parser should do. Here are the things that
the parser should do:
1. build the symbol table
2. build the polynomials tables
3. generate the linked list that represents the program.
The actual “execution” of the input program happens after all these steps are
done. For that, we introduce the execute_program() functiom.
The execute_program() function is a function that takes as argument a
pointer to the first statement node of the program. Then, execute_program() will
iterate over the nodes one at a time and “execute” the node. The following is an
illustration of the high-level loop of execute_program() which also shows how
some statements are executed.
execute_program(struct stmt * start)
{ struct stmt * pc;
int v;
pc = start;
while (pc != NULL) {
switch (pc->stmt_type) {
case POLYEVAL: v = evaluate_polynomial(pc->poly_eval);
cout << v << endl; case INPUT: mem[pc->variable] = inputs[next_input];
next_input++;
break;
// endl is std::endl
....
}
pc = pc->next;
}
}
This code fragment captures the essential of execute_program(). One variable to
highlight is the next_input variable which keeps track of how many inputs are
consumed. Note how next_input is incremented after an input is consumed (by
executing an INPUT statement).
Evaluating Polynomials
What remains to be done is to evaluate the polynomial statements. In order to evaluate
the polynomial, we need to first evaluate the arguments. If the argument is NUM or ID,
the evaluation is straightforward. If the argument is POLYEVAL, then the evaluation function is called recursively to get the value of the argument*. So, the only remaining
part is to explain how to evaluate a polynomial once all arguments have been evaluated.
The evaluation function will have all the argument values stored in a vector and will go
to the representation of the polynomial to be evaluated and evaluate each component of
the polynomial using the arguments already calculated. I omit the details which you
should work on figuring out.
* In a real compiler, procedures are not executed recursively. They are executed
iteratively. Also, they have parameters. This makes procedure execution significantly
more involved than what we are able to do in a first project.
