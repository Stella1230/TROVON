static guint get_data_func_id(tvbuff_t *tvb, int offset)
{
guint8 first_byte;
first_byte =
tvb_reported_length_remaining(tvb, offset) > 0 ? tvb_get_guint8(tvb, offset) : 0;
if ( tvb_bytes_exist(tvb, offset, 4) && first_byte == 0xDE &&
tvb_get_guint24(tvb, offset+1, ENC_BIG_ENDIAN) == 0xADBEEF )
{
return SQLNET_SNS;
}
else
{
return (guint)first_byte;
}
}
static void vsnum_to_vstext_basecustom(gchar *result, guint32 vsnum)
{
g_snprintf(result, ITEM_LABEL_LENGTH, "%d.%d.%d.%d.%d",
vsnum >> 24,
(vsnum >> 20) & 0xf,
(vsnum >> 12) & 0xf,
(vsnum >> 8) & 0xf,
vsnum & 0xff);
}
static void dissect_tns_data(tvbuff_t *tvb, int offset, packet_info *pinfo, proto_tree *tns_tree)
{
proto_tree *data_tree;
guint data_func_id;
gboolean is_request;
static const int * flags[] = {
&hf_tns_data_flag_send,
&hf_tns_data_flag_rc,
&hf_tns_data_flag_c,
&hf_tns_data_flag_reserved,
&hf_tns_data_flag_more,
&hf_tns_data_flag_eof,
&hf_tns_data_flag_dic,
&hf_tns_data_flag_rts,
&hf_tns_data_flag_sntt,
NULL
};
is_request = pinfo->match_uint == pinfo->destport;
data_tree = proto_tree_add_subtree(tns_tree, tvb, offset, -1, ett_tns_data, NULL, "Data");
proto_tree_add_bitmask(data_tree, tvb, offset, hf_tns_data_flag, ett_tns_data_flag, flags, ENC_BIG_ENDIAN);
offset += 2;
data_func_id = get_data_func_id(tvb, offset);
if ( tvb_reported_length_remaining(tvb, offset) > 0 )
{
col_append_fstr(pinfo->cinfo, COL_INFO, ", %s", val_to_str_const(data_func_id, tns_data_funcs, "unknown"));
if ( (data_func_id != SQLNET_SNS) && (try_val_to_str(data_func_id, tns_data_funcs) != NULL) )
{
proto_tree_add_item(data_tree, hf_tns_data_id, tvb, offset, 1, ENC_BIG_ENDIAN);
offset += 1;
}
}
switch (data_func_id)
{
case SQLNET_SET_PROTOCOL:
{
proto_tree *versions_tree;
proto_item *ti;
char sep;
if ( is_request )
{
versions_tree = proto_tree_add_subtree(data_tree, tvb, offset, -1, ett_tns_acc_versions, &ti, "Accepted Versions");
sep = ':';
for (;;) {
guint8 vers;
vers = tvb_get_guint8(tvb, offset);
if (vers == 0) {
break;
}
proto_item_append_text(ti, "%c %u", sep, vers);
sep = ',';
proto_tree_add_uint(versions_tree, hf_tns_data_setp_acc_version, tvb, offset, 1, vers);
offset += 1;
}
offset += 1;
proto_item_set_end(ti, tvb, offset);
proto_tree_add_item(data_tree, hf_tns_data_setp_cli_plat, tvb, offset, -1, ENC_ASCII|ENC_NA);
return;
}
else
{
gint len;
versions_tree = proto_tree_add_subtree(data_tree, tvb, offset, -1, ett_tns_acc_versions, &ti, "Versions");
sep = ':';
for (;;) {
guint8 vers;
vers = tvb_get_guint8(tvb, offset);
if (vers == 0) {
break;
}
proto_item_append_text(ti, "%c %u", sep, vers);
sep = ',';
proto_tree_add_uint(versions_tree, hf_tns_data_setp_version, tvb, offset, 1, vers);
offset += 1;
}
offset += 1;
proto_item_set_end(ti, tvb, offset);
proto_tree_add_item_ret_length(data_tree, hf_tns_data_setp_banner, tvb, offset, -1, ENC_ASCII|ENC_NA, &len);
offset += len;
}
break;
}
case SQLNET_USER_OCI_FUNC:
proto_tree_add_item(data_tree, hf_tns_data_oci_id, tvb, offset, 1, ENC_BIG_ENDIAN);
offset += 1;
break;
case SQLNET_RETURN_OPI_PARAM:
{
guint8 skip = 0, opi = 0;
if ( tvb_bytes_exist(tvb, offset, 11) )
{
if ( tvb_get_ntohl(tvb, offset+2) == 0x4f726163 )
opi = OPI_VERSION2, skip = 1;
else if ( tvb_get_ntohl(tvb, offset+3) == 0x4f726163 )
opi = OPI_VERSION2, skip = 2;
else if ( tvb_get_ntoh64(tvb, offset+3) == 0x0000000c41555448 )
opi = OPI_OSESSKEY, skip = 1;
else if ( tvb_get_ntoh64(tvb, offset+3) == 0x0c0c415554485f53 )
opi = OPI_OSESSKEY, skip = 2;
else if ( tvb_get_ntoh64(tvb, offset+3) == 0x0000001341555448 )
opi = OPI_OAUTH, skip = 1;
else if ( tvb_get_ntoh64(tvb, offset+3) == 0x1313415554485f56 )
opi = OPI_OAUTH, skip = 2;
}
if ( opi == OPI_VERSION2 )
{
proto_tree_add_item(data_tree, hf_tns_data_unused, tvb, offset, skip, ENC_NA);
offset += skip;
guint8 len = tvb_get_guint8(tvb, offset);
proto_tree_add_item(data_tree, hf_tns_data_opi_version2_banner_len, tvb, offset, 1, ENC_BIG_ENDIAN);
offset += 1;
proto_tree_add_item(data_tree, hf_tns_data_opi_version2_banner, tvb, offset, len, ENC_ASCII|ENC_NA);
offset += len + (skip == 1 ? 1 : 0);
proto_tree_add_item(data_tree, hf_tns_data_opi_version2_vsnum, tvb, offset, 4, skip == 1 ? ENC_BIG_ENDIAN : ENC_LITTLE_ENDIAN);
offset += 4;
}
else if ( opi == OPI_OSESSKEY || opi == OPI_OAUTH )
{
proto_tree *params_tree;
proto_item *params_ti;
guint par, params;
if ( skip == 1 )
{
proto_tree_add_item_ret_uint(data_tree, hf_tns_data_opi_num_of_params, tvb, offset, 1, ENC_NA, &params);
offset += 1;
proto_tree_add_item(data_tree, hf_tns_data_unused, tvb, offset, 5, ENC_NA);
offset += 5;
}
else
{
proto_tree_add_item(data_tree, hf_tns_data_unused, tvb, offset, 1, ENC_NA);
offset += 1;
proto_tree_add_item_ret_uint(data_tree, hf_tns_data_opi_num_of_params, tvb, offset, 1, ENC_NA, &params);
offset += 1;
proto_tree_add_item(data_tree, hf_tns_data_unused, tvb, offset, 2, ENC_NA);
offset += 2;
}
params_tree = proto_tree_add_subtree(data_tree, tvb, offset, -1, ett_tns_opi_params, &params_ti, "Parameters");
for ( par = 1; par <= params; par++ )
{
proto_tree *par_tree;
proto_item *par_ti;
guint len, offset_prev;
par_tree = proto_tree_add_subtree(params_tree, tvb, offset, -1, ett_tns_opi_par, &par_ti, "Parameter");
proto_item_append_text(par_ti, " %u", par);
proto_tree_add_item_ret_uint(par_tree, hf_tns_data_opi_param_length, tvb, offset, 1, ENC_NA, &len);
offset += 1;
if ( !(len == 0 || len == 2) )
{
proto_tree_add_item(par_tree, hf_tns_data_opi_param_name, tvb, offset, len, ENC_ASCII|ENC_NA);
offset += len;
}
offset_prev = offset;
offset += skip == 1 ? 4 : 2;
if ( opi == OPI_OSESSKEY )
{
len = tvb_get_guint8(tvb, offset);
}
else
{
len = tvb_get_guint8(tvb, offset_prev) == 0 ? 0 : tvb_get_guint8(tvb, offset);
}
if ( ((opi == OPI_OSESSKEY) && !(len == 0 || len == 9 || len == 0x39))
|| ((opi == OPI_OAUTH) && !(len == 0 || len == 0x39)) )
{
proto_tree_add_item(par_tree, hf_tns_data_unused, tvb, offset_prev, offset - offset_prev, ENC_NA);
proto_tree_add_item(par_tree, hf_tns_data_opi_param_length, tvb, offset, 1, ENC_NA);
offset += 1;
proto_tree_add_item(par_tree, hf_tns_data_opi_param_value, tvb, offset, len, ENC_ASCII|ENC_NA);
offset += len;
offset_prev = offset;
}
else
{
offset += 1;
}
if ( opi == OPI_OSESSKEY )
{
offset += tvb_get_guint8(tvb, offset) == 2 ? 5 : 3;
}
else
{
offset += len == 0 ? 1 : 3;
}
if ( skip == 1 )
{
offset += 1 + ((len == 0 || len == 0x39) ? 3 : 4);
if ( opi == OPI_OAUTH )
{
offset += len == 0 ? 2 : 0;
}
}
proto_tree_add_item(par_tree, hf_tns_data_unused, tvb, offset_prev, offset - offset_prev, ENC_NA);
proto_item_set_end(par_ti, tvb, offset);
}
proto_item_set_end(params_ti, tvb, offset);
}
break;
}
case SQLNET_PIGGYBACK_FUNC:
proto_tree_add_item(data_tree, hf_tns_data_piggyback_id, tvb, offset, 1, ENC_BIG_ENDIAN);
offset += 1;
break;
case SQLNET_SNS:
{
proto_tree_add_item(data_tree, hf_tns_data_id, tvb, offset, 4, ENC_BIG_ENDIAN);
offset += 4;
proto_tree_add_item(data_tree, hf_tns_data_length, tvb, offset, 2, ENC_BIG_ENDIAN);
offset += 2;
if ( is_request )
{
proto_tree_add_item(data_tree, hf_tns_data_sns_cli_vers, tvb, offset, 4, ENC_BIG_ENDIAN);
}
else
{
proto_tree_add_item(data_tree, hf_tns_data_sns_srv_vers, tvb, offset, 4, ENC_BIG_ENDIAN);
}
offset += 4;
proto_tree_add_item(data_tree, hf_tns_data_sns_srvcnt, tvb, offset, 2, ENC_BIG_ENDIAN);
offset -= 10;
break;
}
}
call_data_dissector(tvb_new_subset_remaining(tvb, offset), pinfo, data_tree);
}
static void dissect_tns_connect(tvbuff_t *tvb, int offset, packet_info *pinfo _U_, proto_tree *tns_tree)
{
proto_tree *connect_tree;
guint32 cd_offset, cd_len;
int tns_offset = offset-8;
static const int * flags[] = {
&hf_tns_ntp_flag_hangon,
&hf_tns_ntp_flag_crel,
&hf_tns_ntp_flag_tduio,
&hf_tns_ntp_flag_srun,
&hf_tns_ntp_flag_dtest,
&hf_tns_ntp_flag_cbio,
&hf_tns_ntp_flag_asio,
&hf_tns_ntp_flag_pio,
&hf_tns_ntp_flag_grant,
&hf_tns_ntp_flag_handoff,
&hf_tns_ntp_flag_sigio,
&hf_tns_ntp_flag_sigpipe,
&hf_tns_ntp_flag_sigurg,
&hf_tns_ntp_flag_urgentio,
&hf_tns_ntp_flag_fdio,
&hf_tns_ntp_flag_testop,
NULL
};
connect_tree = proto_tree_add_subtree(tns_tree, tvb, offset, -1,
ett_tns_connect, NULL, "Connect");
proto_tree_add_item(connect_tree, hf_tns_version, tvb,
offset, 2, ENC_BIG_ENDIAN);
offset += 2;
proto_tree_add_item(connect_tree, hf_tns_compat_version, tvb,
offset, 2, ENC_BIG_ENDIAN);
offset += 2;
proto_tree_add_bitmask(connect_tree, tvb, offset, hf_tns_service_options, ett_tns_sopt_flag, tns_service_options, ENC_BIG_ENDIAN);
offset += 2;
proto_tree_add_item(connect_tree, hf_tns_sdu_size, tvb,
offset, 2, ENC_BIG_ENDIAN);
offset += 2;
proto_tree_add_item(connect_tree, hf_tns_max_tdu_size, tvb,
offset, 2, ENC_BIG_ENDIAN);
offset += 2;
proto_tree_add_bitmask(connect_tree, tvb, offset, hf_tns_nt_proto_characteristics, ett_tns_ntp_flag, flags, ENC_BIG_ENDIAN);
offset += 2;
proto_tree_add_item(connect_tree, hf_tns_line_turnaround, tvb,
offset, 2, ENC_BIG_ENDIAN);
offset += 2;
proto_tree_add_item(connect_tree, hf_tns_value_of_one, tvb,
offset, 2, ENC_NA);
offset += 2;
proto_tree_add_item_ret_uint(connect_tree, hf_tns_connect_data_length, tvb,
offset, 2, ENC_BIG_ENDIAN, &cd_len);
offset += 2;
proto_tree_add_item_ret_uint(connect_tree, hf_tns_connect_data_offset, tvb,
offset, 2, ENC_BIG_ENDIAN, &cd_offset);
offset += 2;
proto_tree_add_item(connect_tree, hf_tns_connect_data_max, tvb,
offset, 4, ENC_BIG_ENDIAN);
offset += 4;
proto_tree_add_bitmask(connect_tree, tvb, offset, hf_tns_connect_flags0, ett_tns_conn_flag, tns_connect_flags, ENC_BIG_ENDIAN);
offset += 1;
proto_tree_add_bitmask(connect_tree, tvb, offset, hf_tns_connect_flags1, ett_tns_conn_flag, tns_connect_flags, ENC_BIG_ENDIAN);
offset += 1;
if ((guint32)(offset + 16) <= tns_offset+cd_offset)
{
proto_tree_add_item(connect_tree, hf_tns_trace_cf1, tvb,
offset, 4, ENC_BIG_ENDIAN);
offset += 4;
proto_tree_add_item(connect_tree, hf_tns_trace_cf2, tvb,
offset, 4, ENC_BIG_ENDIAN);
offset += 4;
proto_tree_add_item(connect_tree, hf_tns_trace_cid, tvb,
offset, 8, ENC_BIG_ENDIAN);
}
if ( cd_len > 0)
{
proto_tree_add_item(connect_tree, hf_tns_connect_data, tvb,
tns_offset+cd_offset, -1, ENC_ASCII|ENC_NA);
}
}
static void dissect_tns_accept(tvbuff_t *tvb, int offset, packet_info *pinfo _U_, proto_tree *tns_tree)
{
proto_tree *accept_tree;
guint32 accept_offset, accept_len;
int tns_offset = offset-8;
accept_tree = proto_tree_add_subtree(tns_tree, tvb, offset, -1,
ett_tns_accept, NULL, "Accept");
proto_tree_add_item(accept_tree, hf_tns_version, tvb,
offset, 2, ENC_BIG_ENDIAN);
offset += 2;
proto_tree_add_bitmask(accept_tree, tvb, offset, hf_tns_service_options, ett_tns_sopt_flag, tns_service_options, ENC_BIG_ENDIAN);
offset += 2;
proto_tree_add_item(accept_tree, hf_tns_sdu_size, tvb,
offset, 2, ENC_BIG_ENDIAN);
offset += 2;
proto_tree_add_item(accept_tree, hf_tns_max_tdu_size, tvb,
offset, 2, ENC_BIG_ENDIAN);
offset += 2;
proto_tree_add_item(accept_tree, hf_tns_value_of_one, tvb,
offset, 2, ENC_NA);
offset += 2;
proto_tree_add_item_ret_uint(accept_tree, hf_tns_accept_data_length, tvb,
offset, 2, ENC_BIG_ENDIAN, &accept_len);
offset += 2;
proto_tree_add_item_ret_uint(accept_tree, hf_tns_accept_data_offset, tvb,
offset, 2, ENC_BIG_ENDIAN, &accept_offset);
offset += 2;
proto_tree_add_bitmask(accept_tree, tvb, offset, hf_tns_connect_flags0, ett_tns_conn_flag, tns_connect_flags, ENC_BIG_ENDIAN);
offset += 1;
proto_tree_add_bitmask(accept_tree, tvb, offset, hf_tns_connect_flags1, ett_tns_conn_flag, tns_connect_flags, ENC_BIG_ENDIAN);
if ( accept_len > 0)
{
proto_tree_add_item(accept_tree, hf_tns_accept_data, tvb,
tns_offset+accept_offset, -1, ENC_ASCII|ENC_NA);
}
return;
}
static void dissect_tns_refuse(tvbuff_t *tvb, int offset, packet_info *pinfo _U_, proto_tree *tns_tree)
{
proto_tree *refuse_tree;
refuse_tree = proto_tree_add_subtree(tns_tree, tvb, offset, -1,
ett_tns_refuse, NULL, "Refuse");
proto_tree_add_item(refuse_tree, hf_tns_refuse_reason_user, tvb,
offset, 1, ENC_BIG_ENDIAN);
offset += 1;
proto_tree_add_item(refuse_tree, hf_tns_refuse_reason_system, tvb,
offset, 1, ENC_BIG_ENDIAN);
offset += 1;
proto_tree_add_item(refuse_tree, hf_tns_refuse_data_length, tvb,
offset, 2, ENC_BIG_ENDIAN);
offset += 2;
proto_tree_add_item(refuse_tree, hf_tns_refuse_data, tvb,
offset, -1, ENC_ASCII|ENC_NA);
}
static void dissect_tns_abort(tvbuff_t *tvb, int offset, packet_info *pinfo _U_, proto_tree *tns_tree)
{
proto_tree *abort_tree;
abort_tree = proto_tree_add_subtree(tns_tree, tvb, offset, -1,
ett_tns_abort, NULL, "Abort");
proto_tree_add_item(abort_tree, hf_tns_abort_reason_user, tvb,
offset, 1, ENC_BIG_ENDIAN);
offset += 1;
proto_tree_add_item(abort_tree, hf_tns_abort_reason_system, tvb,
offset, 1, ENC_BIG_ENDIAN);
offset += 1;
proto_tree_add_item(abort_tree, hf_tns_abort_data, tvb,
offset, -1, ENC_ASCII|ENC_NA);
}
static void dissect_tns_marker(tvbuff_t *tvb, int offset, packet_info *pinfo _U_, proto_tree *tns_tree, int is_attention)
{
proto_tree *marker_tree;
if ( is_attention )
{
marker_tree = proto_tree_add_subtree(tns_tree, tvb, offset, -1,
ett_tns_marker, NULL, "Marker");
}
else
{
marker_tree = proto_tree_add_subtree(tns_tree, tvb, offset, -1,
ett_tns_marker, NULL, "Attention");
}
proto_tree_add_item(marker_tree, hf_tns_marker_type, tvb,
offset, 1, ENC_BIG_ENDIAN);
offset += 1;
proto_tree_add_item(marker_tree, hf_tns_marker_data_byte, tvb,
offset, 1, ENC_BIG_ENDIAN);
offset += 1;
proto_tree_add_item(marker_tree, hf_tns_marker_data_byte, tvb,
offset, 1, ENC_BIG_ENDIAN);
}
static void dissect_tns_redirect(tvbuff_t *tvb, int offset, packet_info *pinfo _U_, proto_tree *tns_tree)
{
proto_tree *redirect_tree;
redirect_tree = proto_tree_add_subtree(tns_tree, tvb, offset, -1,
ett_tns_redirect, NULL, "Redirect");
proto_tree_add_item(redirect_tree, hf_tns_redirect_data_length, tvb,
offset, 2, ENC_BIG_ENDIAN);
offset += 2;
proto_tree_add_item(redirect_tree, hf_tns_redirect_data, tvb,
offset, -1, ENC_ASCII|ENC_NA);
}
static void dissect_tns_control(tvbuff_t *tvb, int offset, packet_info *pinfo _U_, proto_tree *tns_tree)
{
proto_tree *control_tree;
control_tree = proto_tree_add_subtree(tns_tree, tvb, offset, -1,
ett_tns_control, NULL, "Control");
proto_tree_add_item(control_tree, hf_tns_control_cmd, tvb,
offset, 2, ENC_BIG_ENDIAN);
offset += 2;
proto_tree_add_item(control_tree, hf_tns_control_data, tvb,
offset, -1, ENC_NA);
}
static guint
get_tns_pdu_len(packet_info *pinfo _U_, tvbuff_t *tvb, int offset, void *data _U_)
{
return tvb_get_ntohs(tvb, offset);
}
static guint
get_tns_pdu_len_nochksum(packet_info *pinfo _U_, tvbuff_t *tvb, int offset, void *data _U_)
{
return tvb_get_ntohl(tvb, offset);
}
static int
dissect_tns(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void *data)
{
guint32 length;
guint16 chksum;
guint8 type;
if (tvb_bytes_exist(tvb, 4, 1)) {
type = tvb_get_guint8(tvb, 4);
if (type < TNS_TYPE_CONNECT || type > TNS_TYPE_MAX)
return 0;
}
chksum = tvb_get_ntohs(tvb, 2);
length = (chksum == 0 || chksum == 4) ? 2 : 4;
tcp_dissect_pdus(tvb, pinfo, tree, tns_desegment, length,
(length == 2 ? get_tns_pdu_len : get_tns_pdu_len_nochksum),
dissect_tns_pdu, data);
return tvb_captured_length(tvb);
}
static int
dissect_tns_pdu(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void* data _U_)
{
proto_tree *tns_tree, *ti;
proto_item *hidden_item;
int offset = 0;
guint32 length;
guint16 chksum;
guint8 type;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "TNS");
col_set_str(pinfo->cinfo, COL_INFO,
(pinfo->match_uint == pinfo->destport) ? "Request" : "Response");
ti = proto_tree_add_item(tree, proto_tns, tvb, 0, -1, ENC_NA);
tns_tree = proto_item_add_subtree(ti, ett_tns);
if (pinfo->match_uint == pinfo->destport)
{
hidden_item = proto_tree_add_boolean(tns_tree, hf_tns_request,
tvb, offset, 0, TRUE);
}
else
{
hidden_item = proto_tree_add_boolean(tns_tree, hf_tns_response,
tvb, offset, 0, TRUE);
}
PROTO_ITEM_SET_HIDDEN(hidden_item);
chksum = tvb_get_ntohs(tvb, offset+2);
if (chksum == 0 || chksum == 4)
{
proto_tree_add_item_ret_uint(tns_tree, hf_tns_length, tvb, offset,
2, ENC_BIG_ENDIAN, &length);
offset += 2;
proto_tree_add_checksum(tns_tree, tvb, offset, hf_tns_packet_checksum,
-1, NULL, pinfo, 0, ENC_BIG_ENDIAN, PROTO_CHECKSUM_NO_FLAGS);
offset += 2;
}
else
{
proto_tree_add_item_ret_uint(tns_tree, hf_tns_length, tvb, offset,
4, ENC_BIG_ENDIAN, &length);
offset += 4;
}
type = tvb_get_guint8(tvb, offset);
proto_tree_add_uint(tns_tree, hf_tns_packet_type, tvb,
offset, 1, type);
offset += 1;
col_append_fstr(pinfo->cinfo, COL_INFO, ", %s (%u)",
val_to_str_const(type, tns_type_vals, "Unknown"), type);
proto_tree_add_item(tns_tree, hf_tns_reserved_byte, tvb,
offset, 1, ENC_NA);
offset += 1;
proto_tree_add_checksum(tns_tree, tvb, offset, hf_tns_header_checksum, -1, NULL, pinfo, 0, ENC_BIG_ENDIAN, PROTO_CHECKSUM_NO_FLAGS);
offset += 2;
switch (type)
{
case TNS_TYPE_CONNECT:
dissect_tns_connect(tvb,offset,pinfo,tns_tree);
break;
case TNS_TYPE_ACCEPT:
dissect_tns_accept(tvb,offset,pinfo,tns_tree);
break;
case TNS_TYPE_REFUSE:
dissect_tns_refuse(tvb,offset,pinfo,tns_tree);
break;
case TNS_TYPE_REDIRECT:
dissect_tns_redirect(tvb,offset,pinfo,tns_tree);
break;
case TNS_TYPE_ABORT:
dissect_tns_abort(tvb,offset,pinfo,tns_tree);
break;
case TNS_TYPE_MARKER:
dissect_tns_marker(tvb,offset,pinfo,tns_tree, 0);
break;
case TNS_TYPE_ATTENTION:
dissect_tns_marker(tvb,offset,pinfo,tns_tree, 1);
break;
case TNS_TYPE_CONTROL:
dissect_tns_control(tvb,offset,pinfo,tns_tree);
break;
case TNS_TYPE_DATA:
dissect_tns_data(tvb,offset,pinfo,tns_tree);
break;
default:
call_data_dissector(tvb_new_subset_remaining(tvb, offset), pinfo,
tns_tree);
break;
}
return tvb_captured_length(tvb);
}
void proto_register_tns(void)
{
static hf_register_info hf[] = {
{ &hf_tns_response, {
"Response", "tns.response", FT_BOOLEAN, BASE_NONE,
NULL, 0x0, "TRUE if TNS response", HFILL }},
{ &hf_tns_request, {
"Request", "tns.request", FT_BOOLEAN, BASE_NONE,
NULL, 0x0, "TRUE if TNS request", HFILL }},
{ &hf_tns_length, {
"Packet Length", "tns.length", FT_UINT16, BASE_DEC,
NULL, 0x0, "Length of TNS packet", HFILL }},
{ &hf_tns_packet_checksum, {
"Packet Checksum", "tns.packet_checksum", FT_UINT16, BASE_HEX,
NULL, 0x0, "Checksum of Packet Data", HFILL }},
{ &hf_tns_header_checksum, {
"Header Checksum", "tns.header_checksum", FT_UINT16, BASE_HEX,
NULL, 0x0, "Checksum of Header Data", HFILL }},
{ &hf_tns_version, {
"Version", "tns.version", FT_UINT16, BASE_DEC,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_compat_version, {
"Version (Compatible)", "tns.compat_version", FT_UINT16, BASE_DEC,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_service_options, {
"Service Options", "tns.service_options", FT_UINT16, BASE_HEX,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_sopt_flag_bconn, {
"Broken Connect Notify", "tns.so_flag.bconn", FT_BOOLEAN, 16,
NULL, 0x2000, NULL, HFILL }},
{ &hf_tns_sopt_flag_pc, {
"Packet Checksum", "tns.so_flag.pc", FT_BOOLEAN, 16,
NULL, 0x1000, NULL, HFILL }},
{ &hf_tns_sopt_flag_hc, {
"Header Checksum", "tns.so_flag.hc", FT_BOOLEAN, 16,
NULL, 0x0800, NULL, HFILL }},
{ &hf_tns_sopt_flag_fd, {
"Full Duplex", "tns.so_flag.fd", FT_BOOLEAN, 16,
NULL, 0x0400, NULL, HFILL }},
{ &hf_tns_sopt_flag_hd, {
"Half Duplex", "tns.so_flag.hd", FT_BOOLEAN, 16,
NULL, 0x0200, NULL, HFILL }},
{ &hf_tns_sopt_flag_dc1, {
"Don't Care", "tns.so_flag.dc1", FT_BOOLEAN, 16,
NULL, 0x0100, NULL, HFILL }},
{ &hf_tns_sopt_flag_dc2, {
"Don't Care", "tns.so_flag.dc2", FT_BOOLEAN, 16,
NULL, 0x0080, NULL, HFILL }},
{ &hf_tns_sopt_flag_dio, {
"Direct IO to Transport", "tns.so_flag.dio", FT_BOOLEAN, 16,
NULL, 0x0010, NULL, HFILL }},
{ &hf_tns_sopt_flag_ap, {
"Attention Processing", "tns.so_flag.ap", FT_BOOLEAN, 16,
NULL, 0x0008, NULL, HFILL }},
{ &hf_tns_sopt_flag_ra, {
"Can Receive Attention", "tns.so_flag.ra", FT_BOOLEAN, 16,
NULL, 0x0004, NULL, HFILL }},
{ &hf_tns_sopt_flag_sa, {
"Can Send Attention", "tns.so_flag.sa", FT_BOOLEAN, 16,
NULL, 0x0002, NULL, HFILL }},
{ &hf_tns_sdu_size, {
"Session Data Unit Size", "tns.sdu_size", FT_UINT16, BASE_DEC,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_max_tdu_size, {
"Maximum Transmission Data Unit Size", "tns.max_tdu_size", FT_UINT16, BASE_DEC,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_nt_proto_characteristics, {
"NT Protocol Characteristics", "tns.nt_proto_characteristics", FT_UINT16, BASE_HEX,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_ntp_flag_hangon, {
"Hangon to listener connect", "tns.ntp_flag.hangon", FT_BOOLEAN, 16,
NULL, 0x8000, NULL, HFILL }},
{ &hf_tns_ntp_flag_crel, {
"Confirmed release", "tns.ntp_flag.crel", FT_BOOLEAN, 16,
NULL, 0x4000, NULL, HFILL }},
{ &hf_tns_ntp_flag_tduio, {
"TDU based IO", "tns.ntp_flag.tduio", FT_BOOLEAN, 16,
NULL, 0x2000, NULL, HFILL }},
{ &hf_tns_ntp_flag_srun, {
"Spawner running", "tns.ntp_flag.srun", FT_BOOLEAN, 16,
NULL, 0x1000, NULL, HFILL }},
{ &hf_tns_ntp_flag_dtest, {
"Data test", "tns.ntp_flag.dtest", FT_BOOLEAN, 16,
NULL, 0x0800, NULL, HFILL }},
{ &hf_tns_ntp_flag_cbio, {
"Callback IO supported", "tns.ntp_flag.cbio", FT_BOOLEAN, 16,
NULL, 0x0400, NULL, HFILL }},
{ &hf_tns_ntp_flag_asio, {
"ASync IO Supported", "tns.ntp_flag.asio", FT_BOOLEAN, 16,
NULL, 0x0200, NULL, HFILL }},
{ &hf_tns_ntp_flag_pio, {
"Packet oriented IO", "tns.ntp_flag.pio", FT_BOOLEAN, 16,
NULL, 0x0100, NULL, HFILL }},
{ &hf_tns_ntp_flag_grant, {
"Can grant connection to another", "tns.ntp_flag.grant", FT_BOOLEAN, 16,
NULL, 0x0080, NULL, HFILL }},
{ &hf_tns_ntp_flag_handoff, {
"Can handoff connection to another", "tns.ntp_flag.handoff", FT_BOOLEAN, 16,
NULL, 0x0040, NULL, HFILL }},
{ &hf_tns_ntp_flag_sigio, {
"Generate SIGIO signal", "tns.ntp_flag.sigio", FT_BOOLEAN, 16,
NULL, 0x0020, NULL, HFILL }},
{ &hf_tns_ntp_flag_sigpipe, {
"Generate SIGPIPE signal", "tns.ntp_flag.sigpipe", FT_BOOLEAN, 16,
NULL, 0x0010, NULL, HFILL }},
{ &hf_tns_ntp_flag_sigurg, {
"Generate SIGURG signal", "tns.ntp_flag.sigurg", FT_BOOLEAN, 16,
NULL, 0x0008, NULL, HFILL }},
{ &hf_tns_ntp_flag_urgentio, {
"Urgent IO supported", "tns.ntp_flag.urgentio", FT_BOOLEAN, 16,
NULL, 0x0004, NULL, HFILL }},
{ &hf_tns_ntp_flag_fdio, {
"Full duplex IO supported", "tns.ntp_flag.dfio", FT_BOOLEAN, 16,
NULL, 0x0002, NULL, HFILL }},
{ &hf_tns_ntp_flag_testop, {
"Test operation", "tns.ntp_flag.testop", FT_BOOLEAN, 16,
NULL, 0x0001, NULL, HFILL }},
{ &hf_tns_line_turnaround, {
"Line Turnaround Value", "tns.line_turnaround", FT_UINT16, BASE_DEC,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_value_of_one, {
"Value of 1 in Hardware", "tns.value_of_one", FT_BYTES, BASE_NONE,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_connect_data_length, {
"Length of Connect Data", "tns.connect_data_length", FT_UINT16, BASE_DEC,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_connect_data_offset, {
"Offset to Connect Data", "tns.connect_data_offset", FT_UINT16, BASE_DEC,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_connect_data_max, {
"Maximum Receivable Connect Data", "tns.connect_data_max", FT_UINT32, BASE_DEC,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_connect_flags0, {
"Connect Flags 0", "tns.connect_flags0", FT_UINT8, BASE_HEX,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_connect_flags1, {
"Connect Flags 1", "tns.connect_flags1", FT_UINT8, BASE_HEX,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_conn_flag_nareq, {
"NA services required", "tns.connect_flags.nareq", FT_BOOLEAN, 8,
NULL, 0x10, NULL, HFILL }},
{ &hf_tns_conn_flag_nalink, {
"NA services linked in", "tns.connect_flags.nalink", FT_BOOLEAN, 8,
NULL, 0x08, NULL, HFILL }},
{ &hf_tns_conn_flag_enablena, {
"NA services enabled", "tns.connect_flags.enablena", FT_BOOLEAN, 8,
NULL, 0x04, NULL, HFILL }},
{ &hf_tns_conn_flag_ichg, {
"Interchange is involved", "tns.connect_flags.ichg", FT_BOOLEAN, 8,
NULL, 0x02, NULL, HFILL }},
{ &hf_tns_conn_flag_wantna, {
"NA services wanted", "tns.connect_flags.wantna", FT_BOOLEAN, 8,
NULL, 0x01, NULL, HFILL }},
{ &hf_tns_trace_cf1, {
"Trace Cross Facility Item 1", "tns.trace_cf1", FT_UINT32, BASE_HEX,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_trace_cf2, {
"Trace Cross Facility Item 2", "tns.trace_cf2", FT_UINT32, BASE_HEX,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_trace_cid, {
"Trace Unique Connection ID", "tns.trace_cid", FT_UINT64, BASE_HEX,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_connect_data, {
"Connect Data", "tns.connect_data", FT_STRING, BASE_NONE,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_accept_data_length, {
"Accept Data Length", "tns.accept_data_length", FT_UINT16, BASE_DEC,
NULL, 0x0, "Length of Accept Data", HFILL }},
{ &hf_tns_accept_data, {
"Accept Data", "tns.accept_data", FT_STRING, BASE_NONE,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_accept_data_offset, {
"Offset to Accept Data", "tns.accept_data_offset", FT_UINT16, BASE_DEC,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_refuse_reason_user, {
"Refuse Reason (User)", "tns.refuse_reason_user", FT_UINT8, BASE_HEX,
NULL, 0x0, "Refuse Reason from Application", HFILL }},
{ &hf_tns_refuse_reason_system, {
"Refuse Reason (System)", "tns.refuse_reason_system", FT_UINT8, BASE_HEX,
NULL, 0x0, "Refuse Reason from System", HFILL }},
{ &hf_tns_refuse_data_length, {
"Refuse Data Length", "tns.refuse_data_length", FT_UINT16, BASE_DEC,
NULL, 0x0, "Length of Refuse Data", HFILL }},
{ &hf_tns_refuse_data, {
"Refuse Data", "tns.refuse_data", FT_STRING, BASE_NONE,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_abort_reason_user, {
"Abort Reason (User)", "tns.abort_reason_user", FT_UINT8, BASE_HEX,
NULL, 0x0, "Abort Reason from Application", HFILL }},
{ &hf_tns_abort_reason_system, {
"Abort Reason (User)", "tns.abort_reason_system", FT_UINT8, BASE_HEX,
NULL, 0x0, "Abort Reason from System", HFILL }},
{ &hf_tns_abort_data, {
"Abort Data", "tns.abort_data", FT_STRING, BASE_NONE,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_marker_type, {
"Marker Type", "tns.marker.type", FT_UINT8, BASE_HEX,
VALS(tns_marker_types), 0x0, NULL, HFILL }},
{ &hf_tns_marker_data_byte, {
"Marker Data Byte", "tns.marker.databyte", FT_UINT8, BASE_HEX,
NULL, 0x0, NULL, HFILL }},
#if 0
{ &hf_tns_marker_data, {
"Marker Data", "tns.marker.data", FT_UINT16, BASE_HEX,
NULL, 0x0, NULL, HFILL }},
#endif
{ &hf_tns_control_cmd, {
"Control Command", "tns.control.cmd", FT_UINT16, BASE_HEX,
VALS(tns_control_cmds), 0x0, NULL, HFILL }},
{ &hf_tns_control_data, {
"Control Data", "tns.control.data", FT_BYTES, BASE_NONE,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_redirect_data_length, {
"Redirect Data Length", "tns.redirect_data_length", FT_UINT16, BASE_DEC,
NULL, 0x0, "Length of Redirect Data", HFILL }},
{ &hf_tns_redirect_data, {
"Redirect Data", "tns.redirect_data", FT_STRING, BASE_NONE,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_data_flag, {
"Data Flag", "tns.data_flag", FT_UINT16, BASE_HEX,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_data_flag_send, {
"Send Token", "tns.data_flag.send", FT_BOOLEAN, 16,
NULL, 0x1, NULL, HFILL }},
{ &hf_tns_data_flag_rc, {
"Request Confirmation", "tns.data_flag.rc", FT_BOOLEAN, 16,
NULL, 0x2, NULL, HFILL }},
{ &hf_tns_data_flag_c, {
"Confirmation", "tns.data_flag.c", FT_BOOLEAN, 16,
NULL, 0x4, NULL, HFILL }},
{ &hf_tns_data_flag_reserved, {
"Reserved", "tns.data_flag.reserved", FT_BOOLEAN, 16,
NULL, 0x8, NULL, HFILL }},
{ &hf_tns_data_flag_more, {
"More Data to Come", "tns.data_flag.more", FT_BOOLEAN, 16,
NULL, 0x20, NULL, HFILL }},
{ &hf_tns_data_flag_eof, {
"End of File", "tns.data_flag.eof", FT_BOOLEAN, 16,
NULL, 0x40, NULL, HFILL }},
{ &hf_tns_data_flag_dic, {
"Do Immediate Confirmation", "tns.data_flag.dic", FT_BOOLEAN, 16,
NULL, 0x80, NULL, HFILL }},
{ &hf_tns_data_flag_rts, {
"Request To Send", "tns.data_flag.rts", FT_BOOLEAN, 16,
NULL, 0x100, NULL, HFILL }},
{ &hf_tns_data_flag_sntt, {
"Send NT Trailer", "tns.data_flag.sntt", FT_BOOLEAN, 16,
NULL, 0x200, NULL, HFILL }},
{ &hf_tns_data_id, {
"Data ID", "tns.data_id", FT_UINT8, BASE_HEX,
VALS(tns_data_funcs), 0x0, NULL, HFILL }},
{ &hf_tns_data_length, {
"Data Length", "tns.data_length", FT_UINT16, BASE_DEC,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_data_oci_id, {
"Call ID", "tns.data_oci.id", FT_UINT8, BASE_HEX|BASE_EXT_STRING,
&tns_data_oci_subfuncs_ext, 0x00, NULL, HFILL }},
{ &hf_tns_data_piggyback_id, {
"Call ID", "tns.data_piggyback.id", FT_UINT8, BASE_HEX|BASE_EXT_STRING,
&tns_data_oci_subfuncs_ext, 0x00, NULL, HFILL }},
{ &hf_tns_data_unused, {
"Unused", "tns.data.unused", FT_BYTES, BASE_NONE,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_data_setp_acc_version, {
"Accepted Version", "tns.data_setp_req.acc_vers", FT_UINT8, BASE_DEC,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_data_setp_cli_plat, {
"Client Platform", "tns.data_setp_req.cli_plat", FT_STRINGZ, BASE_NONE,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_data_setp_version, {
"Version", "tns.data_setp_resp.version", FT_UINT8, BASE_DEC,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_data_setp_banner, {
"Server Banner", "tns.data_setp_resp.banner", FT_STRINGZ, BASE_NONE,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_data_sns_cli_vers, {
"Client Version", "tns.data_sns.cli_vers", FT_UINT32, BASE_CUSTOM,
CF_FUNC(vsnum_to_vstext_basecustom), 0x0, NULL, HFILL }},
{ &hf_tns_data_sns_srv_vers, {
"Server Version", "tns.data_sns.srv_vers", FT_UINT32, BASE_CUSTOM,
CF_FUNC(vsnum_to_vstext_basecustom), 0x0, NULL, HFILL }},
{ &hf_tns_data_sns_srvcnt, {
"Services", "tns.data_sns.srvcnt", FT_UINT16, BASE_DEC,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_data_opi_version2_banner_len, {
"Banner Length", "tns.data_opi.vers2.banner_len", FT_UINT8, BASE_DEC,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_data_opi_version2_banner, {
"Banner", "tns.data_opi.vers2.banner", FT_STRING, BASE_NONE,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_data_opi_version2_vsnum, {
"Version", "tns.data_opi.vers2.version", FT_UINT32, BASE_CUSTOM,
CF_FUNC(vsnum_to_vstext_basecustom), 0x0, NULL, HFILL }},
{ &hf_tns_data_opi_num_of_params, {
"Number of parameters", "tns.data_opi.num_of_params", FT_UINT8, BASE_DEC,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_data_opi_param_length, {
"Length", "tns.data_opi.param_length", FT_UINT8, BASE_DEC,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_data_opi_param_name, {
"Name", "tns.data_opi.param_name", FT_STRING, BASE_NONE,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_data_opi_param_value, {
"Value", "tns.data_opi.param_value", FT_STRING, BASE_NONE,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_reserved_byte, {
"Reserved Byte", "tns.reserved_byte", FT_BYTES, BASE_NONE,
NULL, 0x0, NULL, HFILL }},
{ &hf_tns_packet_type, {
"Packet Type", "tns.type", FT_UINT8, BASE_DEC,
VALS(tns_type_vals), 0x0, "Type of TNS packet", HFILL }}
};
static gint *ett[] = {
&ett_tns,
&ett_tns_connect,
&ett_tns_accept,
&ett_tns_refuse,
&ett_tns_abort,
&ett_tns_redirect,
&ett_tns_marker,
&ett_tns_attention,
&ett_tns_control,
&ett_tns_data,
&ett_tns_data_flag,
&ett_tns_acc_versions,
&ett_tns_opi_params,
&ett_tns_opi_par,
&ett_tns_sopt_flag,
&ett_tns_ntp_flag,
&ett_tns_conn_flag,
&ett_sql
};
module_t *tns_module;
proto_tns = proto_register_protocol(
"Transparent Network Substrate Protocol", "TNS", "tns");
proto_register_field_array(proto_tns, hf, array_length(hf));
proto_register_subtree_array(ett, array_length(ett));
tns_module = prefs_register_protocol(proto_tns, NULL);
prefs_register_bool_preference(tns_module, "desegment_tns_messages",
"Reassemble TNS messages spanning multiple TCP segments",
"Whether the TNS dissector should reassemble messages spanning multiple TCP segments. "
"To use this option, you must also enable \"Allow subdissectors to reassemble TCP streams\" in the TCP protocol settings.",
&tns_desegment);
}
void
proto_reg_handoff_tns(void)
{
dissector_handle_t tns_handle;
tns_handle = create_dissector_handle(dissect_tns, proto_tns);
dissector_add_uint_with_preference("tcp.port", TCP_PORT_TNS, tns_handle);
}
