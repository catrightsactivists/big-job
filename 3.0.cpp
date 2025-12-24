#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* 
 * ============================================================================
 * pzh负责模块：系统架构师 - 数据结构和文件操作
 * ============================================================================
 */

/*
 * 球员信息结构体定义
 * id: 学号(12位数字字符) + 结束符 = 13字节
 * name: 姓名(最大10汉字或20英文字符) + 结束符 = 21字节
 * team: 球队名称(最大15汉字或30英文字符) + 结束符 = 31字节
 * position: 场上位置(5种: PG/SG/SF/PF/C) + 结束符 = 11字节
 * height: 身高(cm)，正常范围100-250
 * weight: 体重(kg)，正常范围40-200
 * jersey: 球衣号码，范围0-99
 * next: 链表指针，用于构建球员数据链表
 */
typedef struct Player {
    char id[13];            /* 学号字段，固定12位数字 */
    char name[21];          /* 姓名字段，存储球员全名 */
    char team[31];          /* 球队字段，记录所属学院或队伍 */
    char position[11];      /* 位置字段，记录球员场上位置 */
    int height;             /* 身高字段，单位厘米 */
    int weight;             /* 体重字段，单位千克 */
    int jersey;             /* 球衣字段，球员背号 */
    struct Player* next;    /* 指针字段，指向下一个球员节点 */
} Player;

Player* player_list = NULL; /* 全局球员链表头指针，初始化为空 */

/*
 * 函数：save_player_data_to_file
 * 作者：pzh
 * 功能：将内存中的球员链表数据保存到二进制文件
 * 参数：filename - 目标文件名
 * 返回值：成功保存的记录数，失败返回0
 * 设计思路：采用二进制写入方式，遍历链表逐个保存
 */
int save_player_data_to_file(const char* filename) {
    FILE* file_ptr = fopen(filename, "wb");  /* 以二进制写模式打开文件 */
    if (file_ptr == NULL) {                  /* 文件打开失败检查 */
        return 0;                            /* 返回0表示操作失败 */
    }
    
    Player* current = player_list;           /* 从头节点开始遍历 */
    int saved_count = 0;                     /* 计数器：已保存记录数 */
    
    while (current != NULL) {                /* 遍历链表直到末尾 */
        size_t write_result = fwrite(current, sizeof(Player), 1, file_ptr);
        if (write_result != 1) {             /* 写入失败检查 */
            fclose(file_ptr);                /* 关闭文件句柄 */
            return 0;                        /* 返回0表示操作失败 */
        }
        current = current->next;             /* 移动到下一个节点 */
        saved_count++;                       /* 成功计数器递增 */
    }
    
    fclose(file_ptr);                        /* 关闭文件句柄释放资源 */
    return saved_count;                      /* 返回成功保存的记录数 */
}

/*
 * 函数：load_player_data_from_file
 * 作者：pzh
 * 功能：从二进制文件加载球员数据到内存链表
 * 参数：filename - 源文件名
 * 返回值：成功加载的记录数，文件不存在返回0
 * 设计思路：先清空现有链表，再从文件读取重建链表
 */
int load_player_data_from_file(const char* filename) {
    FILE* file_ptr = fopen(filename, "rb");  /* 以二进制读模式打开文件 */
    if (file_ptr == NULL) {                  /* 文件不存在或打开失败 */
        return 0;                            /* 返回0表示无数据加载 */
    }
    
    /* 清空现有链表，防止内存泄漏 */
    Player* current = player_list;           /* 获取当前链表头指针 */
    while (current != NULL) {                /* 遍历整个链表 */
        Player* temp = current;              /* 保存当前节点指针 */
        current = current->next;             /* 移动到下一个节点 */
        free(temp);                          /* 释放当前节点内存 */
    }
    player_list = NULL;                      /* 重置链表头指针为空 */
    
    Player temp_buffer;                      /* 临时缓冲区用于读取数据 */
    Player* last_node = NULL;                /* 记录链表最后一个节点 */
    int loaded_count = 0;                    /* 计数器：已加载记录数 */
    
    /* 循环读取文件中的球员记录 */
    while (fread(&temp_buffer, sizeof(Player), 1, file_ptr) == 1) {
        Player* new_node = (Player*)malloc(sizeof(Player));
        if (new_node == NULL) {              /* 内存分配失败检查 */
            break;                           /* 终止循环，返回已加载数 */
        }
        
        memcpy(new_node, &temp_buffer, sizeof(Player));
        new_node->next = NULL;               /* 新节点next指针置空 */
        
        if (player_list == NULL) {           /* 链表为空时的处理 */
            player_list = new_node;          /* 新节点作为头节点 */
        } else {                             /* 链表非空时的处理 */
            last_node->next = new_node;      /* 将新节点链接到末尾 */
        }
        last_node = new_node;                /* 更新末尾节点指针 */
        loaded_count++;                      /* 成功计数器递增 */
    }
    
    fclose(file_ptr);                        /* 关闭文件句柄释放资源 */
    return loaded_count;                     /* 返回成功加载的记录数 */
}

/*
 * 函数：validate_player_data
 * 作者：pzh
 * 功能：验证球员数据的完整性和合法性
 * 参数：player - 待验证的球员结构体指针
 * 返回值：1表示数据合法，0表示数据非法
 * 验证规则：学号12位、姓名非空、身高体重在合理范围、球衣号有效
 */
int validate_player_data(const Player* player) {
    if (strlen(player->id) != 12) return 0;    /* 学号必须为12位 */
    if (strlen(player->name) == 0) return 0;   /* 姓名不能为空字符串 */
    if (player->height < 100 || player->height > 250) return 0; /* 身高范围检查 */
    if (player->weight < 40 || player->weight > 200) return 0;  /* 体重范围检查 */
    if (player->jersey < 0 || player->jersey > 99) return 0;    /* 球衣号范围检查 */
    
    /* 验证场上位置是否合法（必须是5个标准位置之一） */
    const char* valid_positions[] = {"PG", "SG", "SF", "PF", "C"};
    int position_valid = 0;                     /* 位置有效性标志 */
    for (int i = 0; i < 5; i++) {               /* 遍历合法位置数组 */
        if (strcmp(player->position, valid_positions[i]) == 0) {
            position_valid = 1;                 /* 找到匹配位置，标记有效 */
            break;                              /* 提前结束循环 */
        }
    }
    if (!position_valid) return 0;              /* 位置无效返回错误 */
    
    return 1;                                   /* 所有验证通过返回成功 */
}

/* 
 * ============================================================================
 * lyl负责模块：算法工程师 - 球员管理和统计算法
 * ============================================================================
 */

/*
 * 函数：add_new_player
 * 作者：lyl
 * 功能：向球员链表添加新球员记录（头插法）
 * 参数：id-学号, name-姓名, team-球队, position-位置, height-身高, weight-体重, jersey-球衣号
 * 返回值：成功返回新节点指针，失败返回NULL
 * 算法：先查重后插入，时间复杂度O(n)
 */
Player* add_new_player(const char* id, const char* name, const char* team, 
                      const char* position, int height, int weight, int jersey) {
    /* 第一步：查重操作，确保学号唯一性 */
    Player* current = player_list;              /* 从头节点开始遍历 */
    while (current != NULL) {                   /* 遍历整个链表 */
        if (strcmp(current->id, id) == 0) {     /* 比较学号是否相同 */
            return NULL;                        /* 学号重复，返回失败 */
        }
        current = current->next;                /* 移动到下一个节点 */
    }
    
    /* 第二步：内存分配，创建新节点 */
    Player* new_player = (Player*)malloc(sizeof(Player));
    if (new_player == NULL) {                   /* 内存分配失败检查 */
        return NULL;                            /* 返回NULL表示失败 */
    }
    
    /* 第三步：数据填充，复制参数到新节点 */
    strcpy(new_player->id, id);                 /* 复制学号字符串 */
    strcpy(new_player->name, name);             /* 复制姓名字符串 */
    strcpy(new_player->team, team);             /* 复制球队字符串 */
    strcpy(new_player->position, position);     /* 复制位置字符串 */
    new_player->height = height;                /* 赋值身高数值 */
    new_player->weight = weight;                /* 赋值体重数值 */
    new_player->jersey = jersey;                /* 赋值球衣号码 */
    
    /* 第四步：数据验证，确保输入合法性 */
    if (!validate_player_data(new_player)) {    /* 调用验证函数 */
        free(new_player);                       /* 验证失败释放内存 */
        return NULL;                            /* 返回NULL表示失败 */
    }
    
    /* 第五步：链表插入，采用头插法提高效率 */
    new_player->next = player_list;             /* 新节点指向原头节点 */
    player_list = new_player;                   /* 更新头节点为新节点 */
    
    return new_player;                          /* 返回新节点指针 */
}

/*
 * 函数：search_player_by_id
 * 作者：lyl
 * 功能：通过学号在链表中查找球员（线性搜索）
 * 参数：id - 要查找的学号字符串
 * 返回值：找到返回球员指针，未找到返回NULL
 * 时间复杂度：O(n)，n为链表长度
 */
Player* search_player_by_id(const char* id) {
    Player* current = player_list;              /* 从头节点开始遍历 */
    while (current != NULL) {                   /* 遍历整个链表 */
        if (strcmp(current->id, id) == 0) {     /* 字符串比较学号 */
            return current;                     /* 找到匹配，返回指针 */
        }
        current = current->next;                /* 移动到下一个节点 */
    }
    return NULL;                                /* 遍历完毕未找到 */
}

/*
 * 函数：search_player_by_name
 * 作者：lyl
 * 功能：通过姓名在链表中查找球员（精确匹配）
 * 参数：name - 要查找的姓名字符串
 * 返回值：找到返回球员指针，未找到返回NULL
 * 注：此函数为精确匹配，后续可扩展为模糊搜索
 */
Player* search_player_by_name(const char* name) {
    Player* current = player_list;              /* 从头节点开始遍历 */
    while (current != NULL) {                   /* 遍历整个链表 */
        if (strcmp(current->name, name) == 0) { /* 字符串比较姓名 */
            return current;                     /* 找到匹配，返回指针 */
        }
        current = current->next;                /* 移动到下一个节点 */
    }
    return NULL;                                /* 遍历完毕未找到 */
}

/*
 * 函数：delete_player_by_id
 * 作者：lyl
 * 功能：通过学号删除链表中的球员节点
 * 参数：id - 要删除的球员学号
 * 返回值：1表示删除成功，0表示未找到
 * 算法：处理三种情况：头节点删除、中间节点删除、尾节点删除
 */
int delete_player_by_id(const char* id) {
    Player* current = player_list;              /* 当前遍历节点指针 */
    Player* previous = NULL;                    /* 前驱节点指针 */
    
    while (current != NULL) {                   /* 遍历整个链表 */
        if (strcmp(current->id, id) == 0) {     /* 找到匹配学号 */
            if (previous == NULL) {             /* 情况1：删除头节点 */
                player_list = current->next;    /* 更新头节点指针 */
            } else {                            /* 情况2：删除中间或尾节点 */
                previous->next = current->next; /* 前驱节点跳过当前节点 */
            }
            free(current);                      /* 释放当前节点内存 */
            return 1;                           /* 返回删除成功标志 */
        }
        previous = current;                     /* 更新前驱节点指针 */
        current = current->next;                /* 移动到下一个节点 */
    }
    return 0;                                   /* 未找到要删除的节点 */
}

/*
 * 函数：calculate_player_efficiency
 * 作者：lyl
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
 * 作者：lyl
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
 * 作者：lyl
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

/* 
 * ============================================================================
 * gyx负责模块：UI设计师 - 界面美化和用户交互
 * ============================================================================
 */

/* Windows控制台颜色常量定义（16色模式） */
#define COLOR_DEFAULT 7         /* 默认白色前景+黑色背景 */
#define COLOR_TITLE 11          /* 亮青色，用于标题显示 */
#define COLOR_MENU 10           /* 亮绿色，用于菜单显示 */
#define COLOR_ERROR 12          /* 亮红色，用于错误提示 */
#define COLOR_SUCCESS 10        /* 亮绿色，用于成功提示 */
#define COLOR_HIGHLIGHT 14      /* 亮黄色，用于高亮显示 */
#define COLOR_BORDER 8          /* 灰色，用于边框显示 */

/*
 * 函数：set_console_color
 * 作者：gyx
 * 功能：设置Windows控制台文本颜色
 * 参数：color_code - Windows控制台颜色代码
 * 设计思路：调用Windows API实现跨终端颜色兼容
 */
void set_console_color(int color_code) {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_handle, color_code);
}

/*
 * 函数：clear_input_buffer
 * 作者：gyx
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
 * 作者：gyx
 * 功能：显示系统欢迎界面和加载动画
 * 参数：无
 * 特色：使用边框装饰、颜色渐变、动态加载效果
 */
void display_welcome_animation() {
    system("cls");                               /* 清屏命令，确保干净界面 */
    
    set_console_color(COLOR_TITLE);              /* 设置标题颜色 */
    printf("╔══════════════════════════════════════════════════════╗\n");
    printf("║                                                      ║\n");
    printf("║        大学生篮球联赛数据管理系统 v3.0               ║\n");
    printf("║                                                      ║\n");
    printf("║           Basketball Statistics System               ║\n");
    printf("║                                                      ║\n");
    printf("╚══════════════════════════════════════════════════════╝\n\n");
    
    set_console_color(COLOR_MENU);               /* 设置菜单颜色 */
    printf("系统版本：3.0 | 适用平台：Windows 10+\n");
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
 * 作者：gyx
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

/*
 * 函数：display_player_card
 * 作者：gyx
 * 功能：以卡片形式美观显示单个球员详细信息
 * 参数：player - 要显示的球员结构体指针
 * 设计：模拟球员信息卡片，包含边框、颜色、对齐
 */
void display_player_card(const Player* player) {
    if (player == NULL) {                        /* 空指针安全检查 */
        set_console_color(COLOR_ERROR);          /* 设置错误提示颜色 */
        printf("错误：球员信息不存在或已删除！\n");
        set_console_color(COLOR_DEFAULT);        /* 恢复默认颜色 */
        return;                                  /* 提前返回避免崩溃 */
    }
    
    set_console_color(COLOR_HIGHLIGHT);          /* 设置卡片边框颜色 */
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║            球 员 详 细 信 息                 ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    
    set_console_color(COLOR_DEFAULT);            /* 设置内容显示颜色 */
    printf("║  学号：%-12s                          ║\n", player->id);
    printf("║  姓名：%-10s                            ║\n", player->name);
    printf("║  球队：%-15s                       ║\n", player->team);
    printf("║  位置：%-4s                                  ║\n", player->position);
    printf("║  身高：%-3d cm                                ║\n", player->height);
    printf("║  体重：%-3d kg                                ║\n", player->weight);
    printf("║  球衣：#%-2d                                   ║\n", player->jersey);
    
    /* 计算并显示球员效率值 */
    float efficiency = calculate_player_efficiency(player);
    printf("║  效率值：%.1f 分                             ║\n", efficiency);
    
    set_console_color(COLOR_HIGHLIGHT);          /* 恢复边框颜色 */
    printf("╚══════════════════════════════════════════════╝\n");
    set_console_color(COLOR_DEFAULT);            /* 最终恢复默认颜色 */
}

/*
 * 函数：display_all_players_table
 * 作者：gyx
 * 功能：以表格形式显示所有球员信息
 * 参数：无
 * 设计：固定列宽表格，自动换行，对齐精确
 */
void display_all_players_table() {
    if (player_list == NULL) {                   /* 空链表检查 */
        set_console_color(COLOR_ERROR);          /* 设置错误提示颜色 */
        printf("\n当前系统中没有任何球员数据！\n");
        set_console_color(COLOR_DEFAULT);        /* 恢复默认颜色 */
        return;                                  /* 提前返回 */
    }
    
    set_console_color(COLOR_MENU);               /* 设置表格边框颜色 */
    printf("\n┌──────────┬──────────┬──────────┬──────┬──────┬──────┬──────────┐\n");
    printf("│  学号    │  姓名    │  球队    │ 身高 │ 体重 │ 号码 │  效率值  │\n");
    printf("├──────────┼──────────┼──────────┼──────┼──────┼──────┼──────────┤\n");
    
    Player* current = player_list;               /* 从头节点开始遍历 */
    int display_count = 0;                       /* 显示计数器 */
    
    while (current != NULL) {                    /* 遍历整个链表 */
        set_console_color(COLOR_DEFAULT);        /* 设置数据行颜色 */
        
        /* 第1列：学号，固定宽度10字符，左对齐 */
        printf("│%-10s│", current->id);
        
        /* 第2列：姓名，固定宽度10字符，左对齐 */
        printf("%-10s│", current->name);
        
        /* 第3列：球队，固定宽度10字符，过长截断 */
        char team_display[11];                   /* 显示用缓冲区 */
        strncpy(team_display, current->team, 10); /* 复制最多10字符 */
        team_display[10] = '\0';                 /* 确保字符串终止 */
        if (strlen(current->team) > 10) {        /* 字符串过长处理 */
            team_display[9] = '.';               /* 第10位置为省略号 */
        }
        printf("%-10s│", team_display);          /* 输出格式化球队名 */
        
        /* 第4列：身高，固定宽度6字符，右对齐 */
        printf("%6d│", current->height);
        
        /* 第5列：体重，固定宽度6字符，右对齐 */
        printf("%6d│", current->weight);
        
        /* 第6列：球衣号，固定宽度6字符，右对齐 */
        printf("%6d│", current->jersey);
        
        /* 第7列：效率值，固定宽度8字符，右对齐，保留1位小数 */
        float efficiency = calculate_player_efficiency(current);
        printf("%8.1f│\n", efficiency);
        
        current = current->next;                 /* 移动到下一个节点 */
        display_count++;                         /* 计数器递增 */
        
        /* 每显示10行数据添加分隔线，提高可读性 */
        if (display_count % 10 == 0 && current != NULL) {
            set_console_color(COLOR_MENU);       /* 设置分隔线颜色 */
            printf("├──────────┼──────────┼──────────┼──────┼──────┼──────┼──────────┤\n");
        }
    }
    
    set_console_color(COLOR_MENU);               /* 设置表格底部边框颜色 */
    printf("└──────────┴──────────┴──────────┴──────┴──────┴──────┴──────────┘\n");
    printf("共显示 %d 名球员信息\n", display_count); /* 统计信息提示 */
    set_console_color(COLOR_DEFAULT);            /* 恢复默认文本颜色 */
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
    const char* positions[] = {"PG", "SG", "SF", "PF", "C"}; /* 位置名称数组 */
    
    while (current != NULL) {                    /* 再次遍历链表 */
        for (int i = 0; i < 5; i++) {            /* 遍历5个标准位置 */
            if (strcmp(current->position, positions[i]) == 0) {
                position_counts[i]++;            /* 对应位置计数递增 */
                break;                           /* 找到后退出内层循环 */
            }
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
    for (int i = 0; i < 5; i++) {                /* 遍历5个位置 */
        printf("   %s：%d 人 (%.1f%%)\n", 
               positions[i], 
               position_counts[i], 
               (float)position_counts[i] / total_players * 100);
    }
}

/* 
 * ============================================================================
 * 主程序模块：系统入口和流程控制
 * ============================================================================
 */

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
