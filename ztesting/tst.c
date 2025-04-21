


#include "../includes/minishell.h"


int main()
{

    chdir(getenv("HOME"));
    printf ("%s\n",getcwd(NULL, 0));

    // setenv("OLDPWD", getenv("HOME"), 1);

}