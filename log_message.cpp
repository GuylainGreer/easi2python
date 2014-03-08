#include "log_message.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, LogMessage()> LogMessage::get_rule()
{
    return qi::no_case[qi::lit("log")] >> +qi::blank >>
        +(qi::char_ - common::newline)[at_c<0>(_val) += _1];
}
