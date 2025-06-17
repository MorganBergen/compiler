#  input format

a context free grammar defines howinput to your compiler is structured.  it clearly specifies how valid programs can be written.

key terms

non terminals - sybols on the left side of the rules.  they are "expanded" into terminals and/or other non terminals.

terminals -  actual tokens that are recognized by the lexical anlyzer (e.g. `NUM`, `ID`, `PLUS`)

rules / productions -  define how non terminals expand

given the grammar snippet

```
poly_decl -> poly_header EQUAL poly_body SEMICOLON
poly_header -> poly_name
poly_header -> poly_name LPAREN id_list RPAREN
```

`poly_decl`, `poly_header`, `poly_body`, `id_list`
