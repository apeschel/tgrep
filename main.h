#ifndef MAIN_H
#define MAIN_H

// XXX: May be able to simplify and remove this struct.
struct time_stamp
{
    int day;
    int hour;
    int month;
    int second;
    int time;
};

FILE* open_file(char path[]);
void find_start_end_days(FILE *f);
int line_start_offset(int start, char str[]);
long ts_to_time(int d, int h, int m, int s);
void parse_line(struct time_stamp *ts, char str[]);
long midpoint(long x, long y);
void gen_time_stamp(struct time_stamp *ts, int day, int hour, int month, int second);

#endif // MAIN_H
