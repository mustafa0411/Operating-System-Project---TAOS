#include <dispatching.h>
#include <sys_req.h>
#include <processes.h>
#include <sys_call.h>
#include <string.h>
#include <pcb.h>
#include <queue.h>
#include <commhand.h>
#include <rtc.h>
#include <string.h>

typedef void (*proc_func_t)(void);

void yield(void) {
    sys_req(IDLE);
}

void loadr3(void) {
    proc_func_t procs[] = {proc1, proc2, proc3, proc4, proc5};
    const char* names[] = {"proc1", "proc2", "proc3", "proc4", "proc5"};
    int priorities[] = {1, 2, 3, 4, 5};

    for (int i = 0; i < 5; ++i) {
        // CHECK IF PROCESS ALREADY EXISTS
        if (pcb_find(names[i])) {
            printf("Process '%s' already exists, skipping\n", names[i]);
            continue;
        }
        
        // Use pcb_setup instead of manual allocation and initialization
        pcb* p = pcb_setup(names[i], 1, priorities[i]);
        if (!p) {
            printf("Failed to create PCB for %s\n", names[i]);
            continue;
        }
        
        // Set up context at top of stack
        context* ctx = (context*)((char*)p->stack + p->stack_size - sizeof(context));
        memset(ctx, 0, sizeof(context));
        
        ctx->cs = 0x08;
        ctx->ds = ctx->es = ctx->fs = ctx->gs = ctx->ss = 0x10;
        ctx->ebp = (unsigned int)p->stack; // Bottom of stack
        ctx->esp = (unsigned int)p->stack + p->stack_size - sizeof(context) - 16;// Top of stack (where context is)
        ctx->eip = (unsigned int)procs[i];
        ctx->eflags = 0x0202;

        // UPDATE PCB STACK POINTER
        p->stack = (void*)ctx;

        // Insert into ready queue
        pcb_insert(p);
        
        printf("Loaded %s successfully\n", names[i]);
    }
}

void loadr3_suspended(void) {
    proc_func_t procs[] = {proc1, proc2, proc3, proc4, proc5};
    const char* names[] = {"proc1", "proc2", "proc3", "proc4", "proc5"};
    int priorities[] = {1, 2, 3, 4, 5};

    for (int i = 0; i < 5; ++i) {
        // CHECK IF PROCESS ALREADY EXISTS
        if (pcb_find(names[i])) {
            printf("Process '%s' already exists, skipping\n", names[i]);
            continue;
        }

        // Use pcb_setup instead of manual allocation and initialization
        pcb* p = pcb_setup(names[i], 1, priorities[i]); // 1 = User process
        if (!p) {
            printf("Failed to create PCB for %s\n", names[i]);
            continue;
        }
        
        // Set suspended flag after creation
        p->suspended = 1; // SUSPENDED
        
        // Set up context at top of stack
        context* ctx = (context*)((char*)p->stack + p->stack_size - sizeof(context));
        memset(ctx, 0, sizeof(context));
        
        ctx->cs = 0x08;
        ctx->ds = ctx->es = ctx->fs = ctx->gs = ctx->ss = 0x10;
        ctx->ebp = (unsigned int)p->stack;
        ctx->esp = (unsigned int)p->stack + p->stack_size - sizeof(context) - 16;
        ctx->eip = (unsigned int)procs[i];
        ctx->eflags = 0x0202;

        p->stack = (void*)ctx;

        // Insert into suspended queue
        pcb_insert(p);
        
        printf("Loaded %s (suspended) successfully\n", names[i]);
    }
}

int load_process(const char* name) {
    if (!name) {
        printf("Invalid process name\n");
        return 0;
    }

    // CHECK IF PROCESS ALREADY EXISTS
    if (pcb_find(name)) {
        printf("Process '%s' already exists\n", name);
        return 0;
    }

    proc_func_t procs[] = {proc1, proc2, proc3, proc4, proc5};
    const char* names[] = {"proc1", "proc2", "proc3", "proc4", "proc5"};
    int priorities[] = {1, 2, 3, 4, 5};

    int idx = -1;
    for (int i = 0; i < 5; ++i) {
        if (strcmp(names[i], name) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("Unknown process name. Valid: proc1-proc5\n");
        return 0;
    }

   // Use pcb_setup instead of manual allocation and initialization
    pcb* p = pcb_setup(names[idx], 1, priorities[idx]); // 1 = User process
    if (!p) {
        printf("Failed to create PCB\n");
        return 0;
    }

    // Set up context at top of stack
    context* ctx = (context*)((char*)p->stack + p->stack_size - sizeof(context));
    memset(ctx, 0, sizeof(context));
    
    ctx->cs = 0x08;
    ctx->ds = ctx->es = ctx->fs = ctx->gs = ctx->ss = 0x10;
    ctx->ebp = (unsigned int)p->stack;
    ctx->esp = (unsigned int)ctx;
    ctx->eip = (unsigned int)procs[idx];
    ctx->eflags = 0x0202;

    p->stack = (void*)ctx;

    pcb_insert(p);
    return 1;
}

void resume_all(void) {
    int resumed_count = 0;

    while (!is_queue_empty(QID_SUSPENDED)) {
        pcb* p = get_suspended_next();
        if (!p) break;

        p->suspended = 0;

        if (p->state == 1) {  // Ready
            add_to_ready_queue(p);
        } else if (p->state == 2) {  // Blocked
            add_to_blocked_queue(p);
        }
        
        resumed_count++;
    }

    printf("Resumed %d process(es)\n", resumed_count);
}

void create_system_processes(void) {
    // Create Command Handler process (highest process)
    pcb* commhand_pcb = pcb_setup("commhand", 0, 0); // 0 = System process, priority 0
    if (!commhand_pcb) {
        printf("Failed to create PCB for command handler\n");
        return;
    }
    printf("Successfully created commhand PCB\n");

    // Set up the context at top of stack
    context* ctx = (context*)((char*)commhand_pcb->stack + commhand_pcb->stack_size - sizeof(context));
    memset(ctx, 0, sizeof(context));

    // Setting the pcb context structure for commhand
    ctx->cs = 0x08;
    ctx->ds = ctx->es = ctx->fs = ctx->gs = ctx->ss = 0x10;
    ctx->ebp = (unsigned int)commhand_pcb->stack;
    ctx->esp = (unsigned int)commhand_pcb->stack + commhand_pcb->stack_size - sizeof(context) - 16;
    ctx->eip = (unsigned int)commhand_process;
    ctx->eflags = 0x0202;
    
    commhand_pcb->stack = (void*)ctx;
    pcb_insert(commhand_pcb);
    printf("Inserted commhand into queue\n");


    // Create system idle process
    pcb* idle_pcb = pcb_setup("sys_idle", 0, 9); // 0 = System process, priority 9
    if (!idle_pcb) {
        printf("Failed to create PCB for idle process\n");
        return;
    }
    printf("Successfully created sys_idle PCB\n");


    // Set up the context at top of stack
    // This is for setting up the sys_idle_process
    ctx = (context*)((char*)idle_pcb->stack + idle_pcb->stack_size - sizeof(context));
    memset(ctx, 0, sizeof(context));

    // Same context structure as previous pcb but this one is for idle_pcb
    ctx->cs = 0x08;
    ctx->ds = ctx->es = ctx->fs = ctx->gs = ctx->ss = 0x10;
    ctx->ebp = (unsigned int)idle_pcb->stack;
    ctx->esp = (unsigned int)idle_pcb->stack + idle_pcb->stack_size - sizeof(context) - 16;
    ctx->eip = (unsigned int)sys_idle_process;
    ctx->eflags = 0x0202;

    idle_pcb->stack = (void*)ctx;
    pcb_insert(idle_pcb);
    printf("Inserted sys_idle into queue\n");


    printf("Created system processes: commhand (priority 0) and sys_idle (priority 9)\n");
}

// Counter for the amount of available alarm processes
static int alarm_counter = 0;

// temporary alarm data struct variable
static process_alarm_data temp_alarm_data;

void alarm_process(void) {
    // Copy the temporary alarm data to local variables immediately
    char target_time[9];
    char target_date[11];
    char message[200];
    
    // copy the target time, date and the message that the user inputs.
    strncpy(target_time, temp_alarm_data.target_time, 8);
    target_time[8] = '\0';
    
    strncpy(target_date, temp_alarm_data.target_date, 10);
    target_date[10] = '\0';
    
    strncpy(message, temp_alarm_data.message, 199);
    message[199] = '\0';
    
    while (1) {
        if (shutting_down)
        {
            sys_req(EXIT);
            return;
        }

        // Get current time and date
        rtc_time current_t = rtc_get_time();
        rtc_date current_d = rtc_get_date();
        
        // Format current date(account for centuries)
        char current_date[11];
        int current_year = current_d.century * 100 + current_d.year;
        current_date[0] = '0' + (current_year / 1000);
        current_date[1] = '0' + ((current_year / 100) % 10);
        current_date[2] = '0' + ((current_year / 10) % 10);
        current_date[3] = '0' + (current_year % 10);
        current_date[4] = '/';
        current_date[5] = '0' + (current_d.month / 10);
        current_date[6] = '0' + (current_d.month % 10);
        current_date[7] = '/';
        current_date[8] = '0' + (current_d.day / 10);
        current_date[9] = '0' + (current_d.day % 10);
        current_date[10] = '\0';
        
        // Format the current time
        char current_time[9];
        current_time[0] = '0' + (current_t.hour / 10);
        current_time[1] = '0' + (current_t.hour % 10);
        current_time[2] = ':';
        current_time[3] = '0' + (current_t.minute / 10);
        current_time[4] = '0' + (current_t.minute % 10);
        current_time[5] = ':';
        current_time[6] = '0' + (current_t.second / 10);
        current_time[7] = '0' + (current_t.second % 10);
        current_time[8] = '\0';
        
        // Compare date first
        int date_cmp = strcmp(current_date, target_date);
        if (date_cmp > 0 || (date_cmp == 0 && strcmp(current_time, target_time) >= 0)) {
            // Time has arrived - display message and exit
            char alarm_prefix[] = "ALARM: ";
            char newline[] = "\r\n";
            
            sys_req(WRITE, COM1, alarm_prefix, strlen(alarm_prefix));
            sys_req(WRITE, COM1, message, strlen(message));
            sys_req(WRITE, COM1, newline, strlen(newline));
            
            sys_req(EXIT);
            return;
        }
        
        // Not time yet - yield and wait
        sys_req(IDLE);
    }
}

int create_alarm_process(const char* time, const char* message) {    
    if (!time || !message) {
        printf("Invalid alarm parameters\n");
        return 0;
    }
    
    // Validate time format (HH:MM:SS)
    if (strlen(time) != 8 || time[2] != ':' || time[5] != ':') {
        printf("Invalid time format. Use HH:MM:SS\n");
        return 0;
    }
    
    // Validate time values manually
    int hours = (time[0] - '0') * 10 + (time[1] - '0');
    int minutes = (time[3] - '0') * 10 + (time[4] - '0');
    int seconds = (time[6] - '0') * 10 + (time[7] - '0');
    
    if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59) {
        printf("Invalid time values. Hours: 0-23, Minutes/Seconds: 0-59\n");
        return 0;
    }
    
    // Get current time to check if we need to set for next day
    rtc_time current_t = rtc_get_time();
    rtc_date current_d = rtc_get_date();
    
    // Create target time struct
    rtc_time target_time;
    rtc_date target_date;
    
    target_time.hour = hours;
    target_time.minute = minutes;
    target_time.second = seconds;
    
    // Initialize target date with current date
    target_date.month = current_d.month;
    target_date.day = current_d.day;
    target_date.century = current_d.century;
    target_date.year = current_d.year;
    
    // Check if target time has already passed today
    int time_passed = 0;
    if (target_time.hour < current_t.hour) {
        time_passed = 1;
    } else if (target_time.hour == current_t.hour) {
        if (target_time.minute < current_t.minute) {
            time_passed = 1;
        } else if (target_time.minute == current_t.minute) {
            if (target_time.second <= current_t.second) {
                time_passed = 1;
            }
        }
    }
    
    // If time has passed, set for next day
    if (time_passed) {
        target_date.day++;
        
        // Handle month rollover
        int days_in_month = 31;
        switch (target_date.month) {
            case 4: case 6: case 9: case 11:
                days_in_month = 30;
                break;
            case 2: {
                int full_year = target_date.century * 100 + target_date.year;
                if (full_year % 4 == 0 && (full_year % 100 != 0 || full_year % 400 == 0)) {
                    days_in_month = 29;
                } else {
                    days_in_month = 28;
                }
                break;
            }
        }
        
        if (target_date.day > days_in_month) {
            target_date.day = 1;
            target_date.month++;
            
            if (target_date.month > 12) {
                target_date.month = 1;
                target_date.year++;
                
                if (target_date.year > 99) {
                    target_date.year = 0;
                    target_date.century++;
                }
            }
        }
        
        printf("Time has passed today, setting alarm for tomorrow\n");
    }
    
    // Store alarm data in temporary storage
    strncpy(temp_alarm_data.target_time, time, 8);
    temp_alarm_data.target_time[8] = '\0';
    
    int full_year = target_date.century * 100 + target_date.year;
    temp_alarm_data.target_date[0] = '0' + (full_year / 1000);
    temp_alarm_data.target_date[1] = '0' + ((full_year / 100) % 10);
    temp_alarm_data.target_date[2] = '0' + ((full_year / 10) % 10);
    temp_alarm_data.target_date[3] = '0' + (full_year % 10);
    temp_alarm_data.target_date[4] = '/';
    temp_alarm_data.target_date[5] = '0' + (target_date.month / 10);
    temp_alarm_data.target_date[6] = '0' + (target_date.month % 10);
    temp_alarm_data.target_date[7] = '/';
    temp_alarm_data.target_date[8] = '0' + (target_date.day / 10);
    temp_alarm_data.target_date[9] = '0' + (target_date.day % 10);
    temp_alarm_data.target_date[10] = '\0';
    
    strncpy(temp_alarm_data.message, message, 199);
    temp_alarm_data.message[199] = '\0';
    
    // Create unique alarm process name
    char alarm_name[32];
    char base[] = "alarm_";
    int i;
    for (i = 0; base[i] != '\0'; i++) {
        alarm_name[i] = base[i];
    }
    
    ++alarm_counter;
    int temp = alarm_counter;
    char num_str[10];
    int digits = 0;
    int temp_copy = temp;
    
    if (temp == 0) {
        digits = 1;
    } else {
        while (temp_copy > 0) {
            digits++;
            temp_copy /= 10;
        }
    }
    
    num_str[digits] = '\0';
    for (int j = digits - 1; j >= 0; j--) {
        num_str[j] = '0' + (temp % 10);
        temp /= 10;
    }
    
    for (int j = 0; num_str[j] != '\0'; j++) {
        alarm_name[i + j] = num_str[j];
    }
    alarm_name[i + strlen(num_str)] = '\0';
    
    // Create the process
    pcb* alarm_pcb = pcb_setup(alarm_name, 1, 3);
    if (!alarm_pcb) {
        printf("Failed to create PCB for alarm process\n");
        return 0;
    }
    
    // Set up context
    context* ctx = (context*)((char*)alarm_pcb->stack + alarm_pcb->stack_size - sizeof(context));
    memset(ctx, 0, sizeof(context));
    
    ctx->cs = 0x08;
    ctx->ds = ctx->es = ctx->fs = ctx->gs = ctx->ss = 0x10;
    ctx->ebp = (unsigned int)alarm_pcb->stack;
    ctx->esp = (unsigned int)alarm_pcb->stack + alarm_pcb->stack_size - sizeof(context) - 16;
    ctx->eip = (unsigned int)alarm_process;
    ctx->eflags = 0x0202;
    
    alarm_pcb->stack = (void*)ctx;
    pcb_insert(alarm_pcb);
    
    printf("Created alarm process for time %s\n", time);
    return 1;
}
