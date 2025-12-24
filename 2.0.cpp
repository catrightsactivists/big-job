#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct Player {
    char id[13];
    char name[21];
    char team[31];
    char position[11];
    int height;
    int weight;
    int jersey;
    struct Player* next;
} Player;

Player* player_list = NULL;

/* pzh部分：文件操作 */
int save_data() {
    FILE* file = fopen("players.dat", "wb");
    if (file == NULL) return 0;
    
    Player* current = player_list;
    int count = 0;
    while (current != NULL) {
        fwrite(current, sizeof(Player), 1, file);
        current = current->next;
        count++;
    }
    fclose(file);
    return count;
}

int load_data() {
    FILE* file = fopen("players.dat", "rb");
    if (file == NULL) return 0;
    
    Player* current = player_list;
    while (current != NULL) {
        Player* temp = current;
        current = current->next;
        free(temp);
    }
    player_list = NULL;
    
    Player temp;
    Player* last = NULL;
    int count = 0;
    
    while (fread(&temp, sizeof(Player), 1, file) == 1) {
        Player* new_player = (Player*)malloc(sizeof(Player));
        if (new_player == NULL) break;
        
        memcpy(new_player, &temp, sizeof(Player));
        new_player->next = NULL;
        
        if (player_list == NULL) {
            player_list = new_player;
        } else {
            last->next = new_player;
        }
        last = new_player;
        count++;
    }
    
    fclose(file);
    return count;
}

int validate_data(Player* player) {
    if (strlen(player->id) != 12) return 0;
    if (strlen(player->name) == 0) return 0;
    if (player->height < 100 || player->height > 250) return 0;
    if (player->weight < 40 || player->weight > 200) return 0;
    if (player->jersey < 0 || player->jersey > 99) return 0;
    return 1;
}

/* lyl部分：球员管理 */
Player* add_player(char* id, char* name, char* team, char* position, int height, int weight, int jersey) {
    Player* current = player_list;
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) return NULL;
        current = current->next;
    }
    
    Player* new_player = (Player*)malloc(sizeof(Player));
    if (new_player == NULL) return NULL;
    
    strcpy(new_player->id, id);
    strcpy(new_player->name, name);
    strcpy(new_player->team, team);
    strcpy(new_player->position, position);
    new_player->height = height;
    new_player->weight = weight;
    new_player->jersey = jersey;
    
    if (!validate_data(new_player)) {
        free(new_player);
        return NULL;
    }
    
    new_player->next = player_list;
    player_list = new_player;
    
    return new_player;
}

Player* find_by_id(char* id) {
    Player* current = player_list;
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) return current;
        current = current->next;
    }
    return NULL;
}

Player* find_by_name(char* name) {
    Player* current = player_list;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) return current;
        current = current->next;
    }
    return NULL;
}

int delete_player(char* id) {
    Player* current = player_list;
    Player* prev = NULL;
    
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            if (prev == NULL) {
                player_list = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}

float calculate_score(Player* player) {
    if (player == NULL) return 0.0f;
    float score = (float)player->height / 10.0f;
    score += (200.0f - player->weight) / 5.0f;
    score += (float)player->jersey / 2.0f;
    return score;
}

void show_stats() {
    if (player_list == NULL) {
        printf("暂无球员数据\n");
        return;
    }
    
    int total = 0;
    int total_h = 0, total_w = 0;
    int max_h = 0, min_h = 300;
    Player* tallest = NULL, * shortest = NULL;
    
    Player* current = player_list;
    while (current != NULL) {
        total++;
        total_h += current->height;
        total_w += current->weight;
        
        if (current->height > max_h) {
            max_h = current->height;
            tallest = current;
        }
        if (current->height < min_h) {
            min_h = current->height;
            shortest = current;
        }
        current = current->next;
    }
    
    printf("\n=== 统计报告 ===\n");
    printf("球员总数：%d\n", total);
    printf("平均身高：%.1f cm\n", (float)total_h / total);
    printf("平均体重：%.1f kg\n", (float)total_w / total);
    printf("最高球员：%s (%d cm)\n", tallest->name, tallest->height);
    printf("最矮球员：%s (%d cm)\n", shortest->name, shortest->height);
}

/* gyx部分：界面显示 */
void set_color(int color) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, color);
}

void clear_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void welcome() {
    system("cls");
    
    set_color(11);
    printf("╔══════════════════════════════════════════════════════╗\n");
    printf("║                                                      ║\n");
    printf("║        大学生篮球联赛数据管理系统2.0                 ║\n");
    printf("║                                                      ║\n");
    printf("║           Basketball Statistics System               ║\n");
    printf("║                                                      ║\n");
    printf("╚══════════════════════════════════════════════════════╝\n\n");
    
    set_color(10);
    printf("版本：2.0| 适用平台：Windows\n");
    printf("========================================================\n\n");
    
    set_color(7);
    printf("系统初始化");
    for (int i = 0; i < 3; i++) {
        printf(".");
        Sleep(300);
    }
    printf(" 完成！\n\n");
}

void show_menu() {
    set_color(10);
    printf("\n┌──────────────────────────────────────────────┐\n");
    printf("│                主 功 能 菜 单                │\n");
    printf("├──────────────────────────────────────────────┤\n");
    printf("│   1. 添加新球员信息                          │\n");
    printf("│   2. 查看所有球员信息                        │\n");
    printf("│   3. 按学号查找球员                          │\n");
    printf("│   4. 按姓名查找球员                          │\n");
    printf("│   5. 修改球员信息                            │\n");
    printf("│   6. 删除球员记录                            │\n");
    printf("│   7. 球员数据统计                            │\n");
    printf("│   8. 保存数据到文件                          │\n");
    printf("│   9. 从文件加载数据                          │\n");
    printf("│   0. 退出管理系统                            │\n");
    printf("└──────────────────────────────────────────────┘\n");
    set_color(7);
    printf("\n请输入选项 (0-9): ");
}

void show_player_card(Player* player) {
    if (player == NULL) {
        set_color(12);
        printf("错误：球员不存在\n");
        set_color(7);
        return;
    }
    
    set_color(10);
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║            球 员 详 细 信 息                ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    set_color(7);
    
    printf("║  学号：%-12s                                ║\n", player->id);
    printf("║  姓名：%-10s                                ║\n", player->name);
    printf("║  球队：%-15s                                ║\n", player->team);
    printf("║  位置：%-4s                                 ║\n", player->position);
    printf("║  身高：%-3d cm                              ║\n", player->height);
    printf("║  体重：%-3d kg                              ║\n", player->weight);
    printf("║  球衣：#%-2d                                ║\n", player->jersey);
    
    float score = calculate_score(player);
    printf("║  评分：%.1f 分                              ║\n", score);
    
    set_color(10);
    printf("╚══════════════════════════════════════════════╝\n");
    set_color(7);
}

void show_all_players() {
    if (player_list == NULL) {
        set_color(12);
        printf("\n当前没有球员数据！\n");
        set_color(7);
        return;
    }
    
    set_color(10);
    printf("\n┌──────────┬──────────┬──────────┬──────┬──────┬──────┐\n");
    printf("│  学号    │  姓名    │  球队    │ 身高 │ 体重 │ 号码 │\n");
    printf("├──────────┼──────────┼──────────┼──────┼──────┼──────┤\n");
    
    Player* current = player_list;
    int count = 0;
    
    while (current != NULL) {
        set_color(7);
        printf("│%-10s│", current->id);
        printf("%-10s│", current->name);
        
        char team[11];
        strncpy(team, current->team, 10);
        team[10] = '\0';
        if (strlen(current->team) > 10) {
            team[9] = '.';
            team[10] = '\0';
        }
        printf("%-10s│", team);
        
        printf("%6d│", current->height);
        printf("%6d│", current->weight);
        printf("%6d│\n", current->jersey);
        
        current = current->next;
        count++;
        
        if (count % 10 == 0 && current != NULL) {
            set_color(10);
            printf("├──────────┼──────────┼──────────┼──────┼──────┼──────┤\n");
        }
    }
    
    set_color(10);
    printf("└──────────┴──────────┴──────────┴──────┴──────┴──────┘\n");
    printf("共 %d 名球员\n", count);
    set_color(7);
}

int modify_player(char* id) {
    Player* player = find_by_id(id);
    if (player == NULL) {
        set_color(12);
        printf("未找到该球员！\n");
        set_color(7);
        return 0;
    }
    
    char team[31], position[11];
    int height, weight, jersey;
    
    printf("\n--- 修改球员信息 ---\n");
    printf("当前球队：%s，新球队：", player->team);
    scanf("%s", team);
    clear_buffer();
    
    printf("当前位置：%s，新位置(PG/SG/SF/PF/C)：", player->position);
    scanf("%s", position);
    clear_buffer();
    
    printf("当前身高：%d，新身高：", player->height);
    scanf("%d", &height);
    clear_buffer();
    
    printf("当前体重：%d，新体重：", player->weight);
    scanf("%d", &weight);
    clear_buffer();
    
    printf("当前球衣：%d，新球衣：", player->jersey);
    scanf("%d", &jersey);
    clear_buffer();
    
    strcpy(player->team, team);
    strcpy(player->position, position);
    player->height = height;
    player->weight = weight;
    player->jersey = jersey;
    
    if (!validate_data(player)) {
        set_color(12);
        printf("修改失败：数据不合法！\n");
        set_color(7);
        return 0;
    }
    
    set_color(10);
    printf("修改成功！\n");
    set_color(7);
    return 1;
}

/* 主函数 */
int main() {
    welcome();
    load_data();
    
    int choice;
    char input[100];
    
    do {
        show_menu();
        fgets(input, sizeof(input), stdin);
        choice = atoi(input);
        
        switch (choice) {
            case 1: {
                char id[13], name[21], team[31], position[11];
                int height, weight, jersey;
                
                printf("\n--- 添加新球员 ---\n");
                printf("学号(12位): ");
                scanf("%s", id);
                clear_buffer();
                
                printf("姓名: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                
                printf("球队: ");
                fgets(team, sizeof(team), stdin);
                team[strcspn(team, "\n")] = 0;
                
                printf("位置(PG/SG/SF/PF/C): ");
                scanf("%s", position);
                clear_buffer();
                
                printf("身高(cm): ");
                scanf("%d", &height);
                clear_buffer();
                
                printf("体重(kg): ");
                scanf("%d", &weight);
                clear_buffer();
                
                printf("球衣号码(0-99): ");
                scanf("%d", &jersey);
                clear_buffer();
                
                Player* new_player = add_player(id, name, team, position, height, weight, jersey);
                if (new_player != NULL) {
                    set_color(10);
                    printf("添加成功！\n");
                    show_player_card(new_player);
                    set_color(7);
                } else {
                    set_color(12);
                    printf("添加失败！\n");
                    set_color(7);
                }
                break;
            }
                
            case 2:
                show_all_players();
                break;
                
            case 3: {
                char id[13];
                printf("\n请输入学号: ");
                scanf("%s", id);
                clear_buffer();
                
                Player* player = find_by_id(id);
                if (player != NULL) {
                    show_player_card(player);
                } else {
                    set_color(12);
                    printf("未找到该球员！\n");
                    set_color(7);
                }
                break;
            }
                
            case 4: {
                char name[21];
                printf("\n请输入姓名: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                
                Player* player = find_by_name(name);
                if (player != NULL) {
                    show_player_card(player);
                } else {
                    set_color(12);
                    printf("未找到该球员！\n");
                    set_color(7);
                }
                break;
            }
                
            case 5: {
                char id[13];
                printf("\n请输入要修改的球员学号: ");
                scanf("%s", id);
                clear_buffer();
                modify_player(id);
                break;
            }
                
            case 6: {
                char id[13];
                printf("\n请输入要删除的学号: ");
                scanf("%s", id);
                clear_buffer();
                
                if (delete_player(id)) {
                    set_color(10);
                    printf("删除成功！\n");
                    set_color(7);
                } else {
                    set_color(12);
                    printf("删除失败！\n");
                    set_color(7);
                }
                break;
            }
                
            case 7:
                show_stats();
                break;
                
            case 8:
                if (save_data() > 0) {
                    set_color(10);
                    printf("保存成功！\n");
                    set_color(7);
                } else {
                    set_color(12);
                    printf("保存失败！\n");
                    set_color(7);
                }
                break;
                
            case 9:
                if (load_data() > 0) {
                    set_color(10);
                    printf("加载成功！\n");
                    set_color(7);
                } else {
                    set_color(12);
                    printf("加载失败或文件不存在\n");
                    set_color(7);
                }
                break;
                
            case 0:
                if (save_data() > 0) {
                    set_color(10);
                    printf("数据已保存，再见！\n");
                } else {
                    set_color(12);
                    printf("保存失败！\n");
                }
                set_color(7);
                break;
                
            default:
                set_color(12);
                printf("无效选项！\n");
                set_color(7);
        }
        
        if (choice != 0) {
            printf("\n按回车继续...");
            getchar();
        }
        
    } while (choice != 0);
    
    return 0;
}
