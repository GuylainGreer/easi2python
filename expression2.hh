#ifndef EASIPARSE_EXPRESSION_2_HH
#define EASIPARSE_EXPRESSION_2_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/support_utree.hpp>
#include "common.hh"
#include "streaming.hh"

struct Expression2
{
    boost::spirit::utree expression;
    static boost::spirit::qi::rule<
        common::iter, Expression2()> & get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(Expression2, 
                          (boost::spirit::utree, expression));

#endif
