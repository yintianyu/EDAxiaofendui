/*
 * File: decompress_main.cpp
 * Description: main function for decompress
 * Author: Tianyu Yin
 * Date: 2019/11/15
 */

#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include "compressor.hpp"
#include "decompressor.hpp"
#include "datatype.hpp"
#include "period.hpp"
#include "state_machine.hpp"
#include <cstdio>
#include <iomanip>

// Parameters: decompress compressed_waveform decompressed_waveform -sigfile sigfile_name
int main(int argc, char *argv[]){
    if(argc != 5 || strcmp(argv[3], "-sigfile")){
        std::cerr << "Invalid arguments" << std::endl;
        exit(1);
    }
    std::ifstream signal_fstream(argv[4]);
    std::vector<std::string> signal_names;
    char buffer[80];
    while(!signal_fstream.eof()){
        signal_fstream.getline(buffer, 80);
        signal_names.push_back(buffer);
    }
    Decompressor *decompressor = new Decompressor(argv[1], argv[2]);
    decompressor->decompress(signal_names);
    return 0;
}