%{
#include <stdio.h>

void yyerror (const char *s) /* Called by yyparse on error */
{

}
%}
%token tINTTYPE tINTVECTORTYPE tINTMATRIXTYPE tREALTYPE tREALVECTORTYPE tREALMATRIXTYPE tTRANSPOSE tIDENT tDOTPROD tIF tENDIF tREALNUM tINTNUM tAND tOR tGT tLT tGTE tLTE tNE tEQ
%left tOR
%left tAND
%left '+' '-'
%left '*' '/'
%left tDOTPROD
%left tTRANSPOSE


%% /* Grammar rules and actions follow */

prog: stmtlst
;
stmtlst: stmt
|stmt stmtlst
;
stmt: decl
|asgn
|if
;
decl: type vars '=' expr ';'
;
type: tINTTYPE
| tINTVECTORTYPE
| tINTMATRIXTYPE
| tREALTYPE
| tREALVECTORTYPE
| tREALMATRIXTYPE
;    
vars: tIDENT
| tIDENT ',' vars
;
value: tINTNUM
| tREALNUM
| tIDENT
;
asgn: tIDENT '=' expr ';'
;
if: tIF '(' bool ')' stmtlst tENDIF
;
expr : value
| vectorLit
| matrixLit
| transpose
| expr '+' expr
| expr '-' expr
| expr '*' expr
| expr '/' expr
| expr tDOTPROD expr
;
transpose :  tTRANSPOSE '(' expr ')'
;
row : value 
| value ',' row
;
rows : row
| row ';' rows
;
vectorLit: '[' row ']' 
;
matrixLit: '[' rows ']'
;
bool: comp
| bool tAND bool
| bool tOR bool
;
comp: tIDENT relation tIDENT
;
relation: tGT
| tLT
| tGTE
| tLTE
| tNE
| tEQ
;
%%
int main ()
{
if (yyparse()) {
printf("ERROR\n");
return 0;
}
else {
printf("OK\n");
return 0;
}
}
