typedef struct rtc_date {
    int month;
    int day;
    int century;
    int year;
} rtc_date;

typedef struct rtc_time {
    int hour;
    int minute;
    int second;
} rtc_time;

void rtc_set_date(int month, int day, int century, int year);

void rtc_set_time(int hour, int minute, int second);

rtc_date rtc_get_date(void);

rtc_time rtc_get_time(void);

