#include "task_list.h"
#include "../ak/timer.h"

task_t app_task_table[] = {
	/*************************************************************************/
	/* SYSTEM TASK */
	/*************************************************************************/
	{TASK_TIMER_TICK_ID		,	TASK_PRI_LEVEL_7	,	task_timer_tick	},

	/*************************************************************************/
	/* APP TASK */
	/*************************************************************************/
	{OTB_TASK_SHELL_ID		,	TASK_PRI_LEVEL_2	,	task_shell			},
	{OTB_TASK_LIFE_ID		,	TASK_PRI_LEVEL_6	,	task_life			},
	{OTB_TASK_IF_ID			,	TASK_PRI_LEVEL_4	,	task_if				},
	{OTB_TASK_RS485_ID		,	TASK_PRI_LEVEL_4	,	task_rs485			},
	/*************************************************************************/
	/* END OF TABLE */
	/*************************************************************************/
	{AK_TASK_EOT_ID,			TASK_PRI_LEVEL_0,		(pf_task)0			}
};
