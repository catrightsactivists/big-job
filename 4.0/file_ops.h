#ifndef FILE_OPS_H
#define FILE_OPS_H

#include "data_struct.h"

/*
 * 函数声明部分
 * 所有函数都包含详细的参数说明和功能描述
 */

/*
 * 函数：save_player_data_to_file
 * 功能：将内存中的球员链表数据保存到二进制文件
 * 参数：filename - 目标文件名
 * 返回值：成功保存的记录数，失败返回0
 */
int save_player_data_to_file(const char* filename);

/*
 * 函数：load_player_data_from_file
 * 功能：从二进制文件加载球员数据到内存链表
 * 参数：filename - 源文件名
 * 返回值：成功加载的记录数，文件不存在返回0
 */
int load_player_data_from_file(const char* filename);

/*
 * 函数：validate_player_data
 * 功能：验证球员数据的完整性和合法性
 * 参数：player - 待验证的球员结构体指针
 * 返回值：1表示数据合法，0表示数据非法
 */
int validate_player_data(const Player* player);

#endif /* FILE_OPS_H */
