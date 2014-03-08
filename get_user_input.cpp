#include "get_user_input.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, GetUserInput()> GetUserInput::get_rule()
{
    return qi::no_case[qi::lit("input") |
                       qi::lit("ask")] >> +qi::blank >> 
        common::quoted_string[at_c<0>(_val) = _1] >> *qi::blank >>
        common::identifier[at_c<1>(_val) += _1];
}
