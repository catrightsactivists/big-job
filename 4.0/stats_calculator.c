#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_struct.h"
#include "stats_calculator.h"

/*
 * 函数：calculate_player_efficiency
 * 功能：计算球员效率值（简化版PER算法）
 * 参数：player - 球员结构体指针
 * 返回值：效率值浮点数，越高表示效率越高
 * 算法公式：身高/10 + (200-体重)/5 + 球衣号/2 + 位置加权
 * 设计思路：综合考虑身体条件和位置特点
 */
float calculate_player_efficiency(const Player* player) {
    if (player == NULL) return 0.0f;            /* 空指针检查 */
    
    float efficiency = 0.0f;                    /* 初始化效率值 */
    
    /* 第一部分：身体素质评分（占60%） */
    efficiency += (float)player->height / 10.0f;       /* 身高贡献：每10cm得1分 */
    efficiency += (200.0f - player->weight) / 5.0f;    /* 体重贡献：接近标准体重得分高 */
    efficiency += (float)player->jersey / 2.0f;        /* 球衣号贡献：号码越大得分越高 */
    
    /* 第二部分：位置加权评分（占40%） */
    if (strcmp(player->position, "PG") == 0) {         /* 控球后卫加权 */
        efficiency += 5.0f;                            /* PG需要组织能力 */
    } else if (strcmp(player->position, "SG") == 0) {  /* 得分后卫加权 */
        efficiency += 6.0f;                            /* SG需要得分能力 */
    } else if (strcmp(player->position, "SF") == 0) {  /* 小前锋加权 */
        efficiency += 7.0f;                            /* SF需要全面能力 */
    } else if (strcmp(player->position, "PF") == 0) {  /* 大前锋加权 */
        efficiency += 8.0f;                            /* PF需要篮板能力 */
    } else if (strcmp(player->position, "C") == 0) {   /* 中锋加权 */
        efficiency += 9.0f;                            /* C需要防守能力 */
    }
    
    return efficiency;                                 /* 返回最终效率值 */
}

/*
 * 函数：calculate_team_average_stats
 * 功能：计算指定球队的平均统计数据
 * 参数：team_name - 球队名称，统计该队所有球员
 * 返回值：无，直接打印统计结果
 * 统计项：球员数量、平均身高、平均体重、平均效率值
 */
void calculate_team_average_stats(const char* team_name) {
    if (player_list == NULL) {                  /* 空链表检查 */
        printf("暂无球员数据\n");               /* 提示用户无数据 */
        return;                                 /* 提前返回 */
    }
    
    Player* current = player_list;              /* 从头节点开始遍历 */
    int player_count = 0;                       /* 球队球员计数器 */
    int total_height = 0;                       /* 总身高累加器 */
    int total_weight = 0;                       /* 总体重累加器 */
    float total_efficiency = 0.0f;              /* 总效率值累加器 */
    
    while (current != NULL) {                   /* 遍历整个链表 */
        if (strcmp(current->team, team_name) == 0) { /* 比较球队名称 */
            player_count++;                     /* 匹配球队，计数器递增 */
            total_height += current->height;    /* 累加身高 */
            total_weight += current->weight;    /* 累加体重 */
            total_efficiency += calculate_player_efficiency(current); /* 累加效率值 */
        }
        current = current->next;                /* 移动到下一个节点 */
    }
    
    if (player_count == 0) {                    /* 未找到该球队球员 */
        printf("未找到球队 %s 的球员数据\n", team_name);
        return;                                 /* 提前返回 */
    }
    
    /* 计算并输出各项平均值 */
    printf("\n=== 球队 %s 统计报告 ===\n", team_name);
    printf("球员数量：%d 人\n", player_count);
    printf("平均身高：%.1f cm\n", (float)total_height / player_count);
    printf("平均体重：%.1f kg\n", (float)total_weight / player_count);
    printf("平均效率值：%.1f 分\n", total_efficiency / player_count);
}

/*
 * 函数：find_most_efficient_player
 * 功能：在整个联赛中查找效率值最高的球员
 * 参数：无
 * 返回值：效率最高的球员指针，没有球员返回NULL
 * 算法：遍历链表，记录最大值，时间复杂度O(n)
 */
Player* find_most_efficient_player() {
    if (player_list == NULL) return NULL;       /* 空链表检查 */
    
    Player* current = player_list;              /* 当前遍历节点 */
    Player* best_player = player_list;          /* 当前最佳球员指针 */
    float max_efficiency = 0.0f;                /* 当前最高效率值 */
    
    while (current != NULL) {                   /* 遍历整个链表 */
        float efficiency = calculate_player_efficiency(current);
        if (efficiency > max_efficiency) {      /* 发现更高效率值 */
            max_efficiency = efficiency;        /* 更新最高效率值 */
            best_player = current;              /* 更新最佳球员指针 */
        }
        current = current->next;                /* 移动到下一个节点 */
    }
    
    return best_player;                         /* 返回效率最高的球员 */
}
