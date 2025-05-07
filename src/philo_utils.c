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

	j = 1;
	while (argv[j] != NULL)
	{
		i = 0;
		if (argv[j][i] == '\0')
			return (1);
		while (argv[j][i] != '\0')
		{
			if (argv[j][i] > '9' || argv[j][i] < '0')
				return (1);
			i++;
		}
		j++;
	}
	return (0);
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	*death(t_philo_data *data)
{
	int	i;

	i = -1;
	pthread_mutex_lock(&data->m_stop);
	if (&data->m_stop)
	{
		pthread_mutex_unlock(&data->m_stop);
		return (NULL);
	}
	pthread_mutex_unlock(&data->m_stop);
	while (++i < data->number_of_philo)
	{
		pthread_mutex_lock(&data->m_stop);
		data->stop = 1;
		pthread_mutex_unlock(&data->m_stop);
	}
	return (NULL);
}

int	ft_usleep(long long time, t_philo_data *data)
{
	long long	wait;
	long long	tmp;

	wait = get_time() + time;
	tmp = get_time();
	while (tmp < wait)
	{
		pthread_mutex_lock(&data->philo[data->i].m_last_eat);
		if (tmp - data->philo[data->i].last_eat - data->time_to_die > 0)
		{
			pthread_mutex_unlock(&data->philo[data->i].m_last_eat);
			message(&data->philo[data->i], DEAD, 1);
			return (1);
		}
		pthread_mutex_unlock(&data->philo[data->i].m_last_eat);
		pthread_mutex_lock(&data->m_stop);
		if (data->stop)
			return (pthread_mutex_unlock(&data->m_stop) + 1);
		pthread_mutex_unlock(&data->m_stop);
		usleep(1000);
		tmp = get_time();
	}
	return (0);
}
