#ifndef EASIPARSE_FUNCTION_CALL_HH
#define EASIPARSE_FUNCTION_CALL_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"
#include "expression.hh"

struct FunctionCall
{
    std::string name;
    std::vector<Expression> arguments;
    static boost::spirit::qi::rule<common::iter, FunctionCall()>
    get_rule(boost::spirit::qi::rule<common::iter,Expression()> &);
};

BOOST_FUSION_ADAPT_STRUCT(FunctionCall,
                          (std::string,name)
                          (std::vector<Expression>,arguments));

#endif
