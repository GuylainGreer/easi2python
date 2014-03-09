#include "reset_prm.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, ResetPRM()> ResetPRM::get_rule()
{
    static qi::rule<common::iter, ResetPRM()> r = 
        qi::no_case[qi::lit("reset")][_val = ResetPRM()];
    r.name("ResetPRM");
#ifdef ENABLE_SPIRIT_DEBUGGING
    debug(r);
#endif
    return r;
}
