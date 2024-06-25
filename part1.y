%{
#include <stdio.h>
#include "lex.yy.c"
#include <string.h>
#include <stdlib.h>
#include "Sematica.c"


%}
%union{
        char *s;
        struct node *n;
};

%type<n> s p blook blooks functions function  statement init inits for_st while_st return_st des_fun if_st stat exprs expr  assign lhs var_declare arg option3 type_f type option1 option2 AS AS1 val 
%token<s> BOOL CHAR INT REAL STRING INTPOINTER CHARPOINTER REALPOINTER IF ELSE WHILE FOR VAR ARG FUNCTION RETURN NULLL VOID DO OPERATORBOOL OPERATORLOG NOT NUMBER IDENTIFIER LITERALBOOL VALSTRING SLASH VALREAL COMMENT MAIN VALCHAR INC DEC
%nonassoc then
%nonassoc ELSE
%left OPERATORLOG
%left OPERATORBOOL
%left '&' 
%left '*' '/'
%left '+' '-'



%%
s: functions    {node **sons = sonArr(1);sons[0]=$1;$$ = mknode("Code:",sons,1);start_check($$,counter_main);printtree($$,1);}  


functions: 
    function functions  {node **sons = sonArr(2);sons[0]=$1;sons[1]=$2;$$ = mknode("functions",sons,2);} 
    |{$$ = mknode("",NULL,0);}
    |COMMENT functions  {node **sons = sonArr(1);sons[0]=$2;$$ = mknode("functions",sons,1);} 
    
    ;
function:
    FUNCTION IDENTIFIER '(' ')'':' type_f '{' blooks '}'    {node **sons = sonArr(3);sons[0]=mknode($2,NULL,0);sons[1]=$6;sons[2]=$8;$$ = mknode("FUNCTION1",sons,3);AddType($$,$6->type);} 
    |FUNCTION IDENTIFIER '(' ARG IDENTIFIER option3 ':' type_f arg ')'':' type_f '{' blooks '}'   {node **sons = sonArr(8);sons[0]=mknode($2,NULL,0);sons[1]=mknode($4,NULL,0);sons[2]=mknode($5,NULL,0);sons[3]=$6;sons[4]=$8;sons[5]=$9;sons[6]=$12;sons[7]=$14;$$ = mknode("FUNCTION2",sons,8);AddType($$,$12->type);AddType($6,$8->type);} 
    |FUNCTION MAIN '(' ')'':' VOID '{' blooks '}'   {counter_main++ ; node **sons = sonArr(3);sons[0]=mknode($2,NULL,0);sons[1]=mknode($6,NULL,0);sons[2]=$8;$$ = mknode("FUNCTION1",sons,3);AddType($$,"void");} 
    ;

blooks:
    '{' blooks '}' p   {node **sons = sonArr(2);sons[0]=$2;sons[1]=$4;$$ = mknode("blooks",sons,2);} 
    |blook blooks    {node **sons = sonArr(2);sons[0]=$1;sons[1]=$2;$$ = mknode("statements",sons,2);}  
    |blook {$$ = $1;}
    ;
p: blooks {$$ = $1;}
;
blook:
    var_declare {$$=$1;}
    |statement  {$$=$1;}
    |COMMENT {printf("%s\n",$1);$$ = mknode($1,NULL,0);}
    |{$$ = mknode("",NULL,0);}
    ;


statement: 
    des_fun    {$$=$1;}
    |des_fun ';'   {$$=$1;}
    |function   {$$=$1;}
    |while_st   {$$=$1;}
    |for_st {$$=$1;}
    |assign ';' {$$=$1;}
    |if_st  {$$=$1;}
    |exprs ';'  {$$=$1;}
    |';'    {$$ = mknode("",NULL,0);} 
    |return_st {$$=$1;}

    ;


 


/* -------------------------------------------------------------------------------*/
init: 
    IDENTIFIER {$$ = mknode($1,NULL,0);}
    |IDENTIFIER '=' NUMBER  {node **sons = sonArr(2);sons[0]=mknode($1,NULL,0);sons[1]=mknode($3,NULL,0);$$ = mknode("=",sons,2);}
    ;
inits: 
    init ',' inits  {node **sons = sonArr(2);sons[0]=$1;sons[1]=$3;$$ = mknode(",",sons,2);}
    |init   {$$=$1;}
    ;
for_st:
    FOR '(' inits ';' expr ';' exprs ')' stat  {node **sons = sonArr(4);sons[0]=$3;sons[1]=$5;sons[2]=$7;sons[3]=$9;$$ = mknode("FOR",sons,4);}
    ;

while_st:
    WHILE '(' expr ')' stat  {node **sons = sonArr(2);sons[0]=$3;sons[1]=$5;$$ = mknode("WHILE",sons,2);}
    |DO '{' blooks '}' WHILE '(' expr ')' ';'   {node **sons = sonArr(3);sons[0]=$3;sons[1]=mknode($5,NULL,0);sons[2]=$7;$$ = mknode("DO",sons,3);}
    ;

return_st:
    RETURN expr  ';'  {node **sons = sonArr(1);sons[0]=$2;$$ = mknode("RETURN",sons,1);AddType($$,$2->type);}    
    ; 

des_fun: 
    IDENTIFIER '(' exprs ')'   {node **sons = sonArr(2);sons[0]=mknode($1,NULL,0);sons[1]=$3;$$ = mknode("function call",sons,2);}  
    ;


if_st:
     IF '(' expr ')' stat  %prec then  {node **sons = sonArr(2);sons[0]=$3;sons[1]=$5;$$ = mknode("IF",sons,2);} 
    |IF '(' expr ')'  stat  ELSE stat  {node **sons = sonArr(4);sons[0]=$3;sons[1]=$5 ;sons[2]=mknode($6,NULL,0);sons[3]=$7;$$ = mknode("IFELSE",sons,4);} 
    ;

stat:
    '{' blooks '}'  {node **sons = sonArr(1);sons[0]=$2;$$ = mknode("blook",sons,1);} 
    |blook  {$$=$1;}
    ;

exprs:  
    expr ',' exprs {node **sons = sonArr(2);sons[0]=$1;sons[1]=$3;$$ = mknode("expr2",sons,2);} 
    |expr   {node **sons = sonArr(1);sons[0]=$1;$$ = mknode("expr1",sons,1);}
    |{$$ = mknode("",NULL,0);}
    ;

expr:
  
    expr OPERATORBOOL  expr   {node **sons = sonArr(2);sons[0]=$1;sons[1]=$3; $$ = mknode($2,sons,2);}
    |expr OPERATORLOG  expr   {node **sons = sonArr(2);sons[0]=$1;sons[1]=$3; $$ = mknode($2,sons,2);}
    
    |expr '+' expr  {node **sons = sonArr(2);sons[0]=$1;sons[1]=$3; $$ = mknode("+",sons,2);}
    |expr '-' expr   {node **sons = sonArr(2);sons[0]=$1;sons[1]=$3; $$ = mknode("-",sons,2);}
    |expr '*' expr   {node **sons = sonArr(2);sons[0]=$1;sons[1]=$3; $$ = mknode("*",sons,2);}
    |expr '/' expr  {node **sons = sonArr(2);sons[0]=$1;sons[1]=$3; $$ = mknode("/",sons,2);}
    |val  {$$=$1;}
    |des_fun    {$$=$1;}
    |'(' expr ')'   {node **sons = sonArr(1);sons[0]=$2;$$ = mknode("()",sons,1);} 
    |IDENTIFIER INC {node **sons = sonArr(1);sons[0]=mknode($1,NULL,0);$$ = mknode("++",sons,1);AddType($$,"int");} 
    |IDENTIFIER DEC {node **sons = sonArr(1);sons[0]=mknode($1,NULL,0);$$ = mknode("--",sons,1);AddType($$,"int");} 
    |assign {$$=$1;}
    |NOT expr {node **sons = sonArr(1);sons[0]=$2;$$ = mknode("!",sons,1);}
    | '|' IDENTIFIER '|'   {node **sons = sonArr(1);sons[0]=mknode($2,NULL,0);$$ = mknode("|",sons,1);AddType($$,"int");} 
    ;



assign:
   lhs '=' expr   {node **sons = sonArr(2);sons[0]=$1; sons[1]=$3;$$ = mknode("=",sons,2);}  
   |lhs '=' '|' expr '|'    {node **sons = sonArr(2);sons[0]=$1; sons[1]=$4 ;$$ = mknode("|=|",sons,2);} 
   ;

lhs:
    IDENTIFIER  {node **sons = sonArr(1);sons[0]=mknode($1,NULL,0);$$ = mknode("id",sons,1);}
    |'&' expr {node **sons = sonArr(1);sons[0]=$2; $$ = mknode("&",sons,1);}
    |'*' IDENTIFIER {node **sons = sonArr(1);sons[0]=mknode($2,NULL,0); $$ = mknode("pointer",sons,1);}
    |IDENTIFIER '[' expr ']'     {node **sons = sonArr(2);sons[0]=mknode($1,NULL,0); sons[1]=$3; $$ = mknode("[]",sons,2);}  
    |'&' expr '[' expr ']'     {node **sons = sonArr(2);sons[0]=$2; sons[1]=$4; $$ = mknode("&[]",sons,2);}  
;
var_declare:  
    VAR AS option1  ':' type ';'  {node **sons = sonArr(3);sons[0]=$2; sons[1]=$3;sons[2]=$5; $$ = mknode("VAR1",sons,3);AddType($$,$5->type);}  
    |STRING AS1 option2 ';' {node **sons = sonArr(3);sons[0]=mknode($1,NULL,0); sons[1]=$2;sons[2]=$3;  $$ = mknode("VAR2",sons,3);AddType($$,"string");}
    ;
arg:
    ';' ARG IDENTIFIER option3 ':' type_f arg {node **sons = sonArr(4);sons[0]=mknode($3,NULL,0); sons[1]=$4;sons[2]=$6; sons[3]=$7; $$ = mknode("ARG",sons,4);AddType($4,$6->type);}
    |{$$ = mknode("",NULL,0);}
    ;

option3: 
   
    ','IDENTIFIER option3 {node **sons = sonArr(2);sons[0]=mknode($2,NULL,0); sons[1]=$3;$$ = mknode(",",sons,2);AddType($3,$$->type);$$->count_arg++;}  
    |{$$ = mknode("",NULL,0);}
    ;

type_f:
    type   {$$ = $1;}
    |STRING {$$ = mknode($1,NULL,0);AddType($$,"string");}
    |VOID {$$ = mknode($1,NULL,0);AddType($$,"void");}
    ;


type:
    BOOL   {$$ = mknode($1,NULL,0);AddType($$,"bool");}
    |CHAR   {$$ = mknode($1,NULL,0);AddType($$,"char");}
    |INT    {$$ = mknode($1,NULL,0);AddType($$,"int");}
    |REAL   {$$ = mknode($1,NULL,0);AddType($$,"real");}
    |INTPOINTER {$$ = mknode($1,NULL,0);AddType($$,"intpointer");}
    |CHARPOINTER    {$$ = mknode($1,NULL,0);AddType($$,"charpointer");}
    |REALPOINTER    {$$ = mknode($1,NULL,0);AddType($$,"realpointer");}
    ;
option1:
    ','AS option1 {node **sons = sonArr(2);sons[0]=$2; sons[1]=$3;$$ = mknode(",",sons,2);} 
    |{$$ = mknode("",NULL,0);}
    ;
    
option2:
    ',' AS1 option2  {node **sons = sonArr(2);sons[0]=$2; sons[1]=$3;$$ = mknode(",",sons,2);AddType($$,"string");} 
    |{$$ = mknode("",NULL,0);}
    ;
AS1:IDENTIFIER '['NUMBER ']' '=' val   {node **sons = sonArr(3);sons[0]=mknode($1,NULL,0);sons[1]=mknode($3,NULL,0);sons[2]=$6;$$ = mknode("=",sons,3);AddType($$,"string");} 
    |IDENTIFIER '['NUMBER ']' {node **sons = sonArr(2);sons[0]=mknode($1,NULL,0);sons[1]=mknode($3,NULL,0);$$ = mknode("[]",sons,2);AddType($$,"string");}
    |{$$ = mknode("",NULL,0);}
    ;

AS:
    IDENTIFIER '=' val   {node **sons = sonArr(2);sons[0]=mknode($1,NULL,0);sons[1]=$3;$$ = mknode("=",sons,2);} 
    |IDENTIFIER {$$ = mknode($1,NULL,0);}
    |{$$ = mknode("",NULL,0);}
    ;
val:
    VALSTRING   {$$ = mknode($1,NULL,0);AddType($$,"string");}
    |lhs {$$ = $1;AddType($$,$1->type);}
    |LITERALBOOL    {$$ = mknode($1,NULL,0);AddType($$,"bool");}
    |NUMBER {$$ = mknode($1,NULL,0);AddType($$,"int");}
    |VALREAL    {$$ = mknode($1,NULL,0);AddType($$,"real");}
    |NULLL  {$$ = mknode($1,NULL,0);AddType($$,"pointer");}
    |VALCHAR    {$$ = mknode($1,NULL,0);AddType($$,"char");}
    |des_fun    {$$ = $1;}
    ;

    
   

%%



int yyerror(char *e){
	int yydebug=1;
	fflush(stdout);
	fprintf(stderr,"Error %s at line %d\n",e,yylineno);
	fprintf(stderr,"does not accept '%s'\n",(yytext));
	return 0;
}
int main(){
	return yyparse();
}