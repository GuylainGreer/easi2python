#include "comment.hh"
#include "common.hh"
#include <boost/spirit/include/qi.hpp>

using namespace boost::spirit;

qi::rule<common::iter, Comment()> Comment::get_rule()
{
    return "!" >> *(qi::char_ - common::newline);
}
