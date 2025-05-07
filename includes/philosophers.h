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

# define FORK "AT %lld PHILOSOPHER %d TOOK A FORK\n"
# define EAT "AT %lld PHILOSOPHER %d IS EATING\n"
# define SLEEP "AT %lld PHILOSOPHER %d IS SLEEPING\n"
# define THINK "AT %lld PHILOSOPHER %d IS THINKING\n"
# define DEAD "AT %lld PHILOSOPHER %d DIED\n"

//struct

struct s_philo_data;

typedef struct s_philos
{
    int              		 	id;
	long long					last_eat;
	pthread_t	 				thread;
	pthread_mutex_t				m_last_eat;
    pthread_mutex_t    			*fork_left;
    pthread_mutex_t     		*fork_right;
    struct s_philo_data			*data;
}   t_philos;

typedef struct s_philo_data
{
	int					number_of_philo;
	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;
	int					number_of_times;
	int					i;
	int					stop;
	int					must_eat;
	int					eat_count;
	t_philos			*philo;
	pthread_t			*philos;
	pthread_t 			checker;
	pthread_t 			dead_checker;
	pthread_mutex_t		*forks;
	pthread_mutex_t		m_number_of_times;
	pthread_mutex_t		m_stop;
	pthread_mutex_t		m_eat_count;
	pthread_mutex_t		print;
	long long			start;
}		t_philo_data;

//utils

int			ft_atoi(const char *str);
int			check_args(char **argv);
int			ft_usleep(long long time, t_philo_data *data);
int			forks(t_philo_data *data);
int			message(t_philos *philo, char *msg, int dead);
int			init_checker_thread(t_philo_data *data);
void		init_philo(int argc, char **argv, t_philo_data *data);
void		*death(t_philo_data *data);
void		philo_free(t_philo_data *data);
void* 		check_thread(void* argv);
void*		check_dead(void* argv);
void* 		thread_function(void* argv);
long long	get_time(void);

#endif
