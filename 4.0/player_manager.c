#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_struct.h"
#include "file_ops.h"
#include "player_manager.h"

/*
 * 函数：add_new_player
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
 * 函数：update_player_by_id
 * 功能：通过学号修改球员信息
 * 参数：id - 要修改的球员学号
 *        field - 要修改的字段（1-姓名，2-球队，3-位置，4-身高，5-体重，6-球衣号）
 *        new_value - 新值（字符串或整数）
 * 返回值：1表示修改成功，0表示未找到或修改失败
 */
int update_player_by_id(const char* id, int field, void* new_value) {
    Player* player = search_player_by_id(id);   /* 查找球员 */
    if (player == NULL) {
        return 0;                               /* 球员不存在 */
    }
    
    switch (field) {
        case 1: /* 修改姓名 */
            if (new_value != NULL) {
                strcpy(player->name, (char*)new_value);
            }
            break;
        case 2: /* 修改球队 */
            if (new_value != NULL) {
                strcpy(player->team, (char*)new_value);
            }
            break;
        case 3: /* 修改位置 */
            if (new_value != NULL) {
                strcpy(player->position, (char*)new_value);
            }
            break;
        case 4: /* 修改身高 */
            if (new_value != NULL) {
                player->height = *(int*)new_value;
            }
            break;
        case 5: /* 修改体重 */
            if (new_value != NULL) {
                player->weight = *(int*)new_value;
            }
            break;
        case 6: /* 修改球衣号 */
            if (new_value != NULL) {
                player->jersey = *(int*)new_value;
            }
            break;
        default:
            return 0;                           /* 无效字段 */
    }
    
    /* 验证修改后的数据 */
    if (validate_player_data(player)) {
        return 1;                               /* 修改成功 */
    } else {
        return 0;                               /* 数据不合法，修改失败 */
    }
}
