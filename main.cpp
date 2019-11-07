/*
 * File: main.cpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/05
 * Description: The main function of the program
 */

#include <iostream>
#include <string>
#include "compressor.hpp"
#include "decompressor.hpp"
#include "datatype.hpp"
#include "period.hpp"
#include "state_machine.hpp"
#include <cstdio>
#include <iomanip>

int main(int argc, char* argv[]){
    std::string input_filename;
    std::string output_filename;
    std::cout<<std::setprecision(15);
    if(argc < 3){ //only for debug
        input_filename = "cases/test1.wv";
        output_filename = "cases/test1.wv.edaxfd";
    }
    else{
        input_filename = argv[1];
        output_filename = argv[2];
    }

    std::vector<std::string> signal_names;
    Compressor *compressor = new Compressor(input_filename, output_filename);
    compressor->compress();
    compressor->get_signal_names(signal_names);
    delete compressor;
    std::cout << "Compress Complete" << std::endl;
    Decompressor *decompressor = new Decompressor(output_filename, input_filename+".deedaxfd");
    decompressor->decompress(signal_names);
    std::cout << "Decompress Complete" << std::endl;
    return 0;
}

// int main() {
//     Homo_Regulation h;
//     std::vector<original_data> a1;
//     std::vector<compressed_diff> a2;
//     std::vector<original_data> a3;
//     double amp = 5.0;
//     for (int i = 20; i < 35; ++i) {
//         double t = amp * sin(i * 3.14 / 20);
//         a1.push_back(t);
//     }
//     h.compress(a1, amp, a2);
//     h.decompress(a2, amp, a3);
//     auto a3ptr = a3.begin();
//     for (auto i = a2.begin(); i != a2.end(); ++i) {
//         std::cout << *i << std::endl;
//     }
//     for (auto i = a1.begin(); i != a1.end(); ++i) {
//         double diff = abs(*a3ptr - *i) * 100;
//         std::cout << *i << "    " << *a3ptr << "    "  << diff << "%" << std::endl;
//         ++a3ptr;
//     }
//     system("pause");
//     return 0;
// }
