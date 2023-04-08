lexer grammar CLexer;

fragment CHARACTER: [a-zA-Z];
fragment DIGIT: [0-9];

INTEGER: '0' | [1-9] DIGIT*;
STRING: '"' (.)*? '"';

VOID: 'void';
INT: 'int';
CHAR: 'char';
FLOAT: 'float';
DOUBLE: 'double';
SHORT: 'short';
LONG: 'long';

CONST: 'const';
SIGNED: 'signed';
UNSIGNED: 'unsigned';
STATIC: 'static';

ENUM: 'enum';
STRUCT: 'struct';
UNION: 'union';

HEADER_FILE: '<' ~[\\/|:*?",+! .]+ '.h' '>';
INCLUDE: '#include';
TYPEDEF: 'typedef';
RETURN: 'return';
SIZEOF: 'sizeof';

CONTINUE: 'continue';
BREAK: 'break';
FOR: 'for';
WHILE: 'while';
DO: 'do';

IF: 'if';
ELSE: 'else';

SWITCH: 'switch';
CASE: 'case';

OPENPAR: '(';
CLOSEPAR: ')';
LBRACKET: '[';
RBRACKET: ']';
LBRACE: '{';
RBRACE: '}';
COMMA: ',';
SEMICOLON: ';';
COLON: ':';
PERIOD: '.';

ADD: '+';
SUB: '-';
MULTIP: '*';
DIV: '/';
MODULO: '%';

EQUAL: '==';
NOT_EQUAL: '!=';
GREATER_THAN: '>';
LESS_THAN: '<';
GREATER_EQUAL: '>=';
LESS_EQUAL: '<=';

NOT: '!';
AND: '&&';
OR: '||';

ASSIGN: '=';
ADD_ASSIGN: '+=';
SUB_ASSIGN: '-=';
MULTIP_ASSIGN: '*=';
DIV_ASSIGN: '/=';
MODULO_ASSIGN: '%=';

INCREMENT: '++';
DECREMENT: '--';

COMMENT_NEWLINE: '/*' (.)*? '*/' -> channel(HIDDEN);
COMMENT: '//' (.)*? '\n' -> channel(HIDDEN);

ID: ('_' | CHARACTER) (CHARACTER | DIGIT | '_')*;

WS: [ \n\t\r]+ -> skip;
INVALID: .;