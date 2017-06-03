/**
 * @file ctex.cpp
 * @date 03.06.17
 * @author galarius
 * @copyright Copyright © 2017 galarius. All rights reserved.
 * @brief Formla parser and converter from C language into LaTeX
 */

#include "ctex.hpp"
#include "ltree.hpp"
#include "glogger.hpp"
#include "i18n.hpp"

//-------------------------------------------------------------------//
// Constructors, Destructor, Copy/Move Operators
//-------------------------------------------------------------------//


CTex::CTex(const std::vector<std::pair<std::string, std::string>>& grouped_regs)
: grouped_regs_(grouped_regs)
{
    // build full regex expresion
    for (auto const& x : grouped_regs_)
    {
        regex_txt_ += "(" + x.first + ")|";
    }
    regex_txt_.pop_back();  // remove last pipe
    
    // notify about regex expression
    GLogger::instance().logDebug("Regex:"_i18n, regex_txt_);
    
    // init grouped_hits_ map
    for (auto& d : grouped_regs_)
    {
        grouped_hits_.emplace(d.second, 0);
    }
}

CTex::CTex(const CTex& other) :
regex_txt_(other.regex_txt_)
, grouped_regs_(other.grouped_regs_)
, grouped_hits_(other.grouped_hits_)
{ }


CTex& CTex::operator=(const CTex& other)
{
    if(this != &other)
    {
        regex_txt_ = other.regex_txt_;
        grouped_regs_ = other.grouped_regs_;
        grouped_hits_ = other.grouped_hits_;
    }
    return *this;
}

CTex::CTex(CTex &&other)  :
regex_txt_(std::move(other.regex_txt_))
, grouped_regs_(std::move(other.grouped_regs_))
, grouped_hits_(std::move(other.grouped_hits_))
{ }


CTex& CTex::operator=(CTex &&other)
{
    if(this != &other)
    {
        regex_txt_ = std::move(other.regex_txt_);
        grouped_regs_ = std::move(other.grouped_regs_);
        grouped_hits_ = std::move(other.grouped_hits_);
    }
    return *this;
}

//-------------------------------------------------------------------//
// Public methods
//-------------------------------------------------------------------//

std::vector<std::pair<std::string, std::string>> CTex::default_regex()
{
    std::string fregex;
    auto cfunc_library = LexemeLibrary::get_lexemes(LexemeLibrary::function);
    // invert the order, so functions like `atan2` and `log10l` goes
    // before `atan` and `log10`, `log`
    std::reverse(cfunc_library.begin(), cfunc_library.end());
    // build final C functions library expression
    for (auto& f : cfunc_library)
    {
        fregex += f + "|";
    }
    fregex.pop_back();  // remove last pipe
    return  // order is important
    {
        // 1. functions
        { fregex, "function"_i18n },
        // 2. numbers: 0x10h | 1.11e+10 | 1e+10; | all other numbers
        // todo merge: -?\d*\.\d?e[+-]?\d+|-?\d*\.\d+?e[+-]?\d+
        { R"!(-?0x\d+|-?\d*\.\d?e[+-]?\d+|-?\d*\.\d+?e[+-]?\d+|[-+]*\d+\.\d+|[-+]*\.\d+|[-+]*\d+)!", "number"_i18n },
        // 3. operators
        { R"!(\*|\+|\-|\/|\%|\<|\<=|\>|\>=|\==|\!=|\=|\,)!", "operator"_i18n },
        // 4. parenthesis
        { R"!(\(|\))!", "bracket"_i18n },
        // 5. index
        { R"!(\[|\])!", "index"_i18n },
        // 6. variables
        { R"!([a-zA-Z0-9_]+)!", "variable"_i18n },
    };
}

std::string CTex::translate(const std::string& in, EQUATION_TAG_STYLE style)
{
    // build LaTeX expression
    std::string result = eq_open_tag(style);
    auto tokens = lexical_analyzer(in);
    if (tokens.size())
        result += translate(tokens);
    result += eq_close_tag(style);
    return result;
}

int CTex::group_hits(const std::string& group)
{
    if (grouped_hits_.find(group) != grouped_hits_.end())
    {
        return grouped_hits_[group];
    }
    else
    {
        GLogger::instance().logWarn(__FILE__, " : ", __func__," : invalid key", group);
    }
    return 0;
}

//-------------------------------------------------------------------//
// Private methods
//-------------------------------------------------------------------//

std::string CTex::translate(const std::vector<std::string>& tokens)
{
    std::vector<Lexeme> toperators;
    std::vector<Lexeme> lexemes;
    LexemeTree tr;
    int level = 0;
    int pos = 0;
    //------------------------------------------------------------------
    for (auto& t : tokens)
    {
        Lexeme l(t, pos);
        do
        {
            if (l.type() == LexemeLibrary::bracketl) {
                ++level;
                tr.save_parenthesis_pos(l);
                break;
            }
            if (l.type() == LexemeLibrary::bracketr) {
                --level;
                tr.save_parenthesis_pos(l);
                break;
            }
                
            if (LexemeLibrary::is_toperator(l.type()))
            {
                l.update_priority(level);
                toperators.push_back(l);
            }
            else
            {
                lexemes.push_back(l);
            }
        }while(false);
        ++pos;
    }
    
    // sort operators by priority
    std::sort(toperators.rbegin(), toperators.rend(),
              [&](const Lexeme& l1, const Lexeme&l2) -> bool {
                  return l1.priority() < l2.priority();
              });
    GLogger::instance().logTrace("Operation list (sort by priority):"_i18n);
    for(auto& lex : toperators)
    {
        GLogger::instance().logTrace("\t", lex.lexeme(), " with priority: "_i18n, lex.priority(), " and pos: "_i18n, lex.pos());
    }
    GLogger::instance().logTrace("Other lexemes list (sort by position):"_i18n);
    for(auto& lex : lexemes)
    {
        GLogger::instance().logTrace("\t", lex.lexeme(), " with pos: "_i18n, lex.pos());
    };
    
    // I pass - fill with transform operators
    for(auto& op: toperators)
    {
        tr.insert(op);
    }
    GLogger::instance().logTrace("I. Operation tree (sort by position):"_i18n);
    tr.output();
    GLogger::instance().logDebug(tr.display());
    
    // II pass - fill with lexemes
    for(auto& lex: lexemes)
    {
        tr.insert(lex);
    }
    GLogger::instance().logTrace("II. Final tree (sort by position):"_i18n);
    tr.output();
    GLogger::instance().logDebug(tr.display());
    //
    GLogger::instance().logTrace("Appling transformations:"_i18n);
    return tr.transform();	// apply transformation
}

size_t CTex::match_index(std::sregex_iterator it)
{
    size_t index = 0;
    for (; index < it->size(); ++index) {
        if (!it->str(index + 1).empty()) {
            // already matched
            break;
        }
    }
    return index;
}

std::vector<std::string> CTex::lexical_analyzer(const std::string& in)
{
    std::vector<std::string> tokens;
    grouped_hits_.clear();
    try {
        std::regex re(regex_txt_);
        auto begin = std::sregex_iterator(in.begin(), in.end(), re);
        auto end  = std::sregex_iterator();
        for (auto it = begin; it != end; ++it)
        {
            size_t index = match_index(it);
            auto group = grouped_regs_[index].second;
            GLogger::instance().logDebug("\t", it->str(), "\t", group);
            ++grouped_hits_[group];
            tokens.push_back(it->str());
        }
    }
    catch (std::regex_error& ex)
    {
        GLogger::instance().logError(ex.what());
    }
    GLogger::instance().logDebug("Statistics:"_i18n);
    for (auto& d : grouped_hits_)
    {
        GLogger::instance().logDebug("\t", d.first, "\t", d.second);
    }
    return tokens;
}

std::string CTex::eq_open_tag(CTex::EQUATION_TAG_STYLE style)
{
    switch (style) {
        case DISPLAY:   return R"!($$ )!";
        case INLINE:    return R"!($ )!";
        case DOXYFILE:  return R"!(\f$ )!";
    }
}

std::string CTex::eq_close_tag(CTex::EQUATION_TAG_STYLE style)
{
    switch (style) {
        case DISPLAY:   return R"!( $$)!";
        case INLINE:    return R"!( $)!";
        case DOXYFILE:  return R"!( \f$)!";
    }
}
