/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdescamp <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:14:52 by hdescamp          #+#    #+#             */
/*   Updated: 2025/05/05 16:14:55 by hdescamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	message(t_philo_data *data, char *msg, int dead)
{
	pthread_mutex_lock(&data->print);
	pthread_mutex_lock(&data->m_stop);
	if (data->stop)
	{
		pthread_mutex_unlock(&data->print);
		pthread_mutex_unlock(&data->m_stop);
		return (1);
	}
	pthread_mutex_unlock(&data->m_stop);
	printf(msg, get_time() - data->start, data->id + 1);
	if (dead)
	{
		death(data);
		pthread_mutex_unlock(&data->print);
		return (1);
	}
	pthread_mutex_unlock(&data->print);
	return (0);
}

int	fork_choice(t_philo_data *data)
{
	if (pthread_mutex_lock(data->fork_right) != 0)
		return (1);
	message(data, FORK, 0);
	if (data->fork_left == data->fork_right
		|| pthread_mutex_lock(data->fork_left) != 0)
	{
		pthread_mutex_unlock(data->fork_right);
		return (1);
	}
	message(data, FORK, 0);
	return (0);
}

int	forks(t_philo_data *data)
{
	if (fork_choice(data))
		return (1);
	message(data, EAT, 0);
	pthread_mutex_lock(&data->m_last_eat);
	data->last_eat = get_time();
	pthread_mutex_unlock(&data->m_last_eat);
	if (ft_usleep(data->time_to_eat, data))
	{
		pthread_mutex_unlock(data->fork_left);
		pthread_mutex_unlock(data->fork_right);
		return (1);
	}
	pthread_mutex_unlock(data->fork_left);
	pthread_mutex_unlock(data->fork_right);
	return (0);
}

void	philo_free(t_philo_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philo && data->forks)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->m_eat_count);
		pthread_mutex_destroy(&data->m_last_eat);
		pthread_mutex_destroy(&data->m_stop);
		i++;
	}
	i = 0;
	free(data->forks);
	free(data->philo);
	pthread_mutex_destroy(&data->print);
}
