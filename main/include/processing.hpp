/**
 * @file processing.hpp
 * @date 08.06.17
 * @author galarius
 * @copyright Copyright © 2017 galarius. All rights reserved.
 * @brief Lexeme transformation logic
 */

#ifndef processing_h
#define processing_h

#include "lexeme.hpp"

#include <string>

class Processing
{
public:
    Processing() = delete;
    ~Processing() = default;
    
    /**
     * @brief Apply transformation
     * @param[in] toperator transform operator from the list of supported
     * @param[in] a expression 1
     * @param[in] b expression 2
     * @param[in] in_parenthesis whether result should be should be wrapped in parenthesis
     * @return transformation result
     */
    static std::string apply_transform(const Lexeme& toperator, const std::string& a, const std::string& b, bool in_parenthesis = false);
    /**
     * @brief Apply default transform
     * @param expression expression to be transformed
     * @return transformation result
     */
    static std::string apply_default_transform(const std::string& expression);
private:
    /**
     * @brief LaTeX tags to perform transformations from C syntax to LaTeX
     */
    static std::unordered_map<std::string, std::string> tags;
};

#endif /* processing_h */
