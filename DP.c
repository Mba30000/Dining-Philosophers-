#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include "queue.h"

#define NUM 5                           //define the number of philosophers

typedef enum {                          //possible states the philosophers may exhibit
        THINKING,
        EATING,
        HUNGRY
} State_t;

typedef struct {                        //struct to define attributes of each philosopher
        int id;
        State_t state;
} Person_t;

pthread_mutex_t forks[NUM];             //create array of forks of type mutex
Person_t philosophers_details[NUM];     //Array to hold the details of each philosopher
volatile sig_atomic_t terminate = 0;    //Atomic Flag for proper termination
Queue_t fork_queue[NUM];

void* cycle(void* arg) {                                //A cycle to simulate the behavior of the philosophers over time
        Person_t* philosopher = (Person_t*)arg;         //casting the philophers to type person after being cast void
        while (!terminate) {                            // The loop will end when CTRL+C is pressed
                thinking();
                take_fork(*philosopher);
                eating();
                place_fork(*philosopher);
        }
        pthread_mutex_unlock(&forks[philosopher->id]);  //When the program ends each philospher should unlock forks/resources
        pthread_mutex_unlock(&forks[(philosopher->id+1)%NUM]);
}

void thinking() {                                       //waste time to simulate thinking
        sleep(rand() % 5 + 1);
}

void eating() {                                         //waste time to simulate eating
        sleep(rand() % 5 + 1);
}

void take_fork(Person_t philosopher) {
        int id = philosopher.id;                        //get the id to be used to get the right fork
        int nxt = (id+1)%NUM;

        philosopher.state = HUNGRY;
        printf("        -> Philosopher %d is hungry!\n", id);

        queue(&fork_queue[id], id);
        queue(&fork_queue[nxt], id);
        if(id%2==0){                                            //solution to cyclic deadlock; avoids having each philosopher holding a fork
                while(1){
                        if(special_dequeue(&fork_queue[id], id)){  //special dequeue only returns true if the id is at the head
                                pthread_mutex_lock(&forks[id]);
                                break;
                        }
                };

                while(1){
                        if(special_dequeue(&fork_queue[nxt], id)){
                                pthread_mutex_lock(&forks[nxt]);
                                break;
                        }
                };
        }else{
                while(1){
                        if(special_dequeue(&fork_queue[nxt], id)){  //special dequeue only returns true if the id is at the head
                                pthread_mutex_lock(&forks[nxt]);
                                break;
                        }
                };
                while(1){
                        if(special_dequeue(&fork_queue[id], id)){
                                pthread_mutex_lock(&forks[id]);
                                break;
                        }
                };
        }
        printf("                -> Philosopher %d has picked up both forks after waiting!\n", id);

        philosophers_details[id].state = EATING;

        printf("Philosopher %d is eating!\n", id);
}

void place_fork(Person_t philosopher) {
        int id = philosopher.id;
        int nxt = (id+1)%NUM;

        printf("Philosopher %d is putting down forks!\n", id);

        pthread_mutex_unlock(&forks[id]);                       //unlock resources
        pthread_mutex_unlock(&forks[nxt]);

        printf("Philosopher %d has placed down both forks!\n", id);

        philosophers_details[id].state = THINKING;

        printf("Philosopher %d is thinking!\n", id);
}

void handle_cancel(int signal) {
    if (signal == SIGINT) {
        terminate= 1;                                           // Set flag to stop infinite loop in case of CTRL+C
    }
}


int main() {
        pthread_t philosopher_threads[NUM];                     //create a thread for each philosopher

        signal(SIGINT, handle_cancel);                          //signal to ensure  that the program terminates properly

        for (int i = 0; i < NUM; i++) {
                pthread_mutex_init(&forks[i], NULL);            //initiate each thread
                philosophers_details[i].id = i;                 //allocate to specific profile/details
                philosophers_details[i].state = THINKING;       //initiate their state to thinking
                init_queue(&fork_queue[i]);                     //initilize  queue for each fork
                pthread_create(&philosopher_threads[i], NULL, cycle, &philosophers_details[i]); //thread creation
        }

        for (int i = 0; i < NUM; i++) {
                pthread_join(philosopher_threads[i], NULL);     //Ensures the threads are released
                pthread_mutex_destroy(&forks[i]);               //Release all mutexes
        }
    return 0;
}