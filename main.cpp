/*
 * File: main.cpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/05
 * Description: The main function of the program
 */

#include <iostream>
#include <string>
#include "compressor.hpp"

int main(int argc, char* argv[]){
    if(argc < 3){
        std::cerr << "Invalid arguments" << std::endl;
        exit(-1);
    }
    std::string input_filename(argv[1]);
    std::string output_filename(argv[2]);
    Compressor *compressor = new Compressor(input_filename, output_filename);
    compressor->compress();
    delete compressor;
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
