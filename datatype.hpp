/*
 * File: datatype.hpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/06
 * Description: define some data types
 */

#ifndef DATATYPE_HPP
#define DATATYPE_HPP
#include <bitset>

typedef double original_data;
//typedef char compressed_diff; // Can be modify to short if 16 bits are needed
typedef std::bitset<8> compressed_diff;      

#endif