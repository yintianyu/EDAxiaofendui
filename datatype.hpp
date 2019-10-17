/*
 * File: datatype.hpp
 * Author: Tianyu Yin
 * Create Date: 2019/10/06
 * Description: define some data types
 */

#ifndef DATATYPE_HPP
#define DATATYPE_HPP

typedef double original_data;
typedef char compressed_diff; // Can be modify to short if 16 bits are needed
typedef double x_value;
typedef unsigned char index_value;


// Threshold define
#define EPSILON (1e-9) // threshold between C and P
#define YITA (ALPHA * 3 / 4)
#define BETA (1e-4)
#define ALPHA (30)

#define THRESHOLD_HOMO_INHOMO (1e-4) // 差值的最大值最小值之差的阈值，决定采用均匀量化还是非均匀量化


#endif