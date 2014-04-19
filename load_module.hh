#ifndef EASIPARSE_LOAD_MODULE_HH
#define EASIPARSE_LOAD_MODULE_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"
#include "streaming.hh"

struct LoadModule : TuplePrinter<LoadModule>
{
    std::string module_name;
    static boost::spirit::qi::rule<common::iter, LoadModule()> get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(LoadModule,
                          (std::string, module_name));

#endif
