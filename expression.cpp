#include "expression.hh"
#include "common.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include "add_subtract.hh"
#include "multiply_divide.hh"
#include "function_call.hh"

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, Expression()>
Expression::get_rule()
{
    static qi::rule<common::iter,AddSubtract()> add_subtract;
    static qi::rule<common::iter,FunctionCall()> function_call;
    static qi::rule<common::iter, Expression()> r =
        (common::identifier[at_c<0>(_val) = _1] |
         double_[at_c<0>(_val) = _1] |
         common::quoted_string[at_c<1>(_val) = true] | 
         function_call[at_c<0>(_val) = _1] |
         ('(' >> *qi::blank >>
          add_subtract[at_c<0>(_val) = _1] >>
          *qi::blank >> ')'));
    add_subtract = AddSubtract::get_rule(r);
    function_call = FunctionCall::get_rule(r);
    r.name("Expression");
#ifdef ENABLE_SPIRIT_DEBUGGING
    debug(r);
    debug(add_subtract);
    debug(function_call);
#endif
    return r;
}
