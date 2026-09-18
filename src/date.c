#include "date.h"

struct date
{
    unsigned char day;   // Dias de 1 a 31
    unsigned char month; // Meses de 1 a 12
    unsigned short year;
};

Date create_date(unsigned char day, unsigned char month, unsigned short year)
{
    Date new_date = (Date)malloc(sizeof(struct date));
    if (new_date != NULL)
    {
        new_date->day = day;
        new_date->month = month;
        new_date->year = year;
    }
    return new_date;
}

Date dup_date(Date ori_date)
{
    Date cpy_date = (Date)malloc(sizeof(struct date));
    if (cpy_date != NULL)
    {
        cpy_date->day = ori_date->day;
        cpy_date->month = ori_date->month;
        cpy_date->year = ori_date->year;
    }

    return cpy_date;
}

void free_date(Date data)
{
    // Libertamos a estrutura
    if (data != NULL)
    {
        free(data);
    }
}

void print_date(Date d)
{
    if (d != NULL)
        printf(" Date: %02d:%02d:%02d", d->day, d->month, d->year);
}

unsigned char get_day(Date date)
{
    return date->day;
}

unsigned char get_month(Date date)
{
    return date->month;
}

unsigned short get_year(Date date)
{
    return date->year;
}