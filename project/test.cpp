#include <stdio.h>
#include <pthread.h>

int counter = 0; // 共享变量

void *incrementCounter(void *arg)
{
    for (int i = 0; i < 100000; i++)
    {
        counter++; // 这里是非线程安全的
    }
    return NULL;
}

int main()
{
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, incrementCounter, NULL);
    pthread_create(&thread2, NULL, incrementCounter, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("最终计数器值: %d\n", counter); // 可能不是 200000
    return 0;
}

// g++ -pthread test.cpp