#include <unistd.h>

char *ft_strtok(char *str, const char *delim)
{
	int i = 0;
	while (str[i] && str[i] != delim[0])
		i++;
	return(str);
}


// #include<string.h>
// #include<stdio.h>

// int main (void)
// {
// 	char str[] = "HELLO=dsk;fa";
// 	const char *delim = "=";

// 	printf("%s\n", strtok(str, delim));
// 	printf("%s\n", ft_strtok(str, delim));
// 	return (0);	
// }

