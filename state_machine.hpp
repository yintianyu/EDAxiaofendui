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
#include "A_Regulation.hpp"
#include "u_Regulation.hpp"
#include "Homo_Regulation.hpp"

struct X_Vals_Pair{
    X_Vals_Pair(x_value x, original_data value):x(x), value(value){}
    x_value x;
    original_data value;
};

class State_Machine{
    private:
    int signal_idx; // 本状态机负责的信号编号

    original_data base;
    x_value base_time;
    int base_idx;
    int base_count;
    original_data slope;
    enum State{IDLE, INIT, RUN} state;
    std::vector<X_Vals_Pair> c_frames;
    std::vector<X_Vals_Pair> frames;
    std::ofstream &output_fstream;

    int c_count; // Frames need compressing
    int small_signal_count; // 有多少信号是小信号，用于u和A律的选择
    std::vector<uint8_t> c_idxes;

    x_value predict_step; // 第一帧和第二帧之间的时间跨度

    original_data reference_sum;

    REGULATION_TYPE regulation_type; // 这个Period采用的规约手段，A律，u律还是均匀量化

    void save_period();
    void reset();
    void perform_regulation(std::vector<original_data> &to_be_compressed, original_data max_diff, original_data min_diff, 
        std::vector<compressed_diff> &compressed);

    void write_period_to_file(const std::vector<compressed_diff> &compressed, original_data diff_max, bool predict);

    int debug_total_head_size_byte; // 用于计量所有的时间片中时间片头的大小
    public:
    static Regulation *regulator_A, *regulator_u, *regulator_homo;
    State_Machine(std::ofstream &output_fstream);
    void act(original_data data, x_value time, int index, bool debug=true);
    inline void set_signal_idx(int signal_idx){
        assert(this->signal_idx == -1);
        this->signal_idx = signal_idx;
    }
    int period_count;
    ~State_Machine();
};

#endif