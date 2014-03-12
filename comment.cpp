#include "comment.hh"
#include "common.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, Comment()> Comment::get_rule()
{
    static qi::rule<common::iter, Comment()> r =
        "!" >> *qi::blank >> *(qi::char_[at_c<0>(_val) += _1] - common::newline);
    r.name("Comment");
#ifdef ENABLE_SPIRIT_DEBUGGING
    debug(r);
#endif
    return r;
}
