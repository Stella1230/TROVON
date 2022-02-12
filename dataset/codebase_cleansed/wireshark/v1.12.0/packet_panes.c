GtkWidget *
get_notebook_bv_ptr(GtkWidget *nb_ptr)
{
int num;
GtkWidget *bv_page;
num = gtk_notebook_get_current_page(GTK_NOTEBOOK(nb_ptr));
bv_page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(nb_ptr), num);
if (bv_page)
return gtk_bin_get_child(GTK_BIN(bv_page));
else
return NULL;
}
const guint8 *
get_byte_view_data_and_length(GtkWidget *byte_view, guint *data_len)
{
tvbuff_t *byte_view_tvb;
const guint8 *data_ptr;
byte_view_tvb = (tvbuff_t *)g_object_get_data(G_OBJECT(byte_view), E_BYTE_VIEW_TVBUFF_KEY);
if (byte_view_tvb == NULL)
return NULL;
if ((*data_len = tvb_length(byte_view_tvb))) {
data_ptr = tvb_get_ptr(byte_view_tvb, 0, -1);
return data_ptr;
} else
return "";
}
void
set_notebook_page(GtkWidget *nb_ptr, tvbuff_t *tvb)
{
int num;
GtkWidget *bv_page, *bv;
tvbuff_t *bv_tvb;
for (num = 0;
(bv_page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(nb_ptr), num)) != NULL;
num++) {
bv = gtk_bin_get_child(GTK_BIN(bv_page));
bv_tvb = (tvbuff_t *)g_object_get_data(G_OBJECT(bv), E_BYTE_VIEW_TVBUFF_KEY);
if (bv_tvb == tvb) {
gtk_notebook_set_current_page(GTK_NOTEBOOK(nb_ptr), num);
break;
}
}
}
void
redraw_packet_bytes(GtkWidget *nb, frame_data *fd, field_info *finfo)
{
GtkWidget *bv;
const guint8 *data;
guint len;
bv = get_notebook_bv_ptr(nb);
if (bv != NULL) {
data = get_byte_view_data_and_length(bv, &len);
if (data != NULL)
packet_hex_print(bv, data, fd, finfo, len);
}
}
void
redraw_packet_bytes_all(void)
{
if (cfile.current_frame != NULL)
redraw_packet_bytes( byte_nb_ptr_gbl, cfile.current_frame, cfile.finfo_selected);
redraw_packet_bytes_packet_wins();
if (cfile.current_frame != NULL) {
cfile.current_row = -1;
cf_goto_frame(&cfile, cfile.current_frame->num);
}
}
static void
check_expand_trees(GtkTreeView *tree_view, GtkTreeModel *model, GtkTreePath *path,
GtkTreeIter *iter, gboolean scroll_it, gboolean expand_parent)
{
field_info *fi;
do {
GtkTreeIter child;
if (gtk_tree_model_iter_children(model, &child, iter)) {
gtk_tree_model_get(model, iter, 1, &fi, -1);
if (tree_expanded(fi->tree_type)) {
if (expand_parent)
gtk_tree_view_expand_row(tree_view, path, FALSE);
if (scroll_it)
gtk_tree_view_scroll_to_cell(tree_view, path, NULL, TRUE, (prefs.gui_auto_scroll_percentage/100.0f), 0.0f);
gtk_tree_path_down(path);
check_expand_trees(tree_view, model, path, &child, scroll_it, TRUE);
gtk_tree_path_up(path);
}
}
gtk_tree_path_next(path);
} while (gtk_tree_model_iter_next(model, iter));
}
static void
expand_tree(GtkTreeView *tree_view, GtkTreeIter *iter,
GtkTreePath *path, gpointer user_data _U_)
{
field_info *finfo;
GtkTreeModel *model;
model = gtk_tree_view_get_model(tree_view);
gtk_tree_model_get(model, iter, 1, &finfo, -1);
g_assert(finfo);
if(prefs.gui_auto_scroll_on_expand)
gtk_tree_view_scroll_to_cell(tree_view, path, NULL, TRUE, (prefs.gui_auto_scroll_percentage/100.0f), 0.0f);
if (finfo->tree_type != -1)
tree_expanded_set(finfo->tree_type, TRUE);
if (finfo->tree_type != -1 && path) {
g_signal_handlers_block_by_func(tree_view, expand_tree, NULL);
check_expand_trees(tree_view, model, path, iter, FALSE, FALSE);
g_signal_handlers_unblock_by_func(tree_view, expand_tree, NULL);
}
}
static void
collapse_tree(GtkTreeView *tree_view, GtkTreeIter *iter,
GtkTreePath *path _U_, gpointer user_data _U_)
{
field_info *finfo;
GtkTreeModel *model;
model = gtk_tree_view_get_model(tree_view);
gtk_tree_model_get(model, iter, 1, &finfo, -1);
g_assert(finfo);
if (finfo->tree_type != -1)
tree_expanded_set(finfo->tree_type, FALSE);
}
static gboolean
lookup_finfo(GtkTreeModel *model, GtkTreePath *path _U_, GtkTreeIter *iter,
gpointer data)
{
field_info *fi;
struct field_lookup_info *fli = (struct field_lookup_info *)data;
gtk_tree_model_get(model, iter, 1, &fi, -1);
if (fi == fli->fi) {
fli->iter = *iter;
return TRUE;
}
return FALSE;
}
GtkTreePath
*tree_find_by_field_info(GtkTreeView *tree_view, field_info *finfo)
{
GtkTreeModel *model;
struct field_lookup_info fli;
g_assert(finfo != NULL);
model = gtk_tree_view_get_model(tree_view);
fli.fi = finfo;
gtk_tree_model_foreach(model, lookup_finfo, &fli);
return gtk_tree_model_get_path(model, &fli.iter);
}
gboolean
byte_view_select(GtkWidget *widget, GdkEventButton *event)
{
proto_tree *tree;
GtkTreeView *tree_view;
int byte = -1;
tvbuff_t *tvb;
tree = (proto_tree *)g_object_get_data(G_OBJECT(widget), E_BYTE_VIEW_TREE_PTR);
if (tree == NULL) {
return FALSE;
}
tree_view = GTK_TREE_VIEW(g_object_get_data(G_OBJECT(widget),
E_BYTE_VIEW_TREE_VIEW_PTR));
byte = bytes_view_byte_from_xy(BYTES_VIEW(widget), (gint) event->x, (gint) event->y);
if (byte == -1) {
return FALSE;
}
tvb = (tvbuff_t *)g_object_get_data(G_OBJECT(widget), E_BYTE_VIEW_TVBUFF_KEY);
return highlight_field(tvb, byte, tree_view, tree);
}
gboolean
highlight_field(tvbuff_t *tvb, gint byte, GtkTreeView *tree_view,
proto_tree *tree)
{
GtkTreeModel *model = NULL;
GtkTreePath *first_path = NULL, *path = NULL;
GtkTreeIter parent;
field_info *finfo = NULL;
match_data mdata;
struct field_lookup_info fli;
if (cfile.search_in_progress && cfile.string && cfile.decode_data) {
if (cf_find_string_protocol_tree(&cfile, tree, &mdata)) {
finfo = mdata.finfo;
}
} else {
finfo = proto_find_field_from_offset(tree, byte, tvb);
}
if (!finfo) {
return FALSE;
}
model = gtk_tree_view_get_model(tree_view);
fli.fi = finfo;
gtk_tree_model_foreach(model, lookup_finfo, &fli);
first_path = gtk_tree_model_get_path(model, &fli.iter);
gtk_tree_view_expand_row(tree_view, first_path, FALSE);
expand_tree(tree_view, &fli.iter, NULL, NULL);
while (gtk_tree_model_iter_parent(model, &parent, &fli.iter)) {
path = gtk_tree_model_get_path(model, &parent);
gtk_tree_view_expand_row(tree_view, path, FALSE);
expand_tree(tree_view, &parent, NULL, NULL);
fli.iter = parent;
gtk_tree_path_free(path);
}
gtk_tree_selection_select_path(gtk_tree_view_get_selection(tree_view),
first_path);
if (!cfile.search_in_progress) {
if (cfile.hex || (cfile.string && cfile.packet_data)) {
redraw_packet_bytes(byte_nb_ptr_gbl, cfile.current_frame, cfile.finfo_selected);
}
}
gtk_tree_view_scroll_to_cell(tree_view, first_path, NULL, TRUE, 0.5f, 0.0f);
gtk_tree_path_free(first_path);
return TRUE;
}
static gboolean
byte_view_button_press_cb(GtkWidget *widget, GdkEvent *event, gpointer data)
{
GdkEventButton *event_button = NULL;
if(widget == NULL || event == NULL || data == NULL) {
return FALSE;
}
if(event->type == GDK_BUTTON_PRESS) {
event_button = (GdkEventButton *) event;
switch(event_button->button) {
case 1:
return byte_view_select(widget, event_button);
case 3:
return popup_menu_handler(widget, event, data);
default:
return FALSE;
}
}
return FALSE;
}
GtkWidget *
byte_view_new(void)
{
GtkWidget *byte_nb;
byte_nb = gtk_notebook_new();
gtk_notebook_set_tab_pos(GTK_NOTEBOOK(byte_nb), GTK_POS_BOTTOM);
gtk_notebook_set_show_border(GTK_NOTEBOOK(byte_nb), FALSE);
gtk_notebook_set_scrollable(GTK_NOTEBOOK(byte_nb), TRUE);
gtk_notebook_popup_enable(GTK_NOTEBOOK(byte_nb));
add_byte_tab(byte_nb, "", NULL, NULL, NULL);
return byte_nb;
}
static void
byte_view_realize_cb(GtkWidget *bv, gpointer data _U_)
{
const guint8 *byte_data;
guint byte_len;
byte_data = get_byte_view_data_and_length(bv, &byte_len);
if (byte_data == NULL) {
return;
}
packet_hex_print(bv, byte_data, cfile.current_frame, NULL, byte_len);
}
GtkWidget *
add_byte_tab(GtkWidget *byte_nb, const char *name, tvbuff_t *tvb,
proto_tree *tree, GtkWidget *tree_view)
{
GtkWidget *byte_view, *byte_scrollw, *label;
byte_scrollw = scrolled_window_new(NULL, NULL);
gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(byte_scrollw),
GTK_SHADOW_IN);
label = gtk_label_new(name);
gtk_notebook_append_page(GTK_NOTEBOOK(byte_nb), byte_scrollw, label);
gtk_widget_show(byte_scrollw);
byte_view = bytes_view_new();
bytes_view_set_font(BYTES_VIEW(byte_view), user_font_get_regular());
g_object_set_data(G_OBJECT(byte_view), E_BYTE_VIEW_TVBUFF_KEY, tvb);
gtk_container_add(GTK_CONTAINER(byte_scrollw), byte_view);
g_signal_connect(byte_view, "show", G_CALLBACK(byte_view_realize_cb), NULL);
g_signal_connect(byte_view, "button_press_event", G_CALLBACK(byte_view_button_press_cb),
g_object_get_data(G_OBJECT(popup_menu_object), PM_BYTES_VIEW_KEY));
g_object_set_data(G_OBJECT(byte_view), E_BYTE_VIEW_TREE_PTR, tree);
g_object_set_data(G_OBJECT(byte_view), E_BYTE_VIEW_TREE_VIEW_PTR, tree_view);
gtk_widget_show(byte_view);
if (!(gtk_notebook_page_num(GTK_NOTEBOOK(byte_nb), byte_scrollw)))
gtk_notebook_set_show_tabs(GTK_NOTEBOOK(byte_nb), FALSE);
else
gtk_notebook_set_show_tabs(GTK_NOTEBOOK(byte_nb), TRUE);
gtk_notebook_set_current_page(GTK_NOTEBOOK(byte_nb),
gtk_notebook_page_num(GTK_NOTEBOOK(byte_nb), byte_nb));
return byte_view;
}
void
add_byte_views(epan_dissect_t *edt, GtkWidget *tree_view,
GtkWidget *byte_nb_ptr)
{
GSList *src_le;
struct data_source *src;
while (gtk_notebook_get_nth_page(GTK_NOTEBOOK(byte_nb_ptr), 0) != NULL)
gtk_notebook_remove_page(GTK_NOTEBOOK(byte_nb_ptr), 0);
for (src_le = edt->pi.data_src; src_le != NULL; src_le = src_le->next) {
src = (struct data_source *)src_le->data;
add_byte_tab(byte_nb_ptr, get_data_source_name(src), get_data_source_tvb(src), edt->tree,
tree_view);
}
gtk_notebook_set_current_page(GTK_NOTEBOOK(byte_nb_ptr), 0);
}
static void
copy_hex_all_info(GString* copy_buffer, const guint8* data_p, int data_len, gboolean append_text)
{
const int byte_line_length = 16;
int i, j;
gboolean end_of_line = TRUE;
int byte_line_part_length;
GString* hex_str;
GString* char_str;
hex_str = g_string_new("");
char_str= g_string_new("");
i = 0;
while (i<data_len) {
if(end_of_line) {
g_string_append_printf(hex_str,"%04x ",i);
}
g_string_append_printf(hex_str," %02x",*data_p);
if(append_text) {
g_string_append_printf(char_str,"%c",g_ascii_isprint(*data_p) ? *data_p : '.');
}
++data_p;
byte_line_part_length = (++i) % byte_line_length;
if(i == data_len){
for(j = 0; append_text && (j < (byte_line_length - byte_line_part_length)); ++j) {
g_string_append(hex_str," ");
}
end_of_line = TRUE;
} else {
end_of_line = (byte_line_part_length == 0 ? TRUE : FALSE);
}
if (end_of_line){
g_string_append(copy_buffer, hex_str->str);
if(append_text) {
g_string_append_c(copy_buffer, ' ');
g_string_append_c(copy_buffer, ' ');
g_string_append(copy_buffer, char_str->str);
}
g_string_assign(char_str,"");
g_string_assign(hex_str, "\n");
}
}
g_string_free(hex_str, TRUE);
g_string_free(char_str, TRUE);
}
static int
copy_hex_bytes_text_only(GString* copy_buffer, const guint8* data_p, int data_len _U_)
{
gchar to_append;
if(g_ascii_isprint(*data_p)) {
to_append = *data_p;
} else if(*data_p==0x0a) {
to_append = '\n';
} else if(*data_p==0x09) {
to_append = '\t';
} else {
return 1;
}
g_string_append_c(copy_buffer,to_append);
return 1;
}
static
int copy_hex_bytes_hex(GString* copy_buffer, const guint8* data_p, int data_len _U_)
{
g_string_append_printf(copy_buffer, "%02x", *data_p);
return 1;
}
void
copy_hex_cb(GtkWidget * w _U_, gpointer data _U_, copy_data_type data_type)
{
GtkWidget *bv;
guint len = 0;
int bytes_consumed = 0;
int flags;
const guint8* data_p;
GString* copy_buffer = g_string_new("");
bv = get_notebook_bv_ptr(byte_nb_ptr_gbl);
if (bv == NULL) {
simple_dialog(ESD_TYPE_ERROR, ESD_BTN_OK, "Could not find the corresponding text window.");
return;
}
data_p = get_byte_view_data_and_length(bv, &len);
g_assert(data_p != NULL);
flags = data_type & CD_FLAGSMASK;
data_type = (copy_data_type)(data_type & CD_TYPEMASK);
if(flags & CD_FLAGS_SELECTEDONLY) {
int start, end;
start = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(bv), E_BYTE_VIEW_START_KEY));
end = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(bv), E_BYTE_VIEW_END_KEY));
if(start >= 0 && end > start && (end - start <= (int)len)) {
len = end - start;
data_p += start;
}
}
switch(data_type) {
case(CD_ALLINFO):
copy_hex_all_info(copy_buffer, data_p, len, TRUE);
break;
case(CD_HEXCOLUMNS):
copy_hex_all_info(copy_buffer, data_p, len, FALSE);
break;
case(CD_BINARY):
copy_binary_to_clipboard(data_p,len);
break;
default:
while (len > 0){
switch(data_type) {
case (CD_TEXTONLY):
bytes_consumed = copy_hex_bytes_text_only(copy_buffer, data_p, len);
break;
case (CD_HEX):
bytes_consumed = copy_hex_bytes_hex(copy_buffer, data_p, len);
break;
default:
g_assert_not_reached();
break;
}
g_assert(bytes_consumed>0);
data_p += bytes_consumed;
len -= bytes_consumed;
}
break;
}
if(copy_buffer->len > 0) {
copy_to_clipboard(copy_buffer);
}
g_string_free(copy_buffer, TRUE);
}
static gboolean
savehex_save_clicked_cb(gchar *file, int start, int end, const guint8 *data_p)
{
int fd;
fd = ws_open(file, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0666);
if (fd == -1) {
open_failure_alert_box(file, errno, TRUE);
return FALSE;
}
if (ws_write(fd, data_p + start, end - start) < 0) {
write_failure_alert_box(file, errno);
ws_close(fd);
return FALSE;
}
if (ws_close(fd) < 0) {
write_failure_alert_box(file, errno);
return FALSE;
}
return TRUE;
}
void
savehex_cb(GtkWidget * w _U_, gpointer data _U_)
{
win32_export_raw_file(GDK_WINDOW_HWND(gtk_widget_get_window(top_level)), &cfile);
return;
}
static char *
gtk_export_raw_file(int start, int end)
{
GtkWidget *savehex_dlg;
gchar *label;
GtkWidget *dlg_lb;
char *pathname;
savehex_dlg = file_selection_new("Wireshark: Export Selected Packet Bytes", GTK_WINDOW(top_level), FILE_SELECTION_SAVE);
label = g_strdup_printf("Will save %u %s of raw binary data to specified file.",
end - start, plurality(end - start, "byte", "bytes"));
dlg_lb = gtk_label_new(label);
g_free(label);
file_selection_set_extra_widget(savehex_dlg, dlg_lb);
gtk_widget_show(dlg_lb);
pathname = file_selection_run(savehex_dlg);
if (pathname == NULL) {
return NULL;
}
window_destroy(savehex_dlg);
return pathname;
}
void
savehex_cb(GtkWidget * w _U_, gpointer data _U_)
{
int start, end;
guint len;
const guint8 *data_p = NULL;
GtkWidget *bv;
char *pathname;
bv = get_notebook_bv_ptr(byte_nb_ptr_gbl);
if (bv == NULL) {
simple_dialog(ESD_TYPE_ERROR, ESD_BTN_OK, "Could not find the corresponding text window.");
return;
}
start = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(bv), E_BYTE_VIEW_START_KEY));
end = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(bv), E_BYTE_VIEW_END_KEY));
data_p = get_byte_view_data_and_length(bv, &len);
if (data_p == NULL || start == -1 || start > end) {
simple_dialog(ESD_TYPE_ERROR, ESD_BTN_OK, "No data selected to save.");
return;
}
for (;;) {
pathname = gtk_export_raw_file(start, end);
if (pathname == NULL) {
break;
}
if (savehex_save_clicked_cb(pathname, start, end, data_p)) {
g_free(pathname);
break;
}
g_free(pathname);
}
}
static void
packet_hex_update(GtkWidget *bv, const guint8 *pd, int len, int bstart,
int bend, guint32 bmask, int bmask_le,
int astart, int aend,
int pstart, int pend,
int encoding)
{
bytes_view_set_encoding(BYTES_VIEW(bv), encoding);
bytes_view_set_format(BYTES_VIEW(bv), recent.gui_bytes_view);
bytes_view_set_data(BYTES_VIEW(bv), pd, len);
bytes_view_set_highlight_style(BYTES_VIEW(bv), prefs.gui_hex_dump_highlight_style);
bytes_view_set_highlight(BYTES_VIEW(bv), bstart, bend, bmask, bmask_le);
bytes_view_set_highlight_extra(BYTES_VIEW(bv), BYTE_VIEW_HIGHLIGHT_APPENDIX, astart, aend);
bytes_view_set_highlight_extra(BYTES_VIEW(bv), BYTE_VIEW_HIGHLIGHT_PROTOCOL, pstart, pend);
if (bstart != -1 && bend != -1)
bytes_view_scroll_to_byte(BYTES_VIEW(bv), bstart);
bytes_view_refresh(BYTES_VIEW(bv));
}
static field_info *
get_top_finfo(proto_node *node, field_info *finfo)
{
proto_node *child;
field_info *top;
if (node == NULL)
return NULL;
if (PNODE_FINFO(node) == finfo) {
top = finfo;
while (node && node->parent) {
field_info *fi;
node = node->parent;
fi = PNODE_FINFO(node);
if (fi && fi->ds_tvb == finfo->ds_tvb)
top = fi;
}
return top;
}
for (child = node->first_child; child; child = child->next) {
top = get_top_finfo(child, finfo);
if (top)
return top;
}
return NULL;
}
void
packet_hex_print(GtkWidget *bv, const guint8 *pd, frame_data *fd,
field_info *finfo, guint len)
{
int bstart = -1, bend = -1, blen = -1;
guint32 bmask = 0x00; int bmask_le = 0;
int astart = -1, aend = -1, alen = -1;
int pstart = -1, pend = -1, plen = -1;
if (finfo != NULL) {
proto_tree *tree = (proto_tree *)g_object_get_data(G_OBJECT(bv), E_BYTE_VIEW_TREE_PTR);
field_info *top_finfo;
if (cfile.search_in_progress && (cfile.hex || (cfile.string && cfile.packet_data))) {
if (cfile.hex) {
char *p = cfile.sfilter;
blen = 0;
while (*p) {
if (g_ascii_isxdigit(*p++))
blen++;
}
blen = (blen + 1) / 2;
} else {
blen = (int)strlen(cfile.sfilter);
}
bstart = cfile.search_pos - (blen-1);
} else {
blen = finfo->length;
bstart = finfo->start;
}
if (finfo->hfinfo) bmask = finfo->hfinfo->bitmask;
astart = finfo->appendix_start;
alen = finfo->appendix_length;
top_finfo = get_top_finfo(tree, finfo);
if (top_finfo) {
pstart = top_finfo->start;
plen = top_finfo->length;
}
if (FI_GET_FLAG(finfo, FI_LITTLE_ENDIAN))
bmask_le = 1;
else if (FI_GET_FLAG(finfo, FI_BIG_ENDIAN))
bmask_le = 0;
else {
bmask = 0x00;
}
if (bmask == 0x00) {
int bito = FI_GET_BITS_OFFSET(finfo);
int bitc = FI_GET_BITS_SIZE(finfo);
int bitt = bito + bitc;
if (bitt > 0 && bitt < 32) {
bmask = ((1 << bitc) - 1) << ((8-bitt) & 7);
bmask_le = 0;
}
}
}
if (pstart >= 0 && plen > 0 && (guint)pstart < len)
pend = pstart + plen;
if (bstart >= 0 && blen > 0 && (guint)bstart < len)
bend = bstart + blen;
if (astart >= 0 && alen > 0 && (guint)astart < len)
aend = astart + alen;
if (bend == -1 && aend != -1) {
bstart = astart;
bmask = 0x00;
bend = aend;
astart = aend = -1;
}
if (aend != -1 && (guint)aend > len) aend = len;
if (bend != -1 && (guint)bend > len) bend = len;
if (pend != -1 && (guint)pend > len) pend = len;
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_START_KEY, GINT_TO_POINTER(bstart));
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_END_KEY, GINT_TO_POINTER(bend));
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_MASK_KEY, GINT_TO_POINTER(bmask));
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_MASKLE_KEY, GINT_TO_POINTER(bmask_le));
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_APP_START_KEY, GINT_TO_POINTER(astart));
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_APP_END_KEY, GINT_TO_POINTER(aend));
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_PROTO_START_KEY, GINT_TO_POINTER(pstart));
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_PROTO_END_KEY, GINT_TO_POINTER(pend));
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_ENCODE_KEY,
GUINT_TO_POINTER((guint)fd->flags.encoding));
if (recent.gui_bytes_view != BYTES_BITS)
bmask = 0x00;
packet_hex_update(bv, pd, len, bstart, bend, bmask, bmask_le, astart, aend, pstart, pend, fd->flags.encoding);
}
void
packet_hex_editor_print(GtkWidget *bv, const guint8 *pd, frame_data *fd, int offset, int bitoffset, guint len)
{
int bstart = offset, bend = (bstart != -1) ? offset+1 : -1;
guint32 bmask=0; int bmask_le = 0;
int astart = -1, aend = -1;
int pstart = -1, pend = -1;
switch (recent.gui_bytes_view) {
case BYTES_HEX:
bmask = (bitoffset == 0) ? 0xf0 : (bitoffset == 4) ? 0x0f : 0xff;
break;
case BYTES_BITS:
bmask = (1 << (7-bitoffset));
break;
default:
g_assert_not_reached();
break;
}
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_START_KEY, GINT_TO_POINTER(bstart));
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_END_KEY, GINT_TO_POINTER(bend));
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_MASK_KEY, GINT_TO_POINTER(bmask));
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_MASKLE_KEY, GINT_TO_POINTER(bmask_le));
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_APP_START_KEY, GINT_TO_POINTER(astart));
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_APP_END_KEY, GINT_TO_POINTER(aend));
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_ENCODE_KEY,
GUINT_TO_POINTER((guint)fd->flags.encoding));
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_PROTO_START_KEY, GINT_TO_POINTER(pstart));
g_object_set_data(G_OBJECT(bv), E_BYTE_VIEW_PROTO_END_KEY, GINT_TO_POINTER(pend));
packet_hex_update(bv, pd, len, bstart, bend, bmask, bmask_le, astart, aend, pstart, pend, fd->flags.encoding);
}
void
packet_hex_reprint(GtkWidget *bv)
{
int start, end, mask, mask_le, encoding;
int astart, aend;
int pstart, pend;
const guint8 *data;
guint len = 0;
start = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(bv), E_BYTE_VIEW_START_KEY));
end = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(bv), E_BYTE_VIEW_END_KEY));
mask = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(bv), E_BYTE_VIEW_MASK_KEY));
mask_le = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(bv), E_BYTE_VIEW_MASKLE_KEY));
astart = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(bv), E_BYTE_VIEW_APP_START_KEY));
aend = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(bv), E_BYTE_VIEW_APP_END_KEY));
pstart = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(bv), E_BYTE_VIEW_PROTO_START_KEY));
pend = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(bv), E_BYTE_VIEW_PROTO_END_KEY));
data = get_byte_view_data_and_length(bv, &len);
g_assert(data != NULL);
encoding = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(bv), E_BYTE_VIEW_ENCODE_KEY));
if (recent.gui_bytes_view != BYTES_BITS)
mask = 0x00;
packet_hex_update(bv, data, len, start, end, mask, mask_le, astart, aend, pstart, pend, encoding);
}
static void
remember_ptree_widget(GtkWidget *ptreew)
{
ptree_widgets = g_list_append(ptree_widgets, ptreew);
g_signal_connect(ptreew, "destroy", G_CALLBACK(forget_ptree_widget), NULL);
}
static void
forget_ptree_widget(GtkWidget *ptreew, gpointer data _U_)
{
ptree_widgets = g_list_remove(ptree_widgets, ptreew);
}
static void
set_ptree_font_cb(gpointer data, gpointer user_data)
{
#if GTK_CHECK_VERSION(3,0,0)
gtk_widget_override_font((GtkWidget *)data,
(PangoFontDescription *)user_data);
#else
gtk_widget_modify_font((GtkWidget *)data,
(PangoFontDescription *)user_data);
#endif
}
void
set_ptree_font_all(PangoFontDescription *font)
{
g_list_foreach(ptree_widgets, set_ptree_font_cb, font);
}
void proto_draw_colors_init(void)
{
if(colors_ok) {
return;
}
#if 0
get_color(&expert_color_chat);
get_color(&expert_color_note);
get_color(&expert_color_warn);
get_color(&expert_color_error);
get_color(&expert_color_foreground);
#endif
expert_color_comment_str = gdk_color_to_string(&expert_color_comment);
expert_color_chat_str = gdk_color_to_string(&expert_color_chat);
expert_color_note_str = gdk_color_to_string(&expert_color_note);
expert_color_warn_str = gdk_color_to_string(&expert_color_warn);
expert_color_error_str = gdk_color_to_string(&expert_color_error);
expert_color_foreground_str = gdk_color_to_string(&expert_color_foreground);
#if 0
get_color(&hidden_proto_item);
#endif
colors_ok = TRUE;
}
static void
tree_cell_renderer(GtkTreeViewColumn *tree_column _U_, GtkCellRenderer *cell,
GtkTreeModel *tree_model, GtkTreeIter *iter,
gpointer data _U_)
{
field_info *fi;
gtk_tree_model_get(tree_model, iter, 1, &fi, -1);
if(!colors_ok) {
proto_draw_colors_init();
}
g_object_set (cell, "foreground-set", FALSE, NULL);
g_object_set (cell, "background-set", FALSE, NULL);
g_object_set (cell, "underline", PANGO_UNDERLINE_NONE, NULL);
g_object_set (cell, "underline-set", FALSE, NULL);
if(FI_GET_FLAG(fi, FI_GENERATED)) {
}
if(FI_GET_FLAG(fi, FI_HIDDEN)) {
g_object_set (cell, "foreground-gdk", &hidden_proto_item, NULL);
g_object_set (cell, "foreground-set", TRUE, NULL);
}
if (fi && fi->hfinfo) {
if(fi->hfinfo->type == FT_PROTOCOL) {
g_object_set (cell, "background", "gray90", NULL);
g_object_set (cell, "background-set", TRUE, NULL);
g_object_set (cell, "foreground", "black", NULL);
g_object_set (cell, "foreground-set", TRUE, NULL);
}
if((fi->hfinfo->type == FT_FRAMENUM) ||
(FI_GET_FLAG(fi, FI_URL) && IS_FT_STRING(fi->hfinfo->type))) {
render_as_url(cell);
}
}
if(FI_GET_FLAG(fi, PI_SEVERITY_MASK)) {
switch(FI_GET_FLAG(fi, PI_SEVERITY_MASK)) {
case(PI_COMMENT):
g_object_set (cell, "background-gdk", &expert_color_comment, NULL);
g_object_set (cell, "background-set", TRUE, NULL);
break;
case(PI_CHAT):
g_object_set (cell, "background-gdk", &expert_color_chat, NULL);
g_object_set (cell, "background-set", TRUE, NULL);
break;
case(PI_NOTE):
g_object_set (cell, "background-gdk", &expert_color_note, NULL);
g_object_set (cell, "background-set", TRUE, NULL);
break;
case(PI_WARN):
g_object_set (cell, "background-gdk", &expert_color_warn, NULL);
g_object_set (cell, "background-set", TRUE, NULL);
break;
case(PI_ERROR):
g_object_set (cell, "background-gdk", &expert_color_error, NULL);
g_object_set (cell, "background-set", TRUE, NULL);
break;
default:
g_assert_not_reached();
}
g_object_set (cell, "foreground", "black", NULL);
g_object_set (cell, "foreground-set", TRUE, NULL);
}
}
GtkWidget *
proto_tree_view_new(GtkWidget **tree_view_p)
{
GtkWidget *tv_scrollw, *tree_view;
ProtoTreeModel *store;
GtkCellRenderer *renderer;
GtkTreeViewColumn *column;
gint col_offset;
tv_scrollw = scrolled_window_new(NULL, NULL);
gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(tv_scrollw),
GTK_SHADOW_IN);
store = proto_tree_model_new(NULL, prefs.display_hidden_proto_items);
tree_view = tree_view_new(GTK_TREE_MODEL(store));
g_object_unref(G_OBJECT(store));
gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree_view), FALSE);
renderer = gtk_cell_renderer_text_new();
g_object_set (renderer, "ypad", 0, NULL);
col_offset = gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree_view),
-1, "Name", renderer,
"text", 0, NULL);
column = gtk_tree_view_get_column(GTK_TREE_VIEW(tree_view),
col_offset - 1);
gtk_tree_view_column_set_cell_data_func(column, renderer, tree_cell_renderer,
NULL, NULL);
gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(column),
GTK_TREE_VIEW_COLUMN_AUTOSIZE);
g_signal_connect(tree_view, "row-expanded", G_CALLBACK(expand_tree), NULL);
g_signal_connect(tree_view, "row-collapsed", G_CALLBACK(collapse_tree), NULL);
gtk_container_add( GTK_CONTAINER(tv_scrollw), tree_view );
#if GTK_CHECK_VERSION(3,0,0)
gtk_widget_override_font(tree_view, user_font_get_regular());
#else
gtk_widget_modify_font(tree_view, user_font_get_regular());
#endif
remember_ptree_widget(tree_view);
*tree_view_p = tree_view;
return tv_scrollw;
}
void
expand_all_tree(proto_tree *protocol_tree _U_, GtkWidget *tree_view)
{
int i;
for(i=0; i < num_tree_types; i++)
tree_expanded_set(i, TRUE);
gtk_tree_view_expand_all(GTK_TREE_VIEW(tree_view));
}
void
collapse_all_tree(proto_tree *protocol_tree _U_, GtkWidget *tree_view)
{
int i;
for(i=0; i < num_tree_types; i++)
tree_expanded_set(i, FALSE);
gtk_tree_view_collapse_all(GTK_TREE_VIEW(tree_view));
}
static void
tree_view_follow_link(field_info *fi)
{
gchar *url;
if(fi->hfinfo->type == FT_FRAMENUM) {
cf_goto_frame(&cfile, fi->value.value.uinteger);
}
if(FI_GET_FLAG(fi, FI_URL) && IS_FT_STRING(fi->hfinfo->type)) {
url = fvalue_to_string_repr(&fi->value, FTREPR_DISPLAY, NULL);
if(url){
browser_open_url(url);
g_free(url);
}
}
}
gboolean
tree_view_select(GtkWidget *widget, GdkEventButton *event)
{
GtkTreeSelection *sel;
GtkTreePath *path;
if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(widget),
(gint) (((GdkEventButton *)event)->x),
(gint) (((GdkEventButton *)event)->y),
&path, NULL, NULL, NULL))
{
sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
if(event->type == GDK_2BUTTON_PRESS) {
GtkTreeModel *model;
GtkTreeIter iter;
field_info *fi;
if(gtk_tree_selection_get_selected (sel, &model, &iter)) {
if (event->state & GDK_SHIFT_MASK) {
new_packet_window(NULL, TRUE, FALSE);
}
else {
gtk_tree_model_get(model, &iter, 1, &fi, -1);
tree_view_follow_link(fi);
}
}
}
else if (((GdkEventButton *)event)->button != 1) {
gtk_tree_selection_select_path(sel, path);
}
} else {
return FALSE;
}
return TRUE;
}
void
proto_tree_draw_resolve(proto_tree *protocol_tree, GtkWidget *tree_view, const e_addr_resolve *resolv)
{
ProtoTreeModel *model;
GtkTreePath *path;
GtkTreeIter iter;
model = proto_tree_model_new(protocol_tree, prefs.display_hidden_proto_items);
if (resolv)
proto_tree_model_force_resolv(PROTO_TREE_MODEL(model), resolv);
gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view), GTK_TREE_MODEL(model));
g_signal_handlers_block_by_func(tree_view, expand_tree, NULL);
path = gtk_tree_path_new_first();
if (gtk_tree_model_get_iter(GTK_TREE_MODEL(model), &iter, path))
check_expand_trees(GTK_TREE_VIEW(tree_view), GTK_TREE_MODEL(model),
path, &iter, prefs.gui_auto_scroll_on_expand, TRUE);
gtk_tree_path_free(path);
g_signal_handlers_unblock_by_func(tree_view, expand_tree, NULL);
g_object_unref(G_OBJECT(model));
}
void
proto_tree_draw(proto_tree *protocol_tree, GtkWidget *tree_view)
{
proto_tree_draw_resolve(protocol_tree, tree_view, NULL);
}
void
select_bytes_view (GtkWidget *w _U_, gpointer data _U_, gint view)
{
if (recent.gui_bytes_view != view) {
recent.gui_bytes_view = view;
redraw_packet_bytes_all();
}
}
