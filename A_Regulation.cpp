/*
 * File: A_Regulation.cpp
 * Author: Steven Zhang
 * Create Date: 2019/10/05
 * Description: Implement the class A_Regulation
 */

#include <cmath>
#include "A_Regulation.hpp"

int A_Regulation::pieceStarts[8] = {0,16,32,64,128,256,512,1024};
int A_Regulation::minInterval[8] = {1, 1, 2, 4,  8, 16, 32,  64};

void A_Regulation::compress(const std::vector<original_data> &input_diff, original_data max, 
                                  std::vector<compressed_diff> &output_compressed_diff){

    int whichPiece;
	int loc;	
	output_compressed_diff.clear();
	
    std::vector<original_data>::const_iterator it_in;
	for(it_in=input_diff.begin();it_in !=input_diff.end(); it_in++)
    {
		std::bitset<8> bitCode;      //default：every bit is 0;
        bitCode[7] = ( (*it_in)>=0 )? 1 : 0;
        original_data quantify_double = fabs( *it_in)/max *2048;
		int quantify = floor(quanti_double);
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
		loc= floor( (quantify-pieceStarts[whichPiece])/minInterval[whichPiece] );
		if(loc == 16)    
			loc=loc-1;
		std::bitset<8> innerCode(loc);
		bitCode = bitCode| innerCode;
		output_compressed_diff.push_back( bitCode);
    }
}

void A_Regulation::decompress(const std::vector<compressed_diff> &input_diff, original_data max, 
                                    std::vector<original_data> &output_original_data){
    
	int whichPiece;
	int loc;
	original_data odata;
	std::bitset<8> bitCode;
	output_original_data.clear();
	std::vector<compressed_diff>::const_iterator it_in;
	for(it_in=input_diff.begin(); it_in !=input_diff.end(); it_in++){
		bitCode = *it_in;
        whichPiece = int( ( (bitCode<<1) >>5 ).to_ulong() ) ;
		loc        = int( ( (bitCode<<4) >>4 ).to_ulong() ) ;
		odata =(pieceStarts[whichPiece]+(loc+0.5)*minInterval[whichPiece])/2048*max;
		if(bitCode[7] ==0){
			odata = -odata;
		}
		output_original_data.push_back(odata);
	}
}