/*
 * File: decompressor.hpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/28
 * Description: Define Decompressor Class
 */

#ifndef DECOMPRESSOR_HPP
#define DECOMPRESSOR_HPP

#include <fstream>
#include <string>
#include <vector>
#include "datatype.hpp"
#include "outputter/WaveformOutputterForCompetition.h"
#include "outputter/BasicException.h"

class Decompressor{
    public:
    Decompressor(const std::string &input_filename, const std::string &output_filename);
    void decompress(const std::vector<std::string> &names);
    ~Decompressor(){
        input_fstream.close();
    }
    private:
    std::ifstream input_fstream;
    WaveformOutputterForCompetition outputter;
    int identifer_expect;
    int signal_count;
    std::vector<std::string> signal_names;
    std::vector<int> decompress_idxes;


    int read_metadata();
};

#endif