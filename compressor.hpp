/*
 * File: compressor.hpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/05
 * Description: The main function of the program
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
    Compressor(const std::string &input_filename, const std::string &output_filename):output_fstream(output_filename, std::ios::binary), 
    reader(input_filename), state_machine(nullptr){
        identifier = 0x1234;
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