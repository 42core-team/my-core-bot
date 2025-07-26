#ifndef BOT_H
#define BOT_H

#include "core_lib.h"

t_pos *find_path(t_pos start, t_pos target, t_obj *unit, int *path_length);

t_obj *ft_get_core_own(void);
t_obj *ft_get_core_opponent(void);
t_obj *ft_get_resource_nearest(t_pos pos);
t_obj *ft_get_money_nearest(t_pos pos);
t_obj *ft_get_resource_money_nearest(t_pos pos);
t_obj *ft_get_units_opponent_nearest(t_pos pos);
t_obj **ft_get_units_own(void);
t_obj **ft_get_units_opponent(void);

#endif /* BOT_H */
