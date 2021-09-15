#pragma once

// Define the struct
typedef struct process {
  int pid;
  char** nombre;
  int fabrica;
  char* estado;
  struct process *next;
  int tiempo_inicio;
  int* rafagas;
  
  // Estadisticas
  int elecciones;
  int interrupciones;
  int turnaround_time;
  int response_time;
  int waiting_time;

} Process;

Process* process_init(int pid, char **nombre, int tiempo_inicio, int fabrica);

typedef struct queue {
    int cantidad;
    Process *inicio;
    Process *last;
} Queue;

Queue* queue_init(int cantidad);

typedef struct cpu {
    Process *exec;
} Cpu;

Cpu* cpu_init();


