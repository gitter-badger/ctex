/**
 * @file processing.cpp
 * @date 08.06.17
 * @author galarius
 * @copyright 
 */

#include "processing.hpp"
#include "utils.hpp"
#include "glogger.hpp"

#include <sstream>

std::unordered_map<std::string, std::string> Processing::tags{
    { "_", R"!({\_})!" },
    { "/", R"!(\frac)!" },
    { "*", R"!(\cdot)!" },
    { ">=", R"!(\geq)!" },
    { "<=", R"!(\leq)!" },
    { "!=", R"!(\not=)!" },
    { "[", R"!(_{\normalsize)!" },
    { "]", R"!(})!" },
    { "(", R"!(\left()!" },
    { ")", R"!(\right))!" },
};

std::string Processing::apply_transform(const Lexeme& toperator, const std::string& a, const std::string& b, bool in_parenthesis)
{
    const bool space_wrapping = true;
    std::string space = space_wrapping ? " " : "";
    //----------------------------------------------------
    auto replace_op_with_tag = [&](const std::string& op, const std::string& a, const std::string& b) {
        std::stringstream ss;
        ss << a << space << tags[op] << space << b;
        return ss.str();
    };
    //----------------------------------------------------
    std::string result;
    
    switch(toperator.type())
    {
        case LexemeLibrary::operation:
            if (toperator.lexeme() == "/")
            {
                std::stringstream ss;
                ss << tags["/"] << "{" << a << "}" << "{" << b << "}";
                result = ss.str();
            }
            else if (tags.find(toperator.lexeme()) != tags.end())
            {
                result = replace_op_with_tag(toperator.lexeme(), a, b);
            }
            else
            {
                result = a + apply_default_transform(space + toperator.lexeme() + space) + b;
            }
            
            if(in_parenthesis)
            {
                result = tags["("] + space + result + space + tags[")"];
            }
            break;
        case LexemeLibrary::function: {
            std::string x = b.empty() ? a : b;
            if (toperator.lexeme() == "sqrt")
            {
                result = R"!(\sqrt{)!" + x + "}";
            }
            else if(toperator.lexeme() == "pow")
            {
                auto operands = str::split(x, ",");
                std::string a = str::remove_first(operands.front(), tags["("]);
                std::string b = str::remove_last(operands.back(), tags[")"]);
                result = str::trimmed(a) + "^" + str::trimmed(b);
            }
            else
            {
                if(!str::starts_with(x, tags["("]) && !str::ends_with(x, tags[")"]))
                {
                    result = toperator.lexeme() + space + tags["("] +  x + tags[")"];
                }
                else
                {
                    result = toperator.lexeme() + space + x;
                }
            }
            break;
        }
        case LexemeLibrary::index:
            result = replace_op_with_tag(toperator.lexeme(), a, b);
            break;
        default:
            GLogger::instance().logError("Unsupported type:", toperator.type(), " of lexeme:", toperator.lexeme());
            break;
    }
    
    
    return result;
}

std::string Processing::apply_default_transform(const std::string& lex)
{
    std::stringstream ss;
    std::string ret = lex;
    if ( str::find(lex, "_") )
    {
        auto parts = str::split(lex, "_");
        for (auto s : parts)
        {
            ss << s << tags["_"];
        }
        ret = ss.str();
        for (int i = 0; i < tags["_"].length(); ++i)
        {
            ret.pop_back();
        }
        
    }
    return ret;
}
