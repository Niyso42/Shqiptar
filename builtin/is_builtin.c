
#include "../includes/minishell.h"

int is_builtin(char *prompt)
{
    if ((ft_strncmp(prompt, "echo", 4) == 0) && (prompt[4] == ' ' || prompt[4] == '\0'))
        return (1);
    else if ((ft_strncmp(prompt, "pwd", 3) == 0) && (prompt[3] == ' ' || prompt[3] == '\0'))
        return (1);
    else if ((ft_strncmp(prompt, "env", 3) == 0) && (prompt[3] == ' ' || prompt[3] == '\0'))
        return (1);
    return (0);
}

int is_parent_builtin(char *prompt)
{
    if ((ft_strncmp(prompt, "cd", 2) == 0) && (prompt[2] == ' ' || prompt[2] == '\0'))
        return (1);
    else if ((ft_strncmp(prompt, "export", 6) == 0) && (prompt[6] == ' ' || prompt[6] == '\0'))
        return (1);
    else if ((ft_strncmp(prompt, "unset", 5) == 0) && (prompt[5] == ' ' || prompt[5] == '\0'))
        return (1);
    else if ((ft_strncmp(prompt, "exit", 4) == 0) && (prompt[4] == ' ' || prompt[4] == '\0'))
        return (1);
    return (0);
}

void exec_builtin(char **argv, t_data *data)
{
    if (ft_strcmp(argv[0], "env") == 0)
        ft_env(data);
    else if (ft_strcmp(argv[0], "echo") == 0)
        ft_echo(argv);
    else if (ft_strcmp(argv[0], "cd") == 0)
        ft_cd(argv, data);
    else if (ft_strcmp(argv[0], "pwd") == 0)
        ft_pwd(data);
    else if (ft_strcmp(argv[0], "export") == 0)
        ft_export(data, argv);
    else if (ft_strcmp(argv[0], "unset") == 0)
        ft_unset(argv, data);
    else if (ft_strcmp(argv[0], "exit") == 0)
        ft_exit(&argv[1], data);
}