#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

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

/* 全局变量声明 */
extern Player* player_list;

#endif /* DATA_STRUCT_H */
