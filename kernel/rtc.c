#include <rtc.h>
#include <mpx/io.h>
#include <stdlib.h>

void rtc_set_date(int month, int day, int century, int year)
{
    outb(0x70, 0x09); 
    outb(0x71, i_to_bcd(year));    // year
    outb(0x70, 0x32); 
    outb(0x71, i_to_bcd(century)); // century
    outb(0x70, 0x08); 
    outb(0x71, i_to_bcd(month)); // month
    outb(0x70, 0x07); 
    outb(0x71, i_to_bcd(day)); // day
}

void rtc_set_time(int hour, int minute, int second)
{
    // Set RTC registers
    outb(0x70, 0x04); 
    outb(0x71, i_to_bcd(hour)); // hour
    outb(0x70, 0x02); 
    outb(0x71, i_to_bcd(minute)); // minute
    outb(0x70, 0x00); 
    outb(0x71, i_to_bcd(second)); // second
}

rtc_date rtc_get_date(void)
{
    rtc_date d;

    outb(0x70, 0x07); 
    d.day = bcd_to_i(inb(0x71));
    outb(0x70, 0x08); 
    d.month = bcd_to_i(inb(0x71));
    outb(0x70, 0x09); 
    d.year = bcd_to_i(inb(0x71));

    // Try to read century register (0x32)
    outb(0x70, 0x32);
    d.century = bcd_to_i(inb(0x71));
    if (d.century == 0) d.century = 20;

    return d;
}

rtc_time rtc_get_time(void)
{
    rtc_time t;
    // Use the 0x70 register for real time clock.
    outb(0x70, 0x04);
    t.hour = bcd_to_i(inb(0x71));
    outb(0x70, 0x02);
    t.minute = bcd_to_i(inb(0x71));
    outb(0x70, 0x00);
    t.second = bcd_to_i(inb(0x71));
    return t;
}
