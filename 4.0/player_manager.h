#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "data_struct.h"

/*
 * 函数声明部分
 * 所有函数都包含详细的参数说明和功能描述
 */

/*
 * 函数：add_new_player
 * 功能：向球员链表添加新球员记录（头插法）
 * 参数：id-学号, name-姓名, team-球队, position-位置, height-身高, weight-体重, jersey-球衣号
 * 返回值：成功返回新节点指针，失败返回NULL
 */
Player* add_new_player(const char* id, const char* name, const char* team, 
                      const char* position, int height, int weight, int jersey);

/*
 * 函数：search_player_by_id
 * 功能：通过学号在链表中查找球员（线性搜索）
 * 参数：id - 要查找的学号字符串
 * 返回值：找到返回球员指针，未找到返回NULL
 */
Player* search_player_by_id(const char* id);

/*
 * 函数：search_player_by_name
 * 功能：通过姓名在链表中查找球员（精确匹配）
 * 参数：name - 要查找的姓名字符串
 * 返回值：找到返回球员指针，未找到返回NULL
 */
Player* search_player_by_name(const char* name);

/*
 * 函数：delete_player_by_id
 * 功能：通过学号删除链表中的球员节点
 * 参数：id - 要删除的球员学号
 * 返回值：1表示删除成功，0表示未找到
 */
int delete_player_by_id(const char* id);

#endif /* PLAYER_MANAGER_H */
