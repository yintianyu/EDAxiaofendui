/*
 * File: state_machine.hpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/14
 * Description: Define State Machine class of compression
 */

#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP


#include <vector>
#include <fstream>
#include "datatype.hpp"

struct X_Vals_Pair{
    X_Vals_Pair(x_value x, const std::vector<original_data> &values):x(x), values(values){}
    x_value x;
    const std::vector<original_data> values;
};

class State_Machine{
    private:
    int signal_count;

    std::vector<original_data> base;
    x_value base_time;
    int base_idx;
    std::vector<original_data> slope;
    enum State{IDLE, INIT, RUN} state;
    std::vector<X_Vals_Pair> c_frames;
    std::vector<X_Vals_Pair> frames;
    std::ofstream &output_fstream;

    int c_count; // Frames need compressing
    std::vector<uint8_t> c_idxes;

    x_value end_time; // 最后一帧的时间

    void save_period();
    void reset();
    void perform_regulation(const std::vector<std::vector<original_data>> &to_be_compressed, const std::vector<original_data> &max_diff, const std::vector<original_data> &min_diff, 
        std::vector<std::vector<compressed_diff>> &compressed);

    static compressed_x x_value_compress(x_value x); // 压缩x值
    void write_period_to_file(const std::vector<std::vector<compressed_diff>> &compressed, const std::vector<original_data> &diff_max, bool predict);
    public:
    State_Machine(std::ofstream &output_fstream, int signal_count);
    void act(const std::vector<original_data> &data, x_value time, int index);
    ~State_Machine();
};

#endif