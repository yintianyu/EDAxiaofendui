/*
 * File: datatype.hpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/06
 * Description: define some data types
 */

#ifndef DATATYPE_HPP
#define DATATYPE_HPP
#include <bitset>
#include <cassert>

typedef double original_data;
typedef double x_value;
typedef unsigned char index_value;
typedef unsigned short compressed_x;


// Threshold define
#define EPSILON (1e-9) // threshold between C and P
#define YITA ((int)(frames.size() * 3 / 4))
#define BETA (1)
#define ALPHA (256)

#define THRESHOLD_HOMO_INHOMO (1e-4) // 差值的最大值最小值之差的阈值，决定采用均匀量化还是非均匀量化

#define IDENTIFER (0x1234)

//typedef char compressed_diff; // Can be modify to short if 16 bits are needed

typedef std::bitset<16> compressed_diff;
typedef std::bitset<8>  compressed_diff_8bit;

typedef uint8_t compressed_diff_write; // 写入文件的类型，如果使用std::bitset<16> 则改为uint16_t

#define X_VALUE_STEP (16) // x value压缩时每次的放大倍数

#endif