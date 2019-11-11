/*
 * File: compressor.cpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/05
 * Description: Implement the compressor
 */

#include "compressor.hpp"
#include <iostream>

Regulation* State_Machine::regulator_A = new A_Regulation();
Regulation* State_Machine::regulator_u = new u_Regulation();
Regulation* State_Machine::regulator_homo = new Homo_Regulation();

void Compressor::compress(){
    reader.Read();
    reader_x.Read();
    signal_count = reader.GetSignalCount();
    signal_names.clear();
    signal_names.reserve(signal_count);
    for(int i = 0;i < signal_count;++i){
        signal_names.push_back(reader.GetSignalName(i));
    }
    x_values = new std::vector<x_value>();
    while(!reader_x.IsDataFinished()){
        reader_x.ReadNextPointData();
        x_values->push_back(reader_x.GetNextXValue());
    }
    write_metadata_to_file();
    delete x_values;
    state_machines = new std::vector<State_Machine>(signal_count, output_fstream);
    for(int i = 0;i < signal_count;++i){
        (*state_machines)[i].set_signal_idx(i);
    }

    int index = 0;
    while(!reader.IsDataFinished()){
        reader.ReadNextPointData();
        x_value xValue = reader.GetNextXValue();
        for(int i = 0;i < signal_count;++i){
            (*state_machines)[i].act(reader.GetNextSignalValue(i), xValue, index);
        }
        index++;
    }
    delete state_machines;
    delete State_Machine::regulator_A;
    delete State_Machine::regulator_u;
    delete State_Machine::regulator_homo;
}

void Compressor::write_metadata_to_file (){
    assert(output_fstream);
    output_fstream.write((char*)&identifier, sizeof(identifier)); // 写标识符，压缩文件必须以identifier开头，否则解压缩程序会抛弃
    output_fstream.write((char*)&signal_count, sizeof(signal_count)); // 信号数量
    for(const auto &name : signal_names){
        output_fstream << name; // 写入信号名称
        output_fstream << '\n';
    }
    int frame_number = x_values->size(); // 帧数量
    output_fstream.write((char*)&frame_number, sizeof(frame_number));
    for(int i = 0;i < frame_number;++i){
        output_fstream.write((char*)&(*x_values)[i], sizeof((*x_values)[i])); // 每一帧的时间
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