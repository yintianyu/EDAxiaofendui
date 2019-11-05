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
	virtual ~Regulation(){}
    // Virtual Function: compress
    // Input: vector<original_data> input_diff, original_data max, [output] std::vector<compressed_diff> output_compressed_diff
    // Output: None
    virtual void compress(const std::vector<original_data> &input_diff, original_data max, std::vector<compressed_diff> &output_compressed_diff)=0;
    // Virtual Function: decompress
    // Input: vector<compressed_diff> input_diff, original_data max,  [output] std::vector<compressed_diff> output_compressed_diff
    // Output: NOne
    virtual void decompress(const std::vector<compressed_diff> &input_diff, original_data max, std::vector<original_data> &output_original_data)=0;
    
};

#endif