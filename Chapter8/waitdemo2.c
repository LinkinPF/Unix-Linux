//这是wait的第二个demo，主要说明wait的通信作用，告诉父进程子进程是如何结束的

//一个进程以三种方式之一结束：成功、失败、死亡
//Unix惯例，成功的程序调用exit(0)或者从main函数中return 0；

//进程可能会失败，Unix惯例，程序遇到问题而退出调用exit时传给它一个非0的值，程序员可以给不同的错误分配不同的值

//最后，程序可能被一个信号杀死

//wait的第一个demo提到wait返回结束的子进程的PID，那么父进程如何知道子进程是以何种方式推出的呢？
//答案就是wait的参数，注意参考下面的代码，尤其是void parent_code(int childpid); 父进程在调用
//wait的时候传一个整型变量的地址给函数，内核就会吧进程推出的返回值给这个整数变量中，如果进程被杀死了，
//内核就会把信号序号存放在这个变量中。

//这个整数由三部分组成：8个bit记录退出值，7个bit记录信号序号，另一个bit指出发生错误并产生了内核映像(core dump)

//st:----n---             -             -------			共16位
//(exit value)    (core dump flag)   (signal number)
//
//  st----------------     <——————exit(n)
//	wait(&st)


//程序是在waitdemo1的基础上修改的，就可以让父进程接收到exit返回的值17了，正常情况signal和core都是0
//可以加长子进程睡眠时间，然后另起一个终端kill子进程看看结果是什么。

//wait(int*stausptr)如果一个子进程已经退出或者被杀死，对wait的调用立即返回，wait返回结束进程的PID，如果
//stausptr不是NULL，wait将退出状态或者信号序号复制到stausptr指向的整数中，这个值可以用sys/wait.h中的宏检测

//如果调用的进程没有子进程也没有得到终止状态值，则wait返回-1


# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>

# define DELAY 10

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
	exit(17);
}

void parent_code(int childpid)
{
	int wait_rv;		
	int child_status;
	int high_8,low_7,bit_7;

	//这里的wait传回子进程调用exit调用的值
	//wait的返回值告诉父进程那个任务结束了，这样就可以继续有效的处理了
	//wait_rv = wait(NULL);
	wait_rv = wait(&child_status);
	printf("done waiting for %d. Wait returned:%d\n",childpid,wait_rv);

	high_8 = child_status >> 8;
	low_7 = child_status & 0X7F;
	bit_7 = child_status & 0X80;

	printf("status:exit=%d,signal=%d,core=%d\n",high_8,low_7,bit_7);
}
