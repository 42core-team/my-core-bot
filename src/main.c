#include "../include/my_core_bot.h"

void	ft_user_loop(void);

int	main(int argc, char **argv)
{
	// ft_enable_debug(); // uncomment this to show more debug information in the console when running a game
	ft_init_con(&argc, argv);
	ft_loop(&ft_user_loop);
	ft_close_con();
	return (0);
}

// this function is called every time new data is recieved
void	ft_user_loop(void)
{
	printf("Crazy CORE Bot\n");
}
