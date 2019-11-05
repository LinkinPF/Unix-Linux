//说明fork的返回值
//这是shell需要的第二项技术：建立一个程序，就是fork系统调用

# include <stdio.h>
# include <unistd.h>


int main(void)
{
	int ret_from_fork,mypid;

	mypid = getpid();
	printf("Before:my pid is %d\n",mypid);

	//fork()成功以后，返回子进程号给父进程，返回0给子进程
	ret_from_fork = fork();

	sleep(1);
	printf("After:my pid is %d,fork() said %d\n",
		getpid(),ret_from_fork);
}
