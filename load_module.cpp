#include "load_module.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, LoadModule()> LoadModule::get_rule()
{
    static qi::rule<common::iter, LoadModule()> r = 
        qi::no_case[qi::lit("load")] >> +qi::blank >>
        common::quoted_string[at_c<0>(_val) = _1];
    r.name("LoadModule");
#ifdef ENABLE_SPIRIT_DEBUGGING
    debug(r);
#endif
    return r;
}
