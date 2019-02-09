#include <pthread.h>
#include <stdio.h>

//compile command is gcc thread.c -thread

//Good thread tutorial for pthread.h
//https://randu.org/tutorials/threads/

//Random thread function for testing
//void* is used when working with different pointer types
void* readMem(void* args){
    while(1){
        printf("test thread");
    }
}

void* writeMem(void* args){
    while(1){
        printf("test thread");
    }
}

int main(){


    /*The arguments required for pthread_create():
        pthread_t *thread: the actual thread object that contains pthread id
        pthread_attr_t *attr: attributes to apply to this thread
        void *(*start_routine)(void *): the function this thread executes
        void *arg: arguments to pass to thread function above
*/
    //creating thread
    int t1 = pthread_create(pthread_t *thread, NULL,&readMem,NULL)
    if(t1==0){
        printf("thread created");
    }else{
        printf("thread not created");
        return 0;
    }

      int t2 = pthread_create(pthread_t *thread, NULL,&writeMem,NULL)
    if(t2==0){
        printf("thread created");
    }else{
        printf("thread not created");
        return 0;
    }

    return 0;

}