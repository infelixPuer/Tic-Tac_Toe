#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

void GoTo(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleCursorPosition(hConsole, coord);
}

struct Scores {
    char name[25];
    int score;
};

struct Scores highScores[10] = {
    {"---", 0},
    {"---", 0},
    {"---", 0},
    {"---", 0},
    {"---", 0},
    {"---", 0},
    {"---", 0},
    {"---", 0},
    {"---", 0},
    {"---", 0},
};

void printHighScores() {
    printf("Highscores:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d. %s", i + 1, highScores[i].name);
        GoTo(28, 1 + i);
        printf("%d\n", highScores[i].score);
    }
}

void saveHighScores() {
    FILE *fp;

    fp = fopen("highscores.txt", "w");
    for (int i = 0; i < 10; i++) {
        fprintf(fp, "%s", highScores[i].name);
        fprintf(fp, "%c", ' ');
        fprintf(fp, "%d", highScores[i].score);
        fprintf(fp, "%s", "\n");
    }

    fclose(fp);
}

void loadHighScores() {
    FILE *fp;

    fp = fopen("highscores.txt", "r");

    if (fp != NULL) {
        for (int i = 0; i < 10; i++) {
            fscanf(fp, "%s %d\n", highScores[i].name, &highScores[i].score);
        }

        fclose(fp);
    }
}

struct Player_t {
    char name[25];
    char marker;
    int score;
};

struct Player_t player_1 = {" ", ' ', 0};
struct Player_t player_2 = {" ", ' ', 0};

void initializePlayers() {
    int output;

    system("cls");
    printf("Enter player 1 name (max 25 characters): ");
    scanf("%s", player_1.name);
    system("cls");
    printf("Enter player 2 name (max 25 characters): ");
    scanf("%s", player_2.name);
    system("cls");

    printf("Choose player's markers:\n");
    printf("1. Player %s - X, Player %s - O\n", player_1.name, player_2.name);
    printf("2. Player %s - O, Player %s - X\n", player_1.name, player_2.name);
    printf("First turn is for the X player!\n");
    scanf("%d", &output);

    if (output == 1) {
        player_1.marker = 'X';
        player_2.marker = 'O';
    } else {
        player_1.marker = 'O';
        player_2.marker = 'X';
    }

    player_1.score = 0;
    player_2.score = 0;
}

void insertPlayerData(struct Player_t player) {
    int alreadyExist = 0;
    int index;
    int buffer_d;
    char buffer_s[25];

    for (int i = 0; i < 10; i++) {
        if (strcmp(highScores[i].name, player.name) == 0) {
            highScores[i].score += player.score;
            alreadyExist = 1;
            break;
        }
    }

    if (alreadyExist == 0) {
        for (int i = 0; i < 10; i++) {
            if (highScores[i].score <= player.score) {
                index = i;
                break;
            }
        }

        for (int i = index; i < 10; i++) {
            for (int j = i + 1; j < 10; j++) {
                if (i == index) {
                    strcpy(buffer_s, highScores[i].name);
                    strcpy(highScores[i].name, highScores[j].name);
                    strcpy(highScores[j].name, buffer_s);

                    buffer_d = highScores[i].score;
                    highScores[i].score = highScores[j].score;
                    highScores[j].score = buffer_d;
                } else {
                    strcpy(buffer_s, highScores[i].name);
                    strcpy(highScores[i].name, highScores[j].name);
                    strcpy(highScores[j].name, buffer_s);

                    buffer_d = highScores[i].score;
                    highScores[i].score = highScores[j].score;
                    highScores[j].score = buffer_d;
                }
            }
        }

        strcpy(highScores[index].name, player.name);
        highScores[index].score = player.score;
    }
}

void sortHighScoresByScore() {
    int buffer_d;
    char buffer_s[25];

    for (int i = 0; i < 10; i++) {
        for (int j = i + 1; j < 10; j++) {
            if (highScores[i].score < highScores[j].score) {
                strcpy(buffer_s, highScores[i].name);
                strcpy(highScores[i].name, highScores[j].name);
                strcpy(highScores[j].name, buffer_s);

                buffer_d = highScores[i].score;
                highScores[i].score = highScores[j].score;
                highScores[j].score = buffer_d;
            }
        }
    }
}

char board[5][11] = {
    {' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' '},
    {'-', '-', '-', '|', '-', '-', '-', '|', '-', '-', '-'},
    {' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' '},
    {'-', '-', '-', '|', '-', '-', '-', '|', '-', '-', '-'},
    {' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' '}
};

void printBoard() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 11; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

void clearBoard() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i * 2][j * 4 + 1] = ' ';
        }
    }
}

void playerAction(char playerMarker) {
    int input, x, y;

    x = 1;
    y = 0;
    GoTo(x, y);

    do {
        if (kbhit()) {
            input = getch();
            if ((input == 119) && (y != 0)) {
                // move upward
                y -= 2;
                GoTo(x, y);
            }

            if ((input == 115) && (y != 4)) {
                // move downward
                y += 2;
                GoTo(x, y);
            }

            if ((input == 100) && (x != 9)) {
                // move right
                x += 4;
                GoTo(x, y);
            }

            if ((input == 97) && (x != 1)) {
                // move left
                x -= 4;
                GoTo(x, y);
            }
        }
    } while (input != 13);

    if (board[y][x] != ' ') {
        playerAction(playerMarker);
    } else {
        board[y][x] = playerMarker;
    }
}

int checkVictory(char playerMarker) {
    for (int i = 0; i < 3; i++) {
        if ((board[i * 2][1] == board[i * 2][5])
            && (board[i * 2][1] == board[i * 2][9])
            && (board[i * 2][1] == playerMarker)) {
            return 1;
        }
    }

    for (int j = 0; j < 3; j++) {
        if ((board[0][j * 4 + 1] == board[2][j * 4 + 1])
            && (board[0][j * 4 + 1] == board[4][j * 4 + 1])
            && (board[0][j * 4 + 1] == playerMarker)) {
            return 1;
        }
    }

    if ((board[0][1] == board[2][5]) && (board[0][1] == board[4][9]) && (board[0][1] == playerMarker)) {
        return 1;
    }

    if ((board[0][9] == board[2][5]) && (board[0][9] == board[4][1]) && (board[0][9] == playerMarker)) {
        return 1;
    }

    return 0;
}

int freeSpaces() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i * 2][j * 4 + 1] == ' ') {
                return 1;
            }
        }
    }

    return 0;
}


void gameLoop() {
    int currentPlayer;
    int VICTORY_1 = 0;
    int VICTORY_2 = 0;

    initializePlayers();

    if (player_1.marker == 'X') {
       currentPlayer = 1;
    } else if (player_1.marker == 'O') {
        currentPlayer = 2;
    }

    clearBoard();

    do {
        system("cls");
        printBoard();
        if (currentPlayer == 1) {
            printf("Current player: %s (%c)\n", player_1.name, player_1.marker);
        } else if (currentPlayer == 2) {
            printf("Current player: %s (%c)\n", player_2.name, player_2.marker);
        }

        if (currentPlayer == 1) {
            playerAction(player_1.marker);
            VICTORY_1 = checkVictory(player_1.marker);
            currentPlayer++;
        } else {
            playerAction(player_2.marker);
            VICTORY_2 = checkVictory(player_2.marker);
            currentPlayer--;
        }
    } while((freeSpaces() == 1) && (VICTORY_1 == 0) && (VICTORY_2 == 0));

    system("cls");
    if (freeSpaces() == 0) {
        printf("There are no free spaces!\n");
    }
    printBoard();
    printf("\n");
    if (VICTORY_1 == 1) {
        printf("Player %s won!\n", player_1.name);
        player_1.score++;
    } else if (VICTORY_2 == 1) {
        printf("Player %s won!\n\n", player_2.name);
        player_2.score++;
    }

    insertPlayerData(player_1);
    insertPlayerData(player_2);
    sortHighScoresByScore();
}

void printGameRules() {
    printf("CONTROLS\n");
    printf("**********************************************************************\n");
    printf("Use WASD keys to select a place for your mark.\n");
    printf("W - up.\n");
    printf("A - left.\n");
    printf("S - down.\n");
    printf("D - right.\n");
    printf("Press Enter to place your mark.\n");
    printf("**********************************************************************\n");
    printf("RULES\n");
    printf("**********************************************************************\n");
    printf("1. The game is played on a grid that's 3 squares by 3 squares.\n");
    printf("2. You are X, other player is O. Players take turns putting their\nmarks in empty squares.\n");
    printf("3. The first player to get 3 of his marks in a row\n(up, down, across, or diagonally) is the winner.\n");
    printf("4. When all 9 squares are full, the game is over. If no player\nhas 3 marks in a row, the game ends in a tie.\n");
    printf("**********************************************************************\n");
}

int menu() {
    int option;

    printf("1. Start\n");
    printf("2. Game rules\n");
    printf("3. Highscores\n");
    printf("4. Exit\n");

    do {
        scanf("%d", &option);
        if (option < 1 || option > 4) {
            system("cls");
            printf("1. Start\n");
            printf("2. Game rules\n");
            printf("3. Highscores\n");
            printf("4. Exit\n");
            printf("Choose correct option!\n");
        } else {
            break;
        }
    } while(option < 1 || option > 4);

    return option;
}

int main()
{
    SetConsoleTitle("Tic-Tac-Toe");

    int menuOutput;

    clearBoard();
    loadHighScores();

    do {
        menuOutput = menu();

        if (menuOutput == 1) {
            gameLoop();
        } else if (menuOutput == 2) {
            system("cls");
            printGameRules();
            printf("\nPress any key to return to main menu.\n");
            getch();
            system("cls");
        } else if (menuOutput == 3){
            system("cls");
            printHighScores();
            printf("\nPress any key to return to main menu.\n");
            getch();
            system("cls");
        } else {
            saveHighScores();
            break;
        }
    } while(menuOutput != 0);

    return 0;
}
