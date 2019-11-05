//这是wait的第一个demo，主要说明wait的通知作用
//这里体现了wait的两个作用：
//1、wait阻塞调用它的程序直到子进程结束调用exit，这可以说是一个组合，可以使两个进程
//能够同步他们的行为
//2、wait返回结束进程的pid，wait的返回值是调用exit的子进程的PID，

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>

# define DELAY 2

int main()
{
	int newpid;

	void child_code(),parent_code();

	printf("before:mypid is %d\n",getpid());

	if((newpid = fork()) == -1)
		perror("fork");
	else if(newpid == 0)
		child_code(DELAY);
	else
		parent_code(newpid);

	return 0;
}

void child_code(int delay)
{
	printf("child %d here, will sleep for %d seconds\n",getpid(),delay);
	sleep(delay);
	printf("child done. About to exit\n");
	//子进程调用exit就像发送一个信号给父进程以唤醒它
	//这里使用exit返回了17，但是父进程没有使用，第二个demo会使用
	exit(17);
}

void parent_code(int childpid)
{
	int wait_rv;
	//这里的wait传回子进程调用exit调用的值
	//wait的返回值告诉父进程那个任务结束了，这样就可以继续有效的处理了
	wait_rv = wait(NULL);
	printf("done waiting for %d. Wait returned:%d\n",childpid,wait_rv);
}





