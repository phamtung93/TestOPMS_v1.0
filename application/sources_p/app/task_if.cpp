#include <stdbool.h>
#include <stdint.h>

#include "app.h"
#include "app_if.h"
#include "app_dbg.h"
#include "app_data.h"

#include "task_if.h"
#include "task_list.h"
#include "task_list_if.h"

#include "../ak/fsm.h"
#include "../ak/port.h"
#include "../ak/message.h"
#include "../ak/timer.h"

#include "../common/utils.h"

#include "../sys/sys_dbg.h"
#include "../sys/sys_irq.h"
#include "../sys/sys_io.h"

#if defined(RF24)
static void if_des_type_rf24_handler(ak_msg_t* msg);
#endif

void task_if(ak_msg_t* msg) {
	if (msg->if_des_type == IF_TYPE_RF24_AC ||
			msg->if_des_type == IF_TYPE_RF24_WR ||
			msg->if_des_type == IF_TYPE_RF24_GW) {

#if defined(RF24)
		if_des_type_rf24_handler(msg);
#endif

	}
}
#if defined(RF24)
void if_des_type_rf24_handler(ak_msg_t* msg) {
	switch (msg->sig) {
	case OTB_IF_PURE_MSG_IN: {
		APP_DBG("PT_IF_PURE_MSG_IN\n");
		msg_inc_ref_count(msg);
		set_msg_sig(msg, msg->if_sig);
		set_msg_src_task_id(msg, msg->if_src_task_id);
		task_post(msg->if_des_task_id, msg);
	}
		break;

	case OTB_IF_COMMON_MSG_IN: {
		APP_DBG("PT_IF_COMMON_MSG_IN\n");
		msg_inc_ref_count(msg);
		set_msg_sig(msg, msg->if_sig);
		set_msg_src_task_id(msg, msg->if_src_task_id);
		task_post(msg->if_des_task_id, msg);
	}
		break;

	case OTB_IF_PURE_MSG_OUT: {
		APP_DBG("PT_IF_PURE_MSG_OUT\n");
		msg_inc_ref_count(msg);
		set_msg_sig(msg, OTB_RF24_IF_PURE_MSG_OUT);
		task_post(OTB_TASK_RF24_IF_ID, msg);
	}
		break;

	case OTB_IF_COMMON_MSG_OUT: {
		APP_DBG("PT_IF_COMMON_MSG_OUT\n");
		msg_inc_ref_count(msg);
		set_msg_sig(msg, OTB_RF24_IF_COMMON_MSG_OUT);
		task_post(OTB_TASK_RF24_IF_ID, msg);
	}
		break;

	default:
		break;
	}
}
#endif
