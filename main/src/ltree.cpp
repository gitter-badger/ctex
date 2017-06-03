/**
 * @file ltree.cpp
 * @date 03.06.17
 * @author galarius
 * @copyright   Copyright © 2017 galarius. All rights reserved.
 */

#include "ltree.hpp"
#include "glogger.hpp"

#include <algorithm>

LexemeTree::LexemeTree()
: depth_(0),
  root_(nullptr)
{  }

std::string LexemeTree::transform()
{
    if (!root_)
        return std::string();
    std::string res = transform(root_);
    return res;
}

void LexemeTree::insert(const Lexeme& lex)
{
    if (!root_) {
        std::unique_ptr<TreeNode> node(new TreeNode(lex));
        root_ = std::move(node);
    } else {
        insert(root_, lex);  // recursive lexeme addition
    }
}

void LexemeTree::save_parenthesis_pos(const Lexeme& lexeme)
{
    if(lexeme.type() == LexemeLibrary::bracketl)
    {
        lbrackets_pos_.insert(lexeme.pos());
    }
    else if(lexeme.type() == LexemeLibrary::bracketr)
    {
        rbrackets_pos_.insert(lexeme.pos());
    }
    else
    {
        GLogger::instance().logError("Wrong bracket type!");
    }
}

std::string LexemeTree::display()
{
    std::string str;
    std::vector<std::string> strs(2 * (depth_+1) + 1);
    for(auto&s : strs)
    {
        s = std::string(255, ' ');
    }
    
    display(root_, false, 0, 0, strs);
    
    for(auto&s : strs)
    {
        if(!s.empty())
        {
            str.append("\n");
            str.append(s);
        }
    }
    return str;
}

void LexemeTree::output()
{
    output(root_);
}

std::string LexemeTree::transform(const std::unique_ptr<TreeNode> &node)
{
    std::string result;
    do
    {
        if(!node)
            break;
        
        Lexeme& lex = node->data;
        
        GLogger::instance().logTrace("\t", lex.lexeme(), " ", (node->left ? node->left->data.lexeme() : ""), " ", (node->right ? node->right->data.lexeme() : ""));
        
        if (!LexemeLibrary::is_toperator( lex.type() ))
        {
            result = LexemeLibrary::apply_default_transform(lex.lexeme());
            break;
        }
        
        bool in_parenthesis = node->left && lbrackets_pos_.find(node->left->data.pos()-1) != lbrackets_pos_.end() &&
        node->right && rbrackets_pos_.find(node->right->data.pos()+1) != rbrackets_pos_.end();

        result = LexemeLibrary::apply_transform(
                                                lex,
                                                transform(node->left),
                                                transform(node->right),
                                                in_parenthesis);
        
    } while (false);
    return result;
}

void LexemeTree::insert(std::unique_ptr<TreeNode> &node, const Lexeme& lex, unsigned int depth)
{
    if (node)
    {
        if (lex.pos() < node->data.pos())
        {
            insert(node->left, lex, depth + 1);
        }
        else if (lex.pos() > node->data.pos())
        {
            insert(node->right, lex, depth + 1);
        }
    }
    else
    {
        std::unique_ptr<TreeNode> lx(new TreeNode( lex ));
        node = std::move(lx);
        if(depth >= depth_)
            depth_ = depth;
    }
}

void LexemeTree::output(const std::unique_ptr<TreeNode> &node)
{
    if (!node)
        return;
    
    output(node->left);
    GLogger::instance().logTrace("\t", node->data.lexeme(), " pos: ", node->data.pos(), " priority: ", node->data.priority());
    output(node->right);
}

int LexemeTree::display(const std::unique_ptr<TreeNode> &node, bool is_left, int offset, unsigned int depth, std::vector<std::string>& s)
{
    //-------------------------------------------------------
    auto s_assign = [](std::string& s, int index, char val) {
        index = std::max(0, index);
        if ((size_t)index >= s.size()) {
            s.resize(index + 1);
        }
        s[index] = val;
    };
    //-------------------------------------------------------
    
    if (!node)
        return 0;
    
    std::string b = "(" + node->data.lexeme() + ")";
    int width = (int)b.size();
    
    int left  = display(node->left,  true,  offset,                depth + 1, s);
    int right = display(node->right, false, offset + left + width, depth + 1, s);
    
    try {
        
        for (int i = 0; i < width; ++i)
        {
            s_assign(s.at(2 * depth), offset + left + i, b[i]);
        }
        
        if (depth && is_left)
        {
            for (int i = 0; i < width + right; ++i)
            {
                s_assign(s.at(2 * depth - 1), offset + left + width / 2 + i, '-');
            }
            
            s_assign(s.at(2 * depth - 1), offset + left + width / 2, '+');
            s_assign(s.at(2 * depth - 1), offset + left + width + right + width / 2, '+');
            
        }
        else if (depth && !is_left)
        {
            for (int i = 0; i < left + width; ++i)
            {
                s_assign(s.at(2 * depth - 1), offset - width / 2 + i, '-');
            }
            
            s_assign(s.at(2 * depth - 1), offset + left + width / 2, '+');
            s_assign(s.at(2 * depth - 1), offset - width / 2 - 1, '+');
        }
        
    } catch (std::out_of_range& e) {
        
        GLogger::instance().logError("offset: ", offset, " depth: ", depth, " is_left: ", is_left, " reason: ", e.what());
    }
    
    return left + width + right;
}
