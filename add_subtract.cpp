#include "add_subtract.hh"
#include "common.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include "expression.hh"
#include "multiply_divide.hh"
#include "function_call.hh"

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, AddSubtract()>
AddSubtract::get_rule(qi::rule<common::iter,Expression()> & expression)
{
    static qi::rule<common::iter, MultiplyDivide()> multiply_divide =
        MultiplyDivide::get_rule(expression);
    static qi::rule<common::iter, AddSubtract()> r = 
        multiply_divide[push_back(at_c<0>(_val), _1)] % 
        (qi::char_('+')[push_back(at_c<1>(_val), _1)] |
         qi::char_('/')[push_back(at_c<1>(_val), _1)]);
    r.name("AddSubtract");
    return r;
}
