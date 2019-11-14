/*
 * File: Homo_Regulations.cpp
 * Author: Guozhu Xin
 * Create Date: 2019/10/17
 * Description: u quantization
 */

#include "Homo_Regulation.hpp"

void Homo_Regulation :: compress(const std::vector<original_data> &input_diff, original_data max, std::vector<compressed_diff> &output_compressed_diff) {
    compressed_diff t;
    char s = t.size();
    unsigned int maxVal = 1 << (s-1);
    for (auto i = input_diff.begin(); i != input_diff.end(); ++i) {
        compressed_diff res;
        original_data normDiff = fabs(*i) / max;
        res = normDiff == 1 ? (maxVal - 1) : normDiff * maxVal;
        res[s-1] = *i < 0 ? 1 : 0;
        output_compressed_diff.push_back(res);
    }
}

void Homo_Regulation :: decompress(const std::vector<compressed_diff> &input_diff, original_data max, std::vector<original_data> &output_original_data) {
    compressed_diff t;
    char s = t.size();
    unsigned int maxVal = 1 << (s-1);
    original_data res;
    for (auto i = input_diff.begin(); i != input_diff.end(); ++i) {
        auto j = (*i);
        j[s-1] = 0;
        auto num = j.to_ulong();
        double sig = (*i)[s-1] == 1 ? -1 : 1; 
        res = (original_data) (num + 0.5) * max / maxVal;
        res = res * sig;
        output_original_data.push_back(res);
    }
}
