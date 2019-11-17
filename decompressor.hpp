/*
 * File: decompressor.hpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/28
 * Description: Define Decompressor Class
 */

#ifndef DECOMPRESSOR_HPP
#define DECOMPRESSOR_HPP

#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <mutex>
#include "datatype.hpp"
#include "outputter/WaveformOutputterForCompetition.h"
#include "outputter/BasicException.h"

class Decompressor{
    public:
    Decompressor(const std::string &input_filename, const std::string &output_filename);
    void decompress(const std::vector<std::string> &names);
    ~Decompressor(){
        input_fstream.close();
    }
    private:
    std::ifstream input_fstream;
    WaveformOutputterForCompetition outputter;
    int identifier_expect;
    int signal_count;
    std::vector<std::string> signal_names;
    std::vector<int> decompress_idxes;
    int frame_count;
    void _ready(int write_number, int &current_frame);
    void _check(bool &ready, int &write_number);
    std::vector<std::queue<original_data>> output_buffer; // 缓存即将输出到文件中的内容

    std::mutex buffer_mutex; // 用来管理output_buffer的锁


    int read_metadata();
    void write_data_to_file();
};

#endif
