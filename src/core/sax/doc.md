SAX is language similar to VEX in Houdini, it'll be used to edit mesh attributes.

Variables
------------------------------------------------------------------------------------
float var = 5.0; -> local variable named var with value 5.0.
(type)$var = ...; -> mesh variable name var of type `type`, i.e float$var = 5.0; vector$P = {5.0, 1.0, 2.0};

all basis operators will be available such as: +, -, *, /

Abstract Syntax tree
------------------------------------------------------------------------------------

assignment
    -> identifier '=' assignment
    | logical

logical
    -> comparison ( ( '&&' | '||' ) comparison )*

comparison
    -> term ( ( '<' | '>' | '==' | '!=' ) term )*

term
    -> expression ( ( '+' | '-' ) expression )*

expression
    -> unary ( ( '*' | '/' ) unary )*

unary
    -> ( '-' | '!' ) unary
    | primary

primary
    -> literal
    | identifier
    | function_call
    | '(' assignment ')'
