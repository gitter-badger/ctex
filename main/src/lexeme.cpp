/**
 * @file lexeme.cpp
 * @date 03.06.17
 * @author galarius
 * @copyright   Copyright © 2017 galarius. All rights reserved.
 */

#include "lexeme.hpp"
#include "glogger.hpp"
#include "utils.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>

int LexemeLibrary::max_priority = 5;

std::vector<LexemeLibrary::LexData> LexemeLibrary::lex_library {
    // brackets
    { "(", { bracketl, 0 } },
    { ")", { bracketr, 0 } },
    // index
    { "[", { index, 0 } },
    { "]", { index, 0 } },
    // functions
    // http://en.cppreference.com/w/c/numeric/math.html
    { "abs", { function, 1 } },
    { "labs", { function, 1 } },
    { "llabs", { function, 1 } },
    { "div", { function, 1 } },
    { "ldiv", { function, 1 } },
    { "lldiv", { function, 1 } },
    { "imaxabs", { function, 1 } },
    { "imaxdiv", { function, 1 } },
    //// base operations
    { "fabs", { function, 1 } },
    { "fabsf", { function, 1 } },
    { "fabsl", { function, 1 } },
    { "fmod", { function, 1 } },
    { "fmodf", { function, 1 } },
    { "fmodl", { function, 1 } },
    { "reminder", { function, 1 } },
    { "reminderf", { function, 1 } },
    { "reminderl", { function, 1 } },
    { "remquo", { function, 1 } },
    { "remquof", { function, 1 } },
    { "remquol", { function, 1 } },
    { "fma", { function, 1 } },
    { "fmaf", { function, 1 } },
    { "fmal", { function, 1 } },
    { "fmax", { function, 1 } },
    { "fmaxf", { function, 1 } },
    { "fmaxl", { function, 1 } },
    { "fmin", { function, 1 } },
    { "fminf", { function, 1 } },
    { "fminl", { function, 1 } },
    { "fdim", { function, 1 } },
    { "fdimf", { function, 1 } },
    { "fdiml", { function, 1 } },
    { "nan", { function, 1 } },
    { "nanf", { function, 1 } },
    { "nanl", { function, 1 } },
    //// exponentioal functions
    { "exp", { function, 1 } },
    { "expf", { function, 1 } },
    { "expl", { function, 1 } },
    { "exp2", { function, 1 } },
    { "exp2f", { function, 1 } },
    { "exp2l", { function, 1 } },
    { "expm1", { function, 1 } },
    { "expm1f", { function, 1 } },
    { "expm1l", { function, 1 } },
    { "log", { function, 1 } },
    { "logf", { function, 1 } },
    { "logl", { function, 1 } },
    { "log10", { function, 1 } },
    { "log10f", { function, 1 } },
    { "log10l", { function, 1 } },
    { "log2", { function, 1 } },
    { "log2f", { function, 1 } },
    { "log2l", { function, 1 } },
    { "log1p", { function, 1 } },
    { "log1pf", { function, 1 } },
    { "log1pl", { function, 1 } },
    //// power functions
    { "pow", { function, 1 } },
    { "powf", { function, 1 } },
    { "powl", { function, 1 } },
    { "sqrt", { function, 1 } },
    { "sqrf", { function, 1 } },
    { "sqrtl", { function, 1 } },
    { "cbrt", { function, 1 } },
    { "cbrtf", { function, 1 } },
    { "cbrtl", { function, 1 } },
    { "hypot", { function, 1 } },
    { "hypotf", { function, 1 } },
    { "hypotl", { function, 1 } },
    //// trigonometric functions
    { "sin", { function, 1 } },
    { "sinf", { function, 1 } },
    { "sinl", { function, 1 } },
    { "cos", { function, 1 } },
    { "cosf", { function, 1 } },
    { "cosl", { function, 1 } },
    { "tan", { function, 1 } },
    { "tanf", { function, 1 } },
    { "tanl", { function, 1 } },
    { "asin", { function, 1 } },
    { "asinf", { function, 1 } },
    { "asinl", { function, 1 } },
    { "acos", { function, 1 } },
    { "acosf", { function, 1 } },
    { "acosl", { function, 1 } },
    { "atan", { function, 1 } },
    { "atanf", { function, 1 } },
    { "atanl", { function, 1 } },
    { "atan2", { function, 1 } },
    { "atan2f", { function, 1 } },
    { "atan2l", { function, 1 } },
    //// hyperbolic functions
    { "sinh", { function, 1 } },
    { "sinhf", { function, 1 } },
    { "sinhl", { function, 1 } },
    { "cosh", { function, 1 } },
    { "coshf", { function, 1 } },
    { "coshl", { function, 1 } },
    { "tanh", { function, 1 } },
    { "tanhf", { function, 1 } },
    { "tanhl", { function, 1 } },
    { "asinh", { function, 1 } },
    { "asinhf", { function, 1 } },
    { "asinhl", { function, 1 } },
    { "acosh", { function, 1 } },
    { "acoshf", { function, 1 } },
    { "acoshl", { function, 1 } },
    { "atanh", { function, 1 } },
    { "atanhf", { function, 1 } },
    { "atanhl", { function, 1 } },
    //// gamma functions and error counting functions
    { "erf", { function, 1 } },
    { "erff", { function, 1 } },
    { "erfl", { function, 1 } },
    { "erfc", { function, 1 } },
    { "erfcf", { function, 1 } },
    { "erfcl", { function, 1 } },
    { "tgamma", { function, 1 } },
    { "tgammaf", { function, 1 } },
    { "tgammal", { function, 1 } },
    { "lgamma", { function, 1 } },
    { "lgammaf", { function, 1 } },
    { "lgammal", { function, 1 } },
    //// rounding functions
    { "ceil", { function, 1 } },
    { "ceilf", { function, 1 } },
    { "ceill", { function, 1 } },
    { "floor", { function, 1 } },
    { "floorf", { function, 1 } },
    { "floorl", { function, 1 } },
    { "trunc", { function, 1 } },
    { "truncf", { function, 1 } },
    { "truncl", { function, 1 } },
    { "round", { function, 1 } },
    { "lround", { function, 1 } },
    { "llround", { function, 1 } },
    { "nearbyint", { function, 1 } },
    { "nearbyintf", { function, 1 } },
    { "nearbyintl", { function, 1 } },
    { "rint", { function, 1 } },
    { "rintf", { function, 1 } },
    { "rintl", { function, 1 } },
    { "lrint", { function, 1 } },
    { "lrintf", { function, 1 } },
    { "lrintl", { function, 1 } },
    { "llrint", { function, 1 } },
    { "llrintf", { function, 1 } },
    { "llrintl", { function, 1 } },
    //// functions for floating numbers
    { "frexp", { function, 1 } },
    { "frexpf", { function, 1 } },
    { "frexpl", { function, 1 } },
    { "ldexp", { function, 1 } },
    { "ldexpf", { function, 1 } },
    { "ldexpl", { function, 1 } },
    { "modf", { function, 1 } },
    { "modff", { function, 1 } },
    { "modfl", { function, 1 } },
    { "scalbn", { function, 1 } },
    { "scalbnf", { function, 1 } },
    { "scalbnl", { function, 1 } },
    { "scalbln", { function, 1 } },
    { "scalblnf", { function, 1 } },
    { "scalblnl", { function, 1 } },
    { "ilogb", { function, 1 } },
    { "ilogbf", { function, 1 } },
    { "ilogbl", { function, 1 } },
    { "logb", { function, 1 } },
    { "logbf", { function, 1 } },
    { "logbl", { function, 1 } },
    { "nextafter", { function, 1 } },
    { "nextafterf", { function, 1 } },
    { "nextafterl", { function, 1 } },
    { "nexttoward", { function, 1 } },
    { "nexttowardf", { function, 1 } },
    { "nexttowardl", { function, 1 } },
    { "copysign", { function, 1 } },
    { "copysignf", { function, 1 } },
    { "copysignl", { function, 1 } },
    //// classification and comparison
    { "fpclassify", { function, 1 } },
    { "isfinite", { function, 1 } },
    { "isinf", { function, 1 } },
    { "isnan", { function, 1 } },
    { "isnormal", { function, 1 } },
    { "signbit", { function, 1 } },
    { "isgreater", { function, 1 } },
    { "isgreaterequal", { function, 1 } },
    { "isless", { function, 1 } },
    { "islessequal", { function, 1 } },
    { "islessgreater", { function, 1 } },
    { "isunordered", { function, 1 } },
    // operations
    { "/", { operation, 1 } },
    { "*", { operation, 1 } },
    { "%", { operation, 1 } },
    { "+", { operation, 2 } },
    { "-", { operation, 2 } },
    { "<",  { operation, 3 } },
    { "<=", { operation, 3 } },
    { ">",  { operation, 3 } },
    { ">=", { operation, 3 } },
    { "==", { operation, 4 } },
    { "!=", { operation, 4 } },
    { "=", { operation,  5 } },
    { ",", { operation,  6 } },
};

std::unordered_map<std::string, std::string> LexemeLibrary::tags{
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

void LexemeLibrary::add_lexeme(const std::string& lex, Type type, int priority)
{
    // add in the form: { lex, { type, priority } }
    LexemeLibrary::lex_library.push_back(
                                         LexData(std::pair<std::string,
                                                 std::pair<Type, int>>(lex, std::pair<Type, int>(type, priority)))
                                         );
}

std::vector<std::string> LexemeLibrary::get_lexemes(Type type)
{
    std::vector<std::string> lexemes;
    for (auto& l : lex_library) {
        if (l.second.first == type) {
            lexemes.push_back(l.first);
        }
    }
    return lexemes;
}

std::string LexemeLibrary::apply_transform(const Lexeme& toperator, const std::string& a, const std::string& b, bool in_parenthesis)
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
        case LexemeLibrary::function:
    
            if (toperator.lexeme() == "sqrt")
            {
                result = R"!(\sqrt{)!" + (b.empty() ? a : b) + "}";
            }
            else
            {
                std::string x = b.empty() ? a : b;
                
                if(!starts_with(x, tags["("]) && !ends_with(x, tags[")"]))
                {
                    result = toperator.lexeme() + space + tags["("] +  x + tags[")"];
                }
                else
                {
                    result = toperator.lexeme() + space + x;
                }
            }
            break;
        case LexemeLibrary::index:
            result = replace_op_with_tag(toperator.lexeme(), a, b);
            break;
        default:
            GLogger::instance().logError("Unsupported type:", toperator.type(), " of lexeme:", toperator.lexeme());
            break;
    }
    
    
    return result;
}

std::string LexemeLibrary::apply_default_transform(const std::string& lex)
{
    std::stringstream ss;
    std::string ret = lex;
    if ( find(lex, "_") )
    {
        auto parts = split(lex, "_");
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

bool LexemeLibrary::is_supported(const std::string& lex)
{
    return std::any_of(LexemeLibrary::lex_library.cbegin(), LexemeLibrary::lex_library.cend(), [&](const LexData& lex_data) -> bool {
        return lex_data.first.compare(lex) == 0;
    });
}

bool LexemeLibrary::is_toperator(LexemeLibrary::Type type) {
    return type == LexemeLibrary::operation ||
    type == LexemeLibrary::function ||
    type == LexemeLibrary::index;
}

LexemeLibrary::Type LexemeLibrary::get_type(const std::string& lex)
{
    for (auto& lex_data : LexemeLibrary::lex_library)
    {
        if (lex_data.first.compare(lex) == 0)
            return lex_data.second.first;     // <first, second = <first, second>>
    }
    return LexemeLibrary::variable;
}

int LexemeLibrary::get_priority(const std::string& lex)
{
    for (auto& lex_data : LexemeLibrary::lex_library)
    {
        if (lex_data.first.compare(lex) == 0)
            return lex_data.second.second;     // <first, second = <first, second>>
    }
    return -1;
}

Lexeme::Lexeme() :
position_(-1)
, priority_(0)
, type_(LexemeLibrary::placeholder)
{ }

Lexeme::Lexeme(const std::string& lexeme) :
lexeme_(lexeme)
, position_(-1)
, priority_(LexemeLibrary::get_priority(lexeme))
, type_(LexemeLibrary::get_type(lexeme))
{ }

Lexeme::Lexeme(Lexeme const * const _lex) :
lexeme_(_lex->lexeme_)
, position_(_lex->position_)
, priority_(_lex->priority_)
, type_(_lex->type_)
{ }

Lexeme::Lexeme(const std::string& lexeme, int pos) :
lexeme_(lexeme)
, position_(pos)
, priority_(LexemeLibrary::get_priority(lexeme))
, type_(LexemeLibrary::get_type(lexeme))
{ }

void Lexeme::set_lexeme(const std::string& lexeme)
{
    this->lexeme_ = lexeme_;
    this->type_ = LexemeLibrary::get_type(lexeme);
    this->priority_ = LexemeLibrary::get_priority(lexeme);
}
void Lexeme::set_lexeme(const std::string& lexeme, int position)
{
    this->lexeme_ = lexeme_;
    this->position_ = position;
    this->type_ = LexemeLibrary::get_type(lexeme);
    this->priority_ = LexemeLibrary::get_priority(lexeme);
}
void Lexeme::set_position(int position)
{
    this->position_ = position;
}
std::string Lexeme::lexeme() const
{
    return this->lexeme_;
}
LexemeLibrary::Type Lexeme::type() const
{
    return this->type_;
}
int Lexeme::priority() const
{
    return this->priority_;
}
int Lexeme::pos() const
{
    return this->position_;
}
void Lexeme::update_priority(int level)
{
    int base_priority = LexemeLibrary::get_priority(this->lexeme());
    this->priority_ = base_priority - (level * LexemeLibrary::max_priority + 1);
}
bool Lexeme::operator< (const Lexeme& lex) const {
    return (priority_ < lex.priority_) || (priority_ == lex.priority_ && position_ > lex.position_);
}
bool Lexeme::operator> (const Lexeme& lex) const {
    return (priority_ > lex.priority_) || (priority_ == lex.priority_ && position_ < lex.position_);
}
