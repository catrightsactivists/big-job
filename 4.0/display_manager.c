#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "data_struct.h"
#include "stats_calculator.h"
#include "display_manager.h"

/*
 * 函数：set_console_color
 * 功能：设置Windows控制台文本颜色
 * 参数：color_code - Windows控制台颜色代码
 * 设计思路：调用Windows API实现跨终端颜色兼容
 */
void set_console_color(int color_code) {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_handle, color_code);
}

/*
 * 函数：display_player_card
 * 功能：以卡片形式美观显示单个球员详细信息
 * 参数：player - 要显示的球员结构体指针
 * 设计：模拟球员信息卡片，包含边框、颜色、对齐
 */
void display_player_card(const Player* player) {
    if (player == NULL) {
        set_console_color(COLOR_ERROR);
        printf("错误：球员信息不存在或已删除！\n");
        set_console_color(COLOR_DEFAULT);
        return;
    }
    
    set_console_color(COLOR_HIGHLIGHT);
    printf("\n========== 球 员 详 细 信 息 ==========\n");
    set_console_color(COLOR_DEFAULT);
    
    float efficiency = calculate_player_efficiency(player);
    
    // 直接使用冒号对齐，不画边框
    printf("学号   : %s\n", player->id);
    printf("姓名   : %s\n", player->name);
    printf("球队   : %s\n", player->team);
    printf("位置   : %s\n", player->position);
    printf("身高   : %d cm\n", player->height);
    printf("体重   : %d kg\n", player->weight);
    printf("球衣   : #%d\n", player->jersey);
    printf("效率值 : %.1f 分\n", efficiency);
    
    set_console_color(COLOR_HIGHLIGHT);
    printf("========================================\n");
    set_console_color(COLOR_DEFAULT);
}

/*
 * 函数：display_all_players_table
 * 作者：gyx
 * 功能：以表格形式显示所有球员信息
 * 参数：无
 * 设计：使用冒号对齐，简洁清晰，完全适配Dev-C++ 5.11
 */
void display_all_players_table() {
    if (player_list == NULL) {                   /* 空链表检查 */
        set_console_color(COLOR_ERROR);          /* 设置错误提示颜色 */
        printf("\n当前系统中没有任何球员数据！\n");
        set_console_color(COLOR_DEFAULT);        /* 恢复默认颜色 */
        return;                                  /* 提前返回 */
    }
    
    set_console_color(COLOR_HIGHLIGHT);
    printf("\n===================== 所 有 球 员 列 表 =====================\n");
    set_console_color(COLOR_DEFAULT);
    
    Player* current = player_list;               /* 从头节点开始遍历 */
    int player_count = 0;                        /* 球员计数器 */
    
    while (current != NULL) {                    /* 遍历整个链表 */
        player_count++;                          /* 计数递增 */
        
        /* 显示球员序号 */
        set_console_color(COLOR_MENU);
        printf("\n[球员 #%d]\n", player_count);
        set_console_color(COLOR_DEFAULT);
        
        /* 使用固定格式冒号对齐，无需手动填充空格 */
        printf("学号    : %s\n", current->id);
        printf("姓名    : %s\n", current->name);
        printf("球队    : %s\n", current->team);
        printf("位置    : %s\n", current->position);
        printf("身高    : %d cm\n", current->height);
        printf("体重    : %d kg\n", current->weight);
        printf("球衣    : #%d\n", current->jersey);
        
        /* 计算并显示效率值 */
        float efficiency = calculate_player_efficiency(current);
        printf("效率值  : %.1f 分\n", efficiency);
        
        /* 显示分隔线（使用标准ASCII字符，确保兼容性） */
        if (current->next != NULL) {             /* 不是最后一个球员时显示分隔线 */
            set_console_color(COLOR_BORDER);     /* 使用边框颜色 */
            printf("-----------------------------------------------\n");
            set_console_color(COLOR_DEFAULT);    /* 恢复默认颜色 */
        }
        
        current = current->next;                 /* 移动到下一个节点 */
    }
    
    /* 显示统计信息 */
    set_console_color(COLOR_HIGHLIGHT);
    printf("\n=================== 共 %d 名球员 ===================\n", player_count);
    set_console_color(COLOR_DEFAULT);
}

/*
 * 函数：display_statistics_report
 * 作者：gyx
 * 功能：显示详细的联赛统计数据报告
 * 参数：无
 * 设计：分类统计，格式化输出，颜色区分
 */
void display_statistics_report() {
    if (player_list == NULL) {                   /* 空链表检查 */
        set_console_color(COLOR_ERROR);          /* 设置错误提示颜色 */
        printf("暂无球员数据可统计！\n");
        set_console_color(COLOR_DEFAULT);        /* 恢复默认颜色 */
        return;                                  /* 提前返回 */
    }
    
    Player* current = player_list;               /* 遍历指针初始化 */
    int total_players = 0;                       /* 总球员数 */
    int total_height = 0;                        /* 总身高累加 */
    int total_weight = 0;                        /* 总体重累加 */
    int max_height = 0, min_height = 300;        /* 身高极值初始化 */
    int max_weight = 0, min_weight = 300;        /* 体重极值初始化 */
    Player* tallest = NULL, * shortest = NULL;   /* 极值球员指针 */
    Player* heaviest = NULL, * lightest = NULL;  /* 体重极值球员指针 */
    
    /* 第一次遍历：收集基础统计数据 */
    while (current != NULL) {                    /* 遍历整个链表 */
        total_players++;                         /* 球员总数递增 */
        total_height += current->height;         /* 累加身高 */
        total_weight += current->weight;         /* 累加体重 */
        
        /* 更新身高极值和对应球员 */
        if (current->height > max_height) {      /* 发现新的最大身高 */
            max_height = current->height;        /* 更新最大值 */
            tallest = current;                   /* 更新最高球员指针 */
        }
        if (current->height < min_height) {      /* 发现新的最小身高 */
            min_height = current->height;        /* 更新最小值 */
            shortest = current;                  /* 更新最矮球员指针 */
        }
        
        /* 更新体重极值和对应球员 */
        if (current->weight > max_weight) {      /* 发现新的最大体重 */
            max_weight = current->weight;        /* 更新最大值 */
            heaviest = current;                  /* 更新最重球员指针 */
        }
        if (current->weight < min_weight) {      /* 发现新的最小体重 */
            min_weight = current->weight;        /* 更新最小值 */
            lightest = current;                  /* 更新最轻球员指针 */
        }
        
        current = current->next;                 /* 移动到下一个节点 */
    }
    
    /* 计算平均值 */
    float avg_height = (float)total_height / total_players;
    float avg_weight = (float)total_weight / total_players;
    
    /* 第二次遍历：统计各位置球员数量 */
    current = player_list;                       /* 重置遍历指针 */
    int position_counts[5] = {0};                /* 位置计数器数组初始化 */
    
    /* 修改这里：简化位置名称定义，避免Dev-C++解析问题 */
    /* 使用5个独立的字符串变量，而不是数组 */
    const char* pos1 = "PG";
    const char* pos2 = "SG";
    const char* pos3 = "SF";
    const char* pos4 = "PF";
    const char* pos5 = "C";
    
    while (current != NULL) {                    /* 再次遍历链表 */
        /* 遍历5个标准位置 - 逐个比较 */
        if (strcmp(current->position, pos1) == 0) {
            position_counts[0]++;
        } else if (strcmp(current->position, pos2) == 0) {
            position_counts[1]++;
        } else if (strcmp(current->position, pos3) == 0) {
            position_counts[2]++;
        } else if (strcmp(current->position, pos4) == 0) {
            position_counts[3]++;
        } else if (strcmp(current->position, pos5) == 0) {
            position_counts[4]++;
        }
        current = current->next;                 /* 移动到下一个节点 */
    }
    
    /* 输出统计报告 */
    set_console_color(COLOR_HIGHLIGHT);          /* 设置报告标题颜色 */
    printf("\n=== 联赛数据统计报告 ===\n");
    
    set_console_color(COLOR_DEFAULT);            /* 设置报告内容颜色 */
    printf("1. 球员总数：%d 人\n", total_players);
    printf("2. 身高统计：\n");
    printf("   平均身高：%.1f cm\n", avg_height);
    printf("   最高球员：%s (%d cm)\n", tallest->name, tallest->height);
    printf("   最矮球员：%s (%d cm)\n", shortest->name, shortest->height);
    printf("3. 体重统计：\n");
    printf("   平均体重：%.1f kg\n", avg_weight);
    printf("   最重球员：%s (%d kg)\n", heaviest->name, heaviest->weight);
    printf("   最轻球员：%s (%d kg)\n", lightest->name, lightest->weight);
    printf("4. 位置分布：\n");
    /* 输出位置分布时也使用独立变量 */
    printf("   %s：%d 人 (%.1f%%)\n", pos1, position_counts[0], 
           (float)position_counts[0] / total_players * 100);
    printf("   %s：%d 人 (%.1f%%)\n", pos2, position_counts[1], 
           (float)position_counts[1] / total_players * 100);
    printf("   %s：%d 人 (%.1f%%)\n", pos3, position_counts[2], 
           (float)position_counts[2] / total_players * 100);
    printf("   %s：%d 人 (%.1f%%)\n", pos4, position_counts[3], 
           (float)position_counts[3] / total_players * 100);
    printf("   %s：%d 人 (%.1f%%)\n", pos5, position_counts[4], 
           (float)position_counts[4] / total_players * 100);
}
