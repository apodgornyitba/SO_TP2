#ifndef RTC_H
#define RTC_H

//define para los valores de RTC
#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS   0x04
#define DAYS    0x07
#define MONTHS  0x08
#define YEARS   0x09
//constante que marca la diferencia horaria para poder obtener la hora de Argentina
#define TIME_ZONE -3

//define para switch-case
#define SEC   0
#define MIN   1
#define HOUR  2
#define DAY   3
#define MONTH 4
#define YEAR  5

int DateTime(unsigned int selector);
int RTC(unsigned int rtcSelector);

long int getSecs(void);
void hold(int secs);

#endif

