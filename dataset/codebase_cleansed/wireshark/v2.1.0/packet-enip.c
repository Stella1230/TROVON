static void enip_prompt(packet_info *pinfo _U_, gchar* result)
{
g_snprintf(result, MAX_DECODE_AS_PROMPT_LEN, "Dissect unidentified I/O traffic as");
}
static gpointer enip_value(packet_info *pinfo _U_)
{
return 0;
}
static gint
enip_request_equal(gconstpointer v, gconstpointer w)
{
const enip_request_key_t *v1 = (const enip_request_key_t *)v;
const enip_request_key_t *v2 = (const enip_request_key_t *)w;
if ( v1->conversation == v2->conversation
&& v1->session_handle == v2->session_handle
&& v1->type == v2->type
&& ( ( v1->sender_context == v2->sender_context
&& v1->type == EPDT_UNCONNECTED
)
||
( v1->data.connected_transport.connid == v2->data.connected_transport.connid
&& v1->data.connected_transport.sequence == v2->data.connected_transport.sequence
&& v1->type == EPDT_CONNECTED_TRANSPORT
)
)
)
return 1;
return 0;
}
static void
enip_fmt_lir_revision( gchar *result, guint32 revision )
{
g_snprintf( result, ITEM_LABEL_LENGTH, "%d.%02d", (guint8)(( revision & 0xFF00 ) >> 8), (guint8)(revision & 0xFF) );
}
static guint
enip_request_hash (gconstpointer v)
{
const enip_request_key_t *key = (const enip_request_key_t *)v;
guint val;
val = (guint)(key->conversation * 37 + key->session_handle * 93 + key->type * 765);
if (key->type == EPDT_UNCONNECTED)
{
val += ((guint)(key->sender_context * 23));
}
else if (key->type == EPDT_CONNECTED_TRANSPORT)
{
val += ((guint)(key->data.connected_transport.connid * 87 + key->data.connected_transport.sequence * 834));
}
return val;
}
static enip_request_info_t *
enip_match_request( packet_info *pinfo, proto_tree *tree, enip_request_key_t *prequest_key )
{
enip_request_key_t *new_request_key;
enip_request_val_t *request_val;
enip_request_info_t *request_info;
request_info = NULL;
request_val = (enip_request_val_t *)g_hash_table_lookup( enip_request_hashtable, prequest_key );
if (!pinfo->fd->flags.visited)
{
if ( prequest_key && prequest_key->requesttype == ENIP_REQUEST_PACKET )
{
if ( request_val == NULL )
{
new_request_key = (enip_request_key_t *)wmem_memdup(wmem_file_scope(), prequest_key, sizeof(enip_request_key_t));
request_val = wmem_new(wmem_file_scope(), enip_request_val_t);
request_val->frames = wmem_tree_new(wmem_file_scope());
g_hash_table_insert(enip_request_hashtable, new_request_key, request_val );
}
request_info = wmem_new(wmem_file_scope(), enip_request_info_t);
request_info->req_num = pinfo->num;
request_info->rep_num = 0;
request_info->req_time = pinfo->abs_ts;
request_info->cip_info = NULL;
wmem_tree_insert32(request_val->frames, pinfo->num, (void *)request_info);
}
if ( request_val && prequest_key && prequest_key->requesttype == ENIP_RESPONSE_PACKET )
{
request_info = (enip_request_info_t*)wmem_tree_lookup32_le( request_val->frames, pinfo->num );
if ( request_info )
{
request_info->rep_num = pinfo->num;
}
}
}
else
{
if ( request_val )
request_info = (enip_request_info_t *)wmem_tree_lookup32_le( request_val->frames, pinfo->num );
}
if ( tree && request_info )
{
if ( prequest_key && prequest_key->requesttype == ENIP_REQUEST_PACKET )
{
if (request_info->rep_num)
{
proto_item *it;
it = proto_tree_add_uint(tree, hf_enip_response_in,
NULL, 0, 0, request_info->rep_num);
PROTO_ITEM_SET_GENERATED(it);
}
}
else
{
if ( prequest_key && prequest_key->requesttype == ENIP_RESPONSE_PACKET )
{
if (request_info->req_num)
{
proto_item *it;
nstime_t ns;
it = proto_tree_add_uint(tree, hf_enip_response_to,
NULL, 0, 0, request_info->req_num);
PROTO_ITEM_SET_GENERATED(it);
nstime_delta(&ns, &pinfo->abs_ts, &request_info->req_time);
it = proto_tree_add_time(tree, hf_enip_time, NULL, 0, 0, &ns);
PROTO_ITEM_SET_GENERATED(it);
}
}
}
}
return request_info;
}
static gboolean
enip_io_conv_valid(packet_info *pinfo)
{
enip_conn_val_t* conn = (enip_conn_val_t*)p_get_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_CONNECTION_INFO);
if (conn == NULL)
return FALSE;
return (((conn->TransportClass_trigger & CI_TRANSPORT_CLASS_MASK) == 0) ||
((conn->TransportClass_trigger & CI_TRANSPORT_CLASS_MASK) == 1));
}
static gchar *
enip_io_conv_filter(packet_info *pinfo)
{
char *buf;
enip_conn_val_t* conn = (enip_conn_val_t*)p_get_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_CONNECTION_INFO);
if (conn == NULL)
return NULL;
if (conn->close_frame > 0)
{
buf = g_strdup_printf(
"((frame.number == %u) || ((frame.number >= %u) && (frame.number <= %u))) && "
"((enip.cpf.sai.connid == 0x%08x || enip.cpf.sai.connid == 0x%08x) || "
"((cip.cm.conn_serial_num == 0x%04x) && (cip.cm.vendor == 0x%04x) && (cip.cm.orig_serial_num == 0x%08x)))",
conn->open_frame, conn->open_reply_frame, conn->close_frame,
conn->O2TConnID, conn->T2OConnID,
conn->ConnSerialNumber, conn->VendorID, conn->DeviceSerialNumber);
}
else
{
buf = g_strdup_printf(
"((frame.number == %u) || (frame.number >= %u)) && "
"((enip.cpf.sai.connid == 0x%08x || enip.cpf.sai.connid == 0x%08x) || "
"((cip.cm.conn_serial_num == 0x%04x) && (cip.cm.vendor == 0x%04x) && (cip.cm.orig_serial_num == 0x%08x)))",
conn->open_frame, conn->open_reply_frame,
conn->O2TConnID, conn->T2OConnID,
conn->ConnSerialNumber, conn->VendorID, conn->DeviceSerialNumber);
}
return buf;
}
static gboolean
enip_exp_conv_valid(packet_info *pinfo)
{
enip_conn_val_t* conn = (enip_conn_val_t*)p_get_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_CONNECTION_INFO);
if (conn == NULL)
return FALSE;
return (((conn->TransportClass_trigger & CI_TRANSPORT_CLASS_MASK) == 2) ||
((conn->TransportClass_trigger & CI_TRANSPORT_CLASS_MASK) == 3));
}
static gchar *
enip_exp_conv_filter(packet_info *pinfo)
{
char *buf;
enip_conn_val_t* conn = (enip_conn_val_t*)p_get_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_CONNECTION_INFO);
if (conn == NULL)
return NULL;
if (conn->close_frame > 0)
{
buf = g_strdup_printf(
"((frame.number == %u) || ((frame.number >= %u) && (frame.number <= %u))) && "
"((enip.cpf.cai.connid == 0x%08x || enip.cpf.cai.connid == 0x%08x) || "
"((cip.cm.conn_serial_num == 0x%04x) && (cip.cm.vendor == 0x%04x) && (cip.cm.orig_serial_num == 0x%08x)))",
conn->open_frame, conn->open_reply_frame, conn->close_frame,
conn->O2TConnID, conn->T2OConnID,
conn->ConnSerialNumber, conn->VendorID, conn->DeviceSerialNumber);
}
else
{
buf = g_strdup_printf(
"((frame.number == %u) || (frame.number >= %u)) && "
"((enip.cpf.cai.connid == 0x%08x || enip.cpf.cai.connid == 0x%08x) || "
"((cip.cm.conn_serial_num == 0x%04x) && (cip.cm.vendor == 0x%04x) && (cip.cm.orig_serial_num == 0x%08x)))",
conn->open_frame, conn->open_reply_frame,
conn->O2TConnID, conn->T2OConnID,
conn->ConnSerialNumber, conn->VendorID, conn->DeviceSerialNumber);
}
return buf;
}
static gint
enip_conn_equal(gconstpointer v, gconstpointer w)
{
const enip_conn_key_t *v1 = (const enip_conn_key_t *)v;
const enip_conn_key_t *v2 = (const enip_conn_key_t *)w;
if ((v1->ConnSerialNumber == v2->ConnSerialNumber) &&
(v1->VendorID == v2->VendorID) &&
(v1->DeviceSerialNumber == v2->DeviceSerialNumber) &&
((v1->O2TConnID == 0) || (v2->O2TConnID == 0) || (v1->O2TConnID == v2->O2TConnID)) &&
((v1->T2OConnID == 0) || (v2->T2OConnID == 0) || (v1->T2OConnID == v2->T2OConnID)))
return 1;
return 0;
}
static guint
enip_conn_hash (gconstpointer v)
{
const enip_conn_key_t *key = (const enip_conn_key_t *)v;
guint val;
val = (guint)( key->ConnSerialNumber + key->VendorID + key->DeviceSerialNumber );
return val;
}
static void
enip_open_cip_connection( packet_info *pinfo, cip_conn_info_t* connInfo)
{
enip_conn_key_t *conn_key;
enip_conn_val_t *conn_val;
conversation_t *conversation, *conversationTO;
enip_conv_info_t *enip_info;
address dest_address;
struct e_in6_addr ipv6_zero;
if (pinfo->fd->flags.visited)
return;
conn_key = wmem_new(wmem_file_scope(), enip_conn_key_t);
conn_key->ConnSerialNumber = connInfo->ConnSerialNumber;
conn_key->VendorID = connInfo->VendorID;
conn_key->DeviceSerialNumber = connInfo->DeviceSerialNumber;
conn_key->O2TConnID = connInfo->O2T.connID;
conn_key->T2OConnID = connInfo->T2O.connID;
conn_val = (enip_conn_val_t *)g_hash_table_lookup( enip_conn_hashtable, conn_key );
if ( conn_val == NULL )
{
conn_val = wmem_new(wmem_file_scope(), enip_conn_val_t);
conn_val->ConnSerialNumber = connInfo->ConnSerialNumber;
conn_val->VendorID = connInfo->VendorID;
conn_val->DeviceSerialNumber = connInfo->DeviceSerialNumber;
conn_val->O2TConnID = connInfo->O2T.connID;
conn_val->T2OConnID = connInfo->T2O.connID;
conn_val->TransportClass_trigger = connInfo->TransportClass_trigger;
conn_val->safety = connInfo->safety;
conn_val->motion = connInfo->motion;
conn_val->ClassID = connInfo->ClassID;
conn_val->open_frame = connInfo->forward_open_frame;
conn_val->open_reply_frame = pinfo->num;
conn_val->close_frame = 0;
conn_val->connid = enip_unique_connid++;
g_hash_table_insert(enip_conn_hashtable, conn_key, conn_val );
if (((connInfo->TransportClass_trigger & CI_TRANSPORT_CLASS_MASK) == 0) ||
((connInfo->TransportClass_trigger & CI_TRANSPORT_CLASS_MASK) == 1))
{
memset(&ipv6_zero, 0, sizeof(ipv6_zero));
if ((connInfo->O2T.port == 0) || (connInfo->O2T.type == CONN_TYPE_MULTICAST))
connInfo->O2T.port = ENIP_IO_PORT;
if ((connInfo->O2T.ipaddress.type == AT_NONE) ||
((connInfo->O2T.ipaddress.type == AT_IPv4) && ((*(const guint32*)connInfo->O2T.ipaddress.data)) == 0) ||
((connInfo->O2T.ipaddress.type == AT_IPv6) && (memcmp(connInfo->O2T.ipaddress.data, &ipv6_zero, sizeof(ipv6_zero)) == 0)) ||
(connInfo->O2T.type != CONN_TYPE_MULTICAST))
copy_address_shallow(&connInfo->O2T.ipaddress, &pinfo->src);
if ((connInfo->T2O.port == 0) || (connInfo->T2O.type == CONN_TYPE_MULTICAST))
connInfo->T2O.port = ENIP_IO_PORT;
if ((connInfo->T2O.ipaddress.type == AT_NONE) ||
((connInfo->T2O.ipaddress.type == AT_IPv4) && ((*(const guint32*)connInfo->T2O.ipaddress.data)) == 0) ||
((connInfo->T2O.ipaddress.type == AT_IPv6) && (memcmp(connInfo->T2O.ipaddress.data, &ipv6_zero, sizeof(ipv6_zero)) == 0)) ||
(connInfo->T2O.type != CONN_TYPE_MULTICAST))
copy_address_shallow(&connInfo->T2O.ipaddress, &pinfo->dst);
if (connInfo->O2T.ipaddress.type == AT_IPv6)
{
dest_address.type = AT_IPv6;
dest_address.len = 16;
}
else
{
dest_address.type = AT_IPv4;
dest_address.len = 4;
}
dest_address.data = connInfo->O2T.ipaddress.data;
if ((conversation = find_conversation(pinfo->num, &pinfo->dst, &dest_address,
PT_UDP, connInfo->O2T.port, 0, NO_PORT_B)) == NULL) {
conversation = conversation_new(pinfo->num, &pinfo->dst, &dest_address,
PT_UDP, connInfo->O2T.port, 0, NO_PORT2);
}
enip_info = (enip_conv_info_t *)conversation_get_proto_data(conversation, proto_enip);
if (enip_info == NULL)
{
enip_info = wmem_new(wmem_file_scope(), enip_conv_info_t);
enip_info->O2TConnIDs = wmem_tree_new(wmem_file_scope());
enip_info->T2OConnIDs = wmem_tree_new(wmem_file_scope());
conversation_add_proto_data(conversation, proto_enip, enip_info);
}
wmem_tree_insert32(enip_info->O2TConnIDs, connInfo->O2T.connID, (void *)conn_val);
dest_address.data = connInfo->T2O.ipaddress.data;
if ((conversationTO = find_conversation(pinfo->num, &pinfo->src, &dest_address,
PT_UDP, connInfo->T2O.port, 0, NO_PORT_B)) == NULL) {
conversationTO = conversation_new(pinfo->num, &pinfo->src,
&dest_address, PT_UDP,
connInfo->T2O.port, 0, NO_PORT2);
}
enip_info = (enip_conv_info_t *)conversation_get_proto_data(conversationTO, proto_enip);
if (enip_info == NULL)
{
enip_info = wmem_new(wmem_file_scope(), enip_conv_info_t);
enip_info->O2TConnIDs = wmem_tree_new(wmem_file_scope());
enip_info->T2OConnIDs = wmem_tree_new(wmem_file_scope());
conversation_add_proto_data(conversationTO, proto_enip, enip_info);
}
wmem_tree_insert32(enip_info->T2OConnIDs, connInfo->T2O.connID, (void *)conn_val);
}
else
{
conversation = find_or_create_conversation(pinfo);
enip_info = (enip_conv_info_t *)conversation_get_proto_data(conversation, proto_enip);
if (!enip_info)
{
enip_info = wmem_new(wmem_file_scope(), enip_conv_info_t);
enip_info->O2TConnIDs = wmem_tree_new(wmem_file_scope());
enip_info->T2OConnIDs = wmem_tree_new(wmem_file_scope());
conversation_add_proto_data(conversation, proto_enip, enip_info);
}
wmem_tree_insert32(enip_info->O2TConnIDs, connInfo->O2T.connID, (void *)conn_val);
wmem_tree_insert32(enip_info->T2OConnIDs, connInfo->T2O.connID, (void *)conn_val);
}
}
if (!pinfo->fd->flags.visited)
p_add_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_CONNECTION_INFO, conn_val);
}
void
enip_close_cip_connection(packet_info *pinfo, guint16 ConnSerialNumber,
guint16 VendorID, guint32 DeviceSerialNumber )
{
enip_conn_key_t conn_key;
enip_conn_val_t *conn_val;
if (pinfo->fd->flags.visited)
return;
conn_key.ConnSerialNumber = ConnSerialNumber;
conn_key.VendorID = VendorID;
conn_key.DeviceSerialNumber = DeviceSerialNumber;
conn_key.O2TConnID = 0;
conn_key.T2OConnID = 0;
conn_val = (enip_conn_val_t *)g_hash_table_lookup( enip_conn_hashtable, &conn_key );
if ( conn_val )
{
conn_val->close_frame = pinfo->num;
if (!pinfo->fd->flags.visited)
p_add_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_CONNECTION_INFO, conn_val);
}
}
void enip_mark_connection_triad( packet_info *pinfo, guint16 ConnSerialNumber, guint16 VendorID, guint32 DeviceSerialNumber )
{
enip_conn_key_t conn_key;
enip_conn_val_t *conn_val;
conn_key.ConnSerialNumber = ConnSerialNumber;
conn_key.VendorID = VendorID;
conn_key.DeviceSerialNumber = DeviceSerialNumber;
conn_key.O2TConnID = 0;
conn_key.T2OConnID = 0;
conn_val = (enip_conn_val_t *)g_hash_table_lookup( enip_conn_hashtable, &conn_key );
if ( conn_val )
{
p_add_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_CONNECTION_INFO, conn_val);
}
}
static enip_conn_val_t *
enip_get_explicit_connid(packet_info *pinfo, enip_request_key_t *prequest_key, guint32 connid)
{
conversation_t *conversation;
enip_conv_info_t *enip_info;
enip_conn_val_t *conn_val;
enum enip_packet_type requesttype = ENIP_REQUEST_PACKET;
if (prequest_key != NULL)
{
if ((prequest_key->requesttype != ENIP_REQUEST_PACKET) && (prequest_key->requesttype != ENIP_RESPONSE_PACKET ))
return NULL;
requesttype = prequest_key->requesttype;
}
conversation = find_conversation(pinfo->num,
&pinfo->src, &pinfo->dst,
pinfo->ptype,
pinfo->srcport, pinfo->destport, 0);
if (conversation == NULL)
return NULL;
enip_info = (enip_conv_info_t *)conversation_get_proto_data(conversation, proto_enip);
if (!enip_info)
return NULL;
conn_val = NULL;
switch (requesttype )
{
case ENIP_REQUEST_PACKET:
conn_val = (enip_conn_val_t *)wmem_tree_lookup32( enip_info->O2TConnIDs, connid );
if ( conn_val == NULL )
conn_val = (enip_conn_val_t *)wmem_tree_lookup32( enip_info->T2OConnIDs, connid );
break;
case ENIP_RESPONSE_PACKET:
conn_val = (enip_conn_val_t *)wmem_tree_lookup32( enip_info->T2OConnIDs, connid );
if ( conn_val == NULL )
conn_val = (enip_conn_val_t *)wmem_tree_lookup32( enip_info->O2TConnIDs, connid );
break;
case ENIP_CANNOT_CLASSIFY:
break;
}
if ((conn_val == NULL ) || (conn_val->open_reply_frame > pinfo->num))
return NULL;
return conn_val;
}
static enip_conn_val_t *
enip_get_io_connid(packet_info *pinfo, guint32 connid, enum enip_connid_type* pconnid_type)
{
conversation_t *conversation;
enip_conv_info_t *enip_info;
enip_conn_val_t *conn_val = NULL;
*pconnid_type = ECIDT_UNKNOWN;
conversation = find_conversation(pinfo->num,
&pinfo->src, &pinfo->dst,
pinfo->ptype,
pinfo->destport, 0, NO_PORT_B);
if (conversation == NULL)
return NULL;
if ((enip_info = (enip_conv_info_t *)conversation_get_proto_data(conversation, proto_enip)) == NULL)
return NULL;
if (enip_info->O2TConnIDs != NULL)
conn_val = (enip_conn_val_t *)wmem_tree_lookup32( enip_info->O2TConnIDs, connid );
if ( conn_val == NULL )
{
if (enip_info->T2OConnIDs != NULL)
{
if ((conn_val = (enip_conn_val_t *)wmem_tree_lookup32( enip_info->T2OConnIDs, connid)) != NULL)
*pconnid_type = ECIDT_T2O;
}
}
else
{
*pconnid_type = ECIDT_O2T;
}
if ((conn_val == NULL) || ( conn_val->open_reply_frame > pinfo->num ))
return NULL;
return conn_val;
}
static int
dissect_tcpip_status(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
static const int * status[] = {
&hf_tcpip_status_interface_config,
&hf_tcpip_status_mcast_pending,
&hf_tcpip_status_interface_config_pending,
&hf_tcpip_status_acd,
&hf_tcpip_status_reserved,
NULL
};
if (total_len < 4)
{
expert_add_info(pinfo, item, &ei_mal_tcpip_status);
return total_len;
}
proto_tree_add_bitmask(tree, tvb, offset, hf_tcpip_status, ett_tcpip_status, status, ENC_LITTLE_ENDIAN);
return 4;
}
static int
dissect_tcpip_config_cap(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
static const int * capabilities[] = {
&hf_tcpip_config_cap_bootp,
&hf_tcpip_config_cap_dns,
&hf_tcpip_config_cap_dhcp,
&hf_tcpip_config_cap_dhcp_dns_update,
&hf_tcpip_config_cap_config_settable,
&hf_tcpip_config_cap_hardware_config,
&hf_tcpip_config_cap_interface_reset,
&hf_tcpip_config_cap_acd,
&hf_tcpip_config_cap_reserved,
NULL
};
if (total_len < 4)
{
expert_add_info(pinfo, item, &ei_mal_tcpip_config_cap);
return total_len;
}
proto_tree_add_bitmask(tree, tvb, offset, hf_tcpip_config_cap, ett_tcpip_config_cap, capabilities, ENC_LITTLE_ENDIAN);
return 4;
}
static int
dissect_tcpip_config_control(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
static const int * control_bits[] = {
&hf_tcpip_config_control_config,
&hf_tcpip_config_control_dns,
&hf_tcpip_config_control_reserved,
NULL
};
if (total_len < 4)
{
expert_add_info(pinfo, item, &ei_mal_tcpip_config_control);
return total_len;
}
proto_tree_add_bitmask(tree, tvb, offset, hf_tcpip_config_control, ett_tcpip_config_control, control_bits, ENC_LITTLE_ENDIAN);
return 4;
}
static int
dissect_tcpip_physical_link(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
return dissect_padded_epath_len_uint(pinfo, tree, item, tvb, offset, total_len);
}
static int
dissect_tcpip_interface_config(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
guint16 domain_length;
if (total_len < 22)
{
expert_add_info(pinfo, item, &ei_mal_tcpip_interface_config);
return total_len;
}
proto_tree_add_item(tree, hf_tcpip_ic_ip_addr, tvb, offset, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_tcpip_ic_subnet_mask, tvb, offset+4, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_tcpip_ic_gateway, tvb, offset+8, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_tcpip_ic_name_server, tvb, offset+12, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_tcpip_ic_name_server2, tvb, offset+16, 4, ENC_LITTLE_ENDIAN);
domain_length = tvb_get_letohs( tvb, offset+20);
proto_tree_add_item(tree, hf_tcpip_ic_domain_name, tvb, offset+22, domain_length, ENC_ASCII|ENC_NA);
domain_length += domain_length % 2;
return (22+domain_length);
}
static int dissect_tcpip_ssn(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
if (total_len < 6)
{
expert_add_info(pinfo, item, &ei_mal_tcpip_ssn);
return total_len;
}
dissect_cipsafety_ssn(tree, tvb, pinfo, offset, hf_tcpip_snn_timestamp, hf_tcpip_snn_date, hf_tcpip_snn_time);
return 6;
}
static int
dissect_tcpip_mcast_config(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
if (total_len < 8)
{
expert_add_info(pinfo, item, &ei_mal_tcpip_mcast_config);
return total_len;
}
proto_tree_add_item(tree, hf_tcpip_mcast_alloc, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_tcpip_mcast_reserved, tvb, offset+1, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_tcpip_mcast_num_mcast, tvb, offset+2, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_tcpip_mcast_addr_start, tvb, offset+4, 4, ENC_LITTLE_ENDIAN);
return 8;
}
static int
dissect_tcpip_last_conflict(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
tvbuff_t *next_tvb;
gboolean save_info;
if (total_len < 35)
{
expert_add_info(pinfo, item, &ei_mal_tcpip_last_conflict);
return total_len;
}
proto_tree_add_item(tree, hf_tcpip_lcd_acd_activity, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_tcpip_lcd_remote_mac, tvb, offset+1, 6, ENC_NA);
if ( tvb_get_guint8(tvb, offset) == 0 )
proto_tree_add_item(tree, hf_tcpip_lcd_arp_pdu, tvb, offset+7, 28, ENC_NA);
else
{
save_info = col_get_writable(pinfo->cinfo);
col_set_writable(pinfo->cinfo, FALSE);
next_tvb = tvb_new_subset_length(tvb, offset+7, 28);
call_dissector(arp_handle, next_tvb, pinfo, tree);
col_set_writable(pinfo->cinfo, save_info);
}
return 35;
}
static int
dissect_elink_interface_flags(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
static const int * flags[] = {
&hf_elink_iflags_link_status,
&hf_elink_iflags_duplex,
&hf_elink_iflags_neg_status,
&hf_elink_iflags_manual_reset,
&hf_elink_iflags_local_hw_fault,
&hf_elink_iflags_reserved,
NULL
};
if (total_len < 4)
{
expert_add_info(pinfo, item, &ei_mal_elink_interface_flags);
return total_len;
}
proto_tree_add_bitmask(tree, tvb, offset, hf_elink_interface_flags, ett_elink_interface_flags, flags, ENC_LITTLE_ENDIAN);
return 4;
}
static int
dissect_elink_physical_address(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
if (total_len < 6)
{
expert_add_info(pinfo, item, &ei_mal_elink_physical_address);
return total_len;
}
proto_tree_add_item(tree, hf_elink_physical_address, tvb, offset, 6, ENC_NA);
return 6;
}
static int
dissect_elink_interface_counters(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
if (total_len < 44)
{
expert_add_info(pinfo, item, &ei_mal_elink_interface_counters);
return total_len;
}
proto_tree_add_item(tree, hf_elink_icount_in_octets, tvb, offset, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_icount_in_ucast, tvb, offset+4, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_icount_in_nucast, tvb, offset+8, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_icount_in_discards, tvb, offset+12, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_icount_in_errors, tvb, offset+16, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_icount_in_unknown_protos, tvb, offset+20, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_icount_out_octets, tvb, offset+24, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_icount_out_ucast, tvb, offset+28, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_icount_out_nucast, tvb, offset+32, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_icount_out_discards, tvb, offset+36, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_icount_out_errors, tvb, offset+40, 4, ENC_LITTLE_ENDIAN);
return 44;
}
static int
dissect_elink_media_counters(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
if (total_len < 48)
{
expert_add_info(pinfo, item, &ei_mal_elink_media_counters);
return total_len;
}
proto_tree_add_item(tree, hf_elink_mcount_alignment_errors, tvb, offset, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_mcount_fcs_errors, tvb, offset+4, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_mcount_single_collisions, tvb, offset+8, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_mcount_multiple_collisions, tvb, offset+12, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_mcount_sqe_test_errors, tvb, offset+16, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_mcount_deferred_transmission, tvb, offset+20, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_mcount_late_collisions, tvb, offset+24, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_mcount_excessive_collisions, tvb, offset+28, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_mcount_mac_transmit_errors, tvb, offset+32, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_mcount_carrier_sense_errors, tvb, offset+36, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_mcount_frame_too_long, tvb, offset+40, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_mcount_mac_receive_errors, tvb, offset+44, 4, ENC_LITTLE_ENDIAN);
return 48;
}
static int
dissect_elink_interface_control(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
static const int * control_bits[] = {
&hf_elink_icontrol_control_bits_auto_neg,
&hf_elink_icontrol_control_bits_forced_duplex,
&hf_elink_icontrol_control_bits_reserved,
NULL
};
if (total_len < 4)
{
expert_add_info(pinfo, item, &ei_mal_elink_interface_control);
return total_len;
}
proto_tree_add_bitmask(tree, tvb, offset, hf_elink_icontrol_control_bits, ett_elink_icontrol_bits, control_bits, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_elink_icontrol_forced_speed, tvb, offset+2, 2, ENC_LITTLE_ENDIAN);
return 4;
}
static int
dissect_dlr_ring_supervisor_config(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
if (total_len < 12)
{
expert_add_info(pinfo, item, &ei_mal_dlr_ring_supervisor_config);
return total_len;
}
proto_tree_add_item(tree, hf_dlr_rsc_ring_supervisor_enable, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_dlr_rsc_ring_supervisor_precedence, tvb, offset+1, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_dlr_rsc_beacon_interval, tvb, offset+2, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_dlr_rsc_beacon_timeout, tvb, offset+6, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_dlr_rsc_dlr_vlan_id, tvb, offset+10, 2, ENC_LITTLE_ENDIAN);
return 12;
}
static int
dissect_dlr_last_active_node_on_port_1(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
if (total_len < 10)
{
expert_add_info(pinfo, item, &ei_mal_dlr_last_active_node_on_port_1);
return total_len;
}
proto_tree_add_item(tree, hf_dlr_lanp1_dev_ip_addr, tvb, offset, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_dlr_lanp1_dev_physical_address, tvb, offset+4, 6, ENC_NA);
return 10;
}
static int
dissect_dlr_last_active_node_on_port_2(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
if (total_len < 10)
{
expert_add_info(pinfo, item, &ei_mal_dlr_last_active_node_on_port_2);
return total_len;
}
proto_tree_add_item(tree, hf_dlr_lanp2_dev_ip_addr, tvb, offset, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_dlr_lanp2_dev_physical_address, tvb, offset+4, 6, ENC_NA);
return 10;
}
static int
dissect_dlr_ring_protocol_participants_list(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
int pos;
if (total_len % 10)
{
expert_add_info(pinfo, item, &ei_mal_dlr_ring_protocol_participants_list);
return total_len;
}
pos = 0;
while ( pos < total_len)
{
proto_tree_add_item(tree, hf_dlr_rppl_dev_ip_addr, tvb, offset+pos, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_dlr_rppl_dev_physical_address, tvb, offset+pos+4, 6, ENC_NA);
pos+=10;
}
return total_len;
}
static int
dissect_dlr_active_supervisor_address(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
if (total_len < 10)
{
expert_add_info(pinfo, item, &ei_mal_dlr_active_supervisor_address);
return total_len;
}
proto_tree_add_item(tree, hf_dlr_asa_supervisor_ip_addr, tvb, offset, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_dlr_asa_supervisor_physical_address, tvb, offset+4, 6, ENC_NA);
return 10;
}
static int
dissect_dlr_capability_flags(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
static const int * capabilities[] = {
&hf_dlr_capflags_announce_base_node,
&hf_dlr_capflags_beacon_base_node,
&hf_dlr_capflags_reserved1,
&hf_dlr_capflags_supervisor_capable,
&hf_dlr_capflags_redundant_gateway_capable,
&hf_dlr_capflags_flush_frame_capable,
&hf_dlr_capflags_reserved2,
NULL
};
if (total_len < 4)
{
expert_add_info(pinfo, item, &ei_mal_dlr_capability_flags);
return total_len;
}
proto_tree_add_bitmask(tree, tvb, offset, hf_dlr_capability_flags, ett_dlr_capability_flags, capabilities, ENC_LITTLE_ENDIAN);
return 4;
}
static int
dissect_dlr_redundant_gateway_config(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
if (total_len < 11)
{
expert_add_info(pinfo, item, &ei_mal_dlr_redundant_gateway_config);
return total_len;
}
proto_tree_add_item(tree, hf_dlr_rgc_red_gateway_enable, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_dlr_rgc_gateway_precedence, tvb, offset+1, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_dlr_rgc_advertise_interval, tvb, offset+2, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_dlr_rgc_advertise_timeout, tvb, offset+6, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_dlr_rgc_learning_update_enable, tvb, offset+10, 1, ENC_LITTLE_ENDIAN);
return 11;
}
static int
dissect_dlr_active_gateway_address(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
if (total_len < 10)
{
expert_add_info(pinfo, item, &ei_mal_dlr_active_gateway_address);
return total_len;
}
proto_tree_add_item(tree, hf_dlr_aga_ip_addr, tvb, offset, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_dlr_aga_physical_address, tvb, offset+4, 6, ENC_NA);
return 10;
}
static int
dissect_eip_security_cap(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
static const int * capabilities[] = {
&hf_eip_security_capflags_secure_renegotiation,
&hf_eip_security_capflags_reserved,
NULL
};
if (total_len < 4)
{
expert_add_info(pinfo, item, &ei_mal_eip_security_capability_flags);
return total_len;
}
proto_tree_add_bitmask(tree, tvb, offset, hf_eip_security_capability_flags, ett_eip_security_capability_flags, capabilities, ENC_LITTLE_ENDIAN);
return 4;
}
static int
dissect_eip_security_avail_cipher_suites(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
guint32 i, num_suites;
if (total_len < 1)
{
expert_add_info(pinfo, item, &ei_mal_eip_security_avail_cipher_suites);
return total_len;
}
proto_tree_add_item_ret_uint(tree, hf_eip_security_num_avail_cipher_suites, tvb, offset, 1, ENC_NA, &num_suites);
for (i = 0; i < num_suites; i++)
{
proto_tree_add_item(tree, hf_eip_security_avail_cipher_suite, tvb, offset, 2, ENC_BIG_ENDIAN);
offset += 2;
}
return ((num_suites*2)+1);
}
static int
dissect_eip_security_allow_cipher_suites(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
guint32 i, num_suites;
if (total_len < 1)
{
expert_add_info(pinfo, item, &ei_mal_eip_security_allow_cipher_suites);
return total_len;
}
proto_tree_add_item_ret_uint(tree, hf_eip_security_num_allow_cipher_suites, tvb, offset, 1, ENC_NA, &num_suites);
for (i = 0; i < num_suites; i++)
{
proto_tree_add_item(tree, hf_eip_security_allow_cipher_suite, tvb, offset, 2, ENC_BIG_ENDIAN);
offset += 2;
}
return ((num_suites*2)+1);
}
static int
dissect_eip_security_preshared_keys(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
guint32 i, num, id_size, psk_size;
proto_item* ti;
proto_tree* psk_tree;
int start_offset = offset;
if (total_len < 3)
{
expert_add_info(pinfo, item, &ei_mal_eip_security_preshared_keys);
return total_len;
}
ti = proto_tree_add_item_ret_uint(tree, hf_eip_security_num_psk, tvb, offset, 1, ENC_NA, &num);
psk_tree = proto_item_add_subtree(ti, ett_eip_security_psk);
offset++;
for (i = 0; i < num; i++)
{
proto_tree_add_item_ret_uint(psk_tree, hf_eip_security_psk_identity_size, tvb, offset, 1, ENC_NA, &id_size);
if (total_len < (int)(id_size+2))
{
expert_add_info(pinfo, item, &ei_mal_eip_security_preshared_keys);
return total_len;
}
offset++;
proto_tree_add_item(psk_tree, hf_eip_security_psk_identity, tvb, offset, id_size, ENC_NA);
offset += id_size;
proto_tree_add_item_ret_uint(psk_tree, hf_eip_security_psk_size, tvb, offset, 1, ENC_NA, &psk_size);
offset++;
if (total_len < (int)(id_size+psk_size+2))
{
expert_add_info(pinfo, item, &ei_mal_eip_security_preshared_keys);
return total_len;
}
proto_tree_add_item(psk_tree, hf_eip_security_psk, tvb, offset, id_size, ENC_NA);
offset += psk_size;
}
proto_item_set_len(ti, offset-start_offset);
return offset-start_offset;
}
static int
dissect_eip_security_active_certs(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
guint32 i, num, path_size;
proto_item *ti;
proto_tree* cert_tree;
int start_offset = offset;
if (total_len < 2)
{
expert_add_info(pinfo, item, &ei_mal_eip_security_active_certs);
return total_len;
}
ti = proto_tree_add_item_ret_uint(tree, hf_eip_security_num_active_certs, tvb, offset, 1, ENC_NA, &num);
cert_tree = proto_item_add_subtree(ti, ett_eip_security_active_certs);
offset++;
for (i = 0; i < num; i++)
{
path_size = dissect_padded_epath_len_usint(pinfo, cert_tree, ti, tvb, offset, total_len);
offset += path_size;
}
proto_item_set_len(ti, offset-start_offset);
return offset-start_offset;
}
static int
dissect_eip_security_trusted_auths(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
guint32 i, num, path_size;
proto_item *ti;
proto_tree* cert_tree;
int start_offset = offset;
if (total_len < 2)
{
expert_add_info(pinfo, item, &ei_mal_eip_security_trusted_auths);
return total_len;
}
ti = proto_tree_add_item_ret_uint(tree, hf_eip_security_num_trusted_auths, tvb, offset, 1, ENC_NA, &num);
cert_tree = proto_item_add_subtree(ti, ett_eip_security_trusted_auths);
offset++;
for (i = 0; i < num; i++)
{
path_size = dissect_padded_epath_len_usint(pinfo, cert_tree, ti, tvb, offset, total_len);
offset += path_size;
}
proto_item_set_len(ti, offset-start_offset);
return offset-start_offset;
}
static int
dissect_eip_security_cert_revocation_list(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
return dissect_padded_epath_len_usint(pinfo, tree, item, tvb, offset, total_len);
}
static int
dissect_eip_cert_cap_flags(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
static const int * capabilities[] = {
&hf_eip_cert_capflags_push,
&hf_eip_cert_capflags_reserved,
NULL
};
if (total_len < 4)
{
expert_add_info(pinfo, item, &ei_mal_eip_cert_capability_flags);
return total_len;
}
proto_tree_add_bitmask(tree, tvb, offset, hf_eip_cert_capability_flags, ett_eip_cert_capability_flags, capabilities, ENC_LITTLE_ENDIAN);
return 4;
}
static int
dissect_eip_cert_cert_list(packet_info *pinfo, proto_tree *tree, proto_item *item _U_, tvbuff_t *tvb,
int offset, int total_len)
{
guint32 i, num, path_size;
proto_item *ti;
proto_tree* cert_tree;
int start_offset = offset;
ti = proto_tree_add_item_ret_uint(tree, hf_eip_cert_num_certs, tvb, offset, 1, ENC_NA, &num);
cert_tree = proto_item_add_subtree(ti, ett_eip_cert_num_certs);
offset++;
for (i = 0; i < num; i++)
{
path_size = tvb_get_guint8( tvb, offset );
proto_tree_add_item(tree, hf_eip_cert_cert_name, tvb, offset+1, path_size, ENC_ASCII|ENC_NA);
offset += (1+path_size);
path_size = dissect_padded_epath_len_usint(pinfo, cert_tree, ti, tvb, offset, total_len);
offset += path_size;
}
proto_item_set_len(ti, offset-start_offset);
return offset-start_offset;
}
static int
dissect_eip_cert_device_cert(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
guint32 path_size;
proto_tree_add_item(tree, hf_eip_cert_device_cert_status, tvb, offset, 1, ENC_NA);
offset++;
path_size = dissect_padded_epath_len_usint(pinfo, tree, item, tvb, offset, total_len);
return path_size + 1;
}
static int
dissect_eip_cert_ca_cert(packet_info *pinfo, proto_tree *tree, proto_item *item, tvbuff_t *tvb,
int offset, int total_len)
{
guint32 path_size;
proto_tree_add_item(tree, hf_eip_cert_ca_cert_status, tvb, offset, 1, ENC_NA);
offset++;
path_size = dissect_padded_epath_len_usint(pinfo, tree, item, tvb, offset, total_len);
return path_size + 1;
}
static void
enip_init_protocol(void)
{
enip_request_hashtable = g_hash_table_new(enip_request_hash, enip_request_equal);
enip_conn_hashtable = g_hash_table_new(enip_conn_hash, enip_conn_equal);
}
static void
enip_cleanup_protocol(void)
{
g_hash_table_destroy(enip_request_hashtable);
g_hash_table_destroy(enip_conn_hashtable);
}
static void
dissect_cpf(enip_request_key_t *request_key, int command, tvbuff_t *tvb,
packet_info *pinfo, proto_tree *tree, proto_tree *dissector_tree, int offset, guint32 ifacehndl)
{
proto_item *temp_item, *count_item, *type_item, *io_item;
proto_tree *temp_tree, *count_tree, *item_tree, *sockaddr_tree, *io_tree;
int item_count, item_length, item, io_length;
unsigned char name_length;
tvbuff_t *next_tvb;
enip_request_info_t *request_info;
enip_conn_val_t* conn_info = NULL;
gboolean FwdOpen = FALSE;
gboolean FwdOpenReply = FALSE;
enum enip_connid_type connid_type = ECIDT_UNKNOWN;
cip_safety_info_t* cip_safety;
guint32 trans_id, ucmm_request;
conversation_t *conversation;
item_count = tvb_get_letohs( tvb, offset );
count_item = proto_tree_add_item( tree, hf_enip_cpf_itemcount, tvb, offset, 2, ENC_LITTLE_ENDIAN );
count_tree = proto_item_add_subtree( count_item, ett_count_tree );
while ( item_count-- )
{
type_item = proto_tree_add_item( count_tree, hf_enip_cpf_typeid, tvb, offset+2, 2, ENC_LITTLE_ENDIAN );
item_tree = proto_item_add_subtree( type_item, ett_type_tree );
proto_tree_add_item( item_tree, hf_enip_cpf_length, tvb, offset+4, 2, ENC_LITTLE_ENDIAN );
item = tvb_get_letohs( tvb, offset+2 );
item_length = tvb_get_letohs( tvb, offset+4 );
if ( item_length )
{
switch ( item )
{
case CONNECTION_BASED:
proto_tree_add_item(item_tree, hf_enip_cpf_cai_connid, tvb, offset+6, 4, ENC_LITTLE_ENDIAN );
conn_info = enip_get_explicit_connid( pinfo, request_key, tvb_get_letohl( tvb, offset+6 ) );
if ( request_key )
{
request_key->type = EPDT_CONNECTED_TRANSPORT;
request_key->data.connected_transport.connid = (conn_info != NULL) ? conn_info->connid : 0;
}
col_append_fstr(pinfo->cinfo, COL_INFO, ", CONID: 0x%08X", tvb_get_letohl( tvb, offset+6 ) );
break;
case UNCONNECTED_MSG_DTLS:
ifacehndl = ENIP_CIP_INTERFACE;
proto_tree_add_item_ret_uint(item_tree, hf_enip_cpf_ucmm_request, tvb, offset+6, 2, ENC_LITTLE_ENDIAN, &ucmm_request );
proto_tree_add_item(item_tree, hf_enip_cpf_ucmm_msg_type, tvb, offset+6, 2, ENC_LITTLE_ENDIAN );
proto_tree_add_item_ret_uint(item_tree, hf_enip_cpf_ucmm_trans_id, tvb, offset+8, 4, ENC_LITTLE_ENDIAN, &trans_id );
proto_tree_add_item(item_tree, hf_enip_cpf_ucmm_status, tvb, offset+12, 4, ENC_LITTLE_ENDIAN );
offset += 10;
item_length -= 10;
if ( request_key == NULL)
{
conversation = find_or_create_conversation(pinfo);
request_key = wmem_new0(wmem_packet_scope(), enip_request_key_t);
request_key->requesttype = (ucmm_request & 0x8000) ? ENIP_RESPONSE_PACKET : ENIP_REQUEST_PACKET;
request_key->type = EPDT_UNKNOWN;
request_key->session_handle = GPOINTER_TO_UINT(conversation);
request_key->sender_context = trans_id;
request_key->conversation = conversation->index;
}
case UNCONNECTED_MSG:
request_info = NULL;
if ( request_key )
{
request_key->type = EPDT_UNCONNECTED;
request_info = enip_match_request( pinfo, tree, request_key );
}
next_tvb = tvb_new_subset_length( tvb, offset+6, item_length);
p_add_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_REQUEST_INFO, request_info);
if ( tvb_reported_length_remaining(next_tvb, 0) <= 0 || !dissector_try_uint(subdissector_srrd_table, ifacehndl, next_tvb, pinfo, dissector_tree) )
{
if ( tvb_reported_length_remaining(tvb, offset) > 0 )
call_data_dissector(next_tvb, pinfo, dissector_tree);
}
if ((request_info != NULL) && (request_info->cip_info != NULL) &&
(request_info->cip_info->connInfo != NULL) &&
(request_key != NULL) &&
(((request_info->cip_info->bService & 0x7F) == SC_CM_FWD_OPEN) ||
((request_info->cip_info->bService & 0x7F) == SC_CM_LARGE_FWD_OPEN))&&
(request_info->cip_info->dissector == dissector_get_uint_handle( subdissector_class_table, CI_CLS_CM)))
{
if (request_key->requesttype == ENIP_REQUEST_PACKET)
{
FwdOpen = TRUE;
}
else
{
FwdOpenReply = TRUE;
}
}
else
{
p_remove_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_REQUEST_INFO);
}
break;
case CONNECTION_TRANSPORT:
if ( command == SEND_UNIT_DATA )
{
request_info = NULL;
if ( request_key )
{
request_key->type = EPDT_CONNECTED_TRANSPORT;
request_key->data.connected_transport.sequence = tvb_get_letohs( tvb, offset+6 );
request_info = enip_match_request( pinfo, tree, request_key );
}
if ((!pinfo->fd->flags.visited) && (conn_info != NULL))
p_add_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_CONNECTION_INFO, conn_info);
proto_tree_add_item( item_tree, hf_enip_cpf_cdi_seqcnt, tvb, offset+6, 2, ENC_LITTLE_ENDIAN );
next_tvb = tvb_new_subset_length (tvb, offset+8, item_length-2);
if ((conn_info == NULL) && tvb_reported_length(next_tvb) < 2)
{
break;
}
if ((conn_info == NULL) || (conn_info->ClassID == CI_CLS_MR))
{
p_add_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_REQUEST_INFO, request_info);
if (!dissector_try_uint(subdissector_sud_table, ifacehndl, next_tvb, pinfo, dissector_tree) )
{
call_data_dissector(next_tvb, pinfo, dissector_tree );
}
p_remove_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_REQUEST_INFO);
}
else
{
call_dissector_with_data( cip_implicit_handle, next_tvb, pinfo, dissector_tree, GUINT_TO_POINTER(conn_info->ClassID) );
}
}
else
{
if (tvb_reported_length_remaining(tvb, offset+6) > 0)
{
next_tvb = tvb_new_subset_length(tvb, offset+6, item_length);
if (conn_info != NULL)
{
if (conn_info->safety.safety_seg == TRUE)
{
cip_safety = wmem_new(wmem_file_scope(), cip_safety_info_t);
cip_safety->conn_type = connid_type;
cip_safety->server_dir = (conn_info->TransportClass_trigger & CI_PRODUCTION_DIR_MASK) ? TRUE : FALSE;
cip_safety->format = conn_info->safety.format;
p_add_proto_data(wmem_file_scope(), pinfo, proto_cipsafety, 0, cip_safety);
call_dissector(cipsafety_handle, next_tvb, pinfo, dissector_tree);
}
else if (conn_info->motion == TRUE)
{
call_dissector(cipmotion_handle, next_tvb, pinfo, dissector_tree);
}
else
{
io_length = item_length;
if ((conn_info->TransportClass_trigger & CI_TRANSPORT_CLASS_MASK) == 1)
{
proto_tree_add_item( item_tree, hf_enip_cpf_cdi_seqcnt,
tvb, offset+6+(item_length-io_length), 2, ENC_LITTLE_ENDIAN );
io_length -= 2;
}
if ((io_length >= 4) &&
(((connid_type == ECIDT_O2T) && enip_OTrun_idle) ||
((connid_type == ECIDT_T2O) && enip_TOrun_idle)))
{
io_item = proto_tree_add_item( item_tree, hf_enip_cpf_cdi_32bitheader,
tvb, offset+6+(item_length-io_length), 4, ENC_LITTLE_ENDIAN );
io_tree = proto_item_add_subtree( io_item, ett_32bitheader_tree );
proto_tree_add_item(io_tree, hf_enip_cpf_cdi_32bitheader_roo,
tvb, offset+6+(item_length-io_length), 4, ENC_LITTLE_ENDIAN );
proto_tree_add_item(io_tree, hf_enip_cpf_cdi_32bitheader_coo,
tvb, offset+6+(item_length-io_length), 4, ENC_LITTLE_ENDIAN );
proto_tree_add_item(io_tree, hf_enip_cpf_cdi_32bitheader_run_idle,
tvb, offset+6+(item_length-io_length), 4, ENC_LITTLE_ENDIAN );
io_length -= 4;
}
proto_tree_add_item(item_tree, hf_enip_connection_transport_data, tvb, offset+6+(item_length-io_length), io_length, ENC_NA);
}
if (!pinfo->fd->flags.visited)
p_add_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_CONNECTION_INFO, conn_info);
}
else
{
if (!dissector_try_uint(subdissector_io_table, 0, next_tvb, pinfo, dissector_tree))
{
proto_tree_add_item(item_tree, hf_enip_connection_transport_data, tvb, offset+6, item_length, ENC_NA);
}
}
}
}
break;
case LIST_IDENTITY_RESP:
proto_tree_add_item( item_tree, hf_enip_encapver, tvb, offset+6, 2, ENC_LITTLE_ENDIAN );
sockaddr_tree = proto_tree_add_subtree( item_tree, tvb, offset+8, 16, ett_sockadd, NULL, "Socket Address");
proto_tree_add_item(sockaddr_tree, hf_enip_sinfamily,
tvb, offset+8, 2, ENC_BIG_ENDIAN );
proto_tree_add_item(sockaddr_tree, hf_enip_sinport,
tvb, offset+10, 2, ENC_BIG_ENDIAN );
proto_tree_add_item(sockaddr_tree, hf_enip_sinaddr,
tvb, offset+12, 4, ENC_BIG_ENDIAN );
proto_tree_add_item(sockaddr_tree, hf_enip_sinzero,
tvb, offset+16, 8, ENC_NA );
proto_tree_add_item(item_tree, hf_enip_lir_vendor,
tvb, offset+24, 2, ENC_LITTLE_ENDIAN );
proto_tree_add_item(item_tree, hf_enip_lir_devtype,
tvb, offset+26, 2, ENC_LITTLE_ENDIAN );
proto_tree_add_item(item_tree, hf_enip_lir_prodcode,
tvb, offset+28, 2, ENC_LITTLE_ENDIAN );
proto_tree_add_item(item_tree, hf_enip_lir_revision,
tvb, offset+30, 2, ENC_BIG_ENDIAN );
proto_tree_add_item(item_tree, hf_enip_lir_status,
tvb, offset+32, 2, ENC_LITTLE_ENDIAN );
proto_tree_add_item(item_tree, hf_enip_lir_serial,
tvb, offset+34, 4, ENC_LITTLE_ENDIAN );
name_length = tvb_get_guint8( tvb, offset+38 );
proto_tree_add_item( item_tree, hf_enip_lir_namelen,
tvb, offset+38, 1, ENC_LITTLE_ENDIAN );
proto_tree_add_item(item_tree, hf_enip_lir_name,
tvb, offset+39, name_length, ENC_ASCII|ENC_NA );
col_append_fstr( pinfo->cinfo, COL_INFO, ", %s", tvb_format_text(tvb, offset+39, name_length));
proto_tree_add_item(item_tree, hf_enip_lir_state,
tvb, offset+name_length+39, 1, ENC_LITTLE_ENDIAN );
break;
case SOCK_ADR_INFO_OT:
case SOCK_ADR_INFO_TO:
proto_tree_add_item(item_tree, hf_enip_sinfamily,
tvb, offset+6, 2, ENC_BIG_ENDIAN );
proto_tree_add_item(item_tree, hf_enip_sinport,
tvb, offset+8, 2, ENC_BIG_ENDIAN );
proto_tree_add_item(item_tree, hf_enip_sinaddr,
tvb, offset+10, 4, ENC_BIG_ENDIAN );
proto_tree_add_item( item_tree, hf_enip_sinzero,
tvb, offset+14, 8, ENC_NA );
if ((FwdOpen == TRUE) || (FwdOpenReply == TRUE))
{
request_info = (enip_request_info_t *)p_get_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_REQUEST_INFO);
if (request_info != NULL)
{
if (item == SOCK_ADR_INFO_OT)
{
request_info->cip_info->connInfo->O2T.port = tvb_get_ntohs(tvb, offset+8);
alloc_address_tvb(wmem_file_scope(), &request_info->cip_info->connInfo->O2T.ipaddress,
AT_IPv4, sizeof(guint32), tvb, offset+10);
}
else
{
request_info->cip_info->connInfo->T2O.port = tvb_get_ntohs(tvb, offset+8);
alloc_address_tvb(wmem_file_scope(), &request_info->cip_info->connInfo->T2O.ipaddress,
AT_IPv4, sizeof(guint32), tvb, offset+10);
}
}
}
break;
case SEQ_ADDRESS:
conn_info = enip_get_io_connid( pinfo, tvb_get_letohl( tvb, offset+6 ), &connid_type);
proto_tree_add_item(item_tree, hf_enip_cpf_sai_connid, tvb, offset+6, 4, ENC_LITTLE_ENDIAN );
proto_tree_add_item(item_tree, hf_enip_cpf_sai_seqnum, tvb, offset+10, 4, ENC_LITTLE_ENDIAN );
col_add_fstr(pinfo->cinfo, COL_INFO, "Connection: ID=0x%08X, SEQ=%010d",
tvb_get_letohl( tvb, offset+6 ),
tvb_get_letohl( tvb, offset+10 ) );
break;
case LIST_SERVICES_RESP:
proto_tree_add_item( item_tree, hf_enip_encapver, tvb, offset+6, 2, ENC_LITTLE_ENDIAN );
temp_item = proto_tree_add_item( item_tree, hf_enip_lsr_capaflags, tvb, offset+8, 2, ENC_LITTLE_ENDIAN );
temp_tree = proto_item_add_subtree( temp_item, ett_lsrcf );
proto_tree_add_item( temp_tree, hf_enip_lsr_tcp, tvb, offset+8, 2, ENC_LITTLE_ENDIAN );
proto_tree_add_item( temp_tree, hf_enip_lsr_udp, tvb, offset+8, 2, ENC_LITTLE_ENDIAN );
proto_tree_add_item( item_tree, hf_enip_lsr_servicename, tvb, offset+10, 16, ENC_ASCII|ENC_NA );
col_append_fstr( pinfo->cinfo, COL_INFO, ", %s",
tvb_format_stringzpad(tvb, offset+10, 16) );
break;
default:
proto_tree_add_item(item_tree, hf_enip_cpf_data, tvb, offset+6, item_length, ENC_NA);
break;
}
}
offset = offset + item_length + 4;
}
if (FwdOpenReply == TRUE)
{
request_info = (enip_request_info_t *)p_get_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_REQUEST_INFO);
if (request_info != NULL)
{
enip_open_cip_connection(pinfo, request_info->cip_info->connInfo);
}
p_remove_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_REQUEST_INFO);
}
else if (FwdOpen == TRUE)
{
p_remove_proto_data(wmem_file_scope(), pinfo, proto_enip, ENIP_REQUEST_INFO);
}
}
static enum enip_packet_type
classify_packet(packet_info *pinfo)
{
if (((ENIP_ENCAP_PORT == pinfo->srcport && ENIP_ENCAP_PORT != pinfo->destport)) ||
((ENIP_SECURE_PORT == pinfo->srcport && ENIP_SECURE_PORT != pinfo->destport)))
{
return ENIP_RESPONSE_PACKET;
}
else if (((ENIP_ENCAP_PORT != pinfo->srcport && ENIP_ENCAP_PORT == pinfo->destport)) ||
((ENIP_SECURE_PORT != pinfo->srcport && ENIP_SECURE_PORT == pinfo->destport)))
{
return ENIP_REQUEST_PACKET;
}
else
{
return ENIP_CANNOT_CLASSIFY;
}
}
static guint
get_enip_pdu_len(packet_info *pinfo _U_, tvbuff_t *tvb, int offset, void *data _U_)
{
guint16 plen;
plen = tvb_get_letohs(tvb, offset + 2);
return plen + 24;
}
static int
dissect_enip_pdu(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void* data _U_)
{
enum enip_packet_type packet_type;
guint16 encap_cmd, encap_data_length;
const char *pkt_type_str = "";
guint32 ifacehndl;
enip_request_key_t request_key;
conversation_t *conversation;
proto_item *ti;
proto_tree *enip_tree, *header_tree = NULL, *csftree;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "ENIP");
col_clear(pinfo->cinfo, COL_INFO);
encap_cmd = tvb_get_letohs( tvb, 0 );
packet_type = classify_packet(pinfo);
switch ( packet_type )
{
case ENIP_REQUEST_PACKET:
pkt_type_str = "Req";
break;
case ENIP_RESPONSE_PACKET:
pkt_type_str = "Rsp";
break;
case ENIP_CANNOT_CLASSIFY:
default:
pkt_type_str = "?";
}
col_append_sep_fstr(pinfo->cinfo, COL_INFO, " | ", "%s (%s)",
val_to_str(encap_cmd, encap_cmd_vals, "Unknown Command (0x%04x)"),
pkt_type_str );
conversation = find_or_create_conversation(pinfo);
memset( &request_key, 0, sizeof(enip_request_key_t) );
request_key.requesttype = packet_type;
request_key.type = EPDT_UNKNOWN;
request_key.session_handle = tvb_get_letohl( tvb, 4 );
request_key.sender_context = tvb_get_letoh64( tvb, 12 );
request_key.conversation = conversation->index;
encap_data_length = tvb_get_letohs( tvb, 2 );
enip_tree = NULL;
if (tree) {
ti = proto_tree_add_item(tree, proto_enip, tvb, 0, -1, ENC_NA );
enip_tree = proto_item_add_subtree(ti, ett_enip);
header_tree = proto_tree_add_subtree( enip_tree, tvb, 0, 24, ett_enip, NULL, "Encapsulation Header");
proto_tree_add_item( header_tree, hf_enip_command, tvb, 0, 2, ENC_LITTLE_ENDIAN );
encap_data_length = tvb_get_letohs( tvb, 2 );
proto_tree_add_item( header_tree, hf_enip_length, tvb, 2, 2, ENC_LITTLE_ENDIAN );
proto_tree_add_item( header_tree, hf_enip_session, tvb, 4, 4, ENC_LITTLE_ENDIAN );
proto_tree_add_item( header_tree, hf_enip_status, tvb, 8, 4, ENC_LITTLE_ENDIAN );
if ((encap_cmd == LIST_IDENTITY) &&
((encap_data_length == 0) || (packet_type == ENIP_REQUEST_PACKET)))
{
proto_tree_add_item( header_tree, hf_enip_listid_delay, tvb, 12, 2, ENC_LITTLE_ENDIAN );
proto_tree_add_item( header_tree, hf_enip_sendercontex, tvb, 14, 6, ENC_NA );
}
else
{
proto_tree_add_item( header_tree, hf_enip_sendercontex, tvb, 12, 8, ENC_NA );
}
proto_tree_add_item( header_tree, hf_enip_options, tvb, 20, 4, ENC_LITTLE_ENDIAN );
proto_item_append_text( ti, ", Session: 0x%08X, %s", tvb_get_letohl( tvb, 4 ),
val_to_str( encap_cmd, encap_cmd_vals, "Unknown Command (0x%04x)" ) );
}
switch ( encap_cmd )
{
case REGISTER_SESSION:
case UNREGISTER_SESSION:
col_append_fstr( pinfo->cinfo, COL_INFO, ", Session: 0x%08X",
tvb_get_letohl( tvb, 4 ) );
break;
}
if ( encap_data_length )
{
csftree = proto_tree_add_subtree( enip_tree, tvb, 24, encap_data_length,
ett_command_tree, NULL, "Command Specific Data");
switch ( encap_cmd )
{
case NOP:
break;
case LIST_SERVICES:
dissect_cpf( &request_key, encap_cmd, tvb, pinfo, csftree, tree, 24, 0 );
break;
case LIST_IDENTITY:
dissect_cpf( &request_key, encap_cmd, tvb, pinfo, csftree, tree, 24, 0 );
break;
case LIST_INTERFACES:
dissect_cpf( &request_key, encap_cmd, tvb, pinfo, csftree, tree, 24, 0 );
break;
case REGISTER_SESSION:
proto_tree_add_item( csftree, hf_enip_rs_version, tvb, 24, 2, ENC_LITTLE_ENDIAN );
proto_tree_add_item( csftree, hf_enip_rs_optionflags, tvb, 26, 2, ENC_LITTLE_ENDIAN );
break;
case UNREGISTER_SESSION:
break;
case SEND_RR_DATA:
proto_tree_add_item( csftree, hf_enip_srrd_ifacehnd, tvb, 24, 4, ENC_LITTLE_ENDIAN );
proto_tree_add_item( csftree, hf_enip_timeout, tvb, 28, 2, ENC_LITTLE_ENDIAN );
ifacehndl = tvb_get_letohl( tvb, 24 );
dissect_cpf( &request_key, encap_cmd, tvb, pinfo, csftree, tree, 30, ifacehndl );
break;
case SEND_UNIT_DATA:
proto_tree_add_item(csftree, hf_enip_sud_ifacehnd, tvb, 24, 4, ENC_LITTLE_ENDIAN);
proto_tree_add_item( csftree, hf_enip_timeout, tvb, 28, 2, ENC_LITTLE_ENDIAN );
ifacehndl = tvb_get_letohl( tvb, 24 );
dissect_cpf( &request_key, encap_cmd, tvb, pinfo, csftree, tree, 30, ifacehndl );
break;
case INDICATE_STATUS:
case CANCEL:
default:
proto_tree_add_item(header_tree, hf_enip_encap_data, tvb, 24, encap_data_length, ENC_NA);
break;
}
}
col_set_fence(pinfo->cinfo, COL_INFO);
return tvb_captured_length(tvb);
}
static int
dissect_enip_udp(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void *data)
{
if (tvb_captured_length(tvb) < 4)
return 0;
return dissect_enip_pdu(tvb, pinfo, tree, data);
}
static int
dissect_enip_tcp(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void *data)
{
if (tvb_captured_length(tvb) < 4)
return 0;
tcp_dissect_pdus(tvb, pinfo, tree, enip_desegment, 4, get_enip_pdu_len, dissect_enip_pdu, data);
return tvb_captured_length(tvb);
}
static int
dissect_enipio(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void* data _U_)
{
proto_item *ti;
proto_tree *enip_tree;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "ENIP");
ti = proto_tree_add_item(tree, proto_enip, tvb, 0, -1, ENC_NA );
enip_tree = proto_item_add_subtree(ti, ett_enip);
dissect_cpf( NULL, 0xFFFF, tvb, pinfo, enip_tree, tree, 0, 0 );
return tvb_captured_length(tvb);
}
static gboolean
dissect_dlr(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void *data _U_)
{
proto_item *ti;
proto_tree *dlr_tree;
guint8 dlr_subtype;
guint8 dlr_protover;
guint8 dlr_frametype;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "DLR");
col_clear(pinfo->cinfo, COL_INFO);
ti = proto_tree_add_item(tree, proto_dlr, tvb, 0, -1, ENC_NA );
dlr_tree = proto_item_add_subtree( ti, ett_dlr );
dlr_subtype = tvb_get_guint8(tvb, DLR_CFH_SUB_TYPE);
dlr_protover = tvb_get_guint8(tvb, DLR_CFH_PROTO_VERSION);
proto_tree_add_uint( dlr_tree, hf_dlr_ringsubtype, tvb, DLR_CFH_SUB_TYPE, 1, dlr_subtype );
proto_tree_add_uint( dlr_tree, hf_dlr_ringprotoversion, tvb, DLR_CFH_PROTO_VERSION, 1, dlr_protover );
dlr_frametype = tvb_get_guint8(tvb, DLR_MPF_FRAME_TYPE);
proto_tree_add_item( dlr_tree, hf_dlr_frametype, tvb, DLR_MPF_FRAME_TYPE, 1, ENC_BIG_ENDIAN );
proto_tree_add_item( dlr_tree, hf_dlr_sourceport, tvb, DLR_MPF_SOURCE_PORT, 1, ENC_BIG_ENDIAN );
proto_tree_add_item( dlr_tree, hf_dlr_sourceip, tvb, DLR_MPF_SOURCE_IP, 4, ENC_BIG_ENDIAN );
proto_tree_add_item( dlr_tree, hf_dlr_sequenceid, tvb, DLR_MPF_SEQUENCE_ID, 4, ENC_BIG_ENDIAN );
col_add_fstr(pinfo->cinfo, COL_INFO, "%s",
val_to_str(dlr_frametype, dlr_frame_type_vals, "Unknown (0x%04x)") );
if ( dlr_frametype == DLR_FT_BEACON )
{
proto_tree_add_item( dlr_tree, hf_dlr_ringstate, tvb, DLR_BE_RING_STATE, 1, ENC_BIG_ENDIAN );
proto_tree_add_item( dlr_tree, hf_dlr_supervisorprecedence, tvb, DLR_BE_SUPERVISOR_PRECEDENCE, 1, ENC_BIG_ENDIAN );
proto_tree_add_item( dlr_tree, hf_dlr_beaconinterval, tvb, DLR_BE_BEACON_INTERVAL, 4, ENC_BIG_ENDIAN );
proto_tree_add_item( dlr_tree, hf_dlr_beacontimeout, tvb, DLR_BE_BEACON_TIMEOUT, 4, ENC_BIG_ENDIAN );
proto_tree_add_item( dlr_tree, hf_dlr_beaconreserved, tvb, DLR_BE_RESERVED, 20, ENC_NA );
}
else if ( dlr_frametype == DLR_FT_NEIGHBOR_REQ )
{
proto_tree_add_item( dlr_tree, hf_dlr_nreqreserved, tvb, DLR_NREQ_RESERVED, 30, ENC_NA );
}
else if ( dlr_frametype == DLR_FT_NEIGHBOR_RES )
{
proto_tree_add_item( dlr_tree, hf_dlr_nressourceport, tvb, DLR_NRES_SOURCE_PORT, 1, ENC_BIG_ENDIAN );
proto_tree_add_item( dlr_tree, hf_dlr_nresreserved, tvb, DLR_NRES_RESERVED, 29, ENC_NA );
}
else if ( dlr_frametype == DLR_FT_LINK_STAT )
{
proto_item* flag_item;
proto_tree* flag_tree;
flag_item = proto_tree_add_item( dlr_tree, hf_dlr_lnknbrstatus, tvb, DLR_LNS_SOURCE_PORT, 1, ENC_BIG_ENDIAN );
flag_tree = proto_item_add_subtree(flag_item, ett_dlr_lnknbrstatus_flags);
proto_tree_add_item(flag_tree, hf_dlr_lnknbrstatus_port1, tvb, DLR_LNS_SOURCE_PORT, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(flag_tree, hf_dlr_lnknbrstatus_port2, tvb, DLR_LNS_SOURCE_PORT, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(flag_tree, hf_dlr_lnknbrstatus_reserved, tvb, DLR_LNS_SOURCE_PORT, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(flag_tree, hf_dlr_lnknbrstatus_frame_type, tvb, DLR_LNS_SOURCE_PORT, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item( dlr_tree, hf_dlr_lnknbrreserved, tvb, DLR_LNS_RESERVED, 29, ENC_NA );
}
else if ( dlr_frametype == DLR_FT_LOCATE_FLT )
{
proto_tree_add_item( dlr_tree, hf_dlr_lfreserved, tvb, DLR_LF_RESERVED, 30, ENC_NA );
}
else if ( dlr_frametype == DLR_FT_ANNOUNCE )
{
proto_tree_add_item( dlr_tree, hf_dlr_ringstate, tvb, DLR_AN_RING_STATE, 1, ENC_BIG_ENDIAN );
proto_tree_add_item( dlr_tree, hf_dlr_anreserved, tvb, DLR_AN_RESERVED, 29, ENC_NA );
}
else if ( dlr_frametype == DLR_FT_SIGN_ON )
{
guint16 nCnt;
guint16 nNumNodes;
guint16 nOffset;
nNumNodes = tvb_get_ntohs(tvb, DLR_SO_NUM_NODES);
proto_tree_add_uint( dlr_tree, hf_dlr_sonumnodes, tvb, DLR_SO_NUM_NODES, 2, nNumNodes );
for( nCnt = 0, nOffset = DLR_SO_NODE_1_MAC; nCnt < nNumNodes; nCnt++ )
{
proto_tree_add_item( dlr_tree, hf_dlr_somac, tvb, nOffset, 6, ENC_NA );
nOffset += 6;
proto_tree_add_item( dlr_tree, hf_dlr_soip, tvb, nOffset, 4, ENC_BIG_ENDIAN );
nOffset += 4;
}
if ( nOffset < 42 )
{
proto_tree_add_item( dlr_tree, hf_dlr_soreserved, tvb, nOffset, 42 - nOffset, ENC_NA );
}
}
else if ( dlr_frametype == DLR_FT_ADVERTISE )
{
proto_tree_add_item( dlr_tree, hf_dlr_advgatewaystate, tvb, DLR_ADV_GATEWAY_STATE, 1, ENC_BIG_ENDIAN );
proto_tree_add_item( dlr_tree, hf_dlr_advgatewayprecedence, tvb, DLR_ADV_GATEWAY_PRECEDENCE, 1, ENC_BIG_ENDIAN );
proto_tree_add_item( dlr_tree, hf_dlr_advadvertiseinterval, tvb, DLR_ADV_ADVERTISE_INTERVAL, 4, ENC_BIG_ENDIAN );
proto_tree_add_item( dlr_tree, hf_dlr_advadvertisetimeout, tvb, DLR_ADV_ADVERTISE_TIMEOUT, 4, ENC_BIG_ENDIAN );
proto_tree_add_item( dlr_tree, hf_dlr_advlearningupdateenable, tvb, DLR_ADV_LEARNING_UPDATE_ENABLE, 1, ENC_BIG_ENDIAN );
proto_tree_add_item( dlr_tree, hf_dlr_advreserved, tvb, DLR_ADV_RESERVED, 19, ENC_NA );
}
else if ( dlr_frametype == DLR_FT_FLUSH_TABLES )
{
proto_tree_add_item( dlr_tree, hf_dlr_flushlearningupdateenable, tvb, DLR_FLUSH_LEARNING_UPDATE_ENABLE, 1, ENC_BIG_ENDIAN );
proto_tree_add_item( dlr_tree, hf_dlr_flushreserved, tvb, DLR_FLUSH_RESERVED, 29, ENC_NA );
}
else if ( dlr_frametype == DLR_FT_LEARNING_UPDATE )
{
proto_tree_add_item( dlr_tree, hf_dlr_learnreserved, tvb, DLR_LEARN_RESERVED, 34, ENC_NA );
}
else
{
}
return tvb_captured_length(tvb);
}
void
proto_register_enip(void)
{
static hf_register_info hf[] = {
{ &hf_enip_command,
{ "Command", "enip.command",
FT_UINT16, BASE_HEX, VALS(encap_cmd_vals), 0,
"Encapsulation command", HFILL }},
{ &hf_enip_length,
{ "Length", "enip.length",
FT_UINT16, BASE_DEC, NULL, 0,
"Encapsulation length", HFILL }},
{ &hf_enip_session,
{ "Session Handle", "enip.session",
FT_UINT32, BASE_HEX, NULL, 0,
"Session identification", HFILL }},
{ &hf_enip_status,
{ "Status", "enip.status",
FT_UINT32, BASE_HEX, VALS(encap_status_vals), 0,
"Status code", HFILL }},
{ &hf_enip_sendercontex,
{ "Sender Context", "enip.context",
FT_BYTES, BASE_NONE, NULL, 0,
"Information pertinent to the sender", HFILL }},
{ &hf_enip_listid_delay,
{ "Max Response Delay", "enip.listid_delay",
FT_UINT16, BASE_DEC, NULL, 0,
"Maximum random delay allowed by target", HFILL }},
{ &hf_enip_options,
{ "Options", "enip.options",
FT_UINT32, BASE_HEX, NULL, 0,
"Options flags", HFILL }},
{ &hf_enip_encapver,
{ "Encapsulation Version", "enip.encapver",
FT_UINT16, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_enip_sinfamily,
{ "sin_family", "enip.sinfamily",
FT_UINT16, BASE_DEC, NULL, 0,
"Socket Address.Sin Family", HFILL }},
{ &hf_enip_sinport,
{ "sin_port", "enip.sinport",
FT_UINT16, BASE_DEC, NULL, 0,
"Socket Address.Sin Port", HFILL }},
{ &hf_enip_sinaddr,
{ "sin_addr", "enip.sinaddr",
FT_IPv4, BASE_NONE, NULL, 0,
"Socket Address.Sin Addr", HFILL }},
{ &hf_enip_sinzero,
{ "sin_zero", "enip.sinzero",
FT_BYTES, BASE_NONE, NULL, 0,
"Socket Address.Sin Zero", HFILL }},
{ &hf_enip_timeout,
{ "Timeout", "enip.timeout",
FT_UINT16, BASE_DEC, NULL, 0,
"Encapsulation Timeout", HFILL }},
{ &hf_enip_encap_data,
{ "Encap Data", "enip.encap_data",
FT_BYTES, BASE_NONE, NULL, 0,
"Encapsulation Data", HFILL }},
{ &hf_enip_lsr_capaflags,
{ "Capability Flags", "enip.lsr.capaflags",
FT_UINT16, BASE_HEX, NULL, 0,
"ListServices Reply: Capability Flags", HFILL }},
{ &hf_enip_lsr_tcp,
{ "Supports CIP Encapsulation via TCP", "enip.lsr.capaflags.tcp",
FT_BOOLEAN, 16, TFS(&tfs_true_false), 0x0020,
"ListServices Reply: Supports CIP Encapsulation via TCP", HFILL }},
{ &hf_enip_lsr_udp,
{ "Supports CIP Class 0 or 1 via UDP", "enip.lsr.capaflags.udp",
FT_BOOLEAN, 16, TFS(&tfs_true_false), 0x0100,
"ListServices Reply: Supports CIP Class 0 or 1 via UDP", HFILL }},
{ &hf_enip_lsr_servicename,
{ "Name of Service", "enip.lsr.servicename",
FT_STRING, BASE_NONE, NULL, 0,
"ListServices Reply: Name of Service", HFILL }},
{ &hf_enip_rs_version,
{ "Protocol Version", "enip.rs.version",
FT_UINT16, BASE_DEC, NULL, 0,
"Register Session: Protocol Version", HFILL }},
{ &hf_enip_rs_optionflags,
{ "Option Flags", "enip.rs.flags",
FT_UINT16, BASE_HEX, NULL, 0,
"Register Session: Option Flags", HFILL }},
{ &hf_enip_srrd_ifacehnd,
{ "Interface Handle", "enip.srrd.iface",
FT_UINT32, BASE_HEX, VALS(enip_interface_handle_vals), 0,
"SendRRData: Interface handle", HFILL }},
{ &hf_enip_sud_ifacehnd,
{ "Interface Handle", "enip.sud.iface",
FT_UINT32, BASE_HEX, VALS(enip_interface_handle_vals), 0,
"SendUnitData: Interface handle", HFILL }},
{ &hf_enip_lir_vendor,
{ "Vendor ID", "enip.lir.vendor",
FT_UINT16, BASE_HEX|BASE_EXT_STRING, &cip_vendor_vals_ext, 0,
"ListIdentity Reply: Vendor ID", HFILL }},
{ &hf_enip_lir_devtype,
{ "Device Type", "enip.lir.devtype",
FT_UINT16, BASE_DEC|BASE_EXT_STRING, &cip_devtype_vals_ext, 0,
"ListIdentity Reply: Device Type", HFILL }},
{ &hf_enip_lir_prodcode,
{ "Product Code", "enip.lir.prodcode",
FT_UINT16, BASE_DEC, NULL, 0,
"ListIdentity Reply: Product Code", HFILL }},
{ &hf_enip_lir_revision,
{ "Revision", "enip.lir.revision",
FT_UINT16, BASE_CUSTOM, CF_FUNC(enip_fmt_lir_revision), 0,
"ListIdentity Reply: Revision", HFILL }},
{ &hf_enip_lir_status,
{ "Status", "enip.lir.status",
FT_UINT16, BASE_HEX, NULL, 0,
"ListIdentity Reply: Status", HFILL }},
{ &hf_enip_lir_serial,
{ "Serial Number", "enip.lir.serial",
FT_UINT32, BASE_HEX, NULL, 0,
"ListIdentity Reply: Serial Number", HFILL }},
{ &hf_enip_lir_namelen,
{ "Product Name Length", "enip.lir.namelen",
FT_UINT8, BASE_DEC, NULL, 0,
"ListIdentity Reply: Product Name Length", HFILL }},
{ &hf_enip_lir_name,
{ "Product Name", "enip.lir.name",
FT_STRING, BASE_NONE, NULL, 0,
"ListIdentity Reply: Product Name", HFILL }},
{ &hf_enip_lir_state,
{ "State", "enip.lir.state",
FT_UINT8, BASE_HEX, NULL, 0,
"ListIdentity Reply: State", HFILL }},
{ &hf_enip_cpf_itemcount,
{ "Item Count", "enip.cpf.itemcount",
FT_UINT16, BASE_DEC, NULL, 0,
"Common Packet Format: Item Count", HFILL }},
{ &hf_enip_cpf_typeid,
{ "Type ID", "enip.cpf.typeid",
FT_UINT16, BASE_HEX, VALS(cdf_type_vals), 0,
"Common Packet Format: Type of encapsulated item", HFILL }},
{ &hf_enip_cpf_length,
{ "Length", "enip.cpf.length",
FT_UINT16, BASE_DEC, NULL, 0,
"Common Packet Format: Length", HFILL }},
{ &hf_enip_cpf_cdi_seqcnt,
{ "Sequence Count", "enip.cpf.cdi.seqcnt",
FT_UINT16, BASE_HEX, NULL, 0,
"Common Packet Format: Connected Data Item, Sequence Count", HFILL }},
{ &hf_enip_cpf_cdi_32bitheader,
{ "32-bit Header", "enip.cpf.cdi.32bitheader",
FT_UINT32, BASE_HEX, NULL, 0,
"Common Packet Format: Connected Data Item, 32-bit Header", HFILL }},
{ &hf_enip_cpf_cdi_32bitheader_roo,
{ "ROO", "enip.cpf.cdi.roo",
FT_UINT32, BASE_HEX, NULL, 0xC,
"Common Packet Format: Connected Data Item, Ready for Ownership of Outputs", HFILL }},
{ &hf_enip_cpf_cdi_32bitheader_coo,
{ "COO", "enip.cpf.cdi.coo",
FT_UINT32, BASE_HEX, NULL, 0x2,
"Common Packet Format: Connected Data Item, Claim Output Ownership", HFILL }},
{ &hf_enip_cpf_cdi_32bitheader_run_idle,
{ "Run/Idle", "enip.cpf.cdi.run_idle",
FT_UINT32, BASE_HEX, VALS(enip_run_idle_vals), 0x1,
"Common Packet Format: Connected Data Item, Run/Idle", HFILL }},
{ &hf_enip_cpf_cai_connid,
{ "Connection ID", "enip.cpf.cai.connid",
FT_UINT32, BASE_HEX, NULL, 0,
"Common Packet Format: Connection Address Item, Connection Identifier", HFILL }},
{ &hf_enip_cpf_ucmm_request,
{ "Request/Response", "enip.cpf.ucmm.request",
FT_UINT16, BASE_DEC, VALS(cip_sc_rr), 0x8000,
"Common Packet Format: UCMM Request/Response", HFILL }},
{ &hf_enip_cpf_ucmm_msg_type,
{ "Unconn Msg Type", "enip.cpf.ucmm.msg_type",
FT_UINT16, BASE_DEC, VALS(unconn_msg_type_vals), 0x7FFF,
"Common Packet Format: UCMM Transaction ID", HFILL }},
{ &hf_enip_cpf_ucmm_trans_id,
{ "Transaction ID", "enip.cpf.ucmm.trans_id",
FT_UINT32, BASE_HEX, NULL, 0,
"Common Packet Format: UCMM Transaction ID", HFILL }},
{ &hf_enip_cpf_ucmm_status,
{ "UCMM Status", "enip.cpf.ucmm.status",
FT_UINT32, BASE_HEX, VALS(encap_status_vals), 0,
"Common Packet Format: UCMM Status", HFILL }},
{ &hf_enip_cpf_sai_connid,
{ "Connection ID", "enip.cpf.sai.connid",
FT_UINT32, BASE_HEX, NULL, 0,
"Common Packet Format: Sequenced Address Item, Connection Identifier", HFILL }},
{ &hf_enip_cpf_sai_seqnum,
{ "Sequence Number", "enip.cpf.sai.seq",
FT_UINT32, BASE_DEC, NULL, 0,
"Common Packet Format: Sequenced Address Item, Sequence Number", HFILL }},
{ &hf_enip_cpf_data,
{ "Data", "enip.cpf.data",
FT_BYTES, BASE_NONE, NULL, 0,
"Common Packet Format: Unknown Data", HFILL }},
{ &hf_enip_response_in,
{ "Response In", "enip.response_in",
FT_FRAMENUM, BASE_NONE, FRAMENUM_TYPE(FT_FRAMENUM_RESPONSE), 0x0,
"The response to this ENIP request is in this frame", HFILL }},
{ &hf_enip_response_to,
{ "Request In", "enip.response_to",
FT_FRAMENUM, BASE_NONE, FRAMENUM_TYPE(FT_FRAMENUM_REQUEST), 0x0,
"This is a response to the ENIP request in this frame", HFILL }},
{ &hf_enip_time,
{ "Time", "enip.time",
FT_RELATIVE_TIME, BASE_NONE, NULL, 0x0,
"The time between the Call and the Reply", HFILL }},
{ &hf_enip_connection_transport_data,
{ "Data", "enip.connection_transport_data",
FT_BYTES, BASE_NONE|BASE_ALLOW_ZERO, NULL, 0x0,
"Connection Transport Data", HFILL }},
{ &hf_tcpip_status,
{ "Status", "cip.tcpip.status",
FT_UINT32, BASE_HEX, NULL, 0,
NULL, HFILL }},
{ &hf_tcpip_status_interface_config,
{ "Interface Configuration Status", "cip.tcpip.status.interface_config",
FT_UINT32, BASE_DEC, VALS(enip_tcpip_status_interface_config_vals), 0x0000000F,
NULL, HFILL }},
{ &hf_tcpip_status_mcast_pending,
{ "MCast Pending", "cip.tcpip.status.mcast_pending",
FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x00000010,
NULL, HFILL }},
{ &hf_tcpip_status_interface_config_pending,
{ "Interface Configuration Pending", "cip.tcpip.status.interface_config_pending",
FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x00000020,
NULL, HFILL }},
{ &hf_tcpip_status_acd,
{ "ACD Status", "cip.tcpip.status.acd",
FT_UINT32, BASE_DEC, VALS(enip_tcpip_status_acd_vals), 0x00000040,
NULL, HFILL }},
{ &hf_tcpip_status_reserved,
{ "Reserved", "cip.tcpip.status.reserved",
FT_UINT32, BASE_HEX, NULL, 0xFFFFFF80,
NULL, HFILL }},
{ &hf_tcpip_config_cap,
{ "Configuration Capability", "cip.tcpip.config_cap",
FT_UINT32, BASE_HEX, NULL, 0,
NULL, HFILL }},
{ &hf_tcpip_config_cap_bootp,
{ "BOOTP Client", "cip.tcpip.config_cap.bootp",
FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x00000001,
NULL, HFILL }},
{ &hf_tcpip_config_cap_dns,
{ "DNS Client", "cip.tcpip.config_cap.dns",
FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x00000002,
NULL, HFILL }},
{ &hf_tcpip_config_cap_dhcp,
{ "DHCP Client", "cip.tcpip.config_cap.dhcp",
FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x00000004,
NULL, HFILL }},
{ &hf_tcpip_config_cap_dhcp_dns_update,
{ "DHCP-DNS Update", "cip.tcpip.config_cap.dhcp_dns_update",
FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x00000008,
NULL, HFILL }},
{ &hf_tcpip_config_cap_config_settable,
{ "Configuration Settable", "cip.tcpip.config_cap.config_settable",
FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x00000010,
NULL, HFILL }},
{ &hf_tcpip_config_cap_hardware_config,
{ "Hardware Configurable", "cip.tcpip.config_cap.hardware_config",
FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x00000020,
NULL, HFILL }},
{ &hf_tcpip_config_cap_interface_reset,
{ "Interface Configuration Change Requires Reset", "cip.tcpip.config_cap.interface_reset",
FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x00000040,
NULL, HFILL }},
{ &hf_tcpip_config_cap_acd,
{ "ACD Capable", "cip.tcpip.config_cap.acd",
FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x00000080,
NULL, HFILL }},
{ &hf_tcpip_config_cap_reserved,
{ "Reserved", "cip.tcpip.config_cap.reserved",
FT_UINT32, BASE_HEX, NULL, 0xFFFFFF00,
NULL, HFILL }},
{ &hf_tcpip_config_control,
{ "Configuration Control", "cip.tcpip.config_control",
FT_UINT32, BASE_HEX, NULL, 0,
NULL, HFILL }},
{ &hf_tcpip_config_control_config,
{ "Configuration Method", "cip.tcpip.config_control.config",
FT_UINT32, BASE_DEC, VALS(enip_tcpip_config_control_config_vals), 0x0000000F,
NULL, HFILL }},
{ &hf_tcpip_config_control_dns,
{ "DNS Enable", "cip.tcpip.config_control.dns",
FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x00000010,
NULL, HFILL }},
{ &hf_tcpip_config_control_reserved,
{ "Reserved", "cip.tcpip.config_control.reserved",
FT_UINT32, BASE_HEX, NULL, 0xFFFFFFE0,
NULL, HFILL }},
{ &hf_tcpip_ic_ip_addr,
{ "IP Address", "cip.tcpip.ip_addr",
FT_IPv4, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_tcpip_ic_subnet_mask,
{ "Subnet Mask", "cip.tcpip.subnet_mask",
FT_IPv4, BASE_NETMASK, NULL, 0,
NULL, HFILL }},
{ &hf_tcpip_ic_gateway,
{ "Gateway", "cip.tcpip.gateway",
FT_IPv4, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_tcpip_ic_name_server,
{ "Name Server", "cip.tcpip.name_server",
FT_IPv4, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_tcpip_ic_name_server2,
{ "Name Server2", "cip.tcpip.name_server2",
FT_IPv4, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_tcpip_ic_domain_name,
{ "Domain Name", "cip.tcpip.domain_name",
FT_STRING, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_tcpip_hostname,
{ "Hostname", "cip.tcpip.hostname",
FT_STRING, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_tcpip_snn_timestamp,
{ "Safety Network Number (Timestamp)", "cip.tcpip.snn.timestamp",
FT_ABSOLUTE_TIME, ABSOLUTE_TIME_UTC, NULL, 0,
NULL, HFILL }
},
{ &hf_tcpip_snn_date,
{ "Safety Network Number (Manual) Date", "cip.tcpip.snn.date",
FT_UINT16, BASE_HEX, VALS(cipsafety_ssn_date_vals), 0,
NULL, HFILL }
},
{ &hf_tcpip_snn_time,
{ "Safety Network Number (Manual) Time", "cip.tcpip.snn.time",
FT_UINT32, BASE_HEX, NULL, 0,
NULL, HFILL }
},
{ &hf_tcpip_ttl_value,
{ "TTL Value", "cip.tcpip.ttl_value",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_tcpip_mcast_alloc,
{ "Alloc Control", "cip.tcpip.mcast.alloc",
FT_UINT8, BASE_DEC, VALS(enip_tcpip_mcast_alloc_vals), 0,
NULL, HFILL }},
{ &hf_tcpip_mcast_reserved,
{ "Reserved", "cip.tcpip.mcast.reserved",
FT_UINT8, BASE_HEX, NULL, 0,
NULL, HFILL }},
{ &hf_tcpip_mcast_num_mcast,
{ "Num MCast", "cip.tcpip.mcast.num_mcast",
FT_UINT16, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_tcpip_mcast_addr_start,
{ "MCast Start Addr", "cip.tcpip.mcast.addr_start",
FT_IPv4, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_tcpip_select_acd,
{ "Select ACD", "cip.tcpip.select_acd",
FT_BOOLEAN, BASE_NONE, TFS(&tfs_enabled_disabled), 0,
NULL, HFILL }},
{ &hf_tcpip_lcd_acd_activity,
{ "ACD Activity", "cip.tcpip.last_conflict.acd_activity",
FT_UINT8, BASE_DEC, VALS(enip_tcpip_acd_activity_vals), 0,
NULL, HFILL }},
{ &hf_tcpip_lcd_remote_mac,
{ "RemoteMAC", "cip.tcpip.last_conflict.remote_mac",
FT_ETHER, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_tcpip_lcd_arp_pdu,
{ "Arp PDU", "cip.tcpip.last_conflict.arp_pdu",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_tcpip_quick_connect,
{ "Ethernet/IP Quick Connection", "cip.tcpip.quick_connect",
FT_BOOLEAN, 8, TFS(&tfs_enabled_disabled), 0x1,
NULL, HFILL }},
{ &hf_tcpip_encap_inactivity,
{ "Encapsulation Inactivity Timeout", "cip.tcpip.encap_inactivity",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }},
{ &hf_elink_interface_speed,
{ "Interface Speed", "cip.elink.interface_speed",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_interface_flags,
{ "Interface Flags", "cip.elink.iflags",
FT_UINT32, BASE_HEX, NULL, 0,
NULL, HFILL }},
{ &hf_elink_iflags_link_status,
{ "Link Status", "cip.elink.iflags.link_status",
FT_BOOLEAN, 32, TFS(&tfs_active_inactive), 0x00000001,
NULL, HFILL }},
{ &hf_elink_iflags_duplex,
{ "Duplex", "cip.elink.iflags.duplex",
FT_UINT32, BASE_DEC, VALS(enip_elink_duplex_vals), 0x00000002,
NULL, HFILL }},
{ &hf_elink_iflags_neg_status,
{ "Negotiation Status", "cip.elink.iflags.neg_status",
FT_UINT32, BASE_DEC, VALS(enip_elink_iflags_neg_status_vals), 0x0000001C,
NULL, HFILL }},
{ &hf_elink_iflags_manual_reset,
{ "Manual Reset Required", "cip.elink.iflags.manual_reset",
FT_UINT32, BASE_DEC, VALS(enip_elink_iflags_reset_vals), 0x00000020,
NULL, HFILL }},
{ &hf_elink_iflags_local_hw_fault,
{ "Local Hardware Fault", "cip.elink.iflags.local_hw_fault",
FT_UINT32, BASE_DEC, VALS(enip_elink_iflags_hw_fault_vals), 0x00000040,
NULL, HFILL }},
{ &hf_elink_iflags_reserved,
{ "Reserved", "cip.elink.iflags.reserved",
FT_UINT32, BASE_HEX, NULL, 0xFFFFFF80,
NULL, HFILL }},
{ &hf_elink_physical_address,
{ "Physical Address", "cip.elink.physical_address",
FT_ETHER, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_elink_icount_in_octets,
{ "In Octets", "cip.elink.icount.in_octets",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_icount_in_ucast,
{ "In Ucast Packets", "cip.elink.icount.in_ucast",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_icount_in_nucast,
{ "In NUcast Packets", "cip.elink.icount.in_nucast",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_icount_in_discards,
{ "In Discards", "cip.elink.icount.in_discards",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_icount_in_errors,
{ "In Errors", "cip.elink.icount.in_errors",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_icount_in_unknown_protos,
{ "In Unknown Protos", "cip.elink.icount.in_unknown_protos",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_icount_out_octets,
{ "Out Octets", "cip.elink.icount.out_octets",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_icount_out_ucast,
{ "Out Ucast Packets", "cip.elink.icount.out_ucast",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_icount_out_nucast,
{ "Out NUcast Packets", "cip.elink.icount.out_nucast",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_icount_out_discards,
{ "Out Discards", "cip.elink.icount.out_discards",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_icount_out_errors,
{ "Out Errors", "cip.elink.icount.out_errors",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_mcount_alignment_errors,
{ "Alignment Errors", "cip.elink.mcount.alignment_errors",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_mcount_fcs_errors,
{ "FCS Errors", "cip.elink.mcount.fcs_errors",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_mcount_single_collisions,
{ "Single Collisions", "cip.elink.mcount.single_collisions",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_mcount_multiple_collisions,
{ "Multiple Collisions", "cip.elink.mcount.multiple_collisions",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_mcount_sqe_test_errors,
{ "SQE Test Errors", "cip.elink.mcount.sqe_test_errors",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_mcount_deferred_transmission,
{ "Deferred Transmission", "cip.elink.mcount.deferred_transmission",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_mcount_late_collisions,
{ "Late Collisions", "cip.elink.mcount.late_collisions",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_mcount_excessive_collisions,
{ "Excessive Collisions", "cip.elink.mcount.excessive_collisions",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_mcount_mac_transmit_errors,
{ "MAC Transmit Errors", "cip.elink.mcount.mac_transmit_errors",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_mcount_carrier_sense_errors,
{ "Carrier Sense Errors", "cip.elink.mcount.carrier_sense_errors",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_mcount_frame_too_long,
{ "Frame Too Long", "cip.elink.mcount.frame_too_long",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_mcount_mac_receive_errors,
{ "MAC Receive Errors", "cip.elink.mcount.mac_receive_errors",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_icontrol_control_bits,
{ "Control Bits", "cip.elink.icontrol.control_bits",
FT_UINT16, BASE_HEX, NULL, 0,
NULL, HFILL }},
{ &hf_elink_icontrol_control_bits_auto_neg,
{ "Auto-negotiate", "cip.elink.icontrol.control_bits.auto_neg",
FT_BOOLEAN, 16, TFS(&tfs_enabled_disabled), 0x0001,
NULL, HFILL }},
{ &hf_elink_icontrol_control_bits_forced_duplex,
{ "Forced Duplex Mode", "cip.elink.icontrol.control_bits.forced_duplex",
FT_UINT16, BASE_DEC, VALS(enip_elink_duplex_vals), 0x0002,
NULL, HFILL }},
{ &hf_elink_icontrol_control_bits_reserved,
{ "Reserved", "cip.elink.icontrol.control_bits.reserved",
FT_UINT16, BASE_HEX, NULL, 0xFFFC,
NULL, HFILL }},
{ &hf_elink_icontrol_forced_speed,
{ "Forced Interface Speed", "cip.elink.icontrol.forced_speed",
FT_UINT16, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_elink_interface_type,
{ "Interface Type", "cip.elink.interface_type",
FT_UINT8, BASE_DEC, VALS(enip_elink_interface_type_vals), 0,
NULL, HFILL }},
{ &hf_elink_interface_state,
{ "Interface State", "cip.elink.interface_state",
FT_UINT8, BASE_DEC, VALS(enip_elink_interface_state_vals), 0,
NULL, HFILL }},
{ &hf_elink_admin_state,
{ "Admin State", "cip.elink.admin_state",
FT_UINT8, BASE_DEC, VALS(enip_elink_admin_state_vals), 0,
NULL, HFILL }},
{ &hf_elink_interface_label,
{ "Interface Label", "cip.elink.interface_label",
FT_STRING, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_qos_8021q_enable,
{ "802.1Q Tag Enable", "cip.qos.8021q_enable",
FT_BOOLEAN, 8, TFS(&tfs_enabled_disabled), 0x1,
NULL, HFILL }},
{ &hf_qos_dscp_ptp_event,
{ "DSCP PTP Event", "cip.qos.ptp_event",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_qos_dscp_ptp_general,
{ "DSCP PTP General", "cip.qos.ptp_general",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_qos_dscp_urgent,
{ "DSCP Urgent", "cip.qos.urgent",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_qos_dscp_scheduled,
{ "DSCP Scheduled", "cip.qos.scheduled",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_qos_dscp_high,
{ "DSCP High", "cip.qos.high",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_qos_dscp_low,
{ "DSCP Low", "cip.qos.low",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_qos_dscp_explicit,
{ "DSCP Explicit", "cip.qos.explicit",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_network_topology,
{ "Network Topology", "cip.dlr.network_topology",
FT_UINT8, BASE_DEC, VALS(enip_dlr_network_topology_vals), 0,
NULL, HFILL }},
{ &hf_dlr_network_status,
{ "Network Status", "cip.dlr.network_status",
FT_UINT8, BASE_DEC, VALS(enip_dlr_network_status_vals), 0,
NULL, HFILL }},
{ &hf_dlr_ring_supervisor_status,
{ "Ring Supervisor Status", "cip.dlr.ring_supervisor_status",
FT_UINT8, BASE_DEC, VALS(enip_dlr_ring_supervisor_status_vals), 0,
NULL, HFILL }},
{ &hf_dlr_rsc_ring_supervisor_enable,
{ "Ring Supervisor Enable", "cip.dlr.rscconfig.supervisor_enable",
FT_BOOLEAN, 8, TFS(&tfs_true_false), 0,
NULL, HFILL }},
{ &hf_dlr_rsc_ring_supervisor_precedence,
{ "Ring Supervisor Precedence", "cip.dlr.rscconfig.supervisor_precedence",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_rsc_beacon_interval,
{ "Beacon Interval", "cip.dlr.rscconfig.beacon_interval",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_rsc_beacon_timeout,
{ "Beacon Timeout", "cip.dlr.rscconfig.beacon_timeout",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_rsc_dlr_vlan_id,
{ "DLR VLAN ID", "cip.dlr.rscconfig.dlr_vlan_id",
FT_UINT16, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_ring_faults_count,
{ "Ring Faults Count", "cip.dlr.ring_faults_count",
FT_UINT16, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_lanp1_dev_ip_addr,
{ "Device IP Address", "cip.dlr.lanp1.ip_addr",
FT_IPv4, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_lanp1_dev_physical_address,
{ "Device Physical Address", "cip.dlr.lanp1.physical_address",
FT_ETHER, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_lanp2_dev_ip_addr,
{ "Device IP Address", "cip.dlr.lanp2.ip_addr",
FT_IPv4, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_lanp2_dev_physical_address,
{ "Device Physical Address", "cip.dlr.lanp2.physical_address",
FT_ETHER, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_ring_protocol_participants_count,
{ "Participants Count", "cip.dlr.participants_count",
FT_UINT16, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_rppl_dev_ip_addr,
{ "Device IP Address", "cip.dlr.rppl.ip_addr",
FT_IPv4, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_rppl_dev_physical_address,
{ "Device Physical Address", "cip.dlr.rppl.physical_address",
FT_ETHER, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_asa_supervisor_ip_addr,
{ "Supervisor IP Address", "cip.dlr.asa.ip_addr",
FT_IPv4, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_asa_supervisor_physical_address,
{ "Supervisor Physical Address", "cip.dlr.asa.physical_address",
FT_ETHER, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_active_supervisor_precedence,
{ "Active Supervisor Precedence", "cip.dlr.supervisor_precedence",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_capability_flags,
{ "Capability Flags", "cip.dlr.capflags",
FT_UINT32, BASE_HEX, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_capflags_announce_base_node,
{ "Announce-based Ring Node", "cip.dlr.capflags.announce_based",
FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x00000001,
NULL, HFILL }},
{ &hf_dlr_capflags_beacon_base_node,
{ "Beacon-based Ring Node", "cip.dlr.capflags.beacon_based",
FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x00000002,
NULL, HFILL }},
{ &hf_dlr_capflags_reserved1,
{ "Reserved", "cip.dlr.capflags.reserved1",
FT_BOOLEAN, 32, NULL, 0x0000001C,
NULL, HFILL }},
{ &hf_dlr_capflags_supervisor_capable,
{ "Supervisor Capable", "cip.dlr.capflags.supervisor_capable",
FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x00000020,
NULL, HFILL }},
{ &hf_dlr_capflags_redundant_gateway_capable,
{ "Redundant Gatway Capable", "cip.dlr.capflags.redundant_gateway_capable",
FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x00000040,
NULL, HFILL }},
{ &hf_dlr_capflags_flush_frame_capable,
{ "Flush_Table Frame Capable", "cip.dlr.capflags.flush_frame_capable",
FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x00000080,
NULL, HFILL }},
{ &hf_dlr_capflags_reserved2,
{ "Reserved", "cip.dlr.capflags.reserved2",
FT_BOOLEAN, 32, NULL, 0xFFFFFF00,
NULL, HFILL }},
{ &hf_dlr_rgc_red_gateway_enable,
{ "Redundant Gateway Enable", "cip.dlr.rgc.gateway_enable",
FT_BOOLEAN, 8, TFS(&tfs_true_false), 0,
NULL, HFILL }},
{ &hf_dlr_rgc_gateway_precedence,
{ "Gateway Precedence", "cip.dlr.rgc.gateway_precedence",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_rgc_advertise_interval,
{ "Advertise Interval", "cip.dlr.rgc.advertise_interval",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_rgc_advertise_timeout,
{ "Advertise Timeout", "cip.dlr.rgc.advertise_timeout",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_rgc_learning_update_enable,
{ "Learning Update Enable", "cip.dlr.rgc.learning_update_enable",
FT_BOOLEAN, 8, TFS(&tfs_true_false), 0,
NULL, HFILL }},
{ &hf_dlr_redundant_gateway_status,
{ "Redundant Gateway Status", "cip.dlr.redundant_gateway_status",
FT_UINT8, BASE_DEC, VALS(enip_dlr_redundant_gateway_status_vals), 0,
NULL, HFILL }},
{ &hf_dlr_aga_ip_addr,
{ "Active Gateway IP Address", "cip.dlr.aga.ip_addr",
FT_IPv4, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_aga_physical_address,
{ "Active Gateway Physical Address", "cip.dlr.aga.physical_address",
FT_ETHER, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_dlr_active_gateway_precedence,
{ "Active Gateway Precedence", "cip.dlr.active_gateway_precedence",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_eip_security_state,
{ "State", "cip.eip_security.state",
FT_UINT8, BASE_DEC, VALS(eip_security_state_vals), 0,
NULL, HFILL }},
{ &hf_eip_security_verify_client_cert,
{ "Verify Client Certificate", "cip.eip_security.verify_client_cert",
FT_BOOLEAN, 8, NULL, 0,
NULL, HFILL }},
{ &hf_eip_security_send_cert_chain,
{ "Send Certificate Chain", "cip.eip_security.send_cert_chain",
FT_BOOLEAN, 8, NULL, 0,
NULL, HFILL }},
{ &hf_eip_security_check_expiration,
{ "Check Expiration", "cip.eip_security.check_expiration",
FT_BOOLEAN, 8, NULL, 0,
NULL, HFILL }},
{ &hf_eip_security_capability_flags,
{ "Capability Flags", "cip.eip_security.capability_flags",
FT_UINT32, BASE_HEX, NULL, 0,
NULL, HFILL }},
{ &hf_eip_security_capflags_secure_renegotiation,
{ "Secure Renegotiation", "cip.eip_security.capability_flags.secure_renegotiation",
FT_BOOLEAN, 32, TFS(&tfs_supported_not_supported), 0x00000001,
NULL, HFILL }},
{ &hf_eip_security_capflags_reserved,
{ "Reserved", "cip.eip_security.capability_flags.reserved",
FT_UINT32, BASE_HEX, NULL, 0xFFFFFFFE,
NULL, HFILL }},
{ &hf_eip_security_num_avail_cipher_suites,
{ "Number of Available Cipher Suites", "cip.eip_security.num_avail_cipher_suites",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_eip_security_avail_cipher_suite,
{ "Available Cipher Suite", "cip.eip_security.avail_cipher_suite",
FT_UINT16, BASE_HEX|BASE_EXT_STRING, &ssl_31_ciphersuite_ext, 0,
NULL, HFILL }},
{ &hf_eip_security_num_allow_cipher_suites,
{ "Number of Allowed Cipher Suites", "cip.eip_security.num_allow_cipher_suites",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_eip_security_allow_cipher_suite,
{ "Allowed Cipher Suite", "cip.eip_security.allow_cipher_suite",
FT_UINT16, BASE_HEX|BASE_EXT_STRING, &ssl_31_ciphersuite_ext, 0,
NULL, HFILL }},
{ &hf_eip_security_num_psk,
{ "Number of PSKs", "cip.eip_security.num_psk",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_eip_security_psk_identity_size,
{ "PSK Identity Size", "cip.eip_security.psk_identity_size",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_eip_security_psk_identity,
{ "PSK Identity", "cip.eip_security.psk_identity",
FT_BYTES, BASE_NONE|BASE_ALLOW_ZERO, NULL, 0,
NULL, HFILL }},
{ &hf_eip_security_psk_size,
{ "PSK Size", "cip.eip_security.psk_size",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_eip_security_psk,
{ "PSK", "cip.eip_security.psk",
FT_BYTES, BASE_NONE|BASE_ALLOW_ZERO, NULL, 0,
NULL, HFILL }},
{ &hf_eip_security_num_active_certs,
{ "Number of Active Certificates", "cip.eip_security.num_active_certs",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_eip_security_num_trusted_auths,
{ "Number of Trusted Authorities", "cip.eip_security.num_trusted_auths",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_eip_cert_name,
{ "Name", "cip.eip_cert.name",
FT_STRING, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_eip_cert_state,
{ "State", "cip.eip_cert.state",
FT_UINT8, BASE_DEC, VALS(eip_cert_state_vals), 0,
NULL, HFILL }},
{ &hf_eip_cert_device_cert_status,
{ "Certificate Status", "cip.eip_cert.device_cert.status",
FT_UINT8, BASE_DEC, VALS(eip_cert_status_vals), 0,
NULL, HFILL }},
{ &hf_eip_cert_ca_cert_status,
{ "Certificate Status", "cip.eip_cert.ca_cert.status",
FT_UINT8, BASE_DEC, VALS(eip_cert_status_vals), 0,
NULL, HFILL }},
{ &hf_eip_cert_capflags_push,
{ "Path size", "cip.eip_cert.capflags.push",
FT_BOOLEAN, 32, NULL, 0x00000001,
NULL, HFILL }},
{ &hf_eip_cert_capflags_reserved,
{ "Path size", "cip.eip_cert.capflags.reserved",
FT_BOOLEAN, 32, NULL, 0xFFFFFFFE,
NULL, HFILL }},
{ &hf_eip_cert_capability_flags,
{ "Capability flags", "cip.eip_cert.capflags",
FT_UINT32, BASE_HEX, NULL, 0,
NULL, HFILL }},
{ &hf_eip_cert_num_certs,
{ "Number of Certificates", "cip.eip_cert.num_certs",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_eip_cert_cert_name,
{ "Certificate name", "cip.eip_cert.cert_name",
FT_STRING, BASE_NONE, NULL, 0,
NULL, HFILL }},
};
static gint *ett[] = {
&ett_enip,
&ett_path,
&ett_count_tree,
&ett_type_tree,
&ett_command_tree,
&ett_sockadd,
&ett_32bitheader_tree,
&ett_lsrcf,
&ett_tcpip_status,
&ett_tcpip_config_cap,
&ett_tcpip_config_control,
&ett_elink_interface_flags,
&ett_elink_icontrol_bits,
&ett_dlr_capability_flags,
&ett_dlr_lnknbrstatus_flags,
&ett_eip_security_capability_flags,
&ett_eip_security_psk,
&ett_eip_security_active_certs,
&ett_eip_security_trusted_auths,
&ett_eip_cert_capability_flags,
&ett_eip_cert_num_certs,
};
static ei_register_info ei[] = {
{ &ei_mal_tcpip_status, { "cip.malformed.tcpip.status", PI_MALFORMED, PI_ERROR, "Malformed TCP/IP Status", EXPFILL }},
{ &ei_mal_tcpip_config_cap, { "cip.malformed.tcpip.config_cap", PI_MALFORMED, PI_ERROR, "Malformed TCP/IP Configuration Capability", EXPFILL }},
{ &ei_mal_tcpip_config_control, { "cip.malformed.tcpip.config_control", PI_MALFORMED, PI_ERROR, "Malformed TCP/IP Configuration Control", EXPFILL }},
{ &ei_mal_tcpip_interface_config, { "cip.malformed.tcpip.interface_config", PI_MALFORMED, PI_ERROR, "Malformed TCP/IP Interface Configuration", EXPFILL }},
{ &ei_mal_tcpip_ssn, { "cip.malformed.tcpip.ssn", PI_MALFORMED, PI_ERROR, "Malformed TCP/IP Object Safety Network Number", EXPFILL }},
{ &ei_mal_tcpip_mcast_config, { "cip.malformed.tcpip.mcast_config", PI_MALFORMED, PI_ERROR, "Malformed TCP/IP Multicast Config", EXPFILL }},
{ &ei_mal_tcpip_last_conflict, { "cip.malformed.tcpip.last_conflict", PI_MALFORMED, PI_ERROR, "Malformed TCP/IP Last Conflict Detected", EXPFILL }},
{ &ei_mal_elink_interface_flags, { "cip.malformed.elink.interface_flags", PI_MALFORMED, PI_ERROR, "Malformed Ethernet Link Interface Flags", EXPFILL }},
{ &ei_mal_elink_physical_address, { "cip.malformed.elink.physical_address", PI_MALFORMED, PI_ERROR, "Malformed Ethernet Link Physical Address", EXPFILL } },
{ &ei_mal_elink_interface_counters, { "cip.malformed.elink.interface_counters", PI_MALFORMED, PI_ERROR, "Malformed Ethernet Link Interface Counters", EXPFILL }},
{ &ei_mal_elink_media_counters, { "cip.malformed.elink.media_counters", PI_MALFORMED, PI_ERROR, "Malformed Ethernet Link Media Counters", EXPFILL }},
{ &ei_mal_elink_interface_control, { "cip.malformed.elink.interface_control", PI_MALFORMED, PI_ERROR, "Malformed Ethernet Link Interface Control", EXPFILL }},
{ &ei_mal_dlr_ring_supervisor_config, { "cip.malformed.dlr.ring_supervisor_config", PI_MALFORMED, PI_ERROR, "Malformed DLR Ring Supervisor Config", EXPFILL }},
{ &ei_mal_dlr_last_active_node_on_port_1, { "cip.malformed.dlr.last_active_node_on_port_1", PI_MALFORMED, PI_ERROR, "Malformed DLR Last Active Node on Port 1", EXPFILL }},
{ &ei_mal_dlr_last_active_node_on_port_2, { "cip.malformed.dlr.last_active_node_on_port_2", PI_MALFORMED, PI_ERROR, "Malformed DLR Last Active Node on Port 2", EXPFILL }},
{ &ei_mal_dlr_ring_protocol_participants_list, { "cip.malformed.dlr.ring_protocol_participants_list", PI_MALFORMED, PI_ERROR, "Malformed DLR Ring Protocol Participants List", EXPFILL }},
{ &ei_mal_dlr_active_supervisor_address, { "cip.malformed.dlr.active_supervisor_address", PI_MALFORMED, PI_ERROR, "Malformed DLR Active Supervisor Address", EXPFILL }},
{ &ei_mal_dlr_capability_flags, { "cip.malformed.dlr.capability_flags", PI_MALFORMED, PI_ERROR, "Malformed DLR Capability Flag", EXPFILL }},
{ &ei_mal_dlr_redundant_gateway_config, { "cip.malformed.dlr.redundant_gateway_config", PI_MALFORMED, PI_ERROR, "Malformed DLR Redundant Gateway Config", EXPFILL }},
{ &ei_mal_dlr_active_gateway_address, { "cip.malformed.dlr.active_gateway_address", PI_MALFORMED, PI_ERROR, "Malformed DLR Active Gateway Address", EXPFILL }},
{ &ei_mal_eip_security_capability_flags, { "cip.malformed.eip_security.capability_flags", PI_MALFORMED, PI_ERROR, "Malformed EIP Security Capability Flags", EXPFILL }},
{ &ei_mal_eip_security_avail_cipher_suites, { "cip.malformed.eip_security.avail_cipher_suites", PI_MALFORMED, PI_ERROR, "Malformed EIP Security Available Cipher Suites", EXPFILL }},
{ &ei_mal_eip_security_allow_cipher_suites, { "cip.malformed.eip_security.allow_cipher_suites", PI_MALFORMED, PI_ERROR, "Malformed EIP Security Allowed Cipher Suites", EXPFILL }},
{ &ei_mal_eip_security_preshared_keys, { "cip.malformed.eip_security.preshared_keys", PI_MALFORMED, PI_ERROR, "Malformed EIP Security Pre-Shared Keys", EXPFILL }},
{ &ei_mal_eip_security_active_certs, { "cip.malformed.eip_security.active_certs", PI_MALFORMED, PI_ERROR, "Malformed EIP Security Active Device Certificates", EXPFILL }},
{ &ei_mal_eip_security_trusted_auths, { "cip.malformed.eip_security.trusted_auths", PI_MALFORMED, PI_ERROR, "Malformed EIP Security Trusted Authorities", EXPFILL }},
{ &ei_mal_eip_cert_capability_flags, { "cip.malformed.eip_cert.capability_flags", PI_MALFORMED, PI_ERROR, "Malformed EIP Certificate Management Capability Flags", EXPFILL }},
};
static hf_register_info hfdlr[] = {
{ &hf_dlr_ringsubtype,
{ "Ring Sub-Type", "enip.dlr.ringsubtype",
FT_UINT8, BASE_HEX, NULL, 0,
NULL, HFILL }
},
{ &hf_dlr_ringprotoversion,
{ "Ring Protocol Version", "enip.dlr.protversion",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }
},
{ &hf_dlr_frametype,
{ "Frame Type", "enip.dlr.frametype",
FT_UINT8, BASE_HEX, VALS(dlr_frame_type_vals), 0,
NULL, HFILL }
},
{ &hf_dlr_sourceport,
{ "Source Port", "enip.dlr.sourceport",
FT_UINT8, BASE_HEX, VALS(dlr_source_port_vals), 0,
NULL, HFILL }
},
{ &hf_dlr_sourceip,
{ "Source IP", "enip.dlr.sourceip",
FT_IPv4, BASE_NONE, NULL, 0,
"Source IP Address", HFILL }
},
{ &hf_dlr_sequenceid,
{ "Sequence Id", "enip.dlr.seqid",
FT_UINT32, BASE_HEX, NULL, 0,
NULL, HFILL }
},
{ &hf_dlr_ringstate,
{ "Ring State", "enip.dlr.state",
FT_UINT8, BASE_HEX, VALS(dlr_ring_state_vals), 0,
NULL, HFILL }
},
{ &hf_dlr_supervisorprecedence,
{ "Supervisor Precedence", "enip.dlr.supervisorprecedence",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }
},
{ &hf_dlr_beaconinterval,
{ "Beacon Interval", "enip.dlr.beaconinterval",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }
},
{ &hf_dlr_beacontimeout,
{ "Beacon Timeout", "enip.dlr.beacontimeout",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }
},
{ &hf_dlr_beaconreserved,
{ "Reserved", "enip.dlr.beaconreserved",
FT_BYTES, BASE_NONE, NULL, 0,
"Beacon Reserved", HFILL }
},
{ &hf_dlr_nreqreserved,
{ "Reserved", "enip.dlr.nreqreserved",
FT_BYTES, BASE_NONE, NULL, 0,
"Neighbor_Check_Request Reserved", HFILL }
},
{ &hf_dlr_nressourceport,
{ "Request Source Port", "enip.dlr.nressourceport",
FT_UINT8, BASE_HEX, VALS(dlr_source_port_vals), 0,
"Neighbor_Check_Response Source Port", HFILL }
},
{ &hf_dlr_nresreserved,
{ "Reserved", "enip.dlr.nresreserved",
FT_BYTES, BASE_NONE, NULL, 0,
"Neighbor_Check_Response Reserved", HFILL }
},
{ &hf_dlr_lnknbrstatus,
{ "Link/Neighbor Status", "enip.dlr.lnknbrstatus.status",
FT_UINT8, BASE_HEX, NULL, 0,
"Link_Status/Neighbor_Status Status", HFILL }
},
{ &hf_dlr_lnknbrstatus_port1,
{ "Port 1 Active", "enip.dlr.lnknbrstatus.port1",
FT_BOOLEAN, 8, TFS(&tfs_true_false), 0x01,
NULL, HFILL }
},
{ &hf_dlr_lnknbrstatus_port2,
{ "Port 2 Active", "enip.dlr.lnknbrstatus.port2",
FT_BOOLEAN, 8, TFS(&tfs_true_false), 0x02,
NULL, HFILL }
},
{ &hf_dlr_lnknbrstatus_reserved,
{ "Reserved", "enip.dlr.lnknbrstatus.reserved",
FT_BOOLEAN, 8, NULL, 0x7C,
NULL, HFILL }
},
{ &hf_dlr_lnknbrstatus_frame_type,
{ "Link/Neighbor Status Frame Type", "enip.dlr.lnknbrstatus.frame_type",
FT_BOOLEAN, 8, TFS(&dlr_lnknbrstatus_frame_type_vals), 0x80,
NULL, HFILL }
},
{ &hf_dlr_lnknbrreserved,
{ "Reserved", "enip.dlr.lnknbrreserved",
FT_BYTES, BASE_NONE, NULL, 0,
"Link_Status/Neighbor_Status Reserved", HFILL }
},
{ &hf_dlr_lfreserved,
{ "Reserved", "enip.dlr.lfreserved",
FT_BYTES, BASE_NONE, NULL, 0,
"Locate_Fault Reserved", HFILL }
},
{ &hf_dlr_anreserved,
{ "Reserved", "enip.dlr.anreserved",
FT_BYTES, BASE_NONE, NULL, 0,
"Announce Reserved", HFILL }
},
{ &hf_dlr_sonumnodes,
{ "Num nodes", "enip.dlr.sonumnodes",
FT_UINT16, BASE_DEC, NULL, 0,
"Number of Nodes in List", HFILL }
},
{ &hf_dlr_somac,
{ "MAC Address", "enip.dlr.somac",
FT_ETHER, BASE_NONE, NULL, 0,
"Sign_On Node MAC Address", HFILL }
},
{ &hf_dlr_soip,
{ "IP Address", "enip.dlr.soip",
FT_IPv4, BASE_NONE, NULL, 0,
"Sign_On Node IP Address", HFILL }
},
{ &hf_dlr_soreserved,
{ "Reserved", "enip.dlr.soreserved",
FT_BYTES, BASE_NONE, NULL, 0,
"Sign_On Reserved", HFILL }
},
{ &hf_dlr_advgatewaystate,
{ "Gateway Status", "enip.dlr.advgatewaystate",
FT_UINT8, BASE_HEX, VALS(dlr_adv_state_vals), 0,
"Gateway State", HFILL }
},
{ &hf_dlr_advgatewayprecedence,
{ "Gateway Precedence", "enip.dlr.advgatewayprecedence",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }
},
{ &hf_dlr_advadvertiseinterval,
{ "Advertise Interval", "enip.dlr.advadvertiseinterval",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }
},
{ &hf_dlr_advadvertisetimeout,
{ "Advertise Interval", "enip.dlr.advadvertisetimeout",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }
},
{ &hf_dlr_advlearningupdateenable,
{ "Learning Update Enable", "enip.dlr.advlearningupdateenable",
FT_UINT8, BASE_HEX, VALS(dlr_adv_learning_update_vals), 0,
"Advertise Learning Update Enable", HFILL }
},
{ &hf_dlr_advreserved,
{ "Reserved", "enip.dlr.advreserved",
FT_BYTES, BASE_NONE, NULL, 0,
"Advertise Reserved", HFILL }
},
{ &hf_dlr_flushlearningupdateenable,
{ "Learning Update Enable", "enip.dlr.flushlearningupdateenable",
FT_UINT8, BASE_HEX, VALS(dlr_flush_learning_update_vals), 0,
"Flush_Tables Learning Update Enable", HFILL }
},
{ &hf_dlr_flushreserved,
{ "Reserved", "enip.dlr.flushreserved",
FT_BYTES, BASE_NONE, NULL, 0,
"Flush_Tables Reserved", HFILL }
},
{ &hf_dlr_learnreserved,
{ "Reserved", "enip.dlr.learnreserved",
FT_BYTES, BASE_NONE, NULL, 0,
"Learning_Update Reserved", HFILL }
}
};
static gint *ettdlr[] = {
&ett_dlr
};
module_t *enip_module;
expert_module_t* expert_enip;
static build_valid_func enip_da_build_value[1] = {enip_value};
static decode_as_value_t enip_da_values = {enip_prompt, 1, enip_da_build_value};
static decode_as_t enip_da = {"enip", "ENIP I/O", "enip.io", 1, 0, &enip_da_values, NULL, NULL,
decode_as_default_populate_list, decode_as_default_reset, decode_as_default_change, NULL};
proto_enip = proto_register_protocol("EtherNet/IP (Industrial Protocol)", "ENIP", "enip");
proto_enipio = proto_register_protocol("EtherNet/IP I/O", "ENIP I/O", "enip_io");
register_dissector("enip", dissect_enip_tcp, proto_enip);
register_dissector("enip_io", dissect_enipio, proto_enipio);
proto_register_field_array(proto_enip, hf, array_length(hf));
proto_register_subtree_array(ett, array_length(ett));
expert_enip = expert_register_protocol(proto_enip);
expert_register_field_array(expert_enip, ei, array_length(ei));
enip_module = prefs_register_protocol(proto_enip, NULL);
prefs_register_bool_preference(enip_module, "desegment",
"Desegment all EtherNet/IP messages spanning multiple TCP segments",
"Whether the EtherNet/IP dissector should desegment all messages spanning multiple TCP segments",
&enip_desegment);
prefs_register_bool_preference(enip_module, "o2t_run_idle",
"Dissect 32-bit header in the O->T direction",
"Determines whether all I/O connections will assume a 32-bit header in the O->T direction",
&enip_OTrun_idle);
prefs_register_bool_preference(enip_module, "t2o_run_idle",
"Dissect 32-bit header in the T->O direction",
"Determines whether all I/O connections will assume a 32-bit header in the T->O direction",
&enip_TOrun_idle);
prefs_register_obsolete_preference(enip_module, "default_io_dissector");
subdissector_sud_table = register_dissector_table("enip.sud.iface",
"ENIP SendUnitData.Interface Handle", proto_enip, FT_UINT32, BASE_HEX, DISSECTOR_TABLE_NOT_ALLOW_DUPLICATE);
subdissector_srrd_table = register_dissector_table("enip.srrd.iface",
"ENIP SendRequestReplyData.Interface Handle", proto_enip, FT_UINT32, BASE_HEX, DISSECTOR_TABLE_NOT_ALLOW_DUPLICATE);
subdissector_io_table = register_dissector_table("enip.io", "ENIP IO dissector", proto_enip, FT_UINT32, BASE_DEC, DISSECTOR_TABLE_NOT_ALLOW_DUPLICATE);
register_init_routine(&enip_init_protocol);
register_cleanup_routine(&enip_cleanup_protocol);
proto_dlr = proto_register_protocol("Device Level Ring", "DLR", "dlr");
proto_register_field_array(proto_dlr, hfdlr, array_length(hfdlr));
proto_register_subtree_array(ettdlr, array_length(ettdlr));
register_conversation_filter("enip", "ENIP IO", enip_io_conv_valid, enip_io_conv_filter);
register_conversation_filter("enip", "ENIP Explicit", enip_exp_conv_valid, enip_exp_conv_filter);
register_decode_as(&enip_da);
}
void
proto_reg_handoff_enip(void)
{
dissector_handle_t enip_udp_handle, enip_tcp_handle;
dissector_handle_t enipio_handle;
dissector_handle_t dlr_handle;
enip_tcp_handle = find_dissector("enip");
dissector_add_uint("tcp.port", ENIP_ENCAP_PORT, enip_tcp_handle);
enip_udp_handle = create_dissector_handle(dissect_enip_udp, proto_enip);
dissector_add_uint("udp.port", ENIP_ENCAP_PORT, enip_udp_handle);
enipio_handle = find_dissector("enip_io");
dissector_add_uint("udp.port", ENIP_IO_PORT, enipio_handle);
ssl_dissector_add(ENIP_SECURE_PORT, enip_tcp_handle);
dtls_dissector_add(ENIP_SECURE_PORT, enipio_handle);
arp_handle = find_dissector_add_dependency("arp", proto_enip);
cipsafety_handle = find_dissector("cipsafety");
cipmotion_handle = find_dissector("cipmotion");
cip_implicit_handle = find_dissector_add_dependency("cip_implicit", proto_enipio);
dlr_handle = create_dissector_handle(dissect_dlr, proto_dlr);
dissector_add_uint("ethertype", ETHERTYPE_DLR, dlr_handle);
proto_cipsafety = proto_get_id_by_filter_name( "cipsafety" );
subdissector_class_table = find_dissector_table("cip.class.iface");
}
