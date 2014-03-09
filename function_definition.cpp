#include "function_definition.hh"
#include "common.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include "try_catch.hh"

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, FunctionDefinition()>
FunctionDefinition::get_rule(qi::rule<common::iter,Statement()> & s)
{
    static qi::rule<common::iter, FunctionDefinition()> r =
        (qi::no_case[qi::lit("define")] >> +qi::blank >>
         qi::no_case[qi::lit("function")] >> +qi::blank >>
         +(qi::char_ - '(')[at_c<0>(_val) += _1] >>
         '(' >> *(qi::char_ - ')')[at_c<1>(_val) += _1] >> ')' >>
         *(qi::blank | common::newline) >>
         *(s[push_back(at_c<2>(_val), _1)] >> 
           common::end_statement) >>
         qi::no_case[qi::lit("enddefine")]);
    r.name("FunctionDefinition");
#ifdef ENABLE_SPIRIT_DEBUGGING
    debug(r);
#endif
    return r;
}
