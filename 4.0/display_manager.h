#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "data_struct.h"

/* Windows控制台颜色常量定义（16色模式） */
#define COLOR_DEFAULT 7         /* 默认白色前景+黑色背景 */
#define COLOR_TITLE 11          /* 亮青色，用于标题显示 */
#define COLOR_MENU 10           /* 亮绿色，用于菜单显示 */
#define COLOR_ERROR 12          /* 亮红色，用于错误提示 */
#define COLOR_SUCCESS 10        /* 亮绿色，用于成功提示 */
#define COLOR_HIGHLIGHT 14      /* 亮黄色，用于高亮显示 */
#define COLOR_BORDER 8          /* 灰色，用于边框显示 */

/*
 * 函数声明部分
 * 所有函数都包含详细的参数说明和功能描述
 */

/*
 * 函数：set_console_color
 * 功能：设置Windows控制台文本颜色
 * 参数：color_code - Windows控制台颜色代码
 */
void set_console_color(int color_code);

/*
 * 函数：display_player_card
 * 功能：以卡片形式美观显示单个球员详细信息
 * 参数：player - 要显示的球员结构体指针
 */
void display_player_card(const Player* player);

/*
 * 函数：display_all_players_table
 * 功能：以表格形式显示所有球员信息
 * 参数：无
 */
void display_all_players_table();

/*
 * 函数：display_statistics_report
 * 功能：显示详细的联赛统计数据报告
 * 参数：无
 */
void display_statistics_report();

#endif /* DISPLAY_MANAGER_H */
