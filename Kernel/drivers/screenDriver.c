// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <graphicRenderer.h>
#include <screenDriver.h>
#include <font.h>

#define ENTER_KEY 13

static int cursorPosX = 0;
static int cursorPosY = 0;
static int maxX, maxY;

#define FIG_WIDTH  12
#define FIG_HEIGHT 16

void initVideo(){
    initRenderer(0x0000000000005C00);
    //Determino los limites redondeados del posicionamiento de caracteres (para saber donde volver despues de un \b)
    maxX = getWidth();
    maxY = getHeight();
    cleanScreen();
}


int putChar(char c,unsigned int color){
    if (cursorPosX+ABS_WIDTH>maxX) //Me paso de ancho con esta insercion? Si es asi salto
        newLine();

    if (c=='\n'||c==ENTER_KEY){ //newline
        newLine();
        return 1;
    }
    if (c=='\b') //backspace
    {
        backspace();
        return 1;
    }
    int res = renderChar(c,cursorPosX,cursorPosY,color);
    cursorPosX+=ABS_WIDTH;
    return res;
}

int putCharAtPos(char c, unsigned int color, int x, int y){
    if (x + ABS_WIDTH > maxX || y+ABS_HEIGHT>maxY) //NO hay newline
        return -1;

    if (c == '\b') //el backspace funciona como un borrado posicional
    {
        renderArea(x, y, x + ABS_WIDTH, y + ABS_HEIGHT, 0x000000);
        return 1;
    }
    renderArea(x, y, x + ABS_WIDTH, y + ABS_HEIGHT, 0x000000);
    return renderChar(c, x, y, color);
}

void newLine(){
    int scrollDist = ABS_HEIGHT;
    cursorPosX=0;
    cursorPosY += scrollDist;
    if (cursorPosY>=maxY){
        scrollUp(scrollDist);
        cursorPosY = maxY - scrollDist;
    }

}

void cleanScreen(){
    clearAll();
    cursorPosX = 0;
    cursorPosY = 0;
}

void backspace(){
    if (cursorPosX==0){
        if (cursorPosY==0)
            return;
        cursorPosY-=ABS_HEIGHT;
        cursorPosX = maxX;
 
    }
    cursorPosX -= ABS_WIDTH;
    renderArea(cursorPosX, cursorPosY, 
        cursorPosX + ABS_WIDTH, cursorPosY + ABS_HEIGHT, 0x000000);
}
