/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdescamp <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 08:35:20 by hdescamp          #+#    #+#             */
/*   Updated: 2025/03/27 08:35:22 by hdescamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	ft_atoi(const char *str)
{
	int	sign;
	int	result;

	sign = 1;
	result = 0;
	while (*str == ' ')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*(str++) == '-')
			sign *= -1;
	}
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
		{
			result *= 10;
			result += (*(str++) - '0');
		}
		else
			return (result * sign);
	}
	return (result * sign);
}

int	check_args(char **argv)
{
	int	i;
	int	j;

	j = 0;
	while (argv[j] != NULL)
	{
		i = 0;
		/*if (argv[j][i] == '\0')
			return (1);*/
		while (argv[j][i] != '\0')
		{
			if (argv[j][i] >= '9' || argv[j][i] <= '0')
				return (1);
			i++;
		}
		j++;
	}
	return (0);
}