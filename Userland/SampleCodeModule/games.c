// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <lib.h>
#include <libc.h>
#include <games.h>

int isSafe(char grid[9][9], char c, int row, int col);

//Variables hour
int h, m, s;
char hours[2];
char min[2];
char sec[2];

//variables Sudoku
char grid[9][9] = {{'3', '_', '6', '5', '_', '8', '4', '_', '_'},
                   {'5', '2', '_', '_', '_', '_', '_', '_', '_'},
                   {'_', '8', '7', '_', '_', '_', '_', '3', '1'},
                   {'_', '_', '3', '_', '1', '_', '_', '8', '_'},
                   {'9', '_', '_', '8', '6', '3', '_', '_', '5'},
                   {'_', '5', '_', '_', '9', '_', '6', '_', '_'},
                   {'1', '3', '_', '_', '_', '_', '2', '5', '_'},
                   {'_', '_', '_', '_', '_', '_', '_', '7', '4'},
                   {'_', '_', '5', '2', '_', '6', '3', '_', '_'}};
//Sudoku de solucion unica
char gridSolved[9][9] = {{'3', '1', '6', '5', '7', '8', '4', '9', '2'},
                         {'5', '2', '9', '1', '3', '4', '7', '6', '8'},
                         {'4', '8', '7', '6', '2', '9', '5', '3', '1'},
                         {'2', '6', '3', '4', '1', '5', '9', '8', '7'},
                         {'9', '7', '4', '8', '6', '3', '1', '2', '5'},
                         {'8', '5', '1', '7', '9', '2', '6', '4', '3'},
                         {'1', '3', '8', '9', '4', '7', '2', '5', '6'},
                         {'6', '9', '2', '3', '5', '1', '8', '7', '4'},
                         {'7', '4', '5', '2', '8', '6', '3', '1', '9'}};
char num[2] = {0};
char rowChar[2] = {0};
char colChar[2] = {0};
char c[2] = {0};
int perdio;
int termino;

//Varaibles stopWatch
int secs, dec, mins;
char d[2];
char ms[2];
char ss[2];
int t;

//Variables hangman
int gano;
int vidas;
char palabra[] = "ARQUI";
int wlen;
int count;
int guessed[5];
char v[2];

void resetStopwatch()
{
    secs = 0;
    dec = 0;
    mins = 0;
    d[0] = 0;
    ms[0] = 0;
    ss[0] = 0;
    writeAtPos(1, "0", 1, 780, 150);
    writeAtPos(1, "::", 2, 760, 150);
    writeAtPos(1, "00", 2, 740, 150);
    writeAtPos(1, "::", 2, 720, 150);
    writeAtPos(1, "00", 2, 700, 150);

}

void initVarGames()
{
    //inicializando variables para hour
    h = 0, m = 0, s = 0;
    hours[0] = 0;
    min[0] = 0;
    sec[0] = 0;

    //inicializando variables para stopWatch
    char *str1 = "Aclaracion: llega hasta los 60 minutos y se reinicia.";
    char *str2 = "Manejo con el teclado:";
    char *str3 = "Pulse s para parar y reiniciar el cronometro.";
    char *str4 = "Pulse p para pausar el cronometro.";
    char *str5 = "Pulse i para iniciar el cronometro.";
    writeAtPos(1, str1, strlen(str1), 515, 0);
    writeAtPos(1, str2, strlen(str2), 515, 20);
    writeAtPos(1, str3, strlen(str3), 530, 40);
    writeAtPos(1, str4, strlen(str4), 530, 60);
    writeAtPos(1, str5, strlen(str5), 530, 80);
    resetStopwatch();

    //inicializando variables para Sodoku
    char *str13 = "Ingrese numeros del 1 al 9. Fila - Columna - Numero";
    char *str14 = "Si es correcto se impreme en la grilla.";
    char *str15 = "Si es incorrecto se muestra el error.";
    char *str17 = "NUMERO: ";
    char *str18 = "FILA: ";
    char *str19 = "COLUMNA: ";
    writeAtPos(1, str13, strlen(str13), 515, 340);
    writeAtPos(1, str14, strlen(str14), 515, 360);
    writeAtPos(1, str15, strlen(str15), 515, 380);
    writeAtPos(1, str17, strlen(str17), 900, 600);
    writeAtPos(1, str18, strlen(str18), 900, 620);
    writeAtPos(1, str19, strlen(str19), 900, 640);
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            num[0] = grid[row][col];
            writeAtPos(1, num, 1, 600 + (30 * col), 480 + (30 * row));
        }
    }
    perdio = 0;
    termino = 1;

    //inicializando variables para Hangman
    char *str6 = "Ingrese una letra en MAYUSCULA.";
    char *str7 = "Si es correcta se impreme en pantalla.";
    char *str8 = "Si es incorrecta se descuenta una vida.";
    char *str9 = "Aclaracion: no se consideran palabras con letras repetidas.";
    char *str10 = "VIDAS RESTANTES:   5";
    char *str11 = "LETRA INGRESADA:    ";
    char *str12 = "__  __  __  __  __";
    writeAtPos(1, str6, strlen(str6), 0, 340);
    writeAtPos(1, str7, strlen(str7), 0, 360);
    writeAtPos(1, str8, strlen(str8), 0, 380);
    writeAtPos(1, str9, strlen(str9), 0, 400);
    writeAtPos(1, str10, strlen(str10), 50, 450);
    writeAtPos(1, str11, strlen(str11), 50, 470);
    writeAtPos(1, str12, strlen(str12), 200, 600);
    gano = 0;
    vidas = 5;
    count = 0;
    v[0] = 0;
    wlen = strlen(palabra);
    for (int i = 0; i < wlen; i++)
    {
        guessed[i] = 0;
    }

    return;
}

void hour()
{
    h = DateTime(2);
    intToStr(h, hours, 10);
    int lh = strlen(hours);
    if (lh == 1)
    {
        writeAtPos(1, "0", lh, 200, 150);
        writeAtPos(1, hours, lh, 210, 150);
    }
    else
    {
        writeAtPos(1, hours, lh, 200, 150);
    }
    writeAtPos(1, "::", 2, 220, 150);
    m = DateTime(1);
    intToStr(m, min, 10);
    int lm = strlen(min);
    if (lm == 1)
    {
        writeAtPos(1, "0", lm, 240, 150);
        writeAtPos(1, min, lm, 250, 150);
    }
    else
    {
        writeAtPos(1, min, lm, 240, 150);
    }
    writeAtPos(1, "::", 2, 260, 150);
    s = DateTime(0);
    intToStr(s, sec, 10);
    int ls = strlen(sec);
    if (ls == 1)
    {
        writeAtPos(1, "0", ls, 280, 150);
        writeAtPos(1, sec, ls, 290, 150);
    }
    else
    {
        writeAtPos(1, sec, ls, 280, 150);
    }
}

//LINK DE APOYO: https://psp.scenebeta.com/node/82751
//LINK DE APOYO: https://www.youtube.com/watch?v=5KynpVFYXOE
void stopWatch(int ticks, int * startTicks)
{

    //Incrementamos dec cada decima de segundo
    if (ticks > *startTicks + 1) //aca o en while general
    {
    intToStr(dec, d, 10);
    writeAtPos(1, d, 1, 780, 150);
    *startTicks = ticks;
    dec++;
    }
    //si las decimas llegan a 10, reseteamos y sumamos un segundo
    if (dec == 10)
    {
        intToStr(secs, ss, 10);
        int ls = strlen(ss);
        if (ls == 1)
        {
            writeAtPos(1, "0", ls, 740, 150);
            writeAtPos(1, ss, ls, 750, 150);
        }
        else
        {
            writeAtPos(1, ss, ls, 740, 150);
        }
        dec = 0;
        secs++;
    }
    writeAtPos(1, "::", 2, 760, 150);
    //si los segundos llegan a 60 reseteamos y sumamos un minuto
    if (secs == 60)
    {
        intToStr(mins, ms, 10);
        int lm = strlen(ms);
        if (lm == 1)
        {
            writeAtPos(1, "0", lm, 700, 150);
            writeAtPos(1, ms, lm, 710, 150);
        }
        else
        {
            writeAtPos(1, ms, lm, 700, 150);
        }
        secs = 0;
        mins++;
    }
    writeAtPos(1, "::", 2, 720, 150);
    if (mins == 60)
    {
        resetStopwatch(); //Vuelve todo a 0 y empieza la cuenta de nuevo
    }
}

//LINK DE AOPOYO: https://www.geeksforgeeks.org/sudoku-backtracking-7/
//LINK DE AOPOYO: https://gist.github.com/bopbi/c9f94b1b981b84bcb02b
void sudoku(char *numeros)
{
    int row = (numeros[0] - '0') - 1;
    int col = (numeros[1] - '0') - 1;
    rowChar[0] = numeros[0];
    colChar[0] = numeros[1];
    c[0] = numeros[2];

    writeAtPos(1, rowChar, 1, 980, 620);
    writeAtPos(1, colChar, 1, 980, 640);
    writeAtPos(1, c, 1, 980, 600);

    for (int x = 0; x < 9; x++)
    {
        for (int y = 0; y < 9; y++)
        {
            if (grid[x][y] == '_')
            {
                termino = 0;
            }
        }
    }

    if (!termino && grid[row][col] == '_')
    {
        int safe = isSafe(grid, c[0], row, col);
        if (safe == -1)
        {
            writeAtPos(1, "Invalid number in row   ", 24, 520, 450);
            return;
        }
        else if (safe == -2)
        {
            writeAtPos(1, "Invalid number in col   ", 24, 520, 450);
            return;
        }
        else if (safe == -3)
        {
            writeAtPos(1, "Invalid number in square", 24, 520, 450);
            return;
        }
        else if (safe == 1)
        {
            grid[row][col] = c[0];
            writeAtPos(1, c, 1, 600 + (30 * col), 480 + (30 * row));
            writeAtPos(1, "                        ", 24, 520, 450);
            termino = 1;
            return;
        }
    } else if (!termino && grid[row][col] != '_') {
        writeAtPos(1, "Position already filled ", 24, 520, 450);
    } else if (termino) {
        for (int x = 0; x < 9 && !perdio; x++)
        {
            for (int y = 0; y < 9 && !perdio; y++)
            {
                if (grid[x][y] != gridSolved[x][y])
                {
                    perdio = 1;
                }
            }
        }
        
        if (perdio)
        {
            writeAtPos(1, "PERDISTE    ", 12, 900, 600);
            writeAtPos(1, "            ", 12, 900, 620);
            writeAtPos(1, "            ", 12, 900, 640);
        }
        else if (!perdio)
        {
            writeAtPos(1, "GANASTE     ", 12, 900, 600);
            writeAtPos(1, "            ", 12, 900, 620);
            writeAtPos(1, "            ", 12, 900, 640);
        }
    }
}

int isSafe(char grid[9][9], char c, int row, int col)
{

    // Check if we find the same num in the similar row
    for (int x = 0; x < 9; x++)
    {
        if (grid[row][x] == c)
            return -1;
    }

    // Check if we find the same num in the similar column
    for (int x = 0; x < 9; x++)
    {
        if (grid[x][col] == c)
            return -2;
    }

    // Check if we find the same num in the particular 3*3 matrix
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (grid[i + startRow][j + startCol] == c)
                return -3;
        }
    }
    return 1;
}

void hangman(char * c)
{
    if (!gano && (vidas != 0))
    {
        int correcto = 0;
        writeAtPos(1, c, 1, 200, 470);
        for (int i = 0; (i < wlen) && !correcto; i++)
        {
            if (c[0] == palabra[i])
            {
                if (guessed[i] == 0)
                {
                    count++;
                    guessed[i] = 1;
                }
                writeAtPos(1, c, 1, 200 + (30 * i), 580);
                correcto = 1;
            }
        }
        if (count == wlen)
        {
            writeAtPos(1, "GANASTE", 7, 200, 500);
            gano = 1;
        }
        if (correcto == 0)
        {
            vidas--;
        }
        intToStr(vidas, v, 10);
        writeAtPos(1, v, strlen(v), 200, 450);
    }
    else if (vidas == 0)
    {
        writeAtPos(1, "PERDISTE", 8, 200, 500);
    }
    return;
}