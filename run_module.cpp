#include "run_module.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, RunModule()> RunModule::get_rule()
{
    static qi::rule<common::iter, RunModule()> r =
        (qi::no_case['r'] >> -qi::no_case[lit("un")]) >>
        +qi::blank >> (common::identifier | common::quoted_string)[at_c<0>(_val) = _1];
    r.name("RunModule");
    return r;
}
