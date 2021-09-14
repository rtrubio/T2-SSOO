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