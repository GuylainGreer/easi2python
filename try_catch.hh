#ifndef EASIPARSE_TRY_CATCH_HH
#define EASIPARSE_TRY_CATCH_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"
#include "statement.hh"

struct TryCatch
{
    std::vector<Statement> try_block;
    std::vector<Statement> catch_block;
    static boost::spirit::qi::rule<common::iter, TryCatch()>
    get_rule(boost::spirit::qi::rule<common::iter,Statement()> & s);
};

BOOST_FUSION_ADAPT_STRUCT(TryCatch,
                          (std::vector<Statement>, try_block)
                          (std::vector<Statement>, catch_block));

#endif
