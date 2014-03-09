#ifndef EASIPARSE__HH
#define EASIPARSE__HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"
#include "multiply_divide.hh"

struct AddSubtract
{
    std::vector<MultiplyDivide> terms;
    std::vector<char> operations;
    static boost::spirit::qi::rule<common::iter, AddSubtract()>
    get_rule(boost::spirit::qi::rule<common::iter,Expression()> &);
};

BOOST_FUSION_ADAPT_STRUCT(AddSubtract,
                          (std::vector<MultiplyDivide>, terms)
                          (std::vector<char>, operations));

#endif
