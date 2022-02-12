static void dbg(guint level, char *fmt, ...) {
va_list ap;
if (level > debug_level) return;
va_start(ap,fmt);
vfprintf(stderr, fmt, ap);
va_end(ap);
}
static gint
camelsrt_call_equal(gconstpointer k1, gconstpointer k2)
{
const struct camelsrt_call_info_key_t *key1 = (const struct camelsrt_call_info_key_t *) k1;
const struct camelsrt_call_info_key_t *key2 = (const struct camelsrt_call_info_key_t *) k2;
return (key1->SessionIdKey == key2->SessionIdKey) ;
}
static guint
camelsrt_call_hash(gconstpointer k)
{
const struct camelsrt_call_info_key_t *key = (const struct camelsrt_call_info_key_t *) k;
return key->SessionIdKey;
}
static struct camelsrt_call_t *
find_camelsrt_call(struct camelsrt_call_info_key_t *p_camelsrt_call_key)
{
struct camelsrt_call_t *p_camelsrt_call = NULL;
p_camelsrt_call = (struct camelsrt_call_t *)g_hash_table_lookup(srt_calls, p_camelsrt_call_key);
#ifdef DEBUG_CAMELSRT
if(p_camelsrt_call) {
dbg(10,"D%d ", p_camelsrt_call->session_id);
} else {
dbg(23,"Not in hash ");
}
#endif
return p_camelsrt_call;
}
static struct camelsrt_call_t *
new_camelsrt_call(struct camelsrt_call_info_key_t *p_camelsrt_call_key)
{
struct camelsrt_call_info_key_t *p_new_camelsrt_call_key;
struct camelsrt_call_t *p_new_camelsrt_call = NULL;
p_new_camelsrt_call_key = se_new(struct camelsrt_call_info_key_t);
p_new_camelsrt_call_key->SessionIdKey = p_camelsrt_call_key->SessionIdKey;
p_new_camelsrt_call = se_new(struct camelsrt_call_t);
raz_camelsrt_call(p_new_camelsrt_call);
p_new_camelsrt_call->session_id = camelsrt_global_SessionId++;
#ifdef DEBUG_CAMELSRT
dbg(10,"D%d ", p_new_camelsrt_call->session_id);
#endif
g_hash_table_insert(srt_calls, p_new_camelsrt_call_key, p_new_camelsrt_call);
return p_new_camelsrt_call;
}
static void
update_camelsrt_call(struct camelsrt_call_t *p_camelsrt_call, packet_info *pinfo,
guint msg_category)
{
p_camelsrt_call->category[msg_category].req_num = pinfo->fd->num;
p_camelsrt_call->category[msg_category].rsp_num = 0;
p_camelsrt_call->category[msg_category].responded = FALSE;
p_camelsrt_call->category[msg_category].req_time = pinfo->fd->abs_ts;
}
void
camelsrt_init_routine(void)
{
if (srt_calls != NULL) {
#ifdef DEBUG_CAMELSRT
dbg(16,"Destroy hash ");
#endif
g_hash_table_destroy(srt_calls);
}
srt_calls = g_hash_table_new(camelsrt_call_hash, camelsrt_call_equal);
#ifdef DEBUG_CAMELSRT
dbg(16,"Create hash ");
#endif
camelsrt_global_SessionId=1;
gcamel_DisplaySRT=gcamel_PersistentSRT || gcamel_HandleSRT&gcamel_StatSRT;
}
void
camelsrt_call_matching(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree,
struct camelsrt_info_t *p_camelsrt_info)
{
#ifdef DEBUG_CAMELSRT
dbg(10,"tcap_session #%d ", p_camelsrt_info->tcap_session_id);
#endif
switch (p_camelsrt_info->opcode) {
case 0:
camelsrt_begin_call_matching(pinfo, p_camelsrt_info);
camelsrt_request_call_matching(tvb, pinfo, tree, p_camelsrt_info,
CAMELSRT_VOICE_INITIALDP);
break;
case 60:
camelsrt_begin_call_matching(pinfo, p_camelsrt_info);
camelsrt_request_call_matching(tvb, pinfo, tree, p_camelsrt_info,
CAMELSRT_SMS_INITIALDP);
break;
case 78:
camelsrt_begin_call_matching(pinfo, p_camelsrt_info);
camelsrt_request_call_matching(tvb, pinfo, tree, p_camelsrt_info,
CAMELSRT_GPRS_INITIALDP);
break;
case 23:
break;
case 63:
break;
case 81:
break;
case 24:
camelsrt_request_call_matching(tvb, pinfo, tree, p_camelsrt_info,
CAMELSRT_VOICE_DISC );
break;
case 64:
camelsrt_close_call_matching(pinfo, p_camelsrt_info);
tcapsrt_close((struct tcaphash_context_t *)p_camelsrt_info->tcap_context, pinfo);
break;
case 80:
camelsrt_begin_call_matching(pinfo, p_camelsrt_info);
camelsrt_request_call_matching(tvb, pinfo, tree, p_camelsrt_info,
CAMELSRT_GPRS_REPORT);
break;
case 35:
camelsrt_report_call_matching(tvb, pinfo, tree, p_camelsrt_info,
CAMELSRT_VOICE_ACR1 );
break;
case 71:
break;
case 36:
camelsrt_request_call_matching(tvb, pinfo, tree, p_camelsrt_info,
CAMELSRT_VOICE_ACR1 );
break;
case 72:
break;
case 31:
camelsrt_report_call_matching(tvb, pinfo, tree, p_camelsrt_info,
CAMELSRT_VOICE_INITIALDP);
break;
case 65:
camelsrt_report_call_matching(tvb, pinfo, tree, p_camelsrt_info,
CAMELSRT_SMS_INITIALDP);
break;
case 75:
camelsrt_report_call_matching(tvb, pinfo, tree, p_camelsrt_info,
CAMELSRT_GPRS_INITIALDP);
camelsrt_report_call_matching(tvb, pinfo, tree, p_camelsrt_info,
CAMELSRT_GPRS_REPORT);
break;
case 22:
camelsrt_report_call_matching(tvb, pinfo, tree, p_camelsrt_info,
CAMELSRT_VOICE_DISC);
camelsrt_close_call_matching(pinfo, p_camelsrt_info);
break;
case 66:
camelsrt_close_call_matching(pinfo, p_camelsrt_info);
tcapsrt_close((struct tcaphash_context_t *)p_camelsrt_info->tcap_context,pinfo);
break;
case 79:
camelsrt_close_call_matching(pinfo, p_camelsrt_info);
break;
}
}
void
camelsrt_tcap_matching(tvbuff_t *tvb _U_, packet_info *pinfo,
proto_tree *tree _U_,
struct tcaphash_context_t *p_tcap_context)
{
struct camelsrt_info_t *p_camelsrt_info;
#ifdef DEBUG_CAMELSRT
dbg(11,"Camel_CallBack ");
#endif
p_camelsrt_info=camelsrt_razinfo();
p_camelsrt_info->tcap_context=p_tcap_context;
if (p_tcap_context) {
#ifdef DEBUG_CAMELSRT
dbg(11,"Close TCAP ");
#endif
p_camelsrt_info->tcap_session_id = p_tcap_context->session_id;
camelsrt_close_call_matching(pinfo, p_camelsrt_info);
tap_queue_packet(camel_tap, pinfo, p_camelsrt_info);
}
}
static void
camelsrt_begin_call_matching(packet_info *pinfo,
struct camelsrt_info_t *p_camelsrt_info)
{
struct camelsrt_call_t *p_camelsrt_call;
struct camelsrt_call_info_key_t camelsrt_call_key;
p_camelsrt_info->bool_msginfo[CAMELSRT_SESSION]=TRUE;
camelsrt_call_key.SessionIdKey = p_camelsrt_info->tcap_session_id;
#ifdef DEBUG_CAMELSRT
dbg(10,"\n Session begin #%u\n", pinfo->fd->num);
dbg(11,"Search key %lu ",camelsrt_call_key.SessionIdKey);
#endif
p_camelsrt_call = (struct camelsrt_call_t *)g_hash_table_lookup(srt_calls, &camelsrt_call_key);
if (p_camelsrt_call) {
#ifdef DEBUG_CAMELSRT
dbg(22,"Already seen ");
#endif
} else {
#ifdef DEBUG_CAMELSRT
dbg(10,"New key %lu ",camelsrt_call_key.SessionIdKey);
#endif
p_camelsrt_call = new_camelsrt_call(&camelsrt_call_key);
p_camelsrt_call->tcap_context=(struct tcaphash_context_t *)p_camelsrt_info->tcap_context;
update_camelsrt_call(p_camelsrt_call, pinfo,CAMELSRT_SESSION);
#ifdef DEBUG_CAMELSRT
dbg(11,"Update Callback ");
#endif
p_camelsrt_call->tcap_context->callback=camelsrt_tcap_matching;
}
}
static void
camelsrt_request_call_matching(tvbuff_t *tvb, packet_info *pinfo,
proto_tree *tree,
struct camelsrt_info_t *p_camelsrt_info,
guint srt_type )
{
struct camelsrt_call_t *p_camelsrt_call;
struct camelsrt_call_info_key_t camelsrt_call_key;
proto_item *ti, *hidden_item;
#ifdef DEBUG_CAMELSRT
dbg(10,"\n %s #%u\n", val_to_str_const(srt_type, camelSRTtype_naming, "Unk"),pinfo->fd->num);
#endif
camelsrt_call_key.SessionIdKey = p_camelsrt_info->tcap_session_id;
#ifdef DEBUG_CAMELSRT
dbg(11,"Search key %lu ", camelsrt_call_key.SessionIdKey);
#endif
p_camelsrt_call = find_camelsrt_call(&camelsrt_call_key);
if(p_camelsrt_call) {
#ifdef DEBUG_CAMELSRT
dbg(12,"Found ");
#endif
if (gcamel_DisplaySRT)
proto_tree_add_uint(tree, hf_camelsrt_SessionId, tvb, 0,0, p_camelsrt_call->session_id);
if (srt_type==CAMELSRT_VOICE_ACR1) {
if (p_camelsrt_call->category[CAMELSRT_VOICE_ACR1].req_num == 0) {
srt_type=CAMELSRT_VOICE_ACR1;
} else if ( (p_camelsrt_call->category[CAMELSRT_VOICE_ACR2].req_num == 0)
&& (p_camelsrt_call->category[CAMELSRT_VOICE_ACR1].rsp_num != 0)
&& (p_camelsrt_call->category[CAMELSRT_VOICE_ACR1].rsp_num < pinfo->fd->num) ) {
srt_type=CAMELSRT_VOICE_ACR2;
} else if ( (p_camelsrt_call->category[CAMELSRT_VOICE_ACR3].req_num == 0)
&& (p_camelsrt_call->category[CAMELSRT_VOICE_ACR2].rsp_num != 0)
&& (p_camelsrt_call->category[CAMELSRT_VOICE_ACR2].rsp_num < pinfo->fd->num) ) {
srt_type=CAMELSRT_VOICE_ACR3;
} else if (p_camelsrt_call->category[CAMELSRT_VOICE_ACR1].rsp_num != 0
&& p_camelsrt_call->category[CAMELSRT_VOICE_ACR1].rsp_num > pinfo->fd->num) {
srt_type=CAMELSRT_VOICE_ACR1;
} else if ( p_camelsrt_call->category[CAMELSRT_VOICE_ACR2].rsp_num != 0
&& p_camelsrt_call->category[CAMELSRT_VOICE_ACR2].rsp_num > pinfo->fd->num) {
srt_type=CAMELSRT_VOICE_ACR2;
} else if (p_camelsrt_call->category[CAMELSRT_VOICE_ACR1].rsp_num != 0
&& p_camelsrt_call->category[CAMELSRT_VOICE_ACR3].rsp_num > pinfo->fd->num) {
srt_type=CAMELSRT_VOICE_ACR3;
}
#ifdef DEBUG_CAMELSRT
dbg(70,"Request ACR %u ",srt_type);
dbg(70,"ACR1 %u %u",p_camelsrt_call->category[CAMELSRT_VOICE_ACR1].req_num, p_camelsrt_call->category[CAMELSRT_VOICE_ACR1].rsp_num);
dbg(70,"ACR2 %u %u",p_camelsrt_call->category[CAMELSRT_VOICE_ACR2].req_num, p_camelsrt_call->category[CAMELSRT_VOICE_ACR2].rsp_num);
dbg(70,"ACR3 %u %u",p_camelsrt_call->category[CAMELSRT_VOICE_ACR3].req_num, p_camelsrt_call->category[CAMELSRT_VOICE_ACR3].rsp_num);
#endif
}
p_camelsrt_info->bool_msginfo[srt_type]=TRUE;
if (p_camelsrt_call->category[srt_type].req_num == 0) {
#ifdef DEBUG_CAMELSRT
dbg(5,"Set reqlink #%u ", pinfo->fd->num);
#endif
update_camelsrt_call(p_camelsrt_call, pinfo, srt_type);
} else {
if (p_camelsrt_call->category[srt_type].req_num != pinfo->fd->num) {
if (srt_type!=CAMELSRT_VOICE_DISC) {
#ifdef DEBUG_CAMELSRT
dbg(21,"Display_duplicate with req %d ", p_camelsrt_call->category[srt_type].req_num);
#endif
p_camelsrt_info->msginfo[srt_type].is_duplicate = TRUE;
if (gcamel_DisplaySRT){
hidden_item = proto_tree_add_uint(tree, hf_camelsrt_Duplicate, tvb, 0,0, 77);
PROTO_ITEM_SET_HIDDEN(hidden_item);
}
} else {
if (pinfo->fd->num > p_camelsrt_call->category[srt_type].req_num) {
p_camelsrt_call->category[srt_type].req_num = pinfo->fd->num;
#ifdef DEBUG_CAMELSRT
dbg(5,"DISC Set reqlink #%u ", pinfo->fd->num);
#endif
update_camelsrt_call(p_camelsrt_call, pinfo, srt_type);
}
}
}
}
if ( gcamel_DisplaySRT &&
(p_camelsrt_call->category[srt_type].rsp_num != 0) &&
(p_camelsrt_call->category[srt_type].req_num != 0) &&
(p_camelsrt_call->category[srt_type].req_num == pinfo->fd->num) ) {
#ifdef DEBUG_CAMELSRT
dbg(20,"Display_framersplink %d ",p_camelsrt_call->category[srt_type].rsp_num);
#endif
ti = proto_tree_add_uint_format(tree, hf_camelsrt_RequestFrame, tvb, 0, 0,
p_camelsrt_call->category[srt_type].rsp_num,
"Linked response %s in frame %u",
val_to_str_const(srt_type, camelSRTtype_naming, "Unk"),
p_camelsrt_call->category[srt_type].rsp_num);
PROTO_ITEM_SET_GENERATED(ti);
}
}
}
static void
camelsrt_report_call_matching(tvbuff_t *tvb, packet_info *pinfo,
proto_tree *tree,
struct camelsrt_info_t *p_camelsrt_info,
guint srt_type)
{
struct camelsrt_call_t *p_camelsrt_call;
struct camelsrt_call_info_key_t camelsrt_call_key;
nstime_t delta;
proto_item *ti, *hidden_item;
#ifdef DEBUG_CAMELSRT
dbg(10,"\n %s #%u\n", val_to_str_const(srt_type, camelSRTtype_naming, "Unk"),pinfo->fd->num);
#endif
camelsrt_call_key.SessionIdKey = p_camelsrt_info->tcap_session_id;
#ifdef DEBUG_CAMELSRT
dbg(11,"Search key %lu ",camelsrt_call_key.SessionIdKey);
#endif
p_camelsrt_call = find_camelsrt_call(&camelsrt_call_key);
if(p_camelsrt_call) {
#ifdef DEBUG_CAMELSRT
dbg(12,"Found, req=%d ",p_camelsrt_call->category[srt_type].req_num);
#endif
if ( gcamel_DisplaySRT )
proto_tree_add_uint(tree, hf_camelsrt_SessionId, tvb, 0,0, p_camelsrt_call->session_id);
if (srt_type==CAMELSRT_VOICE_ACR1) {
if (p_camelsrt_call->category[CAMELSRT_VOICE_ACR3].req_num != 0
&& p_camelsrt_call->category[CAMELSRT_VOICE_ACR3].req_num < pinfo->fd->num) {
srt_type=CAMELSRT_VOICE_ACR1;
} else if ( p_camelsrt_call->category[CAMELSRT_VOICE_ACR2].req_num != 0
&& p_camelsrt_call->category[CAMELSRT_VOICE_ACR2].req_num < pinfo->fd->num) {
srt_type=CAMELSRT_VOICE_ACR2;
} else if (p_camelsrt_call->category[CAMELSRT_VOICE_ACR1].req_num != 0
&& p_camelsrt_call->category[CAMELSRT_VOICE_ACR1].req_num < pinfo->fd->num) {
srt_type=CAMELSRT_VOICE_ACR1;
}
#ifdef DEBUG_CAMELSRT
dbg(70,"Report ACR %u ",srt_type);
#endif
}
p_camelsrt_info->bool_msginfo[srt_type]=TRUE;
if (p_camelsrt_call->category[srt_type].rsp_num == 0) {
if ( (p_camelsrt_call->category[srt_type].req_num != 0)
&& (pinfo->fd->num > p_camelsrt_call->category[srt_type].req_num) ){
#ifdef DEBUG_CAMELSRT
dbg(14,"Set reslink #%d req %u ",pinfo->fd->num, p_camelsrt_call->category[srt_type].req_num);
#endif
p_camelsrt_call->category[srt_type].rsp_num = pinfo->fd->num;
} else {
#ifdef DEBUG_CAMELSRT
dbg(2,"badreslink #%u req %u ",pinfo->fd->num, p_camelsrt_call->category[srt_type].req_num);
#endif
}
} else {
if (p_camelsrt_call->category[srt_type].rsp_num != pinfo->fd->num) {
#ifdef DEBUG_CAMELSRT
dbg(21,"Display_duplicate rsp=%d ", p_camelsrt_call->category[srt_type].rsp_num);
#endif
p_camelsrt_info->msginfo[srt_type].is_duplicate = TRUE;
if ( gcamel_DisplaySRT ){
hidden_item = proto_tree_add_uint(tree, hf_camelsrt_Duplicate, tvb, 0,0, 77);
PROTO_ITEM_SET_HIDDEN(hidden_item);
}
}
}
if ( (p_camelsrt_call->category[srt_type].req_num != 0) &&
(p_camelsrt_call->category[srt_type].rsp_num != 0) &&
(p_camelsrt_call->category[srt_type].rsp_num == pinfo->fd->num) ) {
p_camelsrt_call->category[srt_type].responded = TRUE;
p_camelsrt_info->msginfo[srt_type].request_available = TRUE;
#ifdef DEBUG_CAMELSRT
dbg(20,"Display_frameReqlink %d ",p_camelsrt_call->category[srt_type].req_num);
#endif
if ( gcamel_DisplaySRT ) {
ti = proto_tree_add_uint_format(tree, hf_camelsrt_ResponseFrame, tvb, 0, 0,
p_camelsrt_call->category[srt_type].req_num,
"Linked request %s in frame %u",
val_to_str_const(srt_type, camelSRTtype_naming, "Unk"),
p_camelsrt_call->category[srt_type].req_num);
PROTO_ITEM_SET_GENERATED(ti);
}
nstime_delta(&delta, &pinfo->fd->abs_ts, &p_camelsrt_call->category[srt_type].req_time);
p_camelsrt_info->msginfo[srt_type].is_delta_time = TRUE;
p_camelsrt_info->msginfo[srt_type].delta_time = delta;
p_camelsrt_info->msginfo[srt_type].req_time = p_camelsrt_call->category[srt_type].req_time;
camelsrt_display_DeltaTime(tree, tvb, &delta, srt_type);
}
}
}
static void
camelsrt_close_call_matching(packet_info *pinfo,
struct camelsrt_info_t *p_camelsrt_info)
{
struct camelsrt_call_t *p_camelsrt_call;
struct camelsrt_call_info_key_t camelsrt_call_key;
nstime_t delta;
p_camelsrt_info->bool_msginfo[CAMELSRT_SESSION]=TRUE;
#ifdef DEBUG_CAMELSRT
dbg(10,"\n Session end #%u\n", pinfo->fd->num);
#endif
camelsrt_call_key.SessionIdKey = p_camelsrt_info->tcap_session_id;
#ifdef DEBUG_CAMELSRT
dbg(11,"Search key %lu ",camelsrt_call_key.SessionIdKey);
#endif
p_camelsrt_call = find_camelsrt_call(&camelsrt_call_key);
if(p_camelsrt_call) {
#ifdef DEBUG_CAMELSRT
dbg(12,"Found ");
#endif
nstime_delta(&delta, &pinfo->fd->abs_ts, &p_camelsrt_call->category[CAMELSRT_SESSION].req_time);
p_camelsrt_call->category[CAMELSRT_SESSION].responded = TRUE;
p_camelsrt_info->msginfo[CAMELSRT_SESSION].request_available = TRUE;
p_camelsrt_info->msginfo[CAMELSRT_SESSION].is_delta_time = TRUE;
p_camelsrt_info->msginfo[CAMELSRT_SESSION].delta_time = delta;
p_camelsrt_info->msginfo[CAMELSRT_SESSION].req_time = p_camelsrt_call->category[CAMELSRT_SESSION].req_time;
if ( !gcamel_PersistentSRT ) {
g_hash_table_remove(srt_calls, &camelsrt_call_key);
#ifdef DEBUG_CAMELSRT
dbg(20,"remove hash ");
#endif
} else {
#ifdef DEBUG_CAMELSRT
dbg(20,"keep hash ");
#endif
}
}
}
static void
camelsrt_display_DeltaTime(proto_tree *tree, tvbuff_t *tvb, nstime_t *value_ptr,
guint category)
{
proto_item *ti;
if ( gcamel_DisplaySRT ) {
switch(category) {
case CAMELSRT_VOICE_INITIALDP:
ti = proto_tree_add_time(tree, hf_camelsrt_DeltaTime31, tvb, 0, 0, value_ptr);
PROTO_ITEM_SET_GENERATED(ti);
break;
case CAMELSRT_VOICE_ACR1:
case CAMELSRT_VOICE_ACR2:
case CAMELSRT_VOICE_ACR3:
ti = proto_tree_add_time(tree, hf_camelsrt_DeltaTime22, tvb, 0, 0, value_ptr);
PROTO_ITEM_SET_GENERATED(ti);
break;
case CAMELSRT_VOICE_DISC:
ti = proto_tree_add_time(tree, hf_camelsrt_DeltaTime35, tvb, 0, 0, value_ptr);
PROTO_ITEM_SET_GENERATED(ti);
break;
case CAMELSRT_GPRS_INITIALDP:
ti = proto_tree_add_time(tree, hf_camelsrt_DeltaTime75, tvb, 0, 0, value_ptr);
PROTO_ITEM_SET_GENERATED(ti);
break;
case CAMELSRT_GPRS_REPORT:
ti = proto_tree_add_time(tree, hf_camelsrt_DeltaTime80, tvb, 0, 0, value_ptr);
PROTO_ITEM_SET_GENERATED(ti);
break;
case CAMELSRT_SMS_INITIALDP:
ti = proto_tree_add_time(tree, hf_camelsrt_DeltaTime65, tvb, 0, 0, value_ptr);
PROTO_ITEM_SET_GENERATED(ti);
break;
default:
break;
}
}
}
struct camelsrt_info_t *
camelsrt_razinfo(void)
{
struct camelsrt_info_t *p_camelsrt_info ;
camelsrt_global_current++;
if(camelsrt_global_current==MAX_CAMEL_INSTANCE){
camelsrt_global_current=0;
}
p_camelsrt_info=&camelsrt_global_info[camelsrt_global_current];
memset(p_camelsrt_info,0,sizeof(struct camelsrt_info_t));
p_camelsrt_info->opcode=255;
return p_camelsrt_info;
}
static void
raz_camelsrt_call (struct camelsrt_call_t *p_camelsrt_call)
{
memset(p_camelsrt_call,0,sizeof(struct camelsrt_call_t));
}
