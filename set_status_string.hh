#ifndef EASIPARSE_SET_STATUS_STRING_HH
#define EASIPARSE_SET_STATUS_STRING_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"
#include "streaming.hh"

struct SetStatusString : TuplePrinter<SetStatusString>
{
    std::string identifier;

    static boost::spirit::qi::rule<common::iter, SetStatusString()> get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(SetStatusString, (std::string, identifier));

#endif
