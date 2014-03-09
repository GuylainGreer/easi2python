#ifndef EASIPARSE_MULTIPLY_DIVIDE_HH
#define EASIPARSE_MULTIPLY_DIVIDE_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"
#include "expression.hh"

struct MultiplyDivide
{
    std::vector<Expression> terms;
    std::vector<char> ops;
    static boost::spirit::qi::rule<common::iter, MultiplyDivide()>
    get_rule(boost::spirit::qi::rule<common::iter,Expression()> &);
};

BOOST_FUSION_ADAPT_STRUCT(MultiplyDivide,
                          (std::vector<Expression>, terms)
                          (std::vector<char>, ops));

#endif
