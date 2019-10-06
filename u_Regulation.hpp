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
    compressed_diff compress(original_data input_diff);
    original_data decompress(compressed_diff input_diff);
    ~u_Regulation(){}
};

#endif