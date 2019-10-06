/*
 * File: A_Regulation.hpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/06
 * Description: Define class A_Regulation
 */

#ifndef A_REGULATION_HPP
#define A_REGULATION_HPP

#include "Regulation.hpp"

class A_Regulation : public Regulation{
    public:
    A_Regulation(){}
    compressed_diff compress(original_data input_diff);
    original_data decompress(compressed_diff input_diff);
    ~A_Regulation(){}
};

#endif