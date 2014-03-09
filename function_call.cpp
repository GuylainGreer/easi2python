#include "function_call.hh"
#include "common.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include "add_subtract.hh"

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, FunctionCall()>
FunctionCall::get_rule(qi::rule<common::iter,Expression()> & expression)
{
    static qi::rule<common::iter, FunctionCall()> r = 
        common::identifier[at_c<0>(_val) = _1] >> *qi::blank >> '(' >> 
        (*qi::space >> expression[push_back(at_c<1>(_val), _1)] >> 
         *qi::space) % ',' >> ')';
    r.name("FunctionCall");
    return r;
}
