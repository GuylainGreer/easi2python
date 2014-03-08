#ifndef EASIPARSE_DOCUMENTATION_HH
#define EASIPARSE_DOCUMENTATION_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"

struct Documentation
{
    std::string documentation;
    int end_doc_line_number;

    static boost::spirit::qi::rule<common::iter, Documentation()> get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(Documentation,
                          (std::string, documentation)
                          (int, end_doc_line_number));

#endif
