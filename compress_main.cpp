/*
 * File: compress_main.cpp
 * Description: main function for compress
 * Author: Tianyu Yin
 * Date: 2019/11/15
 */

#include <iostream>
#include <string>
#include "compressor.hpp"
#include "decompressor.hpp"
#include "datatype.hpp"
#include "period.hpp"
#include "state_machine.hpp"
#include <cstdio>
#include <iomanip>

// Parameters: compress original_waveform compressed_waveform
int main(int argc, char* argv[]){
    if(argc != 3){
        std::cerr << "Invalid arguments" << std::endl;
        exit(1);
    }
    std::string input_filename(argv[1]);
    std::string output_filename(argv[2]);
    Compressor *compressor = new Compressor(input_filename, output_filename);
    compressor->compress();
    delete compressor;
    return 0;
}