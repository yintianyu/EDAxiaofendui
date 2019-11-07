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
typedef unsigned int compressed_x;


// Threshold define
#define EPSILON (1e-9) // threshold between C and P
#define YITA ((int)(frames.size() * 3 / 4))
#define BETA (0.5)
#define ALPHA (255)

#define THRESHOLD_HOMO_INHOMO (1e-4) // 差值的最大值最小值之差的阈值，决定采用均匀量化还是非均匀量化

#define IDENTIFER (0x1234)

typedef enum {REGU_A, REGU_U, REGU_HOMO} REGULATION_TYPE; //这个Period采用的规约手段，A律，u律还是均匀量化

//typedef char compressed_diff; // Can be modify to short if 16 bits are needed

typedef std::bitset<16> compressed_diff;
typedef std::bitset<8>  compressed_diff_8bit;

typedef uint16_t compressed_diff_write; // 写入文件的类型，如果使用std::bitset<16> 则改为uint16_t

#define X_VALUE_STEP (4) // x value压缩时每次的放大倍数
#define X_VALUE_DEVIDE (8) // 有多少位用于存count
#define X_VALUE_UP (24) // 有多少位用于存val
#define X_VALUE_MAX (1 << X_VALUE_UP) // val最大值
#define X_COUNT_MASK (~((-1) << X_VALUE_DEVIDE))

#define X_STEP_BETA (1e2) // 如果和base的时间跨度比predict_step的BETA倍还多，就切下一个。

#define DEBUG_PERIOD (36) // 用于debug的打印周期
#endif