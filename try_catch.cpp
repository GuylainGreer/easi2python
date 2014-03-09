#include "try_catch.hh"
#include "common.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include "function_definition.hh"

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, TryCatch()>
TryCatch::get_rule(qi::rule<common::iter,Statement()> & s)
{
    static qi::rule<common::iter, TryCatch()> r =
        qi::no_case[qi::lit("try")] >> *qi::blank >> -common::newline >> 
        *(s[push_back(at_c<0>(_val), _1)] >>
          common::end_statement) >>
        qi::no_case[qi::lit("onerror")] >>
        *qi::blank >> -common::newline >> 
        *(s[push_back(at_c<1>(_val), _1)] >>
          common::end_statement) >>
        *qi::blank >> qi::no_case[qi::lit("endonerror")];
    r.name("TryCatch");
    return r;
}
