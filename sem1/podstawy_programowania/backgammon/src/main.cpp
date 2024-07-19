#include <ncurses.h>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

#define DEFAULT_PAIR 1
#define RED_PAIR 3
#define WHITE_PAIR 2
#define SELECT_PAIR 4
#define CORRECT_PAIR 5
#define BAR_PAIR 6

#define DEFINE_MAIN                                                                                                               \
    init();                                                                                                                       \
    int ch = 0, row = 5, col = 6, state = 0, dices[2] = {0, 0}, activity = 0, queue = WHITE_PAIR, score[2] = {0, 0}, botMode = 0; \
    Board desk[24], home[2], bar[2];                                                                                              \
    Names names;                                                                                                                  \
    initCheckers(desk, bar, home);

#define ADDHOWRED addToHOF(names.redName, score[1]);
#define ADDHOWWHITE addToHOF(names.whiteName, score[0]);
struct Coords
{
    int row;
    int col;
};
struct Board
{
    int color;
    int count;
};

struct Names
{
    char whiteName[11] = {"Player1"};
    char redName[11] = {"Player2"};
};

Coords getPos(int col, int row)
{
    Coords c;
    if (col / 6 == 0)
    {
        c.col = 53 - col * 4;
        c.row = 19 - row;
    }
    else if (col / 6 == 1)
    {
        c.col = 26 - (col - 6) * 4;
        c.row = 19 - row;
    }
    else if (col / 6 == 2)
    {
        c.col = 6 + (col - 12) * 4;
        c.row = 5 + row;
    }
    else if (col / 6 == 3)
    {
        c.col = 33 + (col - 18) * 4;
        c.row = 5 + row;
    }
    return c;
}

int my_strcmp(const char *str1, const char *str2)
{
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

void my_strcpy(char *dest, char *src)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void input(char *name, char *textBefore, int col = DEFAULT_PAIR)
{
    clear();
    int countB = 0, x, y;
    while (textBefore[countB] != '\0')
        countB++;

    x = (getmaxx(stdscr) - countB - 11) / 2;
    y = getmaxy(stdscr) / 2;
    WINDOW *win = newwin(1, 11 + countB, y, x);
    refresh();

    wattron(win, COLOR_PAIR(col));
    mvwprintw(win, 0, 0, "%s", textBefore);
    mvwscanw(win, 0, countB, "%10s", name);
    wattroff(win, COLOR_PAIR(col));
    delwin(win);
    clear();
}

void roll(int *dice1, int *dice2)
{
    *dice1 = rand() % 6 + 1;
    *dice2 = rand() % 6 + 1;
}

void printDownSide()
{
    mvprintw(13, 0, "   ||                                                   ||");
    for (int i = 0; i < 6; i++)
    {
        mvprintw(14 + i, 0, "   || /\\  ..  /\\  ..  /\\  ..     /\\  ..  /\\  ..  /\\  .. ||");
    }
    mvprintw(20, 0, "   ||===================================================||");
    mvprintw(21, 0, "      12  11  10  09  08  07     06  05  04  03  02  01");
    mvprintw(24, 40, "Matsvei Kasporvich s201627");
}

void printBoard()
{
    mvprintw(3, 6, "13  14  15  16  17  18     19  20  21  22  23  24");
    mvprintw(4, 0, "   ||===================================================||");
    for (int i = 0; i < 6; i++)
    {
        mvprintw(5 + i, 0, "   || ..  \\/  ..  \\/  ..  \\/     ..  \\/  ..  \\/  ..  \\/ ||");
    }
    mvprintw(11, 0, "   ||                                                   ||");
    mvprintw(12, 0, "   ||                       [BAR]                       ||[HOME]");
    printDownSide();
}

void printCheckers(Board desk[24])
{
    Coords coords;
    for (int i = 0; i < 24; i++)
    {
        attron(COLOR_PAIR(desk[i].color));
        for (int j = 0; j < 6; j++)
        {
            if (j < desk[i].count)
            {
                coords = getPos(i, j);
                mvprintw(coords.row, coords.col, "()");
            }
            else
            {
                break;
            }
        }

        if (desk[i].count > 6)
        {
            coords = getPos(i, 6);
            mvprintw(coords.row, coords.col, "+%d", desk[i].count - 6);
        }
        attroff(COLOR_PAIR(desk[i].color));
    }
}

void printHome(Board home[2])
{

    for (int i = 0; i < min(6, home[1].count); i++)
    {
        attron(COLOR_PAIR(home[1].color));
        mvprintw(10 - i, 58, "()");
        attroff(COLOR_PAIR(home[1].color));
    }
    for (int i = 0; i < min(6, home[0].count); i++)
    {
        attron(COLOR_PAIR(home[0].color));
        mvprintw(14 + i, 58, "()");
        attroff(COLOR_PAIR(home[0].color));
    }
    if (home[1].count > 6)
    {
        attron(COLOR_PAIR(home[1].color));
        mvprintw(11, 58, "+%d", home[1].count - 6);
        attroff(COLOR_PAIR(home[1].color));
    }
    if (home[0].count > 6)
    {
        attron(COLOR_PAIR(home[0].color));
        mvprintw(13, 58, "+%d", home[0].count - 6);
        attroff(COLOR_PAIR(home[0].color));
    }
}

void printBar(Board bar[2])
{
    if (bar[0].count > 0)
    {
        attron(COLOR_PAIR(bar[0].color));
        mvprintw(11, 28, "()x%.2d", bar[0].count);
        attroff(COLOR_PAIR(bar[0].color));
    }
    if (bar[1].count > 0)
    {
        attron(COLOR_PAIR(bar[1].color));
        mvprintw(13, 28, "()x%.2d", bar[1].count);
        attroff(COLOR_PAIR(bar[1].color));
    }
}
void printScore(int score[2], Names names)
{
    attron(COLOR_PAIR(WHITE_PAIR));
    mvprintw(0, 0, "%s: %d", names.whiteName, score[0]);
    attroff(COLOR_PAIR(WHITE_PAIR));
    attron(COLOR_PAIR(RED_PAIR));
    mvprintw(1, 0, "%s: %d", names.redName, score[1]);
    attroff(COLOR_PAIR(RED_PAIR));
}

void drawBoard(Board desk[24], Board home[2], Board bar[2])
{
    attron(COLOR_PAIR(DEFAULT_PAIR));
    printBoard();
    attroff(COLOR_PAIR(DEFAULT_PAIR));
    printCheckers(desk);
    printHome(home);
    printBar(bar);
    refresh();
}

void testCheckers(Board *desk)
{
    for (int i = 0; i < 24; i++)
    {
        desk[i] = {0, 0};
    }
    desk[23] = {RED_PAIR, 15};
    desk[0] = {WHITE_PAIR, 15};
}

void initCheckers(Board *desk, Board bar[2], Board home[2])
{
    home[0] = {WHITE_PAIR, 0};
    home[1] = {RED_PAIR, 0};
    bar[0] = {WHITE_PAIR, 0};
    bar[1] = {RED_PAIR, 0};
    for (int i = 0; i < 24; i++)
    {
        switch (i)
        {
        case 0:
            desk[i] = {WHITE_PAIR, 2};
            break;
        case 5:
        case 12:
            desk[i] = {RED_PAIR, 5};
            break;
        case 7:
            desk[i] = {RED_PAIR, 3};
            break;
        case 11:
        case 18:
            desk[i] = {WHITE_PAIR, 5};
            break;
        case 16:
            desk[i] = {WHITE_PAIR, 3};
            break;
        case 23:
            desk[i] = {RED_PAIR, 2};
            break;
        default:
            desk[i] = {0, 0};
            break;
        }
    }
    // testCheckers(desk);
}

void printMenu(int state)
{
    const char *menuOptions[] = {
        "New",
        "Load",
        "Replay",
        "Hall of Fame",
        "Bot",
        "Exit"};

    for (int i = 0; i < 6; i++)
    {
        attron(COLOR_PAIR(DEFAULT_PAIR));
        if (i == state)
        {
            attron(COLOR_PAIR(SELECT_PAIR));
            mvprintw(9 + i, 90, "%i. %s", i + 1, menuOptions[i]);
            attroff(COLOR_PAIR(SELECT_PAIR));
        }
        else
        {
            mvprintw(9 + i, 90, "%i. %s", i + 1, menuOptions[i]);
        }
        attroff(COLOR_PAIR(DEFAULT_PAIR));
    }
}

void printDices(int dice1, int dice2)
{
    attron(COLOR_PAIR(WHITE_PAIR));
    mvprintw(22, 4, "||===||");
    mvprintw(23, 4, "|| %i ||", dice1);
    mvprintw(24, 4, "||===||");
    attroff(COLOR_PAIR(WHITE_PAIR));
    attron(COLOR_PAIR(RED_PAIR));
    mvprintw(22, 13, "||===||");
    mvprintw(23, 13, "|| %i ||", dice2);
    mvprintw(24, 13, "||===||");
    attroff(COLOR_PAIR(RED_PAIR));
}

void init()
{
    initscr();
    start_color();
    init_pair(DEFAULT_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(RED_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(WHITE_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(SELECT_PAIR, COLOR_WHITE, COLOR_CYAN);
    init_pair(CORRECT_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(BAR_PAIR, COLOR_WHITE, COLOR_RED);
    curs_set(0);
    keypad(stdscr, TRUE); // enable arrow keys
    srand(time(0));
}

void printBase(Board desk[24], Board home[2], Board bar[2], int dices[2])
{
    drawBoard(desk, home, bar);
    printDices(dices[0], dices[1]);
}

void clearHistory()
{
    FILE *file = fopen("history", "w");
    fclose(file);
}

int loading(int count, char **files)
{
    int offset = 0, ch = 0;
    while (ch != '\n' && ch != KEY_ENTER)
    {
        clear();
        for (int i = 0; i < count; i++)
        {
            if (i == offset)
            {
                attron(COLOR_PAIR(SELECT_PAIR));
                mvprintw(9 + i, 90, "%i. %s", i + 1, files[i]);
                attroff(COLOR_PAIR(SELECT_PAIR));
            }
            else
                mvprintw(9 + i, 90, "%i. %s", i + 1, files[i]);
        }
        ch = getch();
        if (ch == KEY_UP)
        {
            offset--;
            offset = (offset + count) % count;
        }
        else if (ch == KEY_DOWN)
        {
            offset++;
            offset %= count;
        }
    }
    return offset;
}

void chooseSaveFile(char *filename)
{
    int ch = 0, count = 0;
    char **files = NULL;

    FILE *file = fopen("saves", "r");
    while (!feof(file))
    {
        files = (char **)realloc(files, (count + 1) * sizeof(char *));
        files[count] = (char *)malloc(11 * sizeof(char));
        fscanf(file, "%s\n", files[count]);
        count++;
    }
    fclose(file);

    int offset = loading(count, files);
    copy(files[offset], files[offset] + 11, filename);
    free(files);
}

void saveN(char *fileName, Board desk[24], Board home[2], Board bar[2], int dices[2], int queue, Names names)
{

    FILE *file = fopen(fileName, "w");
    fprintf(file, "%d %d\n", dices[0], dices[1]);
    fprintf(file, "%d\n", queue);
    for (int i = 0; i < 24; i++)
    {
        fprintf(file, "%d %d\n", desk[i].color, desk[i].count);
    }
    fprintf(file, "%d %d\n", home[0].color, home[0].count);
    fprintf(file, "%d %d\n", home[1].color, home[1].count);
    fprintf(file, "%d %d\n", bar[0].color, bar[0].count);
    fprintf(file, "%d %d\n", bar[1].color, bar[1].count);
    fprintf(file, "%s\n", names.whiteName);
    fprintf(file, "%s\n", names.redName);
    fclose(file);
}

void saveGame(Board desk[24], Board home[2], Board bar[2], int dices[2], int queue, Names names, char *fileName)
{
    FILE *file1 = fopen("saves", "r+");
    bool isExist = false;
    int count = 0;
    char fname[11];

    while (!feof(file1))
    {
        fscanf(file1, "%s\n", fname);
        if (my_strcmp(fname, fileName) == 0)
        {
            isExist = true;
            break;
        }
        count++;
    }
    if (!isExist)
        fprintf(file1, "%s\n", fileName);
    fclose(file1);
    saveN(fileName, desk, home, bar, dices, queue, names);
}

void loadGame(Board desk[24], Board home[2], Board bar[2], int dices[2], int *queue, Names *names, char *filename)
{
    FILE *file = fopen(filename, "r");
    fscanf(file, "%d %d\n", &dices[0], &dices[1]);
    fscanf(file, "%d\n", queue);

    for (int i = 0; i < 24; i++)
        fscanf(file, "%d %d\n", &desk[i].color, &desk[i].count);

    int *pairs[] = {&home[0].color, &home[0].count, &home[1].color, &home[1].count, &bar[0].color, &bar[0].count, &bar[1].color, &bar[1].count};
    for (int i = 0; i < 8; i += 2)
        fscanf(file, "%d %d\n", pairs[i], pairs[i + 1]);

    fscanf(file, "%s\n", names->whiteName);
    fscanf(file, "%s\n", names->redName);
    fclose(file);
}

void saveGameHistory(Board desk[24], Board home[2], Board bar[2], int dices[2], int queue, Names names)
{
    FILE *file = fopen("history", "a");
    fprintf(file, "%d %d\n", dices[0], dices[1]);
    fprintf(file, "%d\n", queue);
    for (int i = 0; i < 24; i++)
    {
        fprintf(file, "%d %d\n", desk[i].color, desk[i].count);
    }
    fprintf(file, "%d %d\n", home[0].color, home[0].count);
    fprintf(file, "%d %d\n", home[1].color, home[1].count);
    fprintf(file, "%d %d\n", bar[0].color, bar[0].count);
    fprintf(file, "%d %d\n", bar[1].color, bar[1].count);
    fprintf(file, "%s\n", names.whiteName);
    fprintf(file, "%s\n", names.redName);
    fclose(file);
}

int getCountHistory(int **startPoints)
{
    FILE *file = fopen("history", "r");
    int count = 1;
    int ch;
    while ((ch = fgetc(file)) != EOF)
    {
        if ((count - 1) % 32 == 0)
        {
            *startPoints = (int *)realloc(*startPoints, count * sizeof(int));
            (*startPoints)[(count - 1) / 32] = ftell(file) - 4;
        }
        if (ch == '\n')
        {
            count++;
        }
    }
    fclose(file);
    return count / 32;
}

void scanHistory(Board desk[24], Board home[2], Board bar[2], int dices[2], int *queue, Names *names, int startPoint)
{
    FILE *file = fopen("history", "r");
    fseek(file, startPoint, SEEK_SET);
    fscanf(file, "%d %d\n", &dices[0], &dices[1]);
    fscanf(file, "%d\n", queue);

    Board *boards[] = {desk, home, bar};
    int sizes[] = {24, 2, 2};

    for (int b = 0; b < 3; b++)
    {
        for (int i = 0; i < sizes[b]; i++)
        {
            fscanf(file, "%d %d\n", &boards[b][i].color, &boards[b][i].count);
        }
    }

    fscanf(file, "%s\n", names->whiteName);
    fscanf(file, "%s\n", names->redName);
    fclose(file);
}

void replayGameHistory()
{
    int dices[2], queue, ch = 0, offset = 0;
    Board desk[24], home[2], bar[2];
    int *startPoints = (int *)malloc(sizeof(int));
    *startPoints = 0;
    Names names;
    int count = getCountHistory(&startPoints);

    while (ch != '\n')
    {
        clear();
        mvprintw(getmaxy(stdscr) - 1, getmaxx(stdscr) - 2 * floor(log10(count) + 2), "%d/%d", offset + 1, count);
        scanHistory(desk, home, bar, dices, &queue, &names, startPoints[offset]);
        printBase(desk, home, bar, dices);

        ch = getch();
        offset = (ch == KEY_UP) ? count - 1 : (ch == KEY_DOWN) ? 0
                                          : (ch == KEY_LEFT)   ? (offset + count - 1) % count
                                                               : (offset + 1) % count;
    }
}

void isPossible(Board desk[24], int queue, int secmove, int *way, bool isPrint, int p, Coords coords, int secmoveState)
{
    bool state = (p < 24 && p >= 0) && (desk[p].color == queue || desk[p].count == 0 || (desk[p].count == 1 && desk[p].color != queue));
    int pair;
    if (secmoveState == -1)
    {
        state = state && secmove == secmoveState;
    }
    else
        state = state && secmove != secmoveState;

    if (state)
    {
        coords = getPos(p, 0);
        *way = p;
        if (isPrint)
        {
            if (desk[p].count == 1 && desk[p].color != queue)
                pair = BAR_PAIR;
            else
                pair = CORRECT_PAIR;
            attron(COLOR_PAIR(pair));
            mvprintw(coords.row, coords.col, "()");
            attroff(COLOR_PAIR(pair));
        }
    }
}

bool isHomePossible(int p[3], int k, bool isHome, int ways[3])
{
    bool state = (p[k] >= 24 || p[k] < 0) && isHome;
    if (k == 1)
        state = state && ways[0] != 26 && ways[0] != -2;
    else if (k == 2)
        state = state && ways[0] != 26 && ways[0] != -2 && ways[1] != 26 && ways[1] != -2;

    if (state)
    {
        if (p[k] >= 24)
        {
            if (k == 2)
                ways[k] = 27;
            else
                ways[k] = 26;
        }
        else
        {
            if (k == 2)
                ways[k] = -3;
            else
                ways[k] = -2;
        }
        return false;
    }
    return true;
}

void getPossibleWays(Board desk[24], int queue, int dices[2], int pos, int *ways, int secmove, bool isPrint = true, bool isHome = false)
{
    int k = 1;
    if (queue == WHITE_PAIR)
        k = -1;

    int p[3];
    int c[3] = {2, 1, -1};
    p[0] = pos + dices[0] * k;
    p[1] = pos + dices[1] * k;
    p[2] = pos + (dices[0] + dices[1]) * k;
    Coords coords;

    for (int i = 0; i < 3; i++)
    {
        if (isHomePossible(p, i, isHome, ways))
        {
            isPossible(desk, queue, secmove, &ways[i], isPrint, p[i], coords, c[i]);
        }
    }
    if (ways[0] == ways[1])
        ways[1] = 25;
}

void moveOnDesk(Board desk[24], Board bar[2], int from, int to, Board home[2])
{
    int col = (to >= 26 || from == -1) ? RED_PAIR : WHITE_PAIR;
    if (to >= 26 || to <= -2)
    {
        desk[from].count--;
        home[col - 2].count++;
        return;
    }

    if (from == 24 || from == -1)
    {
        bar[col - 2].count--;
        if (desk[to].count != 1 || desk[to].color == col)
        {
            desk[to].count++;
        }
        else
        {
            bar[desk[to].color - 2].count++;
        }
        desk[to].color = col;
    }
    else
    {
        desk[from].count--;
        if (desk[to].count != 1 || desk[to].color == desk[from].color)
            desk[to].count++;
        else
        {
            bar[desk[to].color - 2].count++;
        }
        desk[to].color = desk[from].color;
    }
}

void printPossibleWays(Board desk[24], int ways[3], int cur, Coords pos)
{
    for (int i = 0; i < 3; i++) // print possible ways
    {
        if (ways[i] != 25 && ways[i] < 24 && ways[i] >= 0) // if way is on desk
        {
            pos = getPos(ways[i], 0);
            if (desk[ways[i]].count == 1 && desk[ways[i]].color != desk[cur].color) // if there is one checker of another color
            {
                attron(COLOR_PAIR(BAR_PAIR));
                mvprintw(pos.row, pos.col, "()");
                attroff(COLOR_PAIR(BAR_PAIR));
            }
            else
            {
                attron(COLOR_PAIR(CORRECT_PAIR));
                mvprintw(pos.row, pos.col, "()");
                attroff(COLOR_PAIR(CORRECT_PAIR));
            }
        }
    }
}

void printSelectedWay(int ways[3], int offset, Coords pos)
{
    attron(COLOR_PAIR(SELECT_PAIR));
    if (ways[offset] == 26 || ways[offset] == -2 || ways[offset] == 27 || ways[offset] == -3)
    {
        mvprintw(12, 58, "[HOME]");
    }
    else if (ways[offset] < 24 && ways[offset] >= 0)
    {
        pos = getPos(ways[offset], 0);
        mvprintw(pos.row, pos.col, "()");
    }
    attroff(COLOR_PAIR(SELECT_PAIR));
}

int enterHandler(int ways[3], int offset, int cur, Board desk[24], Board home[2], Board bar[2], int dices[2])
{
    moveOnDesk(desk, bar, cur, ways[offset], home); // move checker
    if (ways[offset] == 26 || ways[offset] < -2)
    {
        return 1;
    }
    if (ways[offset] == 27 || ways[offset] == -3)
    {
        return 0;
    }
    if (abs(cur - ways[offset]) == dices[0] + dices[1])
        return 0;
    else if (abs(cur - ways[offset]) == dices[0])
        return 1;
    else
        return 2;
}
int moveCheckerKeyHandler(int ch, int *offset, int ways[3], int cur, Board desk[24], Board home[2], Board bar[2], int dices[2], int queue, Names names)
{
    if (ch == KEY_LEFT || ch == KEY_DOWN || ch == KEY_RIGHT || ch == KEY_UP)
    {
        do
        {
            *offset = (ch == KEY_LEFT || ch == KEY_DOWN) ? (*offset + 1) % 3 : (*offset + 2) % 3;
        } while (ways[*offset] == 25);
    }
    else if (ch == KEY_BACKSPACE)
    {
        return 3;
    }
    else if (ch == 's')
    {
        char filename[11];
        input(filename, (char *)"Enter file name: ");
        saveGame(desk, home, bar, dices, queue, names, filename);
    }
    else if (ch == KEY_ENTER || ch == '\n')
    {
        return enterHandler(ways, *offset, cur, desk, home, bar, dices);
    }
    return -1;
}

void totalPrint(Board desk[24], int ways[3], Board home[2], Board bar[2], int dices[2], int cur, Names names, int score[2], Coords pos, int offset = 0)
{
    clear();
    printBase(desk, home, bar, dices);
    printScore(score, names);
    printPossibleWays(desk, ways, cur, pos);
    printSelectedWay(ways, offset, pos);
}

int moveChecker(Board desk[24], int ways[3], Board home[2], Board bar[2], int dices[2], int cur, int queue, Names names, int score[2], int botMode)
{
    int ch = 0, count = 0, offset = 0, b;
    Coords pos;
    for (int i = 0; i < 3; i++) // count possible ways and set offset
    {
        if (ways[i] != 25)
        {
            count++;
            offset = i;
        }
    }
    while (ch != '\n' && ch != KEY_ENTER)
    {
        totalPrint(desk, ways, home, bar, dices, cur, names, score, pos, offset);

        if (botMode == 0 || queue == WHITE_PAIR)
            ch = getch();
        else if (botMode == 1)
            ch = (rand() % 4 == 0) ? KEY_ENTER : KEY_RIGHT;
        b = moveCheckerKeyHandler(ch, &offset, ways, cur, desk, home, bar, dices, queue, names);
        if (b != -1)
            return b;
    }
    return 0;
}

int countUnique(int ways[3])
{
    int count = 0;
    for (int i = 0; i < 3; i++)
    {
        if (ways[i] != 25)
        {
            bool unique = true;
            for (int j = 0; j < i; j++)
            {
                if (ways[j] == ways[i])
                {
                    unique = false;
                    break;
                }
            }
            if (unique)
                count++;
        }
    }
    return count;
}

void fromBar(Board desk[24], Board bar[2], Board home[2], int dices[2], int queue, int *secmove, Names names, int score[2], int botMode)
{
    int ways[3] = {25, 25, 25};
    printBase(desk, home, bar, dices);
    getPossibleWays(desk, queue, dices, (queue == WHITE_PAIR ? 24 : -1), ways, *secmove);
    if (ways[0] == 25 && ways[1] == 25 && ways[2] == 25)
    {
        *secmove = 0;
        return;
    }
    *secmove = moveChecker(desk, ways, home, bar, dices, (queue == WHITE_PAIR ? 24 : -1), queue, names, score, botMode);
    clear();
}

bool checkHome(Board desk[24], Board home[2], Board bar[2], int dices[2], int queue)
{
    int count = 0;
    for (int i = 0; i < 6; i++)
    {
        if (queue == WHITE_PAIR)
        {
            if (desk[i].color == queue)
            {
                count += desk[i].count;
            }
        }
        else
        {
            if (desk[23 - i].color == queue)
            {
                count += desk[23 - i].count;
            }
        }
    }
    if (count == 15 - home[queue - 2].count)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int isEnd(Board desk[24])
{
    int countWhite = 0, countRed = 0;
    for (int i = 0; i < 24; i++)
    {
        if (desk[i].color == WHITE_PAIR)
        {
            countWhite += desk[i].count;
        }
        else
        {
            countRed += desk[i].count;
        }
    }
    if (countWhite == 0)
    {
        return WHITE_PAIR;
    }
    else if (countRed == 0)
    {
        return RED_PAIR;
    }
    else
    {
        return 0;
    }
}

void checkersOnBar(Board desk[24], Board bar[2], int queue, Board home[2], int dices[2], int *secmove, Names names, int score[2], int botMode)
{
    if (bar[queue - 2].count != 0) // if there are checkers on bar
    {
        fromBar(desk, bar, home, dices, queue, secmove, names, score, botMode);
        if (secmove == 0)
            return;
    }
    if (bar[queue - 2].count != 0) // same
    {
        fromBar(desk, bar, home, dices, queue, secmove, names, score, botMode);
        return;
    }
}

void chooseCheckerEnter(int *ch, int *secmove, int *cur, int ways[3], Board desk[24], Board home[2], Board bar[2], int dices[2], int queue, Names names, int score[2], int botMode)
{
    if (ways[0] != 25 || ways[1] != 25 || ways[2] != 25) // if there are possible ways
    {
        if (*secmove == -1)
        {
            *secmove = moveChecker(desk, ways, home, bar, dices, *cur, queue, names, score, botMode); // move checker
            if (*secmove == 3)
            {
                *secmove = -1;
            }
            if (*secmove != 0)
                *ch = 0;
        }
        else
        {
            *secmove = moveChecker(desk, ways, home, bar, dices, *cur, queue, names, score, botMode); // move checker
        }
    }
    else
        *ch = 0;
}

void chooseCheckerKeyHandler(int *ch, int *activity, int *secmove, int ways[3], int count, int *offset, int *cur, Board desk[24], Board home[2], Board bar[2], int dices[2], int queue, Names names, int score[2], int botMode)
{
    if (*ch == 's')
    {
        char filename[11];
        input(filename, (char *)"Enter file name: ");
        saveGame(desk, home, bar, dices, queue, names, filename);
    }
    else if (*ch == KEY_ENTER || *ch == '\n')
    {
        chooseCheckerEnter(ch, secmove, cur, ways, desk, home, bar, dices, queue, names, score, botMode);
    }
    else
    {
        *offset = (*ch == KEY_LEFT || *ch == KEY_DOWN) ? (*offset + 1) % count : (*offset + count - 1) % count;
    }
}

void checkHomeCheckers(int *score, Board desk[24], bool isRed)
{
    for (int i = 0; i < 6; i++)
    {
        if (desk[abs(23 * isRed - i)].color == (isRed ? WHITE_PAIR : RED_PAIR) && desk[abs(23 * isRed - i)].count != 0)
        {
            *score += 3;
            return;
        }
    }
    *score += 2;
}

void scoreHandler(int score[2], Board home[2], Board desk[24])
{
    if (home[0].count == 15)
    {
        if (home[1].count != 0)
            score[0]++;
        else
            checkHomeCheckers(&score[0], desk, 0);
    }
    else
    {
        if (home[0].count != 0)
            score[1]++;
        else
            checkHomeCheckers(&score[1], desk, 1);
    }
}

void pcol(int col, int row, int color, char *str)
{
    attron(COLOR_PAIR(color));
    mvprintw(row, col, "%s", str);
    attroff(COLOR_PAIR(color));
}

void getWays(Board desk[24], int queue, Board bar[2], Board home[2], int dices[2], int *count, int secmove, int *waysCount, int offset, int ways[3], int *cur)
{
    bool isHome = false;
    for (int i = 0; i < 24; i++) // count checkers, get possible ways
    {
        int genWays[3] = {25, 25, 25};
        if (desk[i].color == queue && desk[i].count > 0)
        {
            isHome = checkHome(desk, home, bar, dices, queue);
            (*count)++;
            getPossibleWays(desk, queue, dices, i, genWays, secmove, false, isHome);
            (*waysCount) += countUnique(genWays);
            if (offset + 1 == *count)
            {
                Coords pos;
                pos = getPos(i, min(5, desk[i].count - 1));
                getPossibleWays(desk, queue, dices, i, ways, secmove, true, isHome);
                pcol(pos.col, pos.row, SELECT_PAIR, (char *)"()");
                if (isHome)
                {
                    pcol(58, 12, CORRECT_PAIR, (char *)"[HOME]");
                }
                (*cur) = i;
            }
        }
    }
}

void chooseChecker(Board desk[24], int queue, Board home[2], Board bar[2], int dices[2], int *activity, Names names, int score[2], int botMode)
{
    int ch = 0, count = 0, offset = 0, cur = 0, secmove = -1;

    checkersOnBar(desk, bar, queue, home, dices, &secmove, names, score, botMode);

    while (ch != '\n' && ch != KEY_ENTER) // choose checker to take
    {
        int ways[3] = {25, 25, 25}, waysCount = 0;
        if (isEnd(desk) != 0)
        {
            *activity = 0;
            scoreHandler(score, home, desk);
            return;
        }

        count = 0;
        printBase(desk, home, bar, dices);
        printScore(score, names);
        getWays(desk, queue, bar, home, dices, &count, secmove, &waysCount, offset, ways, &cur);
        // getWays(desk, queue, bar, home, dices, &count, secmove, &waysCount, offset, ways, &cur);

        if (waysCount == 0)
            return;
        // ch = (botMode == 0 || queue == WHITE_PAIR) ? getch() : ((rand() % 4 == 0) ? KEY_ENTER : KEY_RIGHT);
        ch = (botMode == 0 || queue == WHITE_PAIR) ? getch() : ((rand() % 4 == 0) ? KEY_ENTER : KEY_RIGHT);
        clear();
        if (ch == KEY_BACKSPACE)
        {
            saveGame(desk, home, bar, dices, queue, names, (char *)"autosave");
            *activity = 0;
            return;
        }
        chooseCheckerKeyHandler(&ch, activity, &secmove, ways, count, &offset, &cur, desk, home, bar, dices, queue, names, score, botMode);
        // chooseCheckerKeyHandler(&ch, activity, &secmove, ways, count, &offset, &cur, desk, home, bar, dices, queue, names, score, botMode);
    }
}
struct Player
{
    int score;
    char name[11];
};

void sortByScore(Player *players, int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (players[j].score < players[j + 1].score)
            {
                Player temp = players[j];
                players[j] = players[j + 1];
                players[j + 1] = temp;
            }
        }
    }
}

void hallOfFame()
{
    clear();
    FILE *file = fopen("hallOfFame", "r");
    Player *players = NULL;
    int count = 0;
    while (!feof(file))
    {
        players = (Player *)realloc(players, (count + 1) * sizeof(Player));
        fscanf(file, "%s %d\n", players[count].name, &players[count].score);
        count++;
    }
    sortByScore(players, count);
    for (int i = 0; i < count; i++)
    {
        int size = 0;
        while (players[i].name[size] != '\0')
            size++;
        mvprintw(1 + i, (getmaxx(stdscr) - size) / 2, "%s: %d", players[i].name, players[i].score);
    }
    free(players);
    fclose(file);
    getch();
    clear();
}

void addHOF(int playerCount, Player **players, char *name, int score)
{
    for (int i = 0; i < playerCount; i++)
    {
        if ((my_strcmp(name, (*players)[i].name) == 0))
        {
            if (score > (*players)[i].score)
            {
                (*players)[i].score = score;
                break;
            }
        }
    }
}

void addToHOF(char *name, int score)
{
    FILE *file = fopen("hallOfFame", "r");
    Player player;
    Player *players = NULL;
    int playerCount = 0;

    while (!feof(file))
    {
        fscanf(file, "%s %d\n", player.name, &player.score);
        players = (Player *)realloc(players, (playerCount + 1) * sizeof(Player));
        players[playerCount] = player;
        playerCount++;
    }
    fclose(file);

    addHOF(playerCount, &players, name, score);

    file = fopen("hallOfFame", "w");
    for (int i = 0; i < playerCount; i++)
    {
        fprintf(file, "%s %d\n", players[i].name, players[i].score);
    }
    fclose(file);
    free(players);
}

void getFromHOF(char *name, int *score)
{
    FILE *file = fopen("hallOfFame", "r");
    Player player;
    int count = 0;
    while (!feof(file))
    {
        fscanf(file, "%s %d\n", player.name, &player.score);
        mvprintw(1, 0, "%s", name);
        if (my_strcmp(name, player.name) == 0)
        {
            *score = player.score;
            fclose(file);
            return;
        }
    }
    fclose(file);
}

void setBot(int *botMode)
{
    int ch = 0, state = 0;
    const char *menuOptions[3] = {"Player", "Random", "Algorithm"};
    while (ch != '\n' && ch != KEY_ENTER)
    {
        clear();
        for (int i = 0; i < 3; i++)
        {
            attron(COLOR_PAIR(DEFAULT_PAIR));
            if (i == state)
            {
                attron(COLOR_PAIR(SELECT_PAIR));
                mvprintw(9 + i, 90, "%i. %s", i + 1, menuOptions[i]);
                attroff(COLOR_PAIR(SELECT_PAIR));
            }
            else
            {
                mvprintw(9 + i, 90, "%i. %s", i + 1, menuOptions[i]);
            }
            attroff(COLOR_PAIR(DEFAULT_PAIR));
        }
        ch = getch();
        if (ch == KEY_UP)
            state = (state + 2) % 3;
        else if (ch == KEY_DOWN)
            state = (state + 1) % 3;
    }
    *botMode = state;
    clear();
    return;
}

void defaulsNew(Names names, int dices[2], int *queue, Board desk[24], Board home[2], Board bar[2], int score[2])
{
    clearHistory();
    getFromHOF(names.whiteName, &score[0]);
    getFromHOF(names.redName, &score[1]);
    addToHOF(names.whiteName, score[0]);
    addToHOF(names.redName, score[1]);
    while (dices[0] == dices[1])
        roll(&dices[0], &dices[1]);
    if (dices[1] < dices[0])
        *queue = WHITE_PAIR;
    else
        *queue = RED_PAIR;
    saveGameHistory(desk, home, bar, dices, *queue, names);
}

void newgame(Board desk[24], Board home[2], Board bar[2], int dices[2], int *queue, Names names, int score[2], int botMode, int *activity)
{
    initCheckers(desk, bar, home);
    input(names.whiteName, (char *)"Enter name of white player: ", WHITE_PAIR);
    if (botMode == 0)
        input(names.redName, (char *)"Enter name of red player: ", RED_PAIR);
    else if (botMode == 1)
    {
        my_strcpy(names.redName, (char *)"BotRandom");
    }
    else if (botMode == 2)
    {
        my_strcpy(names.redName, (char *)"BotAlg");
    }

    defaulsNew(names, dices, queue, desk, home, bar, score);
    *activity = 1;
}

void menuSC(int state, Board desk[24], Board home[2], Board bar[2], int dices[2], int *queue, Names names, int score[2], int botMode, int *activity, int *ch)
{
    switch (state)
    {
    case 0: // new
        newgame(desk, home, bar, dices, queue, names, score, botMode, activity);
        break;
    case 1: // load
        char filename[11];
        chooseSaveFile(filename);
        loadGame(desk, home, bar, dices, queue, &names, filename);
        *activity = 2;
        break;
    case 2: // replay
        replayGameHistory();
        break;
    case 3: // hall of fame
        hallOfFame();
        break;
    case 4: // bot
        setBot(&botMode);
        break;
    case 5: // exit
        *ch = 'q';
        break;
    }
}

// activity 0 - menu, 1 - game, 2 - continue game
int main()
{
    DEFINE_MAIN;

    do
    {
        if (activity == 0)
        {
            if (ch == KEY_UP)
                state = (state + 5) % 6;
            else if (ch == KEY_DOWN)
                state = (state + 1) % 6;
            else if (ch == KEY_ENTER || ch == '\n')
            {
                if (state == 6)
                    break;
                menuSC(state, desk, home, bar, dices, &queue, names, score, botMode, &activity, &ch);
            }
            printMenu(state);
        }
        if (activity == 1 || activity == 2)
        {
            if (activity == 2)
                activity = 1;
            else
                roll(&dices[0], &dices[1]);
            clear();
            chooseChecker(desk, queue, home, bar, dices, &activity, names, score, botMode);
            saveGameHistory(desk, home, bar, dices, queue, names);
            if (dices[0] == dices[1] && activity == 1)
            {
                chooseChecker(desk, queue, home, bar, dices, &activity, names, score, botMode);
                saveGameHistory(desk, home, bar, dices, queue, names);
            }

            ADDHOWRED;
            ADDHOWWHITE;

            if (activity == 0)
            {
                ch = 0;
                continue;
            }
            queue = (queue + 1) % 2 + 2;
        }
        printBase(desk, home, bar, dices);
        printScore(score, names);
        if (activity == 0 && ch != 'q')
            ch = getch();

    } while (ch != 'q');
    endwin();

    return 0;
}