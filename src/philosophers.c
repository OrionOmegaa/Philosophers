/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdescamp <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:41:58 by hdescamp          #+#    #+#             */
/*   Updated: 2025/03/26 15:42:00 by hdescamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void* thread_function(void* argv)
{
	t_philos *philo = (t_philos *)argv;
	
	t_philo_data *data = philo->data;
	pthread_mutex_lock(&philo->m_stop);
    while (!philo->stop)
	{
		pthread_mutex_unlock(&philo->m_stop);
        message(philo, THINK, 0);
		if (philo->id % 2 == 0)
			usleep(150);
		if (forks(philo))
			return (NULL);
		pthread_mutex_lock(&philo->m_eat_count);
		philo->eat_count++;
		pthread_mutex_unlock(&philo->m_eat_count);
		message(philo, SLEEP, 0);
		if (ft_usleep(data->time_to_sleep, philo))
			return (NULL);
		pthread_mutex_lock(&philo->m_stop);
	}
	pthread_mutex_unlock(&philo->m_stop);
    return NULL;
}
void* check_thread(void* argv)
{
	t_philo_data *data;
	int		i;
	int		all_philos_ate;

	data = (t_philo_data *)argv;
	while (1 && (usleep(100) || 1))
	{
		all_philos_ate = 1;
		i = -1;
		while (++i < data->number_of_philo)
		{
			pthread_mutex_lock(&data->philo[i].m_eat_count);
			if (data->philo[i].eat_count < data->number_of_times)
				all_philos_ate = 0;
			pthread_mutex_unlock(&data->philo[i].m_eat_count);
			pthread_mutex_lock(&data->philo[i].m_stop);
			if (data->philo[i].stop
				&& pthread_mutex_unlock(&data->philo[i].m_stop) == 0)
				return (NULL);
			pthread_mutex_unlock(&data->philo[i].m_stop);
		}
		if (all_philos_ate)
			return (death(data));
	}
	return (NULL);
}

void* check_dead(void* argv)
{
	t_philo_data *data = (t_philo_data *)argv;
	int	i;
	
	i = -1;
	while (1 && (usleep(100) || 1))
	{
		if (++i >= data->number_of_philo)
			i = 0;
		pthread_mutex_lock(&data->philo[i].m_last_eat);
		if (get_time() - data->philo[i].last_eat > data->time_to_die)
		{
			pthread_mutex_unlock(&data->philo[i].m_last_eat);
			message(&data->philo[i], DEAD, 1);
			return (death(data));
		}
		pthread_mutex_unlock(&data->philo[i].m_last_eat);
		pthread_mutex_lock(&data->philo[i].m_stop);
		if (data->philo[i].stop)
		{
			pthread_mutex_unlock(&data->philo[i].m_stop);
			return (NULL);
		}
		pthread_mutex_unlock(&data->philo[i].m_stop);
	}
	return (death(data));
}

int	main(int argc, char **argv)
{
	t_philo_data data;
	int i;

	i = -1;
	if ((argc < 5 || argc > 6) || check_args(argv))
	{
		printf("Invalid Argument\n");
		return (1);
	}
	init_philo(argc, argv, &data);
	init_checker_thread(&data);
	while (++i < data.number_of_philo)
		pthread_join(data.philo[i].thread, NULL);
	if (data.number_of_times > 0)
		pthread_join(data.checker, NULL);
	pthread_join(data.dead_checker, NULL);
	philo_free(&data);
	return (0);
}
