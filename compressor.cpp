/*
 * File: compressor.cpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/05
 * Description: Implement the compressor
 */

#include "compressor.hpp"
#include <iostream>

void Compressor::compress(){
    reader.Read();
    signal_count = reader.GetSignalCount();
    state_machine = new State_Machine(output_fstream, signal_count);
    signal_names.clear();
    signal_names.reserve(signal_count);
    for(int i = 0;i < signal_count;++i){
        signal_names.push_back(reader.GetSignalName(i));
    }
    write_metadata_to_file();
    while(!reader.IsDataFinished()){
        reader.ReadNextPointData();
        x_value xValue = reader.GetNextXValue();
        std::vector<original_data> frame_values(signal_count);
        for(int i = 0;i < signal_count;++i){
            frame_values[i] = reader.GetNextSignalValue(i);
        }
        state_machine->act(frame_values, xValue, 0); // TASK: Check this idx
    }
    std::cout << "[Compressor] period_count:" << state_machine->period_count << std::endl;
    delete state_machine;
}

void Compressor::write_metadata_to_file (){
    assert(output_fstream);
    output_fstream.write((char*)&identifier, sizeof(identifier)); // 写标识符，压缩文件必须以identifier开头，否则解压缩程序会抛弃
    output_fstream.write((char*)&signal_count, sizeof(signal_count)); // 信号数量
    for(const auto &name : signal_names){
        output_fstream << name; // 写入信号名称
        output_fstream << '\n';
    }
}

void Compressor::get_signal_names(std::vector<std::string> &output_signal_names){
    signal_count = reader.GetSignalCount();
    output_signal_names.clear();
    output_signal_names.reserve(signal_count);
    std::cout << "[compressor] signal names: " << std::endl;
    for(int i = 0;i < signal_count;++i){
        std::string name = reader.GetSignalName(i);
        std::cout << name << std::endl;
        output_signal_names.push_back(name);
    }
}