#include <style.h>
#include <help_commands.h>
#include <stdlib.h>
#include <string.h>

void help(void) {
    printf("\n");
    printf("Available Commands:\n");
    printf("%s\t%-16sShow this help message or show detailed help for given command\n", BLUE, "help <command>");
    printf("\t%-16sExit the OS; Asks the user for confirmation.\n", "exit");
    printf("\t%-16sDisplays the current version of the OS\n", "version");
    printf("\t%-16sGet the current date\n", "get_date");
    printf("\t%-16sSet the current date\n", "set_date <date>");
    printf("\t%-16sGet the current time.\n", "get_time");
    printf("\t%-16sSet the current time\n", "set_time <time>");
    printf("%s%16s%s\n", CYAN, " - PCB Commands", BLUE);
    printf("\t%-45sDeletes the given PCB.\n", "delete_pcb <name>");
    printf("\t%-45sSet the given PCB to the blocked state.\n", "block <name>");
    printf("\t%-45sUnblocks the given PCB, changing it's state to Ready.\n", "unblock <name>");
    printf("\t%-45sShows a detailed overview of the given PCB.\n", "show_pcb <name>");
    printf("\t%-45sAllows you to change the priority of an existing PCB.\n", "set_priority <name> <priority:0-9>");
    printf("\t%-45sSuspends the given PCB.\n", "suspend <name>");
    printf("\t%-45sResumes the given PCB.\n", "resume <name>");
    printf("\t%-45sResumes all currently suspended PCBs.\n", "resume_all");
    printf("\t%-45sDisplays the processes in the ready state.\n", "show_ready");
    printf("\t%-45sDisplays the processes in the blocked state.\n", "show_blocked");
    printf("\t%-45sDisplays all processes being tracked.\n", "show_all");
    printf("%s%16s%s\n", CYAN, " - Process Commands", BLUE);
    printf("\t%-45sLoads the given process.\n", "load_process <name>");
    printf("\t%-45sLoads the 5 R3 processes in the ready state.\n", "loadr3");
    printf("\t%-45sLoads the 5 R3 processes in the suspended state.\n", "loadr3_suspended");
    printf("\t%-45sSets an alarm for some time in the future, at which a message will display.\n", "alarm <time:HH:MM:SS> <message>");
    printf("%s%16s%s\n", CYAN, " - Memory Commands", BLUE);
    printf("\t%-45sShows the allocated memory blocks.\n", "showalloc");
    printf("\t%-45sShows the free memory blocks.\n", "showfree");
    printf("\n");
}

void help_with(char* command_name) {
    if (strcmp(command_name, "exit") == 0) {
        printf("\nexit: exit\n");
        printf("----------------------\n");
        printf("%s\tIncludes a confirmation request from the user. Following\n", BLUE);
        printf("\ta positive response, the system breaks the input loop and\n");
        printf("\tenters the shutdown process. Otherwise, nothing happens.\n\n");
    } else if (strcmp(command_name, "version") == 0) {
        printf("\nversion: version\n");
        printf("----------------------\n");
        printf("\t%sPrints out the version of TAos that is running.\n\n", BLUE);
    } else if (strcmp(command_name, "get_date") == 0) {
        printf("\nget_date: get_date\n");
        printf("----------------------\n");
        printf("%s\tPrints out the current date. By default it is set to UTC time.\n\n", BLUE);
    } else if (strcmp(command_name, "set_date") == 0) {
        printf("\nsetdate: setdate <mm/dd/yyyy>\n");
        printf("----------------------\n");
        printf("%s\tPrints out the current system date. By default it is set to UTC time.\n\n", BLUE);
    } else if (strcmp(command_name, "get_time") == 0) {
        printf("\nget_time: get_time\n");
        printf("----------------------\n");
        printf("%s\tPrints out the current system time. By default it is set to UTC time.\n\n", BLUE);
    } else if (strcmp(command_name, "set_time") == 0) {
        printf("\nset_time: set_time <hh:mm:ss>\n");
        printf("----------------------\n");
        printf("%s\tSets the current system time. By default it is set to UTC time.\n\n", BLUE);
    } else if (strcmp(command_name, "delete_pcb") == 0) {
        printf("\ndelete_pcb: delete_pcb <name>\n");
        printf("----------------------\n");
        printf("%s\tDeletes an existing process control block. The user cannot\n", BLUE);
        printf("\tdelete a system process.\n\n");
    } else if (strcmp(command_name, "block") == 0) {
        printf("\nblock: block <name>\n");
        printf("----------------------\n");
        printf("%s\tChanges the value of the ‘state’ field of an existing PCB from\n", BLUE);
        printf("\trunning to block. Block is not able to block a pcb before\n");
        printf("\tit has been created.\n\n");
    } else if (strcmp(command_name, "unblock") == 0) {
        printf("\nunblock: unblock <name>\n");
        printf("----------------------\n");
        printf("%s\tChanges the value of the ‘state’ field of an existing PCB from\n", BLUE);
        printf("\tblock to ready. Block_pcb is not able to block successfully run\n");
        printf("\ta pcb before it has been created.\n\n");
    } else if (strcmp(command_name, "show_pcb") == 0) {
        printf("\nshow_pcb: show_pcb <name>\n");
        printf("----------------------\n");
        printf("%s\tOutput the values of the given PCB.\n\n", BLUE);
    } else if (strcmp(command_name, "set_priority") == 0) {
        printf("\nset_priority: set_priority <name> <priority:0-9>\n");
        printf("----------------------\n");
        printf("%s\tSets the priority of a given PCB. The priority of a system process\n", BLUE);
        printf("\tcannot be changed after being created.\n\n");
    } else if (strcmp(command_name, "suspend") == 0) {
        printf("\nsuspend: suspend <name>\n");
        printf("----------------------\n");
        printf("%s\tChanges the value of the ‘state’ field of an existing PCB from ready\n\n", BLUE);
        printf("\tto suspend. System processes cannot be suspended.\n\n");
    } else if (strcmp(command_name, "resume") == 0) {
        printf("\nresume: resume <name>\n");
        printf("----------------------\n");
        printf("%s\tChanges the value of the ‘state’ field of an existing PCB from suspended\n\n", BLUE);
        printf("\tto ready.\n\n");
    } else if (strcmp(command_name, "show_ready") == 0) {
        printf("\nshow_ready: show_ready <name>\n");
        printf("----------------------\n");
        printf("%s\tPrints out the details of the ready queue, including an overview of each\n", BLUE);
        printf("\tPCB in the queue.\n\n");
    } else if (strcmp(command_name, "show_blocked") == 0) {
        printf("\nshow_blocked: show_blocked <name>\n");
        printf("----------------------\n");
        printf("%s\tPrints out the details of the blocked queue, including an overview of each\n\n", BLUE);
        printf("\tPCB in the queue.\n\n");
    } else if (strcmp(command_name, "show_all") == 0) {
        printf("\nshow_all: show_all <name>\n");
        printf("----------------------\n");
        printf("%s\tPrints out an overview of each PCB.\n\n", BLUE);
    } else if (strcmp(command_name, "resume_all") == 0) {
        printf("\nresume_all: resume_all\n");
        printf("----------------------\n");
        printf("%s\tResumes every currently suspended process.\n\n", BLUE);
    } else if (strcmp(command_name, "load_process") == 0) {
        printf("\nload_process: load_process <name>\n");
        printf("----------------------\n");
        printf("%s\tLoads the given process by name. Valid process names are as follows:\n", BLUE);
        printf("\t proc1, proc2, proc3, proc4, proc5\n\n");
    } else if (strcmp(command_name, "loadr3") == 0) {
        printf("\nloadr3: loadr3\n");
        printf("----------------------\n");
        printf("%s\tLoads all the R3 processes (proc1 through proc5) in the suspended state.\n\n", BLUE);
    } else if (strcmp(command_name, "loadr3_suspended") == 0) {
        printf("\nloadr3_suspended: loadr3_suspended\n");
        printf("----------------------\n");
        printf("%s\tLoads all the R3 processes (proc1 through proc5) in the suspended state.\n\n", BLUE);
    } else if (strcmp(command_name, "alarm") == 0) {
        printf("\nalarm: alarm <time:HH:MM:SS> <message>\n");
        printf("----------------------\n");
        printf("%s\tSets an alarm to go off in the next 24 hours at the specified time. When\n", BLUE);
        printf("\ttriggered, the alarm will display the message assigned to it. The message\n");
        printf("\tmust be enclosed in double quotes.\n\n");
    } else if (strcmp(command_name, "showalloc") == 0) {
        printf("\nshowalloc: showalloc\n");
        printf("----------------------\n");
        printf("%s\tShows all allocated Memory Control Blocks.\n\n", BLUE);
    } else if (strcmp(command_name, "showfree") == 0) {
        printf("\nshowfree: showfree\n");
        printf("----------------------\n");
        printf("%s\tShows all free Memory Control Blocks.\n\n", BLUE);
    } else {
        printf("%s'%s' is not a recognized command.\n", RED, command_name);
    }
}
