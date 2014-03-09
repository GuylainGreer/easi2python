#ifndef EASIPARSE_FUNCTION_DEFINITION_HH
#define EASIPARSE_FUNCTION_DEFINITION_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"
#include "statement.hh"

BOOST_FUSION_ADAPT_STRUCT(Statement,
                          (int,line_number)
                          (Statement::statement_subtype, info));

struct FunctionDefinition
{
    std::string name, parameters;
    std::vector<Statement> statements;
    static boost::spirit::qi::rule<common::iter, FunctionDefinition()>
    get_rule(boost::spirit::qi::rule<common::iter,Statement()> & s);
};

BOOST_FUSION_ADAPT_STRUCT(FunctionDefinition,
                          (std::string, name)
                          (std::string, parameters)
                          (std::vector<Statement>, statements));

#endif
