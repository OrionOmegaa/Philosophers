/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdescamp <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:29:11 by hdescamp          #+#    #+#             */
/*   Updated: 2025/05/06 14:29:23 by hdescamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void init_fork(t_philo_data *data)
{
	int	i;

    i = 0;
    while (i < data->number_of_philo)
    {
        if (i % 2 == 0)
        {
            data->philo[i].fork_left = &data->forks[(i + 1) % data->number_of_philo];
            data->philo[i].fork_right = &data->forks[i];
        }
        else
        {
            data->philo[i].fork_left = &data->forks[i];
            data->philo[i].fork_right = &data->forks[(i + 1) % data->number_of_philo];
        }
        i++;
    }
}

void	init_mutex(t_philo_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_init(&data->m_stop, NULL);
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->m_eat_count, NULL);
	pthread_mutex_init(&data->m_number_of_times, NULL);
	while (i < data->number_of_philo)
	{
		data->philo[i].id = i + 1;
		data->philo[i].data = data;
		data->philo[i].last_eat = data->start;
		pthread_mutex_init(&data->philo[i].m_last_eat, NULL);
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
}

void	init_philo(int argc, char **argv, t_philo_data *data)
{
	int	i;

	i = 0;
	data->number_of_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->number_of_times = ft_atoi(argv[5]);
	else
		data->number_of_times = -1;
	data->start = get_time();
	data->stop = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->number_of_philo);
	data->philo = malloc(sizeof(t_philos) * data->number_of_philo);
	init_mutex(data);
	init_fork(data);
	while (i < data->number_of_philo)
	{
		pthread_create(&data->philo[i].thread, NULL, thread_function, &data->philo[i]);
		usleep(10);
		i++;
	}
}

int	init_checker_thread(t_philo_data *data)
{
	if (data->must_eat != -1 && pthread_create(&data->checker, NULL,
			check_thread, data) != 0)
	{
		printf("Error While creating checker thread\n");
		philo_free(data);
		return (1);
	}
	if (pthread_create(&data->dead_checker, NULL, check_dead, data) != 0)
	{
		printf("Error While creating checker thread\n");
		philo_free(data);
		return (1);
	}
	return (0);
}
