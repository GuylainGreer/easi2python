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

struct Comment
{
    std::string comment;
};

BOOST_FUSION_ADAPT_STRUCT(Comment, (std::string, comment));

struct Documentation
{
    std::string documentation;
    int end_doc_line_number;
};

BOOST_FUSION_ADAPT_STRUCT(Documentation,
                          (std::string, documentation)
                          (int, end_doc_line_number));

struct SetStatusTitle
{
    std::string title;
};

BOOST_FUSION_ADAPT_STRUCT(SetStatusTitle, (std::string, title))

struct SetStatusS
{
    std::string identifier;
};

BOOST_FUSION_ADAPT_STRUCT(SetStatusS, (std::string, identifier));

struct VariableDeclaration
{
    bool is_local = false;
    std::string type;
    std::vector<std::string> names;
};

BOOST_FUSION_ADAPT_STRUCT(VariableDeclaration,
                          (bool, is_local)
                          (std::string, type)
                          (std::vector<std::string>, names));

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
        SetStatusS,
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
    qi::grammar<std::string::iterator,
                std::vector<Statement>()>
{
    EASIRules() : EASIRules::base_type(start)
    {
        newline = qi::char_('\n');
        end_statement = *qi::blank >> (newline | '\\' | qi::eoi);
        end_quote = '"' | end_statement;
        identifier = (qi::alpha | '$' | '#' | '_') >>
            *(qi::alnum | '$' | '#' | '_');
        comment = "!" >> *(qi::char_ - newline);
        documentation =
            ascii::no_case[lit("doc")] >> 
            +qi::blank >> 
            (*(qi::char_ - newline)[at_c<0>(_val) += _1]) >> 
            -(newline >> *qi::blank >> -int_[at_c<1>(_val) = _1] >> 
              +qi::blank >> ascii::no_case[lit("doc_end")]);
        quoted_string = 
            '"' >> *(qi::char_ - end_quote) >> end_quote;
        set_status_title =
            ascii::no_case[lit("status_title")] >> +qi::blank >>
            quoted_string[at_c<0>(_val) = _1];
        set_status_s =
            ascii::no_case[lit("status_s")] >> +qi::blank >>
            quoted_string[at_c<0>(_val) = _1];
        word_list =
            ((+(qi::char_ - (qi::blank | end_statement | ','))) %
             (*qi::blank >> ',' >> *qi::blank));
        variable_declaration =
            (qi::no_case[lit("local")[at_c<0>(_val) = true]] >>
             +qi::blank) ||
             ((lit("string")[at_c<1>(_val) = "string"] |
               lit("int")[at_c<1>(_val) = "int"] ) >> +qi::blank) >>
             word_list[at_c<2>(_val) = _1];
        run_module =
            (qi::no_case['r'] >> -qi::no_case[lit("un")]) >>
            +qi::blank >> (identifier | quoted_string)[at_c<0>(_val) = _1];
        stat_module =
            (qi::no_case['s'] >> -qi::no_case[lit("tatus")]) >>
            +qi::blank >> 
            +(qi::char_ - end_statement)[at_c<0>(_val) += _1];
        function_definition =
            (qi::no_case[qi::lit("define")] >> +qi::blank >>
             qi::no_case[qi::lit("function")] >> +qi::blank >>
             +(qi::char_ - '(')[at_c<0>(_val) += _1] >>
             '(' >> *(qi::char_ - ')')[at_c<1>(_val) += _1] >> ')' >>
             *(qi::blank | newline) >>
             *(statement[push_back(at_c<2>(_val), _1)] >> 
               end_statement) >>
             qi::no_case[qi::lit("enddefine")]);
        remove_string = 
            qi::no_case[qi::lit("rem")] >> +qi::blank >>
            *(qi::char_ - newline)[at_c<0>(_val) += _1];
        show_variable =
            qi::no_case["show"] >> +qi::blank >>
            identifier[at_c<0>(_val) += _1] >>
            -(+qi::blank >> int_[at_c<1>(_val) = _1] >> *qi::blank >>
              ',' >> *qi::blank >>
              int_[at_c<2>(_val) = _1]);
        get_user_input = qi::no_case[qi::lit("input") |
                                     qi::lit("ask")] >> +qi::blank >>
            quoted_string[at_c<0>(_val) = _1] >> *qi::blank >>
            identifier[at_c<1>(_val) += _1];
        load_module = qi::no_case[qi::lit("load")] >> +qi::blank >>
            quoted_string[at_c<0>(_val) = _1];
        goto_label = *(qi::char_ - (qi::blank | ':'))[at_c<0>(_val) += _1] >> ':';
        log_message = qi::no_case[qi::lit("log")] >> +qi::blank >>
            +(qi::char_ - newline)[at_c<0>(_val) += _1];
        try_catch = qi::no_case[qi::lit("try")] >> *qi::blank >> -newline >> 
            *(statement[push_back(at_c<0>(_val), _1)] >> end_statement) >>
            qi::no_case[qi::lit("onerror")] >>
            *qi::blank >> -newline >> 
            *(statement[push_back(at_c<1>(_val), _1)] >> end_statement) >>
            *qi::blank >> qi::no_case[qi::lit("endonerror")];
        reset_prm = qi::no_case[qi::lit("reset")][_val = ResetPRM()];
        import_variables = qi::no_case[qi::lit("import")] >> +qi::blank >>
            (*qi::blank >> identifier[push_back(at_c<0>(_val), _1)] >> *qi::blank) % ',';
        expression = (identifier | double_ | quoted_string[at_c<1>(_val) = true] | 
                      function_call
                     /* | ('(' >> *qi::blank >> +add_subtract >> *qi::blank >> ')')*/)
            [at_c<0>(_val) = _1];
        //multiply_divide;
        function_call = identifier[at_c<0>(_val) = _1] >> *qi::blank >> '(' >> 
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
        start = *(statement >> end_statement);

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
        INIT(identifier);
        INIT(quoted_string);
        INIT(end_statement);
        INIT(newline);
        INIT(end_quote);
        INIT(word_list);
    }

    qi::rule<std::string::iterator,std::vector<Statement>()> start;
    qi::rule<std::string::iterator,Documentation()> documentation;
    qi::rule<std::string::iterator,Comment()> comment;
    qi::rule<std::string::iterator,SetStatusTitle()> set_status_title;
    qi::rule<std::string::iterator,SetStatusS()> set_status_s;
    qi::rule<std::string::iterator,VariableDeclaration()> variable_declaration;
    qi::rule<std::string::iterator,RunModule()> run_module;
    qi::rule<std::string::iterator,StatModule()> stat_module;
    qi::rule<std::string::iterator,FunctionDefinition()> function_definition;
    qi::rule<std::string::iterator,RemoveString()> remove_string;
    qi::rule<std::string::iterator,GetUserInput()> get_user_input;
    qi::rule<std::string::iterator,LoadModule()> load_module;
    qi::rule<std::string::iterator,GotoLabel()> goto_label;
    qi::rule<std::string::iterator,LogMessage()> log_message;
    qi::rule<std::string::iterator,TryCatch()> try_catch;
    qi::rule<std::string::iterator,ShowVariable()> show_variable;
    qi::rule<std::string::iterator,ResetPRM()> reset_prm;
    qi::rule<std::string::iterator,ImportVariables()> import_variables;

    qi::rule<std::string::iterator,std::string()> identifier;
    qi::rule<std::string::iterator,std::string()> quoted_string;
    qi::rule<std::string::iterator,void()> end_statement, newline, end_quote;
    qi::rule<std::string::iterator,std::vector<std::string>()> word_list;
    qi::rule<std::string::iterator,Expression()> expression;
    qi::rule<std::string::iterator,AddSubtract()> add_subtract;
    qi::rule<std::string::iterator,MultiplyDivide()> multiply_divide;
    qi::rule<std::string::iterator,FunctionCall()> function_call;
    qi::rule<std::string::iterator,Statement()> statement;
};

int main()
{
    std::ifstream stream("EASI_testfile.txt");
    stream.unsetf(std::ios::skipws);
    std::string text(std::istream_iterator<char>(stream),
                     (std::istream_iterator<char>()));
    std::string::iterator begin = text.begin(), end = text.end();
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
