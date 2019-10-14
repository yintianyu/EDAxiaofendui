/*
 * File: Regulation.hpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/06
 * Description: Define virtual base class Regulation
 */

#ifndef REGULATION_HPP
#define REGULATION_HPP

#include <vector>
#include "datatype.hpp"


class Regulation{
    public:
    Regulation(){

    }
    // Virtual Function: compress
    // Input: double input_diff
    // Output: compressed_diff
    virtual compressed_diff compress(std::vector<original_data> &input_diff, original_data max);
    // Virtual Function: decompress
    // Input: compressed_diff input_diff
    // Output: original_data
    virtual original_data decompress(std::vector<compressed_diff> input_diff, original_data max);
    virtual ~Regulation(){}
};

#endif