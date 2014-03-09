#include "import_variables.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, ImportVariables()> ImportVariables::get_rule()
{
    qi::rule<common::iter, ImportVariables()> r = 
        qi::no_case[qi::lit("import")] >> +qi::blank >>
        (*qi::blank >>
         common::identifier[push_back(at_c<0>(_val), _1)] >>
         *qi::blank) % ',';
    r.name("ImportVariables");
    return r;
}
