#ifndef EASIPARSE_FUNCTION_DEFINITION_HH
#define EASIPARSE_FUNCTION_DEFINITION_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"
#include "statement.hh"
#include "streaming.hh"

struct FunctionDefinition : TuplePrinter<FunctionDefinition>
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
