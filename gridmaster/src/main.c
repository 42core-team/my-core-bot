#include "bot.h"

#include <time.h>
#include <stdio.h>

void ft_on_tick(unsigned long tick);

int	main(int argc, char **argv)
{
	return core_startGame("Gridmaster", argc, argv, ft_on_tick, false);
}

void move_and_attack(t_obj *unit, t_pos target_pos)
{
	int dx = target_pos.x - unit->pos.x;
	int dy = target_pos.y - unit->pos.y;

	t_pos next_pos = { unit->pos.x, unit->pos.y };
	if (abs(dx) > abs(dy))
	{
		int step = (dx > 0) ? 1 : -1;
		next_pos.x += step;
	}
	else
	{
		int step = (dy > 0) ? 1 : -1;
		next_pos.y += step;
	}

	t_obj *next_pos_obj = core_get_obj_from_pos(next_pos);
	if (next_pos_obj)
	{
		if (next_pos_obj->type == OBJ_MONEY)
			core_action_move(unit, next_pos);
		else
			core_action_attack(unit, next_pos);
	}
	else
	{
		core_action_move(unit, next_pos);
	}
}

int target_unit = 1;

void ft_on_tick(unsigned long tick)
{
	(void)tick;

	// spawn new unit
	if (ft_get_core_own() && ft_get_core_own()->s_core.balance >= core_get_unitConfig(target_unit)->cost)
	{
		core_action_createUnit(target_unit);
		target_unit++;
		if (target_unit > 1)
			target_unit = 0;
	}

	// move units
	t_obj **units = ft_get_units_own();
	for (int i = 0; units && units[i]; i++)
	{
		t_obj *obj = units[i];
		if (obj->state != STATE_ALIVE)
			continue;

		switch ((int)obj->s_unit.unit_type)
		{
			case UNIT_WARRIOR:
				t_obj *closest_opponent = ft_get_units_opponent_nearest(ft_get_core_own()->pos);
				if (closest_opponent)
					move_and_attack(obj, closest_opponent->pos);
				else
					move_and_attack(obj, ft_get_core_opponent()->pos);
				break;

			case UNIT_MINER:
				t_obj *nearest_resource_or_money = ft_get_resource_money_nearest(obj->pos);
				if (nearest_resource_or_money && obj->s_unit.balance <= 0)
					move_and_attack(obj, nearest_resource_or_money->pos);
				else
				{
					move_and_attack(obj, ft_get_core_own()->pos);
					core_action_transferMoney(obj, ft_get_core_own()->pos, 9999999);
				}
				break;
		}
	}
	free(units);
}
