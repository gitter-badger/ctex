/**
 * @file utils.hpp
 * @date 03.06.17
 * @author galarius
 * @copyright Copyright © 2017 galarius. All rights reserved.
 * @brief String helpers
 */

#ifndef utils_hpp
#define utils_hpp

#include <string>
#include <vector>

namespace str
{
    /**
     Checks if string starts with provided prefix

     @param[in] str string to check
     @param[in] prefix prefix to test at the beginning of the str
     @return true if str starts with prefix
     */
    inline bool starts_with(const std::string& s, const std::string& prefix)
    {
        return (s.size() >= prefix.size()) &&
        equal(prefix.begin(), prefix.end(), s.begin());
    }
    /**
     Checks if string ends with provided suffix
     
     @param[in] str string to check
     @param[in] suffix suffix to test at the end of the str
     @return true if str ends with suffix
     */
    inline bool ends_with(const std::string& s, const std::string& suffix)
    {
        return (s.size() >= suffix.size()) &&
        equal(suffix.rbegin(), suffix.rend(), s.rbegin());
    }
    /**
     Shortcut for the string's find method

     @param str container string
     @param pattern pattern to test
     @return true if pattern occured in str
     */
    inline bool find(const std::string& s, const std::string& pattern)
    {
        return s.find(pattern) != std::string::npos;
    }
    /**
     Split text into tokens

     @param text text to split
     @param sep separator
     @return tokens
     */
    inline std::vector<std::string> split(const std::string &text, std::string sep)
    {
        std::vector<std::string> tokens;
        std::size_t start = 0, end = 0;
        while ((end = text.find(sep, start)) != std::string::npos) {
            tokens.push_back(text.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(text.substr(start));
        return tokens;
    }
    inline std::string remove_first(const std::string &s, const std::string &substring)
    {
        std::string res(s);
        auto it = res.find(substring);
        res.erase(it, substring.length());
        return res;
    }
    inline std::string remove_last(const std::string &s, const std::string &substring)
    {
        std::string res(s);
        auto it = res.rfind(substring);
        res.erase(it, substring.length());
        return res;
    }
    inline void ltrim(std::string &s) {
            s.erase(s.begin(),
                    find_if(s.begin(), s.end(),
                    std::not1(std::ptr_fun<int, int>(std::isspace))));
    }
    inline void rtrim(std::string &s) {
        s.erase(find_if(s.rbegin(), s.rend(),
                std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
                s.end());
    }
    inline void trim(std::string& s) {
        ltrim(s);
        rtrim(s);
    }
    inline std::string ltrimmed(std::string s) {
        ltrim(s);
        return s;
    }
    inline std::string rtrimmed(std::string s) {
        rtrim(s);
        return s;
    }
    inline std::string trimmed(std::string s) {
        ltrim(s);
        rtrim(s);
        return s;
    }
}

#endif /* utils_hpp */
