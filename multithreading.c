#include <pthread.h>
#include <stdio.h>

//compile command is gcc thread.c -thread

//Random thread function for testing
//void* is used when working with different pointer types
void* threadFunc(void* args){
    while(1){
        printf("test thread");
    }
}

int main(){

    //creating thread id
    pthread_t id;
    int x;

    //creating thread
    x = pthread_create(&id, NULL,&threadFunc,NULL)
    if(ret==0){
        printf("thread created");
    }else{
        printf("thread not created");
        return 0;
    }

    while(1){
        printf("main function");
    }

    return 0;

}