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

int	print_msg(t_philo_data *data, char *msg, int dead)
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
		update_death(data);
		pthread_mutex_unlock(&data->print);
		return (1);
	}
	pthread_mutex_unlock(&data->print);
	return (0);
}

int	choose_fork(t_philo_data *data)
{
	if (pthread_mutex_lock(data->fork_right) != 0)
		return (1);
	print_msg(data, MSG_FORK, 0);
	if (data->fork_left == data->fork_right
		|| pthread_mutex_lock(data->fork_left) != 0)
	{
		pthread_mutex_unlock(data->fork_right);
		return (1);
	}
	print_msg(data, MSG_FORK, 0);
	return (0);
}

int	my_fork(t_philo_data *data)
{
	if (choose_fork(data))
		return (1);
	print_msg(data, MSG_EAT, 0);
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