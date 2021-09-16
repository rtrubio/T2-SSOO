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
    new_process -> quantum = 0;
    new_process -> elecciones = 0;
    new_process -> interrupciones = 0;
    new_process -> turnaround_time = 0;
    new_process -> response_time = 0;
    new_process -> waiting_time = 0;
    new_process -> rafaga_actual = NULL;
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

Rafaga* rafaga_init(int duracion){
  Rafaga* new_rafaga = malloc(sizeof(Rafaga));
  new_rafaga -> duracion = duracion;
  new_rafaga -> restante = duracion;
  new_rafaga -> next = NULL;
  return new_rafaga;
};

// función scheduler:
// encontrar el process FIFO que este ready y extraerlo de la cola

void scheduler(Queue* queue, Cpu* cpu){
    //------ sale el process se la CPU al final de la cola
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
    //-----------------------------------------------------
    
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
  //Process* new_process;
  Queue* new_queue;
  Queue* process_queue;

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
        Process* new_process = process_init(contador_pid, line[0], atoi(line[1]), atoi(line[2]), atoi(line[3]));
        //printf("Nuevo proceso de id: %i, nombre: %s, estado: %s\n", new_process->pid, new_process->nombre, new_process->estado);
        printf("Nuevo proceso de id: %i\n",new_process->pid);
        printf("nombre: %s\n",new_process->nombre);
        printf("estado: %s\n",new_process->estado);
        
        for (int i = 0; i < 2*(atoi(line[3]))-1; i++){
          Rafaga* new_rafaga = rafaga_init(atoi(line[4+i])); 
          //printf("1\n");
          // caso base cuando es la primera rafaga a agregar:
          if(i==0){
            //printf("2\n");
            new_process->rafaga_actual = new_rafaga;
          }else{
            //printf("3\n");
            // hay que anexar new_rafaga a la ultima rafaga de la linked list
            Rafaga* aux_rafaga = new_process->rafaga_actual;
            printf("aux_rafaga duracion %i\n", new_process->rafaga_actual->duracion);
            //printf("4\n");
            while(aux_rafaga->next != NULL){
              //printf("5\n");
              aux_rafaga = aux_rafaga->next;
            }
            //printf("6\n");
            aux_rafaga->next = new_rafaga;
          }
          // comprobar rafagas bien creadas:
          //Rafaga* aux_rafaga = new_process->rafaga_actual;

        }
        while(new_process->rafaga_actual != NULL){
            printf("***** %s duracion rafaga es: %i\n",new_process->nombre, new_process->rafaga_actual->restante);
            new_process->rafaga_actual = new_process->rafaga_actual->next;
            }

        /*
        int *rafagas[atoi(line[3])];
        for (int i = 0; i < 2*(atoi(line[3]))-1; i++){
          printf("i: %i input: %i\n",i, atoi(line[4+i]));
          int* raf = atoi(line[4+i]);
          //rafagas[i] = atoi(line[4+i]);
          rafagas[i] = raf;
        }
        // int *array_ptr = rafagas;
    
        new_process->rafagas = rafagas;
        // rafagas[0] seria un puntero que podemos referir en los struct process
      
        printf("rafaga: %i \n", new_process->rafagas[0]);
        printf("rafaga: %i \n", new_process->rafagas[1]);
        printf("rafaga: %i \n", new_process->rafagas[2]);
        printf("rafaga: %i \n", new_process->rafagas[3]);
        printf("rafaga: %i \n", new_process->rafagas[4]);
        */

        if(contador_pid == 1){
          new_queue = queue_init(file->len);
          process_queue = queue_init(file->len);
          printf("Nueva cola de largo: %i\n", new_queue->cantidad);
          new_queue->inicio = new_process;
          //printf("new_queue->inicio->rafaga 0]: %i\n", new_queue->inicio->rafagas[0]);
          new_queue->last = new_process;
          printf("el primero y ultimo de la fila es %s\n", new_queue->inicio->nombre);
          
        }else{
          new_queue->last->next = new_process;
          new_queue->last = new_process;
          //printf("new_queue->last->rafaga 0]: %i\n", new_queue->last->rafagas[0]);

        }
  }
  
  int tiempo = 0;
  while(1){
  if(tiempo==50){
    break;
  }
  printf("** ejecutando ciclo while numero %i **\n", tiempo);
  // 1 si es que hay proceso en la CPU:
  if(cpu->exec != NULL){
    printf("estoy ejecutando el proceso %s en la cpu\n", cpu->exec->nombre);
    /*
    // restarle la rafaga 
    int indice_rafaga = 0;
    // obtenemos el indice de la rafaga que se esta ejecutando
    for (int i = 0; i < cpu->exec->cantidad_rafagas; i++){
      if(cpu->exec->rafagas[i] != 0){
        indice_rafaga = i;
        break; //solo quiero el primer indice
      }
    }
    */
    //printf("La ctdad de rafagas restantes de %s es %i\n", cpu->exec->nombre, cpu->exec->rafagas[indice_rafaga]);
    //printf("%i %s %s %i %i\n",cpu->exec->pid, cpu->exec->nombre,cpu->exec->estado,cpu->exec->cantidad_rafagas, cpu->exec->rafagas[indice_rafaga]);
    
    //cpu->exec->rafagas  es del tipo [0][0][14][3][6][3][12]

    // le restamos la ejecucion actual a la rafaga
    //cpu->exec->rafagas[indice_rafaga] = (cpu->exec->rafagas[indice_rafaga])-1;
    printf("1\n");
    printf("restante: %i\n",cpu->exec->rafaga_actual->restante);
    printf("2\n");
    cpu->exec->quantum = (cpu->exec->quantum)-1;
    printf("3\n");
    // si queda la casilla de la rafaga en 0: cede la cpu
    if(cpu->exec->rafaga_actual->restante == 0){
      printf("4\n");
      // como termino la rafaga, la eliminamos de la lista ligada de rafagas:
      cpu->exec->rafaga_actual = cpu->exec->rafaga_actual->next;
      printf("5\n");
      if(cpu->exec->rafaga_actual == NULL){
        printf("6\n");
        // aqui entra si consumimos la ultima rafaga del proceso
        // sale del sistema
        cpu->exec->estado = "FINISHED";
        cpu -> exec = NULL;
        printf("7\n");
      }else{
        printf("8\n");
        // aqui entra si no es la ultima rafaga: poner waiting y a la cola
        cpu->exec->estado = 'WAITING';
        process_queue -> last -> next = cpu -> exec; // y si process queue esta vacia? al inicio tambien
        process_queue -> last = cpu -> exec;
        if(process_queue->inicio == NULL){
          process_queue -> inicio = cpu -> exec;
        }
        cpu -> exec = NULL;
      }
    }else if(cpu->exec->quantum == 0){
      // pasa a ready y se va al final de la cola
      cpu->exec->estado = "READY";
      process_queue -> last -> next = cpu -> exec;
      process_queue -> last = cpu -> exec;
      cpu -> exec = NULL;

    }else{
      printf("else\n");
    }  
  }else{
    // 3: si no hay proceso en la cpu
    printf("[t = %i] No hay ningun proceso ejecutando en la cpu\n",tiempo);
    //printf("-- process_queue->incio tiene rafaga[0]: %i --\n", process_queue->inicio->rafagas[0]);
    Process* aux_new = NULL;
    if(process_queue->inicio != NULL){
      //printf("-- process_queue->incio tiene rafaga[0]: %i --\n", process_queue->inicio->rafagas[0]);
      aux_new = process_queue->inicio;
      //printf("-- aux_new tiene rafaga[0]: %i --\n", aux_new->rafagas[0]);

    }
    //Process* aux_new = process_queue->inicio;
    while(aux_new != NULL){
      printf("!!! WHILE !!!\n");
      if(aux_new->estado == "READY"){
        printf("!!!!! ENCONTRAMOS UN PROCESO READY PARA ENTRAR A LA CPU !!!!!\n");
        // sacar a aux_new de la fila y colocarlo en la cpu
        process_queue -> inicio = aux_new->next;
        aux_new->next = NULL;
        aux_new->estado = "RUNNING";
        cpu->exec = aux_new;
      }else if(aux_new->next != NULL){
        if(aux_new->next->estado == "READY"){
          aux_new->next = aux_new->next->next;
          aux_new->next->next = NULL;
          aux_new->next->next = "RUNNING";
          cpu->exec = aux_new->next;
        }
    }
    aux_new = aux_new->next;
  }
  if(cpu->exec != NULL){
    printf("COLOCAMOS UN PROCESO EN LA CPU\n");
    //printf("%i %s %s %i %i\n",cpu->exec->pid, cpu->exec->nombre,cpu->exec->estado,cpu->exec->cantidad_rafagas, cpu->exec->rafagas[0]);
  }
  }
  // punto 2: recorrer la lista ligada de todos los procesos para encontrar
  // uno que tenga tiempo de inicio == tiempo. y meterlo a la cola
  Process* prioridad;
  int menor_n_fabrica = 10;
  char menor_n_proceso;
  
  // contamos cuantos son los procesos a ingresar
  int ctdad_procesos_init = 0;
  //printf("* new_queue->inicio->rafagas[0]: %i\n", new_queue->inicio->rafagas[0]);
  Process* aux_init = new_queue->inicio;
  //printf("* new_queue->inicio->rafagas[0]: %i\n", new_queue->inicio->rafagas[0]);
  //printf("* aux_init rafagas[0]: %i\n", aux_init->rafagas[0]);
  while(aux_init != NULL){
    if(aux_init->tiempo_inicio == tiempo){
      ctdad_procesos_init++;
    }
    aux_init = aux_init->next;
  }
  
  printf("la ctdad_procesos_init es %i\n", ctdad_procesos_init);

  for (int i = 0; i < ctdad_procesos_init; i++){
    //recorrer new_queue
    //traspasar de new_queue a process_queue el de mayor prioridad
    //printf("1\n");
    Process* aux = new_queue->inicio;
    while(aux != NULL){
      //printf("2\n");
      if(aux->tiempo_inicio == tiempo){
        //printf("3\n");
        if(aux->fabrica < menor_n_fabrica){
          //printf("4\n");
          prioridad = aux;
          }
        }
      aux = aux -> next;
    }
    //printf("5\n");

    // aqui ya elegimos el Process* prioridad
      Process* aux2 = new_queue->inicio;
      //printf("6\n");
      while(aux2 != NULL){
        //printf("7\n");
        if(prioridad == new_queue -> inicio){
          new_queue->inicio = prioridad->next;
        }
        if(prioridad == new_queue -> last){ //solo pasaria cuando hay 1 process
          new_queue->last = NULL;
        }
        if(aux2 -> next == prioridad){
          aux2 -> next = prioridad->next;
          break;
        }
        aux2 = aux2 -> next;
      }
      if(prioridad != NULL){
        prioridad -> next = NULL;
      }
      //printf("el proceso %s salio de new_queue  con rafaga[0]: %i\n", prioridad->nombre, prioridad->rafagas[0]);
      // hasta aqui ya sacamos el proceso_priorida de new_queue
      //falta ingresarlo a process_queue
        
      prioridad->estado = "READY"; // creo que ya esta en READY
      //printf("8\n");
      if (process_queue -> last == NULL){
        process_queue -> inicio = prioridad;
        process_queue -> last = prioridad;
      }else{
        process_queue -> last -> next = prioridad;
        process_queue -> last = prioridad;
      }
      //printf("9\n");
      prioridad = NULL;
      printf("el proceso %s es el primero de process_queue\n", process_queue->inicio->nombre);
      printf("el proceso %s es el ultimo de process_queue\n", process_queue->last->nombre);
  }
  tiempo++;
  }

  //int quantum1 = quantum(new_queue, q, new_queue->inicio);
  //printf("el quantum es %i\n", quantum1);
  free_process(process_queue->inicio);
  free_queue(new_queue);
  free_queue(process_queue);
}

