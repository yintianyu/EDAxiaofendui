/*
 * File: u_Regulations.cpp
 * Author: Guozhu Xin
 * Create Date: 2019/10/17
 * Description: u quantization
 */

#include "u_Regulation.hpp"

void u_Regulation :: compress(const std::vector<original_data> &input_diff, original_data max, std::vector<compressed_diff> &output_compressed_diff) {
    compressed_diff t;
    char s = t.size();
    unsigned int maxVal = 1 << (s-1);
    for (auto i = input_diff.begin(); i != input_diff.end(); ++i) {
        compressed_diff res;
        original_data normDiff = fabs(*i) / max;
        if (normDiff >= 127.0 / 255) {
            res = normDiff == 1 ? (maxVal-1) : maxVal*7/8 + (normDiff - 127.0 / 255) * 255 * maxVal / 1024;
        } 
        else if (normDiff >= 63.0 / 255) {
            res = maxVal*6/8 + (normDiff - 63.0 / 255) * 255.0 * maxVal / 512;
        } 
        else if (normDiff >= 31.0 / 255) {
            res = maxVal*5/8 + (normDiff - 31.0 / 255) * 255.0 * maxVal / 256;
        }
        else if (normDiff >= 15.0 / 255) {
            res = maxVal*4/8 + (normDiff - 15.0 / 255) * 255.0 * maxVal / 128;
        }
        else if (normDiff >= 7.0 / 255) {
            res = maxVal*3/8 + (normDiff - 7.0 / 255) * 255.0 * maxVal / 64;
        }
        else if (normDiff >= 3.0 / 255) {
            res =  maxVal*2/8 + (normDiff - 3.0 / 255) * 255.0 * maxVal / 32;
        }
        else if (normDiff >= 1.0 / 255) {
            res =  maxVal*1/8 + (normDiff - 1.0 / 255) * 255.0 * maxVal / 16;
        }
        else {
            res = normDiff * 255.0 * maxVal / 8;
        }
        res[s-1] = *i < 0 ? 1 : 0;
        output_compressed_diff.push_back(res);
    }
}

void u_Regulation :: decompress(const std::vector<compressed_diff> &input_diff, original_data max, std::vector<original_data> &output_original_data) {
    compressed_diff t;
    char s = t.size();
    unsigned int maxVal = 1 << (s-1);
    output_original_data.resize(input_diff.size());
    int idx = 0;
    original_data res;
    for (auto i = input_diff.begin(); i != input_diff.end(); ++i) {
        auto j = (*i);
        j[s-1] = 0;
        auto num = j.to_ulong();
        double sig = (*i)[s-1] == 1 ? -1 : 1; 
        if (num >= maxVal*7/8) {
            res = (127.0 / 255 + (original_data) (num + 0.5 - maxVal*7/8) * 1024 / (255*maxVal)) * max;
        } 
        else if (num >=  maxVal*6/8) {
            res = (63.0 / 255 + (original_data) (num + 0.5 - maxVal*6/8) * 512 / (255*maxVal)) * max;
        } 
        else if (num >= maxVal*5/8) {
            res = (31.0 / 255 + (original_data) (num + 0.5 - maxVal*5/8) * 256 / (255*maxVal)) * max;
        }
        else if (num >= maxVal*4/8) {
            res = (15.0 / 255 + (original_data) (num + 0.5 - maxVal*4/8) * 128 / (255*maxVal)) * max;
        }
        else if (num >= maxVal*3/8) {
            res = (7.0 / 255 + (original_data) (num + 0.5 - maxVal*3/8) * 64 / (255*maxVal)) * max;
        }
        else if (num >=  maxVal*2/8) {
            res = (3.0 / 255 + (original_data) (num + 0.5 - maxVal*2/8) * 32 / (255*maxVal)) * max;
        }
        else if (num >= maxVal*1/8) {
            res = (1.0 / 255 + (original_data) (num + 0.5 - maxVal*1/8) * 16 / (255*maxVal)) * max;
        }
        else {
            res = max * (original_data) (num + 0.5) * 8 / (255*maxVal);
        }
        res = res * sig;
        output_original_data[idx++] = res;
    }
}





