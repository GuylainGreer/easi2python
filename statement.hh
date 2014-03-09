#ifndef EASIPARSE_STATEMENT_HH
#define EASIPARSE_STATEMENT_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"
#include "import_variables.hh"
#include "reset_prm.hh"
#include "log_message.hh"
#include "goto_label.hh"
#include "load_module.hh"
#include "get_user_input.hh"
#include "show_variable.hh"
#include "remove_string.hh"
#include "stat_module.hh"
#include "run_module.hh"
#include "variable_declaration.hh"
#include "set_status_string.hh"
#include "set_status_title.hh"
#include "documentation.hh"
#include "comment.hh"

struct FunctionDefinition;
struct TryCatch;

struct Statement
{
    int line_number = -1;
    typedef boost::variant<
        Comment,
        Documentation,
        SetStatusTitle,
        SetStatusString,
        VariableDeclaration,
        RunModule,
        StatModule,
        ShowVariable,
        boost::recursive_wrapper<FunctionDefinition>,
        RemoveString,
        GetUserInput,
        LoadModule,
        GotoLabel,
        LogMessage,
        boost::recursive_wrapper<TryCatch>,
        ResetPRM,
        ImportVariables
        > statement_subtype;
    statement_subtype info;
    static boost::spirit::qi::rule<common::iter, Statement()> get_rule();
};

//And now include the recursive ones
#include "function_definition.hh"
#include "try_catch.hh"

#endif
