static void
ucp_stats_tree_init(stats_tree* st)
{
st_ucp_messages = stats_tree_create_node(st, st_str_ucp, 0, TRUE);
st_ucp_ops = stats_tree_create_node(st, st_str_ops, st_ucp_messages, TRUE);
st_ucp_res = stats_tree_create_node(st, st_str_res, st_ucp_messages, TRUE);
st_ucp_results = stats_tree_create_node(st, st_str_ucp_res, 0, TRUE);
st_ucp_results_pos = stats_tree_create_node(st, st_str_pos, st_ucp_results, TRUE);
st_ucp_results_neg = stats_tree_create_node(st, st_str_neg, st_ucp_results, TRUE);
}
static int
ucp_stats_tree_per_packet(stats_tree *st,
packet_info *pinfo _U_,
epan_dissect_t *edt _U_,
const void *p)
{
ucp_tap_rec_t* tap_rec = (ucp_tap_rec_t*)p;
tick_stat_node(st, st_str_ucp, 0, TRUE);
if (tap_rec->message_type == 0)
{
tick_stat_node(st, st_str_ops, st_ucp_messages, TRUE);
tick_stat_node(st, val_to_str(tap_rec->operation, vals_hdr_OT,
"Unknown OT: %d"), st_ucp_ops, FALSE);
}
else
{
tick_stat_node(st, st_str_res, st_ucp_messages, TRUE);
tick_stat_node(st, val_to_str(tap_rec->operation, vals_hdr_OT,
"Unknown OT: %d"), st_ucp_res, FALSE);
tick_stat_node(st, st_str_ucp_res, 0, TRUE);
if (tap_rec->result == 0)
{
tick_stat_node(st, st_str_pos, st_ucp_results, FALSE);
}
else
{
tick_stat_node(st, st_str_neg, st_ucp_results, TRUE);
tick_stat_node(st, val_to_str(tap_rec->result, vals_parm_EC,
"Unknown EC: %d"), st_ucp_results_neg, FALSE);
}
}
return 1;
}
static int
check_ucp(tvbuff_t *tvb, int *endpkt)
{
guint offset = 1;
guint checksum = 0;
int pkt_check, tmp;
int length;
length = tvb_find_guint8(tvb, offset, -1, UCP_ETX);
if (length == -1) {
*endpkt = tvb_reported_length_remaining(tvb, offset);
return UCP_MALFORMED;
}
for (; offset < (guint) (length - 2); offset++)
checksum += tvb_get_guint8(tvb, offset);
checksum &= 0xFF;
tmp = tvb_get_guint8(tvb, offset++);
pkt_check = AHex2Bin(tmp);
tmp = tvb_get_guint8(tvb, offset++);
pkt_check = 16 * pkt_check + AHex2Bin(tmp);
*endpkt = offset + 1;
if (checksum == (guint) pkt_check)
return 0;
else
return UCP_INV_CHK;
}
static time_t
ucp_mktime(const char *datestr)
{
struct tm r_time;
r_time.tm_mday = (10 * (datestr[0] - '0') + (datestr[1] - '0'));
r_time.tm_mon = (10 * (datestr[2] - '0') + (datestr[3] - '0')) - 1;
r_time.tm_year = (10 * (datestr[4] - '0') + (datestr[5] - '0'));
if (r_time.tm_year < 90)
r_time.tm_year += 100;
r_time.tm_hour = (10 * (datestr[6] - '0') + (datestr[7] - '0'));
r_time.tm_min = (10 * (datestr[8] - '0') + (datestr[9] - '0'));
if (datestr[10])
r_time.tm_sec = (10 * (datestr[10] - '0') + (datestr[11] - '0'));
else
r_time.tm_sec = 0;
r_time.tm_isdst = -1;
return mktime(&r_time);
}
static void
ucp_handle_string(proto_tree *tree, tvbuff_t *tvb, int field, int *offset)
{
gint idx, len;
idx = tvb_find_guint8(tvb, *offset, -1, '/');
if (idx == -1) {
len = tvb_length_remaining(tvb, *offset);
tvb_ensure_bytes_exist(tvb, *offset, len + 1);
} else
len = idx - *offset;
if (len > 0)
proto_tree_add_item(tree, field, tvb, *offset, len, ENC_ASCII|ENC_NA);
*offset += len;
if (idx != -1)
*offset += 1;
}
static void
ucp_handle_IRAstring(proto_tree *tree, tvbuff_t *tvb, int field, int *offset)
{
char strval[UCP_BUFSIZ + 1],
*p_dst = strval;
guint8 byte;
int idx = 0;
int tmpoff = *offset;
while (((byte = tvb_get_guint8(tvb, tmpoff++)) != '/') &&
(idx < UCP_BUFSIZ))
{
if (byte >= '0' && byte <= '9')
{
*p_dst = (byte - '0') * 16;
}
else
{
*p_dst = (byte - 'A' + 10) * 16;
}
if ((byte = tvb_get_guint8(tvb, tmpoff++)) == '/')
{
break;
}
if (byte >= '0' && byte <= '9')
{
*p_dst++ += byte - '0';
}
else
{
*p_dst++ += byte - 'A' + 10;
}
idx++;
}
strval[idx] = '\0';
if (idx == UCP_BUFSIZ)
{
while ((tvb_get_guint8(tvb, tmpoff++)) != '/')
;
}
if ((tmpoff - *offset) > 1)
proto_tree_add_string(tree, field, tvb, *offset,
tmpoff - *offset - 1, strval);
*offset = tmpoff;
}
static guint
ucp_handle_byte(proto_tree *tree, tvbuff_t *tvb, int field, int *offset)
{
guint intval = 0;
if ((intval = tvb_get_guint8(tvb, (*offset)++)) != '/') {
proto_tree_add_uint(tree, field, tvb, *offset - 1, 1, intval);
(*offset)++;
}
return intval;
}
static guint
ucp_handle_int(proto_tree *tree, tvbuff_t *tvb, int field, int *offset)
{
gint idx, len;
const char *strval;
guint intval = 0;
idx = tvb_find_guint8(tvb, *offset, -1, '/');
if (idx == -1) {
len = tvb_length_remaining(tvb, *offset);
tvb_ensure_bytes_exist(tvb, *offset, len + 1);
} else
len = idx - *offset;
strval = tvb_get_string(wmem_packet_scope(), tvb, *offset, len);
if (len > 0) {
intval = atoi(strval);
proto_tree_add_uint(tree, field, tvb, *offset, len, intval);
}
*offset += len;
if (idx != -1)
*offset += 1;
return intval;
}
static void
ucp_handle_time(proto_tree *tree, tvbuff_t *tvb, int field, int *offset)
{
gint idx, len;
const char *strval;
time_t tval;
nstime_t tmptime;
idx = tvb_find_guint8(tvb, *offset, -1, '/');
if (idx == -1) {
len = tvb_length_remaining(tvb, *offset);
tvb_ensure_bytes_exist(tvb, *offset, len + 1);
} else
len = idx - *offset;
strval = tvb_get_string(wmem_packet_scope(), tvb, *offset, len);
if (len > 0) {
tval = ucp_mktime(strval);
tmptime.secs = tval;
tmptime.nsecs = 0;
proto_tree_add_time(tree, field, tvb, *offset, len, &tmptime);
}
*offset += len;
if (idx != -1)
*offset += 1;
}
static void
ucp_handle_data(proto_tree *tree, tvbuff_t *tvb, int field, int *offset)
{
int tmpoff = *offset;
while (tvb_get_guint8(tvb, tmpoff++) != '/')
;
if ((tmpoff - *offset) > 1)
proto_tree_add_item(tree, field, tvb, *offset,
tmpoff - *offset - 1, ENC_NA);
*offset = tmpoff;
}
static void
ucp_handle_data_string(proto_tree *tree, tvbuff_t *tvb, int field, int *offset)
{
int tmpoff = *offset;
while (tvb_get_guint8(tvb, tmpoff++) != '/')
;
if ((tmpoff - *offset) > 1)
proto_tree_add_item(tree, field, tvb, *offset,
tmpoff - *offset - 1, ENC_ASCII|ENC_NA);
*offset = tmpoff;
}
static void
ucp_handle_mt(proto_tree *tree, tvbuff_t *tvb, int *offset)
{
guint intval;
intval = ucp_handle_byte(tree, tvb, hf_ucp_parm_MT, offset);
switch (intval) {
case '1':
break;
case '4':
ucp_handle_string(tree, tvb, hf_ucp_parm_NB, offset);
case '2':
ucp_handle_data(tree, tvb, hf_ucp_data_section, offset);
break;
case '3':
ucp_handle_IRAstring(tree, tvb, hf_ucp_parm_AMsg, offset);
break;
case '5':
ucp_handle_byte(tree, tvb, hf_ucp_parm_PNC, offset);
ucp_handle_string(tree, tvb, hf_ucp_parm_LNo, offset);
ucp_handle_string(tree, tvb, hf_ucp_parm_LST, offset);
ucp_handle_string(tree, tvb, hf_ucp_parm_TNo, offset);
break;
case '6':
ucp_handle_data(tree, tvb, hf_ucp_data_section, offset);
ucp_handle_int(tree, tvb, hf_ucp_parm_CS, offset);
break;
default:
break;
}
}
static void
ucp_handle_XSer(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 0;
guint intval;
int service;
int len;
while ((intval = tvb_get_guint8(tvb, offset)) != '/') {
service = AHex2Bin(intval);
intval = tvb_get_guint8(tvb, offset+1);
service = service * 16 + AHex2Bin(intval);
intval = tvb_get_guint8(tvb, offset+2);
len = AHex2Bin(intval);
intval = tvb_get_guint8(tvb, offset+3);
len = len * 16 + AHex2Bin(intval);
proto_tree_add_uint(tree, hf_xser_service, tvb, offset, 2, service);
proto_tree_add_uint(tree, hf_xser_length, tvb, offset+2, 2, len);
proto_tree_add_item(tree, hf_xser_data, tvb, offset+4, len*2, ENC_ASCII|ENC_NA);
offset += 4 + (2 * len);
}
}
static void
add_00O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_OAdC);
UcpHandleString(hf_ucp_parm_OAC);
}
static void
add_00R(proto_tree *tree, tvbuff_t *tvb, ucp_tap_rec_t *tap_rec)
{
int offset = 1;
guint intval;
intval = UcpHandleByte(hf_ucp_parm_ACK);
if (intval == 'A')
{
UcpHandleByte(hf_ucp_parm_BAS);
UcpHandleByte(hf_ucp_parm_LAR);
UcpHandleByte(hf_ucp_parm_L1R);
UcpHandleByte(hf_ucp_parm_L3R);
UcpHandleByte(hf_ucp_parm_LCR);
UcpHandleByte(hf_ucp_parm_LUR);
UcpHandleByte(hf_ucp_parm_LRR);
UcpHandleByte(hf_ucp_parm_RT);
UcpHandleInt(hf_ucp_parm_NoN);
UcpHandleInt(hf_ucp_parm_NoA);
UcpHandleInt(hf_ucp_parm_NoB);
tap_rec->result = 0;
} else {
tap_rec->result = UcpHandleInt(hf_ucp_parm_EC);
UcpHandleString(hf_ucp_parm_SM);
}
}
static void
add_01O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_OAdC);
UcpHandleString(hf_ucp_parm_OAC);
ucp_handle_mt(tree, tvb, &offset);
}
static void
add_01R(proto_tree *tree, tvbuff_t *tvb, ucp_tap_rec_t *tap_rec)
{
int offset = 1;
guint intval;
intval = UcpHandleByte(hf_ucp_parm_ACK);
if (intval == 'N')
tap_rec->result = UcpHandleInt(hf_ucp_parm_EC);
else
tap_rec->result = 0;
UcpHandleString(hf_ucp_parm_SM);
}
static void
add_02O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
guint intval;
guint idx;
intval = UcpHandleInt(hf_ucp_parm_NPL);
for (idx = 0; idx < intval; idx++)
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_OAdC);
UcpHandleString(hf_ucp_parm_OAC);
ucp_handle_mt(tree, tvb, &offset);
}
static void
add_03O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
guint intval;
guint idx;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_OAdC);
UcpHandleString(hf_ucp_parm_OAC);
intval = UcpHandleInt(hf_ucp_parm_NPL);
for (idx = 0; idx < intval; idx++)
UcpHandleString(hf_ucp_parm_GA);
UcpHandleByte(hf_ucp_parm_RP);
UcpHandleString(hf_ucp_parm_LRP);
UcpHandleByte(hf_ucp_parm_PR);
UcpHandleString(hf_ucp_parm_LPR);
UcpHandleByte(hf_ucp_parm_UM);
UcpHandleString(hf_ucp_parm_LUM);
UcpHandleByte(hf_ucp_parm_RC);
UcpHandleString(hf_ucp_parm_LRC);
UcpHandleByte(hf_ucp_parm_DD);
UcpHandleTime(hf_ucp_parm_DDT);
ucp_handle_mt(tree, tvb, &offset);
}
static void
add_04O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_GAdC);
UcpHandleString(hf_ucp_parm_AC);
UcpHandleString(hf_ucp_parm_OAdC);
UcpHandleString(hf_ucp_parm_OAC);
}
static void
add_04R(proto_tree *tree, tvbuff_t *tvb, ucp_tap_rec_t *tap_rec)
{
int offset = 1;
guint intval;
guint idx;
intval = UcpHandleByte(hf_ucp_parm_ACK);
if (intval == 'A') {
intval = UcpHandleInt(hf_ucp_parm_NPL);
for (idx = 0; idx < intval; idx++)
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_GAdC);
tap_rec->result = 0;
} else
tap_rec->result = UcpHandleInt(hf_ucp_parm_EC);
UcpHandleString(hf_ucp_parm_SM);
}
static void
add_05O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
guint intval;
guint idx;
UcpHandleString(hf_ucp_parm_GAdC);
UcpHandleString(hf_ucp_parm_AC);
UcpHandleString(hf_ucp_parm_OAdC);
UcpHandleString(hf_ucp_parm_OAC);
intval = UcpHandleInt(hf_ucp_parm_NPL);
for (idx = 0; idx < intval; idx++)
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleByte(hf_ucp_parm_A_D);
}
static void
add_06O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_AC);
}
static void
add_06R(proto_tree *tree, tvbuff_t *tvb, ucp_tap_rec_t *tap_rec)
{
int offset = 1;
guint intval;
intval = UcpHandleByte(hf_ucp_parm_ACK);
if (intval == 'A') {
UcpHandleTime(hf_ucp_parm_CT);
UcpHandleString(hf_ucp_parm_AAC);
tap_rec->result = 0;
} else
tap_rec->result = UcpHandleInt(hf_ucp_parm_EC);
UcpHandleString(hf_ucp_parm_SM);
}
static void
add_07O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_AC);
UcpHandleString(hf_ucp_parm_NAC);
}
static void
add_08O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_AC);
UcpHandleString(hf_ucp_parm_LAC);
UcpHandleString(hf_ucp_parm_L1P);
UcpHandleString(hf_ucp_parm_L3P);
UcpHandleString(hf_ucp_parm_LRC);
UcpHandleString(hf_ucp_parm_LUM);
UcpHandleString(hf_ucp_parm_LRP);
UcpHandleString(hf_ucp_parm_LST);
}
static void
add_09O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_LNo);
UcpHandleString(hf_ucp_parm_LST);
}
static void
add_09R(proto_tree *tree, tvbuff_t *tvb, ucp_tap_rec_t *tap_rec)
{
int offset = 1;
guint intval;
guint idx;
intval = UcpHandleByte(hf_ucp_parm_ACK);
if (intval == 'A') {
intval = UcpHandleInt(hf_ucp_parm_NPL);
for (idx = 0; idx < intval; idx++)
UcpHandleString(hf_ucp_parm_LST);
tap_rec->result = 0;
} else
tap_rec->result = UcpHandleInt(hf_ucp_parm_EC);
UcpHandleString(hf_ucp_parm_SM);
}
static void
add_10O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_AC);
UcpHandleString(hf_ucp_parm_LNo);
UcpHandleString(hf_ucp_parm_TNo);
UcpHandleData(hf_ucp_parm_STx);
UcpHandleInt(hf_ucp_parm_CS);
}
static void
add_11R(proto_tree *tree, tvbuff_t *tvb, ucp_tap_rec_t *tap_rec)
{
int offset = 1;
guint intval;
guint idx;
intval = UcpHandleByte(hf_ucp_parm_ACK);
if (intval == 'A') {
intval = UcpHandleInt(hf_ucp_parm_NPL);
for (idx = 0; idx < intval; idx++)
UcpHandleString(hf_ucp_parm_GA);
tap_rec->result = 0;
} else
tap_rec->result = UcpHandleInt(hf_ucp_parm_EC);
UcpHandleString(hf_ucp_parm_SM);
}
static void
add_12O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
guint intval;
guint idx;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_AC);
intval = UcpHandleInt(hf_ucp_parm_NPL);
for (idx = 0; idx < intval; idx++)
UcpHandleString(hf_ucp_parm_GA);
}
static void
add_14O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_AC);
UcpHandleString(hf_ucp_parm_MNo);
UcpHandleByte(hf_ucp_parm_R_T);
}
static void
add_14R(proto_tree *tree, tvbuff_t *tvb, ucp_tap_rec_t *tap_rec)
{
int offset = 1;
guint intval;
guint idx;
intval = UcpHandleByte(hf_ucp_parm_ACK);
if (intval == 'A') {
intval = UcpHandleInt(hf_ucp_parm_NPL);
for (idx = 0; idx < intval; idx++)
UcpHandleData(hf_ucp_data_section);
tap_rec->result = 0;
} else {
tap_rec->result = UcpHandleInt(hf_ucp_parm_EC);
UcpHandleString(hf_ucp_parm_SM);
}
}
static void
add_15O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_AC);
UcpHandleTime(hf_ucp_parm_ST);
UcpHandleTime(hf_ucp_parm_SP);
}
static void
add_17O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_AC);
UcpHandleString(hf_ucp_parm_DAdC);
UcpHandleTime(hf_ucp_parm_ST);
UcpHandleTime(hf_ucp_parm_SP);
}
static void
add_19O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_AC);
UcpHandleTime(hf_ucp_parm_ST);
UcpHandleTime(hf_ucp_parm_SP);
}
static void
add_22O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_OAdC);
UcpHandleString(hf_ucp_parm_OAC);
UcpHandleData(hf_ucp_data_section);
UcpHandleInt(hf_ucp_parm_CS);
}
static void
add_23O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_IVR5x);
UcpHandleByte(hf_ucp_parm_REQ_OT);
}
static void
add_23R(proto_tree *tree, tvbuff_t *tvb, ucp_tap_rec_t *tap_rec)
{
int offset = 1;
guint intval;
guint idx;
intval = UcpHandleByte(hf_ucp_parm_ACK);
if (intval == 'A') {
UcpHandleByte(hf_ucp_parm_IVR5x);
intval = UcpHandleInt(hf_ucp_parm_NPL);
for (idx = 0; idx < intval; idx++)
UcpHandleInt(hf_ucp_hdr_OT);
tap_rec->result = 0;
} else
tap_rec->result = UcpHandleInt(hf_ucp_parm_EC);
UcpHandleString(hf_ucp_parm_SM);
}
static void
add_24O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_AC);
UcpHandleByte(hf_ucp_parm_SSTAT);
}
static void
add_24R(proto_tree *tree, tvbuff_t *tvb, ucp_tap_rec_t *tap_rec)
{
int offset = 1;
guint intval;
guint idx;
intval = UcpHandleByte(hf_ucp_parm_ACK);
if (intval == 'A') {
if ((intval = tvb_get_guint8(tvb, offset++)) != '/') {
proto_tree_add_text(tree, tvb, offset - 1, 1,
"GA roaming definitions");
if (intval == 'N') {
proto_tree_add_text(tree, tvb, offset -1, 1,
"Not subscribed/not allowed");
offset++;
} else {
--offset;
intval = UcpHandleInt(hf_ucp_parm_NPL);
for (idx = 0; idx < intval; idx++)
UcpHandleData(hf_ucp_data_section);
}
}
if ((intval = tvb_get_guint8(tvb, offset++)) != '/') {
proto_tree_add_text(tree, tvb, offset - 1, 1,
"Call barring definitions");
if (intval == 'N') {
proto_tree_add_text(tree, tvb, offset -1, 1,
"Not subscribed/not allowed");
offset++;
} else {
--offset;
intval = UcpHandleInt(hf_ucp_parm_NPL);
for (idx = 0; idx < intval; idx++)
UcpHandleData(hf_ucp_data_section);
}
}
if ((intval = tvb_get_guint8(tvb, offset++)) != '/') {
proto_tree_add_text(tree, tvb, offset - 1, 1,
"Deferred delivery definitions");
if (intval == 'N') {
proto_tree_add_text(tree, tvb, offset -1, 1,
"Not subscribed/not allowed");
offset++;
} else {
--offset;
intval = UcpHandleInt(hf_ucp_parm_NPL);
for (idx = 0; idx < intval; idx++)
UcpHandleData(hf_ucp_data_section);
}
}
if ((intval = tvb_get_guint8(tvb, offset++)) != '/') {
proto_tree_add_text(tree, tvb, offset - 1, 1,
"Diversion definitions");
if (intval == 'N') {
proto_tree_add_text(tree, tvb, offset -1, 1,
"Not subscribed/not allowed");
offset++;
} else {
--offset;
intval = UcpHandleInt(hf_ucp_parm_NPL);
for (idx = 0; idx < intval; idx++)
UcpHandleData(hf_ucp_data_section);
}
}
UcpHandleInt(hf_ucp_parm_LMN);
if ((intval = tvb_get_guint8(tvb, offset++)) != '/') {
if (intval == 'N') {
proto_tree_add_string(tree, hf_ucp_parm_NMESS_str, tvb,
offset -1, 1, "Not subscribed/not allowed");
offset++;
} else {
--offset;
UcpHandleInt(hf_ucp_parm_NMESS);
}
}
tap_rec->result = 0;
} else
tap_rec->result = UcpHandleInt(hf_ucp_parm_EC);
UcpHandleString(hf_ucp_parm_SM);
}
static void
add_30O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_OAdC);
UcpHandleString(hf_ucp_parm_AC);
UcpHandleByte(hf_ucp_parm_NRq);
UcpHandleString(hf_ucp_parm_NAdC);
UcpHandleInt(hf_ucp_parm_NPID);
UcpHandleByte(hf_ucp_parm_DD);
UcpHandleTime(hf_ucp_parm_DDT);
UcpHandleTime(hf_ucp_parm_VP);
UcpHandleData(hf_ucp_data_section);
}
static void
add_30R(proto_tree *tree, tvbuff_t *tvb, ucp_tap_rec_t *tap_rec)
{
int offset = 1;
guint intval;
intval = UcpHandleByte(hf_ucp_parm_ACK);
if (intval == 'A') {
UcpHandleTime(hf_ucp_parm_MVP);
tap_rec->result = 0;
} else {
tap_rec->result = UcpHandleInt(hf_ucp_parm_EC);
}
UcpHandleString(hf_ucp_parm_SM);
}
static void
add_31O(proto_tree *tree, tvbuff_t *tvb)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleInt(hf_ucp_parm_PID);
}
static void
add_5xO(proto_tree *tree, tvbuff_t *tvb)
{
guint intval;
int offset = 1;
int tmpoff;
proto_item *ti;
tvbuff_t *tmptvb;
UcpHandleString(hf_ucp_parm_AdC);
UcpHandleString(hf_ucp_parm_OAdC);
UcpHandleString(hf_ucp_parm_AC);
UcpHandleByte(hf_ucp_parm_NRq);
UcpHandleString(hf_ucp_parm_NAdC);
UcpHandleByte(hf_ucp_parm_NT);
UcpHandleInt(hf_ucp_parm_NPID);
UcpHandleByte(hf_ucp_parm_LRq);
UcpHandleString(hf_ucp_parm_LRAd);
UcpHandleInt(hf_ucp_parm_LPID);
UcpHandleByte(hf_ucp_parm_DD);
UcpHandleTime(hf_ucp_parm_DDT);
UcpHandleTime(hf_ucp_parm_VP);
UcpHandleString(hf_ucp_parm_RPID);
UcpHandleTime(hf_ucp_parm_SCTS);
UcpHandleByte(hf_ucp_parm_Dst);
UcpHandleInt(hf_ucp_parm_Rsn);
UcpHandleTime(hf_ucp_parm_DSCTS);
intval = UcpHandleByte(hf_ucp_parm_MT);
UcpHandleString(hf_ucp_parm_NB);
if (intval != '3')
UcpHandleData(hf_ucp_data_section);
else
UcpHandleIRAString(hf_ucp_parm_AMsg);
UcpHandleByte(hf_ucp_parm_MMS);
UcpHandleByte(hf_ucp_parm_PR);
UcpHandleByte(hf_ucp_parm_DCs);
UcpHandleByte(hf_ucp_parm_MCLs);
UcpHandleByte(hf_ucp_parm_RPI);
if (tvb_get_guint8(tvb, offset++) != '/') {
proto_tree_add_string(tree, hf_ucp_parm_CPg, tvb, offset - 1,1,
"(reserved for Code Page)");
offset++;
}
if (tvb_get_guint8(tvb, offset++) != '/') {
proto_tree_add_string(tree, hf_ucp_parm_RPLy, tvb, offset - 1,1,
"(reserved for Reply type)");
offset++;
}
UcpHandleString(hf_ucp_parm_OTOA);
UcpHandleString(hf_ucp_parm_HPLMN);
tmpoff = offset;
while (tvb_get_guint8(tvb, tmpoff++) != '/')
;
if ((tmpoff - offset) > 1) {
int len = tmpoff - offset - 1;
proto_tree *subtree;
ti = proto_tree_add_item(tree, hf_ucp_parm_XSer, tvb, offset, len, ENC_NA);
tmptvb = tvb_new_subset(tvb, offset, len + 1, len + 1);
subtree = proto_item_add_subtree(ti, ett_XSer);
ucp_handle_XSer(subtree, tmptvb);
}
offset = tmpoff;
UcpHandleDataString(hf_ucp_parm_RES4);
UcpHandleDataString(hf_ucp_parm_RES5);
}
static void
add_6xO(proto_tree *tree, tvbuff_t *tvb, guint8 OT)
{
int offset = 1;
UcpHandleString(hf_ucp_parm_OAdC);
UcpHandleByte(hf_ucp_parm_OTON);
UcpHandleByte(hf_ucp_parm_ONPI);
if (OT == 60) {
UcpHandleByte(hf_ucp_parm_STYP0);
} else {
UcpHandleByte(hf_ucp_parm_STYP1);
}
UcpHandleIRAString(hf_ucp_parm_PWD);
UcpHandleIRAString(hf_ucp_parm_NPWD);
UcpHandleString(hf_ucp_parm_VERS);
UcpHandleString(hf_ucp_parm_LAdC);
UcpHandleByte(hf_ucp_parm_LTON);
UcpHandleByte(hf_ucp_parm_LNPI);
if (OT == 60) {
UcpHandleInt(hf_ucp_parm_OPID);
}
UcpHandleDataString(hf_ucp_parm_RES1);
if (OT == 61) {
UcpHandleDataString(hf_ucp_parm_RES2);
}
}
static gboolean
dissect_ucp_heur(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void *data _U_)
{
conversation_t *conversation;
if (tvb_length(tvb) < UCP_HEADER_SIZE)
return FALSE;
if ((tvb_get_guint8(tvb, 0) != UCP_STX) ||
(tvb_get_guint8(tvb, UCP_TRN_OFFSET + UCP_TRN_LEN) != '/') ||
(tvb_get_guint8(tvb, UCP_LEN_OFFSET + UCP_LEN_LEN) != '/') ||
(tvb_get_guint8(tvb, UCP_O_R_OFFSET + UCP_O_R_LEN) != '/') ||
(tvb_get_guint8(tvb, UCP_OT_OFFSET + UCP_OT_LEN) != '/'))
return FALSE;
if (try_val_to_str(tvb_get_guint8(tvb, UCP_O_R_OFFSET), vals_hdr_O_R) == NULL)
return FALSE;
conversation = find_or_create_conversation(pinfo);
conversation_set_dissector(conversation, ucp_handle);
dissect_ucp_tcp(tvb, pinfo, tree);
return TRUE;
}
static guint
get_ucp_pdu_len(packet_info *pinfo _U_, tvbuff_t *tvb, int offset)
{
guint intval=0;
int i;
offset = offset + 4;
for (i = 0; i < UCP_LEN_LEN; i++) {
intval = 10 * intval +
(tvb_get_guint8(tvb, offset) - '0');
offset++;
}
return intval + 2;
}
static void
dissect_ucp_tcp(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
tcp_dissect_pdus(tvb, pinfo, tree, ucp_desegment, UCP_HEADER_SIZE,
get_ucp_pdu_len, dissect_ucp_common);
}
static void
dissect_ucp_common(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
int offset = 0;
guint8 O_R;
guint8 OT;
guint intval;
int i;
int result;
int endpkt;
ucp_tap_rec_t* tap_rec;
proto_item *ti;
proto_item *sub_ti;
proto_tree *ucp_tree;
proto_tree *sub_tree;
tvbuff_t *tmp_tvb;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "UCP");
col_clear(pinfo->cinfo, COL_INFO);
if (tvb_get_guint8(tvb, 0) != UCP_STX){
proto_tree_add_text(tree, tvb, 0, -1,"UCP_STX missing, this is not a new packet");
return;
}
result = check_ucp(tvb, &endpkt);
O_R = tvb_get_guint8(tvb, UCP_O_R_OFFSET);
OT = tvb_get_guint8(tvb, UCP_OT_OFFSET) - '0';
OT = 10 * OT + (tvb_get_guint8(tvb, UCP_OT_OFFSET + 1) - '0');
tap_rec = wmem_new0(wmem_packet_scope(), ucp_tap_rec_t);
tap_rec->message_type = (O_R == 'O' ? 0 : 1);
tap_rec->operation = OT;
col_append_fstr(pinfo->cinfo, COL_INFO, "%s (%s)",
val_to_str_const(OT, vals_hdr_OT, "unknown operation"),
val_to_str(O_R, vals_hdr_O_R, "Unknown (%d)"));
if (result == UCP_INV_CHK)
col_append_str(pinfo->cinfo, COL_INFO, " [checksum invalid]");
if (tree) {
ti = proto_tree_add_item(tree, proto_ucp, tvb, 0, -1, ENC_NA);
ucp_tree = proto_item_add_subtree(ti, ett_ucp);
offset++;
intval = tvb_get_guint8(tvb, offset+0) - '0';
intval = 10 * intval + (tvb_get_guint8(tvb, offset+1) - '0');
proto_tree_add_uint(ucp_tree, hf_ucp_hdr_TRN, tvb, offset,
UCP_TRN_LEN, intval);
offset += UCP_TRN_LEN + 1;
intval = 0;
for (i = 0; i < UCP_LEN_LEN; i++) {
intval = 10 * intval +
(tvb_get_guint8(tvb, offset+i) - '0');
}
proto_tree_add_uint(ucp_tree, hf_ucp_hdr_LEN, tvb, offset,
UCP_LEN_LEN, intval);
offset += UCP_LEN_LEN + 1;
proto_tree_add_uint(ucp_tree, hf_ucp_hdr_O_R, tvb, offset,
UCP_O_R_LEN, O_R);
offset += UCP_O_R_LEN + 1;
proto_tree_add_uint(ucp_tree, hf_ucp_hdr_OT, tvb, offset,
UCP_OT_LEN, OT);
offset += UCP_OT_LEN;
tmp_tvb = tvb_new_subset_remaining(tvb, offset);
sub_ti = proto_tree_add_item(ucp_tree, hf_ucp_oper_section, tvb,
offset, endpkt - offset, ENC_NA);
sub_tree = proto_item_add_subtree(sub_ti, ett_sub);
switch (OT) {
case 0:
O_R == 'O' ? add_00O(sub_tree,tmp_tvb) : add_00R(sub_tree,tmp_tvb, tap_rec);
break;
case 1:
O_R == 'O' ? add_01O(sub_tree,tmp_tvb) : add_01R(sub_tree,tmp_tvb, tap_rec);
break;
case 2:
O_R == 'O' ? add_02O(sub_tree,tmp_tvb) : add_02R(sub_tree,tmp_tvb, tap_rec);
break;
case 3:
O_R == 'O' ? add_03O(sub_tree,tmp_tvb) : add_03R(sub_tree,tmp_tvb, tap_rec);
break;
case 4:
O_R == 'O' ? add_04O(sub_tree,tmp_tvb) : add_04R(sub_tree,tmp_tvb, tap_rec);
break;
case 5:
O_R == 'O' ? add_05O(sub_tree,tmp_tvb) : add_05R(sub_tree,tmp_tvb, tap_rec);
break;
case 6:
O_R == 'O' ? add_06O(sub_tree,tmp_tvb) : add_06R(sub_tree,tmp_tvb, tap_rec);
break;
case 7:
O_R == 'O' ? add_07O(sub_tree,tmp_tvb) : add_07R(sub_tree,tmp_tvb, tap_rec);
break;
case 8:
O_R == 'O' ? add_08O(sub_tree,tmp_tvb) : add_08R(sub_tree,tmp_tvb, tap_rec);
break;
case 9:
O_R == 'O' ? add_09O(sub_tree,tmp_tvb) : add_09R(sub_tree,tmp_tvb, tap_rec);
break;
case 10:
O_R == 'O' ? add_10O(sub_tree,tmp_tvb) : add_10R(sub_tree,tmp_tvb, tap_rec);
break;
case 11:
O_R == 'O' ? add_11O(sub_tree,tmp_tvb) : add_11R(sub_tree,tmp_tvb, tap_rec);
break;
case 12:
O_R == 'O' ? add_12O(sub_tree,tmp_tvb) : add_12R(sub_tree,tmp_tvb, tap_rec);
break;
case 13:
O_R == 'O' ? add_13O(sub_tree,tmp_tvb) : add_13R(sub_tree,tmp_tvb, tap_rec);
break;
case 14:
O_R == 'O' ? add_14O(sub_tree,tmp_tvb) : add_14R(sub_tree,tmp_tvb, tap_rec);
break;
case 15:
O_R == 'O' ? add_15O(sub_tree,tmp_tvb) : add_15R(sub_tree,tmp_tvb, tap_rec);
break;
case 16:
O_R == 'O' ? add_16O(sub_tree,tmp_tvb) : add_16R(sub_tree,tmp_tvb, tap_rec);
break;
case 17:
O_R == 'O' ? add_17O(sub_tree,tmp_tvb) : add_17R(sub_tree,tmp_tvb, tap_rec);
break;
case 18:
O_R == 'O' ? add_18O(sub_tree,tmp_tvb) : add_18R(sub_tree,tmp_tvb, tap_rec);
break;
case 19:
O_R == 'O' ? add_19O(sub_tree,tmp_tvb) : add_19R(sub_tree,tmp_tvb, tap_rec);
break;
case 20:
O_R == 'O' ? add_20O(sub_tree,tmp_tvb) : add_20R(sub_tree,tmp_tvb, tap_rec);
break;
case 21:
O_R == 'O' ? add_21O(sub_tree,tmp_tvb) : add_21R(sub_tree,tmp_tvb, tap_rec);
break;
case 22:
O_R == 'O' ? add_22O(sub_tree,tmp_tvb) : add_22R(sub_tree,tmp_tvb, tap_rec);
break;
case 23:
O_R == 'O' ? add_23O(sub_tree,tmp_tvb) : add_23R(sub_tree,tmp_tvb, tap_rec);
break;
case 24:
O_R == 'O' ? add_24O(sub_tree,tmp_tvb) : add_24R(sub_tree,tmp_tvb, tap_rec);
break;
case 30:
O_R == 'O' ? add_30O(sub_tree,tmp_tvb) : add_30R(sub_tree,tmp_tvb, tap_rec);
break;
case 31:
O_R == 'O' ? add_31O(sub_tree,tmp_tvb) : add_31R(sub_tree,tmp_tvb, tap_rec);
break;
case 51: case 52: case 53: case 54: case 55: case 56: case 57:
case 58:
O_R == 'O' ? add_5xO(sub_tree,tmp_tvb) : add_5xR(sub_tree,tmp_tvb, tap_rec);
break;
case 60: case 61:
O_R == 'O' ? add_6xO(sub_tree,tmp_tvb,OT) : add_6xR(sub_tree,tmp_tvb, tap_rec);
break;
default:
break;
}
}
tap_queue_packet(ucp_tap, pinfo, tap_rec);
return;
}
void
proto_register_ucp(void)
{
static hf_register_info hf[] = {
{ &hf_ucp_hdr_TRN,
{ "Transaction Reference Number", "ucp.hdr.TRN",
FT_UINT8, BASE_DEC, NULL, 0x00,
"Transaction number for this command, used in windowing.",
HFILL
}
},
{ &hf_ucp_hdr_LEN,
{ "Length", "ucp.hdr.LEN",
FT_UINT16, BASE_DEC, NULL, 0x00,
"Total number of characters between <stx>...<etx>.",
HFILL
}
},
{ &hf_ucp_hdr_O_R,
{ "Type", "ucp.hdr.O_R",
FT_UINT8, BASE_DEC, VALS(vals_hdr_O_R), 0x00,
"Your basic 'is a request or response'.",
HFILL
}
},
{ &hf_ucp_hdr_OT,
{ "Operation", "ucp.hdr.OT",
FT_UINT8, BASE_DEC, VALS(vals_hdr_OT), 0x00,
"The operation that is requested with this message.",
HFILL
}
},
{ &hf_ucp_oper_section,
{ "Data", "ucp.parm",
FT_NONE, BASE_NONE, NULL, 0x00,
"The actual content of the operation.",
HFILL
}
},
{ &hf_ucp_parm_AdC,
{ "AdC", "ucp.parm.AdC",
FT_STRING, BASE_NONE, NULL, 0x00,
"Address code recipient.",
HFILL
}
},
{ &hf_ucp_parm_OAdC,
{ "OAdC", "ucp.parm.OAdC",
FT_STRING, BASE_NONE, NULL, 0x00,
"Address code originator.",
HFILL
}
},
{ &hf_ucp_parm_DAdC,
{ "DAdC", "ucp.parm.DAdC",
FT_STRING, BASE_NONE, NULL, 0x00,
"Diverted address code.",
HFILL
}
},
{ &hf_ucp_parm_AC,
{ "AC", "ucp.parm.AC",
FT_STRING, BASE_NONE, NULL, 0x00,
"Authentication code.",
HFILL
}
},
{ &hf_ucp_parm_OAC,
{ "OAC", "ucp.parm.OAC",
FT_STRING, BASE_NONE, NULL, 0x00,
"Authentication code, originator.",
HFILL
}
},
{ &hf_ucp_parm_NAC,
{ "NAC", "ucp.parm.NAC",
FT_STRING, BASE_NONE, NULL, 0x00,
"New authentication code.",
HFILL
}
},
{ &hf_ucp_parm_BAS,
{ "BAS", "ucp.parm.BAS",
FT_UINT8, BASE_DEC, VALS(vals_parm_BAS), 0x00,
"Barring status flag.",
HFILL
}
},
{ &hf_ucp_parm_LAR,
{ "LAR", "ucp.parm.LAR",
FT_UINT8, BASE_DEC, VALS(vals_parm_LAR), 0x00,
"Leg. code for all calls flag.",
HFILL
}
},
{ &hf_ucp_parm_LAC,
{ "LAC", "ucp.parm.LAC",
FT_STRING, BASE_NONE, NULL, 0x00,
"New leg. code for all calls.",
HFILL
}
},
{ &hf_ucp_parm_L1R,
{ "L1R", "ucp.parm.L1R",
FT_UINT8, BASE_DEC, VALS(vals_parm_L1R), 0x00,
"Leg. code for priority 1 flag.",
HFILL
}
},
{ &hf_ucp_parm_L1P,
{ "L1P", "ucp.parm.L1P",
FT_STRING, BASE_NONE, NULL, 0x00,
"New leg. code for level 1 priority.",
HFILL
}
},
{ &hf_ucp_parm_L3R,
{ "L3R", "ucp.parm.L3R",
FT_UINT8, BASE_DEC, VALS(vals_parm_L3R), 0x00,
"Leg. code for priority 3 flag.",
HFILL
}
},
{ &hf_ucp_parm_L3P,
{ "L3P", "ucp.parm.L3P",
FT_STRING, BASE_NONE, NULL, 0x00,
"New leg. code for level 3 priority.",
HFILL
}
},
{ &hf_ucp_parm_LCR,
{ "LCR", "ucp.parm.LCR",
FT_UINT8, BASE_DEC, VALS(vals_parm_LCR), 0x00,
"Leg. code for reverse charging flag.",
HFILL
}
},
{ &hf_ucp_parm_LUR,
{ "LUR", "ucp.parm.LUR",
FT_UINT8, BASE_DEC, VALS(vals_parm_LUR), 0x00,
"Leg. code for urgent message flag.",
HFILL
}
},
{ &hf_ucp_parm_LRR,
{ "LRR", "ucp.parm.LRR",
FT_UINT8, BASE_DEC, VALS(vals_parm_LRR), 0x00,
"Leg. code for repetition flag.",
HFILL
}
},
{ &hf_ucp_parm_RT,
{ "RT", "ucp.parm.RT",
FT_UINT8, BASE_DEC, VALS(vals_parm_RT), 0x00,
"Receiver type.",
HFILL
}
},
{ &hf_ucp_parm_NoN,
{ "NoN", "ucp.parm.NoN",
FT_UINT16, BASE_DEC, NULL, 0x00,
"Maximum number of numerical characters accepted.",
HFILL
}
},
{ &hf_ucp_parm_NoA,
{ "NoA", "ucp.parm.NoA",
FT_UINT16, BASE_DEC, NULL, 0x00,
"Maximum number of alphanumerical characters accepted.",
HFILL
}
},
{ &hf_ucp_parm_NoB,
{ "NoB", "ucp.parm.NoB",
FT_UINT16, BASE_DEC, NULL, 0x00,
"Maximum number of data bits accepted.",
HFILL
}
},
{ &hf_ucp_parm_PNC,
{ "PNC", "ucp.parm.PNC",
FT_UINT8, BASE_DEC, VALS(vals_parm_PNC), 0x00,
"Paging network controller.",
HFILL
}
},
{ &hf_ucp_parm_AMsg,
{ "AMsg", "ucp.parm.AMsg",
FT_STRING, BASE_NONE, NULL, 0x00,
"The alphanumeric message that is being sent.",
HFILL
}
},
{ &hf_ucp_parm_LNo,
{ "LNo", "ucp.parm.LNo",
FT_STRING, BASE_NONE, NULL, 0x00,
"Standard text list number requested by calling party.",
HFILL
}
},
{ &hf_ucp_parm_LST,
{ "LST", "ucp.parm.LST",
FT_STRING, BASE_NONE, NULL, 0x00,
"Legitimisation code for standard text.",
HFILL
}
},
{ &hf_ucp_parm_TNo,
{ "TNo", "ucp.parm.TNo",
FT_STRING, BASE_NONE, NULL, 0x00,
"Standard text number requested by calling party.",
HFILL
}
},
{ &hf_ucp_parm_CS,
{ "CS", "ucp.parm.CS",
FT_UINT8, BASE_DEC, NULL, 0x00,
"Additional character set number.",
HFILL
}
},
{ &hf_ucp_parm_PID,
{ "PID", "ucp.parm.PID",
FT_UINT16, BASE_DEC, VALS(vals_parm_PID), 0x00,
"SMT PID value.",
HFILL
}
},
{ &hf_ucp_parm_NPL,
{ "NPL", "ucp.parm.NPL",
FT_UINT16, BASE_DEC, NULL, 0x00,
"Number of parameters in the following list.",
HFILL
}
},
{ &hf_ucp_parm_GA,
{ "GA", "ucp.parm.GA",
FT_STRING, BASE_NONE, NULL, 0x00,
"GA?? haven't got a clue.",
HFILL
}
},
{ &hf_ucp_parm_RP,
{ "RP", "ucp.parm.RP",
FT_UINT8, BASE_DEC, VALS(vals_parm_RP), 0x00,
"Repetition requested.",
HFILL
}
},
{ &hf_ucp_parm_LRP,
{ "LRP", "ucp.parm.LRP",
FT_STRING, BASE_NONE, NULL, 0x00,
"Legitimisation code for repetition.",
HFILL
}
},
{ &hf_ucp_parm_PR,
{ "PR", "ucp.parm.PR",
FT_UINT8, BASE_DEC, NULL, 0x00,
"Priority requested.",
HFILL
}
},
{ &hf_ucp_parm_LPR,
{ "LPR", "ucp.parm.LPR",
FT_STRING, BASE_NONE, NULL, 0x00,
"Legitimisation code for priority requested.",
HFILL
}
},
{ &hf_ucp_parm_UM,
{ "UM", "ucp.parm.UM",
FT_UINT8, BASE_DEC, VALS(vals_parm_UM), 0x00,
"Urgent message indicator.",
HFILL
}
},
{ &hf_ucp_parm_LUM,
{ "LUM", "ucp.parm.LUM",
FT_STRING, BASE_NONE, NULL, 0x00,
"Legitimisation code for urgent message.",
HFILL
}
},
{ &hf_ucp_parm_RC,
{ "RC", "ucp.parm.RC",
FT_UINT8, BASE_DEC, VALS(vals_parm_RC), 0x00,
"Reverse charging request.",
HFILL
}
},
{ &hf_ucp_parm_LRC,
{ "LRC", "ucp.parm.LRC",
FT_STRING, BASE_NONE, NULL, 0x00,
"Legitimisation code for reverse charging.",
HFILL
}
},
{ &hf_ucp_parm_NRq,
{ "NRq", "ucp.parm.NRq",
FT_UINT8, BASE_DEC, VALS(vals_parm_NRq), 0x00,
"Notification request.",
HFILL
}
},
{ &hf_ucp_parm_GAdC,
{ "GAdC", "ucp.parm.GAdC",
FT_STRING, BASE_NONE, NULL, 0x00,
"Group address code.",
HFILL
}
},
{ &hf_ucp_parm_A_D,
{ "A_D", "ucp.parm.A_D",
FT_UINT8, BASE_DEC, VALS(vals_parm_A_D), 0x00,
"Add to/delete from fixed subscriber address list record.",
HFILL
}
},
{ &hf_ucp_parm_CT,
{ "CT", "ucp.parm.CT",
FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x00,
"Accumulated charges timestamp.",
HFILL
}
},
{ &hf_ucp_parm_AAC,
{ "AAC", "ucp.parm.AAC",
FT_STRING, BASE_NONE, NULL, 0x00,
"Accumulated charges.",
HFILL
}
},
{ &hf_ucp_parm_MNo,
{ "MNo", "ucp.parm.MNo",
FT_STRING, BASE_NONE, NULL, 0x00,
"Message number.",
HFILL
}
},
{ &hf_ucp_parm_R_T,
{ "R_T", "ucp.parm.R_T",
FT_UINT8, BASE_DEC, VALS(vals_parm_R_T), 0x00,
"Message number.",
HFILL
}
},
{ &hf_ucp_parm_NAdC,
{ "NAdC", "ucp.parm.NAdC",
FT_STRING, BASE_NONE, NULL, 0x00,
"Notification address.",
HFILL
}
},
{ &hf_ucp_parm_NT,
{ "NT", "ucp.parm.NT",
FT_UINT8, BASE_DEC, VALS(vals_parm_NT), 0x00,
"Notification type.",
HFILL
}
},
{ &hf_ucp_parm_IVR5x,
{ "IVR5x", "ucp.parm.IVR5x",
FT_STRING, BASE_NONE, NULL, 0x00,
"UCP release number supported/accepted.",
HFILL
}
},
{ &hf_ucp_parm_REQ_OT,
{ "REQ_OT", "ucp.parm.REQ_OT",
FT_UINT8, BASE_DEC, VALS(vals_parm_REQ_OT), 0x00,
"UCP release number supported/accepted.",
HFILL
}
},
{ &hf_ucp_parm_SSTAT,
{ "SSTAT", "ucp.parm.SSTAT",
FT_UINT8, BASE_DEC, VALS(vals_parm_SSTAT), 0x00,
"Supplementary services for which status is requested.",
HFILL
}
},
{ &hf_ucp_parm_LMN,
{ "LMN", "ucp.parm.LMN",
FT_UINT8, BASE_DEC, NULL, 0x00,
"Last message number.",
HFILL
}
},
{ &hf_ucp_parm_NMESS,
{ "NMESS", "ucp.parm.NMESS",
FT_UINT8, BASE_DEC, NULL, 0x00,
"Number of stored messages.",
HFILL
}
},
{ &hf_ucp_parm_NMESS_str,
{ "NMESS_str", "ucp.parm.NMESS_str",
FT_STRING, BASE_NONE, NULL, 0x00,
"Number of stored messages.",
HFILL
}
},
{ &hf_ucp_parm_NPID,
{ "NPID", "ucp.parm.NPID",
FT_UINT16, BASE_DEC, VALS(vals_parm_PID), 0x00,
"Notification PID value.",
HFILL
}
},
{ &hf_ucp_parm_LRq,
{ "LRq", "ucp.parm.LRq",
FT_UINT8, BASE_DEC, VALS(vals_parm_LRq), 0x00,
"Last resort address request.",
HFILL
}
},
{ &hf_ucp_parm_LRAd,
{ "LRAd", "ucp.parm.LRAd",
FT_STRING, BASE_NONE, NULL, 0x00,
"Last resort address.",
HFILL
}
},
{ &hf_ucp_parm_LPID,
{ "LPID", "ucp.parm.LPID",
FT_UINT16, BASE_DEC, VALS(vals_parm_PID), 0x00,
"Last resort PID value.",
HFILL
}
},
{ &hf_ucp_parm_DD,
{ "DD", "ucp.parm.DD",
FT_UINT8, BASE_DEC, VALS(vals_parm_DD), 0x00,
"Deferred delivery requested.",
HFILL
}
},
{ &hf_ucp_parm_DDT,
{ "DDT", "ucp.parm.DDT",
FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x00,
"Deferred delivery time.",
HFILL
}
},
{ &hf_ucp_parm_STx,
{ "STx", "ucp.parm.STx",
FT_NONE, BASE_NONE, NULL, 0x00,
"Standard text.",
HFILL
}
},
{ &hf_ucp_parm_ST,
{ "ST", "ucp.parm.ST",
FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x00,
"Start time.",
HFILL
}
},
{ &hf_ucp_parm_SP,
{ "SP", "ucp.parm.SP",
FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x00,
"Stop time.",
HFILL
}
},
{ &hf_ucp_parm_VP,
{ "VP", "ucp.parm.VP",
FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x00,
"Validity period.",
HFILL
}
},
{ &hf_ucp_parm_RPID,
{ "RPID", "ucp.parm.RPID",
FT_STRING, BASE_NONE, NULL, 0x00,
"Replace PID",
HFILL
}
},
{ &hf_ucp_parm_SCTS,
{ "SCTS", "ucp.parm.SCTS",
FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x00,
"Service Centre timestamp.",
HFILL
}
},
{ &hf_ucp_parm_Dst,
{ "Dst", "ucp.parm.Dst",
FT_UINT8, BASE_DEC, VALS(vals_parm_Dst), 0x00,
"Delivery status.",
HFILL
}
},
{ &hf_ucp_parm_Rsn,
{ "Rsn", "ucp.parm.Rsn",
FT_UINT16, BASE_DEC, VALS(vals_parm_Rsn), 0x00,
"Reason code.",
HFILL
}
},
{ &hf_ucp_parm_DSCTS,
{ "DSCTS", "ucp.parm.DSCTS",
FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x00,
"Delivery timestamp.",
HFILL
}
},
{ &hf_ucp_parm_MT,
{ "MT", "ucp.parm.MT",
FT_UINT8, BASE_DEC, VALS(vals_parm_MT), 0x00,
"Message type.",
HFILL
}
},
{ &hf_ucp_parm_NB,
{ "NB", "ucp.parm.NB",
FT_STRING, BASE_NONE, NULL, 0x00,
"No. of bits in Transparent Data (TD) message.",
HFILL
}
},
{ &hf_ucp_data_section,
{ "Data", "ucp.message",
FT_NONE, BASE_NONE, NULL, 0x00,
"The actual message or data.",
HFILL
}
},
{ &hf_ucp_parm_MMS,
{ "MMS", "ucp.parm.MMS",
FT_UINT8, BASE_DEC, NULL, 0x00,
"More messages to send.",
HFILL
}
},
{ &hf_ucp_parm_DCs,
{ "DCs", "ucp.parm.DCs",
FT_UINT8, BASE_DEC, VALS(vals_parm_DCs), 0x00,
"Data coding scheme (deprecated).",
HFILL
}
},
{ &hf_ucp_parm_MCLs,
{ "MCLs", "ucp.parm.MCLs",
FT_UINT8, BASE_DEC, VALS(vals_parm_MCLs), 0x00,
"Message class.",
HFILL
}
},
{ &hf_ucp_parm_RPI,
{ "RPI", "ucp.parm.RPI",
FT_UINT8, BASE_DEC, VALS(vals_parm_RPI), 0x00,
"Reply path.",
HFILL
}
},
{ &hf_ucp_parm_CPg,
{ "CPg", "ucp.parm.CPg",
FT_STRING, BASE_NONE, NULL, 0x00,
"Reserved for Code Page.",
HFILL
}
},
{ &hf_ucp_parm_RPLy,
{ "RPLy", "ucp.parm.RPLy",
FT_STRING, BASE_NONE, NULL, 0x00,
"Reserved for Reply type.",
HFILL
}
},
{ &hf_ucp_parm_OTOA,
{ "OTOA", "ucp.parm.OTOA",
FT_STRING, BASE_NONE, NULL, 0x00,
"Originator Type Of Address.",
HFILL
}
},
{ &hf_ucp_parm_HPLMN,
{ "HPLMN", "ucp.parm.HPLMN",
FT_STRING, BASE_NONE, NULL, 0x00,
"Home PLMN address.",
HFILL
}
},
{ &hf_ucp_parm_XSer,
{ "Extra services:", "ucp.parm.XSer",
FT_NONE, BASE_NONE, NULL, 0x00,
"Extra services.",
HFILL
}
},
{ &hf_ucp_parm_RES4,
{ "RES4", "ucp.parm.RES4",
FT_STRING, BASE_NONE, NULL, 0x00,
"Reserved for future use.",
HFILL
}
},
{ &hf_ucp_parm_RES5,
{ "RES5", "ucp.parm.RES5",
FT_STRING, BASE_NONE, NULL, 0x00,
"Reserved for future use.",
HFILL
}
},
{ &hf_ucp_parm_OTON,
{ "OTON", "ucp.parm.OTON",
FT_UINT8, BASE_DEC, VALS(vals_parm_OTON), 0x00,
"Originator type of number.",
HFILL
}
},
{ &hf_ucp_parm_ONPI,
{ "ONPI", "ucp.parm.ONPI",
FT_UINT8, BASE_DEC, VALS(vals_parm_ONPI), 0x00,
"Originator numbering plan id.",
HFILL
}
},
{ &hf_ucp_parm_STYP0,
{ "STYP0", "ucp.parm.STYP0",
FT_UINT8, BASE_DEC, VALS(vals_parm_STYP0), 0x00,
"Subtype of operation.",
HFILL
}
},
{ &hf_ucp_parm_STYP1,
{ "STYP1", "ucp.parm.STYP1",
FT_UINT8, BASE_DEC, VALS(vals_parm_STYP1), 0x00,
"Subtype of operation.",
HFILL
}
},
{ &hf_ucp_parm_PWD,
{ "PWD", "ucp.parm.PWD",
FT_STRING, BASE_NONE, NULL, 0x00,
"Current password.",
HFILL
}
},
{ &hf_ucp_parm_NPWD,
{ "NPWD", "ucp.parm.NPWD",
FT_STRING, BASE_NONE, NULL, 0x00,
"New password.",
HFILL
}
},
{ &hf_ucp_parm_VERS,
{ "VERS", "ucp.parm.VERS",
FT_STRING, BASE_NONE, NULL, 0x00,
"Version number.",
HFILL
}
},
{ &hf_ucp_parm_LAdC,
{ "LAdC", "ucp.parm.LAdC",
FT_STRING, BASE_NONE, NULL, 0x00,
"Address for VSMSC list operation.",
HFILL
}
},
{ &hf_ucp_parm_LTON,
{ "LTON", "ucp.parm.LTON",
FT_UINT8, BASE_DEC, NULL, 0x00,
"Type of number list address.",
HFILL
}
},
{ &hf_ucp_parm_LNPI,
{ "LNPI", "ucp.parm.LNPI",
FT_UINT8, BASE_DEC, NULL, 0x00,
"Numbering plan id. list address.",
HFILL
}
},
{ &hf_ucp_parm_OPID,
{ "OPID", "ucp.parm.OPID",
FT_UINT8, BASE_DEC, VALS(vals_parm_OPID), 0x00,
"Originator protocol identifier.",
HFILL
}
},
{ &hf_ucp_parm_RES1,
{ "RES1", "ucp.parm.RES1",
FT_STRING, BASE_NONE, NULL, 0x00,
"Reserved for future use.",
HFILL
}
},
{ &hf_ucp_parm_RES2,
{ "RES2", "ucp.parm.RES2",
FT_STRING, BASE_NONE, NULL, 0x00,
"Reserved for future use.",
HFILL
}
},
{ &hf_ucp_parm_ACK,
{ "(N)Ack", "ucp.parm.ACK",
FT_UINT8, BASE_DEC, VALS(vals_parm_ACK), 0x00,
"Positive or negative acknowledge of the operation.",
HFILL
}
},
{ &hf_ucp_parm_MVP,
{ "MVP", "ucp.parm.MVP",
FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x00,
"Modified validity period.",
HFILL
}
},
{ &hf_ucp_parm_EC,
{ "Error code", "ucp.parm.EC",
FT_UINT8, BASE_DEC, VALS(vals_parm_EC), 0x00,
"The result of the requested operation.",
HFILL
}
},
{ &hf_ucp_parm_SM,
{ "SM", "ucp.parm.SM",
FT_STRING, BASE_NONE, NULL, 0x00,
"System message.",
HFILL
}
},
{ &hf_xser_service,
{ "Type of service", "ucp.xser.service",
FT_UINT8, BASE_HEX, VALS(vals_xser_service), 0x00,
"The type of service specified.",
HFILL
}
},
{ &hf_xser_length,
{ "Length", "ucp.xser.length",
FT_UINT16, BASE_DEC, NULL, 0x00,
NULL,
HFILL
}
},
{ &hf_xser_data,
{ "Data", "ucp.xser.data",
FT_STRING, BASE_NONE, NULL, 0x00,
NULL,
HFILL
}
},
};
static gint *ett[] = {
&ett_ucp,
&ett_sub,
&ett_XSer
};
module_t *ucp_module;
proto_ucp = proto_register_protocol("Universal Computer Protocol",
"UCP", "ucp");
proto_register_field_array(proto_ucp, hf, array_length(hf));
proto_register_subtree_array(ett, array_length(ett));
ucp_tap = register_tap("ucp");
ucp_module = prefs_register_protocol(proto_ucp, NULL);
prefs_register_bool_preference(ucp_module, "desegment_ucp_messages",
"Reassemble UCP messages spanning multiple TCP segments",
"Whether the UCP dissector should reassemble messages spanning"
" multiple TCP segments."
" To use this option, you must also enable "
"\"Allow subdissectors to reassemble TCP streams\" in the "
"TCP protocol settings.",
&ucp_desegment);
}
void
proto_reg_handoff_ucp(void)
{
heur_dissector_add("tcp", dissect_ucp_heur, proto_ucp);
ucp_handle = create_dissector_handle(dissect_ucp_tcp, proto_ucp);
dissector_add_handle("tcp.port", ucp_handle);
stats_tree_register_with_group("ucp", "ucp_messages", "_UCP Messages", 0,
ucp_stats_tree_per_packet, ucp_stats_tree_init,
NULL, REGISTER_STAT_GROUP_TELEPHONY);
}
