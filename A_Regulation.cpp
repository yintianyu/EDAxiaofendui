/*
 * File: A_Regulation.cpp
 * Author: Steven Zhang
 * Create Date: 2019/10/05
 * Description: Implement the class A_Regulation
 */

#include <cmath>
#include "A_Regulation.hpp"

int A_Regulation::pieceStarts_8bit[8] = {0,16,32,64,128,256,512,1024};
int A_Regulation::minInterval_8bit[8] = {1, 1, 2, 4,  8, 16, 32,  64};

int A_Regulation::pieceStarts_16bit[16] = {0,4096,8192,16384,32768,65536,131072,262144};
int A_Regulation::minInterval_16bit[16] = {1,   1,   2,    4,    8,   16,    32,    64};

// 8 bit compress() and decompress()
// !! isn't inherited from Regulation
void A_Regulation::compress(const std::vector<original_data> &input_diff, original_data max,
                                  std::vector<compressed_diff_8bit> &output_compressed_diff){

    int whichPiece;
	int loc;
	output_compressed_diff.clear();

    std::vector<original_data>::const_iterator it_in;
	for(it_in=input_diff.begin();it_in !=input_diff.end(); it_in++)
    {
		std::bitset<8> bitCode;      //default：every bit is 0;
        bitCode[7] = ( (*it_in)>=0 )? 1 : 0;
        original_data quantify_double = fabs( *it_in)/max *2048;
		int quantify = floor(quantify_double);
		if(quantify >=128){
			bitCode[6]=1;
			if(quantify >=512){
				bitCode[5]=1;
				bitCode[4] = (quantify >=1024)? 1: 0;
			}else{
				bitCode[5]=0;
				bitCode[4] = (quantify >=256)?  1: 0;
			}
		}else{
			bitCode[6]=0;
			if(quantify >=32){
				bitCode[5]=1;
				bitCode[4] = (quantify >=64)? 1: 0;
			}else{
				bitCode[5]=0;
				bitCode[4] = (quantify >=16)?  1: 0;
			}
		}
		whichPiece = int( ( (bitCode<<1) >>5 ).to_ulong() ) ;
		loc= floor( (quantify-pieceStarts_8bit[whichPiece])/minInterval_8bit[whichPiece] );
		if(loc == 16)
			loc=loc-1;
		std::bitset<8> innerCode(loc);
		bitCode = bitCode| innerCode;
		output_compressed_diff.push_back( bitCode);
    }
}

// 16bit compress() and decompress()
// inherited from Regulation
void A_Regulation::compress(const std::vector<original_data> &input_diff, original_data max,
                                  std::vector<compressed_diff> &output_compressed_diff){

    int whichPiece;
	int loc;
	output_compressed_diff.clear();

    std::vector<original_data>::const_iterator it_in;
	for(it_in=input_diff.begin();it_in !=input_diff.end(); it_in++)
    {
		std::bitset<16> bitCode;      //default：every bit is 0;
        bitCode[15] = ( (*it_in)>=0 )? 1 : 0;
        original_data quantify_double = fabs( *it_in)/max *524288;
		int quantify = floor(quantify_double);
		if(quantify >=32768){
			bitCode[14]=1;
			if(quantify >=131072){
				bitCode[13]=1;
				bitCode[12] = (quantify >=262144)? 1: 0;
			}else{
				bitCode[13]=0;
				bitCode[12] = (quantify >=65536)?  1: 0;
			}
		}else{
			bitCode[14]=0;
			if(quantify >=8192){
				bitCode[13]=1;
				bitCode[12] = (quantify >=16384)? 1: 0;
			}else{
				bitCode[13]=0;
				bitCode[12] = (quantify >=4096)?  1: 0;
			}
		}
		whichPiece = int( ( (bitCode<<1) >>13 ).to_ulong() ) ;
		loc= floor( (quantify-pieceStarts_16bit[whichPiece])/minInterval_16bit[whichPiece] );
		if(loc == 4096)
			loc=loc-1;
		std::bitset<16> innerCode(loc);
		bitCode = bitCode| innerCode;
		output_compressed_diff.push_back( bitCode);
    }
}

// 8bit compress() and decompress()
// !! isn't inherited from Regulation
void A_Regulation::decompress(const std::vector<compressed_diff_8bit> &input_diff, original_data max,
                                    std::vector<original_data> &output_original_data){

	int whichPiece;
	int loc;
	original_data odata;
	std::bitset<8> bitCode;
	output_original_data.clear();
	std::vector<compressed_diff_8bit>::const_iterator it_in;
	for(it_in=input_diff.begin(); it_in !=input_diff.end(); it_in++){
		bitCode = *it_in;
        whichPiece = int( ( (bitCode<<1) >>5 ).to_ulong() ) ;
		loc        = int( ( (bitCode<<4) >>4 ).to_ulong() ) ;
		odata =(pieceStarts_8bit[whichPiece]+(loc+0.5)*minInterval_8bit[whichPiece])/2048*max;
		if(bitCode[7] ==0){
			odata = -odata;
		}
		output_original_data.push_back(odata);
	}
}

// 16bit compress() and decompress()
// inherited from Regulation
void A_Regulation::decompress(const std::vector<compressed_diff> &input_diff, original_data max,
                                    std::vector<original_data> &output_original_data){

	int whichPiece;
	int loc;
	original_data odata;
	std::bitset<16> bitCode;
	output_original_data.clear();
	std::vector<compressed_diff>::const_iterator it_in;
	for(it_in=input_diff.begin(); it_in !=input_diff.end(); it_in++){
		bitCode = *it_in;
        whichPiece = int( ( (bitCode<<1) >>13 ).to_ulong() ) ;
		loc        = int( ( (bitCode<<4) >>4 ).to_ulong() ) ;
		odata =(pieceStarts_16bit[whichPiece]+(loc+0.5)*minInterval_16bit[whichPiece])/524288*max;
		if(bitCode[15] ==0){
			odata = -odata;
		}
		output_original_data.push_back(odata);
	}
}
