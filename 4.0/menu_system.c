#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "display_manager.h"
#include "menu_system.h"

/*
 * 函数：clear_input_buffer
 * 功能：清空标准输入缓冲区中的残留字符
 * 参数：无
 * 作用：防止scanf等函数读取残留字符导致输入错误
 */
void clear_input_buffer() {
    int ch;                                      /* 临时字符变量 */
    while ((ch = getchar()) != '\n' && ch != EOF); /* 读取直到换行或文件尾 */
}

/*
 * 函数：display_welcome_animation
 * 功能：显示系统欢迎界面和加载动画
 * 参数：无
 * 特色：使用边框装饰、颜色渐变、动态加载效果
 */
void display_welcome_animation() {
    system("cls");                               /* 清屏命令，确保干净界面 */
    
    set_console_color(COLOR_TITLE);              /* 设置标题颜色 */
    printf("╔══════════════════════════════════════════════════════╗\n");
    printf("║                                                      ║\n");
    printf("║        大学生篮球联赛数据管理系统 v4.0               ║\n");
    printf("║                                                      ║\n");
    printf("║           Basketball Statistics System               ║\n");
    printf("║                                                      ║\n");
    printf("╚══════════════════════════════════════════════════════╝\n\n");
    
    set_console_color(COLOR_MENU);               /* 设置菜单颜色 */
    printf("系统版本：4.0 | 适用平台：Windows 10+\n");
    printf("========================================================\n\n");
    
    set_console_color(COLOR_DEFAULT);            /* 恢复默认颜色 */
    printf("正在初始化系统");                    /* 加载提示文本 */
    
    for (int i = 0; i < 5; i++) {                /* 循环5次产生动态效果 */
        printf(".");                             /* 输出进度点 */
        Sleep(200);                              /* 延时200毫秒 */
        fflush(stdout);                          /* 强制刷新输出缓冲区 */
    }
    
    printf(" 准备就绪！\n\n");                   /* 完成初始化提示 */
}

/*
 * 函数：display_main_menu
 * 功能：显示系统主菜单界面
 * 参数：无
 * 设计：使用Unicode边框字符，颜色区分，对齐准确
 */
void display_main_menu() {
    set_console_color(COLOR_MENU);               /* 设置菜单边框颜色 */
    printf("\n┌──────────────────────────────────────────────┐\n");
    printf("│                主 功 能 菜 单                │\n");
    printf("├──────────────────────────────────────────────┤\n");
    printf("│   1. 添加新球员信息                          │\n");
    printf("│   2. 查看所有球员列表                        │\n");
    printf("│   3. 按学号查找球员                          │\n");
    printf("│   4. 按姓名查找球员                          │\n");
    printf("│   5. 修改球员信息                            │\n");
    printf("│   6. 删除球员记录                            │\n");
    printf("│   7. 球员数据统计                            │\n");
    printf("│   8. 球队数据分析                            │\n");
    printf("│   9. 查找效率最高球员                        │\n");
    printf("│  10. 保存数据到文件                          │\n");
    printf("│  11. 从文件加载数据                          │\n");
    printf("│   0. 退出管理系统                            │\n");
    printf("└──────────────────────────────────────────────┘\n");
    set_console_color(COLOR_DEFAULT);            /* 恢复默认文本颜色 */
    printf("\n请输入功能编号 (0-11): ");         /* 用户输入提示 */
}
