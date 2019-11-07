/*
 * File: Regulation.cpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/06
 * Description: Implement State_Machine
 */

#include "state_machine.hpp"
#include <iostream>

#define abs(x) ((x) > 0 ? (x) : -(x))

State_Machine::State_Machine(std::ofstream &output_fstream, int signal_count): signal_count(signal_count), 
base(signal_count), slope(signal_count), output_fstream(output_fstream), small_signal_counts(signal_count, 0), 
regulation_types(signal_count), period_count(0){
    // std::cout << "State_Machine Constructor: " << signal_count << std::endl;
    state = IDLE;
    c_count = 0;
    reference_sum = 0;
    regulator_A = new A_Regulation();
    regulator_u = new u_Regulation();
    regulator_homo = new Homo_Regulation();
    // period_count = 0;
}

void State_Machine::act(const std::vector<original_data> &data, x_value time, int index, bool debug){
    if(data.size() == 0){
        return;
    }
    if(period_count == 0 || debug){
        std::cout << "[DEBUG] Period: " << period_count << " time: " << time << " data[0]: " << data[0];// << std::endl;
    }
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
        reference_sum = 0;
        for(int i = 0;i < signal_count;++i){
            reference_sum += abs(base[i]);
            small_signal_counts[i] = data[i] < 1 ? 1 : 0;  // 顺便将其初始化
        }
        if(reference_sum == 0){
            reference_sum = 1e-16; // epsilon, get rid of dividing 0
        }
        std::cout << std::endl;
        return;
    }
    if(state == INIT){
        for(int i = 0;i < signal_count;++i){
            slope[i] = (data[i] - base[i]) / (time - base_time);
            small_signal_counts[i] += data[i] < 1 ? 1 : 0;
        }
        state = RUN;
        frames.push_back(X_Vals_Pair(time, data));
        predict_step = time - base_time;
        std::cout << " slope[0]: " << slope[0] << std::endl;
        return;
    }
    if(state == RUN){
        std::cout << std::endl;       
        // 检查范数是否满足beta, 斜率和一开始的是不是差的太多
        original_data diff_sum = 0;
        x_value diff_time = time - base_time;
        std::vector<original_data> slope_local(signal_count);
        bool slope_ok = true; // 用于检查当前slope和base的slope差距是不是过大
        for(int i = 0;i < signal_count;++i){
            diff_sum += abs(data[i] - base[i]);
            small_signal_counts[i] += data[i] < 1 ? 1 : 0;
            int last_idx = frames.size()-1;
            slope_local[i] = (data[i] - frames[last_idx].values[i]) / (time - frames[last_idx].x);
            if(abs((slope_local[i] - slope[i]) / (slope[i] + 1e-20)) > 2){
                slope_ok = false;
                std::cout << period_count << " slope_ok=false, abort. "<< slope_local[i] << " " << slope[i] << " " << abs((slope_local[i] - slope[i]) / (slope[i] + 1e-20)) << std::endl;
                break;
            }
        }
        if(diff_sum / reference_sum > BETA || !slope_ok/* || diff_time / predict_step > X_STEP_BETA*/){
            if(slope_ok){
                std::cout << period_count << " BETA, abort. " << diff_sum << " " << reference_sum << std::endl;
            }
            end_time = time;
            save_period();
            reset();
            act(data, time, index, false);
            return;
        }
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
        // 帧的长度是否满足ALPHA
        // std::cout << "diff_sum / reference_sum = " << diff_sum / reference_sum << std::endl;
        if(frames.size() >= ALPHA){
            std::cout << period_count << " ALPHA, abort. " << std::endl;
            end_time = time;
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
    // std::cout << "frame length: " << frames.size() << std::endl;
    if(c_count > YITA){ // 全部都压缩存储
        for(int i = 0;i < signal_count;++i){
            compressed[i].reserve(frames.size());
            to_be_compressed[i].reserve(frames.size());
            diff_max[i] = -10000;
            diff_min[i] = 10000;
        }
        predict = false;
        for(const auto &frame : frames){
            for(int i = 0;i < signal_count;++i){
                original_data diff = frame.values[i] - (base[i] + slope[i] * (frame.x - base_time));
                if(period_count == DEBUG_PERIOD && i == 0){
                    std::cout << frame.values[i] << " - (" << base[i] << " + " << slope[i] << " * (" << frame.x << " - " << base_time << " )) = " << diff << std::endl; 
                }
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
    else{ // 采用预测
        // std::cout << "c_frames.size() = " << c_frames.size() << std::endl;
        for(int i = 0;i < signal_count;++i){
            compressed[i].reserve(c_frames.size());
            to_be_compressed[i].reserve(c_frames.size());
            diff_max[i] = -10000;
            diff_min[i] = 10000;
        }
        predict = true;
        for(const auto &frame : c_frames){
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
    perform_regulation(to_be_compressed, diff_max, diff_min, compressed);
    if(period_count == DEBUG_PERIOD && frames.size()) {
        std::cout << "Check Regulation on Period 1" << std::endl;
        for(int i = 0;i < to_be_compressed[0].size();++i){
            std::cout << "to_be_compressed[0][" << i << "]=" << to_be_compressed[0][i] << std::endl;
        }
    }
    write_period_to_file(compressed, diff_max, predict);
    std::cout << "[Compressor] Period No." << period_count << " frame number: " << frames.size() << std::endl;
    period_count += 1;
}

void State_Machine::perform_regulation(const std::vector<std::vector<original_data>> &to_be_compressed, const std::vector<original_data> &max_diff, const std::vector<original_data> &min_diff, 
        std::vector<std::vector<compressed_diff>> &compressed){
    // std::cout << "In State_Machine::perform_regulation: " << to_be_compressed.size() << " " << to_be_compressed[0].size() << std::endl;
    for(int i = 0;i < signal_count;i++){
        if(max_diff[i] - min_diff[i] >= THRESHOLD_HOMO_INHOMO){ // 非均匀量化
            if(small_signal_counts[i] * 2 < frames.size()){ // 大信号比较多用A律
                regulator_A->compress(to_be_compressed[i], max_diff[i], compressed[i]);
                regulation_types[i] = REGU_A;
            }
            else{
                regulator_u->compress(to_be_compressed[i], max_diff[i], compressed[i]);
                regulation_types[i] = REGU_U;
            }
        }
        else{ // 均匀量化
            regulator_homo->compress(to_be_compressed[i], max_diff[i], compressed[i]);
            regulation_types[i] = REGU_HOMO;
        }
    }
    // std::cout << compressed.size() << " " << compressed[0].size() << std::endl;
}

compressed_x State_Machine::x_value_compress(x_value x){
    compressed_x compressed;
    assert(x < X_VALUE_MAX);
    int count = 0;
    // std::cout << "[x_value_compress] x=" << x << std::endl;
    while(x < X_VALUE_MAX / X_VALUE_STEP){
        x *= X_VALUE_STEP;
        // std::cout << "[x_value_compress] x=" << x << std::endl;
        count += 1;
    }
    // std::cout << "[x_value_compress] count=" << count << std::endl;
    int val = (int)x;
    assert(count < (1 << X_VALUE_DEVIDE));
    compressed = val << X_VALUE_DEVIDE | count;
    return compressed;
}

void State_Machine::write_period_to_file(const std::vector<std::vector<compressed_diff>> &compressed, const std::vector<original_data> &diff_max, bool predict){
    // 先写入时间片头
    assert(output_fstream);
    uint8_t frame_count = frames.size();
    output_fstream.write((char*)&frame_count, sizeof(frame_count)); // 帧数N
    output_fstream.write((char*)&predict, sizeof(char)); // 是否预测
    // output_fstream.write((char*)&signal_count, sizeof(uint16_t)); // 信号数量
    output_fstream.write((char*)&base_time, sizeof(base_time)); // 开始时间（未压缩）
    output_fstream.write((char*)&end_time, sizeof(end_time)); // 结束时间（未压缩）
    std::cout << "[Period Metadata] " << (int)frame_count << " " << predict << " " << base_time << " " << end_time << std::endl;
    for(int i = 0;i < signal_count;++i){
        output_fstream.write((char*)&regulation_types[i], sizeof(regulation_types[i])); // 规约方案
    }
    for(int i = 0;i < signal_count;++i){
        output_fstream.write((char*)&diff_max[i], sizeof(diff_max[i])); // 每个信号的误差最大值
    }
    for(int i = 0;i < signal_count;++i){
        output_fstream.write((char*)&base[i], sizeof(base[i])); // 每个信号的base
    }
    for(int i = 0;i < signal_count;++i){
        output_fstream.write((char*)&slope[i], sizeof(slope[i]));
    }
    for(int i = 1;i < frame_count;++i){
        compressed_x tmp = x_value_compress(frames[i].x - frames[i-1].x); // 压缩每个time的step
        output_fstream.write((char*)&tmp, sizeof(tmp)); // 一共(frame_count - 1)个uint16
    }

    if(predict){
        uint16_t c_frames_number = c_idxes.size();
        // std::cout << c_idxes.size() << " " << compressed.size() << " " << compressed[0].size() << std::endl;
        assert(c_idxes.size() == compressed[0].size());
        output_fstream.write((char*)&c_frames_number, sizeof(c_frames_number)); // 有多少帧需要被压缩
        for(int i = 0;i < c_frames_number;++i){
            output_fstream.write((char*)&c_idxes[i], sizeof(c_idxes[i])); // 记录每个被压缩的帧的编号
        }
        for(int i = 0;i < c_frames_number;++i){
            for(int j = 0;j < signal_count;++j){
                compressed_diff_write tmp = (compressed_diff_write)(compressed[j][i].to_ulong());
                output_fstream.write((char*)&tmp, sizeof(tmp)); // 存储压缩后的差值
            }
        }
    }
    else{ // 不用预测那就不需要写有多少帧被压缩了，直接上差值
        for(int i = 0;i < frame_count;++i){
            for(int j = 0;j < signal_count;++j){
                compressed_diff_write tmp = (compressed_diff_write)(compressed[j][i].to_ulong());
                output_fstream.write((char*)&tmp, sizeof(tmp)); // 存储压缩后的差值
            }
        }
    }

}

void State_Machine::reset(){
    base.clear();
    slope.clear();
    state = IDLE;
    c_frames.clear();
    frames.clear();
    c_idxes.clear();
}

State_Machine::~State_Machine(){
    save_period();
    delete regulator_u;
    delete regulator_homo;
    delete regulator_A;
}