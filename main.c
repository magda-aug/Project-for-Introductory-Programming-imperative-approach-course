#include <stdio.h>

#define ROWS 8
#define COLUMNS 8

#define CHANGE 1
#define LEAVE 0

char Board[COLUMNS][ROWS];
int diff = 0;

void Initialization(){
    for(int i = 0; i < COLUMNS; i++){
        for(int j = 0; j < ROWS; j++){
            Board[i][j] = '-';
        }
    }
    Board[3][3] = 'B';
    Board[4][4] = 'B';
    Board[3][4] = 'C';
    Board[4][3] = 'C';
}

void PrintBoard(){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            printf("%c", Board[j][i]);
        }
        printf("%d\n", i+1);
    }
    printf("abcdefgh\n");
}

void dump(){
    char c;
    do{
	c = getchar();
    }while(c != '\n' && c != EOF);
}

int Read(char instruction[]){
    char col;
    col = getchar();
    char c;
    if(col == '='){
        c = getchar();
        if(c == '\n' || c == EOF)
            return 2;
        else {
            dump();
            return 1;
        }
    }
    if('a' <= col && col <= 'h'){
        char row;
        row = getchar();
        if ('1' <= row && row  <= '8'){
            if(getchar() == '\n'){
                instruction[0] = col - 'a';
                instruction[1] = row - '1';
                return 0;
            }
            else
            {
                dump();
                return 1;
            }
        }
        else {
            ungetc(row,stdin);
            dump();
            return 1;
        }
    }
    else {
        ungetc(col, stdin);
        dump();
        return 1;
    }
}

int Step(int col, int row, int xaxis, int yaxis, int flag){
    int dif = 0;
    int j = row + yaxis;
    int i = col + xaxis;
    while(0 <= j && j < COLUMNS && 0 <= i && i < ROWS && Board[i][j] != '-'){
        if (Board[i][j] == 'B'){
                j += yaxis;
                i += xaxis;
        }
        else if(Board[i][j] == 'C' && (i != col || j != row)){
            int l = i - xaxis;
            int k = j - yaxis;
            if(flag == CHANGE)Board[col][row] = 'C';
            while(l != col || k != row){
                if(flag == CHANGE)Board[l][k] = 'C';
                (dif) += 2;
                l -= xaxis;
                k -= yaxis;
            }
            return dif;
        }
    }
    return dif;
}

int Step2(int col, int row, int xaxis, int yaxis, int flag){
    int dif = 0;
    int i = col + xaxis;
    int j = row + yaxis;

    while(0 <= i && i < COLUMNS && 0 <= j && j < ROWS && Board[i][j] != '-'){
        if (Board[i][j] == 'C'){
                j += yaxis;
                i += xaxis;
        }
        else if(Board[i][j] == 'B' && (i != col || j != row)){
            int l = i - xaxis;
            int k = j - yaxis;
            if(flag == CHANGE)Board[col][row] = 'B';
            while(l != col || k != row){
                if(flag == CHANGE)Board[l][k] = 'B';
                l -= xaxis;
                k -= yaxis;
                dif += 2;
            }
            return dif;
        }
    }
    return dif;
}

int MyMove(int x, int y, int flag){
    int total = 0;
    total += Step2(x, y, 1, 0, flag);
    total += Step2(x, y, 0, 1, flag);
    total += Step2(x, y, 0, -1, flag);
    total += Step2(x, y, -1, 0, flag);
    total += Step2(x, y, 1, 1, flag);
    total += Step2(x, y, 1, -1, flag);
    total += Step2(x, y, -1, 1, flag);
    total += Step2(x, y, -1, -1, flag);
    return total;
}

void WhichMove(int* x, int* y){
    *x = -1;
    *y = -1;
    int dif = 0;
    int p;
    int flag = LEAVE;
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            if(Board[j][i] == '-'){
                 p = MyMove(j, i, flag);
                 if (p > dif){
                    dif = p;
                    *x = j;
                    *y = i;
                 }
	        }
        }
    }
}

int Move(int x, int y, int flag){
    int total = 0;
    total += Step(x, y, 1, 0, flag);
    total += Step(x, y, 0, 1, flag);
    total += Step(x, y, 0, -1, flag);
    total += Step(x, y, -1, 0, flag);
    total += Step(x, y, 1, 1, flag);
    total += Step(x, y, 1, -1, flag);
    total += Step(x, y, -1, 1, flag);
    total += Step(x, y, -1, -1, flag);
    return total;
}

int AnyBlackLegal(){
    int p;
    int x = -1;
    int flag = LEAVE;
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            p = Move(j, i, flag);
            if (p > 0)x = 0;
        }
    }
    return x;
}

void Round(){
    char instruction[2];
    int x, y;
    int c = Read(instruction);
    if (c == 1){
        printf("? %d\n", diff);
        PrintBoard();
    }
    else if(c == 2){
        WhichMove(&x, &y);
        if(x == -1 && y == -1){
            printf("= = %d\n", diff);
            PrintBoard();
        }
        else {
            diff -= MyMove(x, y, CHANGE) + 1;
            printf("= %c%c %d\n", x + 'a', y + '1', diff);
            PrintBoard();
        }
    }
    else if(c == 0){
        if(Move(instruction[0], instruction[1], LEAVE) > 0 && Board[(int)instruction[0]][(int)instruction[1]] == '-'){
            diff += Move(instruction[0], instruction[1], CHANGE) + 1;
            WhichMove(&x, &y);
            if(x == -1 && y == -1){
                printf("%c%c = %d\n", instruction[0] + 'a', instruction[1] + '1', diff);
                PrintBoard();
            }
            else{
                diff -= MyMove(x, y, CHANGE) + 1;
                printf("%c%c %c%c %d\n", instruction[0] + 'a', instruction[1] + '1', x + 'a', y + '1', diff);
                PrintBoard();
            }
        }
        else{
            printf("? %d\n", diff);
            PrintBoard();
        }
    }
}

int main(){
    Initialization();
    PrintBoard();
    char c = getchar();
    while(c != EOF){
        ungetc(c,stdin);
        Round();
        c = getchar();
    }
    return 0;
}


