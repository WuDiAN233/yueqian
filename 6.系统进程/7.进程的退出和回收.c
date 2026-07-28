#include "myhead.h"

int main(void)
{
    pid_t id1;
    pid_t id2;
    pid_t id3;
    int status;

    /* 创建第一个子进程 */
    id1 = fork();

    if (id1 < 0) {
        perror("fork id1");
        return -1;
    }

    if (id1 == 0) {
        /* 第一个子进程 */
        for (int i = 0; i < 5; i++) {
            printf("子进程1负责清除垃圾！PID=%d\n",getpid());
            sleep(1);
        }

        exit(4);
    }

    id2 = fork();

    if (id2 < 0) {
        perror("fork id2");
        return -1;
    }

    if (id2 == 0) {
        /* 第二个子进程 */
        for (int j = 0; j < 5; j++) {
            printf("子进程2负责整理垃圾！PID=%d\n",getpid());
            sleep(1);
        }
        exit(5);
    }

    id3 = fork();

    if (id3 < 0) {
        perror("fork id3");
        return -1;
    }

    if (id3 == 0) {
        /* 第二个子进程 */
        for (int j = 0; j < 5; j++) {
            printf("子进程3负责吃掉垃圾！PID=%d\n",getpid());
            sleep(1);
        }
        exit(6);
    }
    /* 只有原父进程能运行到这里 */
    for (int i = 0; i < 3; i++) {
        printf("父进程负责杀毒！PID=%d\n", getpid());
        sleep(1);
    }

    /* 回收第一个子进程 */
    waitpid(id2, &status, 0);

    if (WIFEXITED(status)) {
        printf("子进程2退出值：%d\n", WEXITSTATUS(status));
    }

    /* 回收第二个子进程 */
    waitpid(id3, &status, 0);

    if (WIFEXITED(status)) {
		printf("子进程3退出值：%d\n", WEXITSTATUS(status));
    }

    waitpid(id1, &status, 0);

    if (WIFEXITED(status)) {
		printf("子进程1退出值：%d\n", WEXITSTATUS(status));
    }
    printf("三个个子进程都已回收\n");

    return 0;
}