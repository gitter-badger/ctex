/**
 * @file lexeme.hpp
 * @date 03.06.17
 * @author galarius
 * @copyright   Copyright © 2017 galarius. All rights reserved.
 * @brief Lexeme description and library
 */

#ifndef lexeme_hpp
#define lexeme_hpp

#include <string>
#include <vector>
#include <unordered_map>

class Lexeme;

/**
 * @class LexemeLibrary
 * @brief The library of supported lexemes
 */
class LexemeLibrary
{
public:
    /**
     * @brief Lexeme type
     */
    enum Type {
        ///@{
        bracketl,   ///< (
        bracketr,   ///< )
        index,      ///< []
        function,
        operation,
        variable,
        unknown,
        placeholder
        ///@}
    };
    /**
     * @brief Lexeme's info
     * <lexeme, <type, priority>>
     */
    using LexData = std::pair<std::string, std::pair<Type, int>>;
public:
    LexemeLibrary() = delete;
    ~LexemeLibrary() = default;
public:
    /**
     * @brief Add lexeme to library
     * @param[in] lex lexeme as string
     * @param[in] type lexeme type
     * @param[in] priority base priority
     */
    static void add_lexeme(const std::string& lex, Type type, int priority);
    /**
     * @brief Get lexemes of selected type
     * @param type lexeme type
     */
    static std::vector<std::string> get_lexemes(Type type);
    /**
     * @brief Checks whether lexeme is supported
     * @param lex lexeme to be checked
     * @return result
     */
    static bool is_supported(const std::string& lex);
    /**
     * @brief Checks whether type considered to be a transform operator
     * @param type type to be checked
     * @return result
     */
    static bool is_toperator(LexemeLibrary::Type type);
    /**
     * @brief Get lexeme's type
     * @param lex lexeme
     * @return lexeme's type from Type enumeration
     * @see Type
     */
    static Type get_type(const std::string& lex);
    /**
     * @brief Get lexeme's priority
     * @param lex lexeme
     * @return lexeme's base priority
     */
    static int get_priority(const std::string& lex);
public:
    /**
     * @brief Max priority level
     */
    static int max_priority;
private:
    /**
     * @brief Supported lexemes
     * <lexeme, <type , priority>>
     */
    static std::vector<LexData> lex_library;
};


/**
 * @brief Lexeme's description: type, position in expression, priority
 */
class Lexeme {
public:
    Lexeme();
    ~Lexeme() = default;
    
    explicit Lexeme(const std::string& lexeme);
    explicit Lexeme(Lexeme const * const _lex);
    Lexeme(const std::string& lexeme, int pos);
    
    /**
     * @brief Set lexeme
     * @param[in] lexeme lexeme to set
     */
    void set_lexeme(const std::string& lexeme);
    
    /**
     * @brief Set lexeme and it's position in an expression
     * @param[in] lexeme lexeme to set
     * @param[in] position lexeme's position in expression
     */
    void set_lexeme(const std::string& lexeme, int position);
    
    /**
     * @brief Set position in expression
     * @param[in] position lexeme's position in expression
     */
    void set_position(int position);
    /**
     * @brief Get lexeme as string
     */
    std::string lexeme() const;
    /**
     * @brief Get lexeme's type
     * @see Type
     */
    LexemeLibrary::Type type() const;
    /**
     * @brief Get Lexeme's priority
     */
    int priority() const;
    /**
     * @brief Get lexeme's position in expression
     * @see Type
     */
    int pos() const;
    
    /**
     * @brief Update lexeme's priority
     * @param level nesting level
     */
    void update_priority(int level);
    
    /**
     * @name Overloading Comparison Operators
     */
    bool operator< (const Lexeme& lex) const;
    bool operator> (const Lexeme& lex) const;
private:
    std::string lexeme_;        ///< @brief string representation
    int position_;				///< @brief lexeme's position in expression
    int priority_;				///< @brief lexeme's priority
    LexemeLibrary::Type type_;  ///< @brief lexems's type
};

#endif /* lexeme_hpp */
