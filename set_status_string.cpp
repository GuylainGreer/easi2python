#include "set_status_string.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, SetStatusString()> SetStatusString::get_rule()
{
    static qi::rule<common::iter, SetStatusString()> r =
        ascii::no_case[lit("status_s")] >> +qi::blank >>
        common::quoted_string[at_c<0>(_val) = _1];
    r.name("SetStatusString");
    return r;
}
