#ifndef STATS_CALCULATOR_H
#define STATS_CALCULATOR_H

#include "data_struct.h"

/*
 * 函数声明部分
 * 所有函数都包含详细的参数说明和功能描述
 */

/*
 * 函数：calculate_player_efficiency
 * 功能：计算球员效率值（简化版PER算法）
 * 参数：player - 球员结构体指针
 * 返回值：效率值浮点数，越高表示效率越高
 */
float calculate_player_efficiency(const Player* player);

/*
 * 函数：calculate_team_average_stats
 * 功能：计算指定球队的平均统计数据
 * 参数：team_name - 球队名称，统计该队所有球员
 * 返回值：无，直接打印统计结果
 */
void calculate_team_average_stats(const char* team_name);

/*
 * 函数：find_most_efficient_player
 * 功能：在整个联赛中查找效率值最高的球员
 * 参数：无
 * 返回值：效率最高的球员指针，没有球员返回NULL
 */
Player* find_most_efficient_player();

#endif /* STATS_CALCULATOR_H */
