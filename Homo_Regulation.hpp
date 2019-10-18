/*
 * File: Homo_Regulation.hpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/06
 * Description: Define class Homo_Regulation
 */

#ifndef HOMO_REGULATION_HPP
#define HOMO_REGULATION_HPP

#include "Regulation.hpp"

class Homo_Regulation : public Regulation{
    public:
    Homo_Regulation(){}
	~Homo_Regulation(){}
    void compress(const std::vector<original_data> &input_diff, original_data max, std::vector<compressed_diff> &output_compressed_diff);
    void decompress(const std::vector<compressed_diff> &input_diff, original_data max, std::vector<original_data> &output_original_data);
    
};

#endif