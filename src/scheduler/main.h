#pragma once

// Define the struct
typedef struct process {
  int pid;
  char* nombre;
  int fabrica;
  char* estado;
  struct process *next; 
} Process;

Process* process_init(int pid, char nombre, int fabrica);

/*
Process* process_init(int pid, char nombre, int fabrica){
    Process* new_process = malloc(sizeof(Process));
    new_process -> pid = pid;
    new_process -> nombre = nombre;
    new_process -> fabrica = fabrica;
    new_process -> estado = "READY";
    return new_process;
}
*/

typedef struct queue {
    int cantidad;
    Process *inicio;
    Process *final;
} Queue;

Queue* queue_init(int cantidad);

/*
Queue* queue_init(int cantidad){
    Queue* new_queue = malloc(sizeof(Queue));
    new_queue -> cantidad = cantidad;
    new_queue -> inicio = NULL;
    new_queue -> final = NULL;
    return new_queue;
}
*/