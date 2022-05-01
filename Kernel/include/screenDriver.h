#ifndef SCREEN_DRIVER_H
#define SCREEN_DRIVER_H

void initVideo();
int putChar(char c, unsigned int color);
int putCharAtPos(char c, unsigned int color, int x, int y);
void cleanScreen();
void newLine();
void backspace();
#endif