static const char *
nbns_type_name (int type)
{
switch (type) {
case T_NB:
return "NB";
case T_NBSTAT:
return "NBSTAT";
}
return "unknown";
}
static proto_tree *
add_rr_to_tree(proto_item *trr, int rr_type, tvbuff_t *tvb, int offset,
const char *name, int namelen,
const char *type_name, const char *class_description,
guint ttl, gushort data_len)
{
proto_tree *rr_tree;
rr_tree = proto_item_add_subtree(trr, rr_type);
proto_tree_add_text(rr_tree, tvb, offset+1, namelen-1, "Name: %s", name);
offset += namelen;
proto_tree_add_text(rr_tree, tvb, offset, 2, "Type: %s", type_name);
offset += 2;
proto_tree_add_text(rr_tree, tvb, offset, 2, "Class: %s", class_description);
offset += 2;
proto_tree_add_text(rr_tree, tvb, offset, 4, "Time to live: %s",
time_secs_to_str(ttl));
offset += 4;
proto_tree_add_text(rr_tree, tvb, offset, 2, "Data length: %u", data_len);
return rr_tree;
}
static int
get_nbns_name(tvbuff_t *tvb, int offset, int nbns_data_offset,
char *name_ret, int name_ret_len, int *name_type_ret)
{
int name_len;
const guchar *name;
const guchar *nbname;
char *nbname_buf;
const guchar *pname;
char cname, cnbname;
int name_type;
char *pname_ret;
size_t idx = 0;
nbname_buf=ep_alloc(NBNAME_BUF_LEN);
nbname = nbname_buf;
name_len = get_dns_name(tvb, offset, 0, nbns_data_offset, &name);
pname = &name[0];
pname_ret=name_ret;
for (;;) {
cname = *pname;
if (cname == '\0')
break;
if (cname == '.')
break;
if (cname < 'A' || cname > 'Z') {
nbname="Illegal NetBIOS name (1st character not between A and Z in first-level encoding)";
goto bad;
}
cname -= 'A';
cnbname = cname << 4;
pname++;
cname = *pname;
if (cname == '\0' || cname == '.') {
nbname="Illegal NetBIOS name (odd number of bytes)";
goto bad;
}
if (cname < 'A' || cname > 'Z') {
nbname="Illegal NetBIOS name (2nd character not between A and Z in first-level encoding)";
goto bad;
}
cname -= 'A';
cnbname |= cname;
pname++;
if (idx < NETBIOS_NAME_LEN) {
nbname_buf[idx++] = cnbname;
}
}
if (idx != NETBIOS_NAME_LEN) {
g_snprintf(nbname_buf, NBNAME_BUF_LEN, "Illegal NetBIOS name (%lu bytes long)",
(unsigned long)idx);
goto bad;
}
name_type = process_netbios_name(nbname, name_ret, name_ret_len);
pname_ret += MIN(strlen(name_ret), (size_t) name_ret_len);
pname_ret += MIN(name_ret_len-(pname_ret-name_ret),
g_snprintf(pname_ret, name_ret_len-(gulong)(pname_ret-name_ret), "<%02x>", name_type));
if (cname == '.') {
g_snprintf(pname_ret, name_ret_len-(gulong)(pname_ret-name_ret), "%s", pname);
}
if (name_type_ret != NULL)
*name_type_ret = name_type;
return name_len;
bad:
if (name_type_ret != NULL)
*name_type_ret = -1;
g_snprintf(pname_ret, name_ret_len-(gulong)(pname_ret-name_ret), "%s", nbname);
return name_len;
}
static int
get_nbns_name_type_class(tvbuff_t *tvb, int offset, int nbns_data_offset,
char *name_ret, int *name_len_ret, int *name_type_ret,
int *type_ret, int *class_ret)
{
int name_len;
int type;
int class;
name_len = get_nbns_name(tvb, offset, nbns_data_offset, name_ret,
*name_len_ret, name_type_ret);
offset += name_len;
type = tvb_get_ntohs(tvb, offset);
offset += 2;
class = tvb_get_ntohs(tvb, offset);
*type_ret = type;
*class_ret = class;
*name_len_ret = name_len;
return name_len + 4;
}
static void
add_name_and_type(proto_tree *tree, tvbuff_t *tvb, int offset, int len,
const char *tag, const char *name, int name_type)
{
if (name_type != -1) {
proto_tree_add_text(tree, tvb, offset, len, "%s: %s (%s)",
tag, name, netbios_name_type_descr(name_type));
} else {
proto_tree_add_text(tree, tvb, offset, len, "%s: %s",
tag, name);
}
}
static int
dissect_nbns_query(tvbuff_t *tvb, int offset, int nbns_data_offset,
column_info *cinfo, proto_tree *nbns_tree)
{
int len;
char *name;
int name_len;
int name_type;
int type;
int class;
const char *type_name;
int data_offset;
int data_start;
proto_tree *q_tree;
proto_item *tq;
name=ep_alloc(MAX_NAME_LEN);
data_start = data_offset = offset;
name_len=MAX_NAME_LEN;
len = get_nbns_name_type_class(tvb, offset, nbns_data_offset, name,
&name_len, &name_type, &type, &class);
data_offset += len;
type_name = nbns_type_name(type);
if (cinfo != NULL)
col_append_fstr(cinfo, COL_INFO, " %s %s", type_name, name);
if (nbns_tree != NULL) {
tq = proto_tree_add_text(nbns_tree, tvb, offset, len,
"%s: type %s, class %s", name, type_name,
dns_class_name(class));
q_tree = proto_item_add_subtree(tq, ett_nbns_qd);
add_name_and_type(q_tree, tvb, offset, name_len, "Name", name,
name_type);
offset += name_len;
proto_tree_add_text(q_tree, tvb, offset, 2, "Type: %s", type_name);
offset += 2;
proto_tree_add_text(q_tree, tvb, offset, 2, "Class: %s",
dns_class_name(class));
}
return data_offset - data_start;
}
static void
nbns_add_nbns_flags(column_info *cinfo, proto_tree *nbns_tree, tvbuff_t *tvb, int offset,
gushort flags, int is_wack)
{
char *buf;
guint16 opcode;
proto_tree *field_tree;
proto_item *tf;
#define MAX_BUF_SIZE (128+1)
buf=ep_alloc(MAX_BUF_SIZE);
opcode = (guint16) ((flags & F_OPCODE) >> OPCODE_SHIFT);
g_snprintf(buf, MAX_BUF_SIZE, "%s", val_to_str(opcode, opcode_vals, "Unknown operation"));
if (flags & F_RESPONSE && !is_wack) {
g_strlcat(buf, " response", MAX_BUF_SIZE);
g_strlcat(buf, ", ", MAX_BUF_SIZE);
g_strlcat(buf, val_to_str(flags & F_RCODE, rcode_vals, "Unknown error"), MAX_BUF_SIZE);
buf[MAX_BUF_SIZE-1] = '\0';
if ((flags & F_RCODE))
col_append_fstr(cinfo, COL_INFO, ", %s",
val_to_str(flags & F_RCODE, rcode_vals,
"Unknown error"));
}
tf = proto_tree_add_uint_format(nbns_tree, hf_nbns_flags,
tvb, offset, 2, flags, "Flags: 0x%04x (%s)", flags, buf);
field_tree = proto_item_add_subtree(tf, ett_nbns_flags);
proto_tree_add_item(field_tree, hf_nbns_flags_response,
tvb, offset, 2, ENC_BIG_ENDIAN);
proto_tree_add_item(field_tree, hf_nbns_flags_opcode,
tvb, offset, 2, ENC_BIG_ENDIAN);
if (flags & F_RESPONSE) {
proto_tree_add_item(field_tree, hf_nbns_flags_authoritative,
tvb, offset, 2, ENC_BIG_ENDIAN);
}
proto_tree_add_item(field_tree, hf_nbns_flags_truncated,
tvb, offset, 2, ENC_BIG_ENDIAN);
proto_tree_add_item(field_tree, hf_nbns_flags_recdesired,
tvb, offset, 2, ENC_BIG_ENDIAN);
if (flags & F_RESPONSE) {
proto_tree_add_item(field_tree, hf_nbns_flags_recavail,
tvb, offset, 2, ENC_BIG_ENDIAN);
}
proto_tree_add_item(field_tree, hf_nbns_flags_broadcast,
tvb, offset, 2, ENC_BIG_ENDIAN);
if (flags & F_RESPONSE && !is_wack) {
proto_tree_add_item(field_tree, hf_nbns_flags_rcode,
tvb, offset, 2, ENC_BIG_ENDIAN);
}
}
static void
nbns_add_nb_flags(proto_tree *rr_tree, tvbuff_t *tvb, int offset, gushort flags)
{
char *buf;
proto_tree *field_tree;
proto_item *tf;
static const value_string nb_flags_ont_vals[] = {
{ NB_FLAGS_ONT_B_NODE, "B-node" },
{ NB_FLAGS_ONT_P_NODE, "P-node" },
{ NB_FLAGS_ONT_M_NODE, "M-node" },
{ NB_FLAGS_ONT_H_NODE, "H-node" },
{ 0, NULL }
};
buf=ep_alloc(MAX_BUF_SIZE);
g_snprintf(buf, MAX_BUF_SIZE, "%s", val_to_str(flags & NB_FLAGS_ONT, nb_flags_ont_vals,
"Unknown"));
g_strlcat(buf, ", ", MAX_BUF_SIZE);
if (flags & NB_FLAGS_G)
g_strlcat(buf, "group", MAX_BUF_SIZE);
else
g_strlcat(buf, "unique", MAX_BUF_SIZE);
buf[MAX_BUF_SIZE-1] = '\0';
tf = proto_tree_add_text(rr_tree, tvb, offset, 2, "Flags: 0x%x (%s)", flags,
buf);
field_tree = proto_item_add_subtree(tf, ett_nbns_nb_flags);
proto_tree_add_text(field_tree, tvb, offset, 2, "%s",
decode_boolean_bitfield(flags, NB_FLAGS_G,
2*8,
"Group name",
"Unique name"));
proto_tree_add_text(field_tree, tvb, offset, 2, "%s",
decode_enumerated_bitfield(flags, NB_FLAGS_ONT,
2*8, nb_flags_ont_vals, "%s"));
}
static void
nbns_add_name_flags(proto_tree *rr_tree, tvbuff_t *tvb, int offset,
gushort flags)
{
char *buf;
proto_item *field_tree;
proto_item *tf;
static const value_string name_flags_ont_vals[] = {
{ NAME_FLAGS_ONT_B_NODE, "B-node" },
{ NAME_FLAGS_ONT_P_NODE, "P-node" },
{ NAME_FLAGS_ONT_M_NODE, "M-node" },
{ 0, NULL }
};
buf=ep_alloc(MAX_BUF_SIZE);
g_snprintf(buf, MAX_BUF_SIZE, "%s", val_to_str(flags & NAME_FLAGS_ONT, name_flags_ont_vals,
"Unknown"));
g_strlcat(buf, ", ", MAX_BUF_SIZE);
if (flags & NAME_FLAGS_G)
g_strlcat(buf, "group", MAX_BUF_SIZE);
else
g_strlcat(buf, "unique", MAX_BUF_SIZE);
if (flags & NAME_FLAGS_DRG)
g_strlcat(buf, ", being deregistered", MAX_BUF_SIZE);
if (flags & NAME_FLAGS_CNF)
g_strlcat(buf, ", in conflict", MAX_BUF_SIZE);
if (flags & NAME_FLAGS_ACT)
g_strlcat(buf, ", active", MAX_BUF_SIZE);
if (flags & NAME_FLAGS_PRM)
g_strlcat(buf, ", permanent node name", MAX_BUF_SIZE);
buf[MAX_BUF_SIZE-1] = '\0';
tf = proto_tree_add_text(rr_tree, tvb, offset, 2, "Name flags: 0x%x (%s)",
flags, buf);
field_tree = proto_item_add_subtree(tf, ett_nbns_name_flags);
proto_tree_add_text(field_tree, tvb, offset, 2, "%s",
decode_boolean_bitfield(flags, NAME_FLAGS_G,
2*8,
"Group name",
"Unique name"));
proto_tree_add_text(field_tree, tvb, offset, 2, "%s",
decode_enumerated_bitfield(flags, NAME_FLAGS_ONT,
2*8, name_flags_ont_vals, "%s"));
proto_tree_add_text(field_tree, tvb, offset, 2, "%s",
decode_boolean_bitfield(flags, NAME_FLAGS_DRG,
2*8,
"Name is being deregistered",
"Name is not being deregistered"));
proto_tree_add_text(field_tree, tvb, offset, 2, "%s",
decode_boolean_bitfield(flags, NAME_FLAGS_CNF,
2*8,
"Name is in conflict",
"Name is not in conflict"));
proto_tree_add_text(field_tree, tvb, offset, 2, "%s",
decode_boolean_bitfield(flags, NAME_FLAGS_ACT,
2*8,
"Name is active",
"Name is not active"));
proto_tree_add_text(field_tree, tvb, offset, 2, "%s",
decode_boolean_bitfield(flags, NAME_FLAGS_PRM,
2*8,
"Permanent node name",
"Not permanent node name"));
}
static int
dissect_nbns_answer(tvbuff_t *tvb, int offset, int nbns_data_offset,
column_info *cinfo, proto_tree *nbns_tree, int opcode)
{
int len;
char *name;
int name_len;
int name_type;
int type;
int class;
const char *class_name;
const char *type_name;
int data_offset;
int cur_offset;
int data_start;
guint ttl;
gushort data_len;
gushort flags;
proto_tree *rr_tree;
proto_item *trr;
char *name_str;
guint num_names;
char *nbname;
gushort name_flags;
data_start = data_offset = offset;
cur_offset = offset;
name=ep_alloc(MAX_NAME_LEN);
name_str=ep_alloc(MAX_NAME_LEN);
nbname=ep_alloc(16+4+1);
name_len=MAX_NAME_LEN;
len = get_nbns_name_type_class(tvb, offset, nbns_data_offset, name,
&name_len, &name_type, &type, &class);
data_offset += len;
cur_offset += len;
type_name = nbns_type_name(type);
class_name = dns_class_name(class);
ttl = tvb_get_ntohl(tvb, data_offset);
data_offset += 4;
cur_offset += 4;
data_len = tvb_get_ntohs(tvb, data_offset);
data_offset += 2;
cur_offset += 2;
switch (type) {
case T_NB:
if (cinfo != NULL) {
if (opcode != OPCODE_WACK) {
col_append_fstr(cinfo, COL_INFO, " %s %s",
type_name,
tvb_ip_to_str(tvb, data_offset+2));
}
}
if (nbns_tree == NULL)
break;
trr = proto_tree_add_text(nbns_tree, tvb, offset,
(data_offset - data_start) + data_len,
"%s: type %s, class %s",
name, type_name, class_name);
g_strlcat(name, " (", MAX_NAME_LEN);
g_strlcat(name, netbios_name_type_descr(name_type), MAX_NAME_LEN);
g_strlcat(name, ")", MAX_NAME_LEN);
rr_tree = add_rr_to_tree(trr, ett_nbns_rr, tvb, offset, name,
name_len, type_name, dns_class_name(class), ttl, data_len);
while (data_len > 0) {
if (opcode == OPCODE_WACK) {
if (data_len < 2) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
flags = tvb_get_ntohs(tvb, cur_offset);
nbns_add_nbns_flags(cinfo, rr_tree, tvb, cur_offset,
flags, 1);
cur_offset += 2;
data_len -= 2;
} else {
if (data_len < 2) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
flags = tvb_get_ntohs(tvb, cur_offset);
nbns_add_nb_flags(rr_tree, tvb, cur_offset,
flags);
cur_offset += 2;
data_len -= 2;
if (data_len < 4) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 4,
"Addr: %s",
tvb_ip_to_str(tvb, cur_offset));
cur_offset += 4;
data_len -= 4;
}
}
break;
case T_NBSTAT:
if (cinfo != NULL)
col_append_fstr(cinfo, COL_INFO, " %s", type_name);
if (nbns_tree == NULL)
break;
trr = proto_tree_add_text(nbns_tree, tvb, offset,
(data_offset - data_start) + data_len,
"%s: type %s, class %s",
name, type_name, class_name);
rr_tree = add_rr_to_tree(trr, ett_nbns_rr, tvb, offset, name,
name_len, type_name, dns_class_name(class), ttl, data_len);
if (data_len < 1) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
num_names = tvb_get_guint8(tvb, cur_offset);
proto_tree_add_text(rr_tree, tvb, cur_offset, 1,
"Number of names: %u", num_names);
cur_offset += 1;
while (num_names != 0) {
if (data_len < NETBIOS_NAME_LEN) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
goto out;
}
tvb_memcpy(tvb, (guint8 *)nbname, cur_offset,
NETBIOS_NAME_LEN);
name_type = process_netbios_name(nbname,
name_str, name_len);
proto_tree_add_text(rr_tree, tvb, cur_offset,
NETBIOS_NAME_LEN, "Name: %s<%02x> (%s)",
name_str, name_type,
netbios_name_type_descr(name_type));
cur_offset += NETBIOS_NAME_LEN;
data_len -= NETBIOS_NAME_LEN;
if (data_len < 2) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
goto out;
}
name_flags = tvb_get_ntohs(tvb, cur_offset);
nbns_add_name_flags(rr_tree, tvb, cur_offset,
name_flags);
cur_offset += 2;
data_len -= 2;
num_names--;
}
if (data_len < 6) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 6,
"Unit ID: %s",
tvb_ether_to_str(tvb, cur_offset));
cur_offset += 6;
data_len -= 6;
if (data_len < 1) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 1,
"Jumpers: 0x%x", tvb_get_guint8(tvb, cur_offset));
cur_offset += 1;
data_len -= 1;
if (data_len < 1) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 1,
"Test result: 0x%x", tvb_get_guint8(tvb, cur_offset));
cur_offset += 1;
data_len -= 1;
if (data_len < 2) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 2,
"Version number: 0x%x", tvb_get_ntohs(tvb, cur_offset));
cur_offset += 2;
data_len -= 2;
if (data_len < 2) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 2,
"Period of statistics: 0x%x",
tvb_get_ntohs(tvb, cur_offset));
cur_offset += 2;
data_len -= 2;
if (data_len < 2) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 2,
"Number of CRCs: %u", tvb_get_ntohs(tvb, cur_offset));
cur_offset += 2;
data_len -= 2;
if (data_len < 2) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 2,
"Number of alignment errors: %u",
tvb_get_ntohs(tvb, cur_offset));
cur_offset += 2;
data_len -= 2;
if (data_len < 2) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 2,
"Number of collisions: %u", tvb_get_ntohs(tvb, cur_offset));
cur_offset += 2;
data_len -= 2;
if (data_len < 2) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 2,
"Number of send aborts: %u", tvb_get_ntohs(tvb, cur_offset));
cur_offset += 2;
data_len -= 2;
if (data_len < 4) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 4,
"Number of good sends: %u", tvb_get_ntohl(tvb, cur_offset));
cur_offset += 4;
data_len -= 4;
if (data_len < 4) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 4,
"Number of good receives: %u",
tvb_get_ntohl(tvb, cur_offset));
cur_offset += 4;
data_len -= 4;
if (data_len < 2) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 2,
"Number of retransmits: %u", tvb_get_ntohs(tvb, cur_offset));
cur_offset += 2;
data_len -= 2;
if (data_len < 2) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 2,
"Number of no resource conditions: %u",
tvb_get_ntohs(tvb, cur_offset));
cur_offset += 2;
data_len -= 2;
if (data_len < 2) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 2,
"Number of command blocks: %u",
tvb_get_ntohs(tvb, cur_offset));
cur_offset += 2;
data_len -= 2;
if (data_len < 2) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 2,
"Number of pending sessions: %u",
tvb_get_ntohs(tvb, cur_offset));
cur_offset += 2;
data_len -= 2;
if (data_len < 2) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 2,
"Max number of pending sessions: %u",
tvb_get_ntohs(tvb, cur_offset));
cur_offset += 2;
data_len -= 2;
if (data_len < 2) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 2,
"Max total sessions possible: %u",
tvb_get_ntohs(tvb, cur_offset));
cur_offset += 2;
data_len -= 2;
if (data_len < 2) {
proto_tree_add_text(rr_tree, tvb, cur_offset,
data_len, "(incomplete entry)");
break;
}
proto_tree_add_text(rr_tree, tvb, cur_offset, 2,
"Session data packet size: %u",
tvb_get_ntohs(tvb, cur_offset));
cur_offset += 2;
out:
break;
default:
if (cinfo != NULL)
col_append_fstr(cinfo, COL_INFO, " %s", type_name);
if (nbns_tree == NULL)
break;
trr = proto_tree_add_text(nbns_tree, tvb, offset,
(data_offset - data_start) + data_len,
"%s: type %s, class %s",
name, type_name, class_name);
rr_tree = add_rr_to_tree(trr, ett_nbns_rr, tvb, offset, name,
name_len, type_name, dns_class_name(class), ttl, data_len);
proto_tree_add_text(rr_tree, tvb, cur_offset, data_len, "Data");
cur_offset += data_len;
break;
}
return cur_offset - data_start;
}
static int
dissect_query_records(tvbuff_t *tvb, int cur_off, int nbns_data_offset,
int count, column_info *cinfo, proto_tree *nbns_tree)
{
int start_off, add_off;
proto_tree *qatree = NULL;
proto_item *ti = NULL;
start_off = cur_off;
if (nbns_tree != NULL) {
ti = proto_tree_add_text(nbns_tree, tvb, start_off, -1, "Queries");
qatree = proto_item_add_subtree(ti, ett_nbns_qry);
}
while (count-- > 0) {
add_off = dissect_nbns_query(tvb, cur_off, nbns_data_offset,
cinfo, qatree);
cur_off += add_off;
}
if (ti != NULL)
proto_item_set_len(ti, cur_off - start_off);
return cur_off - start_off;
}
static int
dissect_answer_records(tvbuff_t *tvb, int cur_off, int nbns_data_offset,
int count, column_info *cinfo, proto_tree *nbns_tree,
int opcode, const char *name)
{
int start_off, add_off;
proto_tree *qatree = NULL;
proto_item *ti = NULL;
start_off = cur_off;
if (nbns_tree != NULL) {
ti = proto_tree_add_text(nbns_tree, tvb, start_off, -1, "%s", name);
qatree = proto_item_add_subtree(ti, ett_nbns_ans);
}
while (count-- > 0) {
add_off = dissect_nbns_answer(tvb, cur_off, nbns_data_offset,
cinfo, qatree, opcode);
cur_off += add_off;
}
if (ti != NULL)
proto_item_set_len(ti, cur_off - start_off);
return cur_off - start_off;
}
static void
dissect_nbns(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
int offset = 0;
int nbns_data_offset;
column_info *cinfo;
proto_tree *nbns_tree = NULL;
proto_item *ti;
guint16 id, flags, opcode, quest, ans, auth, add;
int cur_off;
nbns_data_offset = offset;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "NBNS");
col_clear(pinfo->cinfo, COL_INFO);
id = tvb_get_ntohs(tvb, offset + NBNS_ID);
flags = tvb_get_ntohs(tvb, offset + NBNS_FLAGS);
opcode = (guint16) ((flags & F_OPCODE) >> OPCODE_SHIFT);
if (check_col(pinfo->cinfo, COL_INFO)) {
col_add_fstr(pinfo->cinfo, COL_INFO, "%s%s",
val_to_str(opcode, opcode_vals, "Unknown operation (%u)"),
(flags & F_RESPONSE) ? " response" : "");
cinfo = pinfo->cinfo;
} else {
cinfo = NULL;
}
if (tree) {
ti = proto_tree_add_item(tree, proto_nbns, tvb, offset, -1,
ENC_NA);
nbns_tree = proto_item_add_subtree(ti, ett_nbns);
proto_tree_add_uint(nbns_tree, hf_nbns_transaction_id, tvb,
offset + NBNS_ID, 2, id);
nbns_add_nbns_flags(pinfo->cinfo, nbns_tree, tvb, offset + NBNS_FLAGS,
flags, 0);
}
quest = tvb_get_ntohs(tvb, offset + NBNS_QUEST);
if (tree) {
proto_tree_add_uint(nbns_tree, hf_nbns_count_questions, tvb,
offset + NBNS_QUEST, 2, quest);
}
ans = tvb_get_ntohs(tvb, offset + NBNS_ANS);
if (tree) {
proto_tree_add_uint(nbns_tree, hf_nbns_count_answers, tvb,
offset + NBNS_ANS, 2, ans);
}
auth = tvb_get_ntohs(tvb, offset + NBNS_AUTH);
if (tree) {
proto_tree_add_uint(nbns_tree, hf_nbns_count_auth_rr, tvb,
offset + NBNS_AUTH, 2, auth);
}
add = tvb_get_ntohs(tvb, offset + NBNS_ADD);
if (tree) {
proto_tree_add_uint(nbns_tree, hf_nbns_count_add_rr, tvb,
offset + NBNS_ADD, 2, add);
}
cur_off = offset + NBNS_HDRLEN;
if (quest > 0) {
cur_off += dissect_query_records(tvb, cur_off,
nbns_data_offset, quest,
(!(flags & F_RESPONSE) ? cinfo : NULL), nbns_tree);
}
if (ans > 0) {
cur_off += dissect_answer_records(tvb, cur_off,
nbns_data_offset, ans,
((flags & F_RESPONSE) ? cinfo : NULL), nbns_tree,
opcode, "Answers");
}
if (tree) {
if (auth > 0)
cur_off += dissect_answer_records(tvb, cur_off,
nbns_data_offset,
auth, NULL, nbns_tree, opcode,
"Authoritative nameservers");
if (add > 0)
dissect_answer_records(tvb, cur_off,
nbns_data_offset,
add, NULL, nbns_tree, opcode,
"Additional records");
}
}
static void
dissect_nbdgm(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
int offset = 0;
proto_tree *nbdgm_tree = NULL;
proto_item *ti = NULL;
struct nbdgm_header header;
int flags;
tvbuff_t *next_tvb;
static const value_string error_codes[] = {
{ 0x82, "Destination name not present" },
{ 0x83, "Invalid source name format" },
{ 0x84, "Invalid destination name format" },
{ 0x00, NULL }
};
char *name;
int name_type;
int len;
name=ep_alloc(MAX_NAME_LEN);
col_set_str(pinfo->cinfo, COL_PROTOCOL, "NBDS");
col_clear(pinfo->cinfo, COL_INFO);
header.msg_type = tvb_get_guint8(tvb, offset);
flags = tvb_get_guint8(tvb, offset+1);
header.flags.more = flags & 1;
header.flags.first = (flags & 2) >> 1;
header.flags.node_type = (flags & 12) >> 2;
header.dgm_id = tvb_get_ntohs(tvb, offset+2);
header.src_ip = tvb_get_ipv4(tvb, offset+4);
header.src_port = tvb_get_ntohs(tvb, offset+8);
header.dgm_length = 0;
header.pkt_offset = 0;
header.error_code = 0;
switch (header.msg_type) {
case NBDS_DIRECT_UNIQUE:
case NBDS_DIRECT_GROUP:
case NBDS_BROADCAST:
header.dgm_length = tvb_get_ntohs(tvb, offset+10);
header.pkt_offset = tvb_get_ntohs(tvb, offset+12);
break;
case NBDS_ERROR:
header.error_code = tvb_get_guint8(tvb, offset+10);
break;
}
col_add_str(pinfo->cinfo, COL_INFO,
val_to_str(header.msg_type, nbds_msgtype_vals,
"Unknown message type (0x%02X)"));
if (tree) {
ti = proto_tree_add_item(tree, proto_nbdgm, tvb, offset, -1,
ENC_NA);
nbdgm_tree = proto_item_add_subtree(ti, ett_nbdgm);
proto_tree_add_uint(nbdgm_tree, hf_nbdgm_type, tvb,
offset, 1,
header.msg_type);
proto_tree_add_boolean(nbdgm_tree, hf_nbdgm_fragment, tvb,
offset+1, 1,
header.flags.more);
proto_tree_add_boolean(nbdgm_tree, hf_nbdgm_first, tvb,
offset+1, 1,
header.flags.first);
proto_tree_add_uint(nbdgm_tree, hf_nbdgm_node_type, tvb,
offset+1, 1,
header.flags.node_type);
proto_tree_add_uint(nbdgm_tree, hf_nbdgm_datagram_id, tvb,
offset+2, 2, header.dgm_id);
proto_tree_add_ipv4(nbdgm_tree, hf_nbdgm_src_ip, tvb,
offset+4, 4, header.src_ip);
proto_tree_add_uint(nbdgm_tree, hf_nbdgm_src_port, tvb,
offset+8, 2, header.src_port);
}
offset += 10;
switch (header.msg_type) {
case NBDS_DIRECT_UNIQUE:
case NBDS_DIRECT_GROUP:
case NBDS_BROADCAST:
if (tree) {
proto_tree_add_text(nbdgm_tree, tvb, offset, 2,
"Datagram length: %d bytes", header.dgm_length);
proto_tree_add_text(nbdgm_tree, tvb, offset+2, 2,
"Packet offset: %d bytes", header.pkt_offset);
}
offset += 4;
len = get_nbns_name(tvb, offset, offset, name, MAX_NAME_LEN, &name_type);
if (tree) {
add_name_and_type(nbdgm_tree, tvb, offset, len,
"Source name", name, name_type);
}
offset += len;
len = get_nbns_name(tvb, offset, offset, name, MAX_NAME_LEN, &name_type);
if (tree) {
add_name_and_type(nbdgm_tree, tvb, offset, len,
"Destination name", name, name_type);
}
offset += len;
if (ti != NULL)
proto_item_set_len(ti, offset);
next_tvb = tvb_new_subset_remaining(tvb, offset);
dissect_netbios_payload(next_tvb, pinfo, tree);
break;
case NBDS_ERROR:
if (tree) {
proto_tree_add_text(nbdgm_tree, tvb, offset, 1, "Error code: %s",
val_to_str(header.error_code, error_codes, "Unknown (0x%x)"));
}
offset += 1;
if (ti != NULL)
proto_item_set_len(ti, offset);
break;
case NBDS_QUERY_REQUEST:
case NBDS_POS_QUERY_RESPONSE:
case NBDS_NEG_QUERY_RESPONSE:
len = get_nbns_name(tvb, offset, offset, name, MAX_NAME_LEN, &name_type);
if (tree) {
add_name_and_type(nbdgm_tree, tvb, offset, len,
"Destination name", name, name_type);
}
offset += len;
if (ti != NULL)
proto_item_set_len(ti, offset);
break;
}
}
static int
dissect_nbss_packet(tvbuff_t *tvb, int offset, packet_info *pinfo,
proto_tree *tree, int is_cifs)
{
proto_tree *nbss_tree = NULL;
proto_item *ti = NULL;
proto_tree *field_tree;
proto_item *tf;
guint8 msg_type;
guint8 flags;
volatile int length;
int length_remaining;
int len;
char *name;
int name_type;
gint reported_len;
tvbuff_t *next_tvb;
const char *saved_proto;
void *pd_save;
name=ep_alloc(MAX_NAME_LEN);
length_remaining = tvb_length_remaining(tvb, offset);
if (nbss_desegment && pinfo->can_desegment) {
if (length_remaining < 4) {
return -(4 - length_remaining);
}
}
if (is_cifs) {
flags = 0;
length = tvb_get_ntoh24(tvb, offset + 1);
} else {
flags = tvb_get_guint8(tvb, offset + 1);
length = tvb_get_ntohs(tvb, offset + 2);
if (flags & NBSS_FLAGS_E)
length += 65536;
}
if(!pinfo->fd->flags.visited){
if( ((length+4)>tvb_reported_length_remaining(tvb, offset))
&&(tvb_length_remaining(tvb, offset)>=8)
&&(tvb_get_guint8(tvb,offset+5)=='S')
&&(tvb_get_guint8(tvb,offset+6)=='M')
&&(tvb_get_guint8(tvb,offset+7)=='B') ){
pinfo->want_pdu_tracking=2;
pinfo->bytes_until_next_pdu=(length+4)-tvb_reported_length_remaining(tvb, offset);
}
}
if (nbss_desegment && pinfo->can_desegment) {
if (length_remaining < length + 4) {
return -((length + 4) - length_remaining);
}
}
msg_type = tvb_get_guint8(tvb, offset);
if (tree) {
ti = proto_tree_add_item(tree, proto_nbss, tvb, offset, length + 4, ENC_NA);
nbss_tree = proto_item_add_subtree(ti, ett_nbss);
proto_tree_add_item(nbss_tree, hf_nbss_type, tvb, offset, 1, ENC_NA);
}
offset += 1;
if (is_cifs) {
if (tree) {
proto_tree_add_item(nbss_tree, hf_nbss_cifs_length, tvb, offset, 3, ENC_BIG_ENDIAN);
}
offset += 3;
} else {
if (tree) {
tf = proto_tree_add_uint(nbss_tree, hf_nbss_flags, tvb, offset, 1, flags);
field_tree = proto_item_add_subtree(tf, ett_nbss_flags);
proto_tree_add_text(field_tree, tvb, offset, 1, "%s",
decode_boolean_bitfield(flags, NBSS_FLAGS_E,
8, "Add 65536 to length", "Add 0 to length"));
}
offset += 1;
if (tree) {
proto_tree_add_uint_format(nbss_tree, hf_nbss_length, tvb, offset, 2,
length, "Length: %u", length);
}
offset += 2;
}
switch (msg_type) {
case SESSION_REQUEST:
len = get_nbns_name(tvb, offset, offset, name, MAX_NAME_LEN, &name_type);
if (tree)
add_name_and_type(nbss_tree, tvb, offset, len,
"Called name", name, name_type);
offset += len;
col_append_fstr(pinfo->cinfo, COL_INFO, ", to %s ", name);
len = get_nbns_name(tvb, offset, offset, name, MAX_NAME_LEN, &name_type);
if (tree)
add_name_and_type(nbss_tree, tvb, offset, len,
"Calling name", name, name_type);
col_append_fstr(pinfo->cinfo, COL_INFO, "from %s", name);
break;
case NEGATIVE_SESSION_RESPONSE:
if (tree)
proto_tree_add_text(nbss_tree, tvb, offset, 1,
"Error code: %s",
val_to_str(tvb_get_guint8(tvb, offset),
error_codes, "Unknown (%x)"));
col_append_fstr(pinfo->cinfo, COL_INFO, ", %s",
val_to_str(tvb_get_guint8(tvb, offset),
error_codes, "Unknown (%x)"));
break;
case RETARGET_SESSION_RESPONSE:
if (tree)
proto_tree_add_text(nbss_tree, tvb, offset, 4,
"Retarget IP address: %s",
tvb_ip_to_str(tvb, offset));
offset += 4;
if (tree)
proto_tree_add_text(nbss_tree, tvb, offset, 2,
"Retarget port: %u",
tvb_get_ntohs(tvb, offset));
break;
case SESSION_MESSAGE:
len = tvb_length_remaining(tvb, offset);
reported_len = tvb_reported_length_remaining(tvb, offset);
if (len > length)
len = length;
if (reported_len > length)
reported_len = length;
next_tvb = tvb_new_subset(tvb, offset, len, reported_len);
saved_proto = pinfo->current_proto;
pd_save = pinfo->private_data;
TRY {
dissect_netbios_payload(next_tvb, pinfo, tree);
}
CATCH(BoundsError) {
RETHROW;
}
CATCH(ReportedBoundsError) {
pinfo->private_data = pd_save;
show_reported_bounds_error(tvb, pinfo, tree);
pinfo->current_proto = saved_proto;
}
ENDTRY;
break;
}
return length + 4;
}
static void
dissect_nbss(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
struct tcpinfo *tcpinfo = pinfo->private_data;
int offset = 0;
int max_data;
guint8 msg_type;
guint8 flags;
guint32 length;
int len;
gboolean is_cifs;
proto_tree *nbss_tree;
proto_item *ti;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "NBSS");
col_clear(pinfo->cinfo, COL_INFO);
max_data = tvb_length(tvb);
msg_type = tvb_get_guint8(tvb, offset);
if (pinfo->match_uint == TCP_PORT_CIFS) {
is_cifs = TRUE;
} else {
is_cifs = FALSE;
}
if (!tcpinfo->is_reassembled) {
if (max_data < 4) {
goto continuation;
}
if (tvb_length_remaining(tvb, offset) >=8
&& tvb_get_guint8(tvb,offset+0) == SESSION_MESSAGE
&& tvb_get_guint8(tvb,offset+5) == 'S'
&& tvb_get_guint8(tvb,offset+6) == 'M'
&& tvb_get_guint8(tvb,offset+7) == 'B') {
is_cifs = TRUE;
} else {
is_cifs = FALSE;
}
if (is_cifs) {
flags = 0;
length = tvb_get_ntoh24(tvb, offset + 1);
} else {
flags = tvb_get_guint8(tvb, offset + 1);
length = tvb_get_ntohs(tvb, offset + 2);
if (flags & NBSS_FLAGS_E)
length += 0x10000;
}
if ((flags & (~NBSS_FLAGS_E)) != 0) {
goto continuation;
}
switch (msg_type) {
case SESSION_MESSAGE:
if (length == 0)
goto continuation;
break;
case SESSION_REQUEST:
if (length < 2 || length > 256)
goto continuation;
break;
case POSITIVE_SESSION_RESPONSE:
if (length != 0)
goto continuation;
break;
case NEGATIVE_SESSION_RESPONSE:
if (length != 1)
goto continuation;
break;
case RETARGET_SESSION_RESPONSE:
if (length != 6)
goto continuation;
break;
case SESSION_KEEP_ALIVE:
if (length != 0)
goto continuation;
break;
default:
goto continuation;
}
}
col_add_str(pinfo->cinfo, COL_INFO,
val_to_str(msg_type, message_types, "Unknown (%02x)"));
while (tvb_reported_length_remaining(tvb, offset) > 0) {
len = dissect_nbss_packet(tvb, offset, pinfo, tree, is_cifs);
if (len < 0) {
pinfo->desegment_offset = offset;
pinfo->desegment_len = -len;
return;
}
offset += len;
}
return;
continuation:
col_set_str(pinfo->cinfo, COL_INFO, "NBSS Continuation Message");
if (tree) {
ti = proto_tree_add_item(tree, proto_nbss, tvb, 0, -1, ENC_NA);
nbss_tree = proto_item_add_subtree(ti, ett_nbss);
proto_tree_add_text(nbss_tree, tvb, 0, -1, "Continuation data");
}
}
void
proto_register_nbt(void)
{
static hf_register_info hf_nbns[] = {
{ &hf_nbns_flags,
{ "Flags", "nbns.flags",
FT_UINT16, BASE_HEX, NULL, 0x0,
NULL, HFILL }},
{ &hf_nbns_flags_response,
{ "Response", "nbns.flags.response",
FT_BOOLEAN, 16, TFS(&tfs_flags_response), F_RESPONSE,
"Is the message a response?", HFILL }},
{ &hf_nbns_flags_opcode,
{ "Opcode", "nbns.flags.opcode",
FT_UINT16, BASE_DEC, VALS(opcode_vals), F_OPCODE,
"Operation code", HFILL }},
{ &hf_nbns_flags_authoritative,
{ "Authoritative", "nbns.flags.authoritative",
FT_BOOLEAN, 16, TFS(&tfs_flags_authoritative), F_AUTHORITATIVE,
"Is the server is an authority for the domain?", HFILL }},
{ &hf_nbns_flags_truncated,
{ "Truncated", "nbns.flags.truncated",
FT_BOOLEAN, 16, TFS(&tfs_flags_truncated), F_TRUNCATED,
"Is the message truncated?", HFILL }},
{ &hf_nbns_flags_recdesired,
{ "Recursion desired", "nbns.flags.recdesired",
FT_BOOLEAN, 16, TFS(&tfs_flags_recdesired), F_RECDESIRED,
"Do query recursively?", HFILL }},
{ &hf_nbns_flags_recavail,
{ "Recursion available", "nbns.flags.recavail",
FT_BOOLEAN, 16, TFS(&tfs_flags_recavail), F_RECAVAIL,
"Can the server do recursive queries?", HFILL }},
{ &hf_nbns_flags_broadcast,
{ "Broadcast", "nbns.flags.broadcast",
FT_BOOLEAN, 16, TFS(&tfs_flags_broadcast), F_BROADCAST,
"Is this a broadcast packet?", HFILL }},
{ &hf_nbns_flags_rcode,
{ "Reply code", "nbns.flags.rcode",
FT_UINT16, BASE_DEC, VALS(rcode_vals), F_RCODE,
NULL, HFILL }},
{ &hf_nbns_transaction_id,
{ "Transaction ID", "nbns.id",
FT_UINT16, BASE_HEX, NULL, 0x0,
"Identification of transaction", HFILL }},
{ &hf_nbns_count_questions,
{ "Questions", "nbns.count.queries",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Number of queries in packet", HFILL }},
{ &hf_nbns_count_answers,
{ "Answer RRs", "nbns.count.answers",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Number of answers in packet", HFILL }},
{ &hf_nbns_count_auth_rr,
{ "Authority RRs", "nbns.count.auth_rr",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Number of authoritative records in packet", HFILL }},
{ &hf_nbns_count_add_rr,
{ "Additional RRs", "nbns.count.add_rr",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Number of additional records in packet", HFILL }}
};
static hf_register_info hf_nbdgm[] = {
{ &hf_nbdgm_type,
{ "Message Type", "nbdgm.type",
FT_UINT8, BASE_DEC, VALS(nbds_msgtype_vals), 0x0,
"NBDGM message type", HFILL }},
{ &hf_nbdgm_fragment,
{ "More fragments follow", "nbdgm.next",
FT_BOOLEAN, BASE_NONE, TFS(&tfs_yes_no), 0x0,
"TRUE if more fragments follow", HFILL }},
{ &hf_nbdgm_first,
{ "This is first fragment", "nbdgm.first",
FT_BOOLEAN, BASE_NONE, TFS(&tfs_yes_no), 0x0,
"TRUE if first fragment", HFILL }},
{ &hf_nbdgm_node_type,
{ "Node Type", "nbdgm.node_type",
FT_UINT8, BASE_DEC, VALS(node_type_vals), 0x0,
NULL, HFILL }},
{ &hf_nbdgm_datagram_id,
{ "Datagram ID", "nbdgm.dgram_id",
FT_UINT16, BASE_HEX, NULL, 0x0,
"Datagram identifier", HFILL }},
{ &hf_nbdgm_src_ip,
{ "Source IP", "nbdgm.src.ip",
FT_IPv4, BASE_NONE, NULL, 0x0,
"Source IPv4 address", HFILL }},
{ &hf_nbdgm_src_port,
{ "Source Port", "nbdgm.src.port",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }}
};
static hf_register_info hf_nbss[] = {
{ &hf_nbss_type,
{ "Message Type", "nbss.type",
FT_UINT8, BASE_HEX, VALS(message_types), 0x0,
"NBSS message type", HFILL }},
{ &hf_nbss_flags,
{ "Flags", "nbss.flags",
FT_UINT8, BASE_HEX, NULL, 0x0,
"NBSS message flags", HFILL }},
{ &hf_nbss_length,
{ "Length", "nbss.length",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Length of trailer (payload) following this field in bytes", HFILL }},
{ &hf_nbss_cifs_length,
{ "Length", "nbss.length",
FT_UINT24, BASE_DEC, NULL, 0x0,
"Length trailer (payload) following this field in bytes", HFILL }}
};
static gint *ett[] = {
&ett_nbns,
&ett_nbns_qd,
&ett_nbns_flags,
&ett_nbns_nb_flags,
&ett_nbns_name_flags,
&ett_nbns_rr,
&ett_nbns_qry,
&ett_nbns_ans,
&ett_nbdgm,
&ett_nbss,
&ett_nbss_flags,
};
module_t *nbss_module;
proto_nbns = proto_register_protocol("NetBIOS Name Service", "NBNS", "nbns");
proto_register_field_array(proto_nbns, hf_nbns, array_length(hf_nbns));
proto_nbdgm = proto_register_protocol("NetBIOS Datagram Service",
"NBDS", "nbdgm");
proto_register_field_array(proto_nbdgm, hf_nbdgm, array_length(hf_nbdgm));
proto_nbss = proto_register_protocol("NetBIOS Session Service",
"NBSS", "nbss");
proto_register_field_array(proto_nbss, hf_nbss, array_length(hf_nbss));
proto_register_subtree_array(ett, array_length(ett));
nbss_module = prefs_register_protocol(proto_nbss, NULL);
prefs_register_bool_preference(nbss_module, "desegment_nbss_commands",
"Reassemble NBSS packets spanning multiple TCP segments",
"Whether the NBSS dissector should reassemble packets spanning multiple TCP segments."
" To use this option, you must also enable \"Allow subdissectors to reassemble TCP streams\" in the TCP protocol settings.",
&nbss_desegment);
}
void
proto_reg_handoff_nbt(void)
{
dissector_handle_t nbns_handle, nbdgm_handle, nbss_handle;
nbns_handle = create_dissector_handle(dissect_nbns, proto_nbns);
dissector_add_uint("udp.port", UDP_PORT_NBNS, nbns_handle);
nbdgm_handle = create_dissector_handle(dissect_nbdgm, proto_nbdgm);
dissector_add_uint("udp.port", UDP_PORT_NBDGM, nbdgm_handle);
nbss_handle = create_dissector_handle(dissect_nbss, proto_nbss);
dissector_add_uint("tcp.port", TCP_PORT_NBSS, nbss_handle);
dissector_add_uint("tcp.port", TCP_PORT_CIFS, nbss_handle);
}
