#include "../include/my_core_bot.h"

void	ft_init_func(void *data);
void	ft_user_loop(void *data);

int	main(int argc, char **argv)
{
	int	won;
	// ft_enable_debug(); // uncomment this to show more debug information in the console when running a game
	ft_init_con(NULL, &argc, argv);
	won = ft_loop(&ft_init_func, &ft_user_loop, NULL);
	ft_close_con();
	if (won)
		return (0);
	return (1);
}

// this function is called once at the start of the game
void	ft_init_func(void *data)
{
	(void)data;

	printf("Init CORE Bot\n");
}

// this function is called every time new data is recieved
void	ft_user_loop(void *data)
{
	(void)data;

	/*
	**  my_units = get all my units
	**  loop trough all of my_units:
	**  	if my_unit of my_units is of type Warrior:
	**		if no enemy unit is left
	**			attack the enemy core
	**		else
	**			attack the closest enemy unit
	**	if my_unit of my_units is of type Worker:
	**		get the nearest resource and mine it
	*/

	printf("Crazy CORE Bot\n");
}
