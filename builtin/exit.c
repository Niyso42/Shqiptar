#include "../includes/minishell.h"

static int is_numeric(const char *str)
{
	int i = 0;

	if (!str || !str[0])
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void ft_exit(char **args, t_data *data)
{
    int exit_code;
    
    ft_putstr_fd("exit\n", STDOUT_FILENO);

    if (!args || !args[0])
        exit_code = *data->exit->exit;
    else if (!is_numeric(args[0]))
    {
        ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
        ft_putstr_fd(args[0], STDERR_FILENO);
        ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
        exit_code = 255;
    }
    else if (args[1])
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        *data->exit->exit = 1;
        return;
    }
    else
    {
        exit_code = ft_atoi(args[0]) % 256;
    }

    // Marquer que le shell doit se terminer
    *data->exit->exit = exit_code;
    data->should_exit = 1;
}
