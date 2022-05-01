#include <rtc.h>

static unsigned int cast(unsigned char time)
{
    return (time >> 4) * 10 + (time & 0x0F);
}

int DateTime(unsigned int selector)
{
    switch (selector)
    {
    case SEC:
        return cast(RTC(SECONDS));
    case MIN:
        return cast(RTC(MINUTES));
    case HOUR:
        return cast(RTC(HOURS)) + TIME_ZONE;
    case DAY:
        return cast(RTC(DAYS));
    case MONTH:
        return cast(RTC(MONTHS));
    case YEAR:
        return cast(RTC(YEARS));
    default:
        return -1;
    }
}

long int getSecs(){
    return DateTime(0)+DateTime(1)*60+ DateTime(2)*3600+DateTime(3);
}

//Espera unos segs determinados
void hold(int secs){
    long int initTime,currTime;
    long int timeDif=0;
    int passedDays =0;
    initTime = currTime = getSecs();
    while(1){
        currTime = getSecs();
        timeDif = currTime - initTime + (passedDays * 86400);
        if (timeDif<0){ //Paso un dia
            passedDays++;
            timeDif+=86400;
        }

        if (timeDif>=secs)
            return;

    }
}
