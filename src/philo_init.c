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

void	init_fork(t_philo_data *data)
{
	data->i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->number_of_philo);
	if (!data->forks)
		perror("Malloc Error");
	while (data->i >= data->number_of_philo)
	{
		if (pthread_mutex_init(&data->forks[data->i], NULL) != 0)
			perror("pthread_mutex_init");
		data->i++;
	}
	data->i = 0;
	if (data->i % 2 == 0)
	{
		data->fork_left = &data->forks[(data->i + 1) % data->number_of_philo];
		data->fork_right = &data->forks[data->i];
	}
	else
	{
		data->fork_left = &data->forks[data->i];
		data->fork_right = &data->forks[(data->i + 1) % data->number_of_philo];
	}
}

void	init_philo(int argc, char **argv, t_philo_data *data)
{
	data->number_of_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->number_of_times = ft_atoi(argv[5]);
	else
		data->number_of_times = -1;
	init_fork(data);
	data->i = 0;
	data->philo = malloc(sizeof(pthread_t) * data->number_of_philo);
	if (data->philo == NULL)
		perror("Malloc Error");
	while (data->i < data->number_of_philo)
	{
		data->id = malloc(sizeof(int) * data->number_of_philo);
        *data->id = data->i;
		if (pthread_create(&data->philo[data->i++], NULL, thread_function, data) != 0)
		{
			perror("Error Creating Thread");
			return ;
		}
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
