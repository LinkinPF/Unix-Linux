//这里就可以真正的知道shell是如何运行的了：shell用fork建立新进程，用exec在新进程中运行用户指定的程序
//最后shell用wait等待新进程结束，wait系统调用同时从内核取得退出状态或者信号序号以告知子进程是如何结束的

//每个Unix shell都是使用这三个系统调用租的来实现的

//这个代码是在第一个psh1.c的基础上修改的，主要是修改了void execute(char * arglist[])函数
//这个代码是一个基础，在Chapter9中会优化出来一个更好的shell

//这个shell有一个致命的错误，就是退出psh2的唯一方法是使用ctrl+c，如果在psh等待子进程结束的时候
//按下Ctrl+c，shell和子进程都会结束，因为键盘信号发送给所有连接的进程，psh2和子进程都链接到终端
//当按下中断键，ttr驱动高速内核向所有由这个终端控制的进程发送SIGINT信号，所以都死了

//--------------------------------------------------------------------------------

//！！！思考：用进程编程，思考函数和进程的相似性
//1、(1)execvp/exit 就像 call/return 这种也是结构化编程函数调用是call/return，程序调用是fork,exec/exit
//1、(2)exec/exit 一个C程序可以fork/exec另一个程序，并传入参数，被调用的程序执行一定的操作，
//然后通过exit(n)来返回值，调用它的进程使用wait(&result)来获取exit的返回值，一个fork/exec调用起来的程序可以
//fork/exec别的程序
//
//由exec传递的参数必须是字符串，进程间通信的参数类型是字符串，就强迫子进程的通信必须使用文本位参数类型。
//
//2、全局变量和fork/exec
//全局变量是有害又是有用的，下一章学习好的机制。对exit/wait是一个很好的补充

//exit和exec的其他细节：
//exit是fork的逆操作，进程通过exit来停止运行，fork创建了一个进程，exit删除进程 

//关于exit的具体执行过程，如何释放资源，在书上的P255页写的很详细了。

//P256 关于exec家族
//注意：execlp和execvp中的p代表path。这两个函数在环境变量PATH中列出的路径中查找由
//第一个参数制定的程序，如果知道准确的文件位置，就可以在execl中的第一个参数中制定完整的路径，
//用制定的路径运行比运行execlp更快，而且指定路径更加安全。



# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>

# define MAXARGS 20
# define ARGLEN 100

int main()
{
	char * arglist[MAXARGS + 1];
	int numargs;
	char argbuf[ARGLEN];
	char *makestring();
	void execute();

	numargs = 0;

	while(numargs < MAXARGS)
	{
		printf("Arg[%d]?",numargs);
		if(fgets(argbuf,ARGLEN,stdin) && *argbuf != '\n')
			arglist[numargs++] = makestring(argbuf);
		else
		{
			if(numargs > 0){
				arglist[numargs] = NULL;
				execute(arglist);
				numargs = 0;
			}
		}
	}

	return 0;
}

void execute(char * arglist[])
{
	int pid,exitstatus;

	pid = fork();
	switch(pid) {
		case -1:
			perror("fork failed!\n");
			exit(1);
		case 0:			//子进程进入这里，直接执行命令
			execvp(arglist[0],arglist);
			perror("execvp failed!\n");
			exit(1);	
		default:		//默认父进程进入这里，执行wait系统调用，检测状态
			while(wait(&exitstatus)!=pid)
				;
			printf("child exited with status %d,%d\n",
				exitstatus>>8, exitstatus&0377);		

	}
}


//数组转字符串
char * makestring(char * buf)
{
	char * cp;

	//如果是ls，这里的buf的长度是3，然后把最后一个字符替换成\0，
	//printf("buf:%ld\n",strlen(buf));
	buf[strlen(buf)-1] = '\0';
	//之后这里的buf长度是2，因为strlen不计算\0
	//printf("buf:%ld\n",strlen(buf));
	cp = malloc(strlen(buf) + 1);
	if(cp == NULL)
	{
		fprintf(stderr,"no memory\n");
		exit(1);
	}
	strcpy(cp,buf);
	//printf("cp:%s  buf:%s\n",cp,buf);
	//printf("buf:%ld\n",strlen(buf));
	return cp;
}

