#include "variable_declaration.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, VariableDeclaration()> VariableDeclaration::get_rule()
{
    static qi::rule<common::iter, VariableDeclaration()> r =
        (qi::no_case[lit("local")[at_c<0>(_val) = true]] >>
         +qi::blank) ||
        ((lit("string")[at_c<1>(_val) = "string"] |
          lit("int")[at_c<1>(_val) = "int"] ) >> +qi::blank) >>
        common::word_list[at_c<2>(_val) = _1];
    r.name("VariableDeclaration");
    return r;
}
