//说明fork:
//fork就是解决shell只能运行一条命令这个问题所需要的，使用fork，
//不但能够创建新的进程，而且能分辨原来的进程和新创建的进程，

# include <stdio.h>
# include <unistd.h>


int main(void)
{
	int fork_rv;

	printf("Before:my pid is %d\n",getpid());

	fork_rv = fork();

	if(fork_rv == -1)
		perror("fork");
	else if(fork_rv == 0)
		printf("I am the child. mypid=%d\n",getpid());
	else
		printf("I am the parent. my child is =%d\n",fork_rv);
	return 0;
}