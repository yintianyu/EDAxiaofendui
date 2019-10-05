/*
 * File: compressor.hpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/05
 * Description: The main function of the program
 */

#ifndef COMPRESSOR_HPP
#define COMPRESSOR_HPP

#include <string>
#include "reader/WaveformReaderForCompetition.h"

class Compressor{
    public:
    Compressor(std::string &input_filename, std::string &output_filename):input_filename_(input_filename),
    output_filename_(output_filename){
    }
    void compress();
    private:
    std::string input_filename_;
    std::string output_filename_;
};

#endif