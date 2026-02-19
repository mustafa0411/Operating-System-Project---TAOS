#include <mpx/io.h>
#include <user_commands.h>
#include <stdlib.h>
#include <sys_req.h>
#include <string.h>
#include <rtc.h>
#include <pcb.h>
#include <style.h>

void version(void) {
    printf("Current Version of TAOS: R5\n");
}

void get_date(void) {
    rtc_date date = rtc_get_date();

    // Format CCYY/MM/DD manually
    // Example: 2024 -> century=20, year=24
    int full_year = date.century * 100 + date.year;

    printf("Current Date: %d/%d/%d\n", date.month, date.day, full_year);
}

void set_date(char * date_string) {
    //array to store date values
    int date[3];
    char date_copy[16];
    strncpy(date_copy, date_string, sizeof(date_copy));
    date_copy[sizeof(date_copy)-1] = '\0';

    //split the user input into tokens based on the "/" delimiter and store them in the date array
    char *token = strtok(date_copy, "/");
    for (int i = 0; i < 3; i++) {
        if(!token) {
            printf("Please enter the date in MM/DD/YYYY format.\n");
            return;
        }
        date[i] = atoi(token); // convert string to integer
        token = strtok(NULL, "/");
    }

    //validate the date values
    if(date[0] > 12 || date[0] < 1){
        printf("Invalid month value. Please enter a value between 1 and 12.\n");
        return;
    }
    //validate the day value
    int day_max = 31;
    switch (date[0])
    {
        case 4:
        case 6:
        case 9:
        case 11:
            day_max = 30;
            break;
        case 2:
            if (date[2] % 4 == 0)
                day_max = 29;
            else
                day_max = 28;
            break;

    }
    if(date[1] > day_max || date[1] < 1){
        printf("Invalid date value.\n");
        return;
    }
    //validate the year value
    if(date[2] > 9999 || date[2] < 0){
        printf("Invalid year value. Please enter a value between 0 and 9999.\n");
        return;
    }
    int century = date[2] / 100;
    if (century == 0) century = 20;
    int year = date[2] % 100;
    rtc_set_date(date[0], date[1], century, year);

    printf("Date set to %d/%d/%d\n", date[0], date[1], century * 100 + year);
}

/**
 * @brief Get and display the current time in UTC
 * 
 * @return char* time_str; the current time in string format
 */
void get_time(void) {
    rtc_time t = rtc_get_time();

    // print the correct time string
    printf("Current Time: %02d:%02d:%02d\n", t.hour, t.minute, t.second);
}

void set_time(char* time_string) {
    //array to store time values
    int time[3];
    // copy the input string to safely tokenize
    char time_copy[16];
    strncpy(time_copy, time_string, sizeof(time_copy));
    time_copy[sizeof(time_copy)-1] = '\0'; // null termination

    //take the user input and split it into tokens based on the ":" delimiter
    char *token = strtok(time_copy, ":");
    for (int i = 0; i < 3; i++) {
        if (token == NULL) {
            // Not enough tokens? prompt for correct format
            printf("Please enter the time in HH:MM:SS format.\n");
            return;
        }
        // Convert token to integer and store in time array
        time[i] = atoi(token);
        token = strtok(NULL, ":"); // get the next token
    }
    // time value validations: hours, minutes and seconds.
    if(time[0] > 23 || time[0] < 0){
        printf("%sInvalid hour value. Please enter a value between 0 and 23.\n", RED);
        return;
    }
    if(time[1] > 59 || time[1] < 0){
        printf("%sInvalid minute value. Please enter a value between 0 and 59.\n", RED);
        return;
    }
    if(time[2] > 59 || time[2] < 0){
        printf("%sInvalid second value. Please enter a value between 0 and 59.\n", RED);
        return;
    }

    rtc_set_time(time[0], time[1], time[2]);

    // let user know the time was set.
    printf("Time set to %02d:%02d:%02d\n", time[0], time[1], time[2]);
}


int shutdown(void) {
    // small buffer to detect exit
    char buff[16] = {0};
    printf("Are you sure you want to exit?: (y)es or (n)o \n");
    // read and write the keystrokes to CLI
    sys_req(READ, COM1, buff, sizeof(buff));
    sys_req(WRITE, COM1, "\n", 1);
    if (strcmp(buff, "y") == 0 || strcmp(buff, "yes") == 0) {
        return 1;
    } else if (strcmp(buff, "y") == 0 || strcmp(buff, "no") == 0){
        printf("Shutdown cancelled.\n");
        return 0;
    } else {
        printf("%sInvalid response. Shutdown cancelled.\n", RED);
        return -1;
    }
}
