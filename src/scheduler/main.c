#include <stdio.h>

#include "../file_manager/manager.h"
#include "main.h"

Process* process_init(int pid, char nombre, int fabrica){
    Process* new_process = malloc(sizeof(Process));
    new_process -> pid = pid;
    new_process -> nombre = nombre;
    //strcpy(new_process -> nombre, nombre);
    new_process -> fabrica = fabrica;
    new_process -> estado = "READY";
    //strcpy(new_process -> nombre, "READY");
    return new_process;
};

Queue* queue_init(int cantidad){
    Queue* new_queue = malloc(sizeof(Queue));
    new_queue -> cantidad = cantidad;
    new_queue -> inicio = NULL;
    new_queue -> final = NULL;
    return new_queue;
};

// darle la raiz de la cola
void free_process(Process* proceso){
  if(proceso->next){
    free_process(proceso->next);
  }
  free(proceso);
}

void free_queue(Queue* queue){
  free(queue);
}



int main(int argc, char **argv)
{
  printf("Hello T2!\n");
  Process* new_process;
  Queue* new_queue;
  int contador_pid = 0;

  InputFile *file = read_file("input.txt");

  printf("Reading file of length %i:\n", file->len);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];
    printf(
        "\tProcess %s from factory %s has init time of %s and %s bursts.\n",
        line[0], line[2], line[1], line[3]);
        contador_pid++;
        printf("%s\n", line[0]);
        new_process = process_init(contador_pid, line[0], line[1]);
        printf("Nuevo proceso de id: %i, nombre: %s, estado: %s\n", new_process->pid, new_process->nombre, new_process->estado);
        if(contador_pid == 1){
          new_queue = queue_init(file->len);
          printf("Nueva cola de largo: %i\n", new_queue->cantidad);
          new_queue->inicio = new_process;
        }else{
          new_queue->final->next = new_process;
          new_queue->final = new_process;
        }
  }
  printf("1");
  free_process(new_queue->inicio);
  printf("2");
  free_queue(new_queue);
  printf("3");

}


