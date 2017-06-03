/**
 * @file ltree.hpp
 * @date 03.06.17
 * @author galarius
 * @copyright   Copyright © 2017 galarius. All rights reserved.
 * @brief Lexeme Tree
 */

#ifndef ltree_hpp
#define ltree_hpp

#include "lexeme.hpp"
#include <iostream>
#include <set>

/**
 * @brief Lexeme Tree
 *
 * Lexeme tree build algorithm:
 *  I) Partial tree contains only lexemes considered to be transform operators, like functions, operations...
 *  II) Tree extension by using only lexeme positions relatively to transform operators in tree
 */
class LexemeTree
{
    /**
     * @brief The TreeNode struct
     */
    struct TreeNode {
        /**
         * @brief lexeme
         * @see Lexeme
         */
        Lexeme data;
        std::unique_ptr<TreeNode> left;     ///< smart pointer to the left child
        std::unique_ptr<TreeNode> right;    ///< smart pointer to the right child
        TreeNode() :
        left(nullptr)
        , right(nullptr)
        { }
        explicit TreeNode(const Lexeme& value) :
        data(value)
        ,left(nullptr)
        ,right(nullptr)
        { }
    };
    
public:
    LexemeTree();
    ~LexemeTree() = default;
public:
    /**
     * @brief Perform transformation
     * @return transformation result
     */
    std::string transform();
    /**
     * @brief Insert lexeme into tree
     * @param lexeme lexeme to insert
     * @see Lexeme
     */
    void insert(const Lexeme& lexeme);
    
    /**
     @brief Save bracket position
     @param lexeme bracket
     */
    void save_bracket_pos(const Lexeme& lexeme);
    /**
     * @brief Display tree
     * @return tree structure in std::string
     */
    std::string display();
    /**
     * @brief Print list of nodes properties
     */
    void output();
private:
    /**
     * @brief Recursive transformation
     * @param node nodal lexeme
     * @return transformation result
     */
    std::string transform(const std::unique_ptr<TreeNode>& node);
    /**
     * @brief Recursive insert
     * @param node nodal lexeme
     * @param lexeme lexeme to insert
     * @param depth  current depth
     */
    void insert(std::unique_ptr<TreeNode> &node, const Lexeme& lexeme, unsigned int depth = 0);
    /**
     * @brief Recursive display
     * @param node nodal lexeme
     * @param is_left indicates whether node is the left child of its parent
     * @param offset  offset in a string line
     * @param depth   current depth
     * @param s       string container to draw tree in
     * @return new offset
     */
    int display(const std::unique_ptr<TreeNode> &node, bool is_left, int offset, unsigned int depth, std::vector<std::string>& s);
    /**
     * @brief Recursive tree print
     * @param node nodal lexeme
     */
    void output(const std::unique_ptr<TreeNode> &node);
private:
    unsigned int depth_;               ///< tree depth
    std::unique_ptr<TreeNode> root_;   ///< tree root
    std::set<int> lbrackets_pos_;
    std::set<int> rbrackets_pos_;
};

#endif /* ltree_hpp */
