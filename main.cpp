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
    if(argc < 4){
        std::cerr << "Invalid arguments" << std::endl;
        exit(-1);
    }
    std::string input_filename(argv[1]);
    std::string output_filename(argv[2]);
    std::string decompress_filename(argv[3]);
}
