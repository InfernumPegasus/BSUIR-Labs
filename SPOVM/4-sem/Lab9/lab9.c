#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

sem_t* sem_output_messages;
sem_t* sem_input_messages;
sem_t* sem_buf_size;

void* thread_function(void* shm_id)
{
    sleep(10);
    struct stat stat_item;
    int out_messages;
    sem_getvalue(sem_output_messages, &out_messages);
    int in_messages, int_buf_size = 0, str_size;
    while(1)
    {
        //sem_wait(sem_input_messages);
        sem_getvalue(sem_input_messages, &in_messages);
        if(in_messages != out_messages)
        {
            fstat(*(int*)shm_id, &stat_item);
            char* str = (char*) mmap(NULL, stat_item.st_size, PROT_READ, MAP_SHARED, *(int*)shm_id, 0);
            printf("%s", str);
            out_messages++;
        }
        str_size = stat_item.st_size;
        sem_getvalue(sem_buf_size, &int_buf_size);
        while(int_buf_size > 0)
        {
            sem_wait(sem_buf_size);
            int_buf_size --;
            //stat_item.st_size--;
        }
    }
    return 0;
}

int main()
{
    errno = 0;
    char* str_copy;
    pthread_t thread;

    printf("e - Exit the program\n\n");
    int status, str_size;
    int shm_id = shm_open("/filename", O_CREAT | O_RDWR, 0666);
    if(shm_id == -1)
    {
        printf("Error shm_open\n");
        return 0;
    }
    // sem_output_messages = sem_open("/sem_output_messages", O_CREAT, S_IRUSR | S_IWUSR, 0);
    // sem_input_messages = sem_open("/sem_input_messages", O_CREAT, S_IRUSR | S_IWUSR, 0);
    // sem_buf_size = sem_open("/sem_buf_size", O_CREAT, S_IRUSR | S_IWUSR, 0);
    sem_output_messages = sem_open("/output", O_CREAT, S_IRUSR | S_IWUSR, 0);
    sem_input_messages = sem_open("/input", O_CREAT, S_IRUSR | S_IWUSR, 0);
    sem_buf_size = sem_open("/bufsize", O_CREAT, S_IRUSR | S_IWUSR, 0);
    int input_value, output_value, int_buf_size = 0;
    // sem_getvalue(sem_input_messages, &input_value);
    // sem_getvalue(sem_output_messages, &output_value);
    // while(input_value > output_value)
    // {
    //     sem_post(sem_output_messages);
    //     sem_getvalue(sem_input_messages, &input_value);
    //     sem_getvalue(sem_output_messages, &output_value);
    // }
    char* str = NULL;
    status = pthread_create(&thread, NULL, thread_function, (void*)(&shm_id));
    if(status != 0)
    {
        printf("Thread creation failed!\n");
        return 0;
    }
    while (1)
    {
        str_size = 0;
        str_copy = (char*)calloc(1, sizeof(char));
        for(int i = 0; ; i++)
        {
            str_size = i + 1;
            str_copy[i] = getc(stdin);
            if(str_copy[i] == '\n')
                break;
            if(i != 0)
                str_copy = (char*)realloc(str_copy, (int_buf_size + str_size) * sizeof(char));
        }
        if (strcmp(str_copy, "e\n") == 0)
        {
            sem_getvalue(sem_buf_size, &int_buf_size);
            while(int_buf_size > 0)
            {
                sem_wait(sem_buf_size);
                sem_getvalue(sem_buf_size, &int_buf_size);
            }
            shm_unlink("/filename");
            sem_close(sem_output_messages);
            sem_close(sem_input_messages);
            sem_close(sem_buf_size);
            sem_unlink("/bufsize");
            sem_unlink("/input");
            sem_unlink("/output");
            return 0;
        }
        int new_buf = 0;
        sem_getvalue(sem_buf_size, &int_buf_size);
        for(int i = 0; i < str_size; i++)
        {
            sem_post(sem_buf_size);
            sem_getvalue(sem_buf_size, &int_buf_size);
        }
        ftruncate(shm_id, int_buf_size);
        str = (char*)mmap(NULL, int_buf_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);
        if(str == MAP_FAILED)
        {
            printf("Error mmap\n");
            return(0);
        }
        //strcpy(str, str_copy);
        for(int i = int_buf_size - str_size; i < int_buf_size; i++)
        {
            str[i] = str_copy[i - (int_buf_size - str_size)];
        }
        free(str_copy);

        if(int_buf_size == str_size)
            sem_post(sem_input_messages);
    }
    return 0;
}