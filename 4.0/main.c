/* 
 * ============================================================================
 * 主程序模块：系统入口和流程控制
 * 项目名称：大学生篮球联赛数据管理系统
 * 开发团队：pzh(架构师) lyl(算法工程师) gyx(UI设计师)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_ops.h"
#include "player_manager.h"
#include "stats_calculator.h"
#include "display_manager.h"
#include "menu_system.h"

/*
 * 函数：main
 * 作者：项目组
 * 功能：程序主入口，控制整体流程
 * 设计：无限循环菜单，根据用户选择调用相应功能
 */
int main() {
    display_welcome_animation();                 /* 显示欢迎界面 */
    load_player_data_from_file("players.dat");   /* 启动时加载数据 */
    
    int user_choice = -1;                        /* 用户选择变量初始化 */
    char input_buffer[100];                      /* 输入缓冲区 */
    
    do {
        display_main_menu();                     /* 显示主菜单 */
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;                               /* 输入错误或EOF时退出 */
        }
        
        user_choice = atoi(input_buffer);        /* 转换输入为整数 */
        
        switch (user_choice) {                   /* 根据用户选择分支 */
            case 1: {  /* 添加新球员 */
                char id[13], name[21], team[31], position[11];
                int height, weight, jersey;
                
                printf("\n=== 添加新球员 ===\n");
                printf("请输入12位学号: ");
                scanf("%12s", id);
                clear_input_buffer();
                
                printf("请输入姓名: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;    /* 去除换行符 */
                
                printf("请输入球队: ");
                fgets(team, sizeof(team), stdin);
                team[strcspn(team, "\n")] = 0;    /* 去除换行符 */
                
                printf("请输入位置(PG/SG/SF/PF/C): ");
                scanf("%10s", position);
                clear_input_buffer();
                
                printf("请输入身高(cm): ");
                scanf("%d", &height);
                clear_input_buffer();
                
                printf("请输入体重(kg): ");
                scanf("%d", &weight);
                clear_input_buffer();
                
                printf("请输入球衣号码(0-99): ");
                scanf("%d", &jersey);
                clear_input_buffer();
                
                Player* new_player = add_new_player(id, name, team, position, 
                                                   height, weight, jersey);
                if (new_player != NULL) {
                    set_console_color(COLOR_SUCCESS);
                    printf("? 球员添加成功！\n");
                    display_player_card(new_player);
                    set_console_color(COLOR_DEFAULT);
                } else {
                    set_console_color(COLOR_ERROR);
                    printf("? 球员添加失败！可能原因：学号重复或数据不合法\n");
                    set_console_color(COLOR_DEFAULT);
                }
                break;
            }
                
            case 2:  /* 查看所有球员 */
                display_all_players_table();
                break;
                
            case 3: {  /* 按学号查找 */
                char id[13];
                printf("\n请输入学号: ");
                scanf("%12s", id);
                clear_input_buffer();
                
                Player* found = search_player_by_id(id);
                if (found != NULL) {
                    display_player_card(found);
                } else {
                    set_console_color(COLOR_ERROR);
                    printf("未找到学号为 %s 的球员\n", id);
                    set_console_color(COLOR_DEFAULT);
                }
                break;
            }
                
            case 4: {  /* 按姓名查找 */
                char name[21];
                printf("\n请输入姓名: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;    /* 去除换行符 */
                
                Player* found = search_player_by_name(name);
                if (found != NULL) {
                    display_player_card(found);
                } else {
                    set_console_color(COLOR_ERROR);
                    printf("未找到姓名为 %s 的球员\n", name);
                    set_console_color(COLOR_DEFAULT);
                }
                break;
            }
            
            case 5: {  /* 修改球员信息 */
    char id[13];
    char new_name[21], new_team[31], new_position[11];
    int new_height, new_weight, new_jersey;
    int choice;
    
    printf("\n=== 修改球员信息 ===\n");
    printf("请输入要修改的球员学号: ");
    scanf("%12s", id);
    clear_input_buffer();
    
    Player* player = search_player_by_id(id);
    if (player == NULL) {
        set_console_color(COLOR_ERROR);
        printf("未找到学号为 %s 的球员\n", id);
        set_console_color(COLOR_DEFAULT);
        break;
    }
    
    printf("\n当前球员信息：\n");
    display_player_card(player);
    
    printf("\n请选择要修改的字段：\n");
    printf("1. 姓名\n");
    printf("2. 球队\n");
    printf("3. 位置\n");
    printf("4. 身高\n");
    printf("5. 体重\n");
    printf("6. 球衣号\n");
    printf("0. 取消\n");
    printf("请输入选择 (0-6): ");
    
    scanf("%d", &choice);
    clear_input_buffer();
    
    if (choice == 0) {
        printf("修改已取消\n");
        break;
    }
    
    Player temp_player;  /* 临时存储修改前的数据 */
    memcpy(&temp_player, player, sizeof(Player));
    
    switch (choice) {
        case 1:
            printf("请输入新姓名: ");
            fgets(new_name, sizeof(new_name), stdin);
            new_name[strcspn(new_name, "\n")] = 0;
            strcpy(player->name, new_name);
            break;
        case 2:
            printf("请输入新球队: ");
            fgets(new_team, sizeof(new_team), stdin);
            new_team[strcspn(new_team, "\n")] = 0;
            strcpy(player->team, new_team);
            break;
        case 3:
            printf("请输入新位置(PG/SG/SF/PF/C): ");
            scanf("%10s", new_position);
            clear_input_buffer();
            strcpy(player->position, new_position);
            break;
        case 4:
            printf("请输入新身高(cm): ");
            scanf("%d", &new_height);
            clear_input_buffer();
            player->height = new_height;
            break;
        case 5:
            printf("请输入新体重(kg): ");
            scanf("%d", &new_weight);
            clear_input_buffer();
            player->weight = new_weight;
            break;
        case 6:
            printf("请输入新球衣号(0-99): ");
            scanf("%d", &new_jersey);
            clear_input_buffer();
            player->jersey = new_jersey;
            break;
        default:
            set_console_color(COLOR_ERROR);
            printf("无效选择\n");
            set_console_color(COLOR_DEFAULT);
            break;
    }
    
    /* 验证修改后的数据 */
    if (choice >= 1 && choice <= 6) {
        if (validate_player_data(player)) {
            set_console_color(COLOR_SUCCESS);
            printf("修改成功！\n");
            set_console_color(COLOR_DEFAULT);
        } else {
            /* 验证失败，恢复原数据 */
            memcpy(player, &temp_player, sizeof(Player));
            set_console_color(COLOR_ERROR);
            printf("修改失败！数据不合法，已恢复原数据\n");
            set_console_color(COLOR_DEFAULT);
        }
    }
    break;
}
                
            case 6: {  /* 删除球员 */
                char id[13];
                printf("\n请输入要删除的球员学号: ");
                scanf("%12s", id);
                clear_input_buffer();
                
                if (delete_player_by_id(id)) {
                    set_console_color(COLOR_SUCCESS);
                    printf("? 球员删除成功！\n");
                    set_console_color(COLOR_DEFAULT);
                } else {
                    set_console_color(COLOR_ERROR);
                    printf("? 删除失败！未找到该球员\n");
                    set_console_color(COLOR_DEFAULT);
                }
                break;
            }
                
            case 7:  /* 数据统计 */
                display_statistics_report();
                break;
                
            case 8: {  /* 球队分析 */
                char team_name[31];
                printf("\n请输入要分析的球队名称: ");
                fgets(team_name, sizeof(team_name), stdin);
                team_name[strcspn(team_name, "\n")] = 0;
                calculate_team_average_stats(team_name);
                break;
            }
                
            case 9: {  /* 效率最高球员 */
                Player* best = find_most_efficient_player();
                if (best != NULL) {
                    set_console_color(COLOR_HIGHLIGHT);
                    printf("\n=== 效率最高球员 ===\n");
                    set_console_color(COLOR_DEFAULT);
                    display_player_card(best);
                } else {
                    set_console_color(COLOR_ERROR);
                    printf("暂无球员数据\n");
                    set_console_color(COLOR_DEFAULT);
                }
                break;
            }
                
            case 10:  /* 保存数据 */
                if (save_player_data_to_file("players.dat") > 0) {
                    set_console_color(COLOR_SUCCESS);
                    printf("? 数据保存成功！\n");
                    set_console_color(COLOR_DEFAULT);
                } else {
                    set_console_color(COLOR_ERROR);
                    printf("? 数据保存失败！\n");
                    set_console_color(COLOR_DEFAULT);
                }
                break;
                
            case 11:  /* 加载数据 */
                if (load_player_data_from_file("players.dat") > 0) {
                    set_console_color(COLOR_SUCCESS);
                    printf("? 数据加载成功！\n");
                    set_console_color(COLOR_DEFAULT);
                } else {
                    set_console_color(COLOR_ERROR);
                    printf("注意：数据文件不存在或为空\n");
                    set_console_color(COLOR_DEFAULT);
                }
                break;
                
            case 0:  /* 退出系统 */
                if (save_player_data_to_file("players.dat") > 0) {
                    set_console_color(COLOR_SUCCESS);
                    printf("? 数据已自动保存\n");
                }
                set_console_color(COLOR_HIGHLIGHT);
                printf("感谢使用大学生篮球联赛数据管理系统！\n");
                set_console_color(COLOR_DEFAULT);
                break;
                
            default:  /* 无效输入 */
                set_console_color(COLOR_ERROR);
                printf("错误：请输入0-11之间的有效编号！\n");
                set_console_color(COLOR_DEFAULT);
        }
        
        if (user_choice != 0) {                  /* 非退出选项时暂停 */
            printf("\n按回车键返回主菜单...");
            getchar();                           /* 等待用户按键 */
        }
        
    } while (user_choice != 0);                  /* 用户选择0时退出循环 */
    
    return 0;                                    /* 程序正常退出 */
}
