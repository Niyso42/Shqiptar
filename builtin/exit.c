#include "../includes/minishell.h"

static int is_numeric(const char *str) {
  int i;

  i = 0;
  if (!str || !str[0])
    return (0);
  if (str[i] == '+' || str[i] == '-')
    i++;
  if (!ft_isdigit(str[i]))
    return (0);
  while (str[i]) {
    if (!ft_isdigit(str[i]))
      return (0);
    i++;
  }
  return (1);
}

static int adder(uint64_t a, uint64_t b, uint64_t *final) {
  uint64_t carry = 0;
  uint64_t limit = 1;

  limit <<= 63;
  while (b != 0) {
    carry = a & b;
    a = a ^ b;
    if (a > limit)
      return 0;
    b = carry << 1;
  }
  *final = a;
  return (1);
};

static int ft_add_and_multiply(int digit, uint64_t *final) {
  uint64_t temp = 0;
  for (int j = 0; j < 10; ++j) {
    if (!adder(temp, *final, &temp)) {
      return 0;
    }
  }
  if (!adder(temp, digit, final)) {
    return 0;
  }
  return 1;
}

static int ft_atoi_no_overflow(char *nptr, int *final) {
  {
    int i;
    int sign;
    uint64_t nb;

    i = 0;
    nb = 0;
    sign = 1;
    while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
      i++;
    if (nptr[i] == '-' || nptr[i] == '+') {
      if (nptr[i] == '-')
        sign *= -1;
      i++;
    }

    while (nptr[i] >= '0' && nptr[i] <= '9') {
      if (!ft_add_and_multiply(nptr[i] - '0', &nb))
        return 0;
      i++;
    }
    if (sign == 1 && nb == ((uint64_t)1 << 63))
      return 0;
    *final = (int64_t)nb;
    *final %= 256;
    if (sign == 1 || !*final) {
      *final = nb;
    } else {
      *final = 256 - *final;
    }
    return (1);
  }
}

static int handle_exit_error(char **args) {
  ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
  ft_putstr_fd(args[0], STDERR_FILENO);
  ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
  return (2);
}

void ft_exit(char **args, t_data *data) {
  int exit_code;

  if (!args || !args[0])
    exit_code = *data->exit->exit;
  else if (!is_numeric(args[0]))
    exit_code = handle_exit_error(args);
  else if (args[1]) {
    ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
    *data->exit->exit = 1;
    return;
  } else {
    if (!ft_atoi_no_overflow(args[0], &exit_code))
      exit_code = handle_exit_error(args);
  }
  // ft_putstr_fd("exit\n", STDOUT_FILENO);
  *data->exit->exit = exit_code;
  data->should_exit = 1;
}
