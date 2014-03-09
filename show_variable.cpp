#include "show_variable.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter,ShowVariable()> ShowVariable::get_rule()
{
    static qi::rule<common::iter,ShowVariable()> r = 
        qi::no_case["show"] >> +qi::blank >>
        common::identifier[at_c<0>(_val) += _1] >>
        -(+qi::blank >> int_[at_c<1>(_val) = _1] >> *qi::blank >>
          ',' >> *qi::blank >>
          int_[at_c<2>(_val) = _1]);
    r.name("ShowVariable");
    return r;
}
