#ifndef EASIPARSE_GOTO_LABEL_HH
#define EASIPARSE_GOTO_LABEL_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"
#include "streaming.hh"

struct GotoLabel : TuplePrinter<GotoLabel>
{
    std::string name;
    static boost::spirit::qi::rule<common::iter, GotoLabel()> get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(GotoLabel,
                          (std::string, name));

#endif
