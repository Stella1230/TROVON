static int
dissect_winsrepl_start(tvbuff_t *winsrepl_tvb, _U_ packet_info *pinfo,
int winsrepl_offset, proto_tree *winsrepl_tree)
{
proto_tree *start_tree;
start_tree = proto_tree_add_subtree(winsrepl_tree, winsrepl_tvb, winsrepl_offset, -1,
ett_winsrepl_start, NULL, "WREPL_START_ASSOCIATION");
proto_tree_add_item(start_tree, hf_winsrepl_assoc_ctx, winsrepl_tvb, winsrepl_offset, 4, ENC_BIG_ENDIAN);
winsrepl_offset += 4;
proto_tree_add_item(start_tree, hf_winsrepl_start_minor_version, winsrepl_tvb, winsrepl_offset, 2, ENC_BIG_ENDIAN);
winsrepl_offset += 2;
proto_tree_add_item(start_tree, hf_winsrepl_start_major_version, winsrepl_tvb, winsrepl_offset, 2, ENC_BIG_ENDIAN);
winsrepl_offset += 2;
return winsrepl_offset;
}
static int
dissect_winsrepl_stop(tvbuff_t *winsrepl_tvb, _U_ packet_info *pinfo,
int winsrepl_offset, proto_tree *winsrepl_tree)
{
guint32 reason;
proto_item *stop_item;
proto_tree *stop_tree;
stop_tree = proto_tree_add_subtree(winsrepl_tree, winsrepl_tvb, winsrepl_offset, -1,
ett_winsrepl_stop, &stop_item, "WREPL_STOP_ASSOCIATION");
reason = tvb_get_ntohl(winsrepl_tvb, winsrepl_offset);
proto_tree_add_uint(stop_tree, hf_winsrepl_stop_reason, winsrepl_tvb, winsrepl_offset, 4, reason);
winsrepl_offset += 4;
proto_item_append_text(stop_item, ", Reason: 0x%08X", reason);
return winsrepl_offset;
}
static int
dissect_winsrepl_table_query(tvbuff_t *winsrepl_tvb _U_, packet_info *pinfo _U_,
int winsrepl_offset, proto_tree *winsrepl_tree _U_)
{
return winsrepl_offset;
}
static int
dissect_winsrepl_wins_owner(tvbuff_t *winsrepl_tvb, _U_ packet_info *pinfo,
int winsrepl_offset, proto_tree *winsrepl_tree,
proto_tree *sub_tree, guint32 idx)
{
proto_tree *owner_tree = NULL;
if (sub_tree) {
owner_tree = proto_tree_add_subtree_format(sub_tree, winsrepl_tvb, winsrepl_offset, 24,
ett_winsrepl_owner, NULL, "WINS Owner [%u]", idx);
} else if (winsrepl_tree) {
owner_tree = proto_tree_add_subtree(winsrepl_tree, winsrepl_tvb, winsrepl_offset, 24,
ett_winsrepl_owner, NULL, "WINS Owner");
}
proto_tree_add_item(owner_tree, hf_winsrepl_owner_address, winsrepl_tvb, winsrepl_offset, 4, ENC_BIG_ENDIAN);
winsrepl_offset += 4;
proto_tree_add_item(owner_tree, hf_winsrepl_owner_max_version, winsrepl_tvb, winsrepl_offset, 8, ENC_BIG_ENDIAN);
winsrepl_offset += 8;
proto_tree_add_item(owner_tree, hf_winsrepl_owner_min_version, winsrepl_tvb, winsrepl_offset, 8, ENC_BIG_ENDIAN);
winsrepl_offset += 8;
proto_tree_add_item(owner_tree, hf_winsrepl_owner_type, winsrepl_tvb, winsrepl_offset, 4, ENC_BIG_ENDIAN);
winsrepl_offset += 4;
return winsrepl_offset;
}
static int
dissect_winsrepl_table_reply(tvbuff_t *winsrepl_tvb, packet_info *pinfo,
int winsrepl_offset, proto_tree *winsrepl_tree)
{
proto_tree *table_tree;
guint32 partner_count;
guint32 i;
table_tree = proto_tree_add_subtree(winsrepl_tree, winsrepl_tvb, winsrepl_offset, -1,
ett_winsrepl_table_reply, NULL, "WREPL_REPL_TABLE_REPLY");
partner_count = tvb_get_ntohl(winsrepl_tvb, winsrepl_offset);
proto_tree_add_uint(table_tree, hf_winsrepl_table_partner_count, winsrepl_tvb, winsrepl_offset, 4, partner_count);
winsrepl_offset += 4;
for (i=0; i < partner_count; i++) {
winsrepl_offset = dissect_winsrepl_wins_owner(winsrepl_tvb, pinfo,
winsrepl_offset, table_tree,
table_tree, i);
}
proto_tree_add_item(table_tree, hf_winsrepl_table_initiator, winsrepl_tvb, winsrepl_offset, 4, ENC_BIG_ENDIAN);
winsrepl_offset += 4;
return winsrepl_offset;
}
static int
dissect_winsrepl_send_request(tvbuff_t *winsrepl_tvb, packet_info *pinfo,
int winsrepl_offset, proto_tree *winsrepl_tree)
{
winsrepl_offset = dissect_winsrepl_wins_owner(winsrepl_tvb, pinfo,
winsrepl_offset, winsrepl_tree,
NULL, 0);
return winsrepl_offset;
}
static int
dissect_winsrepl_wins_ip(tvbuff_t *winsrepl_tvb, _U_ packet_info *pinfo,
int winsrepl_offset, proto_tree *winsrepl_tree,
guint32 *addr, proto_tree *sub_tree, guint32 idx)
{
proto_item *ip_item = NULL;
proto_tree *ip_tree = NULL;
if (sub_tree) {
ip_tree = proto_tree_add_subtree_format(sub_tree, winsrepl_tvb, winsrepl_offset, 8,
ett_winsrepl_ip, &ip_item, "WINS IP [%u]", idx);
} else if (winsrepl_tree) {
ip_tree = proto_tree_add_subtree(winsrepl_tree, winsrepl_tvb, winsrepl_offset, 8,
ett_winsrepl_ip, &ip_item, "WINS IP");
}
proto_tree_add_item(ip_tree, hf_winsrepl_ip_owner, winsrepl_tvb, winsrepl_offset, 4, ENC_BIG_ENDIAN);
winsrepl_offset += 4;
*addr = tvb_get_ipv4(winsrepl_tvb, winsrepl_offset);
proto_tree_add_ipv4(ip_tree, hf_winsrepl_ip_ip, winsrepl_tvb, winsrepl_offset, 4, *addr);
proto_item_append_text(ip_item, ": %s", ip_to_str((guint8 *)addr));
winsrepl_offset += 4;
return winsrepl_offset;
}
static int
dissect_winsrepl_wins_address_list(tvbuff_t *winsrepl_tvb, packet_info *pinfo,
int winsrepl_offset, proto_tree *winsrepl_tree,
proto_item *parent_item)
{
proto_item *addr_list_item;
proto_tree *addr_list_tree;
int old_offset = winsrepl_offset;
guint32 num_ips;
guint32 ip;
guint32 i;
addr_list_tree = proto_tree_add_subtree(winsrepl_tree, winsrepl_tvb, winsrepl_offset, -1,
ett_winsrepl_addr_list, &addr_list_item, "WINS Address List");
num_ips = tvb_get_letohl(winsrepl_tvb, winsrepl_offset);
proto_tree_add_uint(addr_list_tree, hf_winsrepl_addr_list_num_ips, winsrepl_tvb, winsrepl_offset, 4, num_ips);
winsrepl_offset += 4;
for (i=0; i < num_ips; i++) {
winsrepl_offset = dissect_winsrepl_wins_ip(winsrepl_tvb, pinfo,
winsrepl_offset, addr_list_tree,
&ip, addr_list_tree, i);
if (i == 0) {
proto_item_append_text(parent_item, ": %s", ip_to_str((guint8 *)&ip));
proto_item_append_text(addr_list_item, ": %s", ip_to_str((guint8 *)&ip));
} else {
proto_item_append_text(parent_item, ", %s", ip_to_str((guint8 *)&ip));
proto_item_append_text(addr_list_item, ", %s", ip_to_str((guint8 *)&ip));
}
}
proto_item_set_len(addr_list_item, winsrepl_offset - old_offset);
return winsrepl_offset;
}
static int
dissect_winsrepl_wins_name(tvbuff_t *winsrepl_tvb, packet_info *pinfo,
int winsrepl_offset, proto_tree *winsrepl_tree,
proto_tree *sub_tree, guint32 idx)
{
proto_item *name_item = NULL, *ti;
proto_tree *name_tree = NULL;
proto_item *flags_item;
proto_tree *flags_tree;
int old_offset = winsrepl_offset;
tvbuff_t *name_tvb = NULL;
guint32 name_len;
char name_str[(NETBIOS_NAME_LEN - 1)*4 + 1];
int name_type;
guint32 flags;
guint32 addr;
if (sub_tree) {
name_tree = proto_tree_add_subtree_format(sub_tree, winsrepl_tvb, winsrepl_offset, -1,
ett_winsrepl_name, &name_item, "WINS Name [%u]", idx);
} else if (winsrepl_tree) {
name_tree = proto_tree_add_subtree(winsrepl_tree, winsrepl_tvb, winsrepl_offset, -1,
ett_winsrepl_name, &name_item, "WINS Name");
}
name_len = tvb_get_ntohl(winsrepl_tvb, winsrepl_offset);
ti = proto_tree_add_uint(name_tree, hf_winsrepl_name_len, winsrepl_tvb, winsrepl_offset, 4, name_len);
if ((gint) name_len < 1) {
expert_add_info(pinfo, ti, &ei_winsrepl_name_len);
THROW(ReportedBoundsError);
}
winsrepl_offset += 4;
name_tvb = tvb_new_subset_length(winsrepl_tvb, winsrepl_offset, name_len);
netbios_add_name("Name", name_tvb, 0, name_tree);
name_type = get_netbios_name(name_tvb, 0, name_str, (NETBIOS_NAME_LEN - 1)*4 + 1);
proto_item_append_text(name_item, ": %s<%02x>", name_str, name_type);
winsrepl_offset += name_len;
winsrepl_offset += 4 - (winsrepl_offset & (4-1));
flags = tvb_get_ntohl(winsrepl_tvb, winsrepl_offset);
flags_item = proto_tree_add_uint(name_tree, hf_winsrepl_name_flags, winsrepl_tvb, winsrepl_offset, 4, flags);
flags_tree = proto_item_add_subtree(flags_item, ett_winsrepl_flags);
proto_tree_add_uint(flags_tree, hf_winsrepl_name_flags_rectype, winsrepl_tvb, winsrepl_offset, 4, flags);
proto_tree_add_uint(flags_tree, hf_winsrepl_name_flags_recstate, winsrepl_tvb, winsrepl_offset, 4, flags);
proto_tree_add_boolean(flags_tree, hf_winsrepl_name_flags_local, winsrepl_tvb, winsrepl_offset, 4, flags);
proto_tree_add_uint(flags_tree, hf_winsrepl_name_flags_hosttype, winsrepl_tvb, winsrepl_offset, 4, flags);
proto_tree_add_boolean(flags_tree, hf_winsrepl_name_flags_static, winsrepl_tvb, winsrepl_offset, 4, flags);
winsrepl_offset += 4;
proto_tree_add_item(name_tree, hf_winsrepl_name_group_flag, winsrepl_tvb, winsrepl_offset, 4, ENC_LITTLE_ENDIAN);
winsrepl_offset += 4;
proto_tree_add_item(name_tree, hf_winsrepl_name_version_id, winsrepl_tvb, winsrepl_offset, 8, ENC_BIG_ENDIAN);
winsrepl_offset += 8;
switch (flags & WREPL_NAME_TYPE_MASK) {
case WREPL_NAME_TYPE_UNIQUE:
case WREPL_NAME_TYPE_NORMAL_GROUP:
addr = tvb_get_ipv4(winsrepl_tvb, winsrepl_offset);
proto_tree_add_ipv4(name_tree, hf_winsrepl_ip_ip, winsrepl_tvb, winsrepl_offset, 4, addr);
proto_item_append_text(name_item, ": %s", ip_to_str((guint8 *)&addr));
winsrepl_offset += 4;
break;
case WREPL_NAME_TYPE_SPECIAL_GROUP:
case WREPL_NAME_TYPE_MULTIHOMED:
winsrepl_offset = dissect_winsrepl_wins_address_list(winsrepl_tvb, pinfo,
winsrepl_offset, name_tree,
name_item);
break;
}
proto_tree_add_item(name_tree, hf_winsrepl_name_unknown, winsrepl_tvb, winsrepl_offset, 4, ENC_BIG_ENDIAN);
winsrepl_offset += 4;
proto_item_set_len(name_item, winsrepl_offset - old_offset);
return winsrepl_offset;
}
static int
dissect_winsrepl_send_reply(tvbuff_t *winsrepl_tvb, packet_info *pinfo,
int winsrepl_offset, proto_tree *winsrepl_tree)
{
proto_tree *rep_tree;
guint32 num_names;
guint32 i;
rep_tree = proto_tree_add_subtree(winsrepl_tree, winsrepl_tvb, winsrepl_offset, -1,
ett_winsrepl_send_reply, NULL, "WREPL_REPL_SEND_REPLY");
num_names = tvb_get_ntohl(winsrepl_tvb, winsrepl_offset);
proto_tree_add_uint(rep_tree, hf_winsrepl_reply_num_names, winsrepl_tvb, winsrepl_offset, 4, num_names);
winsrepl_offset += 4;
for (i=0; i < num_names; i++) {
winsrepl_offset = dissect_winsrepl_wins_name(winsrepl_tvb, pinfo,
winsrepl_offset, rep_tree,
rep_tree, i);
}
return winsrepl_offset;
}
static int
dissect_winsrepl_update(tvbuff_t *winsrepl_tvb, packet_info *pinfo,
int winsrepl_offset, proto_tree *winsrepl_tree)
{
winsrepl_offset = dissect_winsrepl_table_reply(winsrepl_tvb, pinfo,
winsrepl_offset, winsrepl_tree);
return winsrepl_offset;
}
static int
dissect_winsrepl_update2(tvbuff_t *winsrepl_tvb, packet_info *pinfo,
int winsrepl_offset, proto_tree *winsrepl_tree)
{
winsrepl_offset = dissect_winsrepl_table_reply(winsrepl_tvb, pinfo,
winsrepl_offset, winsrepl_tree);
return winsrepl_offset;
}
static int
dissect_winsrepl_inform(tvbuff_t *winsrepl_tvb, packet_info *pinfo,
int winsrepl_offset, proto_tree *winsrepl_tree)
{
winsrepl_offset = dissect_winsrepl_table_reply(winsrepl_tvb, pinfo,
winsrepl_offset, winsrepl_tree);
return winsrepl_offset;
}
static int
dissect_winsrepl_inform2(tvbuff_t *winsrepl_tvb, packet_info *pinfo,
int winsrepl_offset, proto_tree *winsrepl_tree)
{
winsrepl_offset = dissect_winsrepl_table_reply(winsrepl_tvb, pinfo,
winsrepl_offset, winsrepl_tree);
return winsrepl_offset;
}
static int
dissect_winsrepl_replication(tvbuff_t *winsrepl_tvb, packet_info *pinfo,
int winsrepl_offset, proto_item *winsrepl_item, proto_tree *winsrepl_tree)
{
proto_item *repl_item;
proto_tree *repl_tree;
enum wrepl_replication_cmd command;
repl_tree = proto_tree_add_subtree(winsrepl_tree, winsrepl_tvb, winsrepl_offset, -1,
ett_winsrepl_replication, &repl_item, "WREPL_REPLICATION");
command = (enum wrepl_replication_cmd)tvb_get_ntohl(winsrepl_tvb, winsrepl_offset);
proto_tree_add_uint(repl_tree, hf_winsrepl_replication_command, winsrepl_tvb, winsrepl_offset, 4, command);
winsrepl_offset += 4;
switch (command) {
case WREPL_REPL_TABLE_QUERY:
col_set_str(pinfo->cinfo, COL_INFO, "WREPL_REPL_TABLE_QUERY");
proto_item_append_text(winsrepl_item, ", WREPL_REPL_TABLE_QUERY");
proto_item_append_text(repl_item, ", WREPL_REPL_TABLE_QUERY");
winsrepl_offset = dissect_winsrepl_table_query(winsrepl_tvb, pinfo,
winsrepl_offset, repl_tree);
break;
case WREPL_REPL_TABLE_REPLY:
col_set_str(pinfo->cinfo, COL_INFO, "WREPL_REPL_TABLE_REPLY");
proto_item_append_text(winsrepl_item, ", WREPL_REPL_TABLE_REPLY");
proto_item_append_text(repl_item, ", WREPL_REPL_TABLE_REPLY");
winsrepl_offset = dissect_winsrepl_table_reply(winsrepl_tvb, pinfo,
winsrepl_offset, repl_tree);
break;
case WREPL_REPL_SEND_REQUEST:
col_set_str(pinfo->cinfo, COL_INFO, "WREPL_REPL_SEND_REQUEST");
proto_item_append_text(winsrepl_item, ", WREPL_REPL_SEND_REQUEST");
proto_item_append_text(repl_item, ", WREPL_REPL_SEND_REQUEST");
winsrepl_offset = dissect_winsrepl_send_request(winsrepl_tvb, pinfo,
winsrepl_offset, repl_tree);
break;
case WREPL_REPL_SEND_REPLY:
col_set_str(pinfo->cinfo, COL_INFO, "WREPL_REPL_SEND_REPLY");
proto_item_append_text(winsrepl_item, ", WREPL_REPL_SEND_REPLY");
proto_item_append_text(repl_item, ", WREPL_REPL_SEND_REPLY");
winsrepl_offset = dissect_winsrepl_send_reply(winsrepl_tvb, pinfo,
winsrepl_offset, repl_tree);
break;
case WREPL_REPL_UPDATE:
col_set_str(pinfo->cinfo, COL_INFO, "WREPL_REPL_UPDATE");
proto_item_append_text(winsrepl_item, ", WREPL_REPL_UPDATE");
proto_item_append_text(repl_item, ", WREPL_REPL_UPDATE");
winsrepl_offset = dissect_winsrepl_update(winsrepl_tvb, pinfo,
winsrepl_offset, repl_tree);
break;
case WREPL_REPL_UPDATE2:
col_set_str(pinfo->cinfo, COL_INFO, "WREPL_REPL_UPDATE2");
proto_item_append_text(winsrepl_item, ",WREPL_REPL_UPDATE2");
proto_item_append_text(repl_item, ",WREPL_REPL_UPDATE2");
winsrepl_offset = dissect_winsrepl_update2(winsrepl_tvb, pinfo,
winsrepl_offset, repl_tree);
break;
case WREPL_REPL_INFORM:
col_set_str(pinfo->cinfo, COL_INFO, "WREPL_REPL_INFORM");
proto_item_append_text(winsrepl_item, ", WREPL_REPL_INFORM");
proto_item_append_text(repl_item, ", WREPL_REPL_INFORM");
winsrepl_offset = dissect_winsrepl_inform(winsrepl_tvb, pinfo,
winsrepl_offset, repl_tree);
break;
case WREPL_REPL_INFORM2:
col_set_str(pinfo->cinfo, COL_INFO, "WREPL_REPL_INFORM2");
proto_item_append_text(winsrepl_item, ", WREPL_REPL_INFORM2");
proto_item_append_text(repl_item, ", WREPL_REPL_INFORM2");
winsrepl_offset = dissect_winsrepl_inform2(winsrepl_tvb, pinfo,
winsrepl_offset, repl_tree);
break;
}
return winsrepl_offset;
}
static int
dissect_winsrepl_pdu(tvbuff_t *tvb, packet_info *pinfo, proto_tree *parent_tree, void* data _U_)
{
int offset = 0;
proto_item *winsrepl_item;
proto_tree *winsrepl_tree;
enum wrepl_mess_type mess_type;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "WINS-Replication");
col_clear(pinfo->cinfo, COL_INFO);
winsrepl_item = proto_tree_add_item(parent_tree, proto_winsrepl, tvb, offset, -1, ENC_NA);
winsrepl_tree = proto_item_add_subtree(winsrepl_item, ett_winsrepl);
proto_tree_add_item(winsrepl_tree, hf_winsrepl_size, tvb, offset, 4, ENC_BIG_ENDIAN);
offset += 4;
proto_tree_add_item(winsrepl_tree, hf_winsrepl_opcode, tvb, offset, 4, ENC_BIG_ENDIAN);
offset += 4;
proto_tree_add_item(winsrepl_tree, hf_winsrepl_assoc_ctx, tvb, offset, 4, ENC_BIG_ENDIAN);
offset += 4;
mess_type = (enum wrepl_mess_type)tvb_get_ntohl(tvb, offset);
proto_tree_add_uint(winsrepl_tree, hf_winsrepl_mess_type, tvb, offset, 4, mess_type);
offset += 4;
switch (mess_type) {
case WREPL_START_ASSOCIATION:
col_set_str(pinfo->cinfo, COL_INFO, "WREPL_START_ASSOCIATION");
proto_item_append_text(winsrepl_item, ", WREPL_START_ASSOCIATION");
dissect_winsrepl_start(tvb, pinfo,
offset, winsrepl_tree);
break;
case WREPL_START_ASSOCIATION_REPLY:
col_set_str(pinfo->cinfo, COL_INFO, "WREPL_START_ASSOCIATION_REPLY");
proto_item_append_text(winsrepl_item, ", WREPL_START_ASSOCIATION_REPLY");
dissect_winsrepl_start(tvb, pinfo,
offset, winsrepl_tree);
break;
case WREPL_STOP_ASSOCIATION:
col_set_str(pinfo->cinfo, COL_INFO, "WREPL_STOP_ASSOCIATION");
proto_item_append_text(winsrepl_item, ", WREPL_STOP_ASSOCIATION");
dissect_winsrepl_stop(tvb, pinfo,
offset, winsrepl_tree);
break;
case WREPL_REPLICATION:
dissect_winsrepl_replication(tvb, pinfo,
offset, winsrepl_item, winsrepl_tree);
break;
}
return tvb_length(tvb);
}
static guint
get_winsrepl_pdu_len(packet_info *pinfo _U_, tvbuff_t *tvb, int offset)
{
guint pdu_len;
pdu_len=tvb_get_ntohl(tvb, offset);
return pdu_len+4;
}
static int
dissect_winsrepl(tvbuff_t *tvb, packet_info *pinfo, proto_tree *parent_tree, void* data)
{
tcp_dissect_pdus(tvb, pinfo, parent_tree, winsrepl_reassemble, 4, get_winsrepl_pdu_len, dissect_winsrepl_pdu, data);
return tvb_length(tvb);
}
void
proto_register_winsrepl(void)
{
static hf_register_info hf[] = {
{ &hf_winsrepl_size, {
"Packet Size", "winsrepl.size",
FT_UINT32, BASE_DEC, NULL, 0x0,
"WINS Replication Packet Size", HFILL }},
{ &hf_winsrepl_opcode, {
"Opcode", "winsrepl.opcode",
FT_UINT32, BASE_HEX, NULL, 0x0,
"WINS Replication Opcode", HFILL }},
{ &hf_winsrepl_assoc_ctx, {
"Assoc_Ctx", "winsrepl.assoc_ctx",
FT_UINT32, BASE_HEX, NULL, 0x0,
"WINS Replication Assoc_Ctx", HFILL }},
{ &hf_winsrepl_mess_type, {
"Message_Type", "winsrepl.message_type",
FT_UINT32, BASE_DEC, VALS(message_type_vals), 0x0,
"WINS Replication Message_Type", HFILL }},
{ &hf_winsrepl_start_minor_version, {
"Minor Version", "winsrepl.minor_version",
FT_UINT16, BASE_DEC, NULL, 0x0,
"WINS Replication Minor Version", HFILL }},
{ &hf_winsrepl_start_major_version, {
"Major Version", "winsrepl.major_version",
FT_UINT16, BASE_DEC, NULL, 0x0,
"WINS Replication Major Version", HFILL }},
{ &hf_winsrepl_stop_reason, {
"Reason", "winsrepl.reason",
FT_UINT32, BASE_HEX, NULL, 0x0,
"WINS Replication Reason", HFILL }},
{ &hf_winsrepl_replication_command, {
"Replication Command", "winsrepl.repl_cmd",
FT_UINT32, BASE_HEX, VALS(replication_cmd_vals), 0x0,
"WINS Replication Command", HFILL }},
{ &hf_winsrepl_owner_address, {
"Owner Address", "winsrepl.owner_address",
FT_IPv4, BASE_NONE, NULL, 0x0,
"WINS Replication Owner Address", HFILL }},
{ &hf_winsrepl_owner_max_version, {
"Max Version", "winsrepl.max_version",
FT_UINT64, BASE_DEC, NULL, 0x0,
"WINS Replication Max Version", HFILL }},
{ &hf_winsrepl_owner_min_version, {
"Min Version", "winsrepl.min_version",
FT_UINT64, BASE_DEC, NULL, 0x0,
"WINS Replication Min Version", HFILL }},
{ &hf_winsrepl_owner_type, {
"Owner Type", "winsrepl.owner_type",
FT_UINT32, BASE_DEC, NULL, 0x0,
"WINS Replication Owner Type", HFILL }},
{ &hf_winsrepl_table_partner_count, {
"Partner Count", "winsrepl.partner_count",
FT_UINT32, BASE_DEC, NULL, 0x0,
"WINS Replication Partner Count", HFILL }},
{ &hf_winsrepl_table_initiator, {
"Initiator", "winsrepl.initiator",
FT_IPv4, BASE_NONE, NULL, 0x0,
"WINS Replication Initiator", HFILL }},
{ &hf_winsrepl_ip_owner, {
"IP Owner", "winsrepl.ip_owner",
FT_IPv4, BASE_NONE, NULL, 0x0,
"WINS Replication IP Owner", HFILL }},
{ &hf_winsrepl_ip_ip, {
"IP Address", "winsrepl.ip_address",
FT_IPv4, BASE_NONE, NULL, 0x0,
"WINS Replication IP Address", HFILL }},
{ &hf_winsrepl_addr_list_num_ips, {
"Num IPs", "winsrepl.num_ips",
FT_UINT32, BASE_DEC, NULL, 0x0,
"WINS Replication Num IPs", HFILL }},
{ &hf_winsrepl_name_len, {
"Name Len", "winsrepl.name_len",
FT_UINT32, BASE_DEC, NULL, 0x0,
"WINS Replication Name Len", HFILL }},
{ &hf_winsrepl_name_flags, {
"Name Flags", "winsrepl.name_flags",
FT_UINT32, BASE_HEX, NULL, 0x0,
"WINS Replication Name Flags", HFILL }},
{ &hf_winsrepl_name_flags_rectype, {
"Record Type", "winsrepl.name_flags.rectype",
FT_UINT32, BASE_HEX, VALS(rectype_vals), 0x00000003,
"WINS Replication Name Flags Record Type", HFILL }},
{ &hf_winsrepl_name_flags_recstate, {
"Record State", "winsrepl.name_flags.recstate",
FT_UINT32, BASE_HEX, VALS(recstate_vals), 0x0000000C,
"WINS Replication Name Flags Record State", HFILL }},
{ &hf_winsrepl_name_flags_local, {
"Local", "winsrepl.name_flags.local",
FT_BOOLEAN, 32, NULL, 0x00000010,
"WINS Replication Name Flags Local Flag", HFILL }},
{ &hf_winsrepl_name_flags_hosttype, {
"Host Type", "winsrepl.name_flags.hosttype",
FT_UINT32, BASE_HEX, VALS(hosttype_vals), 0x00000060,
"WINS Replication Name Flags Host Type", HFILL }},
{ &hf_winsrepl_name_flags_static, {
"Static", "winsrepl.name_flags.static",
FT_BOOLEAN, 32, NULL, 0x00000080,
"WINS Replication Name Flags Static Flag", HFILL }},
{ &hf_winsrepl_name_group_flag, {
"Name Group Flag", "winsrepl.name_group_flag",
FT_UINT32, BASE_HEX, NULL, 0x0,
"WINS Replication Name Group Flag", HFILL }},
{ &hf_winsrepl_name_version_id, {
"Name Version Id", "winsrepl.name_version_id",
FT_UINT64, BASE_DEC, NULL, 0x0,
"WINS Replication Name Version Id", HFILL }},
{ &hf_winsrepl_name_unknown, {
"Unknown IP", "winsrepl.unknown",
FT_IPv4, BASE_NONE, NULL, 0x0,
"WINS Replication Unknown IP", HFILL }},
{ &hf_winsrepl_reply_num_names, {
"Num Names", "winsrepl.num_names",
FT_UINT32, BASE_DEC, NULL, 0x0,
"WINS Replication Num Names", HFILL }},
};
static gint *ett[] = {
&ett_winsrepl,
&ett_winsrepl_start,
&ett_winsrepl_stop,
&ett_winsrepl_replication,
&ett_winsrepl_owner,
&ett_winsrepl_table_reply,
&ett_winsrepl_ip,
&ett_winsrepl_addr_list,
&ett_winsrepl_name,
&ett_winsrepl_send_reply,
&ett_winsrepl_flags,
};
static ei_register_info ei[] = {
{ &ei_winsrepl_name_len, { "winsrepl.name_len.invalid", PI_MALFORMED, PI_ERROR, "Bad name length", EXPFILL }},
};
module_t *winsrepl_module;
expert_module_t* expert_winsrepl;
proto_winsrepl = proto_register_protocol("WINS (Windows Internet Name Service) Replication",
"WINS-Replication", "winsrepl");
proto_register_subtree_array(ett, array_length(ett));
proto_register_field_array(proto_winsrepl, hf, array_length(hf));
expert_winsrepl = expert_register_protocol(proto_winsrepl);
expert_register_field_array(expert_winsrepl, ei, array_length(ei));
winsrepl_module = prefs_register_protocol(proto_winsrepl, NULL);
prefs_register_bool_preference(winsrepl_module, "reassemble",
"Reassemble WINS-Replication messages spanning multiple TCP segments",
"Whether the WINS-Replication dissector should reassemble messages spanning multiple TCP segments."
" To use this option, you must also enable \"Allow subdissectors to reassemble TCP streams\" in the TCP protocol settings.",
&winsrepl_reassemble);
}
void
proto_reg_handoff_winsrepl(void)
{
dissector_handle_t winsrepl_handle;
winsrepl_handle = new_create_dissector_handle(dissect_winsrepl, proto_winsrepl);
dissector_add_uint("tcp.port", glb_winsrepl_tcp_port, winsrepl_handle);
}
