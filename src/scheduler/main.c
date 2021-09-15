#include <stdio.h>

#include "../file_manager/manager.h"
#include "main.h"

Process* process_init(int pid, char **nombre, int tiempo_inicio, int fabrica){
    Process* new_process = malloc(sizeof(Process));
    new_process -> pid = pid;
    new_process -> nombre = nombre;
    new_process -> fabrica = fabrica;
    new_process -> estado = "READY";
    new_process -> tiempo_inicio = tiempo_inicio;
    new_process -> elecciones = 0;
    new_process -> interrupciones = 0;
    new_process -> turnaround_time = 0;
    new_process -> response_time = 0;
    new_process -> waiting_time = 0;
    printf("retornando nuevo proceso\n");

    
    
    return new_process;
};

Queue* queue_init(int cantidad){
    Queue* new_queue = malloc(sizeof(Queue));
    new_queue -> cantidad = cantidad;
    new_queue -> inicio = NULL;
    new_queue -> last = NULL;
    return new_queue;
};

Cpu* cpu_init(){
  Cpu* cpu = malloc(sizeof(Cpu));
  cpu -> exec = NULL;
  return cpu;
};

// función scheduler:
// encontrar el process FIFO que este ready y extraerlo de la cola

void scheduler(Queue* queue, Cpu* cpu){
    // sale el process se la CPU al final de la cola
    queue -> last -> next = cpu -> exec;
    queue -> last = cpu -> exec;
    cpu -> exec = NULL;

    Process* elegido = queue->inicio;
    while(elegido != NULL){
      // encontrar el primero que este ready
      if(elegido->estado = "READY"){
        break;
      }
      elegido = elegido->next;
    }
    
    // sacamos el process de la cola, arreglando el ->next de su antecesor
    // tambien cubrimos el caso si sacamos el inicio o final de la cola
    Process* aux = queue->inicio;
    while(aux != NULL){
      if(elegido == queue -> inicio){
        queue->inicio = elegido->next;
      }
      if(elegido == queue -> last){ //solo pasaria cuando hay 1 process
        queue->last = NULL;
      }
      if(aux -> next == elegido){
        aux -> next = elegido->next;
        break;
      }
    }

    if(elegido != NULL){
      elegido -> next = NULL;
    }
    cpu -> exec = elegido;
}

// darle la raiz de la cola
void free_process(Process* proceso){
  printf("free de %s\n", proceso->nombre);
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
        //printf("%s\n", line[0]);
        new_process = process_init(contador_pid, line[0], atoi(line[1]), atoi(line[2]));
        //printf("Nuevo proceso de id: %i, nombre: %s, estado: %s\n", new_process->pid, new_process->nombre, new_process->estado);
        printf("Nuevo proceso de id: %i\n",new_process->pid);
        printf("nombre: %s\n",new_process->nombre);
        printf("estado: %s\n",new_process->estado);
        
        int rafagas[atoi(line[3])];
        for (int i = 0; i < 2*(atoi(line[3]))-1; i++){
          rafagas[i] = atoi(line[4+i]);
        }
        // int *array_ptr = rafagas;
        new_process->rafagas = rafagas;
        // rafagas[0] seria un puntero que podemos referir en los struct process
        /*
        printf("rafaga: %i \n", new_process->rafagas[0]);
        printf("rafaga: %i \n", new_process->rafagas[1]);
        printf("rafaga: %i \n", new_process->rafagas[2]);
        printf("rafaga: %i \n", new_process->rafagas[3]);
        printf("rafaga: %i \n", new_process->rafagas[4]);
        */

        if(contador_pid == 1){
          new_queue = queue_init(file->len);
          printf("Nueva cola de largo: %i\n", new_queue->cantidad);
          new_queue->inicio = new_process;
          new_queue->last = new_process;
          printf("el primero y ultimo de la fila es %s\n", new_queue->inicio->nombre);
          
        }else{
          new_queue->last->next = new_process;
          new_queue->last = new_process;
        }
  }
  

  /*
  int tiempo = 0;

  for (int i = 0; i < 1000; i++){
    if(i=0){
      //crear process_queue
    }
    // buscaria en new_queue un process con tiempo_inicio == tiempo
    // si existe lo agrego a process_queue


    // if todos los process terminaron: break
    tiempo++;
  }
  // imprimir estadisticas y output
  */



  free_process(new_queue->inicio);
  free_queue(new_queue);

}


