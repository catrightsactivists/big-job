#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* pzh: 数据结构定义 */
typedef struct Player {
    char id[13];
    char name[21];
    char team[31];
    int height;
    int weight;
    int jersey;
    struct Player* next;
} Player;

Player* player_list = NULL;

/* gyx: 界面美化函数 */
void set_color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void show_welcome() {
    system("cls");
    set_color(11);
    printf("========================================\n");
    printf("    大学生篮球联赛数据管理系统\n");
    printf("========================================\n");
    set_color(7);
}

void show_menu() {
    printf("\n");
    set_color(10);
    printf("┌──────────────────────────────┐\n");
    printf("│          主  菜  单          │\n");
    printf("├──────────────────────────────┤\n");
    printf("│  1. 添加新球员               │\n");
    printf("│  2. 查看所有球员             │\n");
    printf("│  3. 查找球员                 │\n");
    printf("│  4. 删除球员                 │\n");
    printf("│  5. 保存数据                 │\n");
    printf("│  6. 加载数据                 │\n");
    printf("│  7. 退出系统                 │\n");
    printf("└──────────────────────────────┘\n");
    set_color(7);
    printf("\n请选择操作 (1-7): ");
}

/* pzh: 文件操作函数 */
void save_data() {
    FILE* file = fopen("players.dat", "wb");
    Player* current = player_list;
    while (current != NULL) {
        fwrite(current, sizeof(Player), 1, file);
        current = current->next;
    }
    fclose(file);
    set_color(10);
    printf("数据保存成功！\n");
    set_color(7);
}

void load_data() {
    FILE* file = fopen("players.dat", "rb");
    if (file == NULL) return;
    
    Player player;
    while (fread(&player, sizeof(Player), 1, file) == 1) {
        Player* new_player = (Player*)malloc(sizeof(Player));
        *new_player = player;
        new_player->next = player_list;
        player_list = new_player;
    }
    fclose(file);
    set_color(10);
    printf("数据加载成功！\n");
    set_color(7);
}

/* lyl: 球员管理函数 */
void add_player() {
    Player* new_player = (Player*)malloc(sizeof(Player));
    printf("\n--- 添加新球员 ---\n");
    printf("学号: "); scanf("%s", new_player->id);
    printf("姓名: "); scanf("%s", new_player->name);
    printf("球队: "); scanf("%s", new_player->team);
    printf("身高(cm): "); scanf("%d", &new_player->height);
    printf("体重(kg): "); scanf("%d", &new_player->weight);
    printf("球衣号码: "); scanf("%d", &new_player->jersey);
    
    new_player->next = player_list;
    player_list = new_player;
    
    set_color(10);
    printf("球员添加成功！\n");
    set_color(7);
}

void show_all_players() {
    if (player_list == NULL) {
        set_color(12);
        printf("暂无球员数据！\n");
        set_color(7);
        return;
    }
    
    set_color(14);
    printf("┌─────┬─────┬─────┬─────┬─────┬─────┐\n");
    printf("│ 学号│姓名 │球队 │身高 │体重 │号码 │\n");
    printf("├─────┼─────┼─────┼─────┼─────┼─────┤\n");
    set_color(7);
    
    Player* current = player_list;
    int count = 0;
    while (current != NULL) {
        printf("│%5s│%5s│%5s│%5d│%5d│%5d│\n",
               current->id, current->name, current->team,
               current->height, current->weight, current->jersey);
        current = current->next;
        count++;
    }
    
    set_color(14);
    printf("└─────┴─────┴─────┴─────┴─────┴─────┘\n");
    printf("共 %d 名球员\n", count);
    set_color(7);
}

void find_player() {
    char id[13];
    printf("请输入要查找的学号: ");
    scanf("%s", id);
    
    Player* current = player_list;
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            set_color(11);
            printf("\n=== 球员信息 ===\n");
            printf("学号: %s\n", current->id);
            printf("姓名: %s\n", current->name);
            printf("球队: %s\n", current->team);
            printf("身高: %dcm\n", current->height);
            printf("体重: %dkg\n", current->weight);
            printf("号码: %d\n", current->jersey);
            set_color(7);
            return;
        }
        current = current->next;
    }
    
    set_color(12);
    printf("未找到该球员！\n");
    set_color(7);
}

void delete_player() {
    char id[13];
    printf("请输入要删除的学号: ");
    scanf("%s", id);
    
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
            set_color(10);
            printf("球员删除成功！\n");
            set_color(7);
            return;
        }
        prev = current;
        current = current->next;
    }
    
    set_color(12);
    printf("未找到该球员！\n");
    set_color(7);
}

/* lyl: 统计算法 */
void show_stats() {
    if (player_list == NULL) {
        set_color(12);
        printf("暂无数据！\n");
        set_color(7);
        return;
    }
    
    int total = 0, max_height = 0, min_height = 300;
    float avg_height = 0, avg_weight = 0;
    Player* tallest = NULL, * shortest = NULL;
    
    Player* current = player_list;
    while (current != NULL) {
        total++;
        avg_height += current->height;
        avg_weight += current->weight;
        
        if (current->height > max_height) {
            max_height = current->height;
            tallest = current;
        }
        if (current->height < min_height) {
            min_height = current->height;
            shortest = current;
        }
        current = current->next;
    }
    
    avg_height /= total;
    avg_weight /= total;
    
    set_color(11);
    printf("\n=== 数据统计 ===\n");
    printf("球员总数: %d\n", total);
    printf("平均身高: %.1fcm\n", avg_height);
    printf("平均体重: %.1fkg\n", avg_weight);
    printf("最高球员: %s (%dcm)\n", tallest->name, tallest->height);
    printf("最矮球员: %s (%dcm)\n", shortest->name, shortest->height);
    set_color(7);
}

/* gyx: 主菜单系统 */
int main() {
    load_data();
    int choice;
    
    do {
        show_welcome();
        show_menu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: add_player(); break;
            case 2: show_all_players(); break;
            case 3: find_player(); break;
            case 4: delete_player(); break;
            case 5: save_data(); break;
            case 6: load_data(); break;
            case 7: 
                save_data();
                set_color(10);
                printf("感谢使用，再见！\n");
                set_color(7);
                break;
            default:
                set_color(12);
                printf("无效选择！\n");
                set_color(7);
        }
        
        if (choice != 7) {
            printf("\n按任意键继续...");
            getchar(); getchar();
        }
        
    } while (choice != 7);
    
    return 0;
}
