/*
 * File: A_Regulation.cpp
 * Author: Steven Zhang
 * Create Date: 2019/10/05
 * Modified Date: 2019/11/12
 * Description: Implement the class A_Regulation
 */

#include <cmath>
#include "A_Regulation.hpp"

// compress() and decompress() for any bit_width data.
// inherited from Regulation
void A_Regulation::compress(const std::vector<original_data> &input_diff, original_data max,
                                  std::vector<compressed_diff> &output_compressed_diff){
	if(max < 0){
		return ;
	}

    compressed_diff t;
    short s = t.size();
	output_compressed_diff.clear();
	unsigned int maxVal = 1 << (s-1);

    std::vector<original_data>::const_iterator it_in;
	for(it_in=input_diff.begin();it_in !=input_diff.end(); it_in++)
    {
		compressed_diff bitCode;      //default：every bit is 0;
        original_data normDiff = fabs( ( *it_in)/max );
		if (normDiff >= 1.0 /2) {
            bitCode = normDiff == 1 ? (maxVal-1) :( maxVal*7.0/8 +  maxVal*(normDiff - 1.0/2) * (1.0/4) ) ;
        }
        else if (normDiff >= 1.0 /4) {
            bitCode = maxVal*6.0/8 +  maxVal*(normDiff - 1.0/4) * (1.0/2);
        }
        else if (normDiff >= 1.0 /8) {
            bitCode = maxVal*5.0/8 +  maxVal *(normDiff - 1.0/8) *1 ;
        }
        else if (normDiff >= 1.0 /16) {
            bitCode = maxVal*4.0 /8 +  maxVal *(normDiff - 1.0/16) *2 ;
        }
        else if (normDiff >= 1.0 /32) {
            bitCode = maxVal*3.0/8 +  maxVal *(normDiff - 1.0/32) *4 ;
        }
        else if (normDiff >= 1.0 /64) {
            bitCode = maxVal*2.0/8 +  maxVal *(normDiff - 1.0/64) *8 ;
        }
        else if (normDiff >= 1.0 /128) {
            bitCode = maxVal*1.0/8 +  maxVal *(normDiff - 1.0/128) *16 ;
        }
        else {
            bitCode =               maxVal* normDiff * 16;
        }
        bitCode[s-1] = (*it_in < 0) ? 1 : 0;
        output_compressed_diff.push_back(bitCode);
    }
}

void A_Regulation::decompress(const std::vector<compressed_diff> &input_diff, original_data max,
                                    std::vector<original_data> &output_original_data){

	if(max < 0){
		return ;
	}
	compressed_diff t;
    char s = t.size();
	double half_delta_compensate_y = 1.0/8 / (1<<(s-4))  *0.5;
    unsigned int maxVal = 1 << (s-1);

	original_data odata;
	compressed_diff bitCode;
	output_original_data.clear();
	std::vector<compressed_diff>::const_iterator it_in;
	for(it_in=input_diff.begin(); it_in !=input_diff.end(); it_in++){
		bitCode = *it_in;
		bitCode[s-1] = 0;
        double num =(double)(  bitCode.to_ulong()  );
		double rate_y = num /maxVal;
        if (rate_y >= 7.0/8) {
            odata= (original_data)   (1.0/2 +   (rate_y - 7.0/8 +half_delta_compensate_y) / 0.25  ) * max;
        }
        else if (rate_y >= 6.0/8) {
            odata= (original_data)   (1.0/4 +   (rate_y - 6.0/8 +half_delta_compensate_y) / 0.5   ) * max;
        }
        else if (rate_y >= 5.0/8) {
            odata= (original_data)   (1.0/8 +   (rate_y - 5.0/8 +half_delta_compensate_y)         ) * max;
        }
        else if (rate_y >= 4.0/8) {
            odata= (original_data)   (1.0/16 +  (rate_y - 4.0/8 +half_delta_compensate_y) / 2     ) * max;
        }
        else if (rate_y >= 3.0/8) {
            odata= (original_data)   (1.0/32 +  (rate_y - 3.0/8 +half_delta_compensate_y) / 4     ) * max;
        }
        else if (rate_y >= 2.0/8) {
            odata= (original_data)   (1.0/64 +  (rate_y - 2.0/8 +half_delta_compensate_y) / 8     ) * max;
        }
        else if (rate_y >= 1.0/8) {
            odata= (original_data)   (1.0/128 + (rate_y - 1.0/8 +half_delta_compensate_y) / 16    ) * max;
        }
        else {
            odata= (original_data)   (          (rate_y         +half_delta_compensate_y) / 16    ) * max;
        }
		if(  (*it_in )[s-1] == 1   ) {
			odata= -odata;
		}
		output_original_data.push_back(odata);
	}
}

