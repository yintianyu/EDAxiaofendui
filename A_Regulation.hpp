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
	static int pieceStarts_8bit[8] ;
	static int minInterval_8bit[8] ;

	static int pieceStarts_16bit[8] ;
	static int minInterval_16bit[8] ;

    A_Regulation(){}
	~A_Regulation(){}
	// 8 bit compress() and decompress()
	//// !! isn't inherited from Regulation
    void compress(const std::vector<original_data> &input_diff, original_data max, std::vector<compressed_diff_8bit> &output_compressed_diff);
    void decompress(const std::vector<compressed_diff_8bit> &input_diff, original_data max, std::vector<original_data> &output_original_data);
	// 16bit compress() and decompress()
	// inherited from Regulation
	void compress(const std::vector<original_data> &input_diff, original_data max, std::vector<compressed_diff> &output_compressed_diff);
    void decompress(const std::vector<compressed_diff> &input_diff, original_data max, std::vector<original_data> &output_original_data);

};

#endif
