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
    Compressor(std::string &input_filename, std::string &output_filename):output_filename_(output_filename), 
    reader(input_filename){
    }
    void compress();
    private:
    WaveformReaderForCompetition reader;
    std::string output_filename_;
};

#endif