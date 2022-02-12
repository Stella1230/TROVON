static bool
ebt_ip6_mt(const struct sk_buff *skb, struct xt_action_param *par)
{
const struct ebt_ip6_info *info = par->matchinfo;
const struct ipv6hdr *ih6;
struct ipv6hdr _ip6h;
const union pkthdr *pptr;
union pkthdr _pkthdr;
ih6 = skb_header_pointer(skb, 0, sizeof(_ip6h), &_ip6h);
if (ih6 == NULL)
return false;
if (info->bitmask & EBT_IP6_TCLASS &&
FWINV(info->tclass != ipv6_get_dsfield(ih6), EBT_IP6_TCLASS))
return false;
if ((info->bitmask & EBT_IP6_SOURCE &&
FWINV(ipv6_masked_addr_cmp(&ih6->saddr, &info->smsk,
&info->saddr), EBT_IP6_SOURCE)) ||
(info->bitmask & EBT_IP6_DEST &&
FWINV(ipv6_masked_addr_cmp(&ih6->daddr, &info->dmsk,
&info->daddr), EBT_IP6_DEST)))
return false;
if (info->bitmask & EBT_IP6_PROTO) {
uint8_t nexthdr = ih6->nexthdr;
__be16 frag_off;
int offset_ph;
offset_ph = ipv6_skip_exthdr(skb, sizeof(_ip6h), &nexthdr, &frag_off);
if (offset_ph == -1)
return false;
if (FWINV(info->protocol != nexthdr, EBT_IP6_PROTO))
return false;
if (!(info->bitmask & ( EBT_IP6_DPORT |
EBT_IP6_SPORT | EBT_IP6_ICMP6)))
return true;
pptr = skb_header_pointer(skb, offset_ph, sizeof(_pkthdr),
&_pkthdr);
if (pptr == NULL)
return false;
if (info->bitmask & EBT_IP6_DPORT) {
u16 dst = ntohs(pptr->tcpudphdr.dst);
if (FWINV(dst < info->dport[0] ||
dst > info->dport[1], EBT_IP6_DPORT))
return false;
}
if (info->bitmask & EBT_IP6_SPORT) {
u16 src = ntohs(pptr->tcpudphdr.src);
if (FWINV(src < info->sport[0] ||
src > info->sport[1], EBT_IP6_SPORT))
return false;
}
if ((info->bitmask & EBT_IP6_ICMP6) &&
FWINV(pptr->icmphdr.type < info->icmpv6_type[0] ||
pptr->icmphdr.type > info->icmpv6_type[1] ||
pptr->icmphdr.code < info->icmpv6_code[0] ||
pptr->icmphdr.code > info->icmpv6_code[1],
EBT_IP6_ICMP6))
return false;
}
return true;
}
static int ebt_ip6_mt_check(const struct xt_mtchk_param *par)
{
const struct ebt_entry *e = par->entryinfo;
struct ebt_ip6_info *info = par->matchinfo;
if (e->ethproto != htons(ETH_P_IPV6) || e->invflags & EBT_IPROTO)
return -EINVAL;
if (info->bitmask & ~EBT_IP6_MASK || info->invflags & ~EBT_IP6_MASK)
return -EINVAL;
if (info->bitmask & (EBT_IP6_DPORT | EBT_IP6_SPORT)) {
if (info->invflags & EBT_IP6_PROTO)
return -EINVAL;
if (info->protocol != IPPROTO_TCP &&
info->protocol != IPPROTO_UDP &&
info->protocol != IPPROTO_UDPLITE &&
info->protocol != IPPROTO_SCTP &&
info->protocol != IPPROTO_DCCP)
return -EINVAL;
}
if (info->bitmask & EBT_IP6_DPORT && info->dport[0] > info->dport[1])
return -EINVAL;
if (info->bitmask & EBT_IP6_SPORT && info->sport[0] > info->sport[1])
return -EINVAL;
if (info->bitmask & EBT_IP6_ICMP6) {
if ((info->invflags & EBT_IP6_PROTO) ||
info->protocol != IPPROTO_ICMPV6)
return -EINVAL;
if (info->icmpv6_type[0] > info->icmpv6_type[1] ||
info->icmpv6_code[0] > info->icmpv6_code[1])
return -EINVAL;
}
return 0;
}
static int __init ebt_ip6_init(void)
{
return xt_register_match(&ebt_ip6_mt_reg);
}
static void __exit ebt_ip6_fini(void)
{
xt_unregister_match(&ebt_ip6_mt_reg);
}
