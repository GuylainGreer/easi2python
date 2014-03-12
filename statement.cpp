#include "statement.hh"
#include "common.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include "function_definition.hh"
#include "try_catch.hh"

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, Statement()> Statement::get_rule()
{
    //Recursive rules must be stored as well, else SEGFAULT!
    static qi::rule<common::iter,FunctionDefinition()> function_definition;
    static qi::rule<common::iter,TryCatch()> try_catch;
    static qi::rule<common::iter,Expression()> expression;
    static qi::rule<common::iter,Statement()> statement =
        *qi::blank >> -(qi::int_[at_c<0>(_val) = _1] >> 
                        +qi::blank) >>
        (Comment::get_rule().copy() |
         Documentation::get_rule().copy() |
         SetStatusTitle::get_rule().copy() |
         SetStatusString::get_rule().copy() |
         VariableDeclaration::get_rule().copy() |
         RunModule::get_rule().copy() |
         StatModule::get_rule().copy() |
         function_definition |
         RemoveString::get_rule().copy() |
         ShowVariable::get_rule().copy() |
         GetUserInput::get_rule().copy() |
         LoadModule::get_rule().copy() |
         GotoLabel::get_rule().copy() |
         LogMessage::get_rule().copy() |
         try_catch |
         ResetPRM::get_rule().copy() |
         ImportVariables::get_rule().copy() |
         FunctionCall::get_rule(expression).copy())[at_c<1>(_val) = _1];
    statement.name("Statement");
    function_definition = FunctionDefinition::get_rule(statement);
    try_catch = TryCatch::get_rule(statement);
    expression = Expression::get_rule();
#ifdef ENABLE_SPIRIT_DEBUGGING
    debug(statement);
    debug(function_definition);
    debug(try_catch);
#endif
    return statement;
}
