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

int	message(t_philos *philo, char *msg, int dead)
{
	t_philo_data *data = philo->data;
	long timestamp;

	pthread_mutex_lock(&data->message);
	pthread_mutex_lock(&data->m_stop);
	if (data->stop && !dead)
	{
		pthread_mutex_unlock(&data->m_stop);
		pthread_mutex_unlock(&data->message);
		return (1);
	}
	timestamp = get_time() - data->start;
	if (dead)
		data->stop = 1;
	pthread_mutex_unlock(&data->m_stop);
	printf(msg, timestamp, philo->id);
	pthread_mutex_unlock(&data->message);
	return (0);
}

int	message2(t_philos *philo, char *msg, int dead)
{
	t_philo_data *data = philo->data;

	pthread_mutex_lock(&data->message);
	pthread_mutex_lock(&data->m_stop);
	if (data->stop)
	{
		pthread_mutex_unlock(&data->message);
		pthread_mutex_unlock(&data->m_stop);
		return (1);
	}
	pthread_mutex_unlock(&data->m_stop);
	printf(msg, get_time() - data->start, philo->id);
	if (dead)
	{
		death(data);
		pthread_mutex_unlock(&data->message);
		return (1);
	}
	pthread_mutex_unlock(&data->message);
	return (0);
}

int	fork_choice(t_philos *philo)
{
	if (pthread_mutex_lock(philo->fork_right) != 0)
		return (1);
	message(philo, FORK, 0);
	if (philo->fork_left == philo->fork_right
		|| pthread_mutex_lock(philo->fork_left) != 0)
	{
		pthread_mutex_unlock(philo->fork_right);
		return (1);
	}
	message(philo, FORK, 0);
	return (0);
}

int	forks(t_philos *philo)
{
	if (fork_choice(philo))
		return (1);
	message(philo, EAT, 0);
	pthread_mutex_lock(&philo->m_last_eat);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->m_last_eat);
	if (ft_usleep(philo->data->time_to_eat, philo->data))
	{
		pthread_mutex_unlock(philo->fork_left);
		pthread_mutex_unlock(philo->fork_right);
		return (1);
	}
	pthread_mutex_unlock(philo->fork_left);
	pthread_mutex_unlock(philo->fork_right);
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
		pthread_mutex_destroy(&data->philo[i].m_last_eat);
		pthread_mutex_destroy(&data->m_stop);
		i++;
	}
	i = 0;
	free(data->forks);
	free(data->philo);
	pthread_mutex_destroy(&data->print);
}
