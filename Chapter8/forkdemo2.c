//说明fork的返回值

# include <stdio.h>
# include <unistd.h>


int main(void)
{

	printf("Before:my pid is %d\n",getpid());

	fork(); fork(); fork();

	printf("After:my pid is %d\n",getpid());
	return 0;
}
