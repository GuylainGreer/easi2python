#ifndef EASIPARSE_VARIABLE_DECLARATION_HH
#define EASIPARSE_VARIABLE_DECLARATION_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"

struct VariableDeclaration
{
    bool is_local = false;
    std::string type;
    std::vector<std::string> names;

    static boost::spirit::qi::rule<common::iter, VariableDeclaration()> get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(VariableDeclaration,
                          (bool, is_local)
                          (std::string, type)
                          (std::vector<std::string>, names));

#endif
