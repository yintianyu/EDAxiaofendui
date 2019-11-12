#include "period.hpp"
#include <iostream>

x_value *Period::x_values = nullptr;

void Period::read_head(){
    // input_fstream.read((char*)&signal_idx, sizeof(signal_idx)); // 所属信号
    input_fstream.read((char*)&frame_count, sizeof(frame_count)); // 帧数
    frames.resize(frame_count);
    input_fstream.read((char*)&predict, sizeof(predict)); // 是否预测
    input_fstream.read((char*)&base_idx, sizeof(base_idx)); // 开始时间
    frames[0].x = x_values[base_idx];
    // input_fstream.read((char*)&end_time, sizeof(end_time)); // 结束时间
    regulation_type_write tmp;
    input_fstream.read((char*)&tmp, sizeof(tmp)); // 规约方案
    regulation_type = (REGULATION_TYPE)tmp;
    {
        original_data_write tmp;
        input_fstream.read((char*)&tmp, sizeof(tmp)); // 每个信号的误差最大值
        diff_max = tmp;
    }
    {
        original_data_write tmp;
        input_fstream.read((char*)&tmp, sizeof(tmp)); // 每个信号的base
        base = tmp;
    }
    {
        original_data_write tmp;
        input_fstream.read((char*)&tmp, sizeof(tmp)); // 每个信号的slope
        slope = tmp;
    }
}

void Period::decompress(std::vector<original_data> &result, bool debug){
    read_head();
    result.resize(frame_count);
    if(predict){
        uint16_t c_frames_number;
        input_fstream.read((char*)&c_frames_number, sizeof(c_frames_number));
        c_idxes.resize(c_frames_number);
        for(int i = 0;i < c_frames_number;++i){
            input_fstream.read((char*)&c_idxes[i], sizeof(c_idxes[i])); // 记录每个被压缩的帧的编号
        }
        std::vector<compressed_diff> compressed;
        compressed.resize(c_frames_number);
        for(int i = 0;i < c_frames_number;++i){
            compressed_diff_write tmp;
            input_fstream.read((char*)&tmp, sizeof(tmp)); // 压缩后的差值
            compressed[i] = tmp;
        }
        std::vector<original_data> decompressed;
        switch(regulation_type){
            case REGU_A:
                regulator_A->decompress(compressed, diff_max, decompressed);
                break;
            case REGU_U:
                regulator_u->decompress(compressed, diff_max, decompressed);
                break;
            case REGU_HOMO:
                regulator_homo->decompress(compressed, diff_max, decompressed);
                break; 
            default:
                std::cerr << "egulation_type error, die" << std::endl;
                exit(1);                   
        }
        #ifdef DEBUG
            if(debug){
                for(auto i:decompressed){
                    std::cout << "[REGULATION] " << i << std::endl;
                }
            }
        #endif
        result[0] = base;
        int c_idx_i = 0; // 用以记录现在c_idx到哪了
        for(int i = 1;i < frame_count;++i){
            original_data predict_value;
            predict_value = slope * (x_values[base_idx + i] - base_time) + base;
            #ifdef DEBUG
            if(debug){
                std::cout << "[DEBUGPERIOD] x=" << x_values[base_idx + i] << std::endl;
            }
            #endif
            if(c_frames_number > 0 && i == c_idxes[c_idx_i]){
                result[i] = predict_value + decompressed[c_idx_i];
                c_idx_i++;
            }
            else{
                result[i] = predict_value;
            }
        }
    }
    else{
        // Reserved for Boris Johnson;
        std::vector<compressed_diff> compressed(frame_count);
        for(int i = 0;i < frame_count;++i){
            compressed_diff_write tmp;
            input_fstream.read((char*)&tmp, sizeof(tmp)); // 压缩后的差值
            compressed[i] = tmp;
        }
        std::vector<original_data> decompressed;
        switch(regulation_type){
            case REGU_A:
                regulator_A->decompress(compressed, diff_max, decompressed);
                break;
            case REGU_U:
                regulator_u->decompress(compressed, diff_max, decompressed);
                break;
            case REGU_HOMO:
                regulator_homo->decompress(compressed, diff_max, decompressed);
                break; 
            default:
                std::cerr << "egulation_type error, die" << std::endl;
                exit(1);                   
        }
        #ifdef DEBUG
            if(debug){
                for(auto i:decompressed){
                    std::cout << "[REGULATION] " << i << std::endl;
                }
            }
        #endif
        result[0] = base;
        for(int i = 1;i < frame_count;++i){
            original_data predict_value;
            predict_value = slope * (x_values[base_idx + i] - base_time) + base + decompressed[i];
            #ifdef DEBUG
            if(debug){
                std::cout << "[DEBUGPERIOD] x=" << x_values[base_idx + i] << " decompressed=" << decompressed[i] << std::endl;
            }
            #endif
            result[i] = predict_value;
        }
    }
    std::cout << "[Decompress] Period No." << period_count << " frame number: " << (int)frame_count << std::endl;
    ++period_count;
}

/*
 * 只读文件，不处理
 */
void Period::pseudo_decompress(){
    read_head();
    if(predict){
        uint16_t c_frames_number;
        input_fstream.read((char*)&c_frames_number, sizeof(c_frames_number));
        c_idxes.resize(c_frames_number);
        for(int i = 0;i < c_frames_number;++i){
            input_fstream.read((char*)&c_idxes[i], sizeof(c_idxes[i])); // 记录每个被压缩的帧的编号
        }
        for(int i = 0;i < c_frames_number;++i){
            compressed_diff_write tmp;
            input_fstream.read((char*)&tmp, sizeof(tmp)); // 压缩后的差值
        }
    }
    else{
        // Reserved for Boris Johnson;
        for(int i = 0;i < frame_count;++i){
            compressed_diff_write tmp;
            input_fstream.read((char*)&tmp, sizeof(tmp)); // 压缩后的差值
        }
    }
    ++period_count;
}