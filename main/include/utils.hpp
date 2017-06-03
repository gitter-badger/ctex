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

/**
 Checks if string starts with provided prefix

 @param[in] str string to check
 @param[in] prefix prefix to test at the beginning of the str
 @return true if str starts with prefix
 */
inline bool starts_with(const std::string& str, const std::string& prefix)
{
    return (str.size() >= prefix.size()) &&
    equal(prefix.begin(), prefix.end(), str.begin());
}
/**
 Checks if string ends with provided suffix
 
 @param[in] str string to check
 @param[in] suffix suffix to test at the end of the str
 @return true if str ends with suffix
 */
inline bool ends_with(const std::string& str, const std::string& suffix)
{
    return (str.size() >= suffix.size()) &&
    equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}
/**
 Shortcut for the string's find method

 @param str container string
 @param pattern pattern to test
 @return true if pattern occured in str
 */
inline bool find(const std::string& str, const std::string& pattern)
{
    return str.find(pattern) != std::string::npos;
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

#endif /* utils_hpp */
