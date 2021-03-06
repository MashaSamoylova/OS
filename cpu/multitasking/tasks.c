#include "tasks.h"
#include "helpers.h"

extern void irq_timer_handler();

task task_list[10];
unsigned int tasks_num = 10;
unsigned int stack_size = 1024;
unsigned char stacks[10][1024];
unsigned int current_task_index;


void exit_handler() {
    print("exit process with num: ");
    print_num(current_task_index + 1);
    print("\n");
    task_list[current_task_index].is_fineshed = 1;
    disable_intr();
    irq_timer_handler();
}

void init_task(unsigned int func_address) {
    save_state();
    for(int i = 0; i < tasks_num; i++) {
        if(task_list[i].is_fineshed) {
            task_list[i].is_fineshed = 0;
            unsigned int new_stack_pointer = stacks + stack_size*(i + 1) - 4;
            *((unsigned int*)new_stack_pointer) = exit_handler;
            new_stack_pointer -= 4;
            *((unsigned int*)new_stack_pointer) = 514; //eflags
            new_stack_pointer -= 4;
            *((unsigned int*)new_stack_pointer) = 8; // cs
            new_stack_pointer -= 4;
            *((unsigned int*)new_stack_pointer) = func_address; //eip
            new_stack_pointer -= 4*7;
            *((unsigned int*)new_stack_pointer) = new_stack_pointer - 4;
            task_list[i].esp = new_stack_pointer - 4;
            restore_state();
            return;
        }
    }
    print("Tasks limit exceeded\n");
    restore_state();
}

int tick = 0;

void irq_timer_handler_c(task_stack task_context) {
    if ((tick == 1000) || (task_list[current_task_index].is_fineshed)) {
        tick = 0;
        print("-------------------------\n");
        switch_task(&task_context);
    }
    tick++;
    send_byte_to_port(0x20, 0x20); /* master */
}

void switch_task(task_stack* task_context) {
    print("switch task\n");

    task_list[current_task_index].esp = task_context->esp;
    
    for (int i = current_task_index + 1; i < tasks_num; i++) {
        if (!task_list[i].is_fineshed) {
            task_context->esp = task_list[i].esp;
            current_task_index = i;
            return;
        }
    }
    for (int i = 0; i <= current_task_index; i++) {
        if (!task_list[i].is_fineshed) {
            task_context->esp = task_list[i].esp;
            current_task_index = i;
            return;
        }
    }
}

int custom_counter = 0;
void main_task() {
    while(1) {
        if ((custom_counter % 1000000) == 0) { 
           print("Hello from main task! -_0_0_-\n");
        }
        custom_counter++;
    }
}

void task2() {
    print("Hello from task 2\n");
}

void task3() {
    while(1) {
        if ((custom_counter % 1000000) == 0) {
            print("Hello from task 3\n");
        }
        custom_counter++;
    }
}

void task4() {
    while(1) {
        if ((custom_counter % 1000000) == 0) {
           print("Hello from task 4\n");
        }
        custom_counter++;
    }
}

void start_multitasking() {
    for(int i = 0; i < tasks_num; i++) {
        task_list[i].is_fineshed = 1;
    }
    init_task((unsigned int)main_task);
    init_task((unsigned int)task2);
    init_task((unsigned int)task3);
    init_task((unsigned int)task4);
    current_task_index = 0;
    main_task();
}