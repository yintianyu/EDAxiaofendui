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
#include "u_Regulation.hpp"
#include "Homo_Regulation.hpp"

struct X_CompressVals_Pair{
    X_CompressVals_Pair(x_value x, const std::vector<compressed_diff> &values):x(x), values(values){}
    X_CompressVals_Pair():x(0){}
    x_value x;
    std::vector<compressed_diff> values;
};

// Class to describe time period, similar to State_Machine
class Period{
    private:
    int signal_idx;
    std::ifstream &input_fstream;

    original_data base;
    x_value base_time;
    int base_idx;
    original_data slope;
    enum State{IDLE, INIT, RUN} state;
    std::vector<X_CompressVals_Pair> c_frames;
    uint8_t frame_count;

    int c_count; // Frames need compressing
    std::vector<uint8_t> c_idxes;

    uint8_t predict;
    uint8_t *buffer;

    x_value end_time; // 最后一帧的时间

    original_data diff_max;

    void read_head();
    void perform_deregulation(const std::vector<compressed_diff> &compressed, original_data diff_max, std::vector<original_data> &decompressed);
    static Regulation *regulator_A;
    static Regulation *regulator_u;
    static Regulation *regulator_homo;

    REGULATION_TYPE regulation_type;

    
    public:
    Period(std::ifstream &input_fstream):input_fstream(input_fstream){
        // regulator_A = new A_Regulation();
        // regulator_u = new u_Regulation();
        // regulator_homo = new Homo_Regulation();
        buffer = new uint8_t[256 * 3];
        period_count = 0;
    }
    void decompress(std::vector<original_data> &result, bool debug, int debug_signal_idx, int debug_period_count);
    void pseudo_decompress();
    ~Period(){
        // delete regulator_A;
        // delete regulator_u;
        // delete regulator_homo;
        delete[] buffer;
    }
    int period_count;
    static x_value *x_values; // x_values数组
};


#endif
