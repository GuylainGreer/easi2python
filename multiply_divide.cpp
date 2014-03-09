#include "multiply_divide.hh"
#include "common.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include "add_subtract.hh"
#include "function_call.hh"

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, MultiplyDivide()>
MultiplyDivide::get_rule(qi::rule<common::iter,Expression()> & expression)
{
    static qi::rule<common::iter, MultiplyDivide()> r = 
        expression[push_back(at_c<0>(_val), _1)] %
        (qi::char_('*')[push_back(at_c<1>(_val), _1)] |
         qi::char_('/')[push_back(at_c<1>(_val), _1)]);
    r.name("MultiplyDivide");
    return r;
}
