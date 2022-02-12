static unsigned int
nfqueue_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
const struct xt_NFQ_info *tinfo = par->targinfo;
return NF_QUEUE_NR(tinfo->queuenum);
}
static unsigned int
nfqueue_tg_v1(struct sk_buff *skb, const struct xt_action_param *par)
{
const struct xt_NFQ_info_v1 *info = par->targinfo;
u32 queue = info->queuenum;
if (info->queues_total > 1) {
queue = nfqueue_hash(skb, queue, info->queues_total,
par->family, jhash_initval);
}
return NF_QUEUE_NR(queue);
}
static unsigned int
nfqueue_tg_v2(struct sk_buff *skb, const struct xt_action_param *par)
{
const struct xt_NFQ_info_v2 *info = par->targinfo;
unsigned int ret = nfqueue_tg_v1(skb, par);
if (info->bypass)
ret |= NF_VERDICT_FLAG_QUEUE_BYPASS;
return ret;
}
static int nfqueue_tg_check(const struct xt_tgchk_param *par)
{
const struct xt_NFQ_info_v3 *info = par->targinfo;
u32 maxid;
init_hashrandom(&jhash_initval);
if (info->queues_total == 0) {
pr_err("NFQUEUE: number of total queues is 0\n");
return -EINVAL;
}
maxid = info->queues_total - 1 + info->queuenum;
if (maxid > 0xffff) {
pr_err("NFQUEUE: number of queues (%u) out of range (got %u)\n",
info->queues_total, maxid);
return -ERANGE;
}
if (par->target->revision == 2 && info->flags > 1)
return -EINVAL;
if (par->target->revision == 3 && info->flags & ~NFQ_FLAG_MASK)
return -EINVAL;
return 0;
}
static unsigned int
nfqueue_tg_v3(struct sk_buff *skb, const struct xt_action_param *par)
{
const struct xt_NFQ_info_v3 *info = par->targinfo;
u32 queue = info->queuenum;
int ret;
if (info->queues_total > 1) {
if (info->flags & NFQ_FLAG_CPU_FANOUT) {
int cpu = smp_processor_id();
queue = info->queuenum + cpu % info->queues_total;
} else {
queue = nfqueue_hash(skb, queue, info->queues_total,
par->family, jhash_initval);
}
}
ret = NF_QUEUE_NR(queue);
if (info->flags & NFQ_FLAG_BYPASS)
ret |= NF_VERDICT_FLAG_QUEUE_BYPASS;
return ret;
}
static int __init nfqueue_tg_init(void)
{
return xt_register_targets(nfqueue_tg_reg, ARRAY_SIZE(nfqueue_tg_reg));
}
static void __exit nfqueue_tg_exit(void)
{
xt_unregister_targets(nfqueue_tg_reg, ARRAY_SIZE(nfqueue_tg_reg));
}
