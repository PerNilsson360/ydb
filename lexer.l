NON_ZERO_DIGIT          [\x31-\x39]
DIGIT			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%{
#include <stdio.h>
#include "YangAst.hh"
#include "y.tab.hh"
void count();
int yyerror(const char *m);
#include <string>
/* %option debug */
%}

%option noyywrap
%option yylineno

%x str
%x double_quoted_string
%x single_quoted_string
%x identifier
%x qname
%x schema_path
%x date
%x range
%x length
%x integer
%x comment
%x unknown
%x status

%%
"action"                                                        { count(); BEGIN(identifier); return(ACTION_KW); }
"anydata"                                                       { count(); BEGIN(identifier); return(ANYDATA_KW); }
"anyxml"                                                        { count(); BEGIN(identifier); return(ANYXML_KW); }
"argument"                                                      { count(); BEGIN(identifier); return(ARGUMENT_KW); }
"augment"                                                       { count(); BEGIN(schema_path); return(AUGMENT_KW); } 
"base"                                                          { count(); BEGIN(qname); return(BASE_KW); }
"belongs-to"                                                    { count(); BEGIN(identifier); return(BELONGS_TO_KW); }
"bit"                                                           { count(); BEGIN(identifier); return(BIT_KW); }
"case"                                                          { count(); BEGIN(identifier); return(CASE_KW); }
"choice"                                                        { count(); BEGIN(identifier); return(CHOICE_KW); }
"config"                                                        { count(); return(CONFIG_KW); }
"contact"                                                       { count(); BEGIN(str); return(CONTACT_KW); }
"container"	                                                { count(); BEGIN(identifier); return(CONTAINER_KW); }
"default"                                                       { count(); BEGIN(str); return(DEFAULT_KW); } 
"description"                                                   { count(); BEGIN(str); return(DESCRIPTION_KW); }
"enum"                                                          { count(); BEGIN(str); return(ENUM_KW); }
"error-app-tag"                                                 { count(); BEGIN(str); return(ERROR_APP_TAG_KW); }
"error-message"                                                 { count(); BEGIN(str); return(ERROR_MESSAGE_KW); }
"extension"                                                     { count(); BEGIN(identifier); return(EXTENSION_KW); }
"deviation"                                                     { count(); return(DEVIATION_KW); }
"deviate"                                                       { count(); return(DEVIATE_KW); }
"feature"                                                       { count(); BEGIN(identifier); return(FEATURE_KW); }
"fraction-digits"                                               { count(); BEGIN(integer); return(FRACTION_DIGITS_KW); }
"grouping"                                                      { count(); BEGIN(identifier); return(GROUPING_KW); }
"identity"                                                      { count(); BEGIN(identifier); return(IDENTITY_KW); }
"if-feature"                                                    { count(); BEGIN(qname); return(IF_FEATURE_KW); }
"import"                                                        { count(); BEGIN(identifier); return(IMPORT_KW); }
"include"                                                       { count(); BEGIN(identifier); return(INCLUDE_KW); }
"input"                                                         { count(); return(INPUT_KW); }
"key"                                                           { count(); BEGIN(qname); return(KEY_KW); }
"leaf"	                                                        { count(); BEGIN(identifier); return(LEAF_KW); }      
"leaf-list"                                                     { count(); BEGIN(identifier); return(LEAF_LIST_KW); }
"length"                                                        { count(); BEGIN(length); return(LENGTH_KW); }
"list"                                                          { count(); BEGIN(identifier); return(LIST_KW); }       
"mandatory"                                                     { count(); return(MANDATORY_KW); }
"max-elements"                                                  { count(); BEGIN(integer); return(MAX_ELEMENTS_KW); }
"min-elements"                                                  { count(); BEGIN(integer); return(MIN_ELEMENTS_KW); }
"modifier"                                                      { count(); return(MODIFIER_KW); }
"module"		                                        { count(); BEGIN(identifier); return(MODULE_KW); }  
"must"                                                          { count(); BEGIN(str); return(MUST_KW); }
"namespace"                                                     { count(); BEGIN(str); return(NAMESPACE_KW); }
"notification"                                                  { count(); BEGIN(identifier); return(NOTIFICATION_KW); }
"ordered-by"                                                    { count(); return(ORDERED_BY_KW); }
"organization"                                                  { count(); BEGIN(str); return(ORGANIZATION_KW); }
"output"                                                        { count(); return(OUTPUT_KW); }
"path"                                                          { count(); BEGIN(str); return(PATH_KW); }
"pattern"                                                       { count(); BEGIN(str); return(PATTERN_KW); }
"position"                                                      { count(); BEGIN(integer); return(POSITION_KW); }
"prefix"                                                        { count(); BEGIN(identifier); return(PREFIX_KW); }
"presence"                                                      { count(); BEGIN(str); return(PRESENCE_KW); }
"range"                                                         { count(); BEGIN(range); return(RANGE_KW); }
"reference"                                                     { count(); BEGIN(str); return(REFERENCE_KW); }
"refine"                                                        { count(); return(REFINE_KW); }
"require-instance"                                              { count(); return(REQUIRE_INSTANCE_KW); }
"revision"                                                      { count(); BEGIN(date); return(REVISION_KW); }
"revision-date"                                                 { count(); BEGIN(date); return(REVISION_DATE_KW); }
"rpc"                                                           { count(); BEGIN(identifier); return(RPC_KW); }
"status"                                                        { count(); BEGIN(status); return(STATUS_KW); }
"submodule"                                                     { count(); BEGIN(identifier); return(SUBMODULE_KW); }
"type"		                                                { count(); BEGIN(qname); return(TYPE_KW); }
"typedef"                                                       { count(); BEGIN(identifier); return(TYPEDEF_KW); }
"unique"                                                        { count(); BEGIN(schema_path); return(UNIQUE_KW); }
"units"                                                         { count(); BEGIN(str); return(UNITS_KW); }
"uses"                                                          { count(); BEGIN(qname); return(USES_KW); }
"value"                                                         { count(); BEGIN(integer); return(VALUE_KW); }
"when"                                                          { count(); BEGIN(str); return(WHEN_KW); }
"yang-version"                                                  { count(); BEGIN(str); return(YANG_VERSION_KW); }
"yin-element"                                                   { count(); return(YIN_ELEMENT_KW); }
<status>"current"                                               { count(); return(CURRENT_KW); }
<status>"obsolete"                                              { count(); return(OBSOLETE_KW); }
<status>"deprecated"                                            { count(); return(DEPRECATED_KW); }
"false"                                                         { count(); return(FALSE_KW); }
"invert-match"                                                  { count(); return(INVERT_MATCH_KW); }
<range,length>"max"                                             { count(); return(MAX_KW); }
<range,length>"min"                                             { count(); return(MIN_KW); }
"true"                                                          { count(); return(TRUE_KW); }
"system"                                                        { count(); return(SYSTEM_KW); }
"user"                                                          { count(); return(USER_KW); }
<range,length>".."                                              { count(); return(DOTDOT); }
<*>"{"     		                                        { count(); BEGIN(INITIAL); return('{'); }
"}"     		                                        { count(); BEGIN(INITIAL); return('}'); }
<*>";"			                                        { count(); BEGIN(INITIAL); return(';'); }
<str>"+"                                                        { count(); return('+'); }                                                      
<qname,schema_path,unknown>":"			                { count(); return(':'); }
<range>"|"			                                            { count(); return('|'); }
<schema_path>"/"			                                                        { count(); return('/'); }
<date>{DIGIT}{4}"-"{DIGIT}{2}"-"{DIGIT}{2}                                          { count(); yylval.sval = new std::string(yytext); return(DATE); }
<integer,range,length>"-"{DIGIT}*|{DIGIT}*                                          {
                                                                                         count(); 
                                                                                         char* tail = 0; // not realy used
                                                                                         yylval.ival = strtoll(yytext, &tail, 0);
					                                                                     if (errno == ERANGE) {
					                                                                     // @todo error handling
					                                                                     } else {
					                                                                         return(INTEGER);
					                                                                     }
                                                                                      }
<range>"-"{DIGIT}+"."{DIGIT}+|{DIGIT}+"."{DIGIT}+                                     {
                                                                                          count();
                                                                                          char* tail = 0;	
                                                                                          yylval.dval = strtod(yytext, &tail);
                                                                                          if (errno == ERANGE) {
				                                                                          // @todo error handling
                                                                                          } else {
                                                                                              return(DECIMAL);
                                                                                          }
                                                                                      }
({L}|"_")({L}|{DIGIT}|[-_.])*                                                         {
                                                                                           /* A Yang Statment begins either with a keyword or a qname 
                                                                                            * which represents an extension statment.
                                                                                            * In the INITIAL start condition this is what is expected.
                                                                                            */
                                                                                           count(); 
                                                                                           BEGIN(unknown); 
                                                                                           yylval.sval = new std::string(yytext); 
                                                                                           return(IDENTIFIER); 
                                                                                      }
<unknown>({L}|"_")({L}|{DIGIT}|[-_.])*                                                { 
                                                                                           count(); 
                                                                                           BEGIN(str); 
                                                                                           yylval.sval = new std::string(yytext); 
                                                                                           return(IDENTIFIER); 
                                                                                      }
<identifier,qname,schema_path>({L}|"_")({L}|{DIGIT}|[-_.])*                           { count(); yylval.sval = new std::string(yytext); return(IDENTIFIER); }
<str>"\""                                                                             { count(); BEGIN(double_quoted_string); }
<double_quoted_string>([^"])*                                                         { /* @todo escaped chars \" \n \t \\ */
                                                                                           count();
                                                                                           yylval.sval = new std::string(yytext); 
                                                                                           return QUOTED_STRING_PART; 
                                                                                      }
<double_quoted_string>"\""                                                            { count(); BEGIN(str); } 
<str>"'"                                                                              { count(); BEGIN(single_quoted_string); }
<single_quoted_string>([^'])*                                                         {
                                                                                           count();
                                                                                           yylval.sval = new std::string(yytext); 
                                                                                           return QUOTED_STRING_PART; 
                                                                                      }
<single_quoted_string>"'"                                                             { count(); BEGIN(str); } 
<str>([^\x09\x0A\x0B\x0D\x20;{'\""])([^\x09\x0A\x0B\x0D\x20;{"])*                     { 
                                                                                           count();
                                                                                           yylval.sval = new std::string(yytext); 
                                                                                           return UNQUOTED_STRING; 
                                                                                      }
[\x09\x0A\x0B\x0D\x20]		                                                      { count(); /* skip whitespaces in intital start condition */}
<identifier,str,qname,schema_path,date,range,length,integer,status>[\x09\x0A\x0B\x0D\x20]    { 
                                                                                           count(); 
                                                                                           /* Skip whitspeces in all other start conditions except quoted_string and comment.
                                                                                            * @todo This is not the correct behaviour for schema_path and qname.
                                                                                            */
                                                                                      }
<identifier,qname,schema_path,date,range,length,integer>"\""                          { count(); /* these are *-arg-str in grammar so need to skip quotes */}
"/*"                                                                                  { BEGIN(comment); }
<comment>[^*\n]*                                                                      { count(); /* eat anything that's not a '*' */ }
<comment>"*"+[^*/\n]*                                                                 { count(); /* eat up '*'s not followed by '/'s */ }
<comment>\n                                                                           { count(); }
<comment>"*"+"/"                                                                      { BEGIN(INITIAL); }
<*>.			                                                              { count(); fprintf(stderr, "FIXME: ignoring [%x]\n", yytext[0]); yyerror("Unhandled char."); }

%%

int column = 0;

void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;
    /* ECHO; */
}

