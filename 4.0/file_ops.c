#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_struct.h"
#include "file_ops.h"

/* 全局球员链表头指针定义 */
Player* player_list = NULL;

/*
 * 函数：save_player_data_to_file
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
