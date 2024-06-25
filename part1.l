%{
#include <stdio.h>
#include "y.tab.h"

int counter_main=0;
int counter=0;




%}

%%

"false"|"true"   {yylval.s=strdup(yytext);return LITERALBOOL;  }
"main"   {yylval.s=strdup(yytext);return MAIN;} 
"bool" {yylval.s=strdup(yytext);return BOOL;}
"char" {yylval.s=strdup(yytext);return CHAR;}
"int" {yylval.s=strdup(yytext);return INT;}
"real" {yylval.s=strdup(yytext);return REAL;}
"string" {yylval.s=strdup(yytext); return STRING;}
"int*" {yylval.s=strdup(yytext);return INTPOINTER;}
"char*" {yylval.s=strdup(yytext);return CHARPOINTER;}
"real*" {yylval.s=strdup(yytext);return REALPOINTER;}
"if" {yylval.s=strdup(yytext);return IF;}
"else"  {yylval.s=strdup(yytext);return ELSE;}
"while" {yylval.s=strdup(yytext);return WHILE;}
"for" { yylval.s=strdup(yytext);return FOR;};
"var" { yylval.s=strdup(yytext);return VAR;}
"!" {yylval.s=strdup(yytext);return NOT;}

"arg->" { yylval.s=strdup(yytext);return ARG;}
"function" { yylval.s=strdup(yytext);return FUNCTION;}
"return" {yylval.s=strdup(yytext);return RETURN;}
"null" {yylval.s=strdup(yytext);return NULLL;}
"void"  {yylval.s=strdup(yytext);return VOID; }
"do" {yylval.s=strdup(yytext);return DO;}
"=="|">"|"<"|">="|"<="|"!="  {yylval.s=strdup(yytext); return OPERATORBOOL;}
"++"	{yylval.s=strdup(yytext);return INC;}
"--"	{ yylval.s=strdup(yytext);return DEC;}
"&&"|"||" {yylval.s=strdup(yytext);return OPERATORLOG;}
[0-9]+ {yylval.s=strdup(yytext);return NUMBER;}
-?(([0-9]+)|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?) {yylval.s=strdup(yytext);return VALREAL;}
\\n|\\t|\\ {yylval.s=strdup(yytext);return SLASH;}
\'\\0\'|\'\\n\'|\'.\' 	{yylval.s=strdup(yytext);return VALCHAR;}
\/\*.*\*\/ {yylval.s=strdup(yytext);return COMMENT;}
[a-zA-Z]([a-zA-Z0-9_])* {yylval.s=strdup(yytext);return IDENTIFIER;} 
[ \t]+	;   
. {yylval.s=strdup(yytext);return yytext[0];}
\n	yylineno++;
\".*\" {yylval.s=strdup(yytext);return VALSTRING;} 

%%