#ifndef EASIPARSE_COMMENT_HH
#define EASIPARSE_COMMENT_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"

struct Comment
{
    std::string comment;
    static boost::spirit::qi::rule<common::iter, Comment()> get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(Comment, (std::string, comment));

#endif
