/**
 * @file detector.cpp
 * @date 03.06.17
 * @author galarius
 * @copyright   Copyright © 2017 galarius. All rights reserved.
 * @brief C formula detection in stream
 */

#include "detector.hpp"
#include "utils.hpp"
#include "glogger.hpp"
#include "i18n.hpp"

Detector::Detector(std::shared_ptr<CTex> ctex) :
min_op_count_(0)
, min_fn_count_(0)
, ctex_(ctex)
{ }

void Detector::perform(std::ifstream& in, std::ofstream& out)
{
    bool in_formula = false;
    bool in_comment = false;
    bool skip = false;
    static const std::string oc = "/*";
    static const std::string cc = "*/";
    static const std::string ic = "//";
    std::string::size_type pos_oc;
    std::string::size_type pos_cc;
    std::string formula;
    std::string line;
    
    auto should_skip = [](const std::string& line) -> bool
    {
        return line.empty() || line == " " || line == "\t";
    };

    
    for (;!in.eof() && getline(in, line);)
    {
        // erase comments
        skip = false;
        do
        {
            pos_oc = line.find(oc);
            pos_cc = line.find(cc);
            if (pos_oc != std::string::npos ||
                pos_cc != std::string::npos)
            {
                // `.../*`  `.../* */...`
                while (pos_oc != std::string::npos ||
                       pos_cc != std::string::npos)
                {
                    if (pos_oc != std::string::npos &&
                        pos_cc != std::string::npos)
                    {
                        // /* ... */
                        line.erase(pos_oc, pos_cc + cc.length() - pos_oc);
                    }
                    else if (pos_oc != std::string::npos &&
                             pos_cc == std::string::npos)
                    {
                        // /* ...
                        line.erase(pos_oc);
                        in_comment = true;
                    }
                    else if (pos_oc == std::string::npos &&
                             pos_cc != std::string::npos)
                    {
                        // ... */
                        line.erase(0, pos_cc + cc.length());
                        in_comment = false;
                    }
                    
                    pos_oc = line.find(oc);
                    pos_cc = line.find(cc);
                    
                }
                skip = should_skip(line);
            }
            else if (line.find(ic) != std::string::npos)
            {	
                // `...//`
                line = line.substr(0, line.find(ic));
                skip = should_skip(line);
            }
        } while (false);
        //
        
        if (!in_comment && !skip)
        {
            if (find(line, "if") || find(line, "else"))
            {
                out << line << std::endl;
                continue;
            }
            
            if (find(line, "=") && !find(line, "for") && !find(line, "while"))
            {
                in_formula = true;
            }
            
            if (in_formula) {
                formula.append(line);
                if (find(formula, ";")) {
                    process(formula, out);
                    out << formula << std::endl;
                    in_formula = false;
                    formula = std::string();
                }
                continue;
            }
        }
        
        out << line << std::endl;
    }
}

void Detector::set_filter(int min_op_count, int min_fn_count)
{
    min_op_count_ = min_op_count;
    min_fn_count_ = min_fn_count;
}

void Detector::process(const std::string& formula, std::ofstream& stream)
{
    static const std::string id = "CTEX";
    std::string res, log;
    GLogger::instance().start_record();
    GLogger::instance().logInfo("Input: "_i18n, formula);
    res = ctex_->translate(formula);
    GLogger::instance().logInfo("Output:"_i18n, res, "\n");
    log = GLogger::instance().end_record();
    // apply filter
    if (ctex_->group_hits("operator") > min_op_count_ ||
        ctex_->group_hits("function") > min_fn_count_)
    {
        stream << std::endl << "/** " << id << std::endl;
        stream << log;
        stream << "*/" << std::endl;
    }
}
