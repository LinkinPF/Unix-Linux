//这个shell是带有提示符的shell

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>

# define MAXARGS 20
# define ARGLEN 100

int main()
{
	char * arglist[MAXARGS +1];
	int numargs;
	char argbuf[ARGLEN];
	char * makestring();
	int execute();

	numargs=0;

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

int execute(char * arglist[])
{
	execvp(arglist[0],arglist);
	perror("execvp failed");
	exit(1);
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



