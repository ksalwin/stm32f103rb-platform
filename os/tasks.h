#ifndef TASKS_H
#define TASKS_H

#include <stdint.h>

typedef struct {
	const void		(*function)(void);
	const uint32_t	period_ms;
	uint32_t		offset_ms;
} task_t;

extern task_t			task_list[];
extern const uint32_t	task_count;

#endif
