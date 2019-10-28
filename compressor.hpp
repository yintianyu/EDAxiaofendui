/*
 * File: compressor.hpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/05
 * Description: Define Compressor Class.
 */

#ifndef COMPRESSOR_HPP
#define COMPRESSOR_HPP

#include <fstream>
#include <string>
#include <vector>
#include "datatype.hpp"
#include "state_machine.hpp"
#include "reader/WaveformReaderForCompetition.h"

class Compressor{
    public:
    Compressor(const std::string &input_filename, const std::string &output_filename):reader(input_filename), output_fstream(output_filename, std::ios::binary), state_machine(nullptr){
        identifier = IDENTIFER;
    }
    void compress();
    ~Compressor(){
        output_fstream.close();
    }
    private:
    WaveformReaderForCompetition reader;
    std::vector<std::string> signal_names;
    std::ofstream output_fstream;
    State_Machine *state_machine;
    int signal_count;

    int identifier;

    void write_metadata_to_file();
};

#endif