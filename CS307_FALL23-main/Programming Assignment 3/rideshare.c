#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;


// struct for semaphore, written to fit naming convention from slides
typedef struct semaphore{
    int val;
    pthread_mutex_t lock;
    pthread_cond_t c;
} sem_t;

sem_t sem_A;
sem_t sem_B;
int car_id = 0;


// taken from slides
void sem_wait (sem_t *s) {
    // decrement the value of s by one
    // wait if the value of s is negative
    pthread_mutex_lock(&s->lock);
    s->val --;
    pthread_mutex_unlock(&global_lock); // not in slides, but it is necessary to unlock the global lock before waiting in order to prevent deadlock
    if (s->val < 0)
    pthread_cond_wait(&s->c, &s->lock);
    pthread_mutex_unlock(&s->lock);
}

// taken from slides
void sem_post (sem_t *s) {
    // increment the value of s by one
    // wake a sleeping thread if exists
    pthread_mutex_lock(&s->lock);
    s->val ++;
    pthread_cond_signal(&s->c);
    pthread_mutex_unlock(&s->lock);
}

void sem_init(sem_t *sem, int val) {
    pthread_mutex_init(&sem->lock, NULL); 
    pthread_cond_init(&sem->c, NULL);
    sem->val = val;
}

void* create_rider(void* arg){
    
    bool is_car_formed = false;
    char* rider = (char*) arg;
    
    pthread_mutex_lock(&global_lock);
    printf("Thread ID: %ld, Team: %c, I am looking for a car\n", pthread_self(), *rider);
   
    /*
    the algorithm for the formation of a car is as follows:
    1. if the current thread's team's number is 0, then wait
    2. if the current thread's team's number is 1
        a. if other team's number is less than 2 (it can be 0 or 1), then wait 
        b. else (it can be 2 or 3 - it cannot be 4 or more since they would already form a car), then form a car and be the captain
    3. if the current thread's team's number is 2, then wait
    4. if the current thread's team's number is 3, then form a car and be the captain
    */
    
    if (*rider == 'A'){ // if the current thread is from team A
    
        if (sem_A.val == -3){ // condition 4
            is_car_formed = true;
            for(int i = 0; i < 3; i++){
                sem_post(&sem_A); // wake up 3 threads from team A
            }
        }
        else if (sem_A.val == -1 && sem_B.val <= -2){ // condition 2b
            is_car_formed = true;
            sem_post(&sem_A); // wake up 1 thread from team A
            for(int i = 0; i < 2; i++){
                sem_post(&sem_B); // wake up 2 threads from team B
            }
        }
        else{ // other conditions
        
            sem_wait(&sem_A);
        } 
    }
    else{ // if the current thread is from team B
        
        if (sem_B.val == -3){ // condition 4
            is_car_formed = true;
            for(int i = 0; i < 3; i++){
                sem_post(&sem_B); // wake up 3 threads from team B
            }
        }
        else if (sem_B.val == -1 && sem_A.val <= -2){ // condition 2b
            is_car_formed = true;
            sem_post(&sem_B); // wake up 1 thread from team B
            for(int i = 0; i < 2; i++){
                sem_post(&sem_A); // wake up 2 threads from team A
            }
        }
        else{ // other conditions
            sem_wait(&sem_B);
        } 
    }

    printf("Thread ID: %ld, Team: %c, I have found a spot in a car\n", pthread_self(), *rider);
    pthread_barrier_wait(&barrier);

    if(is_car_formed){
        printf("Thread ID: %ld, Team: %c, I am the captain and driving the car with ID %d\n", pthread_self(), *rider, car_id);
        car_id++;
        pthread_barrier_destroy(&barrier);
        pthread_barrier_init(&barrier, NULL, 4);
        pthread_mutex_unlock(&global_lock);
    }
    
}


int main(int argc, char* argv[]){
    int num_riders_A = atoi(argv[1]);
    int num_riders_B = atoi(argv[2]);

    if (num_riders_A %2 != 0 || num_riders_B %2 != 0 || (num_riders_A + num_riders_B) %4 != 0){
        printf("The main terminates\n");
        exit(1);
    }
    pthread_t thread_list[num_riders_A + num_riders_B];
    sem_init(&sem_A, 0);
    sem_init(&sem_B, 0);
    pthread_barrier_init(&barrier, NULL, 4);

    char temp_team_A = 'A';
    char temp_team_B = 'B';
    char *team_A = &temp_team_A;
    char *team_B = &temp_team_B;

    for (int i = 0; i < num_riders_A; i++){
        pthread_create(&thread_list[i], NULL, create_rider, team_A);
    }

    for (int i = num_riders_A; i < num_riders_A + num_riders_B; i++){
        pthread_create(&thread_list[i], NULL, create_rider, team_B);
    }


    for (int i = 0; i < num_riders_A + num_riders_B; i++){
        pthread_join(thread_list[i], NULL);
    }

    pthread_barrier_destroy(&barrier); 
    printf("The main terminates\n");
    return 0;

}