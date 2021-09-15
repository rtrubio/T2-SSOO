#include <stdio.h>

#include "../file_manager/manager.h"
#include "main.h"

Process* process_init(int pid, char **nombre, int tiempo_inicio, int fabrica, int cantidad_rafagas){
    Process* new_process = malloc(sizeof(Process));
    new_process -> pid = pid;
    new_process -> nombre = nombre;
    new_process -> fabrica = fabrica;
    new_process -> estado = "READY";
    new_process -> tiempo_inicio = tiempo_inicio;
    new_process -> cantidad_rafagas = cantidad_rafagas;
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
    
    // aqui calcular quantum del proceso elegido (almacenar o retornar el quantum)

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

int quantum(Queue* queue, int q, Process* process){
  int fabrica = process->fabrica;
  int n = 0;
  int f = 0;
  int fabricas_actuales[4] = {1,2,3,4};

  Process* aux = queue->inicio;
  while(aux != NULL){
    if(aux->fabrica == fabrica){
      n++;
    }
    int indice = (aux->fabrica)-1;
    printf("el indice es: %i\n ",indice);
    fabricas_actuales[indice] = NULL; // asumiendo que fabricas son del 1 al 4
    aux = aux->next;
  }

  // f: las que efectivamente hay en cola
  for (int i = 0; i < 4; i++){
    if(fabricas_actuales[i] == NULL){
      f++;
    }

  }
  printf("el Q es: %i\n ",q);
  printf("el n es: %i\n ",n);    
  printf("el f es: %i\n ",f);

  int quantum = (q)/(n*f);
  return quantum;
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
  int q = 100;
  printf("Hello T2!\n");
  printf("argv 0: %s\n", argv[0]); //scheduler
  printf("argv 1: %s\n", argv[1]); //input.txt
  printf("argv 2: %s\n", argv[2]); //output.csv
  if(argv[3] != NULL){
    printf("argv 3: %s\n", argv[3]); // Q
    q = atoi(argv[3]);
  }

  Cpu* cpu = cpu_init();
  Process* new_process;
  Queue* new_queue;
  int contador_pid = 0;

  InputFile *file = read_file(argv[1]);

  printf("Reading file of length %i:\n", file->len);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];
    printf(
        "\tProcess %s from factory %s has init time of %s and %s bursts.\n",
        line[0], line[2], line[1], line[3]);
        contador_pid++;
        //printf("%s\n", line[0]);
        new_process = process_init(contador_pid, line[0], atoi(line[1]), atoi(line[2]), atoi(line[3]));
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
  
  int tiempo = 0;
  while(1){
  // 1 si es que hay proceso en la CPU:
  if(cpu->exec != NULL){
    // restarle la rafaga 
    int indice_rafaga = 0;
    // obtenemos el indice de la rafaga que se esta ejecutando
    for (int i = 0; i < cpu->exec->cantidad_rafagas; i++){
      if(cpu->exec->rafagas[i] != 0){
        indice_rafaga = i;
      }
    }
    //cpu->exec->rafagas  es del tipo [0][0][14][3][6][3][12]
    cpu->exec->rafagas[indice_rafaga] = cpu->exec->rafagas[indice_rafaga]-1;
    // si queda la casilla de la rafaga en 0: cede la cpu
    if(cpu->exec->rafagas[indice_rafaga] == 0){
      
    }
     
  }

  tiempo++;
  }

  //int quantum1 = quantum(new_queue, q, new_queue->inicio);
  //printf("el quantum es %i\n", quantum1);
  free_process(new_queue->inicio);
  free_queue(new_queue);

}


