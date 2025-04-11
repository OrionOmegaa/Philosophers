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

void* thread_task(t_philo_data *data)
{
	int stop_flag;

	stop_flag = 1;
    while (1)
	{
        if (stop_flag == data->number_of_times)
		{
            break;
		}
		stop_flag++;
    }
    return NULL;
}

void	begin_philo()
{
	
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
}

int	main(int argc, char **argv)
{
	t_philo_data data;
	int	i;

	i = check_args(argv);
	if ((argc < 5 || argc > 6) || (i == 1))
	{
		printf("Invalid Argument\n");
		return (1);
	}
	init_philo(argc, argv, &data);
	thread_task(&data);
	return (0);
}
