/*
 * File: Regulation.hpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/06
 * Description: Define virtual base class Regulation
 */

#ifndef REGULATION_HPP
#define REGULATION_HPP

#include "datatype.hpp"

typedef char compressed_diff; // Can be modify to short if 16 bits are needed

class Regulation{
    public:
    Regulation(){

    }
    // Virtual Function: compress
    // Input: double input_diff
    // Output: compressed_diff
    virtual compressed_diff compress(original_data input_diff);
    // Virtual Function: decompress
    // Input: compressed_diff input_diff
    // Output: original_data
    virtual original_data decompress(compressed_diff input_diff);
    virtual ~Regulation(){}
};

#endif