#include <pthread.h>
#include <stdio.h>

//compile command is gcc thread.c -thread

//Good thread tutorial for pthread.h
//https://randu.org/tutorials/threads/

//Random thread function for testing
//void* is used when working with different pointer types
void* threadFunc(void* args){
    while(1){
        printf("test thread");
    }
}

int main(){


    /*Let's digest the arguments required for pthread_create():
        pthread_t *thread: the actual thread object that contains pthread id
        pthread_attr_t *attr: attributes to apply to this thread
        void *(*start_routine)(void *): the function this thread executes
        void *arg: arguments to pass to thread function above
*/
    //creating thread
    int pthread_create(pthread_t *thread, NULL,&threadFunc,NULL)
    if(ret==0){
        printf("thread created");
    }else{
        printf("thread not created");
        return 0;
    }

    return 0;

}