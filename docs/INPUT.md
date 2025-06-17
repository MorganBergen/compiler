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
