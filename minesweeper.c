#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BOM 10  // �{���̌�

void banmen(char ***array, int *row, int *line);
void disp(char **array, int row, int line);
void freearray(char **array, int row);
void info_bom(char ***array, int row, int line, int x, int y);
void memory(char ***array, int row, int line);
void input_data(int *x, int *y, char *com);
void magic_s(int x, int y, int row, int line, char com, char **array, char **hantei);
void magic_a(int x, int y, int row, int line, char com, char **array, char **hantei);
void magic_m(int x, int y, int row, int line, char com, char **array);
void disp_hantei(char **hantei, int row, int line); 
void calculate_bombs(char **hantei, int row, int line); 
void open_zeros(int x, int y, int row, int line, char **array, char **hantei); 
int check_win(char **array, int row, int line); 
int check_loss(char **array, int row, int line); 

int main() {
    system("clear");

    char **array;
    char **hantei;
    int row = 0; // �s
    int line = 0; // ��
    int x = 0;
    int y = 0;
    char com = ' ';

    banmen(&array, &row, &line); // ��������̔Ֆ�

    // ����̕\���ƃf�[�^����
    disp(array, row, line);
    input_data(&x, &y, &com);

    // �{���̏���^����
    info_bom(&hantei, row, line, x, y);

    // �{������\���i�f�o�b�O�p�j
    //disp_hantei(hantei, row, line);

    while (1) {
        magic_s(x, y, row, line, com, array, hantei);
        disp(array, row, line);
        int win = check_win(array, row, line);
        int loss = check_loss(array, row, line);
        if (win) {
            printf("�Q�[���N���A�I\n");
            break;
        } else if (loss) {
            printf("�Q�[���I�[�o�[�I\n");
            break;
        }
        input_data(&x, &y, &com);
    }

    freearray(array, row);
    freearray(hantei, row);

    return 0;
}

// �Ֆʂ̐݌v
void banmen(char ***array, int *row, int *line) {
    printf("�Ֆʂ̍s�����w�肵�Ă�������: ");
    scanf("%d", row);
    printf("�Ֆʂ̗񐔂��w�肵�Ă�������: ");
    scanf("%d", line);

    memory(array, *row, *line);

    for (int i = 0; i < (*row); i++) {
        for (int j = 0; j < (*line); j++) {
            (*array)[i][j] = '.';
        }
    }
}

// �\�����邽�߂̊֐�
void disp(char **array, int row, int line) {
    printf("*** M Sweeper ***\n");

    printf(" ");
    for (int i = 0; i < line; i++) {
        printf(" %d", i);
    }
    printf("\n");

    for (int i = 0; i < row; i++) {
        printf("%d", i);
        for (int j = 0; j < line; j++) {
            char display_char = array[i][j] == '0' ? ' ' : array[i][j];
            printf(" %c", display_char);
        }
        printf("\n");
    }
}

// free�̂���
void freearray(char **array, int row) {
    for (int i = 0; i < row; i++) {
        free(array[i]);
    }
    free(array);
}

// �{���̏���^����
void info_bom(char ***array, int row, int line, int x, int y) {
    int random_row = 0;
    int random_line = 0;

    srand((unsigned int)time(NULL));
    memory(array, row, line);

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < line; j++) {
            (*array)[i][j] = '.';
        }
    }

    for (int i = 0; i < BOM; i++) {
        random_row = rand() % row;
        random_line = rand() % line;
        if ((random_row == y && random_line == x) || (*array)[random_row][random_line] == 'M') {
            i--;
        } else {
            (*array)[random_row][random_line] = 'M';
        }
    }
    calculate_bombs(*array, row, line);
}

// �������m��
void memory(char ***array, int row, int line) {
    *array = (char **)malloc(row * sizeof(char *));
    if (*array == NULL) {
        printf("�������̊m�ۂɎ��s\n");
        exit(1);
    }

    for (int i = 0; i < row; i++) {
        (*array)[i] = (char *)malloc(line * sizeof(char));
        if ((*array)[i] == NULL) {
            printf("�������̊m�ۂɎ��s");
            for (int j = 0; j < i; j++) {
                free((*array)[j]);
            }
            free(*array);
            exit(1);
        }
    }
}

void input_data(int *x, int *y, char *com) {
    printf("\n\n�R�}���h�̓���:x y [a,s,m]\n");
    scanf("%d %d %c", x, y, com);
}

void magic_s(int x, int y, int row, int line, char com, char **array, char **hantei) {
    if (com == 's') {
        if (hantei[y][x] == '0') {
            open_zeros(x, y, row, line, array, hantei);
        } else {
            array[y][x] = hantei[y][x];
        }
    } else if (com == 'a') {
        magic_a(x, y, row, line, com, array, hantei);
    } else if (com == 'm') {
        magic_m(x, y, row, line, com, array);
    }
}

void magic_m(int x, int y, int row, int line, char com, char **array) {
    array[y][x] = 'm';
}

void magic_a(int x, int y, int row, int line, char com, char **array, char **hantei) {

    int count_m = 0;

    // ���͂�'m'�̐����J�E���g����
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (i >= 0 && i < row && j >= 0 && j < line && !(i == y && j == x) && array[i][j] == 'm') {
                count_m++;
            }
        }
    }

    // 'm'�̐����w�肵�����W�̒l�Ɠ����Ȃ�A���͂̔� 'm' �̃Z���𒲂ׂ� '0' ������ΊJ����
    if (count_m == (hantei[y][x] - '0')) {
        for (int i = y - 1; i <= y + 1; i++) {
            for (int j = x - 1; j <= x + 1; j++) {
                if (i >= 0 && i < row && j >= 0 && j < line && !(i == y && j == x) && array[i][j] != 'm') {
                    if (hantei[i][j] == '0') {
                        open_zeros(j, i, row, line, array, hantei);
                    } else {
                        array[i][j] = hantei[i][j];
                    }
                }
            }
        }
    }
}

// �{�����\���p�̊֐��i�f�o�b�O�p�j
void disp_hantei(char **hantei, int row, int line) {
    printf("*** �{����� ***\n");

    printf(" ");
    for (int i = 0; i < line; i++) {
        printf(" %d", i);
    }
    printf("\n");

    for (int i = 0; i < row; i++) {
        printf("%d", i);
        for (int j = 0; j < line; j++) {
            printf(" %c", hantei[i][j]);
        }
        printf("\n");
    }
}

// ���͂̃{�������v�Z����֐�
void calculate_bombs(char **hantei, int row, int line) {
    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int r = 0; r < row; r++) {
        for (int c = 0; c < line; c++) {
            if (hantei[r][c] != 'M') {
                int count = 0;
                for (int k = 0; k < 8; k++) {
                    int nr = r + dr[k];
                    int nc = c + dc[k];
                    if (nr >= 0 && nr < row && nc >= 0 && nc < line && hantei[nr][nc] == 'M') {
                        count++;
                    }
                }
                hantei[r][c] = count + '0';
            }
        }
    }
}

// �ċA�I��0�̎��͂��J����֐�
void open_zeros(int x, int y, int row, int line, char **array, char **hantei) {
    if (x < 0 || x >= line || y < 0 || y >= row || array[y][x] != '.' || hantei[y][x] == 'M') {
        return;
    }

    array[y][x] = ' '; 

    if (hantei[y][x] == '0') {
        array[y][x] = ' ';
        open_zeros(x - 1, y - 1, row, line, array, hantei);
        open_zeros(x, y - 1, row, line, array, hantei);
        open_zeros(x + 1, y - 1, row, line, array, hantei);
        open_zeros(x - 1, y, row, line, array, hantei);
        open_zeros(x + 1, y, row, line, array, hantei);
        open_zeros(x - 1, y + 1, row, line, array, hantei);
        open_zeros(x, y + 1, row, line, array, hantei);
        open_zeros(x + 1, y + 1, row, line, array, hantei);
    } else {
        array[y][x] = hantei[y][x];
    }
}

// �Q�[���N���A�̃`�F�b�N
int check_win(char **array, int row, int line) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < line; j++) {
            if (array[i][j] == '.' || array[i][j] == '0') {
                return 0; 
            }
        }
    }
    return 1; 
}

// �Q�[���I�[�o�[�̃`�F�b�N
int check_loss(char **array, int row, int line) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < line; j++) {
            if (array[i][j] == 'M') {
                return 1;
            }
        }
    }
    return 0; 
}
