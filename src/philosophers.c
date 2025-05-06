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
}

void* thread_function(void* argv)
{
	t_philo_data *data;
	
	data = (t_philo_data *)argv;

	pthread_mutex_lock(&data->m_stop);
    while (!data->stop)
	{
		pthread_mutex_unlock(&data->m_stop);
        print_msg(data, MSG_THINK, 0);
		if (*data->id % 2 == 0)
			usleep(150);
		if (my_fork(data))
			return (NULL);
		pthread_mutex_lock(&data->m_number_of_times);
		data->number_of_times++;
		pthread_mutex_unlock(&data->m_number_of_times);
		print_msg(data, MSG_SLEEP, 0);
		if (ft_usleep(data->time_to_sleep, data))
			return (NULL);
		pthread_mutex_lock(&data->m_stop);
	}
	pthread_mutex_unlock(&data->m_stop);
    return NULL;
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
	while (data->i <= data->number_of_philo)
	{
		data->id = malloc(sizeof(int) * data->number_of_philo);
        *data->id = data->i + 1;
		if (pthread_create(&data->philo[data->i++], NULL, thread_function, data) != 0)
			perror("Error Creating Thread");
		usleep(10);
	}
}

int	main(int argc, char **argv)
{
	t_philo_data data;
	int i;

	i = 0;
	if ((argc < 5 || argc > 6) || check_args(argv))
	{
		printf("Invalid Argument\n");
		return (1);
	}
	init_philo(argc, argv, &data);
	while (i != data.number_of_philo)
		pthread_join(data.philo[i], NULL);
	return (0);
}
