static void
dissect_dmx_chan(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
col_set_str(pinfo->cinfo, COL_PROTOCOL, "DMX Channels");
col_clear(pinfo->cinfo, COL_INFO);
if (tree != NULL) {
static const char *chan_format[] = {
"%2u%% ",
"0x%02x ",
"%3u "
};
static const char *string_format[] = {
"0x%03x: %s",
"%3u: %s"
};
emem_strbuf_t *chan_str = ep_strbuf_new_label("");
proto_item *item;
guint16 length,r,c,row_count;
guint8 v;
guint offset = 0;
proto_tree *ti = proto_tree_add_item(tree, proto_dmx_chan, tvb, offset, -1, ENC_NA);
proto_tree *dmx_chan_tree = proto_item_add_subtree(ti, ett_dmx_chan);
length = tvb_reported_length_remaining(tvb, offset);
row_count = (length / global_disp_col_count) + ((length % global_disp_col_count) == 0 ? 0 : 1);
for (r = 0; r < row_count;r++) {
for (c = 0;(c < global_disp_col_count) && (((r * global_disp_col_count) + c) < length);c++) {
if ((global_disp_col_count >= 2) && ((c % (global_disp_col_count / 2)) == 0)) {
ep_strbuf_append(chan_str, " ");
}
v = tvb_get_guint8(tvb, (offset + (r * global_disp_col_count) + c));
if (global_disp_chan_val_type == 0) {
v = (v * 100) / 255;
if (v == 100) {
ep_strbuf_append(chan_str, "FL ");
} else {
ep_strbuf_append_printf(chan_str, chan_format[global_disp_chan_val_type], v);
}
} else {
ep_strbuf_append_printf(chan_str, chan_format[global_disp_chan_val_type], v);
}
}
proto_tree_add_none_format(dmx_chan_tree, hf_dmx_chan_output_dmx_data, tvb,
offset+(r * global_disp_col_count), c,
string_format[global_disp_chan_nr_type],
(r * global_disp_col_count) + 1, chan_str->str);
}
item = proto_tree_add_item(dmx_chan_tree, hf_dmx_chan_output_data_filter, tvb,
offset, length, ENC_NA );
PROTO_ITEM_SET_HIDDEN(item);
}
}
void
proto_register_dmx_chan(void)
{
static hf_register_info hf[] = {
{ &hf_dmx_chan_output_data_filter,
{ "DMX data filter",
"dmx_chan.data_filter",
FT_BYTES, BASE_NONE, NULL, 0x0,
NULL, HFILL }},
{ &hf_dmx_chan_output_dmx_data,
{ "DMX data",
"dmx_chan.dmx_data",
FT_NONE, BASE_NONE, NULL, 0x0,
NULL, HFILL }},
};
static gint *ett[] = {
&ett_dmx_chan
};
module_t *dmx_chan_module;
static const enum_val_t disp_chan_val_types[] = {
{ "pro", "Percent", 0 },
{ "hex", "Hexadecimal", 1 },
{ "dec", "Decimal", 2 },
{ NULL, NULL, 0 }
};
static const enum_val_t disp_chan_nr_types[] = {
{ "hex", "Hexadecimal", 0 },
{ "dec", "Decimal", 1 },
{ NULL, NULL, 0 }
};
static const enum_val_t col_count[] = {
{ "6", "6", 6 },
{ "10", "10", 10 },
{ "12", "12", 12 },
{ "16", "16", 16 },
{ "24", "24", 24 },
{ NULL, NULL, 0 }
};
proto_dmx_chan = proto_register_protocol("DMX Channels","DMX Channels", "dmx-chan");
proto_register_field_array(proto_dmx_chan, hf, array_length(hf));
proto_register_subtree_array(ett, array_length(ett));
register_dissector("dmx-chan", dissect_dmx_chan, proto_dmx_chan);
dmx_chan_module = prefs_register_protocol(proto_dmx_chan, NULL);
prefs_register_enum_preference(dmx_chan_module, "dmx_disp_chan_val_type",
"DMX Display channel value type",
"The way DMX values are displayed",
&global_disp_chan_val_type,
disp_chan_val_types, FALSE);
prefs_register_enum_preference(dmx_chan_module, "dmx_disp_chan_nr_type",
"DMX Display channel nr. type",
"The way DMX channel numbers are displayed",
&global_disp_chan_nr_type,
disp_chan_nr_types, FALSE);
prefs_register_enum_preference(dmx_chan_module, "dmx_disp_col_count",
"DMX Display Column Count",
"The number of columns for the DMX display",
&global_disp_col_count,
col_count, FALSE);
}
