/**
 * @file i18n.hpp
 * @date 08.05.17
 * @author Galarius
 * @copyright Copyright (c) 2017 galarius. All rights reserved.
 * @brief internalization
 */

#ifndef ctex_i18n_hpp
#define ctex_i18n_hpp

#include <string>

/**
 * @brief C++11 User-defined literal for internalization
 */
inline std::string operator "" _i18n(const char* str, size_t /*length*/)
{
    std::string tr(str);
    return tr;
}

#endif  // ctex_i18n_hpp
