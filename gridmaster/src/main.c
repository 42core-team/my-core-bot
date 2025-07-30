#include "bot.h"

#include <time.h>
#include <stdio.h>

void ft_on_tick(unsigned long tick);
void ft_on_object_ticked(t_obj *unit, unsigned long tick);

int	main(int argc, char **argv)
{
	return core_startGame("Gridmaster", argc, argv, ft_on_tick, false);
}

int ft_util_distance(t_pos pos1, t_pos pos2)
{
	double x = (double)pos1.x - (double)pos2.x;
	double y = (double)pos1.y - (double)pos2.y;

	if (x < 0)
		x = -x;
	if (y < 0)
		y = -y;

	return ((int)(x + y));
}

int nextUnit = 1;

void ft_on_tick(unsigned long tick)
{
	(void)tick;

	if (ft_get_core_own() && ft_get_core_own()->s_core.balance >= core_get_unitConfig(nextUnit)->cost)
	{
		core_action_createUnit(nextUnit);
		nextUnit++;
		if (nextUnit > 2)
			nextUnit = 0;
	}

	for (int i = 0; game.objects && game.objects[i]; i++)
		ft_on_object_ticked(game.objects[i], tick);
}

void ft_on_object_ticked(t_obj *unit, unsigned long tick)
{
	(void)tick;

	if (unit->state != STATE_ALIVE)
		return;
	if (unit->type != OBJ_UNIT)
		return;
	if (unit->s_unit.team_id != ft_get_core_own()->s_core.team_id)
		return;

	int typeId = unit->s_unit.unit_type;
	if (typeId == UNIT_WARRIOR)
	{
		t_obj * nearestOpponent = ft_get_units_opponent_nearest(ft_get_core_own()->pos);
		if (nearestOpponent)
			move_unit_to(unit, nearestOpponent->pos);
		else
			move_unit_to(unit, ft_get_core_opponent()->pos);
	}
	else if (typeId == UNIT_MINER)
	{
		t_obj * nearestResourceOrMoney = ft_get_resource_money_nearest(unit->pos);
		if (nearestResourceOrMoney)
			move_unit_to(unit, nearestResourceOrMoney->pos);
		else
			move_unit_to(unit, ft_get_core_opponent()->pos);
	}
	else if (typeId == UNIT_CARRIER)
	{
		bool isTouchingCore = ft_util_distance(unit->pos, ft_get_core_own()->pos) <= 1;
		bool isTouchingUnitWithMoney = false;
		t_obj *closestUnitWithMoney = NULL;

		t_obj **units = game.objects;
		double distance = 999999;
		for (int j = 0; units && units[j]; j++)
		{
			if (units[j]->state != STATE_ALIVE || units[j]->type != OBJ_UNIT)
				continue;
			if (units[j]->id == unit->id)
				continue; // dont transfer money from yourself
			if (units[j]->s_unit.balance > 0)
			{
				if (ft_util_distance(unit->pos, units[j]->pos) <= 1)
				{
					isTouchingUnitWithMoney = true;
					closestUnitWithMoney = units[j];
					break;
				}
				if (ft_util_distance(unit->pos, units[j]->pos) < distance)
				{
					closestUnitWithMoney = units[j];
					distance = ft_util_distance(unit->pos, units[j]->pos);
				}
				break;
			}
		}
		if (isTouchingCore && unit->s_unit.balance > 0)
			core_action_transferMoney(unit, ft_get_core_own()->pos, unit->s_unit.balance);
		else if (isTouchingUnitWithMoney)
			core_action_transferMoney(closestUnitWithMoney, unit->pos, closestUnitWithMoney->s_unit.balance);

		if (unit->s_unit.balance <= 0 && closestUnitWithMoney != NULL)
			move_unit_to(unit, closestUnitWithMoney->pos);
		else
			move_unit_to(unit, ft_get_core_own()->pos);
	}
}
