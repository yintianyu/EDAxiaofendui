/*
 * File: compressor.cpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/05
 * Description: Implement the compressor
 */

#include "compressor.hpp"

void Compressor::compress(){
    reader.Read();
    int signal_count = reader.GetSignalCount();
    signal_names.reserve(signal_count);
    for(int i = 0;i < signal_count;++i){
        signal_names.push_back(reader.GetSignalName(i));
    }
    while(!reader.IsDataFinished()){
        reader.ReadNextPointData();
        x_value xValue = reader.GetNextXValue();
        std::vector<original_data> frame_values(signal_count);
        for(int i = 0;i < signal_count;++i){
            frame_values[i] = reader.GetNextSignalValue(i);
        }
    }
}