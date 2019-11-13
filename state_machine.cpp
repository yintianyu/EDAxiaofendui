/*
 * File: Regulation.cpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/06
 * Description: Implement State_Machine
 */

#include "state_machine.hpp"
#include <iostream>
#include <cmath>

// #define abs(x) ((x) > 0 ? (x) : -(x))

State_Machine::State_Machine(std::ofstream &output_fstream): signal_idx(-1), output_fstream(output_fstream), small_signal_count(0), 
period_count(0){
    // std::cout << "State_Machine Constructor: " << signal_count << std::endl;
    state = IDLE;
    c_count = 0;
    reference_sum = 0;
    base_idx = 0;
    base = 0;
    // regulator_A = new A_Regulation();
    // regulator_u = new u_Regulation();
    // regulator_homo = new Homo_Regulation();
    debug_total_head_size_byte = 0;
    // period_count = 0;
}

void State_Machine::act(original_data data, x_value time, int index, bool debug){
    assert(signal_idx >= 0);
    #ifdef DEBUG
    if(debug && signal_idx == DEBUG_SIGNAL && time > DEBUG_TIME - DEBUG_TIME_RANGE && time < DEBUG_TIME + DEBUG_TIME_RANGE){
        std::cout << "[DEBUG] data=" << data << " time=" << time << " period_count=" << period_count;
    }
    #endif
    if(state == IDLE){
        #ifdef DEBUG
        if(debug && signal_idx == DEBUG_SIGNAL && time > DEBUG_TIME - DEBUG_TIME_RANGE && time < DEBUG_TIME + DEBUG_TIME_RANGE)
            std::cout << std::endl;
        #endif
        base = data;
        base_time = time;
        base_idx = index;// TASK: Check this
        state = INIT;
        c_frames.clear();
        frames.clear();
        frames.push_back(X_Vals_Pair(time, data));
        c_idxes.clear();
        c_count = 0;
        reference_sum = 0;
        small_signal_count = data < 1 ? 1 : 0;
        reference_sum = fabs(base);
        if(reference_sum == 0){
            reference_sum = 1e-16; // epsilon, get rid of dividing 0
        }
        return;
    }
    if(state == INIT){
        slope = (data - base) / (time - base_time);
        small_signal_count += data < 1 ? 1 : 0;
        state = RUN;
        frames.push_back(X_Vals_Pair(time, data));
        predict_step = time - base_time;
        #ifdef DEBUG
        if(debug && signal_idx == DEBUG_SIGNAL && time > DEBUG_TIME - DEBUG_TIME_RANGE && time < DEBUG_TIME + DEBUG_TIME_RANGE)
            std::cout << " slope=" << slope << " data=" << data << " base=" << base << " time=" << time << "base_time=" << base_time << std::endl;
        #endif
        return;
    }
    if(state == RUN){    
        // 检查范数是否满足beta, 斜率和一开始的是不是差的太多
        original_data diff_sum = 0;
        original_data slope_local;
        bool slope_ok = true; // 用于检查当前slope和base的slope差距是不是过大
        diff_sum = fabs(data - base);
        small_signal_count += fabs(data) < 1 ? 1 : 0;
        int last_idx = frames.size()-1;
        slope_local = (data - frames[last_idx].value) / (time - frames[last_idx].x);
        if(fabs(slope_local) <= SLOPE_ZERO && fabs(slope) > SLOPE_ZERO){ // 需要回溯
            original_data last_data = frames[last_idx].value;
            x_value x = frames[last_idx].x;
            #ifdef DEBUG
            if(signal_idx == DEBUG_SIGNAL)
                std::cout << "[DIVIDE PERIOD] Period No." << period_count << " SLOPE_ZERO" << std::endl;
            #endif
            frames.pop_back();
            save_period();
            reset();
            act(last_data, x, index, false);
            act(data, time, index, false);
            return;
        }
        if((data == 0 && frames[last_idx].value != 0) || (slope_local != 0 && slope == 0) /*|| fabs((slope_local - slope) / (slope + 1e-25)) > SLOPE_ERROR_BETA*/){
            slope_ok = false;
        }
        if(diff_sum / reference_sum > BETA || !slope_ok/* || diff_time / predict_step > X_STEP_BETA*/){
            #ifdef DEBUG
            if(signal_idx == DEBUG_SIGNAL){
                if(!slope_ok)
                    std::cout << "[DIVIDE PERIOD] Period No." << period_count << " slope_ok " << std::endl;// << (data == 0 && frames[last_idx].value != 0) << " " << (slope_local != 0 && slope == 0) << " " << (fabs((slope_local - slope) / (slope + 1e-25)) > SLOPE_ERROR_BETA) << " " << fabs((slope_local - slope) / (slope + 1e-25)) << " " << slope_local << " " << slope <<std::endl;
                else
                    std::cout << "[DIVIDE PERIOD] Period No." << period_count << " BETA" << std::endl;
            }
            #endif
            save_period();
            reset();
            act(data, time, index, false);
            return;
        }
        bool isPredict = true;
        original_data predict = base + slope * (time - base_time);
        original_data diff = fabs(predict - data);
        #ifdef DEBUG
        if(debug && signal_idx == DEBUG_SIGNAL && time > DEBUG_TIME - DEBUG_TIME_RANGE && time < DEBUG_TIME + DEBUG_TIME_RANGE)
            std::cout << " diff=" << diff << std::endl;
        #endif 
        if(diff / fabs(data) > THRESHOLD_DIFF_DATA_RATIO){ // diff占data比过大，重新分P
            #ifdef DEBUG
            if(signal_idx == DEBUG_SIGNAL){
                std::cout << "[DIVIDE PERIOD] Period No." << period_count << " THRESHOLD_DIFF_DATA_RATIO diff=" << diff << " data=" << fabs(data) << " ratio=" << diff/fabs(data) << std::endl;
            }
            #endif
            save_period();
            reset();
            act(data, time, index, false);
            return;
        }
        if(diff > EPSILON){
            isPredict = false;
        }
        frames.push_back(X_Vals_Pair(time, data));
        if(!isPredict){
            c_count += 1;
            c_frames.push_back(X_Vals_Pair(time, data));
            c_idxes.push_back(frames.size() - 1);
        }
        // 帧的长度是否满足ALPHA
        // std::cout << "diff_sum / reference_sum = " << diff_sum / reference_sum << std::endl;
        if(frames.size() >= ALPHA){
            #ifdef DEBUG
            if(signal_idx == DEBUG_SIGNAL)
                std::cout << "[DIVIDE PERIOD] Period No." << period_count << " ALPHA" << std::endl;
            #endif
            save_period();
            reset();
        }
    }
}



void State_Machine::save_period(){
    bool predict = true;
    std::vector<compressed_diff> compressed;
    std::vector<original_data> to_be_compressed;
    original_data diff_max;
    original_data diff_min;
    std::cout << "[save_period] signal_idx: " << signal_idx << " period No." << period_count << " frame length: " << frames.size() << std::endl;
    if(c_count > YITA){ // 全部都压缩存储
        compressed.reserve(frames.size());
        to_be_compressed.reserve(frames.size());
        diff_max = 0;
        diff_min = 10000000;
        predict = false;
        for(const auto &frame : frames){
            original_data diff = frame.value - (base + slope * (frame.x - base_time));
            if(fabs(diff) > diff_max){
                diff_max = fabs(diff);
            }
            if(fabs(diff) < diff_min){
                diff_min = fabs(diff);
            }
            to_be_compressed.push_back(diff);
        }
    }
    else{ // 采用预测
        compressed.reserve(c_frames.size());
        to_be_compressed.reserve(c_frames.size());
        diff_max = 0;
        diff_min = 10000000;
        predict = true;
        for(const auto &frame : c_frames){
            original_data diff = frame.value - (base + slope * (frame.x - base_time));
            if(fabs(diff) > diff_max){
                diff_max = fabs(diff);
            }
            if(fabs(diff) < diff_min){
                diff_min = fabs(diff);
            }
            to_be_compressed.push_back(diff);
        }
    }
#ifdef DEBUG
    if(signal_idx == DEBUG_SIGNAL && period_count == DEBUG_PERIOD){
        for(auto i:to_be_compressed){
            std::cout << "[DEBUGPERIOD] [REGULATION] " << i << std::endl;
        }
    }
#endif
    perform_regulation(to_be_compressed, diff_max, diff_min, compressed);
    write_period_to_file(compressed, diff_max, predict);
    period_count += 1;
}

void State_Machine::perform_regulation(std::vector<original_data> &to_be_compressed, original_data max_diff, original_data min_diff, 
        std::vector<compressed_diff> &compressed){
    regulator_u->compress(to_be_compressed, max_diff, compressed); //为了测试效果先只使用u律
    regulation_type = REGU_U;
    // if(max_diff - min_diff >= THRESHOLD_HOMO_INHOMO){ // 非均匀量化
    //     if(small_signal_count * 2 < (int)frames.size()){ // 大信号比较多用A律
    //         regulator_A->compress(to_be_compressed, max_diff, compressed);
    //         regulation_type = REGU_A;
    //     }
    //     else{
    //         regulator_u->compress(to_be_compressed, max_diff, compressed);
    //         regulation_type = REGU_U;
    //     }
    // }
    // else{ // 均匀量化
    //     regulator_homo->compress(to_be_compressed, max_diff, compressed);
    //     regulation_type = REGU_HOMO;
    // }
}

void State_Machine::write_period_to_file(const std::vector<compressed_diff> &compressed, original_data diff_max, bool predict){
    // 先写入时间片头
    assert(output_fstream);
    uint8_t frame_count = frames.size();
    output_fstream.write((char*)&signal_idx, sizeof(signal_idx)); // 所属信号
    debug_total_head_size_byte += sizeof(signal_idx);
    output_fstream.write((char*)&frame_count, sizeof(frame_count)); // 帧数N
    debug_total_head_size_byte += sizeof(frame_count);
    output_fstream.write((char*)&predict, sizeof(char)); // 是否预测
    debug_total_head_size_byte += sizeof(char);    
    output_fstream.write((char*)&base_idx, sizeof(base_idx)); // 开始时间序号（未压缩）
    debug_total_head_size_byte += sizeof(base_idx);
    regulation_type_write regulation_type_tmp = regulation_type;
    output_fstream.write((char*)&regulation_type_tmp, sizeof(regulation_type_tmp)); // 规约方案
    debug_total_head_size_byte += sizeof(regulation_type_tmp);
    {
        original_data_write tmp = diff_max;
        output_fstream.write((char*)&tmp, sizeof(tmp)); // 每个信号的误差最大值
        debug_total_head_size_byte += sizeof(tmp);
    }
    {
        original_data_write tmp = base;
        output_fstream.write((char*)&tmp, sizeof(tmp)); // 每个信号的base
        debug_total_head_size_byte += sizeof(tmp);
    }
    {
        original_data_write tmp = slope;
        output_fstream.write((char*)&tmp, sizeof(tmp)); // 每个信号的slope
        debug_total_head_size_byte += sizeof(tmp);
    }
    if(predict){
        uint16_t c_frames_number = c_idxes.size();
        // std::cout << c_idxes.size() << " " << compressed.size() << " " << std::endl;
        assert(c_idxes.size() == compressed.size());
        output_fstream.write((char*)&c_frames_number, sizeof(c_frames_number)); // 有多少帧需要被压缩
        for(int i = 0;i < c_frames_number;++i){
            output_fstream.write((char*)&c_idxes[i], sizeof(c_idxes[i])); // 记录每个被压缩的帧的编号
        }
        for(int i = 0;i < c_frames_number;++i){
            uint32_t tmp = (uint32_t)(compressed[i].to_ulong());
            compressed_diff_write1 w1 = tmp & 0xffff;
            compressed_diff_write2 w2 = (tmp >> 16) && 0xff;
            output_fstream.write((char*)&w1, sizeof(w1)); // 存储压缩后的差值
            output_fstream.write((char*)&w2, sizeof(w2)); // 存储压缩后的差值
        }
    }
    else{ // 不用预测那就不需要写有多少帧被压缩了，直接上差值
        for(int i = 0;i < frame_count;++i){
            uint32_t tmp = (uint32_t)(compressed[i].to_ulong());
            compressed_diff_write1 w1 = tmp & 0xffff;
            compressed_diff_write2 w2 = (tmp >> 16) && 0xff;
            output_fstream.write((char*)&w1, sizeof(w1)); // 存储压缩后的差值
            output_fstream.write((char*)&w2, sizeof(w2)); // 存储压缩后的差值
        }
    }

}

void State_Machine::reset(){
    state = IDLE;
    c_frames.clear();
    frames.clear();
    c_idxes.clear();
}

State_Machine::~State_Machine(){
    if(signal_idx >= 0)
        save_period();
    std::cout << "Signal No." << signal_idx << "State Machine total head size(byte): " << debug_total_head_size_byte;
    std::cout << " Period Number: " << period_count << std::endl;
    // delete regulator_u;
    // delete regulator_homo;
    // delete regulator_A;
}