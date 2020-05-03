%{
#define YYSTYPE char*
#include <stdio.h>

extern int yylineno;
void yyerror (const char *s) /* Called by yyparse on error */
{
if(strcmp(s,"syntax error")==0)
printf("ERROR\n");
else
fprintf(stderr, "%d %s\n", yylineno, s);
}

const char * array[100];
int numVars = 0;
int bodyVars = 0;

void deleteBody()
{
while(0 < bodyVars)
{
int c = 0;
for (c = numVars - 1; c < numVars; c++)
array[c] = array[c+1];

numVars = numVars - 1;
bodyVars = bodyVars - 1;
}
}
void push(const char *str)
{
array[numVars] = str;
numVars = numVars + 1;
}
void bodypush(const char *str)
{
array[numVars] = str;
numVars = numVars + 1;
bodyVars = bodyVars +1;
}

int search(const char * str)
{
int i = 0;
while(i < numVars)
{
if(strcmp(array[i],str)==0)
{
return 1;
}
i = i + 1;
}
yyerror("Undefined Variable");
return 0;
}

int searchVar(const char * str)
{
int i = 0;
while(i < numVars)
{
if(strcmp(array[i],str)==0)
{
yyerror("Redefinition of variable");
return 1;
}
i = i + 1;
}
return 0;
}


%}

%locations

%token tINT tMOD tSTRING tRETURN tPRINT tLPAR tRPAR tCOMMA tASSIGNM tMINUS tPLUS tDIV tSTAR tSEMI tLBRAC tRBRAC tIDENT tINTVAL tSTRINGVAL

%%
prog: stmtlst
;
stmtlst: stmt
|stmt stmtlst
;
stmt: vardecl
|funcdef
|assgn
|print
;
bodystmt:bodyvardecl
|assgn
|print
;
type: tINT
|tSTRING
;

vardecl: type tIDENT tASSIGNM expr tSEMI {
int c = searchVar($2);
if(c !=0)
{
YYABORT;
}
push($2);}
;

bodyvardecl: type tIDENT tASSIGNM expr tSEMI {
int d = searchVar($2);
if(d !=0)
{
YYABORT;
}
bodypush($2);}
;

expr: value
| expr tPLUS expr
| expr tMINUS expr
| expr tSTAR expr
| expr tDIV expr
| expr tMOD expr
| funcall
;

value: tIDENT {
int a = search($1);
if(a == 0)
{
YYABORT;
}
}
|tINTVAL
|tSTRINGVAL
;

actual: value
|value tPLUS value
|value tMINUS value
|value tSTAR value
|value tDIV value
;

funvalue: actual
|actual tCOMMA funvalue
;

funcall: tIDENT tLPAR funvalue tRPAR {

int x = search($1);
if(x == 0)
{
YYABORT;
}
}
;
assgn: tIDENT tASSIGNM expr tSEMI {

int b = search($1);
if(b == 0)
{
printf("a");
YYABORT;
}
}
;

funcdef: type tIDENT tLPAR parameters tRPAR tLBRAC body tRBRAC {
int z = searchVar($2);
if(z !=0)
{
YYABORT;
}
push($2);}
;

parameters: type tIDENT
{
int e = searchVar($2);
if(e !=0)
{
YYABORT;
}
bodypush($2);
}
| type tIDENT tCOMMA parameters
{
int f = searchVar($2);                                                                                                                                                                                                                       if(f !=0)
{
YYABORT;
}
bodypush($2);
}
;

body: bodystmt body
| bodystmt return
;

return: tRETURN expr tSEMI
{
deleteBody();
}
;

print: tPRINT tLPAR expr tRPAR tSEMI
;



%%
int main ()
{
if (yyparse()) {
return 0;
}
else {
printf("OK\n");
return 0;
}
}
