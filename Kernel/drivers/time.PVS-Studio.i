# 1 "/root/Kernel/drivers/time.c"
# 1 "/root/Kernel//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/root/Kernel/drivers/time.c"


# 1 "./include/time.h" 1



void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
# 4 "/root/Kernel/drivers/time.c" 2

static unsigned long ticks = 0;

void timer_handler() {
 ticks++;
}

int ticks_elapsed() {
 return ticks;
}

int seconds_elapsed() {
 return ticks / 18;
}
