/*
 * File: A_Regulation.hpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/06
 * Description: Define class A_Regulation
 * Modifier: Steven Zhang
 * Modify Date: 2019/10/14
 */

#ifndef A_REGULATION_HPP
#define A_REGULATION_HPP

#include "Regulation.hpp"

class A_Regulation : public Regulation{
    public:

    A_Regulation(){}
	~A_Regulation(){}
	
	// compress() and decompress() for any bit_width data.
	// inherited from Regulation
	void compress(const std::vector<original_data> &input_diff, original_data max, std::vector<compressed_diff> &output_compressed_diff);
    void decompress(const std::vector<compressed_diff> &input_diff, original_data max, std::vector<original_data> &output_original_data);

};

#endif
