#include "variable_declaration.hh"
#include "set_status_string.hh"
#include "set_status_title.hh"
#include "documentation.hh"
#include "comment.hh"
#include "common.hh"
#include <boost/spirit/include/qi.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/spirit/include/support_utree.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/include/qi_no_case.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "typename.hh"
#include <vector>
#include <iterator>
#include <fstream>
#include <cctype>
#include <boost/mpl/at.hpp>

namespace std {
    template <class T>
    ostream & operator<<(ostream & s, const vector<T> & v)
    {
        s << "vector(";
        for(auto && i : v)
        {
            if(std::is_same<T, std::string>::value)
                s << "\"" << i << "\", ";
            else
                s << i << ", ";
        }
        return s << ")";
    }
}

using namespace boost::spirit;
using namespace boost::phoenix;

struct FunctionCall;
struct AddSubtract;
struct MultiplyDivide;

struct Expression
{
    typedef boost::variant<
        double,
        std::string,
        boost::recursive_wrapper<FunctionCall>,
        std::vector<boost::recursive_wrapper<AddSubtract> >
        > type;
    bool quoted_string = false;
    type expression;
};

BOOST_FUSION_ADAPT_STRUCT(Expression,
                          (Expression::type, expression)
                          (bool, quoted_string));

struct MultiplyDivide
{
    Expression left, right;
    bool multiply;
};

BOOST_FUSION_ADAPT_STRUCT(MultiplyDivide,
                          (Expression, left)
                          (Expression, right)
                          (bool, multiply));

struct AddSubtract
{
    MultiplyDivide left, right;
    bool add;
};

BOOST_FUSION_ADAPT_STRUCT(AddSubtract,
                          (MultiplyDivide, left)
                          (MultiplyDivide, right)
                          (bool, add));

struct ExpressionStreamer
{
    std::reference_wrapper<std::ostream> stream;
    void operator()(double d)
    {
        stream << d;
    }
};

std::ostream & operator<<(std::ostream & s, Expression e)
{
    return s;
}

struct FunctionCall
{
    std::string name;
    std::vector<Expression> arguments;
};

BOOST_FUSION_ADAPT_STRUCT(FunctionCall,
                          (std::string,name)
                          (std::vector<Expression>,arguments));

struct RunModule
{
    std::string module_name;
};

BOOST_FUSION_ADAPT_STRUCT(RunModule,
                          (std::string,module_name));

struct StatModule
{
    std::string module_name;
};

BOOST_FUSION_ADAPT_STRUCT(StatModule,
                          (std::string,module_name));

struct RemoveString
{
    std::string value;
};

BOOST_FUSION_ADAPT_STRUCT(RemoveString,
                          (std::string, value));

struct ShowVariable
{
    std::string var_name;
    int min = -1, max = -1;
};

BOOST_FUSION_ADAPT_STRUCT(ShowVariable,
                          (std::string, var_name)
                          (int, min)(int, max));

struct GetUserInput
{
    std::string output_message;
    std::string input_into;
};

BOOST_FUSION_ADAPT_STRUCT(GetUserInput,
                          (std::string, output_message)
                          (std::string, input_into));

struct LoadModule
{
    std::string module_name;
};

BOOST_FUSION_ADAPT_STRUCT(LoadModule,
                          (std::string, module_name));

struct GotoLabel
{
    std::string name;
};

BOOST_FUSION_ADAPT_STRUCT(GotoLabel,
                          (std::string, name));

struct LogMessage
{
    std::string message;
};

BOOST_FUSION_ADAPT_STRUCT(LogMessage,
                          (std::string, message));

struct ResetPRM
{
    int dummy = 1;
};

BOOST_FUSION_ADAPT_STRUCT(ResetPRM, (int, dummy));

struct ImportVariables
{
    std::vector<std::string> names;
};

BOOST_FUSION_ADAPT_STRUCT(ImportVariables,
                          (std::vector<std::string>, names));

struct FunctionDefinition;
struct TryCatch;

struct Statement
{
    int line_number = -1;
    typedef boost::variant<
        Comment,
        Documentation,
        SetStatusTitle,
        SetStatusString,
        VariableDeclaration,
        RunModule,
        StatModule,
        ShowVariable,
        boost::recursive_wrapper<FunctionDefinition>,
        RemoveString,
        GetUserInput,
        LoadModule,
        GotoLabel,
        LogMessage,
        boost::recursive_wrapper<TryCatch>,
        ResetPRM,
        ImportVariables
        > statement_subtype;
    statement_subtype info;
};

BOOST_FUSION_ADAPT_STRUCT(Statement,
                          (int,line_number)
                          (Statement::statement_subtype, info));

struct FunctionDefinition
{
    std::string name, parameters;
    std::vector<Statement> statements;
};

BOOST_FUSION_ADAPT_STRUCT(FunctionDefinition,
                          (std::string, name)
                          (std::string, parameters)
                          (std::vector<Statement>, statements));

struct TryCatch
{
    std::vector<Statement> try_block;
    std::vector<Statement> catch_block;
};

BOOST_FUSION_ADAPT_STRUCT(TryCatch,
                          (std::vector<Statement>, try_block)
                          (std::vector<Statement>, catch_block));

struct streamer : public boost::static_visitor<>
{
    std::string inter;
    std::reference_wrapper<std::ostream> stream;
    streamer(std::ostream & stream, std::string inter):
        inter(inter), stream(stream){}
    template <class T>
    void operator()(T t)
    {
        stream << get_cpp_name<T>() << ": ";
        static const int length =
            boost::fusion::result_of::size<T>::value;
        Print<length-1>(t);
    }

    template <int N,class T>
    void Print(T t)
    {
        if(N)
            Print<N?N-1:N>(t);
        stream << boost::fusion::at_c<N>(t) << inter;
    }
};

std::ostream & operator<<(std::ostream & s, const Statement & st)
{
    if(st.line_number != -1)
        s << "line " << st.line_number << ": ";
    streamer str(s, " ");
    boost::apply_visitor(str, st.info);
    return s;
}

std::string filter_name(std::string name)
{
    std::transform(name.begin(), name.end(), name.begin(),
                   [](char c)
                   {
                       if(!std::isalnum(c)) return '_';
                       else return c;
                   });
    return name;
}

template <class ... Things>
void dummy(Things ...){}

template <class T>
using get_attr =
   typename boost::mpl::at_c<typename T::template_params, 0>::type;

template <class ... Rules>
void init(Rules&...rules)
{
    dummy((rules.name(filter_name(get_cpp_name<get_attr<Rules> >())), 0)...);
    dummy((qi::debug((rules)), 0)...);
}

template <class Rule>
void init2(std::string name, Rule & rule)
{
    rule.name(name);
    qi::debug(rule);
}

#define INIT(n) init2(#n, n);

struct EASIRules :
    qi::grammar<common::iter,
                std::vector<Statement>()>
{
    EASIRules() : EASIRules::base_type(start)
    {
        comment = Comment::get_rule();
        documentation = Documentation::get_rule();
        set_status_title = SetStatusTitle::get_rule();
        set_status_s = SetStatusString::get_rule();
        variable_declaration = VariableDeclaration::get_rule();
        run_module =
            (qi::no_case['r'] >> -qi::no_case[lit("un")]) >>
            +qi::blank >> (common::identifier | common::quoted_string)[at_c<0>(_val) = _1];
        stat_module =
            (qi::no_case['s'] >> -qi::no_case[lit("tatus")]) >>
            +qi::blank >> 
            +(qi::char_ - common::end_statement)[at_c<0>(_val) += _1];
        function_definition =
            (qi::no_case[qi::lit("define")] >> +qi::blank >>
             qi::no_case[qi::lit("function")] >> +qi::blank >>
             +(qi::char_ - '(')[at_c<0>(_val) += _1] >>
             '(' >> *(qi::char_ - ')')[at_c<1>(_val) += _1] >> ')' >>
             *(qi::blank | common::newline) >>
             *(statement[push_back(at_c<2>(_val), _1)] >> 
               common::end_statement) >>
             qi::no_case[qi::lit("enddefine")]);
        remove_string = 
            qi::no_case[qi::lit("rem")] >> +qi::blank >>
            *(qi::char_ - common::newline)[at_c<0>(_val) += _1];
        show_variable =
            qi::no_case["show"] >> +qi::blank >>
            common::identifier[at_c<0>(_val) += _1] >>
            -(+qi::blank >> int_[at_c<1>(_val) = _1] >> *qi::blank >>
              ',' >> *qi::blank >>
              int_[at_c<2>(_val) = _1]);
        get_user_input = qi::no_case[qi::lit("input") |
                                     qi::lit("ask")] >> +qi::blank >>
            common::quoted_string[at_c<0>(_val) = _1] >> *qi::blank >>
            common::identifier[at_c<1>(_val) += _1];
        load_module = qi::no_case[qi::lit("load")] >> +qi::blank >>
            common::quoted_string[at_c<0>(_val) = _1];
        goto_label = *(qi::char_ - (qi::blank | ':'))[at_c<0>(_val) += _1] >> ':';
        log_message = qi::no_case[qi::lit("log")] >> +qi::blank >>
            +(qi::char_ - common::newline)[at_c<0>(_val) += _1];
        try_catch = qi::no_case[qi::lit("try")] >> *qi::blank >> -common::newline >> 
            *(statement[push_back(at_c<0>(_val), _1)] >> common::end_statement) >>
            qi::no_case[qi::lit("onerror")] >>
            *qi::blank >> -common::newline >> 
            *(statement[push_back(at_c<1>(_val), _1)] >> common::end_statement) >>
            *qi::blank >> qi::no_case[qi::lit("endonerror")];
        reset_prm = qi::no_case[qi::lit("reset")][_val = ResetPRM()];
        import_variables = qi::no_case[qi::lit("import")] >> +qi::blank >>
            (*qi::blank >> common::identifier[push_back(at_c<0>(_val), _1)] >> *qi::blank) % ',';
        expression = (common::identifier | double_ | common::quoted_string[at_c<1>(_val) = true] | 
                      function_call
                     /* | ('(' >> *qi::blank >> +add_subtract >> *qi::blank >> ')')*/)
            [at_c<0>(_val) = _1];
        //multiply_divide;
        function_call = common::identifier[at_c<0>(_val) = _1] >> *qi::blank >> '(' >> 
            (*qi::space >> expression[push_back(at_c<1>(_val), _1)] >> 
             *qi::space) % ',' >> ')';

        statement = *qi::blank >> -(qi::int_[at_c<0>(_val) = _1] >> 
                                    +qi::blank) >>
            (comment | documentation | set_status_title | set_status_s |
             variable_declaration | run_module | stat_module |
             function_definition | remove_string | show_variable |
             get_user_input | load_module | goto_label | log_message |
             try_catch | reset_prm | import_variables)
            [at_c<1>(_val) = _1];
        start = *(statement >> +common::end_statement);

        init(start,
             documentation,
             comment,
             set_status_title,
             set_status_s,
             variable_declaration,
             run_module,
             stat_module,
             function_definition,
             remove_string,
             get_user_input,
             load_module,
             goto_label,
             log_message,
             try_catch,
             show_variable,
             reset_prm,
             import_variables,
             statement);
        INIT(common::identifier);
        INIT(common::quoted_string);
        INIT(common::end_statement);
        INIT(common::newline);
        INIT(common::end_quote);
        INIT(common::word_list);
    }

    qi::rule<common::iter,std::vector<Statement>()> start;
    qi::rule<common::iter,Documentation()> documentation;
    qi::rule<common::iter,Comment()> comment;
    qi::rule<common::iter,SetStatusTitle()> set_status_title;
    qi::rule<common::iter,SetStatusString()> set_status_s;
    qi::rule<common::iter,VariableDeclaration()> variable_declaration;
    qi::rule<common::iter,RunModule()> run_module;
    qi::rule<common::iter,StatModule()> stat_module;
    qi::rule<common::iter,FunctionDefinition()> function_definition;
    qi::rule<common::iter,RemoveString()> remove_string;
    qi::rule<common::iter,GetUserInput()> get_user_input;
    qi::rule<common::iter,LoadModule()> load_module;
    qi::rule<common::iter,GotoLabel()> goto_label;
    qi::rule<common::iter,LogMessage()> log_message;
    qi::rule<common::iter,TryCatch()> try_catch;
    qi::rule<common::iter,ShowVariable()> show_variable;
    qi::rule<common::iter,ResetPRM()> reset_prm;
    qi::rule<common::iter,ImportVariables()> import_variables;

    qi::rule<common::iter,Expression()> expression;
    qi::rule<common::iter,AddSubtract()> add_subtract;
    qi::rule<common::iter,MultiplyDivide()> multiply_divide;
    qi::rule<common::iter,FunctionCall()> function_call;
    qi::rule<common::iter,Statement()> statement;
};

int main()
{
    std::ifstream stream("EASI_testfile.txt");
    stream.unsetf(std::ios::skipws);
    std::string text(std::istream_iterator<char>(stream),
                     (std::istream_iterator<char>()));
    common::iter begin = text.begin(), end = text.end();
    std::vector<Statement> s;
    EASIRules easi;
    try
    {
        parse(begin, end, easi, s);
    } catch (std::exception & e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
    std::cout << std::boolalpha;
    std::cout << "Parsed " << s.size() << " statements\n";
    std::copy(s.begin(), s.end(),
              std::ostream_iterator<Statement>(std::cout,"\n"));
    std::cout << "Left over: \"";
    std::copy(begin, end, std::ostream_iterator<char>(std::cout));
    std::cout << "\"\n";
}
