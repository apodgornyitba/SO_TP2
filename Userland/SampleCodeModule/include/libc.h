// WRAPPEO DE FUNCIONES DE ASM Y ALGUNAS PRECISIONES PARA USO DE USUARIO

#ifndef LIB_C_H
#define LIB_C_H


#define BUFFER_SIZE 512
#define SEC 0
#define MIN 1
#define KEY_SEM_ID 8


//IN: String, Parametros variables (formatos)
//OUT: Longitud del string
int printf(char *str, ...) ;
//IN: Char a imprimir
//OUT: 0 en caso de error, 1 caso contrario
int putChar(char c);
//IN: String con formato, parametros variables (formatos)
//OUT: Longitud del string
int scanf(const char *format, ...);
//IN:void
//OUT: Char leido de teclado
int getChar();
//IN: void
//OUT: Size del buffer leido
int readLn();
//IN: String
//OUT: Longitud del string
int strlen(char *s);
//IN: Buffer destino, String origen
//OUT: Referencia al buffer destino con el source copiado
char *strcpy(char *destination, const char *source);
//IN: Buffer destino, String origen
//OUT: Copia en destino hasta el primer espacio en el string source
char *strcpyTilSpace(char *destination, const char *source);
//IN: String que representa un numero, longitud del string
//OUT: String convertido en integer
int strToInt(char *str);
//IN: buffer
//OUT: integer
char *intToStr(int num, char *str, int base);
void clearBuff();
//IN: Integer
//OUT: Valor absoluto del integer
int abs(int n);
//IN: String, String
//OUT: 0 si son iguales, >0 si String1 > String2, <0 caso contrario
int strcmp(const char *str1, const char *str2);
//IN: Numero a convertir, buffer a guardar el numero, cuantos bytes ocupa
//OUT: Referencia al buffer con el numero guardado
char *intToHexa(long long num, char *str, int bytes);
//IN: hexa  a convertir
//OUT: numero
int hexaToInt(char *num);
void sleep(unsigned int seconds);
int getSecondsElapsed();

#endif
