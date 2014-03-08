#ifndef EASIPARSE_SHOW_VARIABLE_HH
#define EASIPARSE_SHOW_VARIABLE_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"

struct ShowVariable
{
    std::string var_name;
    int min = -1, max = -1;

    static boost::spirit::qi::rule<common::iter,ShowVariable()> get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(ShowVariable,
                          (std::string, var_name)
                          (int, min)(int, max));

#endif
