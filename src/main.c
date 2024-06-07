#include "/core/con_lib.h"
#include <stdio.h>

void	ft_user_loop(void);

#define WARRIOR_ID 1
#define WORKER_ID 2

int	main(int argc, char **argv)
{
	// ft_enable_debug();
	ft_init_con(&argc, argv);
	ft_loop(&ft_user_loop);
	ft_close_con();
	return (0);
}

int	ft_get_team_worker_count(void)
{
	int	count;
	int	ind;

	count = 0;
	ind = 0;
	while (game.units[ind].id)
	{
		if (game.units[ind].type_id == WORKER_ID && game.units[ind].team_id == game.my_team_id)
			count++;
		ind++;
	}

	return (count);
}

int	ft_get_team_warrior_count(void)
{
	int	count;
	int	ind;

	count = 0;
	ind = 0;
	while (game.units[ind].id)
	{
		if (game.units[ind].type_id == WARRIOR_ID && game.units[ind].team_id == game.my_team_id)
			count++;
		ind++;
	}

	return (count);
}

int	ft_get_enemy_worker_count(void)
{
	int	count;
	int	ind;

	count = 0;
	ind = 0;
	while (game.units[ind].id)
	{
		if (game.units[ind].type_id == WORKER_ID && game.units[ind].team_id != game.my_team_id)
			count++;
		ind++;
	}

	return (count);
}

int	ft_get_enemy_warrior_count(void)
{
	int	count;
	int	ind;

	count = 0;
	ind = 0;
	while (game.units[ind].id)
	{
		if (game.units[ind].type_id == WARRIOR_ID && game.units[ind].team_id != game.my_team_id)
			count++;
		ind++;
	}

	return (count);
}

int	ft_get_resource_count(void)
{
	int	count;
	int	ind;

	count = 0;
	ind = 0;
	while (game.resources[ind].id)
	{
		count++;
		ind++;
	}

	return (count);
}

t_unit	**ft_get_team_workers(void)
{
	int		i = 0;
	int		unit_cnt = 0;
	int		total = ft_get_team_worker_count();
	t_unit	**units;

	i = 0;
	units = malloc(sizeof(t_unit *) * (total + 1));
	if (!units)
		return (NULL);
	units[total] = NULL;
	while (game.units[i].id)
	{
		if (game.units[i].team_id == game.my_team_id
			&& game.units[i].type_id == WORKER_ID)
		{
			units[unit_cnt] = &game.units[i];
			unit_cnt++;
		}
		i++;
	}
	return (units);
}

t_unit	**ft_get_team_warriors(void)
{
	int		i = 0;
	int		unit_cnt = 0;
	int		total = ft_get_team_warrior_count();
	t_unit	**units;

	i = 0;
	units = malloc(sizeof(t_unit *) * (total + 1));
	if (!units)
		return (NULL);
	units[total] = NULL;
	while (game.units[i].id)
	{
		if (game.units[i].team_id == game.my_team_id
			&& game.units[i].type_id == WARRIOR_ID)
		{
			units[unit_cnt] = &game.units[i];
			unit_cnt++;
		}
		i++;
	}
	return (units);
}

t_unit	**ft_get_enemy_workers(void)
{
	int		i = 0;
	int		unit_cnt = 0;
	int		total = ft_get_enemy_worker_count();
	t_unit	**units;

	i = 0;
	units = malloc(sizeof(t_unit *) * (total + 1));
	if (!units)
		return (NULL);
	units[total] = NULL;
	while (game.units[i].id)
	{
		if (game.units[i].team_id != game.my_team_id
			&& game.units[i].type_id == WORKER_ID)
		{
			units[unit_cnt] = &game.units[i];
			unit_cnt++;
		}
		i++;
	}
	return (units);
}

t_unit	**ft_get_enemy_warriors(void)
{
	int		i = 0;
	int		unit_cnt = 0;
	int		total = ft_get_enemy_warrior_count();
	t_unit	**units;

	i = 0;
	units = malloc(sizeof(t_unit *) * (total + 1));
	if (!units)
		return (NULL);
	units[total] = NULL;
	while (game.units[i].id)
	{
		if (game.units[i].team_id != game.my_team_id
			&& game.units[i].type_id == WARRIOR_ID)
		{
			units[unit_cnt] = &game.units[i];
			unit_cnt++;
		}
		i++;
	}
	return (units);
}

int	ft_distance(int x1, int y1, int x2, int y2)
{
	return (abs(x1 - x2) + abs(y1 - y2));
}

t_unit	*ft_get_closest_enemy(t_unit *unit)
{
	int		ind;
	int		min_dist = 1000000;
	t_unit	*closest = NULL;
	t_unit	**enemie_warriors = ft_get_enemy_warriors();
	t_unit	**enemie_workers = ft_get_enemy_workers();

	if (enemie_warriors)
	{
		ind = 0;
		while (enemie_warriors[ind])
		{
			if (ft_distance(unit->x, unit->y, enemie_warriors[ind]->x, enemie_warriors[ind]->y) < min_dist)
			{
				min_dist = ft_distance(unit->x, unit->y, enemie_warriors[ind]->x, enemie_warriors[ind]->y);
				closest = enemie_warriors[ind];
			}
			ind++;
		}
	}
	if (enemie_workers)
	{
		ind = 0;
		while (enemie_workers[ind])
		{
			if (ft_distance(unit->x, unit->y, enemie_workers[ind]->x, enemie_workers[ind]->y) < min_dist)
			{
				min_dist = ft_distance(unit->x, unit->y, enemie_workers[ind]->x, enemie_workers[ind]->y);
				closest = enemie_workers[ind];
			}
			ind++;
		}
	}

	free(enemie_warriors);
	free(enemie_workers);

	return (closest);
}

t_resource	*ft_get_closest_resource(t_unit *unit)
{
	int			ind = 0;
	int			min_dist = 1000000;
	t_resource	*closest = NULL;

	while (game.resources[ind].id)
	{
		if (ft_distance(unit->x, unit->y, game.resources[ind].x, game.resources[ind].y) < min_dist)
		{
			min_dist = ft_distance(unit->x, unit->y, game.resources[ind].x, game.resources[ind].y);
			closest = &game.resources[ind];
		}
		ind++;
	}

	return (closest);
}

t_core	*ft_get_enemy_core(void)
{
	int		ind = 0;
	t_core	*core = NULL;

	while (game.cores[ind].id)
	{
		if (game.cores[ind].team_id != game.my_team_id)
		{
			core = &game.cores[ind];
			break ;
		}
		ind++;
	}

	return (core);
}

void	ft_print_teamname(void)
{
	int	ind = 0;

	while (game.config.teams[ind].id)
	{
		if (game.config.teams[ind].id == game.my_team_id)
		{
			printf("Teamname: %s(%ld)\n", game.config.teams[ind].name, game.config.teams[ind].id);
			break ;
		}
		ind++;
	}
}

/**
 * void ft_travel_to_id(unsigned long id, unsigned long x, unsigned long y);
 * void ft_travel_to(t_unit *unit, unsigned long x, unsigned long y);
 * void ft_travel_dir_id(unsigned long id, long x, long y);
 * void ft_travel_dir(t_unit *unit, long x, long y);
 * void ft_create_type_id(unsigned long type_id);
 * void ft_create(t_unit_config *unit_config);
 * void ft_attack_id(unsigned long attacker_id, unsigned long target_id);
 * void ft_attack_unit(t_unit *attacker, t_unit *target);
 * void ft_attack_resource(t_unit *attacker, t_resource *target);
 * void ft_attack_core(t_unit *attacker, t_core *target);
 *
 * int	ft_get_team_worker_count(void);
 * int	ft_get_team_warrior_count(void);
 * int	ft_get_enemy_worker_count(void);
 * int	ft_get_enemy_warrior_count(void);
 * int	ft_get_resource_count(void);
 * t_unit	**ft_get_team_workers(void);
 * t_unit	**ft_get_team_warriors(void);
 * t_unit	**ft_get_enemy_workers(void);
 * t_unit	**ft_get_enemy_warriors(void);
 * int	ft_distance(int x1, int y1, int x2, int y2);
 * t_unit	*ft_get_closest_enemy(t_unit *unit);
 * t_resource	*ft_get_closest_resource(t_unit *unit);
 * t_core	*ft_get_enemy_core(void);
*/
// this function is called every time new data is recieved
void	ft_user_loop(void)
{
	// ft_print_teamname();
	t_unit	**team_workers = ft_get_team_workers();
	t_unit	**team_warriors = ft_get_team_warriors();

	ft_create_type_id(WARRIOR_ID);
	ft_create_type_id(WORKER_ID);

	int	ind = 0;
	if (team_workers)
	{
		while (team_workers[ind])
		{
			//looping through all workers from your team
			ind++;
		}
	}

	ind = 0;
	if (team_warriors)
	{
		while (team_warriors[ind])
		{
			//looping through all warriors from your team
			ind++;
		}
	}

	free(team_workers);
	free(team_warriors);
}
