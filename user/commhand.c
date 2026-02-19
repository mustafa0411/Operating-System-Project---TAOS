#include <commhand.h>
#include <sys_req.h>
#include <string.h>
#include <stdlib.h> 
#include <user_commands.h>
#include <help_commands.h>
#include <pcb_user_commands.h>
#include <pcb.h>
#include <style.h>
#include <dispatching.h>
#include <queue.h>
#include <shaw.h>
#include <mcb_user_commands.h>


int shutting_down = 0;

void commhand_process(void) {
    printf("DEBUG: commhand_process() started - running as a process!\n");
    
    // This is the process entry point for the command handler
    commhand(); // Call the original command handler
}

void commhand(void)
{
    printf("\033[38;5;196m"); // set color to orange
    // printf("_________ _______  _______  _______ \n");
    // printf("\\__   __/(  ___  )(  ___  )(  ____ \\\n");
    printf(" _________________       ____           _____             ______  \n");
    printf("/                 \\ ____|\\   \\     ____|\\    \\        ___|\\     \\ \n");
    printf("\\______     ______//    /\\    \\   /     /\\    \\      |    |\\     \\ \n");
    printf("\033[38;5;202m");
    // printf("   ) (   | (   ) || (   ) || (    \\/\n");
    // printf("   | |   | (___) || |   | || (_____ \n");
    printf("   \\( /    /  )/  |    |  |    | /     /  \\    \\     |    |/____/| \n");
    printf("    ' |   |   '   |    |__|    ||     |    |    | ___|    \\|   | | \n");
    printf("      |   |       |    .--.    ||     |    |    ||    \\    \\___|/ \n");
    printf("\033[38;5;208m");
    // printf("   | |   |  ___  || |   | |(_____  )\n");
    // printf("   | |   | (   ) || |   | |      ) |\n");
    printf("     /   //       |    |  |    ||\\     \\  /    /||    |\\     \\    \n");
    printf("    /___//        |____|  |____|| \\_____\\/____/ ||\\ ___\\|_____|   \n");
    printf("\033[38;5;214m");
    // printf("   | |   | )   ( || (___) |/\\____) |\n");
    // printf("   )_(   |/     \\|(_______)\\_______)\n");
    printf("   |`   |         |    |  |    | \\ |    ||    | /| |    |     |   \n");
    printf("   |____|         |____|  |____|  \\|____||____|/  \\|____|_____|   \n");
    printf("     \\(             \\(      )/       \\(    )/        \\(    )/     \n");
    printf("\n");
    printf(RESET);
    printf("----------------Totally Awesome Operating System----------------\n");

    printf("\n");
    printf("                         Welcome to TAOS!\n");
    printf("\n");
    printf("Type 'help' for a list of commands\n");
    printf("\n");

    for(;;) {
        sys_req(IDLE);
        printf("%sTAos%s>%s ", YELLOW, GREEN, RESET);

        char buff[100] = { 0 };
        sys_req(READ, COM1, buff, sizeof(buff));

        if (strlen(buff) == 0)
            continue;

        // process the command
        if (strcmp(buff, "exit") == 0) {
            if (shutdown() == 1) {
                shutting_down = 1;
                sys_req(EXIT);
                return;
            }

            continue;
        }

        execute_command(buff);
    }
}        

void execute_command(char* command) {

    // Get the command name from the command string
    char* command_name = strtok(command, " ");


    if (strcmp(command_name, "version") == 0) {
        char* arg = strtok(NULL, " ");
        if (arg != NULL) {
            printf("%sThe command 'version' does not take any additional arguments.\n", RED);
            return;
        }
        version();
        return;
    }

    else if (strcmp(command_name, "help") == 0) {
        char* arg1 = strtok(NULL, " ");

        if (arg1 == NULL) {
            help();
            return;
        }

        char* arg2 = strtok(NULL, " ");

        if (arg2 != NULL) {
            printf("%sCommand 'help' accepts at most one argument.\n", RED);
            return;
        }
        
        help_with(arg1);
    }
    else if (strcmp(command_name, "get_time") == 0){
        char* arg = strtok(NULL, " ");
        if (arg != NULL) {
            printf("%sThe command 'get_time' does not take any additional arguments.\n", RED);
            return;
        }
        get_time();
    }
    else if (strcmp(command_name, "get_date") == 0){
        char* arg = strtok(NULL, " ");
        if (arg != NULL) {
            printf("%sThe command 'get_date' does not take any additional arguments.\n", RED);
            return;
        }
        get_date();
    }
    else if (strcmp(command_name, "set_time") == 0) {
        char* arg1 = strtok(NULL, " ");
        // Too few arguments
        if (arg1 == NULL) {
            printf("%sUsage: set_time <time: hh:mm:ss>\n", RED);
            return;
        }
        
        char* arg2 = strtok(NULL, " ");
        // Too many arguments
        if (arg2 != NULL) {
            printf("%sToo many arguments. Usage: set_time <time: hh:mm:ss>\n", RED);
            return;
        }

        //pass the command arguments to the set_time function
        set_time(arg1);
    }
    else if (strcmp(command_name, "set_date") == 0){
        char* arg1 = strtok(NULL, " ");
        // Too few arguments
        if (arg1 == NULL) {
            printf("%sUsage: set_date <date: mm/dd/yyyy>)\n", RED);
            return;
        }
        
        char *arg2 = strtok(NULL, " ");
        // Too many arguments
        if (arg2 != NULL) {
            printf("%sThe command 'set_date' requires exactly one argument (date in mm/dd/yyyy format)\n", RED);
            return;
        }
        
        //pass the command arguments to the set_date function
        set_date(arg1);
    }
    // deletepcb command
    else if (strcmp(command_name, "delete_pcb") == 0) {
        char* name = strtok(NULL, " ");
        char* extra = strtok(NULL, " "); 
        // error checking for name of pcb(if it exists)
        if(!name) {
            printf("%sUsage: delete_pcb <name>\n", RED);
            return;
        }
        // error checking if the user enters more arguments
        if(extra) {
            printf("%sToo many arguments. Usage: delete_pcb <name>\n", RED);
            return;
        }
        delete_pcb(name);// call delete pcb, remove pcb and free memory
    }
    // block pcb command
    else if (strcmp(command_name, "block") == 0) {
        char* name = strtok(NULL, " ");
        char* extra = strtok(NULL, " ");
        // error checking for name of pcb(if it exists)
        if(!name) {
            printf("%sUsage: block <name>\n", RED);
            return;
        }
        // error checking if the user enters more arguments
        if(extra) {
            printf("%sToo many arguments. Usage: block <name>\n", RED);
            return;
        }
        block_pcb(name);// call block pcb, set it to block state

    }

    else if (strcmp(command_name, "shaw") == 0) {
        print_the_silk();
        return;
    }
    else if (strcmp(command_name, "unblock") == 0) {
        char* name = strtok(NULL, " ");
        char* extra = strtok(NULL, " ");
        // error checking for name of pcb(if it exists)
        if(!name) {
            printf("%sUsage: unblock <name>\n", RED);
            return;
        }
        // error checking if the user enters more arguments
        if(extra) {
            printf("%sToo many arguments. Usage: unblock <name>\n", RED);
            return;
        }
        unblock_pcb(name);// call unblock pcb, set pcb to ready

    }
    // loadr3 command 
    else if (strcmp(command_name, "loadr3") == 0) {
        char* arg = strtok(NULL, " ");
        if (arg != NULL) {
            printf("%sThe command 'loadr3' does not take any additional arguments.\n", RED);
            return;
        }
        loadr3(); // call loadr3 which loads all test processes in a non-suspended ready state
        printf("Loaded 5 R3 test processes\n");
    }
    // Loadr3_suspended
    else if (strcmp(command_name, "loadr3_suspended") == 0) {
        char* arg = strtok(NULL, " ");
        if (arg != NULL) {
            printf("%sThe command 'loadr3_suspended' does not take any additional arguments.\n", RED);
            return;
        }
        loadr3_suspended(); // call loadr3_suspended, similar to loadr3 but suspends the processes.
        printf("Loaded 5 R3 test processes (suspended)\n");
    }
    // Load process
    else if (strcmp(command_name, "load_process") == 0) {
        char* name = strtok(NULL, " ");
        char* extra = strtok(NULL, " ");
        if (!name) {
            printf("%sUsage: load_process <name>\n", RED);
            printf("Valid names: proc1, proc2, proc3, proc4, proc5\n");
            return;
        }
        if (extra) {
            printf("%sToo many arguments. Usage: load_process <name>\n", RED);
            return;
        }
        if (load_process(name)) {
            printf("Loaded process '%s'\n", name);
        }
    }
    // resume all
    else if (strcmp(command_name, "resume_all") == 0) {
        char* arg = strtok(NULL, " ");
        if (arg != NULL) {
            printf("%sThe command 'resume_all' does not take any additional arguments.\n", RED);
            return;
        }
        resume_all(); // resume all suspended processes.
        printf("Resumed all suspended processes\n");
    }
    // showpcb command
    else if (strcmp(command_name, "show_pcb") == 0) {
        char* name = strtok(NULL, " ");
        char* extra = strtok(NULL, " ");
        if (!name) {
            printf("%sUsage: show_pcb <name>\n", RED);
            return;
        }
        // error checking if the user enters more arguments
        if (extra) {
            printf("%sToo many arguments. Usage: show_pcb <name>\n", RED);
            return;
        }
        show_pcb(name);
    }
    else if (strcmp(command_name, "set_priority") == 0) {
        char* name = strtok(NULL, " ");
        char* prio_str = strtok(NULL, " ");
        char* extra = strtok(NULL, " ");
        if (!name || !prio_str) {
            printf("%sUsage: set_priority <name> <priority:0-9>\n", RED);
            return;
        }
        // error checking if the user enters more arguments
        if (extra) {
            printf("%sToo many arguments. Usage: set_priority <name> <priority:0-9>\n", RED);
            return;
        }

        int prio = atoi(prio_str);
        set_pcb_priority(name, prio);
    }
    else if (strcmp(command_name, "suspend") == 0) {
        char* name = strtok(NULL, " ");
        char* extra = strtok(NULL, " ");
        if (!name) {
            printf("%sUsage: suspend <name>\n", RED);
            return;
        }
        // error checking if the user enters more arguments
        if (extra) {
            printf("%sToo many arguments. Usage: suspend <name>\n", RED);
            return;
        }
        suspend_pcb(name);
    }
    else if (strcmp(command_name, "resume") == 0) {
        char* name = strtok(NULL, " ");
        char* extra = strtok(NULL, " ");
        if (!name) {
            printf("%sUsage: resume <name>\n", RED);
            return;
        }
        // error checking if the user enters more arguments
        if (extra) {
            printf("%sToo many arguments. Usage: resume <name>\n", RED);
            return;
        }
        resume_pcb(name);
    }
    else if (strcmp(command_name, "show_ready") == 0) {
        char* arg = strtok(NULL, " ");
        if (arg != NULL) {
            printf("%sThe command 'show_ready' does not take any additional arguments.\n", RED);
            return;
        }
        show_ready_pcb();
    }
    else if (strcmp(command_name, "show_blocked") == 0) {
        char* arg = strtok(NULL, " ");
        if (arg != NULL) {
            printf("%sThe command 'show_blocked' does not take any additional arguments.\n", RED);
            return;
        }
        show_blocked_pcb();
    }
    else if (strcmp(command_name, "show_all") == 0) {
        char* arg = strtok(NULL, " ");
        if (arg != NULL) {
            printf("%sThe command 'show_all' does not take any additional arguments.\n", RED);
            return;
        }
        show_all_pcb();
    }
else if (strcmp(command_name, "alarm") == 0) {
        char* time_arg = strtok(NULL, " ");
        if (!time_arg) {
            printf("Usage: alarm HH:MM:SS \"message\"\n");
            return;
        }
        
        // Get the rest of the string after the time argument
        char* remaining = strtok(NULL, "");
        if (!remaining) {
            printf("Usage: alarm HH:MM:SS \"message\"\n");
            printf("Message must be provided\n");
            return;
        }
        
        // Skip any leading whitespace
        while (*remaining == ' ' || *remaining == '\t') {
            remaining++;
        }
        
        // Check for opening quote
        if (*remaining != '"') {
            printf("Usage: alarm HH:MM:SS \"message\"\n");
            printf("Message must be enclosed in quotes\n");
            return;
        }
        
        remaining++; // Skip opening quote
        
        // Find the closing quote manually
        char* message_end = remaining;
        while (*message_end != '\0' && *message_end != '"') {
            message_end++;
        }
        
        if (*message_end != '"') {
            printf("Usage: alarm HH:MM:SS \"message\"\n");
            printf("Message must be enclosed in quotes\n");
            return;
        }
        
        // Null-terminate the message
        *message_end = '\0';
        
        if (create_alarm_process(time_arg, remaining)) {
            printf("Alarm set successfully\n");
        }
    } else if (strcmp(command_name, "showalloc") == 0) {
        char* arg = strtok(NULL, " ");
        if (arg != NULL) {
            printf("%sThe command 'showalloc' does not take any additional arguments.\n", RED);
            return;
        }
        usr_show_allocated(NULL);
    } else if (strcmp(command_name, "showfree") == 0) {
        char* arg = strtok(NULL, " ");
        if (arg != NULL) {
            printf("%sThe command 'showfree' does not take any additional arguments.\n", RED);
            return;
        }
        usr_show_free(NULL);
    }

    // If the command is not recognized, print an error message
    else  {
        printf("%sUnknown command '%s'. Use 'help' for a list of valid commands.\n", RED, command_name);
    }
}
