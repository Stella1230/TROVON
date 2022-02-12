static void handle_memory_error_event(struct OpalMemoryErrorData *merr_evt)
{
uint64_t paddr_start, paddr_end;
pr_debug("%s: Retrieved memory error event, type: 0x%x\n",
__func__, merr_evt->type);
switch (merr_evt->type) {
case OPAL_MEM_ERR_TYPE_RESILIENCE:
paddr_start = be64_to_cpu(merr_evt->u.resilience.physical_address_start);
paddr_end = be64_to_cpu(merr_evt->u.resilience.physical_address_end);
break;
case OPAL_MEM_ERR_TYPE_DYN_DALLOC:
paddr_start = be64_to_cpu(merr_evt->u.dyn_dealloc.physical_address_start);
paddr_end = be64_to_cpu(merr_evt->u.dyn_dealloc.physical_address_end);
break;
default:
return;
}
for (; paddr_start < paddr_end; paddr_start += PAGE_SIZE) {
memory_failure(paddr_start >> PAGE_SHIFT, 0, 0);
}
}
static void handle_memory_error(void)
{
unsigned long flags;
struct OpalMemoryErrorData *merr_evt;
struct OpalMsgNode *msg_node;
spin_lock_irqsave(&opal_mem_err_lock, flags);
while (!list_empty(&opal_memory_err_list)) {
msg_node = list_entry(opal_memory_err_list.next,
struct OpalMsgNode, list);
list_del(&msg_node->list);
spin_unlock_irqrestore(&opal_mem_err_lock, flags);
merr_evt = (struct OpalMemoryErrorData *)
&msg_node->msg.params[0];
handle_memory_error_event(merr_evt);
kfree(msg_node);
spin_lock_irqsave(&opal_mem_err_lock, flags);
}
spin_unlock_irqrestore(&opal_mem_err_lock, flags);
}
static void mem_error_handler(struct work_struct *work)
{
handle_memory_error();
}
static int opal_memory_err_event(struct notifier_block *nb,
unsigned long msg_type, void *msg)
{
unsigned long flags;
struct OpalMsgNode *msg_node;
if (msg_type != OPAL_MSG_MEM_ERR)
return 0;
msg_node = kzalloc(sizeof(*msg_node), GFP_ATOMIC);
if (!msg_node) {
pr_err("MEMORY_ERROR: out of memory, Opal message event not"
"handled\n");
return -ENOMEM;
}
memcpy(&msg_node->msg, msg, sizeof(struct opal_msg));
spin_lock_irqsave(&opal_mem_err_lock, flags);
list_add(&msg_node->list, &opal_memory_err_list);
spin_unlock_irqrestore(&opal_mem_err_lock, flags);
schedule_work(&mem_error_work);
return 0;
}
static int __init opal_mem_err_init(void)
{
int ret;
if (!opal_mem_err_nb_init) {
ret = opal_message_notifier_register(
OPAL_MSG_MEM_ERR, &opal_mem_err_nb);
if (ret) {
pr_err("%s: Can't register OPAL event notifier (%d)\n",
__func__, ret);
return ret;
}
opal_mem_err_nb_init = 1;
}
return 0;
}
