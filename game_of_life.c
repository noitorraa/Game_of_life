#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25

void init_board(char board[HEIGHT][WIDTH], int input);
void print_board(char board[HEIGHT][WIDTH]);
int count_neighbors(char board[HEIGHT][WIDTH], int i, int j);
void update_board(char board[HEIGHT][WIDTH]);
char menu();
int kbhit();
void clear();
char input_button(double *speed);

int main() {
    system("stty -icanon");
    char board[HEIGHT][WIDTH];
    char input = menu();
    double speed = 2;
    init_board(board, input);
    print_board(board);
    do {
        input = input_button(&speed);
        printf(
            "!!!Нажмите \'q\' для выхода, \'w\' для увеличения скорости и \'s\' для уменьшения "
            "скорости!!!\n\n");
        update_board(board);
        print_board(board);
        usleep(50000 * speed);
        clear();
    } while (input != 'q');
    return 0;
}

char input_button(double *speed) {
    char button = '0';
    if (kbhit()) {
        button = getc(stdin);
        if (button == 'w' && *speed > 0.1) {
            *speed /= 2;
        } else if (button == 's' && *speed < 15) {
            *speed *= 2;
        }
    }
    return button;
}

int kbhit() {
    struct timeval tv;
    fd_set rdfs;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&rdfs);
    FD_SET(STDIN_FILENO, &rdfs);
    select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &rdfs);
}

char menu() {
    printf("***************************************************************************\n");
    printf("Нажмите \'1\', чтобы выбрать уровень \'Ружьё\'\n");
    printf("Нажмите \'2\', чтобы выбрать уровень \'Поезд\'\n");
    printf("Нажмите \'3\', чтобы выбрать уровень \'Пульсар\'\n");
    printf("Нажмите \'4\', чтобы выбрать уровень \'Космический корабль\'\n");
    printf("Нажмите \'5\', чтобы выбрать уровень \'MWSS\'\n");
    printf("Нажмите \'6\', чтобы создать свой уровень\n");
    printf("Для того, чтобы увеличить скорость, нажмите в игре \'w\'\n");
    printf("Для того, чтобы уменьшить скорость, нажмите в игре \'s\'\n");
    printf("***************************************************************************\n");
    char input;
    do {
        input = getchar();
    } while (input != '1' && input != '2' && input != '3' && input != '4' && input != '5' && input != '6');
    return input;
}

void init_board(char board[HEIGHT][WIDTH], int input) {
    char *T;
    if (input == '1') {
        T = "gun.txt";
    } else if (input == '2') {
        T = "train.txt";
    } else if (input == '3') {
        T = "pulsar.txt";
    } else if (input == '4') {
        T = "spaceship.txt";
    } else if (input == '5') {
        T = "MWSS.txt";
    }
    if (input == '1' || input == '2' || input == '3' || input == '4' || input == '5') {
        FILE *fp;
        fp = fopen(T, "r");
        if (fp == NULL) {
            printf("Error opening file\n");
            return;
        }
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                char c;
                if (fscanf(fp, "%c", &c) == 1) {
                    board[i][j] = (c == '1') ? 1 : 0;
                } else {
                    printf("Error reading file\n");
                    fclose(fp);
                    return;
                }
            }
        }
        fclose(fp);
    }
    if (input == '6')
    {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                board[i][j] = rand() % 2;
            }
        }
    }
}

void print_board(char board[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == 0) {
                printf(" ");
            } else {
                printf("•");
            }
        }
        printf("\n");
    }
}

int count_neighbors(char board[HEIGHT][WIDTH], int i, int j) {
    int count = 0;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            int ni = i + x;
            int nj = j + y;
            if (ni >= 0 && ni < HEIGHT && nj >= 0 && nj < WIDTH && board[ni][nj] == 1) {
                count++;
            }
        }
    }

    if (board[i][j] == 1) {
        count--;
    }
    return count;
}

void update_board(char board[HEIGHT][WIDTH]) {
    char new_board[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int neighbors = count_neighbors(board, i, j);
            if (board[i][j] == 1 && (neighbors < 2 || neighbors > 3)) {
                new_board[i][j] = 0;
            } else if (board[i][j] == 0 && neighbors == 3) {
                new_board[i][j] = 1;
            } else {
                new_board[i][j] = board[i][j];
            }
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            board[i][j] = new_board[i][j];
        }
    }
}

void clear() {
    printf("\033[2J");
    printf("\033[0;0f");
}
