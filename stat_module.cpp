#include "stat_module.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, StatModule()> StatModule::get_rule()
{
    static qi::rule<common::iter, StatModule()> r =
        (qi::no_case['s'] >> -qi::no_case[lit("tatus")]) >>
        +qi::blank >> 
        +(qi::char_ - common::end_statement)[at_c<0>(_val) += _1];
    r.name("StatModule");
#ifdef ENABLE_SPIRIT_DEBUGGING
    debug(r);
#endif
    return r;
}
