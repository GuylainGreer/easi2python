#ifndef EASIPARSE_REMOVE_STRING_HH
#define EASIPARSE_REMOVE_STRING_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"
#include "streaming.hh"

struct RemoveString : TuplePrinter<RemoveString>
{
    std::string value;
    static boost::spirit::qi::rule<common::iter,RemoveString()> get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(RemoveString,
                          (std::string, value));

#endif
