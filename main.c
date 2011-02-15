#include <stdio.h>

#include "main.h"
#define BUF_SIZE 4096

int starting_day;
int ending_day;

int main()
{
    struct time_stamp ts;
    FILE* f;

    f = open_file("./haproxy.log");

    find_start_end_days(f);
    printf("STARTING DAY: %d\n", starting_day);
    printf("ENDING DAY:   %d\n", ending_day);

    fclose(f);
    return 0;
}

FILE* open_file(char path[])
{
    FILE *f;
    f = fopen(path, "rb");
    setvbuf(f, NULL, _IOFBF, BUF_SIZE);

    return f;
}

void find_start_end_days(FILE *f)
{
    struct time_stamp ts;
    long pos;
    char str[BUF_SIZE];

    pos = ftell(f);

    // Starting day
    fseek(f, 0, SEEK_SET);
    fread(str, sizeof(char), BUF_SIZE, f);
    parse_line(&ts, str);
    starting_day = ts.day;

    // Ending day
    // XXX: Not working.
    fseek(f, -BUF_SIZE, SEEK_END);
    int offset = fread(str, sizeof(char), BUF_SIZE, f);
    printf("str: %s\n", str);
    offset -= 2;
    printf("offset: %d\n", offset);
    line_start_offset(offset, str);
    parse_line(&ts, str + offset);
    ending_day = ts.day;
}

int line_start_offset(int start, char str[])
{
    int i = start;

    for (i = start; i > 0; i--)
    {
        if (str[i] == '\n')
        {
            i++;
            break;
        }
    }

    return i;
}

long ts_to_time(int d, int h, int m, int s)
{
    long time = s + (60 * m) + (60 * 60 * h);

    if (d == ending_day)
    {
        time += (24 * 60 * 60);
    }

    return time;
}

void gen_time_stamp(struct time_stamp *ts, int d, int h, int m, int s)
{
    ts->day = d;
    ts->hour = h;
    ts->month = m;
    ts->second = s;
    ts->time = ts_to_time(d, h, m, s);
}

void parse_line(struct time_stamp *ts, char str[])
{
    int d, h, m, s;

    printf("%s\n", str);
    sscanf(str, "%*s %d %d:%d:%d", &d, &h, &m, &s);
    gen_time_stamp(ts, d, h, m, s);
}

long midpoint(long x, long y)
{
    long mid = (x >> 1) + (y >> 1);

    // Offset takes into account the situation where both x and y are odd.
    long offset = (x & 1) & (y & 1);

    return mid + offset;
}

