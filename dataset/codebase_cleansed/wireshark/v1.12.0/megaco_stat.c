static void
megacostat_reset(void *pms)
{
megacostat_t *ms=(megacostat_t *)pms;
int i;
for(i=0;i<NUM_TIMESTATS;i++) {
ms->rtd[i].num=0;
ms->rtd[i].min_num=0;
ms->rtd[i].max_num=0;
ms->rtd[i].min.secs=0;
ms->rtd[i].min.nsecs=0;
ms->rtd[i].max.secs=0;
ms->rtd[i].max.nsecs=0;
ms->rtd[i].tot.secs=0;
ms->rtd[i].tot.nsecs=0;
}
ms->open_req_num=0;
ms->disc_rsp_num=0;
ms->req_dup_num=0;
ms->rsp_dup_num=0;
}
static void
megacostat_draw(void *pms)
{
megacostat_t *ms=(megacostat_t *)pms;
int i;
char str[3][256];
GtkListStore *store;
GtkTreeIter iter;
store = GTK_LIST_STORE(gtk_tree_view_get_model(ms->table));
gtk_list_store_clear(store);
for(i=0;i<NUM_TIMESTATS;i++) {
if(ms->rtd[i].num==0){
continue;
}
g_snprintf(str[0], sizeof(char[256]), "%8.2f msec", nstime_to_msec(&(ms->rtd[i].min)));
g_snprintf(str[1], sizeof(char[256]), "%8.2f msec", nstime_to_msec(&(ms->rtd[i].max)));
g_snprintf(str[2], sizeof(char[256]), "%8.2f msec", get_average(&(ms->rtd[i].tot), ms->rtd[i].num));
gtk_list_store_append(store, &iter);
gtk_list_store_set(store, &iter,
0, val_to_str(i,megaco_message_type,"Other"),
1, ms->rtd[i].num,
2, str[0],
3, str[1],
4, str[2],
5, ms->rtd[i].min_num,
6, ms->rtd[i].max_num,
-1);
}
}
static void
win_destroy_cb(GtkWindow *win _U_, gpointer data)
{
megacostat_t *ms=(megacostat_t *)data;
remove_tap_listener(ms);
if(ms->filter){
g_free(ms->filter);
ms->filter=NULL;
}
g_free(ms);
}
static void
gtk_megacostat_init(const char *opt_arg, void *userdata _U_)
{
megacostat_t *ms;
GString *error_string;
GtkWidget *bt_close;
GtkWidget *bbox;
pref_t *megaco_ctx_track,*h248_ctx_track;
megaco_ctx_track = prefs_find_preference(prefs_find_module("megaco"),"ctx_info");
h248_ctx_track = prefs_find_preference(prefs_find_module("h248"),"ctx_info");
if (!megaco_ctx_track || !h248_ctx_track) {
return;
}
if (!*megaco_ctx_track->varp.boolp || !*h248_ctx_track->varp.boolp) {
simple_dialog(ESD_TYPE_ERROR, ESD_BTN_OK, "%s", "Track Context option at Protocols -> MEGACO and Protocols -> H248 preferences has to be set to true to enable measurement of service response times.");
return;
}
ms=(megacostat_t *)g_malloc(sizeof(megacostat_t));
if(strncmp(opt_arg,"megaco,srt,",11) == 0){
ms->filter=g_strdup(opt_arg+11);
} else {
ms->filter=NULL;
}
megacostat_reset(ms);
ms->win = dlg_window_new("MEGACO SRT");
gtk_window_set_destroy_with_parent (GTK_WINDOW(ms->win), TRUE);
gtk_window_set_default_size(GTK_WINDOW(ms->win), 550, 150);
ms->vbox=ws_gtk_box_new(GTK_ORIENTATION_VERTICAL, 6, FALSE);
init_main_stat_window(ms->win, ms->vbox, "MEGACO Service Response Time (SRT) Statistics", ms->filter);
ms->scrolled_window = scrolled_window_new(NULL, NULL);
ms->table = create_stat_table(ms->scrolled_window, ms->vbox, 7, titles);
error_string=register_tap_listener("megaco", ms, ms->filter, 0, megacostat_reset, megacostat_packet, megacostat_draw);
if(error_string){
simple_dialog(ESD_TYPE_ERROR, ESD_BTN_OK, "%s", error_string->str);
g_string_free(error_string, TRUE);
g_free(ms->filter);
g_free(ms);
return;
}
bbox = dlg_button_row_new(GTK_STOCK_CLOSE, NULL);
gtk_box_pack_start(GTK_BOX(ms->vbox), bbox, FALSE, FALSE, 0);
bt_close = (GtkWidget *)g_object_get_data(G_OBJECT(bbox), GTK_STOCK_CLOSE);
window_set_cancel_button(ms->win, bt_close, window_cancel_button_cb);
g_signal_connect(ms->win, "delete_event", G_CALLBACK(window_delete_event_cb), NULL);
g_signal_connect(ms->win, "destroy", G_CALLBACK(win_destroy_cb), ms);
gtk_widget_show_all(ms->win);
window_present(ms->win);
cf_retap_packets(&cfile);
gdk_window_raise(gtk_widget_get_window(ms->win));
}
void
register_tap_listener_gtkmegacostat(void)
{
register_param_stat(&megaco_srt_dlg, "MEGACO",
REGISTER_STAT_GROUP_RESPONSE_TIME);
}
