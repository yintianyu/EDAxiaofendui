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
    Compressor(const std::string &input_filename, const std::string &output_filename):reader(input_filename), reader_x(input_filename), output_fstream(output_filename, std::ios::binary), state_machines(nullptr){
        identifier = IDENTIFIER;
    }
    void compress();
    void get_signal_names(std::vector<std::string> &output_signal_names);
    ~Compressor(){
        output_fstream.close();
    }
    private:
    WaveformReaderForCompetition reader;
    WaveformReaderForCompetition reader_x;
    std::vector<std::string> signal_names;
    std::ofstream output_fstream;
    std::vector<State_Machine> *state_machines;
    std::vector<x_value> *x_values;
    int signal_count;

    int identifier;

    void write_metadata_to_file();
};

#endif