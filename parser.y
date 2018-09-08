%{
#include <cstdio>
#include <cstring>
#include <limits>
#include <list>
#include "Common.hh"
#include "YangAst.hh"
int yylex();
extern int yylineno;
extern char *yytext;
int yyerror(const char *m) { printf("line %d: %s : %s\n", yylineno, yytext, m); }
#define YYDEBUG 1
#define YYERROR_VERBOSE 1

Yang::Ast::ModuleBases* moduleBases;
%}

%verbose

%token <sval> UNQUOTED_STRING
%token <sval> QUOTED_STRING_PART
%token <sval> IDENTIFIER
%token <ival> INTEGER
%token <dval> DECIMAL
%token <sval> DATE
%token DOTDOT

%token ACTION_KW ANYDATA_KW ANYXML_KW ARGUMENT_KW AUGMENT_KW BASE_KW BELONGS_TO_KW BIT_KW CASE_KW CHOICE_KW CONFIG_KW
%token CONTACT_KW CONTAINER_KW DEFAULT_KW DESCRIPTION_KW ENUM_KW ERROR_APP_TAG_KW ERROR_MESSAGE_KW
%token EXTENSION_KW DEVIATION_KW DEVIATE_KW FEATURE_KW FRACTION_DIGITS_KW GROUPING_KW IDENTITY_KW 
%token IF_FEATURE_KW IMPORT_KW INCLUDE_KW INPUT_KW KEY_KW LEAF_KW LEAF_LIST_KW LENGTH_KW LIST_KW
%token MANDATORY_KW MAX_ELEMENTS_KW MIN_ELEMENTS_KW MODIFIER_KW MODULE_KW MUST_KW NAMESPACE_KW NOTIFICATION_KW
%token ORDERED_BY_KW ORGANIZATION_KW OUTPUT_KW PATH_KW PATTERN_KW POSITION_KW PREFIX_KW PRESENCE_KW
%token RANGE_KW REFERENCE_KW REFINE_KW REQUIRE_INSTANCE_KW REVISION_KW REVISION_DATE_KW RPC_KW
%token STATUS_KW SUBMODULE_KW TYPE_KW TYPEDEF_KW UNIQUE_KW UNITS_KW USES_KW VALUE_KW WHEN_KW
%token YANG_VERSION_KW YIN_ELEMENT_KW CURRENT_KW OBSOLETE_KW DEPRECATED_KW FALSE_KW INVERT_MATCH_KW 
%token MAX_KW MIN_KW TRUE_KW SYSTEM_KW USER_KW

%type <moduleBases> top_stmts
%type <moduleBase> top_stmt
%type <module> module_stmt
%type <lval> module_body_stmts
%type <nval> module_body_stmt
%type <subModule> submodule_stmt
%type <lval> submodule_body_stmts
%type <nval> submodule_body_stmt
%type <nval> yang_version_stmt
%type <nval> namespace_stmt
%type <prefix> prefix_stmt
%type <belongsTo> belongs_to_stmt
%type <nval> organization_stmt
%type <nval> contact_stmt
%type <nval> description_stmt
%type <nval> reference_stmt
%type <nval> units_stmt
%type <revision> revision_stmt
%type <lval> revision_body
%type <nval> revision_body_stmt
%type <extension> extension_stmt
%type <lval> extension_stmt_body
%type <nval> extension_stmt_body_stmt
%type <nval> argument_stmt
%type <tval> type_stmt
%type <lval> type_body_stmts
%type <nval> type_body_stmt
%type <fractionDigits> fraction_digits_stmt
%type <length> length_stmt
%type <lval> length_body_stmts
%type <nval> length_body_stmt
%type <pattern> pattern_stmt
%type <lval> pattern_stmt_body
%type <nval> pattern_stmt_body_stmt
%type <invertMatch> modifier_stmt
%type <ranges> range_stmt
%type <nval> status_stmt
%type <config> config_stmt
%type <mandatory> mandatory_stmt
%type <presence> presence_stmt
%type <userOrdered> ordered_by_stmt
%type <must> must_stmt
%type <lval> must_stmt_body
%type <nval> must_stmt_body_stmt
%type <sval> status_arg
%type <errorMessage> error_message_stmt
%type <errorAppTag> error_app_tag_stmt
%type <minElements> min_elements_stmt
%type <maxElements> max_elements_stmt
%type <value> value_stmt
%type <grouping> grouping_stmt
%type <lval> grouping_stmt_body
%type <nval> grouping_stmt_body_stmt
%type <nval> feature_stmt
%type <lval> feature_stmt_body
%type <nval> feature_stmt_body_stmt
%type <nval> if_feature_stmt
%type <nval> typedef_stmt
%type <lval> typedef_stmt_body
%type <nval> typedef_stmt_body_stmt
%type <sval> revision_date_stmt
%type <bval> true_false_arg
%type <nval> identity_stmt
%type <lval> identity_stmt_body
%type <nval> identity_stmt_body_stmt
%type <qval> identifier_ref
%type <nval> default_stmt
%type <enumSpec> enum_specification
%type <lval> enum_specification_body
%type <nval> enum_specification_body_stmt
%type <nval> path_stmt
%type <nval> require_instance_stmt
%type <nval> bit_stmt
%type <lval> bit_body
%type <nval> bit_body_stmt
%type <nval> position_stmt
%type <nval> base_stmt
%type <container> container_stmt
%type <lval> container_stmt_body
%type <nval> container_stmt_body_stmt
%type <leaf> leaf_stmt
%type <lval> leaf_stmt_body
%type <nval> leaf_stmt_body_stmt
%type <leafList> leaf_list_stmt
%type <lval> leaf_list_stmt_body
%type <nval> leaf_list_stmt_body_stmt
%type <list> list_stmt
%type <lval> list_stmt_body
%type <nval> list_stmt_body_stmt
%type <nval> import_stmt
%type <nval> include_stmt
%type <nval> key_stmt
%type <unique> unique_stmt
%type <uniqueArg> unique_arg
%type <qlval> key_arg
%type <choice> choice_stmt
%type <lval> choice_body
%type <nval> choice_body_stmt
%type <cas> case_stmt
%type <lval> case_stmt_body
%type <nval> case_stmt_body_stmt
%type <anyData> anydata_stmt
%type <lval> anydata_anyxml_stmt_body
%type <nval> anydata_anyxml_stmt_body_stmt
%type <anyXml> anyxml_stmt
%type <uses> uses_stmt
%type <lval> uses_stmt_body
%type <nval> uses_stmt_body_stmt
%type <augment> augment_stmt
%type <lval> augment_body
%type <nval> augment_body_stmt
%type <when> when_stmt
%type <lval> when_stmt_body
%type <nval> when_stmt_body_stmt
%type <rpc> rpc_stmt
%type <action> action_stmt
%type <lval> rpc_action_stmt_body
%type <nval> rpc_action_stmt_body_stmt
%type <input> input_stmt
%type <lval> input_output_stmt_body
%type <nval> input_output_stmt_body_stmt
%type <output> output_stmt
%type <notification> notification_stmt
%type <lval> notification_stmt_body
%type <nval> notification_stmt_body_stmt
%type <unknown> unknown_stmt
%type <lval> range_arg
%type <lval> range_parts
%type <nval> range_part
%type <dval> range_boundary
%type <lval> length_arg
%type <lval> length_parts
%type <nval> length_part
%type <ival> length_boundary
%type <absoluteSchemaNodeId> absolute_schema_nodeid
%type <descendantSchemaNodeId> descendant_schema_nodeid
%type <qval> node_identifier
%type <sval> string
%type <sval> quoted_string

%start top_stmts

%union {
    bool bval;
    std::string* sval;;
    int64_t ival;
    double dval;
    Yang::Ast::Node* nval;
    Yang::Ast::ModuleBase* moduleBase;
    Yang::Ast::ModuleBases* moduleBases;
    Yang::Ast::Module* module;
    Yang::Ast::SubModule* subModule;
    Yang::Ast::YangVersion* yangVersion;
    Yang::Ast::ModuleHeader* moduleHeader;
    Yang::Ast::Prefix* prefix;
    Yang::Ast::BelongsTo* belongsTo;
    Yang::Ast::Revision* revision;
    Yang::Ast::Extension* extension;
    Ydb::Common::Qname* qval;
    Yang::Ast::FractionDigits* fractionDigits;
    Yang::Ast::Ranges* ranges;
    Yang::Ast::Length* length;
    Yang::Ast::Pattern* pattern;
    Yang::Ast::InvertMatch* invertMatch;
    Yang::Ast::Config* config;
    Yang::Ast::Mandatory* mandatory;
    Yang::Ast::Presence* presence;
    Yang::Ast::UserOrdered* userOrdered;
    Yang::Ast::Must* must;
    Yang::Ast::ErrorMessage* errorMessage;
    Yang::Ast::ErrorAppTag* errorAppTag;
    Yang::Ast::MinElements* minElements;
    Yang::Ast::MaxElements* maxElements;
    Yang::Ast::Value* value;
    Yang::Ast::Grouping* grouping;
    Yang::Ast::EnumSpec* enumSpec;
    Yang::Ast::Container* container;
    Yang::Ast::Leaf* leaf;
    Yang::Ast::LeafList* leafList;
    Yang::Ast::List* list;
    Yang::Ast::Unique* unique;
    std::list<Yang::Ast::DescendantSchemaNodeId*>* uniqueArg;
    Yang::Ast::Choice* choice;
    Yang::Ast::Augment* augment;
    Yang::Ast::When* when;
    Yang::Ast::Rpc* rpc;
    Yang::Ast::Action* action;
    Yang::Ast::Input* input;
    Yang::Ast::Output* output;
    Yang::Ast::Notification* notification;
    Yang::Ast::Unknown* unknown;
    Yang::Ast::Case* cas;
    Yang::Ast::AnyData* anyData;
    Yang::Ast::AnyXml* anyXml;
    Yang::Ast::Uses* uses;
    Yang::Ast::Type* tval;
    Yang::Ast::AbsoluteSchemaNodeId* absoluteSchemaNodeId;
    Yang::Ast::DescendantSchemaNodeId* descendantSchemaNodeId;
    Yang::Ast::Nodes* lval;
    Ydb::Common::Qnames* qlval;
}

%%

top_stmts
: top_stmt                                                           { $$ = moduleBases = new Yang::Ast::ModuleBases(1, $1); }
| top_stmts top_stmt                                                 { $1->push_back($2); $$ = $1; }
;

top_stmt
: module_stmt                                                        { $$ = $1; }
| submodule_stmt                                                     { $$ = $1; }
;

/////////////////////////////////////////////////////////////////////////////////////////////
// draft-ietf-netmod-rfc6020bis-08
/*    module-stmt         = optsep module-keyword sep identifier-arg-str */
/*                          optsep */
/*                          "{" stmtsep */
/*                              module-header-stmts */
/*                              linkage-stmts */
/*                              meta-stmts */
/*                              revision-stmts */
/*                              body-stmts */
/*                          "}" optsep */
module_stmt
: MODULE_KW IDENTIFIER '{' module_body_stmts '}'                     { $$ = new Yang::Ast::Module($2, $4); }
;

module_body_stmts
: module_body_stmt                                                   { $$ = new Yang::Ast::Nodes(1, $1); }
| module_body_stmts module_body_stmt                                 { $1->push_back($2); $$ = $1; }
;

module_body_stmt
: yang_version_stmt                                                  { $$ = $1; } /* module-header-stmts */
| namespace_stmt                                                     { $$ = $1; } 
| prefix_stmt                                                        { $$ = $1; } 
| import_stmt                                                        { $$ = $1; } /* linkage-stmts*/
| include_stmt                                                       { $$ = $1; }
| organization_stmt                                                  { $$ = $1; } /* meta-stmts*/
| contact_stmt                                                       { $$ = $1; }
| description_stmt                                                   { $$ = $1; } 
| reference_stmt                                                     { $$ = $1; }
| revision_stmt                                                      { $$ = $1; } /* revision-stmts */
| extension_stmt                                                     { $$ = $1; } /* body-stmts */
| feature_stmt                                                       { $$ = $1; }
| identity_stmt                                                      { $$ = $1; }
| typedef_stmt                                                       { $$ = $1; }
| grouping_stmt                                                      { $$ = $1; }
| container_stmt                                                     { $$ = $1; } /*  data-def-stmt */
| leaf_stmt                                                          { $$ = $1; }
| leaf_list_stmt                                                     { $$ = $1; }
| list_stmt                                                          { $$ = $1; }
| choice_stmt                                                        { $$ = $1; }
| anydata_stmt                                                       { $$ = $1; }
| anyxml_stmt                                                        { $$ = $1; }
| uses_stmt                                                          { $$ = $1; }
| augment_stmt                                                       { $$ = $1; }
| rpc_stmt                                                           { $$ = $1; }    
| notification_stmt                                                  { $$ = $1; } 
/*                            deviation-stmt) */
| unknown_stmt                                                       { $$ = $1; }
;

/*    submodule-stmt      = optsep submodule-keyword sep identifier-arg-str */
/*                          optsep */
/*                          "{" stmtsep */
/*                              submodule-header-stmts */
/*                              linkage-stmts */
/*                              meta-stmts */
/*                              revision-stmts */
/*                              body-stmts */
/*                          "}" optsep */
submodule_stmt
: SUBMODULE_KW IDENTIFIER '{' submodule_body_stmts '}'               { $$ = new Yang::Ast::SubModule($2, $4); }
;

submodule_body_stmts
: submodule_body_stmt                                                { $$ = new Yang::Ast::Nodes(1, $1); }
| submodule_body_stmts submodule_body_stmt                           { $1->push_back($2); $$ = $1; }
;

submodule_body_stmt
: yang_version_stmt                                                  { $$ = $1; } /* submodule-header-stmts */
| belongs_to_stmt                                                    { $$ = $1; } 
| import_stmt                                                        { $$ = $1; } /* linkage-stmts*/
| include_stmt                                                       { $$ = $1; }
| organization_stmt                                                  { $$ = $1; } /* meta-stmts*/
| contact_stmt                                                       { $$ = $1; }
| description_stmt                                                   { $$ = $1; } 
| reference_stmt                                                     { $$ = $1; }
| revision_stmt                                                      { $$ = $1; } /* revision-stmts */
| extension_stmt                                                     { $$ = $1; } /* body-stmts */
| feature_stmt                                                       { $$ = $1; }
| identity_stmt                                                      { $$ = $1; }
| typedef_stmt                                                       { $$ = $1; }
| grouping_stmt                                                      { $$ = $1; }
| container_stmt                                                     { $$ = $1; } /*  data-def-stmt */
| leaf_stmt                                                          { $$ = $1; }
| leaf_list_stmt                                                     { $$ = $1; }
| list_stmt                                                          { $$ = $1; }
| choice_stmt                                                        { $$ = $1; }
| anydata_stmt                                                       { $$ = $1; }
| anyxml_stmt                                                        { $$ = $1; }
| uses_stmt                                                          { $$ = $1; }
| augment_stmt                                                       { $$ = $1; }
| rpc_stmt                                                           { $$ = $1; }                    
| notification_stmt                                                  { $$ = $1; }
/*                            deviation-stmt) */
| unknown_stmt                                                       { $$ = $1; } 
;

/*    module-header-stmts = ;; these stmts can appear in any order */
/*                          yang-version-stmt */
/*                          namespace-stmt */
/*                          prefix-stmt */

/*    submodule-header-stmts = */
/*                          ;; these stmts can appear in any order */
/*                          yang-version-stmt */
/*                          belongs-to-stmt */

/*    meta-stmts          = ;; these stmts can appear in any order */
/*                          [organization-stmt] */
/*                          [contact-stmt] */
/*                          [description-stmt] */
/*                          [reference-stmt] */

/*    linkage-stmts       = ;; these stmts can appear in any order */
/*                          *import-stmt */
/*                          *include-stmt */

/*    revision-stmts      = *revision-stmt */

/*    body-stmts          = *(extension-stmt / */
/*                            feature-stmt / */
/*                            identity-stmt / */
/*                            typedef-stmt / */
/*                            grouping-stmt / */
/*                            data-def-stmt / */
/*                            augment-stmt / */
/*                            rpc-stmt / */
/*                            notification-stmt / */
/*                            deviation-stmt) */

/*    data-def-stmt       = container-stmt / */
/*                          leaf-stmt / */
/*                          leaf-list-stmt / */
/*                          list-stmt / */
/*                          choice-stmt / */
/*                          anydata-stmt / */
/*                          anyxml-stmt / */
/*                          uses-stmt */

/*    yang-version-stmt   = yang-version-keyword sep yang-version-arg-str stmtend */
yang_version_stmt
: YANG_VERSION_KW string ';'                                 { $$ = new Yang::Ast::YangVersion($2); }
;

/*    yang-version-arg-str = < a string that matches the rule > < yang-version-arg > */
/*    yang-version-arg    = "1.1" */

/*    import-stmt         = import-keyword sep identifier-arg-str optsep */
/*                          "{" stmtsep */
/*                              prefix-stmt */
/*                              [revision-date-stmt] */
/*                          "}" stmtsep */
import_stmt
: IMPORT_KW IDENTIFIER '{' prefix_stmt '}'                           { $$ = new Yang::Ast::Import($2, $4); }
| IMPORT_KW IDENTIFIER '{' prefix_stmt revision_date_stmt '}'        { $$ = new Yang::Ast::Import($2, $4, $5); }
;

/*    include-stmt        = include-keyword sep identifier-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               [revision-date-stmt] */
/*                           "}") stmtsep */
include_stmt
: INCLUDE_KW IDENTIFIER ';'                                           { $$ = new Yang::Ast::Include($2); }
| INCLUDE_KW IDENTIFIER '{' '}'                                       { $$ = new Yang::Ast::Include($2); }
| INCLUDE_KW IDENTIFIER '{' revision_date_stmt '}'                    { $$ = new Yang::Ast::Include($2, $4); }
;

/*    namespace-stmt      = namespace-keyword sep uri-str stmtend */
namespace_stmt
: NAMESPACE_KW string ';'                                      {  $$ = new Yang::Ast::NameSpace($2); }
;

/*    uri-str             = < a string that matches the rule > < URI in RFC 3986 > */

/*    prefix-stmt         = prefix-keyword sep prefix-arg-str stmtend */
prefix_stmt
: PREFIX_KW IDENTIFIER ';'                                            { $$ = new Yang::Ast::Prefix($2); }
;

/*    belongs-to-stmt     = belongs-to-keyword sep identifier-arg-str optsep */
/*                          "{" stmtsep */
/*                              prefix-stmt */
/*                          "}" stmtsep */
belongs_to_stmt
: BELONGS_TO_KW IDENTIFIER '{' prefix_stmt '}'                        { $$ = new Yang::Ast::BelongsTo($2, $4); }
;

/*    organization-stmt   = organization-keyword sep string stmtend */
organization_stmt
: ORGANIZATION_KW string ';'                                    { $$ = new Yang::Ast::Organization($2); }
;

/*    contact-stmt        = contact-keyword sep string stmtend */
contact_stmt
: CONTACT_KW string ';'                                         { $$ = new Yang::Ast::Contact($2); }
;

/*    description-stmt    = description-keyword sep string stmtend */
description_stmt
: DESCRIPTION_KW string ';'                                     { $$ = new Yang::Ast::Description($2); }
;

/*    reference-stmt      = reference-keyword sep string stmtend */
reference_stmt
: REFERENCE_KW string ';'                                       { $$ = new Yang::Ast::Reference($2); }
;

/*    units-stmt          = units-keyword sep string stmtend */
units_stmt
: UNITS_KW string ';'                                           { $$ = new Yang::Ast::Units($2); }

/*    revision-stmt       = revision-keyword sep revision-date optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                           "}") stmtsep */
revision_stmt
: REVISION_KW DATE ';'                                             { $$ = new Yang::Ast::Revision($2); }
| REVISION_KW DATE '{' revision_body '}'                           { $$ = new Yang::Ast::Revision($2, $4); }
;

revision_body
: revision_body_stmt                                                { $$ = new Yang::Ast::Nodes(1, $1); }
| revision_body revision_body_stmt                                  { $1->push_back($2); $$ = $1; }
;

revision_body_stmt
: description_stmt                                                   { $$ = $1; }
| reference_stmt                                                     { $$ = $1; }
| unknown_stmt                                                       { $$ = $1; } 
;

/*    revision-date       = date-arg-str */
/*    revision-date-stmt  = revision-date-keyword sep revision-date stmtend */
revision_date_stmt
: REVISION_DATE_KW DATE ';'                                         { $$ = $2; }
;

/*    extension-stmt      = extension-keyword sep identifier-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [argument-stmt] */
/*                               [status-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                           "}") stmtsep */
extension_stmt
: EXTENSION_KW IDENTIFIER ';'                                       { $$ = new Yang::Ast::Extension($2); }
| EXTENSION_KW IDENTIFIER '{' extension_stmt_body '}'               { $$ = new Yang::Ast::Extension($2, $4); }       
;

extension_stmt_body
: extension_stmt_body_stmt                                          { $$ = new Yang::Ast::Nodes(1, $1); }
| extension_stmt_body extension_stmt_body_stmt                      { $1->push_back($2); $$ = $1; }
;

extension_stmt_body_stmt
: argument_stmt                                                     { $$ = $1; }
| status_stmt                                                       { $$ = $1; }
| description_stmt                                                  { $$ = $1; }
| reference_stmt                                                    { $$ = $1; }
| unknown_stmt                                                      { $$ = $1; } 
;

/*    argument-stmt       = argument-keyword sep identifier-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               [yin-element-stmt] */
/*                           "}") stmtsep */
argument_stmt
: ARGUMENT_KW IDENTIFIER ';'                                        { $$ = new Yang::Ast::Argument($2); }
;

/*    yin-element-stmt    = yin-element-keyword sep yin-element-arg-str */
/*                          stmtend */

/*    yin-element-arg-str = < a string that matches the rule > < yin-element-arg > */

/*    yin-element-arg     = true-keyword / false-keyword */
true_false_arg
: TRUE_KW                                                         { $$ = true; }
| FALSE_KW                                                        { $$ = false; }
;

/*    identity-stmt       = identity-keyword sep identifier-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               *if-feature-stmt */
/*                               *base-stmt */
/*                               [status-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                           "}") stmtsep */
identity_stmt
: IDENTITY_KW IDENTIFIER ';'                                       { $$ = new Yang::Ast::Identity($2); }
| IDENTITY_KW IDENTIFIER '{' '}'                                   { $$ = new Yang::Ast::Identity($2); }
| IDENTITY_KW IDENTIFIER '{' identity_stmt_body '}'                { $$ = new Yang::Ast::Identity($2, $4); }
;

identity_stmt_body
: identity_stmt_body_stmt                                          { $$ = new Yang::Ast::Nodes(1, $1); }
| identity_stmt_body identity_stmt_body_stmt                       { $1->push_back($2); $$ = $1; }
;

identity_stmt_body_stmt
: base_stmt                                                        { $$ = $1; }
| status_stmt                                                      { $$ = $1; }
| description_stmt                                                 { $$ = $1; }
| reference_stmt                                                   { $$ = $1; }
| unknown_stmt                                                       { $$ = $1; } 
;

/*    base-stmt           = base-keyword sep identifier-ref-arg-str stmtend */
base_stmt
: BASE_KW identifier_ref ';'                                          { $$ = new Yang::Ast::Base($2); }
;

/*    feature-stmt        = feature-keyword sep identifier-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               *if-feature-stmt */
/*                               [status-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                           "}") stmtsep */
feature_stmt
: FEATURE_KW IDENTIFIER ';'                                         { $$ = new Yang::Ast::Feature($2); }
| FEATURE_KW IDENTIFIER '{' '}'                                     { $$ = new Yang::Ast::Feature($2); }
| FEATURE_KW IDENTIFIER '{' feature_stmt_body '}'                   { $$ = new Yang::Ast::Feature($2, $4); }
;

feature_stmt_body
: feature_stmt_body_stmt                                            { $$ = new Yang::Ast::Nodes(1, $1); }
| feature_stmt_body feature_stmt_body_stmt                          { $1->push_back($2); $$ = $1; }
;
 
feature_stmt_body_stmt
: if_feature_stmt                                                   { $$ = $1; }
| status_stmt                                                       { $$ = $1; }
| description_stmt                                                  { $$ = $1; }
| reference_stmt                                                    { $$ = $1; }
| unknown_stmt                                                       { $$ = $1; } 
;

/*    if-feature-stmt     = if-feature-keyword sep if-feature-expr-str stmtend */
if_feature_stmt
: IF_FEATURE_KW identifier_ref ';'                                  { $$ = new Yang::Ast::IfFeature($2); } /* @todo implement complete grammar */
;

/*    if-feature-expr-str = < a string that matches the rule > < if-feature-expr > */
/*    if-feature-expr     = "(" if-feature-expr ")" / if-feature-expr sep boolean-operator sep if-feature-expr / not-keyword sep if-feature-expr / identifier-ref-arg */

/*    boolean-operator = and-keyword / or-keyword */

/*    typedef-stmt        = typedef-keyword sep identifier-arg-str optsep */
/*                          "{" stmtsep */
/*                              ;; these stmts can appear in any order */
/*                              type-stmt */
/*                              [units-stmt] */
/*                              [default-stmt] */
/*                              [status-stmt] */
/*                              [description-stmt] */
/*                              [reference-stmt] */
/*                           "}" stmtsep */
typedef_stmt
: TYPEDEF_KW IDENTIFIER '{' typedef_stmt_body '}'                    { $$ = new Yang::Ast::Typedef($2, $4); }
;

typedef_stmt_body
: typedef_stmt_body_stmt                                             { $$ = new Yang::Ast::Nodes(1, $1); }
| typedef_stmt_body typedef_stmt_body_stmt                           { $1->push_back($2); $$ = $1; }
;

typedef_stmt_body_stmt
: type_stmt                                                          { $$ = $1; }
| units_stmt                                                         { $$ = $1; }
| default_stmt                                                       { $$ = $1; }
| status_stmt                                                        { $$ = $1; }
| description_stmt                                                   { $$ = $1; }
| reference_stmt                                                     { $$ = $1; }
| unknown_stmt                                                       { $$ = $1; } 
;

/*    type-stmt           = type-keyword sep identifier-ref-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               [type-body-stmts] */
/*                           "}") stmtsep */
type_stmt
: TYPE_KW identifier_ref ';'                                          { $$ = new Yang::Ast::Type($2); }
| TYPE_KW identifier_ref '{' '}'                                      { $$ = new Yang::Ast::Type($2); }
| TYPE_KW identifier_ref '{' type_body_stmts '}'                      { $$ = new Yang::Ast::Type($2, $4); }
;

/*    type-body-stmts     = numerical-restrictions / */
/*                          decimal64-specification / */
/*                          string-restrictions / */
/*                          enum-specification / */
/*                          leafref-specification / */
/*                          identityref-specification / */
/*                          instance-identifier-specification / */
/*                          bits-specification / */
/*                          union-specification / */
/*                          binary-specification */
type_body_stmts
: type_body_stmt                                                      { $$ = new Yang::Ast::Nodes(1, $1); } 
| type_body_stmts type_body_stmt                                      { $1->push_back($2); $$ = $1; }
;

type_body_stmt
: range_stmt                                                          { $$ = $1; } /* numerical_restrictions, decimal64_specification */
| fraction_digits_stmt                                                { $$ = $1; } /* decimal64_specification */
| length_stmt                                                         { $$ = $1; } /* string_restrictions, binary-specification */
| pattern_stmt                                                        { $$ = $1; } /* string_restrictions */
| enum_specification                                                  { $$ = $1; }
| path_stmt                                                           { $$ = $1; } /* leafref_specification */
| require_instance_stmt                                               { $$ = $1; } /* leafref_specification, instance-identifier-specification */
| base_stmt                                                           { $$ = $1; } /* identityref-specification */
| bit_stmt                                                            { $$ = $1; } /* bits-specification */
| type_stmt                                                           { $$ = $1; } /* union-specification */    
| unknown_stmt                                                        { $$ = $1; } 
;

/*    numerical-restrictions = range-stmt */

/*    range-stmt          = range-keyword sep range-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [error-message-stmt] */
/*                               [error-app-tag-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                            "}") stmtsep */
range_stmt
: RANGE_KW range_arg  ';'                                               { $$ = new Yang::Ast::Ranges($2); }
;                          

/*    decimal64-specification = ;; these stmts can appear in any order */
/*                              fraction-digits-stmt */
/*                              [range-stmt] */

/*    fraction-digits-stmt = fraction-digits-keyword sep fraction-digits-arg-str stmtend */
fraction_digits_stmt
: FRACTION_DIGITS_KW INTEGER ';'                                       { 
                                                                             int64_t i = $2; 
                                                                             if (i < 1 && i > 18) {
                                                                                 YYERROR; 
                                                                             } else {
                                                                                 $$ = new Yang::Ast::FractionDigits(i); 
                                                                             }
                                                                        } 
;
/*    fraction-digits-arg-str = < a string that matches the rule > < fraction-digits-arg > */
/*    fraction-digits-arg = ("1" ["0" / "1" / "2" / "3" / "4" / "5" / "6" / "7" / "8"]) / "2" / "3" / "4" / "5" / "6" / "7" / "8" / "9" */

/*    string-restrictions = ;; these stmts can appear in any order */
/*                          [length-stmt] */
/*                          *pattern-stmt */

/*    length-stmt         = length-keyword sep length-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [error-message-stmt] */
/*                               [error-app-tag-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                            "}") stmtsep */
length_stmt
: LENGTH_KW length_arg ';'                                            { $$ = new Yang::Ast::Length($2); }
| LENGTH_KW length_arg '{' length_body_stmts '}'                      { $$ = new Yang::Ast::Length($2, $4); }
;

length_body_stmts
: length_body_stmt                                                    { $$ = new Yang::Ast::Nodes(1, $1); }
| length_body_stmts length_body_stmt                                  { $1->push_back($2); $$ = $1; }
;

length_body_stmt
: error_message_stmt                                                  { $$ = $1; }
| error_app_tag_stmt                                                  { $$ = $1; }
| description_stmt                                                    { $$ = $1; }
| reference_stmt                                                      { $$ = $1; }
| unknown_stmt                                                       { $$ = $1; } 
;

/*    pattern-stmt        = pattern-keyword sep string optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [modifier-stmt] */
/*                               [error-message-stmt] */
/*                               [error-app-tag-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                            "}") stmtsep */
pattern_stmt
: PATTERN_KW string ';'                                        { $$ = new Yang::Ast::Pattern($2); }
| PATTERN_KW string '{' pattern_stmt_body '}' ';'              { $$ = new Yang::Ast::Pattern($2, $4); }
;

pattern_stmt_body
: pattern_stmt_body_stmt                                              { $$ = new Yang::Ast::Nodes(1, $1); }
| pattern_stmt_body pattern_stmt_body_stmt                            { $1->push_back($2); $$ = $1; }
;

pattern_stmt_body_stmt
: modifier_stmt                                                       { $$ = $1; }
| error_message_stmt                                                  { $$ = $1; }
| error_app_tag_stmt                                                  { $$ = $1; }
| description_stmt                                                    { $$ = $1; }
| reference_stmt                                                      { $$ = $1; }
| unknown_stmt                                                        { $$ = $1; } 
;

/*    modifier-stmt       = modifier-keyword sep modifier-arg-str stmtend */
/*    modifier-arg-str    = < a string that matches the rule > < modifier-arg > */
/*    modifier-arg        = invert-match-keyword */
modifier_stmt
: MODIFIER_KW INVERT_MATCH_KW                                         { $$ = new Yang::Ast::InvertMatch(); }
;
/*    default-stmt        = default-keyword sep string stmtend */
default_stmt
: DEFAULT_KW string ';'                                        { $$ = new Yang::Ast::Default($2); }
;

/*    enum-specification  = 1*enum-stmt */
/*    enum-stmt           = enum-keyword sep string optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               *if-feature-stmt */
/*                               [value-stmt] */
/*                               [status-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                            "}") stmtsep */
enum_specification
: ENUM_KW string ';'                               { $$ = new Yang::Ast::EnumSpec($2); }
| ENUM_KW string '{' enum_specification_body '}'   { $$ = new Yang::Ast::EnumSpec($2, $4); }
;

enum_specification_body
: enum_specification_body_stmt                            { $$ = new Yang::Ast::Nodes(1, $1); }
| enum_specification_body enum_specification_body_stmt    { $1->push_back($2); $$ = $1; }
;

enum_specification_body_stmt
: if_feature_stmt                                         { $$ = $1; }
| value_stmt                                              { $$ = $1; }
| status_stmt                                             { $$ = $1; }
| description_stmt                                        { $$ = $1; }
| reference_stmt                                          { $$ = $1; }
| unknown_stmt                                            { $$ = $1; } 
;

/*    leafref-specification = */
/*                          ;; these stmts can appear in any order */
/*                          path-stmt */
/*                          [require-instance-stmt] */

/*    path-stmt           = path-keyword sep path-arg-str stmtend */
path_stmt
: PATH_KW string                                               { $$ = new Yang::Ast::Path($2); }
;

/*    require-instance-stmt = require-instance-keyword sep require-instance-arg-str stmtend */
require_instance_stmt
: REQUIRE_INSTANCE_KW true_false_arg                            { $$ = new Yang::Ast::RequireInstance($2); }
;

/*    require-instance-arg-str = < a string that matches the rule > < require-instance-arg > */
/*    require-instance-arg = true-keyword / false-keyword */

/*    instance-identifier-specification = [require-instance-stmt] */

/*    identityref-specification = 1*base-stmt */

/*    union-specification = 1*type-stmt */

/*    binary-specification = [length-stmt] */

/*    bits-specification  = 1*bit-stmt */
/*    bit-stmt            = bit-keyword sep identifier-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               *if-feature-stmt */
/*                               [position-stmt] */
/*                               [status-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                           "}") stmtsep */
bit_stmt
: BIT_KW IDENTIFIER ';'                                               { $$ = new Yang::Ast::Bit($2); }
| BIT_KW IDENTIFIER '{' bit_body '}'                                  { $$ = new Yang::Ast::Bit($2, $4); }
;

bit_body
: bit_body_stmt                                                       { $$ = new Yang::Ast::Nodes(1, $1); }
| bit_body bit_body_stmt                                              { $1->push_back($2); $$ = $1; }
;

bit_body_stmt
: if_feature_stmt                                                     { $$ = $1; }
| position_stmt                                                       { $$ = $1; }
| status_stmt                                                         { $$ = $1; }
| description_stmt                                                    { $$ = $1; }
| reference_stmt                                                      { $$ = $1; }
| unknown_stmt                                                        { $$ = $1; } 
;

/*    position-stmt       = position-keyword sep position-value-arg-str stmtend */
position_stmt
: POSITION_KW INTEGER ';'                                              { 
                                                                          int64_t i = $2; 
                                                                          if (i < 1) { /* @todo unsigned int from lexer */
                                                                                 YYERROR; 
                                                                             } else {
                                                                                 $$ = new Yang::Ast::Position(i); 
                                                                             }
                                                                       }
;
/*    position-value-arg-str = < a string that matches the rule > < position-value-arg > */
/*    position-value-arg  = non-negative-integer-value */

/*    status-stmt         = status-keyword sep status-arg-str stmtend */
status_stmt
: STATUS_KW status_arg ';'                                           { $$ = new Yang::Ast::Status($2); }
;

/*    status-arg-str      = < a string that matches the rule > */
/*                          < status-arg > */

/*    status-arg          = current-keyword / */
/*                          obsolete-keyword / */
/*                          deprecated-keyword */
status_arg
: CURRENT_KW                                                         { $$ = new std::string("current"); } /* @todo these should be enums */
| OBSOLETE_KW                                                        { $$ = new std::string("obsolete"); }
| DEPRECATED_KW                                                      { $$ = new std::string("deprecated"); }
;

/*    config-stmt         = config-keyword sep config-arg-str stmtend */
config_stmt
: CONFIG_KW true_false_arg ';'                                           { $$ = new Yang::Ast::Config($2); }

/*    config-arg-str      = < a string that matches the rule > < config-arg > */
/*    config-arg          = true-keyword / false-keyword */

/*    mandatory-stmt      = mandatory-keyword sep mandatory-arg-str stmtend */
mandatory_stmt
: MANDATORY_KW true_false_arg ';'                                         { $$ = new Yang::Ast::Mandatory($2); }

/*    mandatory-arg-str   = < a string that matches the rule > < mandatory-arg > */
/*    mandatory-arg       = true-keyword / false-keyword */

/*    presence-stmt       = presence-keyword sep string stmtend */
presence_stmt
: PRESENCE_KW string ';'                                           { $$ = new Yang::Ast::Presence($2); }

/*    ordered-by-stmt     = ordered-by-keyword sep ordered-by-arg-str stmtend */
/*    ordered-by-arg-str  = < a string that matches the rule > < ordered-by-arg > */
/*    ordered-by-arg      = user-keyword / system-keyword */
ordered_by_stmt
: ORDERED_BY_KW USER_KW ';'                                                { $$ = new Yang::Ast::UserOrdered(true); }
| ORDERED_BY_KW SYSTEM_KW ';'                                             { $$ = new Yang::Ast::UserOrdered(false); }
;

/*    must-stmt           = must-keyword sep string optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [error-message-stmt] */
/*                               [error-app-tag-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                            "}") stmtsep */
must_stmt
: MUST_KW string ';'                                         { $$ = new Yang::Ast::Must($2); }
| MUST_KW string '{' must_stmt_body '}'                      { $$ = new Yang::Ast::Must($2, $4); }
;

must_stmt_body
: must_stmt_body_stmt                                               { $$ = new Yang::Ast::Nodes(1, $1); }
| must_stmt_body must_stmt_body_stmt                                { $1->push_back($2); $$ = $1; }
;

must_stmt_body_stmt
: error_message_stmt                                                { $$ = $1; }
| error_app_tag_stmt                                                { $$ = $1; }
| description_stmt                                                  { $$ = $1; }
| reference_stmt                                                    { $$ = $1; }
| unknown_stmt                                                       { $$ = $1; } 
;

/*    error-message-stmt  = error-message-keyword sep string stmtend */
error_message_stmt
: ERROR_MESSAGE_KW string ';'                                 { $$ = new Yang::Ast::ErrorMessage($2); }
;

/*    error-app-tag-stmt  = error-app-tag-keyword sep string stmtend */
error_app_tag_stmt
: ERROR_APP_TAG_KW string ';'                                 { $$ = new Yang::Ast::ErrorAppTag($2); }
;

/*    min-elements-stmt   = min-elements-keyword sep min-value-arg-str stmtend */
/*    min-value-arg-str   = < a string that matches the rule > < min-value-arg > */
/*    min-value-arg       = non-negative-integer-value */
min_elements_stmt
: MIN_ELEMENTS_KW INTEGER ';'                                 { $$ = new Yang::Ast::MinElements($2); } /* @todo check non negative int */
;


/*    max-elements-stmt   = max-elements-keyword sep max-value-arg-str stmtend */
/*    max-value-arg-str   = < a string that matches the rule > < max-value-arg > */
/*    max-value-arg       = unbounded-keyword / positive-integer-value */
max_elements_stmt
: MAX_ELEMENTS_KW INTEGER ';'                                 { $$ = new Yang::Ast::MaxElements($2); } /* @todo check non negative int and add unbounded keyword */
;

/*    value-stmt          = value-keyword sep integer-value-str stmtend */
value_stmt
: VALUE_KW INTEGER ';'                                        { $$ = new Yang::Ast::Value($2); }
;

/*    integer-value-str   = < a string that matches the rule > < integer-value > */

/*    grouping-stmt       = grouping-keyword sep identifier-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [status-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                               *(typedef-stmt / grouping-stmt) */
/*                               *data-def-stmt */
/*                               *action-stmt */
/*                               *notification-stmt */
/*                           "}") stmtsep */
grouping_stmt
: GROUPING_KW IDENTIFIER ';'                                          { $$ = new Yang::Ast::Grouping($2); }
| GROUPING_KW IDENTIFIER '{' grouping_stmt_body '}'                   { $$ = new Yang::Ast::Grouping($2, $4); }
;

grouping_stmt_body
: grouping_stmt_body_stmt                                             { $$ = new Yang::Ast::Nodes(1, $1); }
| grouping_stmt_body grouping_stmt_body_stmt                          { $1->push_back($2); $$ = $1; }
;

grouping_stmt_body_stmt
: status_stmt                                                         { $$ = $1; }       
| description_stmt                                                    { $$ = $1; }
| reference_stmt                                                      { $$ = $1; }
| typedef_stmt                                                        { $$ = $1; }
| grouping_stmt                                                       { $$ = $1; }
| container_stmt                                                      { $$ = $1; } /* data-def-stmt */
| leaf_stmt                                                           { $$ = $1; }
| leaf_list_stmt                                                      { $$ = $1; }
| list_stmt                                                           { $$ = $1; }
| anydata_stmt                                                        { $$ = $1; }
| anyxml_stmt                                                         { $$ = $1; }
| choice_stmt                                                         { $$ = $1; }
| uses_stmt                                                           { $$ = $1; }
| action_stmt                                                         { $$ = $1; }
| notification_stmt                                                   { $$ = $1; }
| unknown_stmt                                                        { $$ = $1; } 
;

/*    container-stmt      = container-keyword sep identifier-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [when-stmt] */
/*                               *if-feature-stmt */
/*                               *must-stmt */
/*                               [presence-stmt] */
/*                               [config-stmt] */
/*                               [status-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                               *(typedef-stmt / grouping-stmt) */
/*                               *data-def-stmt */
/*                               *action-stmt */
/*                               *notification-stmt */
/*                           "}") stmtsep */
container_stmt
: CONTAINER_KW IDENTIFIER ';'                                           { $$ = new Yang::Ast::Container($2); }
| CONTAINER_KW IDENTIFIER '{' container_stmt_body '}'                   { $$ = new Yang::Ast::Container($2, $4); }
;

container_stmt_body
: container_stmt_body_stmt                                              { $$ = new Yang::Ast::Nodes(1, $1); }
| container_stmt_body container_stmt_body_stmt                          { $1->push_back($2); $$ = $1; }
;

container_stmt_body_stmt
: when_stmt                                                             { $$ = $1; }
| if_feature_stmt                                                       { $$ = $1; }
| must_stmt                                                             { $$ = $1; }
| presence_stmt                                                         { $$ = $1; }
| config_stmt                                                           { $$ = $1; }
| status_stmt                                                           { $$ = $1; }
| description_stmt                                                      { $$ = $1; }
| reference_stmt                                                        { $$ = $1; }
| typedef_stmt                                                          { $$ = $1; }
| grouping_stmt                                                         { $$ = $1; }
| container_stmt                                                        { $$ = $1; } /* *data-def-stmt */
| leaf_stmt                                                             { $$ = $1; }
| leaf_list_stmt                                                        { $$ = $1; }
| list_stmt                                                             { $$ = $1; }
| anydata_stmt                                                          { $$ = $1; }
| anyxml_stmt                                                           { $$ = $1; }
| choice_stmt                                                           { $$ = $1; }
| uses_stmt                                                             { $$ = $1; }
| action_stmt                                                           { $$ = $1; }
| notification_stmt                                                     { $$ = $1; }
| unknown_stmt                                                          { $$ = $1; }
;

/*    leaf-stmt           = leaf-keyword sep identifier-arg-str optsep */
/*                          "{" stmtsep */
/*                              ;; these stmts can appear in any order */
/*                              [when-stmt] */
/*                              *if-feature-stmt */
/*                              type-stmt */
/*                              [units-stmt] */
/*                              *must-stmt */
/*                              [default-stmt] */
/*                              [config-stmt] */
/*                              [mandatory-stmt] */
/*                              [status-stmt] */
/*                              [description-stmt] */
/*                              [reference-stmt] */
/*                           "}" stmtsep */
leaf_stmt
: LEAF_KW IDENTIFIER '{' leaf_stmt_body '}'                             { $$ = new Yang::Ast::Leaf($2, $4); }
;

leaf_stmt_body
: leaf_stmt_body_stmt                                                   { $$ = new Yang::Ast::Nodes(1, $1); }
| leaf_stmt_body leaf_stmt_body_stmt                                    { $1->push_back($2); $$ = $1; }
;

leaf_stmt_body_stmt
: when_stmt                                                             { $$ = $1; }
| if_feature_stmt                                                       { $$ = $1; }
| type_stmt                                                             { $$ = $1; }
| units_stmt                                                            { $$ = $1; }
| must_stmt                                                             { $$ = $1; }
| default_stmt                                                          { $$ = $1; }
| config_stmt                                                           { $$ = $1; }
| mandatory_stmt                                                        { $$ = $1; }
| status_stmt                                                           { $$ = $1; }
| description_stmt                                                      { $$ = $1; }
| reference_stmt                                                        { $$ = $1; }
| unknown_stmt                                                          { $$ = $1; } 
;


/*    leaf-list-stmt      = leaf-list-keyword sep identifier-arg-str optsep */
/*                          "{" stmtsep */
/*                              ;; these stmts can appear in any order */
/*                              [when-stmt] */
/*                              *if-feature-stmt */
/*                              type-stmt stmtsep */
/*                              [units-stmt] */
/*                              *must-stmt */
/*                              *default-stmt */
/*                              [config-stmt] */
/*                              [min-elements-stmt] */
/*                              [max-elements-stmt] */
/*                              [ordered-by-stmt] */
/*                              [status-stmt] */
/*                              [description-stmt] */
/*                              [reference-stmt] */
/*                           "}" stmtsep */
leaf_list_stmt
: LEAF_LIST_KW IDENTIFIER '{' leaf_list_stmt_body '}'                    { $$ = new Yang::Ast::LeafList($2, $4); }
;

leaf_list_stmt_body
: leaf_list_stmt_body_stmt                                               { $$ = new Yang::Ast::Nodes(1, $1); }
| leaf_list_stmt_body leaf_list_stmt_body_stmt                           { $1->push_back($2); $$ = $1; }
;

leaf_list_stmt_body_stmt
: when_stmt                                                              { $$ = $1; }
| if_feature_stmt                                                        { $$ = $1; }
| type_stmt                                                              { $$ = $1; }
| units_stmt                                                             { $$ = $1; }
| must_stmt                                                              { $$ = $1; }
| default_stmt                                                           { $$ = $1; }
| config_stmt                                                            { $$ = $1; }
| min_elements_stmt                                                      { $$ = $1; }
| max_elements_stmt                                                      { $$ = $1; }
| ordered_by_stmt                                                        { $$ = $1; }
| status_stmt                                                            { $$ = $1; }
| description_stmt                                                       { $$ = $1; }
| reference_stmt                                                         { $$ = $1; }
| unknown_stmt                                                           { $$ = $1; } 
;

/*    list-stmt           = list-keyword sep identifier-arg-str optsep */
/*                          "{" stmtsep */
/*                              ;; these stmts can appear in any order */
/*                              [when-stmt] */
/*                              *if-feature-stmt */
/*                              *must-stmt */
/*                              [key-stmt] */
/*                              *unique-stmt */
/*                              [config-stmt] */
/*                              [min-elements-stmt] */
/*                              [max-elements-stmt] */
/*                              [ordered-by-stmt] */
/*                              [status-stmt] */
/*                              [description-stmt] */
/*                              [reference-stmt] */
/*                              *(typedef-stmt / grouping-stmt) */
/*                              1*data-def-stmt */
/*                              *action-stmt */
/*                              *notification-stmt */
/*                           "}" stmtsep */
list_stmt
: LIST_KW IDENTIFIER '{' list_stmt_body '}'                               { $$ = new Yang::Ast::List($2, $4); }
;

list_stmt_body
: list_stmt_body_stmt                                                     { $$ = new Yang::Ast::Nodes(1, $1); }
| list_stmt_body list_stmt_body_stmt                                      { $1->push_back($2); $$ = $1; }
;

list_stmt_body_stmt
: when_stmt                                                               { $$ = $1; }
| if_feature_stmt                                                         { $$ = $1; }
| must_stmt                                                               { $$ = $1; }
| key_stmt                                                                { $$ = $1; }
| unique_stmt                                                             { $$ = $1; }
| config_stmt                                                             { $$ = $1; }
| min_elements_stmt                                                       { $$ = $1; }
| max_elements_stmt                                                       { $$ = $1; }
| ordered_by_stmt                                                         { $$ = $1; }
| status_stmt                                                             { $$ = $1; }
| description_stmt                                                        { $$ = $1; }
| reference_stmt                                                          { $$ = $1; }
| typedef_stmt                                                            { $$ = $1; }
| grouping_stmt                                                           { $$ = $1; }
| container_stmt                                                          { $$ = $1; } /* data-def-stmt */
| leaf_stmt                                                               { $$ = $1; }
| leaf_list_stmt                                  { $$ = $1; }
| list_stmt                                       { $$ = $1; }
| anydata_stmt                                    { $$ = $1; }
| anyxml_stmt                                     { $$ = $1; }
| choice_stmt                                     { $$ = $1; }
| uses_stmt                                       { $$ = $1; }
| action_stmt                                     { $$ = $1; }
| notification_stmt                               { $$ = $1; }
| unknown_stmt                                                             { $$ = $1; } 
;

/*    key-stmt            = key-keyword sep key-arg-str stmtend */
key_stmt
: KEY_KW key_arg ';'                                { $$ = new Yang::Ast::Key($2); }
;

/*    key-arg-str         = < a string that matches the rule > < key-arg > */
/*    key-arg             = node-identifier *(sep node-identifier) */
key_arg
: node_identifier                             { $$ = new Ydb::Common::Qnames(1, $1); }
| key_arg node_identifier                     { $1->push_back($2); $$ = $1; }
;

/*    unique-stmt         = unique-keyword sep unique-arg-str stmtend */
/*    unique-arg-str      = < a string that matches the rule > < unique-arg > */
/*    unique-arg          = descendant-schema-nodeid *(sep descendant-schema-nodeid) */
unique_stmt
: UNIQUE_KW unique_arg ';'                        { $$ = new Yang::Ast::Unique($2); }
; 

unique_arg
: descendant_schema_nodeid                        { $$ = new std::list<Yang::Ast::DescendantSchemaNodeId*>(1, $1); }
| unique_arg descendant_schema_nodeid             { $1->push_back($2); $$ = $1; }

/*    choice-stmt         = choice-keyword sep identifier-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [when-stmt] */
/*                               *if-feature-stmt */
/*                               [default-stmt] */
/*                               [config-stmt] */
/*                               [mandatory-stmt] */
/*                               [status-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                               *(short-case-stmt / case-stmt) */
/*                           "}") stmtsep */
choice_stmt
: CHOICE_KW IDENTIFIER ';'                      { $$ = new Yang::Ast::Choice($2); }
| CHOICE_KW IDENTIFIER '{' choice_body '}'      { $$ = new Yang::Ast::Choice($2, $4); }
;

choice_body
: choice_body_stmt                              { $$ = new Yang::Ast::Nodes(1, $1); }
| choice_body choice_body_stmt                  { $1->push_back($2); $$ = $1; }
;

choice_body_stmt
: when_stmt                                     { $$ = $1; }
| if_feature_stmt                               { $$ = $1; }
| default_stmt                                  { $$ = $1; }
| config_stmt                                   { $$ = $1; }
| mandatory_stmt                                { $$ = $1; }
| status_stmt                                   { $$ = $1; }
| description_stmt                              { $$ = $1; }
| reference_stmt                                { $$ = $1; }
| choice_stmt                                   { $$ = $1; } /* short-case-stmt */
| container_stmt                                { $$ = $1; }
| leaf_stmt                                     { $$ = $1; }
| leaf_list_stmt                                { $$ = $1; }
| list_stmt                                     { $$ = $1; }
| anydata_stmt                                  { $$ = $1; }
| anyxml_stmt                                   { $$ = $1; }
| case_stmt                                     { $$ = $1; }
| unknown_stmt                                                       { $$ = $1; } 
;

/*    short-case-stmt     = choice-stmt / */
/*                          container-stmt / */
/*                          leaf-stmt / */
/*                          leaf-list-stmt / */
/*                          list-stmt / */
/*                          anydata-stmt / */
/*                          anyxml-stmt */

/*    case-stmt           = case-keyword sep identifier-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [when-stmt] */
/*                               *if-feature-stmt */
/*                               [status-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                               *data-def-stmt */
/*                           "}") stmtsep */
case_stmt
: CASE_KW IDENTIFIER ';'                          { $$ = new Yang::Ast::Case($2); }
| CASE_KW IDENTIFIER '{' case_stmt_body '}'       { $$ = new Yang::Ast::Case($2, $4); }
;

case_stmt_body
: case_stmt_body_stmt                             { $$ = new Yang::Ast::Nodes(1, $1); }
| case_stmt_body case_stmt_body_stmt              { $1->push_back($2); $$ = $1; }
;

case_stmt_body_stmt
: when_stmt                                       { $$ = $1; }
| if_feature_stmt                                 { $$ = $1; }
| status_stmt                                     { $$ = $1; }
| description_stmt                                { $$ = $1; }
| reference_stmt                                  { $$ = $1; }
| choice_stmt                                     { $$ = $1; } /* data-def-stmt */
| container_stmt                                  { $$ = $1; }
| leaf_stmt                                       { $$ = $1; }
| leaf_list_stmt                                  { $$ = $1; }
| list_stmt                                       { $$ = $1; }
| anydata_stmt                                   { $$ = $1; }
| anyxml_stmt                                    { $$ = $1; }
| unknown_stmt                                                       { $$ = $1; } 
;

/*    anydata-stmt        = anydata-keyword sep identifier-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [when-stmt] */
/*                               *if-feature-stmt */
/*                               *must-stmt */
/*                               [config-stmt] */
/*                               [mandatory-stmt] */
/*                               [status-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                            "}") stmtsep */
anydata_stmt
: ANYDATA_KW IDENTIFIER ';'                               { $$ = new Yang::Ast::AnyData($2); }
| ANYDATA_KW IDENTIFIER '{' anydata_anyxml_stmt_body '}'  { $$ = new Yang::Ast::AnyData($2, $4); }
;

anydata_anyxml_stmt_body
: anydata_anyxml_stmt_body_stmt                           { $$ = new Yang::Ast::Nodes(1, $1); }
| anydata_anyxml_stmt_body anydata_anyxml_stmt_body_stmt  { $1->push_back($2); $$ = $1; }
;

anydata_anyxml_stmt_body_stmt
: when_stmt                                               { $$ = $1; }
| if_feature_stmt                                         { $$ = $1; }
| must_stmt                                               { $$ = $1; }
| config_stmt                                             { $$ = $1; }
| mandatory_stmt                                          { $$ = $1; }
| status_stmt                                             { $$ = $1; }
| description_stmt                                        { $$ = $1; }
| reference_stmt                                          { $$ = $1; }
| unknown_stmt                                                       { $$ = $1; } 
;  

/*    anyxml-stmt         = anyxml-keyword sep identifier-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [when-stmt] */
/*                               *if-feature-stmt */
/*                               *must-stmt */
/*                               [config-stmt] */
/*                               [mandatory-stmt] */
/*                               [status-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                            "}") stmtsep */
anyxml_stmt
: ANYXML_KW IDENTIFIER ';'                               { $$ = new Yang::Ast::AnyXml($2); }
| ANYXML_KW IDENTIFIER '{' anydata_anyxml_stmt_body '}'  { $$ = new Yang::Ast::AnyXml($2, $4); }
;

/*    uses-stmt           = uses-keyword sep identifier-ref-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [when-stmt] */
/*                               *if-feature-stmt */
/*                               [status-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                               *refine-stmt */
/*                               *uses-augment-stmt */
/*                           "}") stmtsep */
uses_stmt
: USES_KW identifier_ref ';'                    { $$ = new Yang::Ast::Uses($2); }
| USES_KW identifier_ref '{' uses_stmt_body '}' { $$ = new Yang::Ast::Uses($2, $4); }
;

uses_stmt_body
: uses_stmt_body_stmt                           { $$ = new Yang::Ast::Nodes(1, $1); }
| uses_stmt_body uses_stmt_body_stmt            { $1->push_back($2); $$ = $1; }
;

uses_stmt_body_stmt
: when_stmt                                     { $$ = $1; }
| if_feature_stmt                               { $$ = $1; }
| status_stmt                                   { $$ = $1; }
| description_stmt                              { $$ = $1; }
| reference_stmt                                { $$ = $1; }
/* | refine_stmt */
/* | uses_augment_stmt */
| unknown_stmt                                                       { $$ = $1; } 
;

/*    refine-stmt         = refine-keyword sep refine-arg-str optsep */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               *if-feature-stmt */
/*                               *must-stmt */
/*                               [presence-stmt] */
/*                               [default-stmt] */
/*                               [config-stmt] */
/*                               [mandatory-stmt] */
/*                               [min-elements-stmt] */
/*                               [max-elements-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                             "}" stmtsep */

/*    refine-arg-str      = < a string that matches the rule > */
/*                          < refine-arg > */

/*    refine-arg          = descendant-schema-nodeid */

/*    uses-augment-stmt   = augment-keyword sep uses-augment-arg-str optsep */
/*                          "{" stmtsep */
/*                              ;; these stmts can appear in any order */
/*                              [when-stmt] */
/*                              *if-feature-stmt */
/*                              [status-stmt] */
/*                              [description-stmt] */
/*                              [reference-stmt] */
/*                              1*(data-def-stmt / case-stmt / */
/*                                 action-stmt / notification-stmt) */
/*                           "}" stmtsep */

/*    uses-augment-arg-str = < a string that matches the rule > */
/*                           < uses-augment-arg > */

/*    uses-augment-arg    = descendant-schema-nodeid */

/*    augment-stmt        = augment-keyword sep augment-arg-str optsep */
/*                          "{" stmtsep */
/*                              ;; these stmts can appear in any order */
/*                              [when-stmt] */
/*                              *if-feature-stmt */
/*                              [status-stmt] */
/*                              [description-stmt] */
/*                              [reference-stmt] */
/*                              1*(data-def-stmt / case-stmt / */
/*                                 action-stmt / notification-stmt) */
/*                           "}" stmtsep */
augment_stmt
: AUGMENT_KW absolute_schema_nodeid '{' augment_body '}'       { $$ = new Yang::Ast::Augment($2, $4); }
;

augment_body
: augment_body_stmt                                            { $$ = new Yang::Ast::Nodes(1, $1); }
| augment_body augment_body_stmt                               { $1->push_back($2); $$ = $1; }
;

augment_body_stmt
: when_stmt                                                    { $$ = $1; }
| if_feature_stmt                                              { $$ = $1; }
| status_stmt                                                  { $$ = $1; }
| description_stmt                                             { $$ = $1; }
| reference_stmt                                               { $$ = $1; }
| case_stmt                                                    { $$ = $1; }
| container_stmt                                               { $$ = $1; } /* data-def-stmt */
| leaf_stmt                                                    { $$ = $1; }
| leaf_list_stmt                                               { $$ = $1; }
| list_stmt                                                    { $$ = $1; }
| anydata_stmt                                                 { $$ = $1; }
| anyxml_stmt                                                  { $$ = $1; }
| choice_stmt                                                  { $$ = $1; }
| uses_stmt                                                    { $$ = $1; }
| action_stmt                                                  { $$ = $1; }
| notification_stmt                                            { $$ = $1; } 
| unknown_stmt                                                       { $$ = $1; } 
;


/*    augment-arg-str     = < a string that matches the rule > */
/*                          < augment-arg > */

/*    augment-arg         = absolute-schema-nodeid */

/*    when-stmt           = when-keyword sep string optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                            "}") stmtsep */
when_stmt
: WHEN_KW string ';'                     { $$ = new Yang::Ast::When($2); }
| WHEN_KW string '{' when_stmt_body '}'  { $$ = new Yang::Ast::When($2, $4); }
;

when_stmt_body
: when_stmt_body_stmt                           { $$ = new Yang::Ast::Nodes(1, $1); }
| when_stmt_body when_stmt_body_stmt            { $1->push_back($2); $$ = $1; }
;

when_stmt_body_stmt
: description_stmt                              { $$ = $1; }
| reference_stmt                                { $$ = $1; }
| unknown_stmt                                                       { $$ = $1; } 
;

/*    rpc-stmt            = rpc-keyword sep identifier-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               *if-feature-stmt */
/*                               [status-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                               *(typedef-stmt / grouping-stmt) */
/*                               [input-stmt] */
/*                               [output-stmt] */
/*                           "}") stmtsep */
rpc_stmt
: RPC_KW IDENTIFIER ';'                           { $$ = new Yang::Ast::Rpc($2); }
| RPC_KW IDENTIFIER '{' rpc_action_stmt_body '}'  { $$ = new Yang::Ast::Rpc($2, $4); }
;

rpc_action_stmt_body
: rpc_action_stmt_body_stmt                       { $$ = new Yang::Ast::Nodes(1, $1); }
| rpc_action_stmt_body rpc_action_stmt_body_stmt  { $1->push_back($2); $$ = $1; }
;

rpc_action_stmt_body_stmt
: if_feature_stmt                                 { $$ = $1; }
| status_stmt                                     { $$ = $1; }
| description_stmt                                { $$ = $1; }
| reference_stmt                                  { $$ = $1; }
| typedef_stmt                                    { $$ = $1; }
| grouping_stmt                                   { $$ = $1; }
| input_stmt                                      { $$ = $1; }
| output_stmt                                     { $$ = $1; }
| unknown_stmt                                                       { $$ = $1; } 
;

/*    action-stmt         = action-keyword sep identifier-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               *if-feature-stmt */
/*                               [status-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                               *(typedef-stmt / grouping-stmt) */
/*                               [input-stmt] */
/*                               [output-stmt] */
/*                           "}") stmtsep */

action_stmt
: ACTION_KW IDENTIFIER ';'                             { $$ = new Yang::Ast::Action($2); }
| ACTION_KW IDENTIFIER '{' rpc_action_stmt_body '}'    { $$ = new Yang::Ast::Action($2, $4); }
;


/*    input-stmt          = input-keyword optsep */
/*                          "{" stmtsep */
/*                              ;; these stmts can appear in any order */
/*                              *must-stmt */
/*                              *(typedef-stmt / grouping-stmt) */
/*                              1*data-def-stmt */
/*                          "}" stmtsep */
input_stmt
: INPUT_KW '{' input_output_stmt_body '}'              { $$ = new Yang::Ast::Input($3); }
;

input_output_stmt_body
: input_output_stmt_body_stmt                          { $$ = new Yang::Ast::Nodes(1, $1); }
| input_output_stmt_body input_output_stmt_body_stmt   { $1->push_back($2); $$ = $1; }
;

input_output_stmt_body_stmt
: must_stmt                                            { $$ = $1; }
| typedef_stmt                                         { $$ = $1; }
| grouping_stmt                                        { $$ = $1; }
| container_stmt                                       { $$ = $1; } /* data-def-stmt */
| leaf_stmt                                            { $$ = $1; }
| leaf_list_stmt                                       { $$ = $1; }
| list_stmt                                            { $$ = $1; }
| anydata_stmt                                         { $$ = $1; }
| anyxml_stmt                                          { $$ = $1; }
| choice_stmt                                          { $$ = $1; }
| uses_stmt                                            { $$ = $1; }
| unknown_stmt                                                       { $$ = $1; } 
;

/*    output-stmt         = output-keyword optsep */
/*                          "{" stmtsep */
/*                              ;; these stmts can appear in any order */
/*                              *must-stmt */
/*                              *(typedef-stmt / grouping-stmt) */
/*                              1*data-def-stmt */
/*                          "}" stmtsep */
output_stmt
: OUTPUT_KW '{' input_output_stmt_body '}'              { $$ = new Yang::Ast::Output($3); }
;

/*    notification-stmt   = notification-keyword sep identifier-arg-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               *if-feature-stmt */
/*                               *must-stmt */
/*                               [status-stmt] */
/*                               [description-stmt] */
/*                               [reference-stmt] */
/*                               *(typedef-stmt / grouping-stmt) */
/*                               *data-def-stmt */
/*                           "}") stmtsep */
notification_stmt
: NOTIFICATION_KW IDENTIFIER ';'                            { $$ = new Yang::Ast::Notification($2); }
| NOTIFICATION_KW IDENTIFIER '{' notification_stmt_body '}' { $$ = new Yang::Ast::Notification($2, $4); }
;

notification_stmt_body
: notification_stmt_body_stmt                                { $$ = new Yang::Ast::Nodes(1, $1); }
| notification_stmt_body notification_stmt_body_stmt         { $1->push_back($2); $$ = $1; }
;

notification_stmt_body_stmt
: if_feature_stmt                                            { $$ = $1; }
| must_stmt                                                  { $$ = $1; }
| status_stmt                                                { $$ = $1; }
| description_stmt                                           { $$ = $1; }
| reference_stmt                                             { $$ = $1; }
| typedef_stmt                                               { $$ = $1; }
| grouping_stmt                                              { $$ = $1; }
| container_stmt                                             { $$ = $1; } /* data-def-stmt */
| leaf_stmt                                                  { $$ = $1; }
| leaf_list_stmt                                             { $$ = $1; }
| list_stmt                                                  { $$ = $1; }
| anydata_stmt                                               { $$ = $1; }
| anyxml_stmt                                                { $$ = $1; }
| choice_stmt                                                { $$ = $1; }
| uses_stmt                                                  { $$ = $1; }
| unknown_stmt                                                       { $$ = $1; } 
;

/*    deviation-stmt      = deviation-keyword sep deviation-arg-str optsep */
/*                          "{" stmtsep */
/*                              ;; these stmts can appear in any order */
/*                              [description-stmt] */
/*                              [reference-stmt] */
/*                              (deviate-not-supported-stmt / */
/*                                1*(deviate-add-stmt / */
/*                                   deviate-replace-stmt / */
/*                                   deviate-delete-stmt)) */
/*                          "}" stmtsep */

/*    deviation-arg-str   = < a string that matches the rule > */
/*                          < deviation-arg > */

/*    deviation-arg       = absolute-schema-nodeid */

/*    deviate-not-supported-stmt = */
/*                          deviate-keyword sep */
/*                          not-supported-keyword-str stmtend */

/*    deviate-add-stmt    = deviate-keyword sep add-keyword-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [units-stmt] */
/*                               *must-stmt */
/*                               *unique-stmt */
/*                               [default-stmt] */
/*                               [config-stmt] */
/*                               [mandatory-stmt] */
/*                               [min-elements-stmt] */
/*                               [max-elements-stmt] */
/*                           "}") stmtsep */

/*    deviate-delete-stmt = deviate-keyword sep delete-keyword-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [units-stmt] */
/*                               *must-stmt */
/*                               *unique-stmt */
/*                               [default-stmt] */
/*                           "}") stmtsep */

/*    deviate-replace-stmt = deviate-keyword sep replace-keyword-str optsep */
/*                          (";" / */
/*                           "{" stmtsep */
/*                               ;; these stmts can appear in any order */
/*                               [type-stmt] */
/*                               [units-stmt] */
/*                               [default-stmt] */
/*                               [config-stmt] */
/*                               [mandatory-stmt] */
/*                               [min-elements-stmt] */
/*                               [max-elements-stmt] */
/*                           "}") stmtsep */

/*    not-supported-keyword-str = < a string that matches the rule > < not-supported-keyword > */
/*    add-keyword-str     = < a string that matches the rule > < add-keyword > */
/*    delete-keyword-str  = < a string that matches the rule > < delete-keyword >
/*    replace-keyword-str = < a string that matches the rule > < replace-keyword > */

/*    ;; represents the usage of an extension statement */
/*    unknown-statement   = prefix ":" identifier [sep string] optsep */
/*                          (";" / */
/*                           "{" optsep */
/*                               *((yang-stmt / unknown-statement) optsep) */
/*                            "}") stmtsep */
unknown_stmt
: IDENTIFIER ':' IDENTIFIER ';'                         { $$ = new Yang::Ast::Unknown(new Ydb::Common::Qname($1, $3), 0, 0); }
|  IDENTIFIER ':' IDENTIFIER string ';'                 { $$ = new Yang::Ast::Unknown(new Ydb::Common::Qname($1, $3), $4, 0); }
  /* | IDENTIFIER ':' IDENTIFIER '{' unknown_stmt_body '}' */
;


/*    yang-stmt           = action-stmt / */
/*                          anydata-stmt / */
/*                          anyxml-stmt / */
/*                          argument-stmt / */
/*                          augment-stmt / */
/*                          base-stmt / */
/*                          belongs-to-stmt / */
/*                          bit-stmt / */
/*                          case-stmt / */
/*                          choice-stmt / */
/*                          config-stmt / */
/*                          contact-stmt / */
/*                          container-stmt / */
/*                          default-stmt / */
/*                          description-stmt / */
/*                          deviate-add-stmt / */
/*                          deviate-delete-stmt / */
/*                          deviate-not-supported-stmt / */
/*                          deviate-replace-stmt / */
/*                          deviation-stmt / */
/*                          enum-stmt / */
/*                          error-app-tag-stmt / */
/*                          error-message-stmt / */
/*                          extension-stmt / */
/*                          feature-stmt / */
/*                          fraction-digits-stmt / */
/*                          grouping-stmt / */
/*                          identity-stmt / */
/*                          if-feature-stmt / */
/*                          import-stmt / */
/*                          include-stmt / */
/*                          input-stmt / */
/*                          key-stmt / */
/*                          leaf-list-stmt / */
/*                          leaf-stmt / */
/*                          length-stmt / */
/*                          list-stmt / */
/*                          mandatory-stmt / */
/*                          max-elements-stmt / */
/*                          min-elements-stmt / */
/*                          modifier-stmt / */
/*                          module-stmt / */
/*                          must-stmt / */
/*                          namespace-stmt / */
/*                          notification-stmt / */
/*                          ordered-by-stmt / */
/*                          organization-stmt / */
/*                          output-stmt / */
/*                          path-stmt / */
/*                          pattern-stmt / */
/*                          position-stmt / */
/*                          prefix-stmt / */
/*                          presence-stmt / */
/*                          range-stmt / */
/*                          reference-stmt / */
/*                          refine-stmt / */
/*                          require-instance-stmt / */
/*                          revision-date-stmt / */
/*                          revision-stmt / */
/*                          rpc-stmt / */
/*                          status-stmt / */
/*                          submodule-stmt / */
/*                          typedef-stmt / */
/*                          type-stmt / */
/*                          unique-stmt / */
/*                          units-stmt / */
/*                          uses-augment-stmt / */
/*                          uses-stmt / */
/*                          value-stmt / */
/*                          when-stmt / */
/*                          yang-version-stmt / */
/*                          yin-element-stmt */

/*    ;; Ranges */

/*    range-arg-str       = < a string that matches the rule > < range-arg > */
/*    range-arg           = range-part *(optsep "|" optsep range-part) */
range_arg
: range_parts                                                               { $$ = $1; }
;

range_parts
: range_part                                                                 { $$ = new Yang::Ast::Nodes(1, $1); }               
| range_parts '|' range_part                                                 { $1->push_back($3); $$ = $1; }
;

/*    range-part          = range-boundary [optsep ".." optsep range-boundary] */
range_part
: range_boundary                                                             { $$ = new Yang::Ast::Range($1); }
| range_boundary DOTDOT range_boundary                                       { $$ = new Yang::Ast::Range($1, $3); }
;

/*    range-boundary      = min-keyword / max-keyword / integer-value / decimal-value */
range_boundary
: MIN_KW                                                                     { $$ = std::numeric_limits<double>::min(); }
| MAX_KW                                                                     { $$ = std::numeric_limits<double>::max(); }
| INTEGER                                                                    { $$ = $1; }
| DECIMAL                                                                    { $$ = $1; }
;

/*    ;; Lengths */
/*    length-arg-str      = < a string that matches the rule > <length-arg > */
/*    length-arg          = length-part *(optsep "|" optsep length-part) */
length_arg
: length_parts                                                               { $$ = $1; }
;

length_parts
: length_part                                                                { $$ = new Yang::Ast::Nodes(1, $1); }               
| length_parts '|' length_part                                               { $1->push_back($3); $$ = $1; }
;

/*    length-part         = length-boundary [optsep ".." optsep length-boundary] */
length_part
: length_boundary                                                            { $$ = new Yang::Ast::IntegerRange($1); }
| length_boundary DOTDOT length_boundary                                     { $$ = new Yang::Ast::IntegerRange($1, $3); }
;

/*    length-boundary     = min-keyword / max-keyword / non-negative-integer-value */
length_boundary
: MIN_KW                                                                     { $$ = std::numeric_limits<int64_t>::min(); }
| MAX_KW                                                                     { $$ = std::numeric_limits<int64_t>::max(); }
| INTEGER                                                                    {  /* @todo should be unsigned int in lexer */
                                                                                 int64_t i = $1; 
                                                                                 if (i < 0) {
                                                                                     YYERROR; 
                                                                                 } else {
                                                                                     $$ = $1; 
                                                                                 } 
                                                                              }
;

/*    ;; Date */

/*    date-arg-str        = < a string that matches the rule > < date-arg > */
/*    date-arg            = 4DIGIT "-" 2DIGIT "-" 2DIGIT */

/*    ;; Schema Node Identifiers */

/*    schema-nodeid       = absolute-schema-nodeid / descendant-schema-nodeid */
/*    absolute-schema-nodeid = 1*("/" node-identifier) */
absolute_schema_nodeid
: '/' node_identifier                                                          { $$ = new Yang::Ast::AbsoluteSchemaNodeId($2); }
| absolute_schema_nodeid '/' node_identifier                                   { $1->add($3); $$ = $1; }
;

/*    descendant-schema-nodeid = node-identifier [absolute-schema-nodeid] */
descendant_schema_nodeid
: node_identifier                                                              { $$ = new Yang::Ast::DescendantSchemaNodeId($1); }
| node_identifier absolute_schema_nodeid                                       { $$ = new Yang::Ast::DescendantSchemaNodeId($1, $2); }
;

/*    node-identifier     = [prefix ":"] identifier */
node_identifier
: IDENTIFIER                                                                   { $$ = new Ydb::Common::Qname(0, $1); }
| IDENTIFIER ':' IDENTIFIER                                                    { $$ = new Ydb::Common::Qname($1, $3); }
;

/*    ;; Instance Identifiers */

/*    instance-identifier = 1*("/" (node-identifier *predicate)) */
/*    predicate           = "[" *WSP (predicate-expr / pos) *WSP "]" */

/*    predicate-expr      = (node-identifier / ".") *WSP "=" *WSP ((DQUOTE string DQUOTE) / (SQUOTE string SQUOTE)) */
/*    pos                 = non-negative-integer-value */


/*    ;; leafref path */

/*    path-arg-str        = < a string that matches the rule > < path-arg > */
/*    path-arg            = absolute-path / relative-path */
/*    absolute-path       = 1*("/" (node-identifier *path-predicate)) */
/*    relative-path       = 1*(".." "/") descendant-path */

/*    descendant-path     = node-identifier [*path-predicate absolute-path] */
/*    path-predicate      = "[" *WSP path-equality-expr *WSP "]" */
/*    path-equality-expr  = node-identifier *WSP "=" *WSP path-key-expr */
/*    path-key-expr       = current-function-invocation *WSP "/" *WSP rel-path-keyexpr */
/*    rel-path-keyexpr    = 1*(".." *WSP "/" *WSP) *(node-identifier *WSP "/" *WSP) *node-identifier */

/*    ;;; Keywords, using RFC 7405 syntax for case-sensitive strings */

/*    ;; statement keywords */
/*    action-keyword      = %s"action" */
/*    anydata-keyword     = %s"anydata" */
/*    anyxml-keyword      = %s"anyxml" */
/*    argument-keyword    = %s"argument" */
/*    augment-keyword     = %s"augment" */
/*    base-keyword        = %s"base" */
/*    belongs-to-keyword  = %s"belongs-to" */
/*    bit-keyword         = %s"bit" */
/*    case-keyword        = %s"case" */
/*    choice-keyword      = %s"choice" */
/*    config-keyword      = %s"config" */
/*    contact-keyword     = %s"contact" */
/*    container-keyword   = %s"container" */
/*    default-keyword     = %s"default" */
/*    description-keyword = %s"description" */
/*    enum-keyword        = %s"enum" */
/*    error-app-tag-keyword = %s"error-app-tag" */
/*    error-message-keyword = %s"error-message" */
/*    extension-keyword   = %s"extension" */
/*    deviation-keyword   = %s"deviation" */
/*    deviate-keyword     = %s"deviate" */
/*    feature-keyword     = %s"feature" */
/*    fraction-digits-keyword = %s"fraction-digits" */
/*    grouping-keyword    = %s"grouping" */
/*    identity-keyword    = %s"identity" */
/*    if-feature-keyword  = %s"if-feature" */
/*    import-keyword      = %s"import" */
/*    include-keyword     = %s"include" */
/*    input-keyword       = %s"input" */
/*    key-keyword         = %s"key" */
/*    leaf-keyword        = %s"leaf" */
/*    leaf-list-keyword   = %s"leaf-list" */
/*    length-keyword      = %s"length" */
/*    list-keyword        = %s"list" */
/*    mandatory-keyword   = %s"mandatory" */
/*    max-elements-keyword = %s"max-elements" */
/*    min-elements-keyword = %s"min-elements" */
/*    modifier-keyword    = %s"modifier" */
/*    module-keyword      = %s"module" */
/*    must-keyword        = %s"must" */
/*    namespace-keyword   = %s"namespace" */
/*    notification-keyword= %s"notification" */
/*    ordered-by-keyword  = %s"ordered-by" */
/*    organization-keyword= %s"organization" */
/*    output-keyword      = %s"output" */
/*    path-keyword        = %s"path" */
/*    pattern-keyword     = %s"pattern" */
/*    position-keyword    = %s"position" */
/*    prefix-keyword      = %s"prefix" */
/*    presence-keyword    = %s"presence" */
/*    range-keyword       = %s"range" */
/*    reference-keyword   = %s"reference" */
/*    refine-keyword      = %s"refine" */
/*    require-instance-keyword = %s"require-instance" */
/*    revision-keyword    = %s"revision" */
/*    revision-date-keyword = %s"revision-date" */
/*    rpc-keyword         = %s"rpc" */
/*    status-keyword      = %s"status" */
/*    submodule-keyword   = %s"submodule" */
/*    type-keyword        = %s"type" */
/*    typedef-keyword     = %s"typedef" */
/*    unique-keyword      = %s"unique" */
/*    units-keyword       = %s"units" */
/*    uses-keyword        = %s"uses" */
/*    value-keyword       = %s"value" */
/*    when-keyword        = %s"when" */
/*    yang-version-keyword= %s"yang-version" */
/*    yin-element-keyword = %s"yin-element" */

/*    ;; other keywords */

/*    add-keyword         = %s"add" */
/*    current-keyword     = %s"current" */
/*    delete-keyword      = %s"delete" */
/*    deprecated-keyword  = %s"deprecated" */
/*    false-keyword       = %s"false" */
/*    invert-match-keyword = %s"invert-match" */
/*    max-keyword         = %s"max" */
/*    min-keyword         = %s"min" */
/*    not-supported-keyword = %s"not-supported" */
/*    obsolete-keyword    = %s"obsolete" */
/*    replace-keyword     = %s"replace" */
/*    system-keyword      = %s"system" */
/*    true-keyword        = %s"true" */
/*    unbounded-keyword   = %s"unbounded" */
/*    user-keyword        = %s"user" */
/*    and-keyword         = %s"and" */
/*    or-keyword          = %s"or" */
/*    not-keyword         = %s"not" */

/*    current-function-invocation = current-keyword *WSP "(" *WSP ")" */

/*    ;;; Basic Rules */

/*    prefix-arg-str      = < a string that matches the rule > < prefix-arg > */
/*    prefix-arg          = prefix */
/*    prefix              = identifier */

/*    identifier-arg-str  = < a string that matches the rule > < identifier-arg > */

/*    identifier-arg      = identifier */

/*    ;; An identifier MUST NOT start with (('X'|'x') ('M'|'m') ('L'|'l')) */
/*    identifier          = (ALPHA / "_") *(ALPHA / DIGIT / "_" / "-" / ".") */

/*    identifier-ref-arg-str = < a string that matches the rule > < identifier-ref-arg > */
/*    identifier-ref-arg  = identifier-ref */
identifier_ref
: IDENTIFIER                            { $$ =  new Ydb::Common::Qname(0, $1); }
| IDENTIFIER ':' IDENTIFIER             { $$ =  new Ydb::Common::Qname($1, $3); }
;

/*    identifier-ref      = [prefix ":"] identifier */

string
: UNQUOTED_STRING                       { $$ = $1; }
| quoted_string                         { $$ = $1; }
;

quoted_string
: QUOTED_STRING_PART                    { $$ = $1; }
| quoted_string '+' QUOTED_STRING_PART  { $1->append(*$3); delete $3; $$ = $1; }
;

/*    string              = < an unquoted string as returned by > */
/*                          < the scanner, that matches the rule > */
/*                          < yang-string > */

/*    yang-string        = *yang-char */

/*    ;; any Unicode or ISO/IEC 10646 character including tab, carriage */
/*    ;; return, and line feed, but excluding the other C0 control */
/*    ;; characters, the surrogate blocks, and the noncharacters. */
/*    yang-char = %x9 / %xA / %xD / %x20-D7FF / */
/*                                ; exclude surrogate blocks %xD800-DFFF */
/*               %xE000-FDCF /    ; exclude noncharacters %xFDD0-FDEF */
/*               %xFDF0-FFFD /    ; exclude noncharacters %xFFFE-FFFF */
/*               %x10000-1FFFD /  ; exclude noncharacters %x1FFFE-1FFFF */
/*               %x20000-2FFFD /  ; exclude noncharacters %x2FFFE-2FFFF */
/*               %x30000-3FFFD /  ; exclude noncharacters %x3FFFE-3FFFF */
/*               %x40000-4FFFD /  ; exclude noncharacters %x4FFFE-4FFFF */
/*               %x50000-5FFFD /  ; exclude noncharacters %x5FFFE-5FFFF */
/*               %x60000-6FFFD /  ; exclude noncharacters %x6FFFE-6FFFF */
/*               %x70000-7FFFD /  ; exclude noncharacters %x7FFFE-7FFFF */
/*               %x80000-8FFFD /  ; exclude noncharacters %x8FFFE-8FFFF */
/*               %x90000-9FFFD /  ; exclude noncharacters %x9FFFE-9FFFF */
/*               %xA0000-AFFFD /  ; exclude noncharacters %xAFFFE-AFFFF */
/*               %xB0000-BFFFD /  ; exclude noncharacters %xBFFFE-BFFFF */
/*               %xC0000-CFFFD /  ; exclude noncharacters %xCFFFE-CFFFF */
/*               %xD0000-DFFFD /  ; exclude noncharacters %xDFFFE-DFFFF */
/*               %xE0000-EFFFD /  ; exclude noncharacters %xEFFFE-EFFFF */
/*               %xF0000-FFFFD /  ; exclude noncharacters %xFFFFE-FFFFF */
/*               %x100000-10FFFD  ; exclude noncharacters %x10FFFE-10FFFF */

/*    integer-value       = ("-" non-negative-integer-value)  / non-negative-integer-value */

/*    non-negative-integer-value = "0" / positive-integer-value */

/*    positive-integer-value = (non-zero-digit *DIGIT) */

/*    zero-integer-value  = 1*DIGIT */

/*    stmtend             = optsep (";" / "{" stmtsep "}") stmtsep */

/*    sep                 = 1*(WSP / line-break) */
/*                          ; unconditional separator */

/*    optsep              = *(WSP / line-break) */

/*    stmtsep             = *(WSP / line-break / unknown-statement) */

/*    line-break          = CRLF / LF */

/*    non-zero-digit      = %x31-39 */

/*    decimal-value       = integer-value ("." zero-integer-value) */

/*    SQUOTE              = %x27 */
/*                          ; ' (Single Quote) */

/*    ;;; RFC 5234 core rules. */

/*    ALPHA               = %x41-5A / %x61-7A */
/*                          ; A-Z / a-z */

/*    CR                  = %x0D */
/*                          ; carriage return */

/*    CRLF                = CR LF */
/*                          ; Internet standard new line */

/*    DIGIT               = %x30-39 */
/*                          ; 0-9 */

/*    DQUOTE              = %x22 */
/*                          ; double quote */

/*    HEXDIG              = DIGIT / */
/*                          %x61 / %x62 / %x63 / %x64 / %x65 / %x66 */
/*                          ; only lower-case a..f */

/*    HTAB                = %x09 */
/*                          ; horizontal tab */

/*    LF                  = %x0A */
/*                          ; linefeed */

/*    SP                  = %x20 */
/*                          ; space */

/*    VCHAR               = %x21-7E */
/*                          ; visible (printing) characters */

/*    WSP                 = SP / HTAB */
/*                          ; whitespace */


%%
