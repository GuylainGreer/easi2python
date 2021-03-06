#include "documentation.hh"
#include "common.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, Documentation()> Documentation::get_rule()
{
    static qi::rule<common::iter, Documentation()> r =
        ascii::no_case[lit("doc")] >> 
        +qi::blank >> 
        (*(qi::char_ - common::newline)[at_c<0>(_val) += _1]) >> 
        -(common::newline >> *qi::blank >> -int_[at_c<1>(_val) = _1] >> 
          +qi::blank >> ascii::no_case[lit("doc_end")]);
    r.name("Documentation");
#ifdef ENABLE_SPIRIT_DEBUGGING
    debug(r);
#endif
    return r;
}
