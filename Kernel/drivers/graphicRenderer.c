#include <stdint.h>
#include <graphicRenderer.h>
#include <font.h>

struct vbe_mode_info_structure *screenData;

static int width = 0;
static int height = 0;

#define FIG_HEIGHT 16
#define FIG_WIDTH 12

void initRenderer(long int infoLoc){
	screenData = (void *)infoLoc;
	width = screenData->width;
	height = screenData->height;
}

int getHeight(){
	return height;
}

int getWidth()
{
	return width;
}

//https://wiki.osdev.org/Drawing_In_Protected_Mode
int renderPixel(unsigned int x, unsigned int y, unsigned int color)
{
	if (x < 0 || y < 0 || x > width || y > height)
		return -1;

	char *pos = (char *)((uint64_t)screenData->framebuffer + (x + y * width) * 3);
	*pos = (color & 0x0000FF);
	*(pos+1) = (color & 0x00FF00) >> 8;
	*(pos+2) = (color & 0xFF0000) >> 16;
	return 0;

}

//NO se recomienda su uso para areas grandes, es perfectamente correcto para uso en caracteres/figuras
int renderArea(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color){
	if (x2<x1 || y2<y1)
		return -1;
	if (x1 < 0 || y1 < 0 || x2 > width || y2 > height)
		return -2;
	for (int i=x1;i<=x2;i++){
		for (int j=y1;j<=y2;j++)
			renderPixel(i,j,color);
	}
	return 0;
}

	// segun https://jared.geek.nz/2014/jan/custom-fonts-for-microcontrollers
int renderChar(unsigned char c, unsigned int x, unsigned int y, unsigned int color)
{
	if (x < 0 || y < 0 || x + (ABS_WIDTH) > width || y + (ABS_HEIGHT) > height)
		return -1;

	// 'font' es un arreglo multidimensional de [96][FONT_WIDTH]
	// que es un arreglo 1D de tamano 96*FONT_WIDTH.
	const uint8_t *chr = charBitmap(c);

	// Draw pixels
    for (int j = 0; j < FONT_HEIGHT; j++)
    {
        for (int i = 0; i < FONT_WIDTH; i++){
			if (chr[j] & (1 << i)) //En esta posicion hay un bit encendido
                renderArea(x + ABS_WIDTH - (i + 1) * FONT_SCALING, y + j * FONT_SCALING, 
                ABS_WIDTH + x - i * FONT_SCALING, y + (j + 1) * FONT_SCALING, color);
        }
    }
    return 0;
}

void clearAll(){
	//Podria usar
	//renderArea(0,0,width,height,BG_COLOR);
	// Pero esta forma es MUCHO mas rapida y eficiente (reduzco la cantidad de llamadas)
	// Se pudo reducir cuanto tarda el clearAll de 0.5s a practicamente milisegundos.
	char *pos = (char *)((uint64_t)screenData->framebuffer);
	for (int i=0;i<height*width*3;i++)
		pos[i]=0;

}

int scrollUp(int pixels){
	if (pixels>height){
		clearAll();
		return -1;
	}
	char *pos = (char *)((uint64_t)screenData->framebuffer);
	for (int i = 0; i < width; i++){
		for (int j=0;j<height-pixels;j++){
			for (int k=0;k<3;k++){ // per color
				*pos = *(pos + (pixels * width) * 3);
				pos++;
			}
		}
	}
	renderArea(0,height-pixels,width,height,0x000000);
	return 0;
}

void separateMainScreen() {
      for (int y = 0; y < height; y++) {
            for (int x = 0; x < ABS_WIDTH / 2; x++) {
                  renderPixel(width / 2 - ABS_WIDTH + x, y, 0xFFFFFF);
            }
      }
	  for (int i = 0; i < 4; i++) {
            for (int x = 0; x < width; x++) {
                  renderPixel(x, (height/2.5 + i), 0xFFFFFF);
            }	
      }
}