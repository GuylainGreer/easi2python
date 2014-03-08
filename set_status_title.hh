#ifndef EASIPARSE_SET_STATUS_TITLE_HH
#define EASIPARSE_SET_STATUS_TITLE_HH

#include "common.hh"
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

struct SetStatusTitle
{
    std::string title;

    static boost::spirit::qi::rule<common::iter, SetStatusTitle()> get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(SetStatusTitle, (std::string, title))

#endif
