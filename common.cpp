
#include "common.hh"
#include <boost/spirit/include/qi.hpp> 
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, void()> common::newline =
    '\r' || qi::char_('\n');

qi::rule<common::iter, void()> common::end_statement =
    *qi::blank >> (common::newline | '\\');

qi::rule<common::iter, void()> common::end_quote =
    '"' | common::newline;

qi::rule<common::iter, std::string()> common::identifier =
    //Is it really necessary to specify each action like this?
    //Oh well...
    (qi::alpha[_val = _1] |
     qi::char_('$')[_val = _1] |
     qi::char_('#')[_val = _1] |
     qi::char_('_')[_val = _1]) >>
    *(qi::alnum[_val += _1] |
      qi::char_('$')[_val += _1] |
      qi::char_('#')[_val += _1] |
      qi::char_('_')[_val += _1]);

qi::rule<common::iter, std::string()> common::quoted_string =
    '"' >> *(qi::char_ - common::end_quote) >> common::end_quote;

qi::rule<common::iter, std::vector<std::string>()> common::id_list =
    common::identifier[push_back(_val, _1)] %
     (*qi::blank >> ',' >> *qi::blank);
