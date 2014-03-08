#include "goto_label.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, GotoLabel()> GotoLabel::get_rule()
{
    return *(qi::char_ - (qi::blank | ':'))[at_c<0>(_val) += _1] >> ':';
}
