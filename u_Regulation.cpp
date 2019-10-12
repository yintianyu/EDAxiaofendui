/*
 * File: u_Regulations.cpp
 * Author: Guozhu Xin
 * Create Date: 2019/10/11
 * Description: u quantization
 */

#include "u_Regulation.hpp"
#include <cassert>
#include "./reader/WaveformReaderForCompetition.h"

compressed_diff u_Regulation::compress(original_data input_diff) {
    assert(input_diff >= 0 && input_diff <= 1);
    compressed_diff res;
    if (input_diff >= 127.0 / 255) {
        res = input_diff == 1 ? 255 : 224 + (input_diff - 127.0 / 255) * 255.0 / 4;
    } 
    else if (input_diff >= 63.0 / 255) {
        res = 192 + (input_diff - 63.0 / 255) * 255.0 / 2;
    } 
    else if (input_diff >= 31.0 / 255) {
        res = 160 + (input_diff - 31.0 / 255) * 255.0;
    }
    else if (input_diff >= 15.0 / 255) {
        res = 128 + (input_diff - 15.0 / 255) * 510.0;
    }
    else if (input_diff >= 7.0 / 255) {
        res = 96 + (input_diff - 7.0 / 255) * 1020.0;
    }
    else if (input_diff >= 3.0 / 255) {
        res = 64 + (input_diff - 3.0 / 255) * 2040.0;
    }
    else if (input_diff >= 1.0 / 255) {
        res = 32 + (input_diff - 1.0 / 255) * 4080.0;
    }
    else {
        res = input_diff * 8160.0;
    }
    return res;
}

original_data u_Regulation::decompress(compressed_diff input_diff) {
    original_data res;
    if (input_diff >= 224) {
        res = 127.0 / 255 + (original_data) (input_diff - 224) * 4.0 / 255;
    } 
    else if (input_diff >= 192) {
        res = 63.0 / 255 + (original_data) (input_diff - 192) * 2.0 / 255;
    } 
    else if (input_diff >= 160) {
        res = 31.0 / 255 + (original_data) (input_diff - 160) / 255;
    }
    else if (input_diff >= 128) {
        res = 15.0 / 255 + (original_data) (input_diff - 128) / 510;
    }
    else if (input_diff >= 96) {
        res = 7.0 / 255 + (original_data) (input_diff - 96) / 1020;
    }
    else if (input_diff >= 64) {
        res = 3.0 / 255 + (original_data) (input_diff - 64) / 2040;
    }
    else if (input_diff >= 32) {
        res = 1.0 / 255 + (original_data) (input_diff - 32) / 4080;
    }
    else {
        res = (original_data) input_diff / 8160;
    }
    return res;
}



