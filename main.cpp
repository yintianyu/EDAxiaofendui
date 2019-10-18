/*
 * File: main.cpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/05
 * Description: The main function of the program
 */

#include <iostream>
#include <string>
#include "compressor.hpp"

int main(int argc, char* argv[]){
    if(argc < 3){
        std::cerr << "Invalid arguments" << std::endl;
        exit(-1);
    }
    std::string input_filename(argv[1]);
    std::string output_filename(argv[2]);
    Compressor *compressor = new Compressor(input_filename, output_filename);
    compressor->compress();
    delete compressor;
    return 0;
}
