#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/tuple.hpp>
#include "expression2.hh"
//#include "function_call.hh"

using namespace boost::spirit;
using namespace boost::phoenix;

qi::rule<common::iter, Expression2()> &
Expression2::get_rule()
{
    static qi::rule<common::iter, Expression2()> r;
    static qi::rule<common::iter, utree()> expression;
    static qi::rule<
        common::iter,
        utree::list_type()> signed_expression;
    static qi::rule<common::iter, utree()> term;
    static qi::rule<common::iter, utree()> factor;
    static qi::rule<common::iter, utree()> number;
    static qi::rule<common::iter, utf8_symbol_type()> term_sym, fact_sym;
    signed_expression = 
        *qi::blank >>
        -qi::char_("-")[push_back(_val, true)] >> 
        expression[push_back(_val, _1)];
    expression = *qi::blank >>
        term >> *qi::blank >> -(term_sym >> *qi::blank >> expression);
    term_sym = qi::char_("+-");
    term = *qi::blank >> 
        factor >> *qi::blank >> -(fact_sym >> *qi::blank >> term);
    factor = number | (*qi::blank >> '(' >> 
                       signed_expression >> *qi::blank >> ')');
    fact_sym = qi::char_("*/");
    number =
        double_ |
        common::quoted_string |
        common::identifier;
    r = signed_expression[at_c<0>(_val) = _1];
#ifdef ENABLE_SPIRIT_DEBUGGING
    r.name("ExpressionRoot");
    expression.name("Expression");
    signed_expression.name("SignedExpression");
    term.name("Term");
    factor.name("Factor");
    number.name("Number");
    term_sym.name("TermSymbol");
    fact_sym.name("FactorSymbol");
    debug(r);
    debug(expression);
    debug(signed_expression);
    debug(term);
    debug(factor);
    debug(number);
    debug(term_sym);
    debug(fact_sym);
#endif
    return r;
}

