/**
 * @file utils.hpp
 * @date 03.06.17
 * @author galarius
 * @copyright   Copyright © 2017 galarius. All rights reserved.
 */

#ifndef utils_hpp
#define utils_hpp

#include <string>
#include <vector>

inline bool starts_with(const std::string& s, const std::string& prefix)
{
    return (s.size() >= prefix.size()) &&
    equal(prefix.begin(), prefix.end(), s.begin());
}

inline bool ends_with(const std::string& s, const std::string& suffix) {
    
    return (s.size() >= suffix.size()) &&
    equal(suffix.rbegin(), suffix.rend(), s.rbegin());
    
}

inline bool find(const std::string& a, const std::string& b)
{
    return a.find(b) != std::string::npos;
}

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
