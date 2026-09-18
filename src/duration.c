#include "duration.h"

struct time
{
    unsigned char hours;   // horas de 0 a 99
    unsigned char minutes; // minutos de 0 a 59
    unsigned char seconds; // segundos de 0  59
};

Time create_time(unsigned char hours, unsigned char minutes, unsigned char seconds)
{
    Time new_time = (Time)malloc(sizeof(struct time));
    if (new_time != NULL)
    {
        new_time->hours = hours;
        new_time->minutes = minutes;
        new_time->seconds = seconds;
    }
    return new_time;
}

Time dup_time(Time ori_time)
{
    Time new_time = (Time)malloc(sizeof(struct time));
    if (new_time != NULL)
    {
        new_time->hours = ori_time->hours;
        new_time->minutes = ori_time->minutes;
        new_time->seconds = ori_time->seconds;
    }
    return new_time;
}

void free_time(Time t)
{
    // Libertamos a estrutura
    if (t != NULL)
    {
        free(t);
    }
}

unsigned int get_hour(Time t)
{
    return t->hours;
}

void print_time(Time t)
{
    if (t != NULL)
        printf(" Time: %02d:%02d:%02d", t->hours, t->minutes, t->seconds);
}

unsigned int calculate_duration(Time t)
{
    unsigned int calc = 0;
    if (t != NULL)
        calc = t->hours * 60 * 60 + t->minutes * 60 + t->seconds;
    return calc;
}
