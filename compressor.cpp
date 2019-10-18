/*
 * File: compressor.cpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/05
 * Description: Implement the compressor
 */

#include "compressor.hpp"

void Compressor::compress(){
    reader.Read();
    signal_count = reader.GetSignalCount();
    state_machine = new State_Machine(output_fstream, signal_count);
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
    delete state_machine;
}

void Compressor::write_metadata_to_file (){
    assert(output_fstream);
    output_fstream.write((char*)&identifier, sizeof(identifier)); // 写标识符，压缩文件必须以identifier开头，否则解压缩程序会抛弃
    output_fstream.write((char*)&signal_count, sizeof(signal_count)); // 信号数量
    for(const auto &name : signal_names){
        output_fstream << name; // 写入信号名称
    }
}