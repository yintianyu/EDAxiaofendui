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
typedef float compressed_x;
typedef double original_data_write;

typedef int x_value_idx;


// Threshold define
#define EPSILON (1e-8) // threshold between C and P
#define YITA ((int)(frames.size() * 3 / 4))
#define BETA (15)
#define ALPHA (255)

#define THRESHOLD_DIFF_DATA_RATIO (1e-2) // 如果diff和data的比值超过该值，则重新分P

#define THRESHOLD_HOMO_INHOMO (1e-4) // 差值的最大值最小值之差的阈值，决定采用均匀量化还是非均匀量化

#define IDENTIFIER (0x1234)

typedef enum {REGU_A, REGU_U, REGU_HOMO} REGULATION_TYPE; //这个Period采用的规约手段，A律，u律还是均匀量化
typedef unsigned char regulation_type_write;

//typedef char compressed_diff; // Can be modify to short if 16 bits are needed

typedef std::bitset<24> compressed_diff;
typedef std::bitset<8>  compressed_diff_8bit;

typedef uint16_t compressed_diff_write1; // 写入文件的类型，如果使用std::bitset<16> 则改为uint16_t
typedef uint8_t compressed_diff_write2; // 写入文件的类型，如果使用std::bitset<16> 则改为uint16_t

#define X_VALUE_STEP (4) // x value压缩时每次的放大倍数
#define X_VALUE_DEVIDE (8) // 有多少位用于存count
#define X_VALUE_UP (24) // 有多少位用于存val
#define X_VALUE_MAX (1 << X_VALUE_UP) // val最大值
#define X_COUNT_MASK (~((-1) << X_VALUE_DEVIDE))

#define SLOPE_ERROR_BETA (1.5) // 斜率的相对误差阈值
#define SLOPE_ZERO (100) // 斜率绝对值为0的阈值


#define DEBUG_PERIOD (129) // 用于debug的打印周期
#define DEBUG_SIGNAL (1) // 用于debug的信号

#define DEBUG_TIME (1.953487817764674e-08) // 用于debug的时间
#define DEBUG_TIME_RANGE (1e-8) // 用于debug的时间范围
#endif