/*
 * File: decompressor.cpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/05
 * Description: Implement Decompressor Class
 */

#include "decompressor.hpp"
#include "period.hpp"
#include <iostream>
#include <thread>
#include <algorithm>

Regulation* Period::regulator_A = new A_Regulation();
Regulation* Period::regulator_u = new u_Regulation();
Regulation* Period::regulator_homo = new Homo_Regulation();

Decompressor::Decompressor(const std::string &input_filename, const std::string &output_filename):input_fstream(input_filename, std::ios::binary), outputter(output_filename){
    std::cout << "[Decompressor] output_filename: " << output_filename << std::endl;
    identifier_expect = IDENTIFIER;
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
            exit(1);
        }
    }

    std::sort(decompress_idxes.begin(), decompress_idxes.end()); // 解决了信号名乱序的情况下输出对不上的问题
    std::vector<std::string> tmp;
    for(auto i : decompress_idxes){
        tmp.push_back(signal_names[i]);
    }

    int decompress_signal_count = names.size();
    output_buffer.resize(decompress_signal_count); // 缓存即将输出到文件中的内容
    std::vector<int> decompress_idxes_reverse(signal_count, -1); // 用于反查待解压缩信号的编号
    for(int i = 0;i < decompress_signal_count;++i){
        decompress_idxes_reverse[decompress_idxes[i]] = i;
    }
    outputter.OutputHead(Period::x_values[0], Period::x_values[frame_count-1], tmp);
    Period *period = new Period(input_fstream);
    std::vector<original_data> result;
    int debug_period_count = 0;
    while(input_fstream.peek() != EOF){
        result.clear();
        int signal_idx;
        input_fstream.read((char*)&signal_idx, sizeof(signal_idx));
        assert(signal_idx >= 0 && signal_idx < signal_count);
        int decompress_idx = decompress_idxes_reverse[signal_idx];
        if(decompress_idx == -1){
            period->pseudo_decompress(); // 跳过这个period
        }
        else{
            period->decompress(result, signal_idx == DEBUG_SIGNAL && debug_period_count == DEBUG_PERIOD, signal_idx, debug_period_count);
        }
        for(const auto &element:result){
            output_buffer[decompress_idx].push(element);
        }
        if(signal_idx == DEBUG_SIGNAL){
            debug_period_count += 1;
        }
        write_data_to_file();
    }
    write_data_to_file();
    outputter.FinishOutput();

    delete period;
}

int Decompressor::read_metadata (){
    int identifier;
    input_fstream.read((char*)&identifier, sizeof(identifier));
    if(identifier != identifier_expect){
        std::cerr << "Indentifier error, not edaxfd file. Expected " << identifier_expect << ", got " << identifier << std::endl;
        exit(1);
    }
    input_fstream.read((char*)&signal_count, sizeof(signal_count));
    signal_names.resize(signal_count);
    // std::cout << "Decompress signal names:" << std::endl;
    for(int i = 0;i < signal_count;++i){
        input_fstream >> signal_names[i];
        // std::cout << signal_names[i] << std::endl;
    }
    char tmp;
    input_fstream.read((char*)&tmp, sizeof(char));
    input_fstream.read((char*)&frame_count, sizeof(frame_count));
    Period::x_values = new x_value[frame_count];
    input_fstream.read((char*)Period::x_values, frame_count * sizeof(x_value));
    // for(int i = 0;i < frame_count;++i){
    //     input_fstream.read((char*)&Period::x_values[i], sizeof(Period::x_values[i]));
    // }
    return 0;
}

void Decompressor::write_data_to_file(){
    static int current_frame = 0;
    if(current_frame < frame_count){
        bool ready = true;
        int write_number = 2147483647;
        _check(ready, write_number);
        if(ready){
            _ready(write_number, current_frame);
        }
    }
    return;
}

void Decompressor::_check(bool &ready, int &write_number){
        for(const auto &list_per_signal:output_buffer){
            if(list_per_signal.size() == 0){
                ready = false;
                break;
            }
            else{
                write_number = list_per_signal.size() < (uint32_t)write_number ? list_per_signal.size() : write_number;
            }
        }
}

void Decompressor::_ready(int write_number, int &current_frame){
    for(int i = 0;i < write_number;++i){
        outputter.OutputXValue(Period::x_values[current_frame]);
        #ifdef DEBUG
        std::cout << "[WRITE] time=" << Period::x_values[current_frame];
        #endif
        for(auto &list_per_signal:output_buffer){
            #ifdef DEBUG
            std::cout << " " << list_per_signal.front();
            #endif
            outputter.OutputSignalValue(list_per_signal.front());
            list_per_signal.pop();
        }
        #ifdef DEBUG
        std::cout << std::endl;
        #endif
        outputter.FinishOnePointData();
        current_frame += 1;
    }
}
