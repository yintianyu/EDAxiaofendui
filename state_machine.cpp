/*
 * File: Regulation.cpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/06
 * Description: Implement State_Machine
 */

#include "state_machine.hpp"
#include "A_Regulation.hpp"

#define abs(x) ((x) > 0 ? (x) : -(x))

State_Machine::State_Machine(std::ofstream &output_fstream, int signal_count):output_fstream(output_fstream), 
signal_count(signal_count), base(signal_count), slope(signal_count){
    c_count = 0;
}

void State_Machine::act(const std::vector<original_data> &data, x_value time, int index){
    if(state == IDLE){
        base = data;
        base_time = time;
        base_idx = index;// TASK: Check this
        state = INIT;
        c_frames.clear();
        frames.clear();
        frames.push_back(X_Vals_Pair(time, data));
        c_idxes.clear();
        c_count = 0;
        return;
    }
    if(state == INIT){
        for(int i = 0;i < signal_count;++i){
            slope[i] = (data[i] - base[i]) / (time - base_time);
        }
        state = RUN;
        frames.push_back(X_Vals_Pair(time, data));
        return;
    }
    if(state == RUN){
        frames.push_back(X_Vals_Pair(time, data));
        bool isPredict = true;
        for(int i = 0;i < signal_count;++i){
            original_data predict = base[i] + slope[i] * (time - base_time);
            if(abs(predict - data[i]) > EPSILON){
                isPredict = false;
                break;
            }
        }
        if(!isPredict){
            c_count += 1;
            c_frames.push_back(X_Vals_Pair(time, data));
            c_idxes.push_back(frames.size() - 1);
        }
        // 检查范数是否满足beta，帧的长度是否满足ALPHA
        original_data diff_sum = 0;
        original_data reference_sum = 0;
        for(int i = 0;i < signal_count;++i){
            diff_sum += abs(data[i] - base[i]);
            reference_sum += abs(base[i]);
        }
        if(diff_sum / reference_sum > BETA || frames.size() >= ALPHA){
            save_period();
            reset();
        }
    }
}



void State_Machine::save_period(){
    bool predict = true;
    std::vector<std::vector<compressed_diff>> compressed(signal_count);
    std::vector<std::vector<original_data>> to_be_compressed(signal_count);
    std::vector<original_data> diff_max(signal_count);
    std::vector<original_data> diff_min(signal_count);
    if(c_count > YITA){
        for(int i = 0;i < signal_count;++i){
            compressed[i].reserve(frames.size());
            to_be_compressed[i].reserve(frames.size());
            diff_max[i] = -10000;
            diff_min[i] = 10000;
        }
        // 全部都压缩存储
        predict = false;
        for(auto &frame : frames){
            for(int i = 0;i < signal_count;++i){
                original_data diff = frame.values[i] - (base[i] + slope[i] * (frame.x - base_time));
                if(diff > diff_max[i]){
                    diff_max[i] = diff;
                }
                if(diff < diff_min[i]){
                    diff_min[i] = diff;
                }
                to_be_compressed[i].push_back(diff);
            }
        }

    }
}

void State_Machine::perform_regulation(const std::vector<std::vector<original_data>> &to_be_compressed, const std::vector<original_data> &max_diff, const std::vector<original_data> &min_diff, 
        std::vector<std::vector<compressed_diff>> &compressed){
    for(int i = 0;i < signal_count;i++){
        if(max_diff[i] - min_diff[i] >= THRESHOLD_HOMO_INHOMO){ // 非均匀量化
            Regulation *regulator = new A_Regulation(); // TODO: 先统一用A律，怎么把C律用上再改
            regulator->compress(to_be_compressed[i], max_diff[i], compressed[i]);
            delete regulator;
        }
        else{ // 均匀量化
            
        }
    }
}