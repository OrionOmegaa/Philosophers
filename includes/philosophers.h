/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdescamp <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:44:24 by hdescamp          #+#    #+#             */
/*   Updated: 2025/03/26 15:44:27 by hdescamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

//includes

# include <string.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

//define

# define MSG_FORK "%lld %d has taken a fork\n"
# define MSG_EAT "%lld %d is eating\n"
# define MSG_SLEEP "%lld %d is sleeping\n"
# define MSG_THINK "%lld %d is thinking\n"
# define MSG_DEAD "%lld %d died\n"

//struct

typedef struct s_philo_data
{
    int					number_of_philo;
	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;
	int					number_of_times;
	int					i;
	int					stop;
	int* 				id;
	pthread_t* 			philo;
	pthread_mutex_t*	forks;
	pthread_mutex_t		m_number_of_times;
	pthread_mutex_t		m_stop;
	pthread_mutex_t		m_eat_count;
	pthread_mutex_t		m_last_eat;
	pthread_mutex_t		print;
	pthread_mutex_t		*fork_left;
	pthread_mutex_t		*fork_right;
	long long			last_eat;
	long long			start;
}		t_philo_data;

//utils

int			ft_atoi(const char *str);
int			check_args(char **argv);
int			ft_usleep(long long time, t_philo_data *data);
int			my_fork(t_philo_data *data);
int			print_msg(t_philo_data *data, char *msg, int dead);
void		*update_death(t_philo_data *data);
long long	get_time(void);

#endif
