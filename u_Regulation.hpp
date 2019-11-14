/*
 * File: u_Regulation.hpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/06
 * Description: Define class A_Regulation
 */

#ifndef U_REGULATION_HPP
#define U_REGULATION_HPP

#include "Regulation.hpp"

class u_Regulation : public Regulation{
    public:
    u_Regulation(){}
    void compress(const std::vector<original_data> &input_diff, original_data max, std::vector<compressed_diff> &output_compressed_diff);
    void decompress(const std::vector<compressed_diff> &input_diff, original_data max, std::vector<original_data> &output_original_data);
    ~u_Regulation(){}
};

#endif