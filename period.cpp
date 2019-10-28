#include "period.hpp"

x_value Period::x_value_decompress(compressed_x compressed){
    int count = compressed & 0xff;
    x_value val = compressed >> 4;
    val /= (1 << count);
    return val;
}

void Period::read_head(){
    input_fstream.read((char*)&frame_count, sizeof(frame_count)); // 帧数
    frames.resize(frame_count);
    input_fstream.read((char*)&predict, sizeof(predict)); // 是否预测
    input_fstream.read((char*)&base_time, sizeof(base_time)); // 开始时间
    frames[0].x = base_time;
    input_fstream.read((char*)&end_time, sizeof(end_time)); // 结束时间
    for(int i = 0;i < signal_count;++i){
        input_fstream.read((char*)&diff_max[i], sizeof(diff_max[i])); // 每个信号的误差最大值
    }
    for(int i = 0;i < signal_count;++i){
        input_fstream.read((char*)&base[i], sizeof(base[i])); // 每个信号的base
    }
    for(int i = 1;i < frame_count;++i){
        compressed_x tmp;
        input_fstream.read((char*)&tmp, sizeof(tmp));
        x_value diff = x_value_decompress(tmp);
        frames[i].x = frames[i-1].x + diff; // x值拿过来
    }
}

void Period::decompress(const std::vector<int> &decompress_idxes, x_value &start, x_value &end){
    read_head();
    if(predict){
        uint16_t c_frames_number;
        input_fstream.read((char*)&c_frames_number, sizeof(c_frames_number));
        c_idxes.resize(c_frames_number);
        for(int i = 0;i < c_frames_number;++i){
            input_fstream.read((char*)&c_idxes[i], sizeof(c_idxes[i])); // 记录每个被压缩的帧的编号
        }
        std::vector<std::vector<compressed_diff>> compressed(c_frames_number);
        for(int i = 0;i < c_frames_number;++i){
            compressed[i].resize(signal_count);
            for(int j = 0;j < signal_count;++j){
                compressed_diff_write tmp;
                input_fstream.read((char*)&tmp, sizeof(tmp)); // 压缩后的差值
                compressed[j][i] = tmp;
            }
        }
        std::vector<std::vector<original_data>> decompressed(decompress_idxes.size());
        int decompressed_i = 0;
        for(auto decompress_idx:decompress_idxes){
            regulator->decompress(compressed[decompress_idx], diff_max[decompress_idx], decompressed[decompressed_i++]);
        }
        outputter.OutputXValue(frames[0].x);
        for(int idx:decompress_idxes){
            outputter.OutputSignalValue(base[idx]);
        }
        outputter.FinishOnePointData();
        int c_idx_i = 0; // 用以记录现在c_idx到哪了
        int decompress_number = decompress_idxes.size();
        for(int i = 1;i < frame_count;++i){
            outputter.OutputXValue(frames[i].x);
            std::vector<original_data> predict_values(decompress_number);
            for(int j = 0;j < decompress_number;++j){
                predict_values[j] = slope[decompress_idxes[j]] * (frames[i].x - base_time) + base[decompress_idxes[j]];
            }
            if(i == c_idxes[c_idx_i]){
                c_idx_i++;
                for(int j = 0;j < decompress_number;++j){
                    outputter.OutputSignalValue(predict_values[j] + decompressed[i][j]);
                }
            }
            else{
                for(int j = 0;j < decompress_number;++j){
                    outputter.OutputSignalValue(predict_values[j]);
                }
            }
            outputter.FinishOnePointData();
        }
    }
    else{
        // Reserved for Boris Johnson;
        std::vector<std::vector<compressed_diff>> compressed(frame_count);
        for(int i = 0;i < frame_count;++i){
            compressed[i].resize(signal_count);
            for(int j = 0;j < signal_count;++j){
                compressed_diff_write tmp;
                input_fstream.read((char*)&tmp, sizeof(tmp)); // 压缩后的差值
                compressed[j][i] = tmp;
            }
        }
        std::vector<std::vector<original_data>> decompressed(decompress_idxes.size());
        int decompressed_i = 0;
        for(auto decompress_idx:decompress_idxes){
            regulator->decompress(compressed[decompress_idx], diff_max[decompress_idx], decompressed[decompressed_i++]);
        }
        outputter.OutputXValue(frames[0].x);
        for(int idx:decompress_idxes){
            outputter.OutputSignalValue(base[idx]);
        }
        outputter.FinishOnePointData();
        int c_idx_i = 0; // 用以记录现在c_idx到哪了
        int decompress_number = decompress_idxes.size();
        for(int i = 1;i < frame_count;++i){
            outputter.OutputXValue(frames[i].x);
            std::vector<original_data> predict_values(decompress_number);
            for(int j = 0;j < decompress_number;++j){
                predict_values[j] = slope[decompress_idxes[j]] * (frames[i].x - base_time) + base[decompress_idxes[j]];
            }
            for(int j = 0;j < decompress_number;++j){
                outputter.OutputSignalValue(predict_values[j]);
            }
            outputter.FinishOnePointData();
        }
    }
    if(start == -1){
        start = base_time;
    }
    if(end == -1){
        end = frames[frame_count-1].x;
    }
}