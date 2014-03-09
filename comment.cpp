#include "comment.hh"
#include "common.hh"
#include <boost/spirit/include/qi.hpp>

using namespace boost::spirit;

qi::rule<common::iter, Comment()> Comment::get_rule()
{
    static qi::rule<common::iter, Comment()> r =
        "!" >> *(qi::char_ - common::newline);
    r.name("Comment");
#ifdef ENABLE_SPIRIT_DEBUGGING
    debug(r);
#endif
    return r;
}
