//这就是一个很简单的shell，一个程序运行一个程序
//shell需要的三项技术：get command, execute command, wait for command to finish
//简单来说就是：运行一个程序，建立一个程序，等待exit()
//执行一个程序的技术就是execvp系统调用


# include <stdio.h>
# include <unistd.h>

void main(void)
{
	char * arglist[3];

	arglist[0] = "ls";
	arglist[1] = "-l";
	arglist[2] = 0;
	printf("*** About to exec -l\n");
	//int execvp(const char* file, const char* argv[]);
	//第一个是要运行的文件，回到环境变量PATH中去找
	//第二个参数是一个参数列表，第0个参数是文件名
	//argv的最后一个必须是NULL
	//失败返回-1，成功无返回值，但是，失败会在当前进程运行，执行成功后，直接结束当前进程，可以在子进程中运行.
	execvp("ls",arglist);
	printf("*** ls is done\n");
}

/*
程序执行完以后不会显示第二个printf，因为execvp会把要执行的程序替换当前程序，execvp将程序从磁盘载入进程
以便它可以被运行，但是载入到那个进程？就载入当前进程

exec系统调用从当前进程中把当前程序的机器指令清楚，然后在空的进程中载入调用时指定的程序代码，最后运行这个新程序。
exec调整进程的内存分配使之适应新的程序对内存的要求，相同的进程不同的内容。
*/