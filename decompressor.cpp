/*
 * File: decompressor.cpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/05
 * Description: Implement Decompressor Class
 */

#include "decompressor.hpp"
#include "period.hpp"
#include <iostream>

Decompressor::Decompressor(const std::string &input_filename, const std::string &output_filename):input_fstream(input_filename, std::ios::binary), outputter(output_filename){
    std::cout << "[Decompressor] output_filename: " << output_filename << std::endl;
    identifer_expect = IDENTIFER;
}


void Decompressor::decompress(const std::vector<std::string> &names){
    if(read_metadata()){
        std::cerr << "Failed to read metadata, please check input file, die" << std::endl;
        return;
    }
    for(const auto &name:names){
        int i = 0;
        for(i = 0;i < signal_count;++i){
            if(signal_names[i] == name){
                decompress_idxes.push_back(i);
                break;
            }
        }
        if(i == signal_count){
            std::cerr << "Signal name: " << name << " not found, die" << std::endl;
            return;
        }
    }
    outputter.OutputHead(0, 0, names);
    x_value start = -1, end = -1;
    Period *period = new Period(input_fstream, signal_count, outputter);
    int period_count = 0;
    while(input_fstream.peek() != EOF){
        end = -1;
        period->decompress(decompress_idxes, start, end);
        period_count += 1;
    }
    std::cout << "[Decompressor] period_count: " << period->period_count << std::endl;
    outputter.FinishOutput();
    delete period;
}

int Decompressor::read_metadata (){
    int identifer;
    input_fstream.read((char*)&identifer, sizeof(identifer));
    if(identifer != identifer_expect){
        std::cerr << "Indentifier error, not edaxfd file. Expected " << identifer_expect << ", got " << identifer_expect << std::endl;
        return 1;
    }
    input_fstream.read((char*)&signal_count, sizeof(signal_count));
    signal_names.resize(signal_count);
    std::cout << "Decompress signal names:" << std::endl;
    for(int i = 0;i < signal_count;++i){
        input_fstream >> signal_names[i];
        std::cout << signal_names[i] << std::endl;
    }
    char tmp;
    input_fstream.read((char*)&tmp, sizeof(char));
    return 0;
}