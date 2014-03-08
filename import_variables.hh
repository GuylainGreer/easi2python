#ifndef EASIPARSE_IMPORT_VARIABLES_HH
#define EASIPARSE_IMPORT_VARIABLES_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"

struct ImportVariables
{
    std::vector<std::string> names;
    static boost::spirit::qi::rule<common::iter, ImportVariables()> get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(ImportVariables,
                          (std::vector<std::string>, names));

#endif
