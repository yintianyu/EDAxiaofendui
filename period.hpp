/*
 * File: compressor.hpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/05
 * Description: Define the Period Class
 */

#ifndef PERIOD_HPP
#define PERIOD_HPP

#include <vector>
#include <string>
#include <fstream>
#include "outputter/WaveformOutputterForCompetition.h"
#include "datatype.hpp"
#include "A_Regulation.hpp"

struct X_CompressVals_Pair{
    X_CompressVals_Pair(x_value x, const std::vector<compressed_diff> &values):x(x), values(values){}
    x_value x;
    const std::vector<compressed_diff> values;
};

// Class to describe time period, similar to State_Machine
class Period{
    private:
    int signal_count;

    std::vector<original_data> base;
    x_value base_time;
    int base_idx;
    std::vector<original_data> slope;
    enum State{IDLE, INIT, RUN} state;
    std::vector<X_CompressVals_Pair> c_frames;
    std::vector<X_CompressVals_Pair> frames;
    std::ifstream &input_fstream;
    uint8_t frame_count;

    int c_count; // Frames need compressing
    std::vector<uint8_t> c_idxes;

    uint8_t predict;

    x_value end_time; // 最后一帧的时间

    std::vector<original_data> diff_max;

    void read_head();
    WaveformOutputterForCompetition &outputter;
    Regulation *regulator;

    
    public:
    Period(std::ifstream &input_fstream, int signal_count, WaveformOutputterForCompetition &outputter):input_fstream(input_fstream), signal_count(signal_count), outputter(outputter){
        regulator = new A_Regulation();
    }
    static x_value x_value_decompress(compressed_x compressed);
    void decompress(const std::vector<int> &decompress_idxes, x_value &start, x_value &end);
    ~Period(){
        delete regulator;
    }
};


#endif
