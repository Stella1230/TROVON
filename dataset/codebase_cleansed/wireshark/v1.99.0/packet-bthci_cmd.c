static void bthci_cmd_vendor_prompt(packet_info *pinfo _U_, gchar* result)
{
g_snprintf(result, MAX_DECODE_AS_PROMPT_LEN, "Vendor as");
}
static gpointer bthci_cmd_vendor_value(packet_info *pinfo _U_)
{
return NULL;
}
static void
save_local_device_name(tvbuff_t *tvb, gint offset, packet_info *pinfo,
guint8 size, hci_data_t *hci_data)
{
gint i = 0;
guint8 length;
wmem_tree_key_t key[4];
guint32 k_interface_id;
guint32 k_adapter_id;
guint32 k_frame_number;
gchar *name;
localhost_name_entry_t *localhost_name_entry;
if (!(!pinfo->fd->flags.visited && hci_data)) return;
while (i < size) {
length = tvb_get_guint8(tvb, offset + i);
if (length == 0) break;
switch(tvb_get_guint8(tvb, offset + i + 1)) {
case 0x08:
case 0x09:
name = tvb_get_string_enc(wmem_packet_scope(), tvb, offset + i + 2, length - 1, ENC_ASCII);
k_interface_id = hci_data->interface_id;
k_adapter_id = hci_data->adapter_id;
k_frame_number = pinfo->fd->num;
key[0].length = 1;
key[0].key = &k_interface_id;
key[1].length = 1;
key[1].key = &k_adapter_id;
key[2].length = 1;
key[2].key = &k_frame_number;
key[3].length = 0;
key[3].key = NULL;
localhost_name_entry = (localhost_name_entry_t *) wmem_new(wmem_file_scope(), localhost_name_entry_t);
localhost_name_entry->interface_id = k_interface_id;
localhost_name_entry->adapter_id = k_adapter_id;
localhost_name_entry->name = wmem_strdup(wmem_file_scope(), name);
wmem_tree_insert32_array(hci_data->localhost_name, key, localhost_name_entry);
break;
}
i += length + 1;
}
}
static int
dissect_bthci_cmd_cod_mask(tvbuff_t *tvb, int offset, packet_info *pinfo _U_, proto_tree *tree)
{
proto_item *cod_mask_item;
proto_item *cod_mask_tree;
cod_mask_item = proto_tree_add_item(tree, hf_bthci_cmd_cod_class_of_device_mask, tvb, offset, 3, ENC_LITTLE_ENDIAN);
cod_mask_tree = proto_item_add_subtree(cod_mask_item, ett_cod_mask);
proto_tree_add_item(cod_mask_tree, hf_bthci_cmd_cod_minor_device_class_mask, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_mask_tree, hf_bthci_cmd_cod_format_type_mask, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset += 1;
proto_tree_add_item(cod_mask_tree, hf_bthci_cmd_cod_major_service_class_information_mask, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_mask_tree, hf_bthci_cmd_cod_major_service_class_telephony_mask, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_mask_tree, hf_bthci_cmd_cod_major_service_class_audio_mask, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_mask_tree, hf_bthci_cmd_cod_major_service_class_object_transfer_mask, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_mask_tree, hf_bthci_cmd_cod_major_service_class_capturing_mask, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_mask_tree, hf_bthci_cmd_cod_major_service_class_rendering_mask, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_mask_tree, hf_bthci_cmd_cod_major_service_class_networking_mask, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_mask_tree, hf_bthci_cmd_cod_major_service_class_positioning_mask, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_mask_tree, hf_bthci_cmd_cod_major_service_class_reserved_mask, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_mask_tree, hf_bthci_cmd_cod_major_service_class_limited_discoverable_mode_mask, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_mask_tree, hf_bthci_cmd_cod_major_device_class_mask, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset += 2;
return offset;
}
static int
dissect_bthci_cmd_flow_spec(tvbuff_t *tvb, int offset, packet_info *pinfo _U_, proto_tree *tree, gboolean tx)
{
proto_item *ti_flow_spec;
proto_tree *ti_flow_spec_subtree;
ti_flow_spec = proto_tree_add_none_format(tree, hf_bthci_cmd_flow_spec, tvb, offset, 16, tx?"Tx Flow Spec ":"Rx Flow Spec");
ti_flow_spec_subtree = proto_item_add_subtree(ti_flow_spec, ett_flow_spec_subtree);
proto_tree_add_item(ti_flow_spec_subtree, hf_bthci_cmd_flow_spec_identifier, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(ti_flow_spec_subtree, hf_bthci_cmd_flow_spec_service_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(ti_flow_spec_subtree, hf_bthci_cmd_flow_spec_sdu_size, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(ti_flow_spec_subtree, hf_bthci_cmd_flow_spec_sdu_arrival_time, tvb, offset, 4, ENC_LITTLE_ENDIAN);
offset+=4;
proto_tree_add_item(ti_flow_spec_subtree, hf_bthci_cmd_flow_spec_access_latency, tvb, offset, 4, ENC_LITTLE_ENDIAN);
offset+=4;
proto_tree_add_item(ti_flow_spec_subtree, hf_bthci_cmd_flush_to_us, tvb, offset, 4, ENC_LITTLE_ENDIAN);
offset+=4;
return offset;
}
static int
dissect_link_control_cmd(tvbuff_t *tvb, int offset, packet_info *pinfo, proto_tree *tree, guint16 cmd_ocf)
{
proto_item *item;
guint32 clock_value;
switch (cmd_ocf) {
case 0x0001:
proto_tree_add_item(tree, hf_bthci_cmd_lap, tvb, offset, 3, ENC_LITTLE_ENDIAN);
offset+=3;
item = proto_tree_add_item(tree, hf_bthci_cmd_inq_length, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g sec)", 1.28*tvb_get_guint8(tvb, offset));
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_num_responses, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0003:
item = proto_tree_add_item(tree, hf_bthci_cmd_max_period_length, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g sec)", 1.28*tvb_get_letohs(tvb, offset));
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_min_period_length, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g sec)", 1.28*tvb_get_letohs(tvb, offset));
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_lap, tvb, offset, 3, ENC_LITTLE_ENDIAN);
offset+=3;
item = proto_tree_add_item(tree, hf_bthci_cmd_inq_length, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g sec)", 1.28*tvb_get_guint8(tvb, offset));
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_num_responses, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0005:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_2dh1, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_3dh1, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_dm1, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_dh1, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_2dh3, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_3dh3, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_dm3, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_dh3, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_2dh5, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_3dh5, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_dm5, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_dh5, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_page_scan_repetition_mode, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_page_scan_mode, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
item = proto_tree_add_item(tree, hf_bthci_cmd_clock_offset, tvb, offset, 2, ENC_LITTLE_ENDIAN);
clock_value = tvb_get_letohs(tvb, 13) & 32767;
proto_item_append_text(item, " (%g msec)", 1.25*clock_value);
proto_tree_add_item(tree, hf_bthci_cmd_clock_offset_valid , tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_allow_role_switch, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0006:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_reason, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0007:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_hv1, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_hv2, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_hv3, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x0008:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
break;
case 0x0009:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_role, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x000a:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_reason, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x000b:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_link_key, tvb, offset, 16, ENC_NA);
offset+=16;
break;
case 0x000c:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
break;
case 0x000d:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_pin_code_length ,tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_pin_code ,tvb, offset, 16, ENC_ASCII|ENC_NA);
offset+=16;
break;
case 0x000e:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
break;
case 0x000f:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_2dh1, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_3dh1, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_dm1, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_dh1, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_2dh3, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_3dh3, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_dm3, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_dh3, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_2dh5, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_3dh5, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_dm5, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_packet_type_dh5, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x0011:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x0013:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_encryption_enable, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0017:
proto_tree_add_item(tree, hf_bthci_cmd_key_flag, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0019:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_page_scan_repetition_mode, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_page_scan_mode, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
item = proto_tree_add_item(tree, hf_bthci_cmd_clock_offset, tvb, offset, 2, ENC_LITTLE_ENDIAN);
clock_value = tvb_get_letohs(tvb, offset) & 32767;
proto_item_append_text(item, " (%g msec)", 1.25*clock_value);
proto_tree_add_item(tree, hf_bthci_cmd_clock_offset_valid , tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x001a:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
break;
case 0x001c:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_page_number, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0028:
case 0x0029:
if (cmd_ocf == 0x0028) {
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
} else {
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
}
proto_tree_add_item(tree, hf_bthci_cmd_transmit_bandwidth, tvb, offset, 4, ENC_LITTLE_ENDIAN);
offset+=4;
proto_tree_add_item(tree, hf_bthci_cmd_receive_bandwidth, tvb, offset, 4, ENC_LITTLE_ENDIAN);
offset+=4;
proto_tree_add_item(tree, hf_bthci_cmd_max_latency_ms, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_input_unused, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_input_coding, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_input_data_format, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_input_sample_size, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_linear_pcm_bit_pos, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_air_coding_format, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_retransmission_effort, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_sco_packet_type_hv1, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_sco_packet_type_hv2, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_sco_packet_type_hv3, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_sco_packet_type_ev3, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_sco_packet_type_ev4, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_sco_packet_type_ev5, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_sco_packet_type_2ev3, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_sco_packet_type_3ev3, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_sco_packet_type_2ev5, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_sco_packet_type_3ev5, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x002a:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_reason, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0015:
case 0x001b:
case 0x001d:
case 0x001f:
case 0x0020:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x002b:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_io_capability, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_oob_data_present, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_auth_requirements, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0034:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_reason, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x002c:
case 0x002d:
case 0x002f:
case 0x0033:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
break;
case 0x002e:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_passkey, tvb, offset, 4, ENC_LITTLE_ENDIAN);
offset+=4;
break;
case 0x0030:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_hash_c, tvb, offset, 16, ENC_NA);
offset+=16;
proto_tree_add_item(tree, hf_bthci_cmd_randomizer_r, tvb, offset, 16, ENC_NA);
offset+=16;
break;
case 0x0035:
case 0x0036:
proto_tree_add_item(tree, hf_bthci_cmd_physical_link_handle, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_dedicated_amp_key_length, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_dedicated_amp_key_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_dedicated_amp_key, tvb, offset, -1, ENC_NA);
offset+=tvb_length_remaining(tvb, offset);
break;
case 0x0037:
proto_tree_add_item(tree, hf_bthci_cmd_physical_link_handle, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_reason, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0038:
case 0x0039:
proto_tree_add_item(tree, hf_bthci_cmd_physical_link_handle, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
offset = dissect_bthci_cmd_flow_spec(tvb, offset, pinfo, tree, TRUE);
offset = dissect_bthci_cmd_flow_spec(tvb, offset, pinfo, tree, FALSE);
break;
case 0x003a:
proto_tree_add_item(tree, hf_bthci_cmd_logical_link_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x003b:
proto_tree_add_item(tree, hf_bthci_cmd_physical_link_handle, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_flow_spec_identifier, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x003c:
proto_tree_add_item(tree, hf_bthci_cmd_logical_link_handle, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
offset = dissect_bthci_cmd_flow_spec(tvb, offset, pinfo, tree, TRUE);
offset = dissect_bthci_cmd_flow_spec(tvb, offset, pinfo, tree, FALSE);
break;
case 0x0002:
case 0x0004:
break;
case 0x003D:
case 0x003E:
case 0x003F:
case 0x0040:
case 0x0041:
case 0x0042:
case 0x0043:
case 0x0044:
proto_tree_add_expert(tree, pinfo, &ei_command_undecoded, tvb, offset, -1);
offset += tvb_length_remaining(tvb, offset);
break;
}
return offset;
}
static int
dissect_link_policy_cmd(tvbuff_t *tvb, int offset, packet_info *pinfo _U_, proto_tree *tree, guint16 cmd_ocf)
{
proto_item *item;
guint16 timeout;
switch (cmd_ocf) {
case 0x0001:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_max_interval_hold, tvb, offset, 2, ENC_LITTLE_ENDIAN);
timeout = tvb_get_letohs(tvb, offset);
proto_item_append_text(item, " slots (%g msec)", timeout*0.625);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_min_interval_hold, tvb, offset, 2, ENC_LITTLE_ENDIAN);
timeout = tvb_get_letohs(tvb, offset);
proto_item_append_text(item, " slots (%g msec)", timeout*0.625);
offset+=2;
break;
case 0x0003:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_max_interval_sniff, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " slots (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_min_interval_sniff, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " slots (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_sniff_attempt, tvb, offset, 2, ENC_LITTLE_ENDIAN);
timeout = tvb_get_letohs(tvb, offset);
if (timeout>0) {
proto_item_append_text(item, " (%g msec)", (2*timeout-1)*0.625);
} else {
proto_item_append_text(item, " (0 msec)");
}
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_timeout, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " slots (%g msec)", tvb_get_letohs(tvb, offset)*1.25);
offset+=2;
break;
case 0x0005:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_max_interval_beacon, tvb, offset, 2, ENC_LITTLE_ENDIAN);
timeout = tvb_get_letohs(tvb, offset);
proto_item_append_text(item, " slots (%g msec)", timeout*0.625);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_min_interval_beacon, tvb, offset, 2, ENC_LITTLE_ENDIAN);
timeout = tvb_get_letohs(tvb, offset);
proto_item_append_text(item, " slots (%g msec)", timeout*0.625);
offset+=2;
break;
case 0x0007:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_flags, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_service_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_token_rate, tvb, offset, 4, ENC_LITTLE_ENDIAN);
offset+=4;
proto_tree_add_item(tree, hf_bthci_cmd_peak_bandwidth, tvb, offset, 4, ENC_LITTLE_ENDIAN);
offset+=4;
proto_tree_add_item(tree, hf_bthci_cmd_latency, tvb, offset, 4, ENC_LITTLE_ENDIAN);
offset+=4;
proto_tree_add_item(tree, hf_bthci_cmd_delay_variation, tvb, offset, 4, ENC_LITTLE_ENDIAN);
offset+=4;
break;
case 0x000b:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_role, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0004:
case 0x0006:
case 0x0009:
case 0x000c:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x000d:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
case 0x000f:
proto_tree_add_item(tree, hf_bthci_cmd_link_policy_setting_switch, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_link_policy_setting_hold , tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_link_policy_setting_sniff , tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_link_policy_setting_park , tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x0010:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_flags, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_service_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_token_rate, tvb, offset, 4, ENC_LITTLE_ENDIAN);
offset+=4;
proto_tree_add_item(tree, hf_bthci_cmd_token_bucket_size, tvb, offset, 4, ENC_LITTLE_ENDIAN);
offset+=4;
proto_tree_add_item(tree, hf_bthci_cmd_peak_bandwidth, tvb, offset, 4, ENC_LITTLE_ENDIAN);
offset+=4;
proto_tree_add_item(tree, hf_bthci_cmd_latency, tvb, offset, 4, ENC_LITTLE_ENDIAN);
offset+=4;
break;
case 0x0011:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_max_latency, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " slots (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_min_remote_timeout, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " slots (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_min_local_timeout, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " slots (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
break;
case 0x00e:
break;
}
return offset;
}
static int
dissect_host_controller_baseband_cmd(tvbuff_t *tvb, int offset, packet_info *pinfo,
proto_tree *tree, guint16 cmd_ocf, hci_data_t *hci_data)
{
proto_item *item;
guint16 timeout;
guint8 filter_type, filter_condition_type, num8;
int i;
switch (cmd_ocf) {
case 0x0001:
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_00, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_01, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_02, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_03, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_04, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_05, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_06, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_07, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_10, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_11, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_12, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_13, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_14, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_17, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_20, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_21, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_23, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_24, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_25, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_26, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_27, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_30, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_31, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_32, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_33, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_34, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_35, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_36, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_37, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_40, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_41, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_42, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_53, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_54, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_55, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_56, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_57, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_60, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_61, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_62, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_63, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_64, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_65, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_67, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_70, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_72, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask_73, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0005:
proto_tree_add_item(tree, hf_bthci_cmd_filter_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
filter_type = tvb_get_guint8(tvb, 3);
offset++;
switch (filter_type) {
case 0x01:
proto_tree_add_item(tree, hf_bthci_cmd_inquiry_result_filter_condition_type,
tvb, offset, 1, ENC_LITTLE_ENDIAN);
filter_condition_type = tvb_get_guint8(tvb, offset);
offset++;
switch (filter_condition_type) {
case 0x01:
call_dissector(btcommon_cod_handle, tvb_new_subset_length(tvb, offset, 3), pinfo, tree);
offset += 3;
offset=dissect_bthci_cmd_cod_mask(tvb, offset, pinfo, tree);
break;
case 0x02:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
break;
default:
break;
}
break;
case 0x02:
proto_tree_add_item(tree, hf_bthci_cmd_connection_setup_filter_condition_type,
tvb, offset, 1, ENC_LITTLE_ENDIAN);
filter_condition_type = tvb_get_guint8(tvb, offset);
offset++;
switch (filter_condition_type) {
case 0x00:
proto_tree_add_item(tree, hf_bthci_cmd_auto_acc_flag, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x01:
call_dissector(btcommon_cod_handle, tvb_new_subset_length(tvb, offset, 3), pinfo, tree);
offset += 3;
offset=dissect_bthci_cmd_cod_mask(tvb, offset, pinfo, tree);
proto_tree_add_item(tree, hf_bthci_cmd_auto_acc_flag, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x02:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_auto_acc_flag, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
default:
break;
}
break;
default:
break;
}
break;
case 0x000a:
proto_tree_add_item(tree, hf_bthci_cmd_pin_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x000d:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_read_all_flag, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0011:
proto_tree_add_item(tree, hf_bthci_cmd_num_link_keys, tvb, offset, 1, ENC_LITTLE_ENDIAN);
num8 = tvb_get_guint8(tvb, offset);
offset += 1;
for (i = 0; i < num8; i++) {
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_link_key, tvb, offset, 16, ENC_NA);
offset += 16;
}
break;
case 0x0012:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_delete_all_flag, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0013:
proto_tree_add_item(tree, hf_bthci_cmd_device_name, tvb, offset, 248, ENC_ASCII | ENC_NA);
if (!pinfo->fd->flags.visited) {
wmem_tree_key_t key[4];
guint32 k_interface_id;
guint32 k_adapter_id;
guint32 k_frame_number;
gchar *name;
localhost_name_entry_t *localhost_name_entry;
k_interface_id = hci_data->interface_id;
k_adapter_id = hci_data->adapter_id;
k_frame_number = pinfo->fd->num;
name = tvb_get_string_enc(wmem_packet_scope(), tvb, offset, 248, ENC_ASCII);
key[0].length = 1;
key[0].key = &k_interface_id;
key[1].length = 1;
key[1].key = &k_adapter_id;
key[2].length = 1;
key[2].key = &k_frame_number;
key[3].length = 0;
key[3].key = NULL;
localhost_name_entry = (localhost_name_entry_t *) wmem_new(wmem_file_scope(), localhost_name_entry_t);
localhost_name_entry->interface_id = k_interface_id;
localhost_name_entry->adapter_id = k_adapter_id;
localhost_name_entry->name = wmem_strdup(wmem_file_scope(), name);
wmem_tree_insert32_array(hci_data->localhost_name, key, localhost_name_entry);
}
offset += 248;
break;
case 0x0016:
item = proto_tree_add_item(tree, hf_bthci_cmd_timeout, tvb, offset, 2, ENC_LITTLE_ENDIAN);
timeout = tvb_get_letohs(tvb, offset);
proto_item_append_text(item, " slots (%g msec)", timeout*0.625);
offset+=2;
break;
case 0x0018:
item = proto_tree_add_item(tree, hf_bthci_cmd_timeout, tvb, offset, 2, ENC_LITTLE_ENDIAN);
timeout = tvb_get_letohs(tvb, offset);
if (timeout > 0) {
proto_item_append_text(item, " slots (%g msec)", timeout*0.625);
} else {
proto_item_append_text(item, " Illegal Page Timeout");
}
offset+=2;
break;
case 0x001a:
proto_tree_add_item(tree, hf_bthci_cmd_scan_enable,
tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0020:
proto_tree_add_item(tree, hf_bthci_cmd_authentication_enable,
tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0022:
proto_tree_add_item(tree, hf_bthci_cmd_encrypt_mode, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0024:
call_dissector(btcommon_cod_handle, tvb_new_subset_length(tvb, offset, 3), pinfo, tree);
offset += 3;
break;
case 0x0026:
proto_tree_add_item(tree, hf_bthci_cmd_input_unused,
tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_input_coding,
tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_input_data_format,
tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_input_sample_size,
tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_linear_pcm_bit_pos,
tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_air_coding_format,
tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x0028:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_timeout, tvb, offset, 2, ENC_LITTLE_ENDIAN);
timeout = tvb_get_letohs(tvb, offset);
if (timeout>0) {
proto_item_append_text(item, " slots (%g msec)", timeout*0.625);
} else {
proto_item_append_text(item, " (= No Automatic Flush )");
}
offset+=2;
break;
case 0x002a:
proto_tree_add_item(tree, hf_bthci_cmd_num_broadcast_retransmissions,
tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x002c:
proto_tree_add_item(tree, hf_bthci_cmd_hold_mode_act_page,
tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_hold_mode_act_inquiry,
tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_hold_mode_act_periodic,
tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x002d:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle,
tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_tx_power_level_type,
tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x002f:
proto_tree_add_item(tree, hf_bthci_cmd_sco_flow_control,
tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0031:
proto_tree_add_item(tree, hf_bthci_cmd_flow_contr_enable,
tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0033:
proto_tree_add_item(tree, hf_bthci_cmd_host_data_packet_length_acl,
tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_host_data_packet_length_sco,
tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_host_total_num_acl_data_packets,
tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_host_total_num_sco_data_packets,
tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x0035:
proto_tree_add_item(tree, hf_bthci_cmd_num_handles,
tvb, offset, 1, ENC_LITTLE_ENDIAN);
num8 = tvb_get_guint8(tvb, offset);
offset++;
for (i=0; i<num8; i++) {
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle,
tvb, offset+(i*4), 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_num_compl_packets,
tvb, offset+2+(i*4), 2, ENC_LITTLE_ENDIAN);
}
break;
case 0x0037:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle,
tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_timeout,
tvb, offset, 2, ENC_LITTLE_ENDIAN);
timeout = tvb_get_letohs(tvb, offset);
if (timeout>0) {
proto_item_append_text(item, " slots (%g msec)", timeout*0.625);
} else {
proto_item_append_text(item, " (= No Link Supervision Timeout)");
}
offset+=2;
break;
case 0x003a:
proto_tree_add_item(tree, hf_bthci_cmd_num_curr_iac, tvb, offset, 1, ENC_LITTLE_ENDIAN);
num8 = tvb_get_guint8(tvb, offset);
offset++;
for (i=0; i<num8; i++) {
proto_tree_add_item(tree, hf_bthci_cmd_iac_lap, tvb, offset+(i*3), 3, ENC_LITTLE_ENDIAN);
}
offset += num8 * 3;
break;
case 0x003c:
proto_tree_add_item(tree, hf_bthci_cmd_page_scan_period_mode,
tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x003e:
proto_tree_add_item(tree, hf_bthci_cmd_page_scan_mode,
tvb, 3, 1, ENC_LITTLE_ENDIAN);
break;
case 0x003f:
proto_tree_add_item(tree, hf_bthci_cmd_afh_ch_classification, tvb, offset, 10, ENC_NA);
offset+=10;
break;
case 0x0008:
case 0x0027:
case 0x0036:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x001c:
case 0x001e:
item = proto_tree_add_item(tree, hf_bthci_cmd_interval, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " slots (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_window, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " slots (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
break;
case 0x0043:
proto_tree_add_item(tree, hf_bthci_cmd_scan_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0045:
proto_tree_add_item(tree, hf_bthci_cmd_inq_mode, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0047:
proto_tree_add_item(tree, hf_bthci_cmd_scan_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0049:
proto_tree_add_item(tree, hf_bthci_cmd_afh_ch_assessment_mode, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0052:
proto_tree_add_item(tree, hf_bthci_cmd_fec_required, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
call_dissector(btcommon_eir_handle, tvb_new_subset_length(tvb, offset, 240), pinfo, tree);
save_local_device_name(tvb, offset, pinfo, 240, hci_data);
offset += 240;
break;
case 0x0053:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x0056:
proto_tree_add_item(tree, hf_bthci_cmd_simple_pairing_mode, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0059:
proto_tree_add_item(tree, hf_bthci_cmd_tx_power, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x005b:
proto_tree_add_item(tree, hf_bthci_cmd_err_data_reporting, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x005f:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_flush_packet_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0060:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_notification_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0062:
item = proto_tree_add_item(tree, hf_bthci_cmd_timeout, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " slots (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
break;
case 0x0063:
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask2_00, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask2_01, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask2_02, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask2_03, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask2_04, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask2_05, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask2_06, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask2_07, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask2_10, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask2_11, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask2_12, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask2_13, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask2_14, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_evt_mask2_15, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset+=7;
break;
case 0x0065:
proto_tree_add_item(tree, hf_bthci_cmd_location_domain_aware, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_location_domain, tvb, offset, 2, ENC_ASCII | ENC_NA);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_location_domain_options, tvb, offset, 1, ENC_ASCII | ENC_NA);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_location_options, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0067:
proto_tree_add_item(tree, hf_bthci_cmd_flow_control_mode, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0068:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_tx_power_level_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0069:
proto_tree_add_item(tree, hf_bthci_cmd_logical_link_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x006a:
proto_tree_add_item(tree, hf_bthci_cmd_logical_link_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_flush_to_us, tvb, offset, 4, ENC_LITTLE_ENDIAN);
offset+=4;
break;
case 0x006b:
proto_tree_add_item(tree, hf_bthci_cmd_physical_link_handle, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_short_range_mode, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x006d:
proto_tree_add_item(tree, hf_bthci_cmd_le_supported_host, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_le_simultaneous_host, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x003:
case 0x009:
case 0x00B:
case 0x014:
case 0x015:
case 0x017:
case 0x019:
case 0x01B:
case 0x01D:
case 0x01F:
case 0x021:
case 0x023:
case 0x025:
case 0x029:
case 0x02B:
case 0x02E:
case 0x038:
case 0x039:
case 0x03B:
case 0x03D:
case 0x042:
case 0x044:
case 0x046:
case 0x048:
case 0x051:
case 0x055:
case 0x057:
case 0x058:
case 0x05A:
case 0x061:
case 0x064:
case 0x066:
case 0x06C:
break;
case 0x06E:
case 0x06F:
case 0x070:
case 0x071:
case 0x072:
case 0x073:
case 0x074:
case 0x075:
case 0x076:
case 0x077:
case 0x078:
proto_tree_add_expert(tree, pinfo, &ei_command_undecoded, tvb, offset, -1);
offset += tvb_length_remaining(tvb, offset);
break;
}
return offset;
}
static int
dissect_informational_parameters_cmd(tvbuff_t *tvb, int offset, packet_info *pinfo _U_,
proto_tree *tree, guint16 cmd_ocf)
{
switch (cmd_ocf) {
case 0x0004:
proto_tree_add_item(tree, hf_bthci_cmd_page_number, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x001:
case 0x002:
case 0x003:
case 0x005:
case 0x007:
case 0x009:
case 0x00A:
case 0x00B:
break;
}
return offset;
}
static int
dissect_status_parameters_cmd(tvbuff_t *tvb, int offset, packet_info *pinfo _U_,
proto_tree *tree, guint16 cmd_ocf)
{
switch (cmd_ocf) {
case 0x0001:
case 0x0002:
case 0x0003:
case 0x0005:
case 0x0006:
case 0x0008:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x0007:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_which_clock, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0009:
break;
case 0x000a:
proto_tree_add_item(tree, hf_bthci_cmd_physical_link_handle, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_length_so_far, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_amp_assoc_length, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x000b:
proto_tree_add_item(tree, hf_bthci_cmd_physical_link_handle, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_length_so_far, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_amp_remaining_assoc_length, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_amp_assoc_fragment, tvb, offset, -1, ENC_NA);
offset+=tvb_length_remaining(tvb, offset);
break;
case 0x00D:
break;
case 0x00C:
proto_tree_add_expert(tree, pinfo, &ei_command_undecoded, tvb, offset, -1);
offset += tvb_length_remaining(tvb, offset);
break;
}
return offset;
}
static int
dissect_testing_cmd(tvbuff_t *tvb, int offset, packet_info *pinfo _U_, proto_tree *tree, guint16 cmd_ocf)
{
switch (cmd_ocf) {
case 0x0002:
proto_tree_add_item(tree, hf_bthci_cmd_loopback_mode, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0004:
proto_tree_add_item(tree, hf_bthci_cmd_simple_pairing_debug_mode, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0007:
proto_tree_add_item(tree, hf_bthci_cmd_enable_amp_recv_reports, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_amp_recv_report_interval, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x001:
case 0x003:
case 0x008:
break;
case 0x009:
proto_tree_add_expert(tree, pinfo, &ei_command_undecoded, tvb, offset, -1);
offset += tvb_length_remaining(tvb, offset);
break;
}
return offset;
}
static gint
dissect_le_cmd(tvbuff_t *tvb, int offset, packet_info *pinfo, proto_tree *tree, guint16 cmd_ocf, hci_data_t *hci_data)
{
proto_item *item;
proto_item *sub_item;
proto_tree *sub_tree;
switch(cmd_ocf) {
case 0x0001:
proto_tree_add_item(tree, hf_bthci_cmd_le_evt_mask_00, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_le_evt_mask_01, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_le_evt_mask_02, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_le_evt_mask_03, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_le_evt_mask_04, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset+=8;
break;
case 0x0005:
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
break;
case 0x0006:
item = proto_tree_add_item(tree, hf_bthci_cmd_le_advts_interval_min, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_le_advts_interval_max, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_le_advts_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_le_own_address_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_le_direct_address_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_le_advts_channel_map_1, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_le_advts_channel_map_2, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(tree, hf_bthci_cmd_le_advts_channel_map_3, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_le_advts_filter_policy, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x0008:
case 0x0009:
proto_tree_add_item(tree, hf_bthci_cmd_le_data_length, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
call_dissector(btcommon_ad_handle, tvb_new_subset_length(tvb, offset, 31), pinfo, tree);
save_local_device_name(tvb, offset, pinfo, 31, hci_data);
offset += 31;
break;
case 0x000a:
proto_tree_add_item(tree, hf_bthci_cmd_le_advts_enable, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x000b:
proto_tree_add_item(tree, hf_bthci_cmd_le_scan_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
item = proto_tree_add_item(tree, hf_bthci_cmd_le_scan_interval, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_le_scan_window, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_le_own_address_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_le_scan_filter_policy, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x000c:
proto_tree_add_item(tree, hf_bthci_cmd_le_scan_enable, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_le_filter_dublicates, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x000d:
item = proto_tree_add_item(tree, hf_bthci_cmd_le_scan_interval, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_le_scan_window, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_le_initiator_filter_policy, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_le_peer_address_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
proto_tree_add_item(tree, hf_bthci_cmd_le_own_address_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
item = proto_tree_add_item(tree, hf_bthci_cmd_le_con_interval_min, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g msec)", tvb_get_letohs(tvb, offset)*1.25);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_le_con_interval_max, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g msec)", tvb_get_letohs(tvb, offset)*1.25);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_le_con_latency, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (number events)");
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_le_supervision_timeout, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g sec)", tvb_get_letohs(tvb, offset)*0.01);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_le_min_ce_length, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_le_max_ce_length, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
break;
case 0x0011:
case 0x0012:
proto_tree_add_item(tree, hf_bthci_cmd_le_address_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
offset = dissect_bd_addr(hf_bthci_cmd_bd_addr, tree, tvb, offset);
break;
case 0x0013:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_le_con_interval_min, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g msec)", tvb_get_letohs(tvb, offset)*1.25);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_le_con_interval_max, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g msec)", tvb_get_letohs(tvb, offset)*1.25);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_le_con_latency, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (number events)");
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_le_supervision_timeout, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g sec)", tvb_get_letohs(tvb, offset)*0.01);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_le_min_ce_length, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
item = proto_tree_add_item(tree, hf_bthci_cmd_le_max_ce_length, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%g msec)", tvb_get_letohs(tvb, offset)*0.625);
offset+=2;
break;
case 0x0014:
sub_item = proto_tree_add_item(tree, hf_bthci_cmd_le_channel_map, tvb, offset, 5, ENC_NA);
sub_tree = proto_item_add_subtree(sub_item, ett_le_channel_map);
call_dissector(btcommon_le_channel_map_handle, tvb_new_subset_length(tvb, offset, 5), pinfo, sub_tree);
offset += 5;
break;
case 0x0015:
case 0x0016:
case 0x001b:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
break;
case 0x0017:
proto_tree_add_item(tree, hf_bthci_cmd_key, tvb, offset, 16, ENC_NA);
offset+=16;
proto_tree_add_item(tree, hf_bthci_cmd_plaintext_data, tvb, offset, 16, ENC_NA);
offset+=16;
break;
case 0x0019:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_random_number, tvb, offset, 8, ENC_NA);
offset+=8;
proto_tree_add_item(tree, hf_bthci_cmd_encrypted_diversifier, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_le_long_term_key, tvb, offset, 16, ENC_NA);
offset+=16;
break;
case 0x001a:
proto_tree_add_item(tree, hf_bthci_cmd_connection_handle, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(tree, hf_bthci_cmd_le_long_term_key, tvb, offset, 16, ENC_NA);
offset+=16;
break;
case 0x001d:
item = proto_tree_add_item(tree, hf_bthci_cmd_rx_freqency, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%d MHz)", 2402 + 2*tvb_get_guint8(tvb, offset));
offset++;
break;
case 0x001e:
item = proto_tree_add_item(tree, hf_bthci_cmd_tx_freqency, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_item_append_text(item, " (%d MHz)", 2402 + 2*tvb_get_guint8(tvb, offset));
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_test_data_length, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
proto_tree_add_item(tree, hf_bthci_cmd_test_packet_payload, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset++;
break;
case 0x002:
case 0x003:
case 0x007:
case 0x00E:
case 0x00F:
case 0x010:
case 0x018:
case 0x01C:
case 0x01F:
break;
}
return offset;
}
static gint
dissect_bthci_cmd(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void *data)
{
proto_item *ti_cmd;
proto_tree *bthci_cmd_tree;
guint16 opcode;
guint16 ocf;
guint8 param_length;
guint8 ogf;
gint offset = 0;
proto_item *ti_opcode;
proto_tree *opcode_tree;
gint hfx;
hci_data_t *hci_data;
if (data == NULL)
return 0;
hci_data = (hci_data_t *) data;
ti_cmd = proto_tree_add_item(tree, proto_bthci_cmd, tvb, offset, -1, ENC_NA);
bthci_cmd_tree = proto_item_add_subtree(ti_cmd, ett_bthci_cmd);
switch (pinfo->p2p_dir) {
case P2P_DIR_SENT:
col_set_str(pinfo->cinfo, COL_INFO, "Sent ");
break;
case P2P_DIR_RECV:
col_set_str(pinfo->cinfo, COL_INFO, "Rcvd ");
break;
default:
col_add_fstr(pinfo->cinfo, COL_INFO, "Unknown direction %d ",
pinfo->p2p_dir);
break;
}
SET_ADDRESS(&pinfo->src, AT_STRINGZ, 5, "host");
SET_ADDRESS(&pinfo->dst, AT_STRINGZ, 11, "controller");
opcode = tvb_get_letohs(tvb, offset);
ocf = opcode & 0x03ff;
ogf = (guint8) (opcode >> 10);
proto_item_append_text(ti_cmd," - %s", val_to_str_ext(opcode, &bthci_cmd_opcode_vals_ext, "Unknown 0x%04x"));
col_set_str(pinfo->cinfo, COL_PROTOCOL, "HCI_CMD");
ti_opcode = proto_tree_add_item(bthci_cmd_tree, hf_bthci_cmd_opcode, tvb, offset, 2, ENC_LITTLE_ENDIAN);
opcode_tree = proto_item_add_subtree(ti_opcode, ett_opcode);
proto_tree_add_item(opcode_tree, hf_bthci_cmd_ogf, tvb, offset, 2, ENC_LITTLE_ENDIAN);
if (ogf == HCI_OGF_LINK_CONTROL)
hfx = hf_bthci_cmd_ocf_link_control;
else if (ogf == HCI_OGF_LINK_POLICY)
hfx = hf_bthci_cmd_ocf_link_policy;
else if (ogf == HCI_OGF_HOST_CONTROLLER)
hfx = hf_bthci_cmd_ocf_host_controller_and_baseband;
else if (ogf == HCI_OGF_INFORMATIONAL)
hfx = hf_bthci_cmd_ocf_informational;
else if (ogf == HCI_OGF_STATUS)
hfx = hf_bthci_cmd_ocf_status;
else if (ogf == HCI_OGF_TESTING)
hfx = hf_bthci_cmd_ocf_testing;
else if (ogf == HCI_OGF_LOW_ENERGY)
hfx = hf_bthci_cmd_ocf_low_energy;
else if (ogf == HCI_OGF_LOGO_TESTING)
hfx = hf_bthci_cmd_ocf_logo_testing;
else
hfx = hf_bthci_cmd_ocf;
proto_tree_add_item(opcode_tree, hfx, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset+=2;
proto_tree_add_item(bthci_cmd_tree, hf_bthci_cmd_param_length, tvb, offset, 1, ENC_LITTLE_ENDIAN);
param_length = tvb_get_guint8(tvb, offset);
offset++;
if (ogf == HCI_OGF_VENDOR_SPECIFIC) {
col_append_fstr(pinfo->cinfo, COL_INFO, "Vendor Command 0x%04X (opcode 0x%04X)", ocf, opcode);
dissector_try_uint_new(vendor_dissector_table, HCI_VENDOR_DEFAULT, tvb, pinfo, tree, TRUE, hci_data);
proto_tree_add_item(bthci_cmd_tree, hf_bthci_cmd_parameter, tvb, offset, tvb_captured_length_remaining(tvb, offset), ENC_NA);
return tvb_captured_length(tvb);
} else {
col_append_fstr(pinfo->cinfo, COL_INFO, "%s", val_to_str_ext(opcode, &bthci_cmd_opcode_vals_ext, "Unknown 0x%04x"));
}
if (param_length > 0) {
switch (ogf) {
case HCI_OGF_LINK_CONTROL:
offset = dissect_link_control_cmd(tvb, offset, pinfo, bthci_cmd_tree, ocf);
break;
case HCI_OGF_LINK_POLICY:
offset = dissect_link_policy_cmd(tvb, offset, pinfo, bthci_cmd_tree, ocf);
break;
case HCI_OGF_HOST_CONTROLLER:
offset = dissect_host_controller_baseband_cmd(tvb, offset, pinfo, bthci_cmd_tree, ocf, hci_data);
break;
case HCI_OGF_INFORMATIONAL:
offset = dissect_informational_parameters_cmd(tvb, offset, pinfo, bthci_cmd_tree, ocf);
break;
case HCI_OGF_STATUS:
offset = dissect_status_parameters_cmd(tvb, offset, pinfo, bthci_cmd_tree, ocf);
break;
case HCI_OGF_TESTING:
offset = dissect_testing_cmd(tvb, offset, pinfo, bthci_cmd_tree, ocf);
break;
case HCI_OGF_LOW_ENERGY:
offset = dissect_le_cmd(tvb, offset, pinfo, bthci_cmd_tree, ocf, hci_data);
break;
default:
proto_tree_add_expert(bthci_cmd_tree, pinfo, &ei_command_unknown, tvb, 3, -1);
offset += tvb_length_remaining(tvb, offset);
break;
}
}
if (tvb_length_remaining(tvb, offset) > 0) {
proto_tree_add_expert(bthci_cmd_tree, pinfo, &ei_command_parameter_unexpected, tvb, offset, -1);
}
return offset;
}
void
proto_register_bthci_cmd(void)
{
module_t *module;
expert_module_t *expert_bthci_cmd;
guint i_opcode = 0;
guint i_array;
guint i_string_array;
static hf_register_info hf[] = {
{ &hf_bthci_cmd_opcode,
{ "Command Opcode", "bthci_cmd.opcode",
FT_UINT16, BASE_HEX|BASE_EXT_STRING, &bthci_cmd_opcode_vals_ext, 0x0,
"HCI Command Opcode", HFILL }
},
{ &hf_bthci_cmd_ogf,
{ "Opcode Group Field", "bthci_cmd.opcode.ogf",
FT_UINT16, BASE_HEX|BASE_EXT_STRING, &bthci_ogf_vals_ext, 0xfc00,
NULL, HFILL }
},
{ &hf_bthci_cmd_ocf_link_control,
{ "Opcode Command Field", "bthci_cmd.opcode.ocf",
FT_UINT16, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_ocf_link_control_vals_ext, 0x03ff,
NULL, HFILL }
},
{ &hf_bthci_cmd_ocf_link_policy,
{ "Opcode Command Field", "bthci_cmd.opcode.ocf",
FT_UINT16, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_ocf_link_policy_vals_ext, 0x03ff,
NULL, HFILL }
},
{ &hf_bthci_cmd_ocf_host_controller_and_baseband,
{ "Opcode Command Field", "bthci_cmd.opcode.ocf",
FT_UINT16, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_ocf_host_controller_and_baseband_vals_ext, 0x03ff,
NULL, HFILL }
},
{ &hf_bthci_cmd_ocf_informational,
{ "Opcode Command Field", "bthci_cmd.opcode.ocf",
FT_UINT16, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_ocf_informational_vals_ext, 0x03ff,
NULL, HFILL }
},
{ &hf_bthci_cmd_ocf_status,
{ "Opcode Command Field", "bthci_cmd.opcode.ocf",
FT_UINT16, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_ocf_status_vals_ext, 0x03ff,
NULL, HFILL }
},
{ &hf_bthci_cmd_ocf_testing,
{ "Opcode Command Field", "bthci_cmd.opcode.ocf",
FT_UINT16, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_ocf_testing_vals_ext, 0x03ff,
NULL, HFILL }
},
{ &hf_bthci_cmd_ocf_low_energy,
{ "Opcode Command Field", "bthci_cmd.opcode.ocf",
FT_UINT16, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_ocf_low_energy_vals_ext, 0x03ff,
NULL, HFILL }
},
{ &hf_bthci_cmd_ocf_logo_testing,
{ "Opcode Command Field", "bthci_cmd.opcode.ocf",
FT_UINT16, BASE_HEX, NULL, 0x03ff,
NULL, HFILL }
},
{ &hf_bthci_cmd_ocf,
{ "Opcode Command Field", "bthci_cmd.opcode.ocf",
FT_UINT16, BASE_HEX, NULL, 0x03ff,
NULL, HFILL }
},
{ &hf_bthci_cmd_param_length,
{ "Parameter Total Length", "bthci_cmd.param_length",
FT_UINT8, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_lap,
{ "LAP", "bthci_cmd.lap",
FT_UINT24, BASE_HEX, NULL, 0x0,
"LAP for the inquiry access code", HFILL }
},
{ &hf_bthci_cmd_inq_length,
{ "Inquiry Length", "bthci_cmd.inq_length",
FT_UINT8, BASE_DEC, NULL, 0x0,
"Inquiry Length (*1.28s)", HFILL }
},
{ &hf_bthci_cmd_num_responses,
{ "Num Responses", "bthci_cmd.num_responses",
FT_UINT8, BASE_DEC, NULL, 0x0,
"Number of Responses", HFILL }
},
{ &hf_bthci_cmd_encrypt_mode,
{ "Encryption Mode", "bthci_cmd.encrypt_mode",
FT_UINT8, BASE_HEX, VALS(cmd_encrypt_mode_vals), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_bd_addr,
{ "BD_ADDR", "bthci_cmd.bd_addr",
FT_ETHER, BASE_NONE, NULL, 0x0,
"Bluetooth Device Address", HFILL}
},
{ &hf_bthci_cmd_packet_type_2dh1,
{ "Packet Type 2-DH1", "bthci_cmd.packet_type_2dh1",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0002,
NULL, HFILL }
},
{ &hf_bthci_cmd_packet_type_3dh1,
{ "Packet Type 3-DH1", "bthci_cmd.packet_type_3dh1",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0004,
NULL, HFILL }
},
{ &hf_bthci_cmd_packet_type_dm1,
{ "Packet Type DM1", "bthci_cmd.packet_type_dm1",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0008,
NULL, HFILL }
},
{ &hf_bthci_cmd_packet_type_dh1,
{ "Packet Type DH1", "bthci_cmd.packet_type_dh1",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0010,
NULL, HFILL }
},
{ &hf_bthci_cmd_packet_type_2dh3,
{ "Packet Type 2-DH3", "bthci_cmd.packet_type_2dh3",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0100,
NULL, HFILL }
},
{ &hf_bthci_cmd_packet_type_3dh3,
{ "Packet Type 3-DH3", "bthci_cmd.packet_type_3dh3",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0200,
NULL, HFILL }
},
{ &hf_bthci_cmd_packet_type_dm3,
{ "Packet Type DM3", "bthci_cmd.packet_type_dm3",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0400,
NULL, HFILL }
},
{ &hf_bthci_cmd_packet_type_dh3,
{ "Packet Type DH3", "bthci_cmd.packet_type_dh3",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0800,
NULL, HFILL }
},
{ &hf_bthci_cmd_packet_type_2dh5,
{ "Packet Type 2-DH5", "bthci_cmd.packet_type_2dh5",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x1000,
NULL, HFILL }
},
{ &hf_bthci_cmd_packet_type_3dh5,
{ "Packet Type 3-DH5", "bthci_cmd.packet_type_3dh5",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x2000,
NULL, HFILL }
},
{ &hf_bthci_cmd_packet_type_dm5,
{ "Packet Type DM5", "bthci_cmd.packet_type_dm5",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x4000,
NULL, HFILL }
},
{ &hf_bthci_cmd_packet_type_dh5,
{ "Packet Type DH5", "bthci_cmd.packet_type_dh5",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x8000,
NULL, HFILL }
},
{ &hf_bthci_cmd_page_scan_mode,
{ "Page Scan Mode", "bthci_cmd.page_scan_mode",
FT_UINT8, BASE_HEX, VALS(bthci_cmd_page_scan_modes), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_page_scan_repetition_mode,
{ "Page Scan Repetition Mode", "bthci_cmd.page_scan_repetition_mode",
FT_UINT8, BASE_HEX, VALS(bthci_cmd_page_scan_repetition_modes), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_page_scan_period_mode,
{ "Page Scan Period Mode", "bthci_cmd.page_scan_period_mode",
FT_UINT8, BASE_HEX, VALS(bthci_cmd_page_scan_period_modes), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_clock_offset,
{ "Clock Offset", "bthci_cmd.clock_offset",
FT_UINT16, BASE_HEX, NULL, 0x7FFF,
"Bit 2-16 of the Clock Offset between CLKmaster-CLKslave", HFILL }
},
{ &hf_bthci_cmd_clock_offset_valid,
{ "Clock_Offset_Valid_Flag", "bthci_cmd.clock_offset_valid",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x8000,
"Indicates if clock offset is valid", HFILL }
},
{ &hf_bthci_cmd_allow_role_switch,
{ "Allow Role Switch", "bthci_cmd.allow_role_switch",
FT_UINT8, BASE_HEX, VALS(cmd_role_switch_modes), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_max_period_length,
{ "Max Period Length", "bthci_cmd.max_period_length",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Maximum amount of time specified between consecutive inquiries.", HFILL }
},
{ &hf_bthci_cmd_min_period_length,
{ "Min Period Length", "bthci_cmd.min_period_length",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Minimum amount of time specified between consecutive inquiries.", HFILL }
},
{ &hf_bthci_cmd_connection_handle,
{ "Connection Handle", "bthci_cmd.connection_handle",
FT_UINT16, BASE_HEX, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_reason,
{ "Reason", "bthci_cmd.reason",
FT_UINT8, BASE_HEX|BASE_EXT_STRING, &bthci_cmd_status_vals_ext, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_num_link_keys,
{ "Number of Link Keys", "bthci_cmd.num_link_keys",
FT_UINT8, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_link_key,
{ "Link Key", "bthci_cmd.link_key",
FT_BYTES, BASE_NONE, NULL, 0x0,
"Link Key for the associated BD_ADDR", HFILL }
},
{ &hf_bthci_cmd_packet_type_hv1,
{ "Packet Type HV1", "bthci_cmd.packet_type_hv1",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0020,
NULL, HFILL }
},
{ &hf_bthci_cmd_packet_type_hv2,
{ "Packet Type HV2", "bthci_cmd.packet_type_hv2",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0040,
NULL, HFILL }
},
{ &hf_bthci_cmd_packet_type_hv3,
{ "Packet Type HV3", "bthci_cmd.packet_type_hv3",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0080,
NULL, HFILL }
},
{ &hf_bthci_cmd_role,
{ "Role", "bthci_cmd.role",
FT_UINT8, BASE_HEX, VALS(cmd_role_vals), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_pin_code_length,
{ "PIN Code Length", "bthci_cmd.pin_code_length",
FT_UINT8, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_pin_code,
{ "PIN Code", "bthci_cmd.pin_code",
FT_STRING, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_pin_type,
{ "PIN Type", "bthci_cmd.pin_type",
FT_UINT8, BASE_HEX, VALS(cmd_pin_types), 0x0,
"PIN Types", HFILL }
},
{ &hf_bthci_cmd_encryption_enable,
{ "Encryption Enable", "bthci_cmd.encryption_enable",
FT_UINT8, BASE_HEX, VALS(cmd_encryption_enable), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_key_flag,
{ "Key Flag", "bthci_cmd.key_flag",
FT_UINT8, BASE_HEX, VALS(cmd_key_flag), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_max_interval_hold,
{ "Hold Mode Max Interval", "bthci_cmd.hold_mode_max_int",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Maximal acceptable number of Baseband slots to wait in Hold Mode.", HFILL }
},
{ &hf_bthci_cmd_min_interval_hold,
{ "Hold Mode Min Interval", "bthci_cmd.hold_mode_min_int",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Minimum acceptable number of Baseband slots to wait in Hold Mode.", HFILL }
},
{ &hf_bthci_cmd_max_interval_sniff,
{ "Sniff Max Interval", "bthci_cmd.sniff_max_int",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Maximal acceptable number of Baseband slots between each sniff period.", HFILL }
},
{ &hf_bthci_cmd_min_interval_sniff,
{ "Sniff Min Interval", "bthci_cmd.sniff_min_int",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Minimum acceptable number of Baseband slots between each sniff period.", HFILL }
},
{ &hf_bthci_cmd_sniff_attempt,
{ "Sniff Attempt", "bthci_cmd.sniff_attempt",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Number of Baseband receive slots for sniff attempt.", HFILL }
},
{ &hf_bthci_cmd_timeout,
{ "Timeout", "bthci_cmd.timeout",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Number of Baseband slots for timeout.", HFILL }
},
{ &hf_bthci_cmd_max_interval_beacon,
{ "Beacon Max Interval", "bthci_cmd.beacon_max_int",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Maximal acceptable number of Baseband slots between consecutive beacons.", HFILL }
},
{ &hf_bthci_cmd_min_interval_beacon,
{ "Beacon Min Interval", "bthci_cmd.beacon_min_int",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Minimum acceptable number of Baseband slots between consecutive beacons.", HFILL }
},
{ &hf_bthci_cmd_flags,
{ "Flags", "bthci_cmd.flags",
FT_UINT8, BASE_HEX, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_service_type,
{ "Service Type", "bthci_cmd.service_type",
FT_UINT8, BASE_HEX, VALS(cmd_service_types), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_token_rate,
{ "Available Token Rate", "bthci_cmd.token_rate",
FT_UINT32, BASE_DEC, NULL, 0x0,
"Token Rate, in bytes per second", HFILL }
},
{ &hf_bthci_cmd_token_bucket_size,
{ "Available Token Bucket Size", "bthci_cmd.token_bucket_size",
FT_UINT32, BASE_DEC, NULL, 0x0,
"Token Bucket Size in bytes", HFILL }
},
{ &hf_bthci_cmd_peak_bandwidth,
{ "Peak Bandwidth", "bthci_cmd.peak_bandwidth",
FT_UINT32, BASE_DEC, NULL, 0x0,
"Peak Bandwidth, in bytes per second", HFILL }
},
{ &hf_bthci_cmd_latency,
{ "Latency", "bthci_cmd.latency",
FT_UINT32, BASE_DEC, NULL, 0x0,
"Latency, in microseconds", HFILL }
},
{ &hf_bthci_cmd_delay_variation,
{ "Delay Variation", "bthci_cmd.delay_variation",
FT_UINT32, BASE_DEC, NULL, 0x0,
"Delay Variation, in microseconds", HFILL }
},
{ &hf_bthci_cmd_link_policy_setting_switch,
{ "Enable Master Slave Switch", "bthci_cmd.link_policy_switch",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0001,
NULL, HFILL }
},
{ &hf_bthci_cmd_link_policy_setting_hold,
{ "Enable Hold Mode", "bthci_cmd.link_policy_hold",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0002,
NULL, HFILL }
},
{ &hf_bthci_cmd_link_policy_setting_sniff,
{ "Enable Sniff Mode", "bthci_cmd.link_policy_sniff",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0004,
NULL, HFILL }
},
{ &hf_bthci_cmd_link_policy_setting_park,
{ "Enable Park Mode", "bthci_cmd.link_policy_park",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0008,
NULL, HFILL }
},
{ &hf_bthci_cmd_filter_type,
{ "Filter Type", "bthci_cmd.filter_type",
FT_UINT8, BASE_HEX, VALS(cmd_filter_types), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_inquiry_result_filter_condition_type,
{ "Filter Condition Type", "bthci_cmd.filter_condition_type",
FT_UINT8, BASE_HEX, VALS(cmd_inquiry_result_filter_condition_types), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_connection_setup_filter_condition_type,
{ "Filter Condition Type", "bthci_cmd.filter_condition_type",
FT_UINT8, BASE_HEX, VALS(cmd_connection_setup_filter_condition_types), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_cod_class_of_device_mask,
{ "Class of Device Mask", "bthci_cmd.class_of_device_mask",
FT_UINT24, BASE_HEX, NULL, 0x0,
"Bit Mask used to determine which bits of the Class of Device parameter are of interest.", HFILL }
},
{ &hf_bthci_cmd_cod_major_device_class_mask,
{ "Major Device Class Mask", "bthci_cmd.class_of_device_mask.major_device_class",
FT_UINT16, BASE_HEX, NULL, 0x1F,
NULL, HFILL }
},
{ &hf_bthci_cmd_cod_major_service_class_information_mask,
{ "Major Service Classes Mask: Information", "bthci_cmd.class_of_device_mask.major_service_classes.information",
FT_BOOLEAN, 16, NULL, 0x8000,
NULL, HFILL }
},
{ &hf_bthci_cmd_cod_major_service_class_telephony_mask,
{ "Major Service Classes Mask: Telephony", "bthci_cmd.class_of_device_mask.major_service_classes.telephony",
FT_BOOLEAN, 16, NULL, 0x4000,
NULL, HFILL }
},
{ &hf_bthci_cmd_cod_major_service_class_audio_mask,
{ "Major Service Classes Mask: Audio", "bthci_cmd.class_of_device_mask.major_service_classes.audio",
FT_BOOLEAN, 16, NULL, 0x2000,
NULL, HFILL }
},
{ &hf_bthci_cmd_cod_major_service_class_object_transfer_mask,
{ "Major Service Classes Mask: Object Transfer", "bthci_cmd.class_of_device_mask.major_service_classes.object_transfer",
FT_BOOLEAN, 16, NULL, 0x1000,
NULL, HFILL }
},
{ &hf_bthci_cmd_cod_major_service_class_capturing_mask,
{ "Major Service Classes Mask: Capturing", "bthci_cmd.class_of_device_mask.major_service_classes.capturing",
FT_BOOLEAN, 16, NULL, 0x0800,
NULL, HFILL }
},
{ &hf_bthci_cmd_cod_major_service_class_rendering_mask,
{ "Major Service Classes Mask: Rendering", "bthci_cmd.class_of_device_mask.major_service_classes.rendering",
FT_BOOLEAN, 16, NULL, 0x0400,
NULL, HFILL }
},
{ &hf_bthci_cmd_cod_major_service_class_networking_mask,
{ "Major Service Classes Mask: Networking", "bthci_cmd.class_of_device_mask.major_service_classes.networking",
FT_BOOLEAN, 16, NULL, 0x0200,
NULL, HFILL }
},
{ &hf_bthci_cmd_cod_major_service_class_positioning_mask,
{ "Major Service Classes Mask: Positioning", "bthci_cmd.class_of_device_mask.major_service_classes.positioning",
FT_BOOLEAN, 16, NULL, 0x0100,
NULL, HFILL }
},
{ &hf_bthci_cmd_cod_major_service_class_reserved_mask,
{ "Major Service Classes Mask: Reserved", "bthci_cmd.class_of_device_mask.major_service_classes.reserved",
FT_UINT16, BASE_HEX, NULL, 0x00C0,
NULL, HFILL }
},
{ &hf_bthci_cmd_cod_major_service_class_limited_discoverable_mode_mask,
{ "Major Service Classes Mask: Limited Discoverable Mode", "bthci_cmd.class_of_device_mask.major_service_classes.limited_discoverable_mode",
FT_BOOLEAN, 16, NULL, 0x0020,
NULL, HFILL }
},
{ &hf_bthci_cmd_cod_minor_device_class_mask,
{ "Minor Device Class Mask", "bthci_cmd.class_of_device_mask.minor_device_class",
FT_UINT8, BASE_HEX, NULL, 0xFC,
NULL, HFILL }
},
{ &hf_bthci_cmd_cod_format_type_mask,
{ "Format Type Mask", "bthci_cmd.class_of_device_mask.format_type",
FT_UINT8, BASE_HEX, NULL, 0x03,
NULL, HFILL }
},
{ &hf_bthci_cmd_auto_acc_flag,
{ "Auto Accept Flag", "bthci_cmd.auto_accept_flag",
FT_UINT8, BASE_HEX, VALS(cmd_auto_acc_flag_values), 0x0,
"Class of Device of Interest", HFILL }
},
{ &hf_bthci_cmd_read_all_flag,
{ "Read All Flag", "bthci_cmd.read_all_flag",
FT_UINT8, BASE_HEX, VALS(cmd_read_all_flag_values), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_delete_all_flag,
{ "Delete All Flag", "bthci_cmd.delete_all_flag",
FT_UINT8, BASE_HEX, VALS(cmd_delete_all_flag_values), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_authentication_enable,
{ "Authentication Enable", "bthci_cmd.auth_enable",
FT_UINT8, BASE_HEX, VALS(cmd_authentication_enable_values), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_input_unused,
{ "Unused bits", "bthci_cmd.voice.unused",
FT_UINT16, BASE_HEX, NULL, 0xfc00,
NULL, HFILL }
},
{ &hf_bthci_cmd_input_coding,
{ "Input Coding", "bthci_cmd.voice.input_coding",
FT_UINT16, BASE_DEC, VALS(cmd_input_coding_values), 0x0300,
"Authentication Enable", HFILL }
},
{ &hf_bthci_cmd_input_data_format,
{ "Input Data Format", "bthci_cmd.voice.input_data_format",
FT_UINT16, BASE_DEC, VALS(cmd_input_data_format_values), 0x00c0,
NULL, HFILL }
},
{ &hf_bthci_cmd_input_sample_size,
{ "Input Sample Size", "bthci_cmd.voice.input_sample_size",
FT_UINT16, BASE_DEC, VALS(cmd_input_sample_size_values), 0x0020,
NULL, HFILL }
},
{ &hf_bthci_cmd_linear_pcm_bit_pos,
{ "Linear PCM Bit Position", "bthci_cmd.voice.linear_pcm_bit_pos",
FT_UINT16, BASE_DEC, NULL, 0x001c,
"# bit pos. that MSB of sample is away from starting at MSB", HFILL }
},
{ &hf_bthci_cmd_air_coding_format,
{ "Air Coding Format", "bthci_cmd.voice.air_coding_format",
FT_UINT16, BASE_DEC, VALS(cmd_air_coding_format_values), 0x0003,
NULL, HFILL }
},
{ &hf_bthci_cmd_num_broadcast_retransmissions,
{ "Num Broadcast Retran", "bthci_cmd.num_broad_retran",
FT_UINT8, BASE_DEC, NULL, 0x0,
"Number of Broadcast Retransmissions", HFILL }
},
{ &hf_bthci_cmd_hold_mode_act_page,
{ "Suspend Page Scan", "bthci_cmd.hold_mode_page",
FT_UINT8, BASE_DEC, VALS(cmd_boolean), 0x1,
"Device can enter low power state", HFILL }
},
{ &hf_bthci_cmd_hold_mode_act_inquiry,
{ "Suspend Inquiry Scan", "bthci_cmd.hold_mode_inquiry",
FT_UINT8, BASE_DEC, VALS(cmd_boolean), 0x2,
"Device can enter low power state", HFILL }
},
{ &hf_bthci_cmd_hold_mode_act_periodic,
{ "Suspend Periodic Inquiries", "bthci_cmd.hold_mode_periodic",
FT_UINT8, BASE_DEC, VALS(cmd_boolean), 0x4,
"Device can enter low power state", HFILL }
},
{ &hf_bthci_cmd_scan_enable,
{ "Scan Enable", "bthci_cmd.scan_enable",
FT_UINT8, BASE_HEX, VALS(bthci_cmd_scan_enable_values), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_interval,
{ "Interval", "bthci_cmd.interval",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_window,
{ "Interval", "bthci_cmd.window",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Window", HFILL }
},
{ &hf_bthci_cmd_device_name,
{ "Device Name", "bthci_cmd.device_name",
FT_STRINGZ, BASE_NONE, NULL, 0x0,
"Userfriendly descriptive name for the device", HFILL }
},
{ &hf_bthci_cmd_num_curr_iac,
{ "Number of Current IAC", "bthci_cmd.num_curr_iac",
FT_UINT8, BASE_DEC, NULL, 0x0,
"Number of IACs which are currently in use", HFILL }
},
{ &hf_bthci_cmd_iac_lap,
{ "IAC LAP", "bthci_cmd.num_curr_iac",
FT_UINT24, BASE_HEX, NULL, 0x0,
"LAP(s)used to create IAC", HFILL }
},
{ &hf_bthci_cmd_evt_mask_00,
{ "Inquiry Complete", "bthci_cmd.evt_mask_00",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x01,
"Inquiry Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_01,
{ "Inquiry Result", "bthci_cmd.evt_mask_01",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x02,
"Inquiry Result Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_02,
{ "Connect Complete", "bthci_cmd.evt_mask_02",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x04,
"Connection Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_03,
{ "Connect Request", "bthci_cmd.evt_mask_03",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x08,
"Connect Request Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_04,
{ "Disconnect Complete", "bthci_cmd.evt_mask_04",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x10,
"Disconnect Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_05,
{ "Auth Complete", "bthci_cmd.evt_mask_05",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x20,
"Auth Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_06,
{ "Remote Name Req Complete", "bthci_cmd.evt_mask_06",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x40,
"Remote Name Req Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_07,
{ "Encrypt Change", "bthci_cmd.evt_mask_07",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x80,
"Encrypt Change Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_10,
{ "Change Connection Link Key Complete", "bthci_cmd.evt_mask_10",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x01,
"Change Connection Link Key Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_11,
{ "Master Link Key Complete", "bthci_cmd.evt_mask_11",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x02,
"Master Link Key Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_12,
{ "Read Remote Supported Features", "bthci_cmd.evt_mask_12",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x04,
"Read Remote Supported Features Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_13,
{ "Read Remote Ver Info Complete", "bthci_cmd.evt_mask_13",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x08,
"Read Remote Ver Info Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_14,
{ "QoS Setup Complete", "bthci_cmd.evt_mask_14",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x10,
"QoS Setup Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_17,
{ "Hardware Error", "bthci_cmd.evt_mask_17",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x80,
"Hardware Error Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_20,
{ "Flush Occurred", "bthci_cmd.evt_mask_20",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x01,
"Flush Occurred Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_21,
{ "Role Change", "bthci_cmd.evt_mask_21",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x02,
"Role Change Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_23,
{ "Mode Change", "bthci_cmd.evt_mask_23",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x08,
"Mode Change Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_24,
{ "Return Link Keys", "bthci_cmd.evt_mask_24",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x10,
"Return Link Keys Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_25,
{ "PIN Code Request", "bthci_cmd.evt_mask_25",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x20,
"PIN Code Request Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_26,
{ "Link Key Request", "bthci_cmd.evt_mask_26",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x40,
"Link Key Request Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_27,
{ "Link Key Notification", "bthci_cmd.evt_mask_27",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x80,
"Link Key Notification Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_30,
{ "Loopback Command", "bthci_cmd.evt_mask_30",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x01,
"Loopback Command Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_31,
{ "Data Buffer Overflow" , "bthci_cmd.evt_mask_31",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x02,
"Data Buffer Overflow Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_32,
{ "Max Slots Change", "bthci_cmd.evt_mask_32",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x04,
"Max Slots Change Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_33,
{ "Read Clock Offset Complete", "bthci_cmd.evt_mask_33",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x08,
"Read Clock Offset Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_34,
{ "Connection Packet Type Changed", "bthci_cmd.evt_mask_34",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x10,
"Connection Packet Type Changed Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_35,
{ "QoS Violation", "bthci_cmd.evt_mask_35",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x20,
"QoS Violation Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_36,
{ "Page Scan Mode Change", "bthci_cmd.evt_mask_36",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x40,
"Page Scan Mode Change Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_37,
{ "Page Scan Repetition Mode Change", "bthci_cmd.evt_mask_37",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x80,
"Page Scan Repetition Mode Change Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_40,
{ "Flow Specification Complete", "bthci_cmd.evt_mask_40",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x01,
"Flow Specification Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_41,
{ "Inquiry Result With RSSI", "bthci_cmd.evt_mask_41",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x02,
"Inquiry Result With RSSI Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_42,
{ "Read Remote Ext. Features Complete", "bthci_cmd.evt_mask_42",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x04,
"Read Remote Ext. Features Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_53,
{ "Synchronous Connection Complete", "bthci_cmd.evt_mask_53",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x08,
"Synchronous Connection Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_54,
{ "Synchronous Connection Changed", "bthci_cmd.evt_mask_54",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x10,
"Synchronous Connection Changed Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_55,
{ "Sniff Subrate", "bthci_cmd.evt_mask_55",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x20,
"Sniff Subrate Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_56,
{ "Extended Inquiry Result", "bthci_cmd.evt_mask_56",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x40,
"Extended Inquiry Result Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_57,
{ "Encryption Key Refresh Complete", "bthci_cmd.evt_mask_57",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x80,
"Encryption Key Refresh Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_60,
{ "IO Capability Request", "bthci_cmd.evt_mask_60",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x01,
"IO Capability Request Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_61,
{ "IO Capability Response", "bthci_cmd.evt_mask_61",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x02,
"IO Capability Response Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_62,
{ "User Confirmation Request", "bthci_cmd.evt_mask_62",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x04,
"User Confirmation Request Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_63,
{ "User Passkey Request", "bthci_cmd.evt_mask_63",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x08,
"User Passkey Request Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_64,
{ "Remote OOB Data Request", "bthci_cmd.evt_mask_64",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x10,
"Remote OOB Data Request Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_65,
{ "Simple Pairing Complete", "bthci_cmd.evt_mask_65",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x20,
"Simple Pairing Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_67,
{ "Link Supervision Timeout Changed", "bthci_cmd.evt_mask_67",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x80,
"Link Supervision Timeout Changed Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_70,
{ "Enhanced Flush Complete", "bthci_cmd.evt_mask_70",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x01,
"Enhanced Flush Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_72,
{ "User Passkey Notification", "bthci_cmd.evt_mask_72",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x04,
"User Passkey Notification Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask_73,
{ "Keypress Notification", "bthci_cmd.evt_mask_73",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x08,
"Keypress Notification Bit", HFILL }
},
{ &hf_bthci_cmd_sco_flow_control,
{ "SCO Flow Control","bthci_cmd.flow_control",
FT_UINT8, BASE_HEX, VALS(cmd_en_disabled), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_num_handles,
{ "Number of Handles", "bthci_cmd.num_handles",
FT_UINT8, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_num_compl_packets,
{ "Number of Completed Packets", "bthci_cmd.num_compl_packets",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Number of Completed HCI Data Packets", HFILL }
},
{ &hf_bthci_cmd_flow_contr_enable,
{ "Flow Control Enable", "bthci_cmd.flow_contr_enable",
FT_UINT8, BASE_HEX, VALS(cmd_flow_contr_enable), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_host_data_packet_length_acl,
{"Host ACL Data Packet Length (bytes)", "bthci_cmd.max_data_length_acl",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Max Host ACL Data Packet length of data portion host is able to accept", HFILL }
},
{ &hf_bthci_cmd_host_data_packet_length_sco,
{"Host SCO Data Packet Length (bytes)", "bthci_cmd.max_data_length_sco",
FT_UINT8, BASE_DEC, NULL, 0x0,
"Max Host SCO Data Packet length of data portion host is able to accept", HFILL }
},
{ &hf_bthci_cmd_host_total_num_acl_data_packets,
{"Host Total Num ACL Data Packets", "bthci_cmd.max_data_num_acl",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Total Number of HCI ACL Data Packets that can be stored in the data buffers of the Host", HFILL }
},
{ &hf_bthci_cmd_host_total_num_sco_data_packets,
{"Host Total Num SCO Data Packets", "bthci_cmd.max_data_num_sco",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Total Number of HCI SCO Data Packets that can be stored in the data buffers of the Host", HFILL }
},
{ &hf_bthci_cmd_loopback_mode,
{"Loopback Mode", "bthci_cmd.loopback_mode",
FT_UINT8, BASE_HEX, VALS(cmd_loopback_modes), 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_page_number,
{"Page Number", "bthci_cmd.page_number",
FT_UINT8, BASE_DEC, NULL, 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_transmit_bandwidth,
{"Tx Bandwidth (bytes/s)", "bthci_cmd.tx_bandwidth",
FT_UINT32, BASE_DEC, NULL, 0x0,
"Tx Bandwidth", HFILL}
},
{ &hf_bthci_cmd_receive_bandwidth,
{"Rx Bandwidth (bytes/s)", "bthci_cmd.rx_bandwidth",
FT_UINT32, BASE_DEC, NULL, 0x0,
"Rx Bandwidth", HFILL}
},
{ &hf_bthci_cmd_max_latency_ms,
{"Max. Latency (ms)", "bthci_cmd.max_latency_ms",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_max_latency,
{"Max. Latency", "bthci_cmd.max_latency",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Max. Latency in baseband slots", HFILL}
},
{ &hf_bthci_cmd_retransmission_effort,
{"Retransmission Effort", "bthci_cmd.retransmission_effort",
FT_UINT8, BASE_DEC, VALS(cmd_rtx_effort), 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_scan_type,
{"Scan Type", "bthci_cmd.inq_scan_type",
FT_UINT8, BASE_DEC, VALS(cmd_scan_types), 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_inq_mode,
{"Inquiry Mode", "bthci_cmd.inq_scan_type",
FT_UINT8, BASE_DEC, VALS(cmd_inq_modes), 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_fec_required,
{"FEC Required", "bthci_cmd.fec_required",
FT_UINT8, BASE_DEC, VALS(cmd_boolean), 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_err_data_reporting,
{"Erroneous Data Reporting", "bthci_cmd.err_data_reporting",
FT_UINT8, BASE_DEC, VALS(cmd_en_disabled), 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_tx_power,
{"Power Level (dBm)", "bthci_cmd.power_level",
FT_INT8, BASE_DEC, NULL, 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_sco_packet_type_hv1,
{ "Packet Type HV1", "bthci_cmd.sco_packet_type_hv1",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0001,
NULL, HFILL }
},
{ &hf_bthci_cmd_sco_packet_type_hv2,
{ "Packet Type HV2", "bthci_cmd.sco_packet_type_hv2",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0002,
NULL, HFILL }
},
{ &hf_bthci_cmd_sco_packet_type_hv3,
{ "Packet Type HV3", "bthci_cmd.sco_packet_type_hv3",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0004,
NULL, HFILL }
},
{ &hf_bthci_cmd_sco_packet_type_ev3,
{ "Packet Type EV3", "bthci_cmd.sco_packet_type_ev3",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0008,
NULL, HFILL }
},
{ &hf_bthci_cmd_sco_packet_type_ev4,
{ "Packet Type EV4", "bthci_cmd.sco_packet_type_ev4",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0010,
NULL, HFILL }
},
{ &hf_bthci_cmd_sco_packet_type_ev5,
{ "Packet Type EV5", "bthci_cmd.sco_packet_type_ev5",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0020,
NULL, HFILL }
},
{ &hf_bthci_cmd_sco_packet_type_2ev3,
{ "Packet Type 2-EV3", "bthci_cmd.sco_packet_type_2ev3",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0040,
NULL, HFILL }
},
{ &hf_bthci_cmd_sco_packet_type_3ev3,
{ "Packet Type 3-EV3", "bthci_cmd.sco_packet_type_3ev3",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0080,
NULL, HFILL }
},
{ &hf_bthci_cmd_sco_packet_type_2ev5,
{ "Packet Type 2-EV5", "bthci_cmd.sco_packet_type_2ev5",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0100,
NULL, HFILL }
},
{ &hf_bthci_cmd_sco_packet_type_3ev5,
{ "Packet Type 3-EV5", "bthci_cmd.sco_packet_type_3ev5",
FT_UINT16, BASE_DEC, VALS(cmd_boolean), 0x0200,
NULL, HFILL }
},
{ &hf_bthci_cmd_min_remote_timeout,
{"Min. Remote Timeout", "bthci_cmd.min_remote_timeout",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Min. Remote Timeout in baseband slots", HFILL}
},
{ &hf_bthci_cmd_min_local_timeout,
{"Min. Local Timeout", "bthci_cmd.min_local_timeout",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Min. Local Timeout in baseband slots", HFILL}
},
{ &hf_bthci_cmd_flush_packet_type,
{"Packet Type", "bthci_cmd.flush_packet_type",
FT_UINT8, BASE_DEC, VALS(cmd_flush_pkt_type), 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_afh_ch_assessment_mode,
{"AFH Channel Assessment Mode", "bthci_cmd.afh_ch_assessment_mode",
FT_UINT8, BASE_DEC, VALS(cmd_en_disabled), 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_afh_ch_classification,
{ "Channel Classification", "bthci_cmd.afh_ch_classification",
FT_NONE, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_which_clock,
{"Which Clock", "bthci_cmd.which_clock",
FT_UINT8, BASE_DEC, VALS(cmd_which_clock), 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_io_capability,
{"IO Capability", "bthci_cmd.io_capability",
FT_UINT8, BASE_DEC, VALS(bthci_cmd_io_capability_vals), 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_oob_data_present,
{"OOB Data Present", "bthci_cmd.oob_data_present",
FT_UINT8, BASE_DEC, VALS(bthci_cmd_oob_data_present_vals), 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_auth_requirements,
{"Authentication Requirements", "bthci_cmd.auth_requirements",
FT_UINT8, BASE_DEC|BASE_EXT_STRING, &bthci_cmd_auth_req_vals_ext, 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_passkey,
{"Passkey", "bthci_cmd.passkey",
FT_UINT32, BASE_DEC, NULL, 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_hash_c,
{"Hash C", "bthci_cmd.hash_c",
FT_BYTES, BASE_NONE, NULL, 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_randomizer_r,
{"Randomizer R", "bthci_cmd.randomizer_r",
FT_BYTES, BASE_NONE, NULL, 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_simple_pairing_mode,
{"Simple Pairing Mode", "bthci_cmd.simple_pairing_mode",
FT_UINT8, BASE_DEC, VALS(cmd_en_disabled), 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_simple_pairing_debug_mode,
{"Simple Pairing Debug Mode", "bthci_cmd.simple_pairing_debug_mode",
FT_UINT8, BASE_DEC, VALS(cmd_en_disabled), 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_notification_type,
{"Notification Type", "bthci_cmd.notification_type",
FT_UINT8, BASE_DEC, VALS(bthci_cmd_notification_types), 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_physical_link_handle,
{"Physical Link Handle", "bthci_cmd.physical_link_handle",
FT_UINT8, BASE_HEX, NULL, 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_dedicated_amp_key_length,
{"Dedicated AMP Key Length", "bthci_cmd.dedicated_amp_key_length",
FT_UINT8, BASE_DEC, NULL, 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_dedicated_amp_key_type,
{"Dedicated AMP Key Type", "bthci_cmd.dedicated_amp_key_type",
FT_UINT8, BASE_DEC, VALS(bthci_cmd_amp_key_type), 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_dedicated_amp_key,
{"Dedicated AMP Key Type", "bthci_cmd.dedicated_amp_key",
FT_BYTES, BASE_NONE, NULL, 0x0,
NULL, HFILL}
},
{ &hf_bthci_cmd_flow_spec,
{ "Flow Spec", "bthci_cmd.flow_spec",
FT_NONE, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_flow_spec_sdu_size,
{ "Maximum SDU Size", "bthci_cmd.sdu_size",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_flow_spec_sdu_arrival_time,
{ "SDU Inter-arrival Time (us)", "bthci_cmd.sdu_arrival_time",
FT_UINT32, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_flow_spec_identifier,
{ "Identifier", "bthci_cmd.ident",
FT_UINT8, BASE_HEX, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_flow_spec_access_latency,
{ "Access Latency (us)", "bthci_cmd.access_latency",
FT_UINT32, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_flow_spec_service_type,
{ "Service Type", "bthci_cmd.servicetype",
FT_UINT8, BASE_HEX, VALS(cmd_flow_spec_servicetype), 0x0,
"Level of service required", HFILL }
},
{ &hf_bthci_cmd_flush_to_us,
{ "Flush Timeout (us)", "bthci_cmd.flushto",
FT_UINT32, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_logical_link_handle,
{ "Logical Link Handle", "bthci_cmd.logical_link_handle",
FT_UINT16, BASE_HEX, NULL, 0x0FFF,
NULL, HFILL }
},
{ &hf_bthci_cmd_evt_mask2_00,
{ "Physical Link Complete", "bthci_cmd.evt_mask2_00",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x01,
"Physical Link Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask2_01,
{ "Channel Selected", "bthci_cmd.evt_mask2_01",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x02,
"Channel Selected Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask2_02,
{ "Disconnection Physical Link", "bthci_cmd.evt_mask2_02",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x04,
"Disconnection Physical Link Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask2_03,
{ "Physical Link Loss Early Warning", "bthci_cmd.evt_mask2_03",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x08,
"Physical Link Loss Early Warning Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask2_04,
{ "Physical Link Recovery", "bthci_cmd.evt_mask2_04",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x10,
"Physical Link Recovery Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask2_05,
{ "Logical Link Complete", "bthci_cmd.evt_mask2_05",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x20,
"Logical Link Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask2_06,
{ "Disconnection Logical Link Complete", "bthci_cmd.evt_mask2_06",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x40,
"Disconnection Logical Link Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask2_07,
{ "Flow Spec Modify Complete", "bthci_cmd.evt_mask2_07",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x80,
"Flow Spec Modify Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask2_10,
{ "Number Of Completed Data Blocks", "bthci_cmd.evt_mask2_10",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x01,
"Number Of Completed Data Blocks Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask2_11,
{ "AMP Start Test", "bthci_cmd.evt_mask2_11",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x02,
"AMP Start Test Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask2_12,
{ "AMP Test End", "bthci_cmd.evt_mask2_12",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x04,
"AMP Test End Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask2_13,
{ "AMP Receiver Report", "bthci_cmd.evt_mask2_13",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x08,
"AMP Receiver Report Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask2_14,
{ "Short Range Mode Change Complete", "bthci_cmd.evt_mask2_14",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x10,
"Short Range Mode Change Complete Bit", HFILL }
},
{ &hf_bthci_cmd_evt_mask2_15,
{ "AMP Status Change", "bthci_cmd.evt_mask2_15",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x20,
"AMP Status Change Bit", HFILL }
},
{ &hf_bthci_cmd_location_domain_aware,
{ "Location Domain Aware", "bthci_cmd.location_domain_aware",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_location_domain,
{ "Location Domain", "bthci_cmd.location_domain",
FT_STRING, BASE_NONE, NULL, 0x0,
"ISO 3166-1 Country Code", HFILL }
},
{ &hf_bthci_cmd_location_domain_options,
{ "Location Domain Options", "bthci_cmd.location_domain_options",
FT_STRING, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_location_options,
{ "Location Options", "bthci_cmd.location_options",
FT_UINT8, BASE_HEX, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_flow_control_mode,
{ "Flow Control Mode", "bthci_cmd.flow_control_mode",
FT_UINT8, BASE_HEX, VALS(cmd_flow_ctrl_mode), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_tx_power_level_type,
{ "Tx Power Level Type", "bthci_cmd.tx_power_level_type",
FT_UINT8, BASE_HEX, VALS(cmd_power_level_types), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_short_range_mode,
{ "Short Range Mode", "bthci_cmd.short_range_mode",
FT_UINT8, BASE_HEX, VALS(cmd_en_disabled), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_enable_amp_recv_reports,
{ "Enable AMP Receiver Reports", "bthci_cmd.enable_amp_recv_reports",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_amp_recv_report_interval,
{ "AMP Receiver Report Interval (s)", "bthci_cmd.amp_recv_report_interval",
FT_UINT8, BASE_DEC, 0x0, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_length_so_far,
{ "Length So Far", "bthci_cmd.length_so_far",
FT_UINT16, BASE_DEC, 0x0, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_amp_assoc_length,
{ "AMP Assoc Length", "bthci_cmd.amp_assoc_length",
FT_UINT16, BASE_DEC, 0x0, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_amp_remaining_assoc_length,
{ "AMP Remaining Assoc Length", "bthci_cmd.amp_remaining_assoc_length",
FT_UINT16, BASE_DEC, 0x0, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_amp_assoc_fragment,
{ "AMP Assoc Fragment", "bthci_cmd.amp_assoc_fragment",
FT_BYTES, BASE_NONE, 0x0, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_supported_host,
{ "LE Supported Host", "bthci_cmd.le_supported_host",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_simultaneous_host,
{ "Simultaneous LE Host", "bthci_cmd.le_simlutaneous_host",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x0,
"Support for both LE and BR/EDR to same device", HFILL }
},
{ &hf_bthci_cmd_le_evt_mask_00,
{ "LE Connection Complete", "bthci_cmd.le_evt_mask_00",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x01,
"LE Connection Complete Bit", HFILL }
},
{ &hf_bthci_cmd_le_evt_mask_01,
{ "LE Advertising Report", "bthci_cmd.le_evt_mask_01",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x02,
"LE Advertising Report Bit", HFILL }
},
{ &hf_bthci_cmd_le_evt_mask_02,
{ "LE Connection Update Complete", "bthci_cmd.le_evt_mask_02",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x04,
"LE Connection Update Complete Bit", HFILL }
},
{ &hf_bthci_cmd_le_evt_mask_03,
{ "LE Read Remote Used Features Complete", "bthci_cmd.le_evt_mask_03",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x08,
"LE Read Remote Used Features Complete Bit", HFILL }
},
{ &hf_bthci_cmd_le_evt_mask_04,
{ "LE Long Term Key Request", "bthci_cmd.le_evt_mask_04",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x10,
"LE Long Term Key Request Bit", HFILL }
},
{ &hf_bthci_cmd_le_advts_interval_min,
{ "Advertising Interval Min", "bthci_cmd.le_advts_interval_min",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_advts_interval_max,
{ "Advertising Interval Max", "bthci_cmd.le_advts_interval_max",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_advts_type,
{ "Advertising Type", "bthci_cmd.le_advts_type",
FT_UINT8, BASE_HEX, VALS(cmd_le_advertising_types), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_address_type,
{ "Address Type", "bthci_cmd.le_address_type",
FT_UINT8, BASE_HEX, VALS(bthci_cmd_address_types_vals), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_own_address_type,
{ "Own Address Type", "bthci_cmd.le_own_address_type",
FT_UINT8, BASE_HEX, VALS(bthci_cmd_address_types_vals), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_peer_address_type,
{ "Peer Address Type", "bthci_cmd.le_peer_address_type",
FT_UINT8, BASE_HEX, VALS(bthci_cmd_address_types_vals), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_direct_address_type,
{ "Direct Address Type", "bthci_cmd.le_direct_address_type",
FT_UINT8, BASE_HEX, VALS(bthci_cmd_address_types_vals), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_advts_channel_map_1,
{ "Channel 37", "bthci_cmd.le_advts_ch_map_1",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x01,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_advts_channel_map_2,
{ "Channel 38", "bthci_cmd.le_advts_ch_map_2",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x02,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_advts_channel_map_3,
{ "Channel 39", "bthci_cmd.le_advts_ch_map_3",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x04,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_advts_filter_policy,
{ "Advertising Filter Policy", "bthci_cmd.le_advts_filter_policy",
FT_UINT8, BASE_HEX, VALS(cmd_le_advertising_filter_policy), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_data_length,
{ "Data Length", "bthci_cmd.le_data_length",
FT_UINT8, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_advts_enable,
{ "Advertising Enable", "bthci_cmd.le_advts_enable",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_scan_enable,
{ "Scan Enable", "bthci_cmd.le_scan_enable",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_filter_dublicates,
{ "Filter Dublicates", "bthci_cmd.le_filter_dublicates",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_scan_type,
{ "Scan Type", "bthci_cmd.le_scan_type",
FT_UINT8, BASE_HEX, VALS(cmd_le_scan_types), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_scan_interval,
{ "Scan Interval", "bthci_cmd.le_scan_interval",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_scan_window,
{ "Scan Window", "bthci_cmd.le_scan_window",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_scan_filter_policy,
{ "Scan Filter Policy", "bthci_cmd.le_scan_filter_policy",
FT_UINT8, BASE_HEX, VALS(cmd_le_scan_filter_policy), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_initiator_filter_policy,
{ "Initiator Filter Policy", "bthci_cmd.le_initiator_filter_policy",
FT_UINT8, BASE_HEX, VALS(cmd_init_filter_policy), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_con_interval_min,
{ "Connection Interval Min", "bthci_cmd.le_con_interval_min",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_con_interval_max,
{ "Connection Interval Max", "bthci_cmd.le_con_interval_max",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_con_latency,
{ "Connection Latency", "bthci_cmd.le_con_latency",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_supervision_timeout,
{ "Supervision Timeout", "bthci_cmd.le_supv_timeout",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_min_ce_length,
{ "Min CE Length", "bthci_cmd.le_min_ce_length",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Min. Connection Event Length", HFILL }
},
{ &hf_bthci_cmd_le_max_ce_length,
{ "Max CE Length", "bthci_cmd.le_max_ce_length",
FT_UINT16, BASE_DEC, NULL, 0x0,
"Max. Connection Event Length", HFILL }
},
{ &hf_bthci_cmd_le_channel_map,
{ "Channel Map", "bthci_cmd.le_channel_map",
FT_BYTES, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_key,
{ "Key", "bthci_cmd.le_key",
FT_BYTES, BASE_NONE, NULL, 0x0,
"Encryption Key", HFILL }
},
{ &hf_bthci_cmd_plaintext_data,
{ "Plaintext", "bthci_cmd.le_plaintext",
FT_BYTES, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_random_number,
{ "Random Number", "bthci_cmd.le_random_number",
FT_BYTES, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_encrypted_diversifier,
{ "Encrypted Diversifier", "bthci_cmd.le_encrypted_diversifier",
FT_UINT16, BASE_HEX, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_le_long_term_key,
{ "Long Term Key", "bthci_cmd.le_long_tem_key",
FT_BYTES, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_rx_freqency,
{ "Rx Frequency", "bthci_cmd.rx_freqency",
FT_UINT8, BASE_HEX, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_tx_freqency,
{ "Tx Frequency", "bthci_cmd.tx_freqency",
FT_UINT8, BASE_HEX, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_test_data_length,
{ "Test Data Length", "bthci_cmd.le_test_data_length",
FT_UINT8, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_test_packet_payload,
{ "Packet Payload", "bthci_cmd.le_test_data_length",
FT_UINT8, BASE_HEX, VALS(cmd_le_test_pkt_payload), 0x0,
NULL, HFILL }
},
{ &hf_bthci_cmd_parameter,
{ "Parameter", "bthci_cmd.parameter",
FT_NONE, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
};
static ei_register_info ei[] = {
{ &ei_command_unknown, { "bthci_cmd.expert.command.unknown.", PI_PROTOCOL, PI_WARN, "Unknown command", EXPFILL }},
{ &ei_command_parameter_unexpected, { "bthci_cmd.expert.parameter.unexpected", PI_PROTOCOL, PI_WARN, "Unexpected command parameter", EXPFILL }},
{ &ei_command_undecoded, { "bthci_cmd.expert.command.undecoded", PI_PROTOCOL, PI_UNDECODED, "Command undecoded", EXPFILL }}
};
static gint *ett[] = {
&ett_bthci_cmd,
&ett_opcode,
&ett_cod,
&ett_cod_mask,
&ett_flow_spec_subtree,
&ett_le_channel_map
};
static build_valid_func bthci_cmd_vendor_da_build_value[1] = {bthci_cmd_vendor_value};
static decode_as_value_t bthci_cmd_vendor_da_values = {bthci_cmd_vendor_prompt, 1, bthci_cmd_vendor_da_build_value};
static decode_as_t bthci_cmd_vendor_da = {"bthci_evt", "Vendor", "bthci_cmd.vendor", 1, 0, &bthci_cmd_vendor_da_values, NULL, NULL,
decode_as_default_populate_list, decode_as_default_reset, decode_as_default_change, NULL};
static const struct _opcode_value_string_arrays {
guint ogf;
const value_string *string_array;
guint length;
} opcode_value_string_arrays[] = {
{ 0x01, bthci_cmd_ocf_link_control_vals, array_length(bthci_cmd_ocf_link_control_vals) },
{ 0x02, bthci_cmd_ocf_link_policy_vals, array_length(bthci_cmd_ocf_link_policy_vals) },
{ 0x03, bthci_cmd_ocf_host_controller_and_baseband_vals, array_length(bthci_cmd_ocf_host_controller_and_baseband_vals) },
{ 0x04, bthci_cmd_ocf_informational_vals, array_length(bthci_cmd_ocf_informational_vals) },
{ 0x05, bthci_cmd_ocf_status_vals, array_length(bthci_cmd_ocf_status_vals) },
{ 0x06, bthci_cmd_ocf_testing_vals, array_length(bthci_cmd_ocf_testing_vals) },
{ 0x08, bthci_cmd_ocf_low_energy_vals, array_length(bthci_cmd_ocf_low_energy_vals) },
};
bthci_cmd_opcode_vals[i_opcode].value = 0;
bthci_cmd_opcode_vals[i_opcode].strptr = "No Operation";
i_opcode += 1;
for (i_array = 0; i_array < array_length(opcode_value_string_arrays); i_array += 1) {
for (i_string_array = 0; i_string_array < opcode_value_string_arrays[i_array].length - 1; i_string_array += 1) {
bthci_cmd_opcode_vals[i_opcode].value = opcode_value_string_arrays[i_array].string_array[i_string_array].value | (opcode_value_string_arrays[i_array].ogf << 10);
bthci_cmd_opcode_vals[i_opcode].strptr = opcode_value_string_arrays[i_array].string_array[i_string_array].strptr;
i_opcode += 1;
}
}
proto_bthci_cmd = proto_register_protocol("Bluetooth HCI Command", "HCI_CMD", "bthci_cmd");
bthci_cmd_handle = new_register_dissector("bthci_cmd", dissect_bthci_cmd, proto_bthci_cmd);
proto_register_field_array(proto_bthci_cmd, hf, array_length(hf));
proto_register_subtree_array(ett, array_length(ett));
expert_bthci_cmd = expert_register_protocol(proto_bthci_cmd);
expert_register_field_array(expert_bthci_cmd, ei, array_length(ei));
vendor_dissector_table = register_dissector_table("bthci_cmd.vendor", "BT HCI Vendor", FT_UINT16, BASE_HEX);
module = prefs_register_protocol(proto_bthci_cmd, NULL);
prefs_register_static_text_preference(module, "hci_cmd.version",
"Bluetooth HCI version: 4.0 (Core)",
"Version of protocol supported by this dissector.");
register_decode_as(&bthci_cmd_vendor_da);
}
void
proto_reg_handoff_bthci_cmd(void)
{
dissector_add_uint("hci_h4.type", HCI_H4_TYPE_CMD, bthci_cmd_handle);
dissector_add_uint("hci_h1.type", BTHCI_CHANNEL_COMMAND, bthci_cmd_handle);
}
static gint
dissect_eir_ad_data(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
proto_item *entry_item;
proto_tree *entry_tree;
proto_item *sub_item;
gint offset = 0;
guint8 length;
guint8 type;
guint8 data_size;
gint64 end_offset;
guint i_uuid;
data_size = tvb_length(tvb);
while (offset < data_size) {
length = tvb_get_guint8(tvb, offset);
if (length <= 0) break;
type = tvb_get_guint8(tvb, offset + 1);
entry_item = proto_tree_add_none_format(tree, hf_btcommon_eir_ad_entry, tvb, offset, length + 1, "%s",
val_to_str_const(type, bthci_cmd_eir_data_type_vals, "Unknown"));
entry_tree = proto_item_add_subtree(entry_item, ett_eir_ad_entry);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_length, tvb, offset, 1, ENC_NA);
offset += 1;
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_type, tvb, offset, 1, ENC_NA);
offset += 1;
length -= 1;
switch (type) {
case 0x01:
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_flags_reserved, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_flags_le_bredr_support_host, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_flags_le_bredr_support_controller, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_flags_bredr_not_support, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_flags_le_general_discoverable_mode, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_flags_le_limited_discoverable_mode, tvb, offset, 1, ENC_NA);
offset += 1;
break;
case 0x02:
case 0x03:
case 0x14:
case 0x16:
end_offset = offset + length;
while (offset < end_offset) {
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_uuid_16, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset += 2;
}
break;
case 0x04:
case 0x05:
case 0x1F:
case 0x20:
end_offset = offset + length;
while (offset < end_offset) {
if (tvb_get_ntohs(tvb, offset) == 0x0000) {
sub_item = proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_uuid_32, tvb, offset, 4, ENC_BIG_ENDIAN);
proto_item_append_text(sub_item, " (%s)", val_to_str_ext_const(tvb_get_ntohs(tvb, offset + 2), &bt_sig_uuid_vals_ext, "Unknown"));
} else {
sub_item = proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_custom_uuid, tvb, offset, 4, ENC_NA);
i_uuid = 0;
while (custom_uuid[i_uuid].name) {
if (custom_uuid[i_uuid].size != 4) {
i_uuid += 1;
continue;
}
if (tvb_memeql(tvb, offset, custom_uuid[i_uuid].uuid, 4) == 0) {
proto_item_append_text(sub_item, " (%s)", custom_uuid[i_uuid].name);
break;
}
i_uuid += 1;
}
}
offset += 4;
}
break;
case 0x06:
case 0x07:
case 0x15:
case 0x21:
end_offset = offset + length;
while (offset < end_offset) {
if (tvb_get_ntohs(tvb, offset) == 0x0000 &&
tvb_get_ntohl(tvb, offset + 4) == 0x1000 &&
tvb_get_ntoh64(tvb, offset + 8) == G_GUINT64_CONSTANT(0x800000805F9B34FB)) {
sub_item = proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_uuid_128, tvb, offset, 16, ENC_NA);
proto_item_append_text(sub_item, " (%s)", val_to_str_ext_const(tvb_get_ntohs(tvb, offset + 2), &bt_sig_uuid_vals_ext, "Unknown"));
} else {
sub_item = proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_custom_uuid, tvb, offset, 16, ENC_NA);
i_uuid = 0;
while (custom_uuid[i_uuid].name) {
if (custom_uuid[i_uuid].size != 16) {
i_uuid += 1;
continue;
}
if (tvb_memeql(tvb, offset, custom_uuid[i_uuid].uuid, 16) == 0) {
proto_item_append_text(sub_item, " (%s)", custom_uuid[i_uuid].name);
break;
}
i_uuid += 1;
}
}
offset += 16;
}
break;
case 0x08:
case 0x09:
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_name, tvb, offset, length, ENC_ASCII | ENC_NA);
proto_item_append_text(entry_item, ": %s", tvb_format_text(tvb,offset, length));
offset += length;
break;
case 0x10:
if (length == 16) {
sub_item = proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_data, tvb, offset, 16, ENC_NA);
expert_add_info(pinfo, sub_item, &ei_eir_ad_undecoded);
} else if (length == 8) {
guint16 vendor_id_source;
guint16 vendor_id;
guint16 product_id;
const gchar *str_val;
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_did_vendor_id_source, tvb, offset, 2, ENC_LITTLE_ENDIAN);
vendor_id_source = tvb_get_letohs(tvb, offset);
offset += 2;
if (vendor_id_source == DID_VENDOR_ID_SOURCE_BLUETOOTH_SIG) {
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_did_vendor_id_bluetooth_sig, tvb, offset, 2, ENC_LITTLE_ENDIAN);
} else if (vendor_id_source == DID_VENDOR_ID_SOURCE_USB_FORUM) {
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_did_vendor_id_usb_forum, tvb, offset, 2, ENC_LITTLE_ENDIAN);
} else {
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_did_vendor_id, tvb, offset, 2, ENC_LITTLE_ENDIAN);
}
vendor_id = tvb_get_letohs(tvb, offset);
offset += 2;
sub_item = proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_did_product_id, tvb, offset, 2, ENC_LITTLE_ENDIAN);
product_id = tvb_get_letohs(tvb, offset);
offset += 2;
if (vendor_id_source == DID_VENDOR_ID_SOURCE_USB_FORUM) {
str_val = val_to_str_ext_const(vendor_id << 16 | product_id, &ext_usb_products_vals, "Unknown");
proto_item_append_text(sub_item, " (%s)", str_val);
}
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_did_version, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset += 2;
} else {
sub_item = proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_data, tvb, offset, length, ENC_NA);
expert_add_info(pinfo, sub_item, &ei_command_unknown);
}
break;
case 0x0A:
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_tx_power, tvb, offset, 1, ENC_NA);
offset += 1;
break;
case 0x0B:
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_ssp_oob_length, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset += 2;
break;
case 0x0C:
offset = dissect_bd_addr(hf_btcommon_eir_ad_bd_addr, tree, tvb, offset);
break;
case 0x0D:
call_dissector(btcommon_cod_handle, tvb_new_subset_length(tvb, offset, 3), pinfo, entry_tree);
offset += 3;
break;
case 0x0E:
case 0x1D:
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_hash_c, tvb, offset, 16, ENC_NA);
offset += 16;
break;
case 0x0F:
case 0x1E:
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_randomizer_r, tvb, offset, 16, ENC_NA);
offset += 16;
break;
case 0x11:
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_oob_flags_reserved, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_oob_flags_address_type, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_oob_flags_le_bredr_support, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_oob_flags_le_supported_host, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_oob_flags_data_present, tvb, offset, 1, ENC_NA);
offset += 1;
break;
case 0x12:
sub_item = proto_tree_add_item(tree, hf_btcommon_eir_ad_connection_interval_min, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(sub_item, " (%g msec)", tvb_get_letohs(tvb, offset) * 1.25);
offset += 2;
sub_item = proto_tree_add_item(tree, hf_btcommon_eir_ad_connection_interval_max, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(sub_item, " (%g msec)", tvb_get_letohs(tvb, offset) * 1.25);
offset += 2;
proto_item_append_text(entry_item, ": %g - %g msec", tvb_get_letohs(tvb, offset - 4) * 1.25, tvb_get_letohs(tvb, offset - 2) * 1.25);
break;
case 0x17:
case 0x18:
end_offset = offset + length;
while (offset < end_offset) {
offset = dissect_bd_addr(hf_btcommon_eir_ad_bd_addr, entry_tree, tvb, offset);
}
break;
case 0x19:
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_appearance, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(entry_item,": %s", val_to_str(tvb_get_letohs(tvb, offset), bthci_cmd_appearance_vals, "Unknown"));
offset += 2;
break;
case 0x1A:
sub_item = proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_advertising_interval, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_item_append_text(sub_item, " (%g msec)", tvb_get_letohs(tvb, offset) * 0.625);
offset += 2;
break;
case 0x1B:
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_le_bd_addr_reserved, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_le_bd_addr_type, tvb, offset, 1, ENC_NA);
offset += 1;
offset = dissect_bd_addr(hf_btcommon_eir_ad_bd_addr, entry_tree, tvb, offset);
break;
case 0x1C:
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_le_role, tvb, offset, 1, ENC_NA);
offset += 1;
break;
case 0x3D:
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_3ds_factory_test_mode, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_3ds_reserved, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_3ds_send_battery_level_report_on_startup, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_3ds_battery_level_reporting, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_3ds_association_notification, tvb, offset, 1, ENC_NA);
offset += 1;
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_3ds_path_loss_threshold, tvb, offset, 1, ENC_NA);
offset += 1;
break;
case 0xFF: {
guint16 company_id;
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_company_id, tvb, offset, 2, ENC_LITTLE_ENDIAN);
company_id = tvb_get_letohs(tvb, offset);
offset += 2;
length -= 2;
if (company_id == 0x000F && tvb_get_guint8(tvb, offset) == 0) {
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_3ds_legacy_fixed, tvb, offset, 1, ENC_NA);
offset += 1;
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_3ds_legacy_test_mode, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_3ds_legacy_fixed_6, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_3ds_legacy_ignored_5, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_3ds_legacy_fixed_4, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_3ds_legacy_ignored_1_3, tvb, offset, 1, ENC_NA);
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_3ds_legacy_3d_capable_tv, tvb, offset, 1, ENC_NA);
offset += 1;
proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_3ds_legacy_path_loss_threshold, tvb, offset, 1, ENC_NA);
offset += 1;
} else {
sub_item = proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_data, tvb, offset, length, ENC_NA);
expert_add_info(pinfo, sub_item, &ei_eir_ad_undecoded);
offset += length;
}
}
break;
default:
sub_item = proto_tree_add_item(entry_tree, hf_btcommon_eir_ad_data, tvb, offset, length, ENC_NA);
expert_add_info(pinfo, sub_item, &ei_eir_ad_unknown);
offset += length;
}
}
if (tvb_length_remaining(tvb, offset) > 0) {
proto_tree_add_item(tree, hf_btcommon_eir_ad_unused, tvb, offset, -1, ENC_NA);
offset = tvb_length(tvb);
}
return offset + data_size;
}
static gint
dissect_btcommon_cod(tvbuff_t *tvb, packet_info *pinfo _U_, proto_tree *tree, void *data _U_)
{
proto_item *cod_item;
proto_item *cod_tree;
guint16 major_service_classes;
guint8 major_device_class;
guint8 minor_device_class;
const gchar *minor_device_class_name;
gint offset = 0;
cod_item = proto_tree_add_item(tree, hf_btcommon_cod_class_of_device, tvb, offset, 3, ENC_LITTLE_ENDIAN);
cod_tree = proto_item_add_subtree(cod_item, ett_cod);
major_device_class = tvb_get_guint8(tvb, offset + 1) & 0x1F;
minor_device_class = tvb_get_guint8(tvb, offset) >> 2;
switch(major_device_class) {
case 0x01:
proto_tree_add_item(cod_tree, hf_btcommon_cod_minor_device_class_computer, tvb, offset, 1, ENC_LITTLE_ENDIAN);
minor_device_class_name = val_to_str_ext_const(minor_device_class, &bthci_cmd_cod_minor_device_class_computer_vals_ext, "Unknown");
break;
case 0x02:
proto_tree_add_item(cod_tree, hf_btcommon_cod_minor_device_class_phone, tvb, offset, 1, ENC_LITTLE_ENDIAN);
minor_device_class_name = val_to_str_ext_const(minor_device_class, &bthci_cmd_cod_minor_device_class_phone_vals_ext, "Unknown");
break;
case 0x03:
proto_tree_add_item(cod_tree, hf_btcommon_cod_minor_device_class_lan_net_load_factor, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_tree, hf_btcommon_cod_minor_device_class_lan_net_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
minor_device_class_name = val_to_str_ext_const(minor_device_class, &bthci_cmd_cod_minor_device_class_lan_net_load_factor_vals_ext, "Unknown");
break;
case 0x04:
proto_tree_add_item(cod_tree, hf_btcommon_cod_minor_device_class_audio_video, tvb, offset, 1, ENC_LITTLE_ENDIAN);
minor_device_class_name = val_to_str_ext_const(minor_device_class, &bthci_cmd_cod_minor_device_class_audio_video_vals_ext, "Unknown");
break;
case 0x05:
proto_tree_add_item(cod_tree, hf_btcommon_cod_minor_device_class_peripheral_class, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_tree, hf_btcommon_cod_minor_device_class_peripheral_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
minor_device_class_name = val_to_str_ext_const(minor_device_class, &bthci_cmd_cod_minor_device_class_peripheral_class_vals_ext, "Unknown");
break;
case 0x06:
proto_tree_add_item(cod_tree, hf_btcommon_cod_minor_device_class_imaging_class_printer, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_tree, hf_btcommon_cod_minor_device_class_imaging_class_scanner, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_tree, hf_btcommon_cod_minor_device_class_imaging_class_camera, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_tree, hf_btcommon_cod_minor_device_class_imaging_class_display, tvb, offset, 1, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_tree, hf_btcommon_cod_minor_device_class_imaging_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
minor_device_class_name = val_to_str_ext_const(minor_device_class, &bthci_cmd_cod_minor_device_class_imaging_type_vals_ext, "Unknown");
break;
case 0x07:
proto_tree_add_item(cod_tree, hf_btcommon_cod_minor_device_class_wearable, tvb, offset, 1, ENC_LITTLE_ENDIAN);
minor_device_class_name = val_to_str_ext_const(minor_device_class, &bthci_cmd_cod_minor_device_class_wearable_vals_ext, "Unknown");
break;
case 0x08:
proto_tree_add_item(cod_tree, hf_btcommon_cod_minor_device_class_toy, tvb, offset, 1, ENC_LITTLE_ENDIAN);
minor_device_class_name = val_to_str_ext_const(minor_device_class, &bthci_cmd_cod_minor_device_class_toy_vals_ext, "Unknown");
break;
case 0x09:
proto_tree_add_item(cod_tree, hf_btcommon_cod_minor_device_class_health, tvb, offset, 1, ENC_LITTLE_ENDIAN);
minor_device_class_name = val_to_str_ext_const(minor_device_class, &bthci_cmd_cod_minor_device_class_health_vals_ext, "Unknown");
break;
default:
minor_device_class_name = "Unknown";
proto_tree_add_item(cod_tree, hf_btcommon_cod_minor_device_class_unknown, tvb, offset, 1, ENC_LITTLE_ENDIAN);
}
proto_tree_add_item(cod_tree, hf_btcommon_cod_format_type, tvb, offset, 1, ENC_LITTLE_ENDIAN);
offset += 1;
proto_tree_add_item(cod_tree, hf_btcommon_cod_major_service_class_information, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_tree, hf_btcommon_cod_major_service_class_telephony, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_tree, hf_btcommon_cod_major_service_class_audio, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_tree, hf_btcommon_cod_major_service_class_object_transfer, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_tree, hf_btcommon_cod_major_service_class_capturing, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_tree, hf_btcommon_cod_major_service_class_rendering, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_tree, hf_btcommon_cod_major_service_class_networking, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_tree, hf_btcommon_cod_major_service_class_positioning, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_tree, hf_btcommon_cod_major_service_class_reserved, tvb, offset, 2, ENC_LITTLE_ENDIAN);
proto_tree_add_item(cod_tree, hf_btcommon_cod_major_service_class_limited_discoverable_mode, tvb, offset, 2, ENC_LITTLE_ENDIAN);
major_service_classes = tvb_get_letohs(tvb, offset) >> 5;
proto_tree_add_item(cod_tree, hf_btcommon_cod_major_device_class, tvb, offset, 2, ENC_LITTLE_ENDIAN);
offset += 2;
proto_item_append_text(cod_item, " (%s:%s - services:",
val_to_str_ext_const(major_device_class, &bthci_cmd_cod_major_device_class_vals_ext, "Unknown"),
minor_device_class_name);
if (major_service_classes & 0x001) proto_item_append_text(cod_item, " LimitedDiscoverableMode");
if (major_service_classes & 0x008) proto_item_append_text(cod_item, " Positioning");
if (major_service_classes & 0x010) proto_item_append_text(cod_item, " Networking");
if (major_service_classes & 0x020) proto_item_append_text(cod_item, " Rendering");
if (major_service_classes & 0x040) proto_item_append_text(cod_item, " Capturing");
if (major_service_classes & 0x080) proto_item_append_text(cod_item, " ObjectTransfer");
if (major_service_classes & 0x100) proto_item_append_text(cod_item, " Audio");
if (major_service_classes & 0x200) proto_item_append_text(cod_item, " Telephony");
if (major_service_classes & 0x400) proto_item_append_text(cod_item, " Information");
proto_item_append_text(cod_item, ")");
return offset;
}
static gint
dissect_btcommon_ad(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void *data _U_)
{
proto_item *main_item;
proto_tree *main_tree;
main_item = proto_tree_add_item(tree, hf_btcommon_eir_ad_advertising_data, tvb, 0, -1, ENC_NA);
main_tree = proto_item_add_subtree(main_item, ett_eir_ad);
return dissect_eir_ad_data(tvb, pinfo, main_tree);
}
static gint
dissect_btcommon_eir(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void *data _U_)
{
proto_item *main_item;
proto_tree *main_tree;
main_item = proto_tree_add_item(tree, hf_btcommon_eir_ad_extended_inquiry_response_data, tvb, 0, -1, ENC_NA);
main_tree = proto_item_add_subtree(main_item, ett_eir_ad);
return dissect_eir_ad_data(tvb, pinfo, main_tree);
}
static gint
dissect_btcommon_le_channel_map(tvbuff_t *tvb, packet_info *pinfo _U_, proto_tree *tree, void *data _U_)
{
gint offset = 0;
proto_tree_add_item(tree, hf_btcommon_le_channel_map_0, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_1, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_2, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_3, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_4, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_5, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_6, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_7, tvb, offset, 1, ENC_NA);
offset += 1;
proto_tree_add_item(tree, hf_btcommon_le_channel_map_8, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_9, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_10, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_11, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_12, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_13, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_14, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_15, tvb, offset, 1, ENC_NA);
offset += 1;
proto_tree_add_item(tree, hf_btcommon_le_channel_map_16, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_17, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_18, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_19, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_20, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_21, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_22, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_23, tvb, offset, 1, ENC_NA);
offset += 1;
proto_tree_add_item(tree, hf_btcommon_le_channel_map_24, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_25, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_26, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_27, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_28, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_29, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_30, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_31, tvb, offset, 1, ENC_NA);
offset += 1;
proto_tree_add_item(tree, hf_btcommon_le_channel_map_32, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_33, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_34, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_35, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_36, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_37, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_38, tvb, offset, 1, ENC_NA);
proto_tree_add_item(tree, hf_btcommon_le_channel_map_39, tvb, offset, 1, ENC_NA);
offset += 1;
return offset;
}
void
proto_register_btcommon(void)
{
expert_module_t *expert_module;
static hf_register_info hf[] = {
{ &hf_btcommon_eir_ad_extended_inquiry_response_data,
{ "Extended Inquiry Response Data", "btcommon.eir_ad.extended_inquiry_response_data",
FT_NONE, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_advertising_data,
{ "Advertising Data", "btcommon.eir_ad.advertising_data",
FT_NONE, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_entry,
{ "Entry", "btcommon.eir_ad.entry",
FT_NONE, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_unused,
{ "Unused", "btcommon.eir_ad.unused",
FT_NONE, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_data,
{"Data", "btcommon.eir_ad.entry.data",
FT_BYTES, BASE_NONE, NULL, 0x0,
NULL, HFILL}
},
{ &hf_btcommon_eir_ad_length,
{ "Length", "btcommon.eir_ad.entry.length",
FT_UINT8, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_type,
{ "Type", "btcommon.eir_ad.entry.type",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_eir_data_type_vals_ext, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_company_id,
{ "Company ID", "btcommon.eir_ad.entry.company_id",
FT_UINT16, BASE_HEX | BASE_EXT_STRING, &bthci_evt_comp_id_ext, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_flags_reserved,
{ "Reserved", "btcommon.eir_ad.entry.flags.reserved",
FT_UINT8, BASE_HEX, NULL, 0xE0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_flags_le_bredr_support_host,
{ "Simultaneous LE and BR/EDR to Same Device Capable (Host)", "btcommon.eir_ad.entry.flags.le_bredr_support_host",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x10,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_flags_le_bredr_support_controller,
{ "Simultaneous LE and BR/EDR to Same Device Capable (Controller)", "btcommon.eir_ad.entry.flags.le_bredr_support_controller",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x08,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_flags_bredr_not_support,
{ "BR/EDR Not Supported", "btcommon.eir_ad.entry.flags.bredr_not_supported",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x04,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_flags_le_general_discoverable_mode,
{ "LE General Discoverable Mode", "btcommon.eir_ad.entry.flags.le_general_discoverable_mode",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x02,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_flags_le_limited_discoverable_mode,
{ "LE Limited Discoverable Mode", "btcommon.eir_ad.entry.flags.le_limited_discoverable_mode",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x01,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_uuid_16,
{ "UUID 16", "btcommon.eir_ad.entry.uuid_16",
FT_UINT16, BASE_HEX | BASE_EXT_STRING, &bt_sig_uuid_vals_ext, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_uuid_32,
{ "UUID 32", "btcommon.eir_ad.entry.uuid_32",
FT_UINT32, BASE_HEX | BASE_EXT_STRING, &bt_sig_uuid_vals_ext, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_uuid_128,
{ "UUID 128", "btcommon.eir_ad.entry.uuid_128",
FT_BYTES, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_custom_uuid,
{ "Custom UUID", "btcommon.eir_ad.entry.custom_uuid",
FT_BYTES, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_name,
{ "Device Name", "btcommon.eir_ad.entry.device_name",
FT_STRING, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_tx_power,
{"Power Level (dBm)", "btcommon.eir_ad.entry.power_level",
FT_INT8, BASE_DEC, NULL, 0x0,
NULL, HFILL}
},
{ &hf_btcommon_eir_ad_ssp_oob_length,
{ "SSP OOB Length", "btcommon.eir_ad.entry.ssp_oob_length",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_bd_addr,
{ "BD_ADDR", "btcommon.eir_ad.entry.bd_addr",
FT_ETHER, BASE_NONE, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_le_bd_addr_reserved,
{ "Reserved", "btcommon.eir_ad.entry.le_bd_addr.reserved",
FT_BOOLEAN, 8, NULL, 0xFE,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_le_bd_addr_type,
{ "Type", "btcommon.eir_ad.entry.le_bd_addr.type",
FT_BOOLEAN, 8, NULL, 0x01,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_le_role,
{ "Type", "btcommon.eir_ad.entry.le_role",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &le_role_vals_ext, 0x01,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_did_vendor_id_source,
{ "Vendor ID Source", "btcommon.eir_ad.entry.did.vendor_id_source",
FT_UINT16, BASE_HEX | BASE_EXT_STRING, &did_vendor_id_source_vals_ext, 0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_did_vendor_id,
{ "Vendor ID", "btcommon.eir_ad.entry.did.vendor_id",
FT_UINT16, BASE_HEX, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_did_vendor_id_bluetooth_sig,
{ "Vendor ID", "btcommon.eir_ad.entry.did.vendor_id",
FT_UINT16, BASE_HEX | BASE_EXT_STRING, &bthci_evt_comp_id_ext, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_did_vendor_id_usb_forum,
{ "Vendor ID", "btcommon.eir_ad.entry.did.vendor_id",
FT_UINT16, BASE_HEX | BASE_EXT_STRING, &ext_usb_vendors_vals, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_did_product_id,
{ "Product ID", "btcommon.eir_ad.entry.did.product_id",
FT_UINT16, BASE_HEX, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_did_version,
{ "Version", "btcommon.eir_ad.entry.did.version",
FT_UINT16, BASE_HEX, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_3ds_association_notification,
{ "3DS Association Notification", "btcommon.eir_ad.entry.3ds.association_notification",
FT_BOOLEAN, 8, NULL, 0x01,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_3ds_battery_level_reporting,
{ "3DS Battery Level Reporting", "btcommon.eir_ad.entry.3ds.battery_level_reporting",
FT_BOOLEAN, 8, NULL, 0x02,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_3ds_send_battery_level_report_on_startup,
{ "3DS Send Battery Level Report on Startup", "btcommon.eir_ad.entry.3ds.send_battery_level_report_on_startup",
FT_BOOLEAN, 8, NULL, 0x04,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_3ds_reserved,
{ "Reserved", "btcommon.eir_ad.entry.3ds.reserved",
FT_BOOLEAN, 8, NULL, 0x78,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_3ds_factory_test_mode,
{ "3DS Factory Test Mode", "btcommon.eir_ad.entry.3ds.factory_test_mode",
FT_BOOLEAN, 8, NULL, 0x80,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_3ds_path_loss_threshold,
{ "3DS Path Loss Threshold", "btcommon.eir_ad.entry.3ds.path_loss_threshold",
FT_UINT8, BASE_DEC, NULL, 0x00,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_3ds_legacy_fixed,
{ "3DS Legacy Fixed", "btcommon.eir_ad.entry.3ds_legacy.fixed_byte",
FT_UINT8, BASE_DEC, NULL, 0x00,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_3ds_legacy_3d_capable_tv,
{ "3DS Legacy Capable TV", "btcommon.eir_ad.entry.3ds_legacy.capable_tv",
FT_BOOLEAN, 8, NULL, 0x01,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_3ds_legacy_ignored_1_3,
{ "3DS Legacy Ignored", "btcommon.eir_ad.entry.3ds_legacy.ignored.1_3",
FT_BOOLEAN, 8, NULL, 0x0E,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_3ds_legacy_fixed_4,
{ "3DS Legacy Fixed", "btcommon.eir_ad.entry.3ds_legacy.fixed.4",
FT_BOOLEAN, 8, NULL, 0x10,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_3ds_legacy_ignored_5,
{ "3DS Legacy Ignored", "btcommon.eir_ad.entry.3ds_legacy.ignored.5",
FT_BOOLEAN, 8, NULL, 0x20,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_3ds_legacy_fixed_6,
{ "3DS Legacy Fixed", "btcommon.eir_ad.entry.3ds_legacy.fixed.4",
FT_BOOLEAN, 8, NULL, 0x40,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_3ds_legacy_test_mode,
{ "3DS Legacy Test Mode", "btcommon.eir_ad.entry.3ds_legacy.test_mode",
FT_BOOLEAN, 8, NULL, 0x80,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_3ds_legacy_path_loss_threshold,
{ "3DS Legacy Path Loss Threshold", "btcommon.eir_ad.entry.3ds_legacy.path_loss_threshold",
FT_UINT8, BASE_DEC, NULL, 0x00,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_advertising_interval,
{ "Advertising Interval", "btcommon.eir_ad.entry.advertising_interval",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_appearance,
{ "Appearance", "btcommon.eir_ad.entry.appearance",
FT_UINT16, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_appearance_vals_ext, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_hash_c,
{"Hash C", "btcommon.eir_ad.entry.hash_c",
FT_BYTES, BASE_NONE, NULL, 0x0,
NULL, HFILL}
},
{ &hf_btcommon_eir_ad_randomizer_r,
{"Randomizer R", "btcommon.eir_ad.entry.randomizer_r",
FT_BYTES, BASE_NONE, NULL, 0x0,
NULL, HFILL}
},
{ &hf_btcommon_eir_ad_oob_flags_reserved,
{ "Reserved", "btcommon.eir_ad.entry.oob_flags.oob_reserved",
FT_UINT8, BASE_HEX, NULL, 0xF0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_oob_flags_address_type,
{ "Address Type", "btcommon.eir_ad.entry.oob_flags.oob_address_type",
FT_UINT8, BASE_HEX, VALS(bthci_cmd_address_types_vals), 0x08,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_oob_flags_le_bredr_support,
{ "Simultaneous LE and BR/EDR to Same Device Capable (Host)", "btcommon.eir_ad.entry.oob_flags.oob_le_bredr_support",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x04,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_oob_flags_le_supported_host,
{ "LE Supported By Host", "btcommon.eir_ad.entry.oob_flags.oob_le_supported_host",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x02,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_oob_flags_data_present,
{ "OOB Data Present", "btcommon.eir_ad.entry.oob_flags.oob_data_present",
FT_UINT8, BASE_HEX, VALS(cmd_boolean), 0x01,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_connection_interval_min,
{ "Connection Interval Min", "btcommon.eir_ad.entry.connection_interval_min",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_eir_ad_connection_interval_max,
{ "Connection Interval Max", "btcommon.eir_ad.entry.connection_interval_max",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_cod_class_of_device,
{ "Class of Device", "btcommon.cod.class_of_device",
FT_UINT24, BASE_HEX, NULL, 0x0,
NULL, HFILL }
},
{ &hf_btcommon_cod_major_service_class_information,
{ "Major Service Classes: Information", "btcommon.cod.major_service_classes.information",
FT_BOOLEAN, 16, NULL, 0x8000,
NULL, HFILL }
},
{ &hf_btcommon_cod_major_service_class_telephony,
{ "Major Service Classes: Telephony", "btcommon.cod.major_service_classes.telephony",
FT_BOOLEAN, 16, NULL, 0x4000,
NULL, HFILL }
},
{ &hf_btcommon_cod_major_service_class_audio,
{ "Major Service Classes: Audio", "btcommon.cod.major_service_classes.audio",
FT_BOOLEAN, 16, NULL, 0x2000,
NULL, HFILL }
},
{ &hf_btcommon_cod_major_service_class_object_transfer,
{ "Major Service Classes: Object Transfer", "btcommon.cod.major_service_classes.object_transfer",
FT_BOOLEAN, 16, NULL, 0x1000,
NULL, HFILL }
},
{ &hf_btcommon_cod_major_service_class_capturing,
{ "Major Service Classes: Capturing", "btcommon.cod.major_service_classes.capturing",
FT_BOOLEAN, 16, NULL, 0x0800,
NULL, HFILL }
},
{ &hf_btcommon_cod_major_service_class_rendering,
{ "Major Service Classes: Rendering", "btcommon.cod.major_service_classes.rendering",
FT_BOOLEAN, 16, NULL, 0x0400,
NULL, HFILL }
},
{ &hf_btcommon_cod_major_service_class_networking,
{ "Major Service Classes: Networking", "btcommon.cod.major_service_classes.networking",
FT_BOOLEAN, 16, NULL, 0x0200,
NULL, HFILL }
},
{ &hf_btcommon_cod_major_service_class_positioning,
{ "Major Service Classes: Positioning", "btcommon.cod.major_service_classes.positioning",
FT_BOOLEAN, 16, NULL, 0x0100,
NULL, HFILL }
},
{ &hf_btcommon_cod_major_service_class_reserved,
{ "Major Service Classes: Reserved", "btcommon.cod.major_service_classes.reserved",
FT_UINT16, BASE_HEX, NULL, 0x00C0,
NULL, HFILL }
},
{ &hf_btcommon_cod_major_service_class_limited_discoverable_mode,
{ "Major Service Classes: Limited Discoverable Mode", "btcommon.cod.major_service_classes.limited_discoverable_mode",
FT_BOOLEAN, 16, NULL, 0x0020,
NULL, HFILL }
},
{ &hf_btcommon_cod_major_device_class,
{ "Major Device Class", "btcommon.cod.major_device_class",
FT_UINT16, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_cod_major_device_class_vals_ext, 0x1F,
NULL, HFILL }
},
{ &hf_btcommon_cod_minor_device_class_computer,
{ "Minor Device Class", "btcommon.cod.minor_device_class",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_cod_minor_device_class_computer_vals_ext, 0xFC,
NULL, HFILL }
},
{ &hf_btcommon_cod_minor_device_class_phone,
{ "Minor Device Class", "btcommon.cod.minor_device_class",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_cod_minor_device_class_phone_vals_ext, 0xFC,
NULL, HFILL }
},
{ &hf_btcommon_cod_minor_device_class_lan_net_load_factor,
{ "Minor Device Class: Load Factor", "btcommon.cod.minor_device_class.load_factor",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_cod_minor_device_class_lan_net_load_factor_vals_ext, 0xE0,
NULL, HFILL }
},
{ &hf_btcommon_cod_minor_device_class_lan_net_type,
{ "Minor Device Class: Type", "btcommon.cod.minor_device_class.type",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_cod_minor_device_class_lan_net_type_vals_ext, 0x1C,
NULL, HFILL }
},
{ &hf_btcommon_cod_minor_device_class_audio_video,
{ "Minor Device Class", "btcommon.cod.minor_device_class",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_cod_minor_device_class_audio_video_vals_ext, 0xFC,
NULL, HFILL }
},
{ &hf_btcommon_cod_minor_device_class_peripheral_class,
{ "Minor Device Class", "btcommon.cod.minor_device_class",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_cod_minor_device_class_peripheral_class_vals_ext, 0xC0,
NULL, HFILL }
},
{ &hf_btcommon_cod_minor_device_class_peripheral_type,
{ "Minor Device Class", "btcommon.cod.minor_device_class",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_cod_minor_device_class_peripheral_type_vals_ext, 0x3C,
NULL, HFILL }
},
{ &hf_btcommon_cod_minor_device_class_imaging_class_printer,
{ "Minor Device Class: Class: Printer", "btcommon.cod.minor_device_class.class.printer",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_cod_minor_device_class_imaging_type_vals_ext, 0x80,
NULL, HFILL }
},
{ &hf_btcommon_cod_minor_device_class_imaging_class_scanner,
{ "Minor Device Class: Class: Scanner", "btcommon.cod.minor_device_class.class.scanner",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_cod_minor_device_class_imaging_type_vals_ext, 0x40,
NULL, HFILL }
},
{ &hf_btcommon_cod_minor_device_class_imaging_class_camera,
{ "Minor Device Class: Class: Camera", "btcommon.cod.minor_device_class.class.camera",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_cod_minor_device_class_imaging_type_vals_ext, 0x20,
NULL, HFILL }
},
{ &hf_btcommon_cod_minor_device_class_imaging_class_display,
{ "Minor Device Class: Class: Display", "btcommon.cod.minor_device_class.class.display",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_cod_minor_device_class_imaging_type_vals_ext, 0x10,
NULL, HFILL }
},
{ &hf_btcommon_cod_minor_device_class_imaging_type,
{ "Minor Device Class: Type", "btcommon.cod.minor_device_class.type",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_cod_minor_device_class_imaging_type_vals_ext, 0x0C,
NULL, HFILL }
},
{ &hf_btcommon_cod_minor_device_class_wearable,
{ "Minor Device Class", "btcommon.cod.minor_device_class",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_cod_minor_device_class_wearable_vals_ext, 0xFC,
NULL, HFILL }
},
{ &hf_btcommon_cod_minor_device_class_toy,
{ "Minor Device Class", "btcommon.cod.minor_device_class",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_cod_minor_device_class_toy_vals_ext, 0xFC,
NULL, HFILL }
},
{ &hf_btcommon_cod_minor_device_class_health,
{ "Minor Device Class", "btcommon.cod.minor_device_class",
FT_UINT8, BASE_HEX | BASE_EXT_STRING, &bthci_cmd_cod_minor_device_class_health_vals_ext, 0xFC,
NULL, HFILL }
},
{ &hf_btcommon_cod_minor_device_class_unknown,
{ "Minor Device Class", "btcommon.cod.minor_device_class",
FT_UINT8, BASE_HEX, NULL, 0xFC,
NULL, HFILL }
},
{ &hf_btcommon_cod_format_type,
{ "Format Type", "btcommon.cod.format_type",
FT_UINT8, BASE_HEX, NULL, 0x03,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_39,
{ "RF Channel 39 (2480 MHz - Reserved for Advertising - 39)", "btcommon.le_channel_map.39",
FT_BOOLEAN, 8, NULL, 0x80,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_38,
{ "RF Channel 12 (2426 MHz - Reserved for Advertising - 38)", "btcommon.le_channel_map.38",
FT_BOOLEAN, 8, NULL, 0x40,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_37,
{ "RF Channel 0 (2402 MHz - Reserved for Advertising - 37)", "btcommon.le_channel_map.37",
FT_BOOLEAN, 8, NULL, 0x20,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_36,
{ "RF Channel 38 (2478 MHz - Data - 36)", "btcommon.le_channel_map.36",
FT_BOOLEAN, 8, NULL, 0x10,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_35,
{ "RF Channel 37 (2476 MHz - Data - 35)", "btcommon.le_channel_map.35",
FT_BOOLEAN, 8, NULL, 0x08,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_34,
{ "RF Channel 36 (2474 MHz - Data - 34)", "btcommon.le_channel_map.34",
FT_BOOLEAN, 8, NULL, 0x04,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_33,
{ "RF Channel 35 (2472 MHz - Data - 33)", "btcommon.le_channel_map.33",
FT_BOOLEAN, 8, NULL, 0x02,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_32,
{ "RF Channel 34 (2470 MHz - Data - 32)", "btcommon.le_channel_map.32",
FT_BOOLEAN, 8, NULL, 0x01,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_31,
{ "RF Channel 33 (2468 MHz - Data - 31)", "btcommon.le_channel_map.31",
FT_BOOLEAN, 8, NULL, 0x80,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_30,
{ "RF Channel 32 (2466 MHz - Data - 30)", "btcommon.le_channel_map.30",
FT_BOOLEAN, 8, NULL, 0x40,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_29,
{ "RF Channel 31 (2464 MHz - Data - 29)", "btcommon.le_channel_map.29",
FT_BOOLEAN, 8, NULL, 0x20,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_28,
{ "RF Channel 30 (2462 MHz - Data - 28)", "btcommon.le_channel_map.28",
FT_BOOLEAN, 8, NULL, 0x10,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_27,
{ "RF Channel 29 (2460 MHz - Data - 27)", "btcommon.le_channel_map.27",
FT_BOOLEAN, 8, NULL, 0x08,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_26,
{ "RF Channel 28 (2458 MHz - Data - 26)", "btcommon.le_channel_map.26",
FT_BOOLEAN, 8, NULL, 0x04,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_25,
{ "RF Channel 27 (2456 MHz - Data - 25)", "btcommon.le_channel_map.25",
FT_BOOLEAN, 8, NULL, 0x02,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_24,
{ "RF Channel 26 (2454 MHz - Data - 24)", "btcommon.le_channel_map.24",
FT_BOOLEAN, 8, NULL, 0x01,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_23,
{ "RF Channel 25 (2452 MHz - Data - 23)", "btcommon.le_channel_map.23",
FT_BOOLEAN, 8, NULL, 0x80,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_22,
{ "RF Channel 24 (2450 MHz - Data - 22)", "btcommon.le_channel_map.22",
FT_BOOLEAN, 8, NULL, 0x40,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_21,
{ "RF Channel 23 (2448 MHz - Data - 21)", "btcommon.le_channel_map.21",
FT_BOOLEAN, 8, NULL, 0x20,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_20,
{ "RF Channel 22 (2446 MHz - Data - 20)", "btcommon.le_channel_map.20",
FT_BOOLEAN, 8, NULL, 0x10,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_19,
{ "RF Channel 21 (2444 MHz - Data - 19)", "btcommon.le_channel_map.19",
FT_BOOLEAN, 8, NULL, 0x08,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_18,
{ "RF Channel 20 (2442 MHz - Data - 18)", "btcommon.le_channel_map.18",
FT_BOOLEAN, 8, NULL, 0x04,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_17,
{ "RF Channel 19 (2440 MHz - Data - 17)", "btcommon.le_channel_map.17",
FT_BOOLEAN, 8, NULL, 0x02,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_16,
{ "RF Channel 18 (2438 MHz - Data - 16)", "btcommon.le_channel_map.16",
FT_BOOLEAN, 8, NULL, 0x01,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_15,
{ "RF Channel 17 (2436 MHz - Data - 15)", "btcommon.le_channel_map.15",
FT_BOOLEAN, 8, NULL, 0x80,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_14,
{ "RF Channel 16 (2434 MHz - Data - 14)", "btcommon.le_channel_map.14",
FT_BOOLEAN, 8, NULL, 0x40,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_13,
{ "RF Channel 15 (2432 MHz - Data - 13)", "btcommon.le_channel_map.13",
FT_BOOLEAN, 8, NULL, 0x20,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_12,
{ "RF Channel 14 (2430 MHz - Data - 12)", "btcommon.le_channel_map.12",
FT_BOOLEAN, 8, NULL, 0x10,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_11,
{ "RF Channel 13 (2428 MHz - Data - 11)", "btcommon.le_channel_map.11",
FT_BOOLEAN, 8, NULL, 0x08,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_10,
{ "RF Channel 11 (2424 MHz - Data - 10)", "btcommon.le_channel_map.10",
FT_BOOLEAN, 8, NULL, 0x04,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_9,
{ "RF Channel 10 (2422 MHz - Data - 9)", "btcommon.le_channel_map.9",
FT_BOOLEAN, 8, NULL, 0x02,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_8,
{ "RF Channel 9 (2420 MHz - Data - 8)", "btcommon.le_channel_map.8",
FT_BOOLEAN, 8, NULL, 0x01,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_7,
{ "RF Channel 8 (2418 MHz - Data - 7)", "btcommon.le_channel_map.7",
FT_BOOLEAN, 8, NULL, 0x80,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_6,
{ "RF Channel 7 (2416 MHz - Data - 6)", "btcommon.le_channel_map.6",
FT_BOOLEAN, 8, NULL, 0x40,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_5,
{ "RF Channel 6 (2414 MHz - Data - 5)", "btcommon.le_channel_map.5",
FT_BOOLEAN, 8, NULL, 0x20,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_4,
{ "RF Channel 5 (2412 MHz - Data - 4)", "btcommon.le_channel_map.4",
FT_BOOLEAN, 8, NULL, 0x10,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_3,
{ "RF Channel 4 (2410 MHz - Data - 3)", "btcommon.le_channel_map.3",
FT_BOOLEAN, 8, NULL, 0x08,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_2,
{ "RF Channel 3 (2408 MHz - Data - 2)", "btcommon.le_channel_map.2",
FT_BOOLEAN, 8, NULL, 0x04,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_1,
{ "RF Channel 2 (2406 MHz - Data - 1)", "btcommon.le_channel_map.1",
FT_BOOLEAN, 8, NULL, 0x02,
NULL, HFILL }
},
{ &hf_btcommon_le_channel_map_0,
{ "RF Channel 1 (2404 MHz - Data - 0)", "btcommon.le_channel_map.0",
FT_BOOLEAN, 8, NULL, 0x01,
NULL, HFILL }
}
};
static gint *ett[] = {
&ett_eir_ad,
&ett_eir_ad_entry,
};
static ei_register_info ei[] = {
{ &ei_eir_ad_undecoded, { "btcommon.eir_ad.undecoded", PI_PROTOCOL, PI_UNDECODED, "Undecoded", EXPFILL }},
{ &ei_eir_ad_unknown, { "btcommon.eir_ad.unknown", PI_PROTOCOL, PI_WARN, "Unknown data", EXPFILL }},
};
proto_btcommon = proto_register_protocol("Bluetooth Common", "BT Common", "btcommon");
proto_register_field_array(proto_btcommon, hf, array_length(hf));
proto_register_subtree_array(ett, array_length(ett));
expert_module = expert_register_protocol(proto_btcommon);
expert_register_field_array(expert_module, ei, array_length(ei));
btcommon_ad_handle = new_register_dissector("btcommon.eir_ad.ad", dissect_btcommon_ad, proto_btcommon);
btcommon_eir_handle = new_register_dissector("btcommon.eir_ad.eir", dissect_btcommon_eir, proto_btcommon);
btcommon_cod_handle = new_register_dissector("btcommon.cod", dissect_btcommon_cod, proto_btcommon);
btcommon_le_channel_map_handle = new_register_dissector("btcommon.le_channel_map", dissect_btcommon_le_channel_map, proto_btcommon);
}
