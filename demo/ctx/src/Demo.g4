grammar Demo; // Combined grammar for lexer and parser

// Conventions:
// - parser rules start with lowe case
// - lexer reules start with upper case

//
// Program
//

program
    :   statement (NEWLINE statement)*
    ;


//
// Statements
//

statement
    :   assignment
    |   print
    ;
    
assignment
    :   IDENTIFIER ASSIGN expression
    ;

print
    :   'print' value=expression #PrintStatement
    ;


//
// Expressions
//

// Order of rules implies order of precedence
expression
    :   '(' expression ')' #ParanthesisExpression
    |   op= (ADD | SUB) expression          # UnaryOperationExpression
    |   lhs=expression MUL rhs=expression # MulOperationExpression
    |   lhs=expression DIV rhs=expression # DivOperationExpression
    |   lhs=expression ADD rhs=expression # AddOperationExpression
    |   lhs=expression SUB rhs=expression # SubOperationExpression
    |   name=IDENTIFIER # VariableExpression
    |   value=literal # LiteralExpression
    ;


//
// Operators
//

ASSIGN
    : '='
    ;

ADD : '+'
    ;

SUB : '-'
    ;

MUL : '*'
    ;

DIV : '/'
    ;


//
// Literals
//

literal
	:   integerLiteral
    |   stringLiteral
	;

integerLiteral
    :   NUMBER
    ;
    
stringLiteral
    :   '"' CHAR* '"'
    ;

fragment
CHAR
    :   ~["\\\r\n]
    |   ESCAPE_SEQUENCE
    ;

fragment
ESCAPE_SEQUENCE
    :   SIMPLE_ESCAPE_SEQUENCE
    |   HEXADECIMAL_ESCAPE_SEQUENCE
    ;

fragment
SIMPLE_ESCAPE_SEQUENCE
    :   '\\' ["?abfnrtv\\]
    ;

fragment
HEXADECIMAL_ESCAPE_SEQUENCE
    :   '\\x' HEXADECIMAL_DIGIT+
    ;

fragment
NUMBER
    :   [0-9]+
    ;

fragment
HEXADECIMAL_DIGIT
    :   [0-9a-fA-F]
    ;

//
// Identifiers
//

IDENTIFIER
	:	LETTER LETTER_OR_DIGIT*
	;

fragment
LETTER
	:	[a-zA-Z$_]
	;

fragment
LETTER_OR_DIGIT
	:	[a-zA-Z0-9$_]
	;


//
// Whitespace and comments
//

WS  :  [ \t\u000C]+ -> skip
    ;
    
NEWLINE
   : '\r'? '\n'
   ;

COMMENT
    :   '/*' .*? '*/' -> skip
    ;

LINE_COMMENT
    :   '//' ~[\r\n]* -> skip
    ;