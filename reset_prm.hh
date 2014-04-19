#ifndef EASIPARSE_RESET_PRM_HH
#define EASIPARSE_RESET_PRM_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"
#include "streaming.hh"

struct ResetPRM : TuplePrinter<ResetPRM>
{
    int dummy = 1;
    static boost::spirit::qi::rule<common::iter, ResetPRM()> get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(ResetPRM, (int, dummy));

#endif
