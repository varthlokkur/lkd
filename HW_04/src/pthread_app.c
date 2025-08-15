# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_dinner_data {
	int				num_ph;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				num_must_eat;
	long int		begin_time;
}	t_dinner_data;

typedef struct s_philosopher{
	int				num;
	int				count_eat;
	int				*died;
	int				flag;
	int				last_eat;
	int 			eat_all;
	pthread_t		*th;
	pthread_mutex_t	*fork;
	pthread_mutex_t	message;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	t_dinner_data	d_dinner;
}	t_philosopher;

int main(int argc, char **argv)
{
	t_dinner_data	d_dinner;

	if (argc < 5 || argc >6)
	{
		printf("Input 5 or 6 arguments!");
		return (0);
	}
	/*if (check_arg(argv) == -1)
		return (0);
	init_input(argc, argv, &d_dinner);
	// valid input
	if (valid_input(&d_dinner) == -1)
		return (1);
	print(&d_dinner);
	create_phs(&d_dinner);*/
	printf("the end!\n");
	return 0;
}