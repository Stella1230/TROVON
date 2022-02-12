static void
lte_rrc_timeConnFailure_r10_fmt(gchar *s, guint32 v)
{
g_snprintf(s, ITEM_LABEL_LENGTH, "%ums (%u)", 100*v, v);
}
static void
lte_rrc_ue_RxTxTimeDiffResult_fmt(gchar *s, guint32 v)
{
if (v == 0) {
g_snprintf(s, ITEM_LABEL_LENGTH, "T < 2Ts (0)");
} else if (v < 2048) {
g_snprintf(s, ITEM_LABEL_LENGTH, "%uTs <= T < %uTs (%u)", v*2, (v+1)*2, v);
} else if (v < 4095) {
g_snprintf(s, ITEM_LABEL_LENGTH, "%uTs <= T < %uTs (%u)", (v*8)-12288, ((v+1)*8)-12288, v);
} else {
g_snprintf(s, ITEM_LABEL_LENGTH, "20472Ts <= T (4095)");
}
}
static lte_rrc_private_data_t* lte_rrc_get_private_data(asn1_ctx_t *actx)
{
if (actx->private_data != NULL) {
return (lte_rrc_private_data_t*)actx->private_data;
}
else {
lte_rrc_private_data_t* new_struct =
(lte_rrc_private_data_t*)wmem_alloc0(wmem_packet_scope(), sizeof(lte_rrc_private_data_t));
actx->private_data = new_struct;
return new_struct;
}
}
static drx_config_t* private_data_get_drx_config(asn1_ctx_t *actx)
{
lte_rrc_private_data_t *private_data = (lte_rrc_private_data_t*)lte_rrc_get_private_data(actx);
return &private_data->drx_config;
}
static drb_mapping_t* private_data_get_drb_mapping(asn1_ctx_t *actx)
{
lte_rrc_private_data_t *private_data = (lte_rrc_private_data_t*)lte_rrc_get_private_data(actx);
return &private_data->drb_mapping;
}
static guint8 private_data_get_rat_type(asn1_ctx_t *actx)
{
lte_rrc_private_data_t *private_data = (lte_rrc_private_data_t*)lte_rrc_get_private_data(actx);
return private_data->rat_type;
}
static void private_data_set_rat_type(asn1_ctx_t *actx, guint8 rat_type)
{
lte_rrc_private_data_t *private_data = (lte_rrc_private_data_t*)lte_rrc_get_private_data(actx);
private_data->rat_type = rat_type;
}
static guint8 private_data_get_rat_target_type(asn1_ctx_t *actx)
{
lte_rrc_private_data_t *private_data = (lte_rrc_private_data_t*)lte_rrc_get_private_data(actx);
return private_data->target_rat_type;
}
static void private_data_set_rat_target_type(asn1_ctx_t *actx, guint8 target_rat_type)
{
lte_rrc_private_data_t *private_data = (lte_rrc_private_data_t*)lte_rrc_get_private_data(actx);
private_data->target_rat_type = target_rat_type;
}
static guint8 private_data_get_si_or_psi_geran(asn1_ctx_t *actx)
{
lte_rrc_private_data_t *private_data = (lte_rrc_private_data_t*)lte_rrc_get_private_data(actx);
return private_data->si_or_psi_geran;
}
static void private_data_set_si_or_psi_geran(asn1_ctx_t *actx, guint8 si_or_psi_geran)
{
lte_rrc_private_data_t *private_data = (lte_rrc_private_data_t*)lte_rrc_get_private_data(actx);
private_data->si_or_psi_geran = si_or_psi_geran;
}
static guint16 private_data_get_message_identifier(asn1_ctx_t *actx)
{
lte_rrc_private_data_t *private_data = (lte_rrc_private_data_t*)lte_rrc_get_private_data(actx);
return private_data->message_identifier;
}
static void private_data_set_message_identifier(asn1_ctx_t *actx, guint16 message_identifier)
{
lte_rrc_private_data_t *private_data = (lte_rrc_private_data_t*)lte_rrc_get_private_data(actx);
private_data->message_identifier = message_identifier;
}
static guint8 private_data_get_ra_preambles(asn1_ctx_t *actx)
{
lte_rrc_private_data_t *private_data = (lte_rrc_private_data_t*)lte_rrc_get_private_data(actx);
return private_data->ra_preambles;
}
static void private_data_set_ra_preambles(asn1_ctx_t *actx, guint8 ra_preambles)
{
lte_rrc_private_data_t *private_data = (lte_rrc_private_data_t*)lte_rrc_get_private_data(actx);
private_data->ra_preambles = ra_preambles;
}
static pdcp_security_info_t* private_data_pdcp_security_algorithms(asn1_ctx_t *actx)
{
lte_rrc_private_data_t *private_data = (lte_rrc_private_data_t*)lte_rrc_get_private_data(actx);
return &private_data->pdcp_security;
}
static meas_capabilities_item_band_mappings_t* private_data_meas_capabilities_item_band_mappings(asn1_ctx_t *actx)
{
lte_rrc_private_data_t *private_data = (lte_rrc_private_data_t*)lte_rrc_get_private_data(actx);
return &private_data->meas_capabilities_item_band_mappings;
}
static void set_freq_band_indicator(guint32 value, asn1_ctx_t *actx)
{
meas_capabilities_item_band_mappings_t *mappings = private_data_meas_capabilities_item_band_mappings(actx);
if (mappings->number_of_bands_set < 256) {
mappings->band_by_item[mappings->number_of_bands_set++] = (guint16)value;
}
}
static void
lte_rrc_localTimeOffset_fmt(gchar *s, guint32 v)
{
gint32 time_offset = (gint32) v;
g_snprintf(s, ITEM_LABEL_LENGTH, "UTC time %c %dhr %dmin (%d)",
(time_offset < 0) ? '-':'+', abs(time_offset) >> 2,
(abs(time_offset) & 0x03) * 15, time_offset);
}
static void
dissect_lte_rrc_warningMessageSegment(tvbuff_t *warning_msg_seg_tvb, proto_tree *tree, packet_info *pinfo, guint8 dataCodingScheme)
{
guint32 offset;
guint8 nb_of_pages, length, *str;
proto_item *ti;
tvbuff_t *cb_data_page_tvb, *cb_data_tvb;
int i;
nb_of_pages = tvb_get_guint8(warning_msg_seg_tvb, 0);
ti = proto_tree_add_uint(tree, hf_lte_rrc_warningMessageSegment_nb_pages, warning_msg_seg_tvb, 0, 1, nb_of_pages);
if (nb_of_pages > 15) {
expert_add_info_format(pinfo, ti, &ei_lte_rrc_number_pages_le15,
"Number of pages should be <=15 (found %u)", nb_of_pages);
nb_of_pages = 15;
}
for (i = 0, offset = 1; i < nb_of_pages; i++) {
length = tvb_get_guint8(warning_msg_seg_tvb, offset+82);
cb_data_page_tvb = tvb_new_subset_length(warning_msg_seg_tvb, offset, length);
cb_data_tvb = dissect_cbs_data(dataCodingScheme, cb_data_page_tvb, tree, pinfo, 0);
if (cb_data_tvb) {
str = tvb_get_string_enc(wmem_packet_scope(), cb_data_tvb, 0, tvb_reported_length(cb_data_tvb), ENC_UTF_8|ENC_NA);
proto_tree_add_string_format(tree, hf_lte_rrc_warningMessageSegment_decoded_page, warning_msg_seg_tvb, offset, 83,
str, "Decoded Page %u: %s", i+1, str);
}
offset += 83;
}
}
static void
dissect_lte_rrc_featureGroupIndicators(tvbuff_t *featureGroupIndicators_tvb, asn1_ctx_t *actx)
{
proto_tree *subtree;
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_featureGroupIndicators);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_1, featureGroupIndicators_tvb, 0, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_2, featureGroupIndicators_tvb, 1, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_3, featureGroupIndicators_tvb, 2, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_4, featureGroupIndicators_tvb, 3, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_5, featureGroupIndicators_tvb, 4, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_6, featureGroupIndicators_tvb, 5, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_7, featureGroupIndicators_tvb, 6, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_8, featureGroupIndicators_tvb, 7, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_9, featureGroupIndicators_tvb, 8, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_10, featureGroupIndicators_tvb, 9, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_11, featureGroupIndicators_tvb, 10, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_12, featureGroupIndicators_tvb, 11, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_13, featureGroupIndicators_tvb, 12, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_14, featureGroupIndicators_tvb, 13, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_15, featureGroupIndicators_tvb, 14, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_16, featureGroupIndicators_tvb, 15, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_17, featureGroupIndicators_tvb, 16, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_18, featureGroupIndicators_tvb, 17, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_19, featureGroupIndicators_tvb, 18, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_20, featureGroupIndicators_tvb, 19, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_21, featureGroupIndicators_tvb, 20, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_22, featureGroupIndicators_tvb, 21, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_23, featureGroupIndicators_tvb, 22, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_24, featureGroupIndicators_tvb, 23, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_25, featureGroupIndicators_tvb, 24, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_26, featureGroupIndicators_tvb, 25, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_27, featureGroupIndicators_tvb, 26, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_28, featureGroupIndicators_tvb, 27, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_29, featureGroupIndicators_tvb, 28, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_30, featureGroupIndicators_tvb, 29, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_31, featureGroupIndicators_tvb, 30, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_32, featureGroupIndicators_tvb, 31, 1, ENC_BIG_ENDIAN);
}
static void
dissect_lte_rrc_featureGroupIndRel10(tvbuff_t *featureGroupIndRel10_tvb, asn1_ctx_t *actx)
{
proto_tree *subtree;
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_featureGroupIndRel10);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_101, featureGroupIndRel10_tvb, 0, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_102, featureGroupIndRel10_tvb, 1, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_103, featureGroupIndRel10_tvb, 2, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_104, featureGroupIndRel10_tvb, 3, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_105, featureGroupIndRel10_tvb, 4, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_106, featureGroupIndRel10_tvb, 5, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_107, featureGroupIndRel10_tvb, 6, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_108, featureGroupIndRel10_tvb, 7, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_109, featureGroupIndRel10_tvb, 8, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_110, featureGroupIndRel10_tvb, 9, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_111, featureGroupIndRel10_tvb, 10, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_112, featureGroupIndRel10_tvb, 11, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_113, featureGroupIndRel10_tvb, 12, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_114, featureGroupIndRel10_tvb, 13, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_115, featureGroupIndRel10_tvb, 14, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_116, featureGroupIndRel10_tvb, 15, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_117, featureGroupIndRel10_tvb, 16, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_118, featureGroupIndRel10_tvb, 17, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_119, featureGroupIndRel10_tvb, 18, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_120, featureGroupIndRel10_tvb, 19, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_121, featureGroupIndRel10_tvb, 20, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_122, featureGroupIndRel10_tvb, 21, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_123, featureGroupIndRel10_tvb, 22, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_124, featureGroupIndRel10_tvb, 23, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_125, featureGroupIndRel10_tvb, 24, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_126, featureGroupIndRel10_tvb, 25, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_127, featureGroupIndRel10_tvb, 26, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_128, featureGroupIndRel10_tvb, 27, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_129, featureGroupIndRel10_tvb, 28, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_130, featureGroupIndRel10_tvb, 29, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_131, featureGroupIndRel10_tvb, 30, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_132, featureGroupIndRel10_tvb, 31, 1, ENC_BIG_ENDIAN);
}
static void
dissect_lte_rrc_featureGroupIndRel9Add(tvbuff_t *featureGroupIndRel9Add_tvb, asn1_ctx_t *actx)
{
proto_tree *subtree;
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_featureGroupIndRel9Add);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_33, featureGroupIndRel9Add_tvb, 0, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_34, featureGroupIndRel9Add_tvb, 1, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_35, featureGroupIndRel9Add_tvb, 2, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_36, featureGroupIndRel9Add_tvb, 3, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_37, featureGroupIndRel9Add_tvb, 4, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_38, featureGroupIndRel9Add_tvb, 5, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_39, featureGroupIndRel9Add_tvb, 6, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_40, featureGroupIndRel9Add_tvb, 7, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_41, featureGroupIndRel9Add_tvb, 8, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_42, featureGroupIndRel9Add_tvb, 9, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_43, featureGroupIndRel9Add_tvb, 10, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_44, featureGroupIndRel9Add_tvb, 11, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_45, featureGroupIndRel9Add_tvb, 12, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_46, featureGroupIndRel9Add_tvb, 13, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_47, featureGroupIndRel9Add_tvb, 14, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_48, featureGroupIndRel9Add_tvb, 15, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_49, featureGroupIndRel9Add_tvb, 16, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_50, featureGroupIndRel9Add_tvb, 17, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_51, featureGroupIndRel9Add_tvb, 18, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_52, featureGroupIndRel9Add_tvb, 19, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_53, featureGroupIndRel9Add_tvb, 20, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_54, featureGroupIndRel9Add_tvb, 21, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_55, featureGroupIndRel9Add_tvb, 22, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_56, featureGroupIndRel9Add_tvb, 23, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_57, featureGroupIndRel9Add_tvb, 24, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_58, featureGroupIndRel9Add_tvb, 25, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_59, featureGroupIndRel9Add_tvb, 26, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_60, featureGroupIndRel9Add_tvb, 27, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_61, featureGroupIndRel9Add_tvb, 28, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_62, featureGroupIndRel9Add_tvb, 29, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_63, featureGroupIndRel9Add_tvb, 30, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_eutra_cap_feat_group_ind_64, featureGroupIndRel9Add_tvb, 31, 1, ENC_BIG_ENDIAN);
}
static guint32 drx_lookup_onDurationTimer(guint32 idx)
{
static const guint32 vals[] = {1,2,3,4,5,6,8,10,20,30,40,50,60,80,100,200};
if (idx < (sizeof(vals)/sizeof(guint32))) {
return vals[idx];
}
return (sizeof(vals)/(sizeof(guint32)) - 1);
}
static guint32 drx_lookup_inactivityTimer(guint32 idx)
{
static const guint32 vals[] = {
1,2,3,4,5,6,8,10,20,30,40,50,60,80,100,200,300, 500,750,1280,1920,2560,0
};
if (idx < (sizeof(vals)/sizeof(guint32))) {
return vals[idx];
}
return (sizeof(vals)/(sizeof(guint32)) - 1);
}
static guint32 drx_lookup_retransmissionTimer(guint32 idx)
{
static const guint32 vals[] = {1,2,4,6,8,16,24,33};
if (idx < (sizeof(vals)/sizeof(guint32))) {
return vals[idx];
}
return (sizeof(vals)/(sizeof(guint32)) - 1);
}
static guint32 drx_lookup_longCycle(guint32 idx)
{
static const guint32 vals[] = {
10,20,32,40,64,80,128,160,256,320,512,640,1024,1280,2048,2560
};
if (idx < (sizeof(vals)/sizeof(guint32))) {
return vals[idx];
}
return (sizeof(vals)/(sizeof(guint32)) - 1);
}
static guint32 drx_lookup_longCycle_v1130(guint32 idx)
{
static const guint32 vals[] = {
60,70
};
if (idx < (sizeof(vals)/sizeof(guint32))) {
return vals[idx];
}
return (sizeof(vals)/(sizeof(guint32)) - 1);
}
static guint32 drx_lookup_shortCycle(guint32 idx)
{
static const guint32 vals[] = {
2,5,8,10,16,20,32,40,64,80,128,160,256,320,512,640
};
if (idx < (sizeof(vals)/sizeof(guint32))) {
return vals[idx];
}
return (sizeof(vals)/(sizeof(guint32)) - 1);
}
static void drx_check_config_sane(drx_config_t *config, asn1_ctx_t *actx)
{
if (config->onDurationTimer >= config->longCycle) {
expert_add_info_format(actx->pinfo, actx->created_item, &ei_lte_rrc_invalid_drx_config,
"OnDurationTimer (%u) should be less than long cycle (%u)",
config->onDurationTimer, config->longCycle);
}
if (config->shortCycleConfigured) {
if (config->shortCycle >= config->longCycle) {
expert_add_info_format(actx->pinfo, actx->created_item, &ei_lte_rrc_invalid_drx_config,
"Short DRX cycle (%u) must be shorter than long cycle (%u)",
config->shortCycle, config->longCycle);
}
else if (config->shortCycle && ((config->longCycle % config->shortCycle) != 0)) {
expert_add_info_format(actx->pinfo, actx->created_item, &ei_lte_rrc_invalid_drx_config,
"Short DRX cycle (%u) must divide the long cycle (%u) exactly",
config->shortCycle, config->longCycle);
}
if (config->onDurationTimer >= config->shortCycle) {
expert_add_info_format(actx->pinfo, actx->created_item, &ei_lte_rrc_invalid_drx_config,
"OnDurationTimer (%u) should not be longer than the short cycle (%u)",
config->onDurationTimer, config->shortCycle);
}
}
}
static void sr_lookup_configindex(guint32 config_index, guint16 *periodicity, guint16 *offset)
{
if (config_index < 5) {
*periodicity = 5;
*offset = config_index;
} else if (config_index < 15) {
*periodicity = 10;
*offset = config_index - 5;
}
else if (config_index < 35) {
*periodicity = 20;
*offset = config_index - 15;
}
else if (config_index < 75) {
*periodicity = 40;
*offset = config_index - 35;
}
else if (config_index < 155) {
*periodicity = 80;
*offset = config_index - 75;
}
else if (config_index < 157) {
*periodicity = 2;
*offset = config_index - 155;
}
else {
*periodicity = 1;
*offset = 0;
}
}
static int
dissect_lte_rrc_T_dl_Bandwidth(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
6, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_phich_Duration(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_phich_Resource(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PHICH_Config(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PHICH_Config, PHICH_Config_sequence);
return offset;
}
static int
dissect_lte_rrc_T_systemFrameNumber(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *value_tvb;
guint32 sfn;
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
8, 8, FALSE, &value_tvb, NULL);
sfn = tvb_get_guint8(value_tvb, 0);
col_append_fstr(actx->pinfo->cinfo, COL_INFO, " (SFN=%u)", sfn);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
10, 10, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_MasterInformationBlock(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "MasterInformationBlock");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MasterInformationBlock, MasterInformationBlock_sequence);
return offset;
}
static int
dissect_lte_rrc_BCCH_BCH_MessageType(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_lte_rrc_MasterInformationBlock(tvb, offset, actx, tree, hf_index);
return offset;
}
static int
dissect_lte_rrc_BCCH_BCH_Message(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BCCH_BCH_Message, BCCH_BCH_Message_sequence);
return offset;
}
static int
dissect_lte_rrc_BOOLEAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_boolean(tvb, offset, actx, tree, hf_index, NULL);
return offset;
}
static int
dissect_lte_rrc_T_ac_BarringFactor(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_ac_BarringTime(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_5(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
5, 5, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_AC_BarringConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AC_BarringConfig, AC_BarringConfig_sequence);
return offset;
}
static int
dissect_lte_rrc_T_ac_BarringInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_ac_BarringInfo, T_ac_BarringInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_T_numberOfRA_Preambles(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint value;
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, &value, FALSE, 0, NULL);
private_data_set_ra_preambles(actx, value);
return offset;
}
static int
dissect_lte_rrc_T_sizeOfRA_PreamblesGroupA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint ra_value, value;
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
15, &value, FALSE, 0, NULL);
ra_value = private_data_get_ra_preambles(actx);
if (value > ra_value) {
expert_add_info_format(actx->pinfo, actx->created_item, &ei_lte_rrc_too_many_group_a_rapids,
"Group A size (%s) > Total RA size (%s)!",
val_to_str_const(value, lte_rrc_T_sizeOfRA_PreamblesGroupA_vals, "Unknown"),
val_to_str_const(ra_value, lte_rrc_T_numberOfRA_Preambles_vals, "Unknown"));
}
set_mac_lte_rapid_ranges((value+1)*4, (ra_value+1)*4);
private_data_set_ra_preambles(actx, 0);
return offset;
}
static int
dissect_lte_rrc_T_messageSizeGroupA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_messagePowerOffsetGroupB(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_preamblesGroupAConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_preamblesGroupAConfig, T_preamblesGroupAConfig_sequence);
return offset;
}
static int
dissect_lte_rrc_T_preambleInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_preambleInfo, T_preambleInfo_sequence);
private_data_set_ra_preambles(actx, 0);
return offset;
}
static int
dissect_lte_rrc_T_powerRampingStep(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_preambleInitialReceivedTargetPower(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PowerRampingParameters(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PowerRampingParameters, PowerRampingParameters_sequence);
return offset;
}
static int
dissect_lte_rrc_PreambleTransMax(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
11, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_ra_ResponseWindowSize(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_mac_ContentionResolutionTimer(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_ra_SupervisionInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_ra_SupervisionInfo, T_ra_SupervisionInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_1_8(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 8U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_RACH_ConfigCommon(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RACH_ConfigCommon, RACH_ConfigCommon_sequence);
return offset;
}
static int
dissect_lte_rrc_T_modificationPeriodCoeff(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_BCCH_Config(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BCCH_Config, BCCH_Config_sequence);
return offset;
}
static int
dissect_lte_rrc_T_defaultPagingCycle(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_nB(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PCCH_Config(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PCCH_Config, PCCH_Config_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_837(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 837U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_63(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 63U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_15(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 15U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_94(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 94U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_PRACH_ConfigInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PRACH_ConfigInfo, PRACH_ConfigInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_PRACH_ConfigSIB(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PRACH_ConfigSIB, PRACH_ConfigSIB_sequence);
return offset;
}
static int
dissect_lte_rrc_T_referenceSignalPower(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-60, 50U, NULL, FALSE);
proto_item_append_text(actx->created_item, "dBm");
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_3(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 3U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_PDSCH_ConfigCommon(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PDSCH_ConfigCommon, PDSCH_ConfigCommon_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_1_4(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 4U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_hoppingMode(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_98(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 98U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_pusch_ConfigBasic(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_pusch_ConfigBasic, T_pusch_ConfigBasic_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_29(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 29U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_7(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 7U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_UL_ReferenceSignalsPUSCH(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UL_ReferenceSignalsPUSCH, UL_ReferenceSignalsPUSCH_sequence);
return offset;
}
static int
dissect_lte_rrc_PUSCH_ConfigCommon(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PUSCH_ConfigCommon, PUSCH_ConfigCommon_sequence);
return offset;
}
static int
dissect_lte_rrc_T_deltaPUCCH_Shift(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
3, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_2047(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 2047U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_PUCCH_ConfigCommon(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PUCCH_ConfigCommon, PUCCH_ConfigCommon_sequence);
return offset;
}
static int
dissect_lte_rrc_NULL(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_null(tvb, offset, actx, tree, hf_index);
return offset;
}
static int
dissect_lte_rrc_T_srs_BandwidthConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_srs_SubframeConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_srs_MaxUpPts(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_26(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_26, T_setup_26_sequence);
return offset;
}
static int
dissect_lte_rrc_SoundingRS_UL_ConfigCommon(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SoundingRS_UL_ConfigCommon, SoundingRS_UL_ConfigCommon_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_p0_NominalPUSCH(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-126, 24U, NULL, FALSE);
proto_item_append_text(actx->created_item, "dBm");
return offset;
}
static int
dissect_lte_rrc_T_alpha(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_p0_NominalPUCCH(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-127, -96, NULL, FALSE);
proto_item_append_text(actx->created_item, "dBm");
return offset;
}
static int
dissect_lte_rrc_T_deltaF_PUCCH_Format1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
3, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_deltaF_PUCCH_Format1b(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
3, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_deltaF_PUCCH_Format2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_deltaF_PUCCH_Format2a(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
3, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_deltaF_PUCCH_Format2b(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
3, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_DeltaFList_PUCCH(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DeltaFList_PUCCH, DeltaFList_PUCCH_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_M1_6(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-1, 6U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_UplinkPowerControlCommon(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UplinkPowerControlCommon, UplinkPowerControlCommon_sequence);
return offset;
}
static int
dissect_lte_rrc_UL_CyclicPrefixLength(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_deltaF_PUCCH_Format3_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_deltaF_PUCCH_Format1bCS_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_UplinkPowerControlCommon_v1020(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UplinkPowerControlCommon_v1020, UplinkPowerControlCommon_v1020_sequence);
return offset;
}
static int
dissect_lte_rrc_RadioResourceConfigCommonSIB_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, RadioResourceConfigCommonSIB_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_RadioResourceConfigCommonSIB(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RadioResourceConfigCommonSIB, RadioResourceConfigCommonSIB_sequence);
return offset;
}
static int
dissect_lte_rrc_T_t300(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_t301(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_t310(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
7, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_n310(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_t311(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
7, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_n311(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_UE_TimersAndConstants(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_TimersAndConstants, UE_TimersAndConstants_sequence);
return offset;
}
static int
dissect_lte_rrc_ARFCN_ValueEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, maxEARFCN, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_ul_Bandwidth(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
6, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_AdditionalSpectrumEmission(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 32U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_freqInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_freqInfo, T_freqInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_T_radioframeAllocationPeriod(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
6, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_6(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
6, 6, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_24(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
24, 24, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_T_subframeAllocation(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_subframeAllocation, T_subframeAllocation_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_MBSFN_SubframeConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBSFN_SubframeConfig, MBSFN_SubframeConfig_sequence);
return offset;
}
static int
dissect_lte_rrc_MBSFN_SubframeConfigList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBSFN_SubframeConfigList, MBSFN_SubframeConfigList_sequence_of,
1, maxMBSFN_Allocations, FALSE);
return offset;
}
static int
dissect_lte_rrc_TimeAlignmentTimer(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_lateNonCriticalExtension_03(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string_containing_pdu_new(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, dissect_SystemInformationBlockType2_v8h0_IEs_PDU);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType2_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, SystemInformationBlockType2_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType2_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, SystemInformationBlockType2_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SIB2 ");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType2, SystemInformationBlockType2_sequence);
return offset;
}
static int
dissect_lte_rrc_T_q_Hyst(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_t_Evaluation(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_t_HystNormal(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_1_16(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 16U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_MobilityStateParameters(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MobilityStateParameters, MobilityStateParameters_sequence);
return offset;
}
static int
dissect_lte_rrc_T_sf_Medium(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_sf_High(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_q_HystSF(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_q_HystSF, T_q_HystSF_sequence);
return offset;
}
static int
dissect_lte_rrc_T_speedStateReselectionPars(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_speedStateReselectionPars, T_speedStateReselectionPars_sequence);
return offset;
}
static int
dissect_lte_rrc_T_cellReselectionInfoCommon(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_cellReselectionInfoCommon, T_cellReselectionInfoCommon_sequence);
return offset;
}
static int
dissect_lte_rrc_ReselectionThreshold(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 31U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_CellReselectionPriority(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 7U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_cellReselectionServingFreqInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_cellReselectionServingFreqInfo, T_cellReselectionServingFreqInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_Q_RxLevMin(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-70, -22, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_P_Max(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-30, 33U, NULL, FALSE);
proto_item_append_text(actx->created_item, "dBm");
return offset;
}
static int
dissect_lte_rrc_AllowedMeasBandwidth(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
6, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PresenceAntennaPort1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_boolean(tvb, offset, actx, tree, hf_index, NULL);
return offset;
}
static int
dissect_lte_rrc_NeighCellConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *neigh_cell_config_tvb = NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, -1,
2, 2, FALSE, &neigh_cell_config_tvb, NULL);
if (neigh_cell_config_tvb) {
actx->created_item = proto_tree_add_uint(tree, hf_index, neigh_cell_config_tvb, 0, 1, tvb_get_bits8(neigh_cell_config_tvb, 0, 2));
}
return offset;
}
static int
dissect_lte_rrc_T_Reselection(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 7U, NULL, FALSE);
proto_item_append_text(actx->created_item, "s");
return offset;
}
static int
dissect_lte_rrc_T_sf_Medium_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_sf_High_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_SpeedStateScaleFactors(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SpeedStateScaleFactors, SpeedStateScaleFactors_sequence);
return offset;
}
static int
dissect_lte_rrc_T_intraFreqCellReselectionInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_intraFreqCellReselectionInfo, T_intraFreqCellReselectionInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_OCTET_STRING(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, NULL);
return offset;
}
static int
dissect_lte_rrc_ReselectionThresholdQ_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 31U, NULL, FALSE);
proto_item_append_text(actx->created_item, "dB");
return offset;
}
static int
dissect_lte_rrc_T_s_IntraSearch_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_s_IntraSearch_v920, T_s_IntraSearch_v920_sequence);
return offset;
}
static int
dissect_lte_rrc_T_s_NonIntraSearch_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_s_NonIntraSearch_v920, T_s_NonIntraSearch_v920_sequence);
return offset;
}
static int
dissect_lte_rrc_Q_QualMin_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-34, -3, NULL, FALSE);
proto_item_append_text(actx->created_item, "dB");
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType3_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, SystemInformationBlockType3_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType3_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, SystemInformationBlockType3_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType3(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SIB3 ");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType3, SystemInformationBlockType3_sequence);
return offset;
}
static int
dissect_lte_rrc_PhysCellId(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 503U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_Q_OffsetRange(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
31, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_IntraFreqNeighCellInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IntraFreqNeighCellInfo, IntraFreqNeighCellInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_IntraFreqNeighCellList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IntraFreqNeighCellList, IntraFreqNeighCellList_sequence_of,
1, maxCellIntra, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_range(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PhysCellIdRange(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PhysCellIdRange, PhysCellIdRange_sequence);
return offset;
}
static int
dissect_lte_rrc_IntraFreqBlackCellList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IntraFreqBlackCellList, IntraFreqBlackCellList_sequence_of,
1, maxCellBlack, FALSE);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType4(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SIB4 ");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType4, SystemInformationBlockType4_sequence);
return offset;
}
static int
dissect_lte_rrc_InterFreqNeighCellInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_InterFreqNeighCellInfo, InterFreqNeighCellInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_InterFreqNeighCellList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_InterFreqNeighCellList, InterFreqNeighCellList_sequence_of,
1, maxCellInter, FALSE);
return offset;
}
static int
dissect_lte_rrc_InterFreqBlackCellList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_InterFreqBlackCellList, InterFreqBlackCellList_sequence_of,
1, maxCellBlack, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_threshX_Q_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_threshX_Q_r9, T_threshX_Q_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_InterFreqCarrierFreqInfo_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, InterFreqCarrierFreqInfo_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_InterFreqCarrierFreqInfo_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, InterFreqCarrierFreqInfo_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_InterFreqCarrierFreqInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_InterFreqCarrierFreqInfo, InterFreqCarrierFreqInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_InterFreqCarrierFreqList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_InterFreqCarrierFreqList, InterFreqCarrierFreqList_sequence_of,
1, maxFreq, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_lateNonCriticalExtension_04(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string_containing_pdu_new(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, dissect_SystemInformationBlockType5_v8h0_IEs_PDU);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType5(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SIB5 ");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType5, SystemInformationBlockType5_sequence);
return offset;
}
static int
dissect_lte_rrc_ARFCN_ValueUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 16383U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_INTEGER_M60_M13(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-60, -13, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_p_MaxUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-50, 33U, NULL, FALSE);
proto_item_append_text(actx->created_item, "dBm");
return offset;
}
static int
dissect_lte_rrc_T_q_QualMin(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-24, 0U, NULL, FALSE);
proto_item_append_text(actx->created_item, "dB");
return offset;
}
static int
dissect_lte_rrc_T_threshX_Q_r9_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_threshX_Q_r9_01, T_threshX_Q_r9_01_sequence);
return offset;
}
static int
dissect_lte_rrc_CarrierFreqUTRA_FDD_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, CarrierFreqUTRA_FDD_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_CarrierFreqUTRA_FDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CarrierFreqUTRA_FDD, CarrierFreqUTRA_FDD_sequence);
return offset;
}
static int
dissect_lte_rrc_CarrierFreqListUTRA_FDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CarrierFreqListUTRA_FDD, CarrierFreqListUTRA_FDD_sequence_of,
1, maxUTRA_FDD_Carrier, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_p_MaxUTRA_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-50, 33U, NULL, FALSE);
proto_item_append_text(actx->created_item, "dBm");
return offset;
}
static int
dissect_lte_rrc_CarrierFreqUTRA_TDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CarrierFreqUTRA_TDD, CarrierFreqUTRA_TDD_sequence);
return offset;
}
static int
dissect_lte_rrc_CarrierFreqListUTRA_TDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CarrierFreqListUTRA_TDD, CarrierFreqListUTRA_TDD_sequence_of,
1, maxUTRA_TDD_Carrier, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_lateNonCriticalExtension_05(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string_containing_pdu_new(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, dissect_SystemInformationBlockType6_v8h0_IEs_PDU);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType6(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SIB6 ");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType6, SystemInformationBlockType6_sequence);
return offset;
}
static int
dissect_lte_rrc_ARFCN_ValueGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 1023U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_BandIndicatorGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_ExplicitListOfARFCNs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ExplicitListOfARFCNs, ExplicitListOfARFCNs_sequence_of,
0, 31, FALSE);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_31(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 31U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_equallySpacedARFCNs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_equallySpacedARFCNs, T_equallySpacedARFCNs_sequence);
return offset;
}
static int
dissect_lte_rrc_OCTET_STRING_SIZE_1_16(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
1, 16, FALSE, NULL);
return offset;
}
static int
dissect_lte_rrc_T_followingARFCNs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_followingARFCNs, T_followingARFCNs_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_CarrierFreqsGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CarrierFreqsGERAN, CarrierFreqsGERAN_sequence);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_8(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
8, 8, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_45(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 45U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_p_MaxGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 39U, NULL, FALSE);
proto_item_append_text(actx->created_item, "dBm");
return offset;
}
static int
dissect_lte_rrc_T_commonInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_commonInfo, T_commonInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_CarrierFreqsInfoGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CarrierFreqsInfoGERAN, CarrierFreqsInfoGERAN_sequence);
return offset;
}
static int
dissect_lte_rrc_CarrierFreqsInfoListGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CarrierFreqsInfoListGERAN, CarrierFreqsInfoListGERAN_sequence_of,
1, maxGNFG, FALSE);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType7(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SIB7 ");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType7, SystemInformationBlockType7_sequence);
return offset;
}
static int
dissect_lte_rrc_T_synchronousSystemTime(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *sync_system_time_tvb = NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
39, 39, FALSE, &sync_system_time_tvb, NULL);
if (sync_system_time_tvb) {
guint64 bits;
nstime_t ts;
proto_tree *subtree;
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_timeInfo);
bits = tvb_get_bits64(sync_system_time_tvb, 0, 39, ENC_BIG_ENDIAN);
ts.secs = (time_t)(bits/100) + 315964800;
ts.nsecs = (int)(bits%100)*10000000;
proto_tree_add_time(subtree, hf_lte_rrc_cdma_time, sync_system_time_tvb, 0, 5, &ts);
proto_tree_add_time(subtree, hf_lte_rrc_local_time, sync_system_time_tvb, 0, 5, &ts);
}
return offset;
}
static int
dissect_lte_rrc_T_asynchronousSystemTime(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *async_system_time_tvb = NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
49, 49, FALSE, &async_system_time_tvb, NULL);
if (async_system_time_tvb) {
guint64 bits;
nstime_t ts;
proto_tree *subtree;
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_timeInfo);
bits = tvb_get_bits64(async_system_time_tvb, 0, 49, ENC_BIG_ENDIAN);
ts.secs = (time_t)((bits*8)/1228800) + 315964800;
ts.nsecs = (int)(((bits%153600)*8*1000000000)/1228800);
proto_tree_add_time(subtree, hf_lte_rrc_cdma_time, async_system_time_tvb, 0, 7, &ts);
proto_tree_add_time(subtree, hf_lte_rrc_local_time, async_system_time_tvb, 0, 7,&ts);
}
return offset;
}
static int
dissect_lte_rrc_T_cdma_SystemTime(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_cdma_SystemTime, T_cdma_SystemTime_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_SystemTimeInfoCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemTimeInfoCDMA2000, SystemTimeInfoCDMA2000_sequence);
return offset;
}
static int
dissect_lte_rrc_PreRegistrationZoneIdHRPD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 255U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_SecondaryPreRegistrationZoneIdListHRPD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SecondaryPreRegistrationZoneIdListHRPD, SecondaryPreRegistrationZoneIdListHRPD_sequence_of,
1, 2, FALSE);
return offset;
}
static int
dissect_lte_rrc_PreRegistrationInfoHRPD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PreRegistrationInfoHRPD, PreRegistrationInfoHRPD_sequence);
return offset;
}
static int
dissect_lte_rrc_BandclassCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
32, NULL, TRUE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_BandClassInfoCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandClassInfoCDMA2000, BandClassInfoCDMA2000_sequence);
return offset;
}
static int
dissect_lte_rrc_BandClassListCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandClassListCDMA2000, BandClassListCDMA2000_sequence_of,
1, maxCDMA_BandClass, FALSE);
return offset;
}
static int
dissect_lte_rrc_ARFCN_ValueCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 2047U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_PhysCellIdCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, maxPNOffset, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_PhysCellIdListCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PhysCellIdListCDMA2000, PhysCellIdListCDMA2000_sequence_of,
1, 16, FALSE);
return offset;
}
static int
dissect_lte_rrc_NeighCellsPerBandclassCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_NeighCellsPerBandclassCDMA2000, NeighCellsPerBandclassCDMA2000_sequence);
return offset;
}
static int
dissect_lte_rrc_NeighCellsPerBandclassListCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_NeighCellsPerBandclassListCDMA2000, NeighCellsPerBandclassListCDMA2000_sequence_of,
1, 16, FALSE);
return offset;
}
static int
dissect_lte_rrc_NeighCellCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_NeighCellCDMA2000, NeighCellCDMA2000_sequence);
return offset;
}
static int
dissect_lte_rrc_NeighCellListCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_NeighCellListCDMA2000, NeighCellListCDMA2000_sequence_of,
1, 16, FALSE);
return offset;
}
static int
dissect_lte_rrc_CellReselectionParametersCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellReselectionParametersCDMA2000, CellReselectionParametersCDMA2000_sequence);
return offset;
}
static int
dissect_lte_rrc_T_parametersHRPD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_parametersHRPD, T_parametersHRPD_sequence);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_15(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
15, 15, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_16(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
16, 16, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_7(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
7, 7, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_12(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
12, 12, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_3(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
3, 3, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_CSFB_RegistrationParam1XRTT(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSFB_RegistrationParam1XRTT, CSFB_RegistrationParam1XRTT_sequence);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_42(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
42, 42, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_T_parameters1XRTT(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_parameters1XRTT, T_parameters1XRTT_sequence);
return offset;
}
static int
dissect_lte_rrc_PhysCellIdListCDMA2000_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PhysCellIdListCDMA2000_v920, PhysCellIdListCDMA2000_v920_sequence_of,
0, 24, FALSE);
return offset;
}
static int
dissect_lte_rrc_NeighCellsPerBandclassCDMA2000_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_NeighCellsPerBandclassCDMA2000_v920, NeighCellsPerBandclassCDMA2000_v920_sequence);
return offset;
}
static int
dissect_lte_rrc_NeighCellsPerBandclassListCDMA2000_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_NeighCellsPerBandclassListCDMA2000_v920, NeighCellsPerBandclassListCDMA2000_v920_sequence_of,
1, 16, FALSE);
return offset;
}
static int
dissect_lte_rrc_NeighCellCDMA2000_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_NeighCellCDMA2000_v920, NeighCellCDMA2000_v920_sequence);
return offset;
}
static int
dissect_lte_rrc_NeighCellListCDMA2000_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_NeighCellListCDMA2000_v920, NeighCellListCDMA2000_v920_sequence_of,
1, 16, FALSE);
return offset;
}
static int
dissect_lte_rrc_CellReselectionParametersCDMA2000_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellReselectionParametersCDMA2000_v920, CellReselectionParametersCDMA2000_v920_sequence);
return offset;
}
static int
dissect_lte_rrc_T_powerDownReg_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_CSFB_RegistrationParam1XRTT_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSFB_RegistrationParam1XRTT_v920, CSFB_RegistrationParam1XRTT_v920_sequence);
return offset;
}
static int
dissect_lte_rrc_AC_BarringConfig1XRTT_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AC_BarringConfig1XRTT_r9, AC_BarringConfig1XRTT_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType8_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, SystemInformationBlockType8_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_T_csfb_DualRxTxSupport_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType8_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, SystemInformationBlockType8_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_1_maxPLMN_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, maxPLMN_r11, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_systemTimeInfo_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_systemTimeInfo_r11, T_systemTimeInfo_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_40_OF_PhysCellIdCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_40_OF_PhysCellIdCDMA2000, SEQUENCE_SIZE_1_40_OF_PhysCellIdCDMA2000_sequence_of,
1, 40, FALSE);
return offset;
}
static int
dissect_lte_rrc_NeighCellsPerBandclassCDMA2000_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_NeighCellsPerBandclassCDMA2000_r11, NeighCellsPerBandclassCDMA2000_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_16_OF_NeighCellsPerBandclassCDMA2000_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_16_OF_NeighCellsPerBandclassCDMA2000_r11, SEQUENCE_SIZE_1_16_OF_NeighCellsPerBandclassCDMA2000_r11_sequence_of,
1, 16, FALSE);
return offset;
}
static int
dissect_lte_rrc_NeighCellCDMA2000_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_NeighCellCDMA2000_r11, NeighCellCDMA2000_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_16_OF_NeighCellCDMA2000_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_16_OF_NeighCellCDMA2000_r11, SEQUENCE_SIZE_1_16_OF_NeighCellCDMA2000_r11_sequence_of,
1, 16, FALSE);
return offset;
}
static int
dissect_lte_rrc_CellReselectionParametersCDMA2000_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellReselectionParametersCDMA2000_r11, CellReselectionParametersCDMA2000_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_parametersHRPD_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_parametersHRPD_r11, T_parametersHRPD_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_csfb_DualRxTxSupport_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_parameters1XRTT_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_parameters1XRTT_r11, T_parameters1XRTT_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_ParametersCDMA2000_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ParametersCDMA2000_r11, ParametersCDMA2000_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_parametersCDMA2000_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_parametersCDMA2000_r11, T_parametersCDMA2000_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_SIB8_PerPLMN_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SIB8_PerPLMN_r11, SIB8_PerPLMN_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_SIB8_PerPLMN_List_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SIB8_PerPLMN_List_r11, SIB8_PerPLMN_List_r11_sequence_of,
1, maxPLMN_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType8_eag_3(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, SystemInformationBlockType8_eag_3_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType8(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SIB8 ");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType8, SystemInformationBlockType8_sequence);
return offset;
}
static int
dissect_lte_rrc_T_hnb_Name(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *hnb_name_tvb = NULL;
offset = dissect_per_octet_string(tvb, offset, actx, tree, -1,
1, 48, FALSE, &hnb_name_tvb);
actx->created_item = proto_tree_add_item(tree, hf_index, hnb_name_tvb, 0, -1, ENC_UTF_8|ENC_NA);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SIB9 ");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType9, SystemInformationBlockType9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_messageIdentifier(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *msg_id_tvb = NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, -1,
16, 16, FALSE, &msg_id_tvb, NULL);
if (msg_id_tvb) {
actx->created_item = proto_tree_add_item(tree, hf_index, msg_id_tvb, 0, 2, ENC_BIG_ENDIAN);
}
return offset;
}
static int
dissect_lte_rrc_T_serialNumber(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *serial_nb_tvb = NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
16, 16, FALSE, &serial_nb_tvb, NULL);
if (serial_nb_tvb) {
proto_tree *subtree;
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_serialNumber);
proto_tree_add_item(subtree, hf_lte_rrc_serialNumber_gs, serial_nb_tvb, 0, 2, ENC_BIG_ENDIAN);
proto_tree_add_item(subtree, hf_lte_rrc_serialNumber_msg_code, serial_nb_tvb, 0, 2, ENC_BIG_ENDIAN);
proto_tree_add_item(subtree, hf_lte_rrc_serialNumber_upd_nb, serial_nb_tvb, 0, 2, ENC_BIG_ENDIAN);
}
return offset;
}
static int
dissect_lte_rrc_T_warningType(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *warning_type_tvb = NULL;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
2, 2, FALSE, &warning_type_tvb);
if (warning_type_tvb) {
proto_tree *subtree;
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_warningType);
proto_tree_add_item(subtree, hf_lte_rrc_warningType_value, warning_type_tvb, 0, 2, ENC_BIG_ENDIAN);
proto_tree_add_item(subtree, hf_lte_rrc_warningType_emergency_user_alert, warning_type_tvb, 0, 2, ENC_BIG_ENDIAN);
proto_tree_add_item(subtree, hf_lte_rrc_warningType_popup, warning_type_tvb, 0, 2, ENC_BIG_ENDIAN);
}
return offset;
}
static int
dissect_lte_rrc_OCTET_STRING_SIZE_50(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
50, 50, FALSE, NULL);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SIB10 ");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType10, SystemInformationBlockType10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_messageIdentifier_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *msg_id_tvb = NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, -1,
16, 16, FALSE, &msg_id_tvb, NULL);
if (msg_id_tvb) {
private_data_set_message_identifier(actx, tvb_get_ntohs(msg_id_tvb, 0) << 16);
actx->created_item = proto_tree_add_item(tree, hf_index, msg_id_tvb, 0, 2, ENC_BIG_ENDIAN);
}
return offset;
}
static int
dissect_lte_rrc_T_serialNumber_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *serial_nb_tvb = NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
16, 16, FALSE, &serial_nb_tvb, NULL);
if (serial_nb_tvb) {
proto_tree *subtree;
private_data_set_message_identifier(actx,
private_data_get_message_identifier(actx) | tvb_get_ntohs(serial_nb_tvb, 0));
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_serialNumber);
proto_tree_add_item(subtree, hf_lte_rrc_serialNumber_gs, serial_nb_tvb, 0, 2, ENC_BIG_ENDIAN);
proto_tree_add_item(subtree, hf_lte_rrc_serialNumber_msg_code, serial_nb_tvb, 0, 2, ENC_BIG_ENDIAN);
proto_tree_add_item(subtree, hf_lte_rrc_serialNumber_upd_nb, serial_nb_tvb, 0, 2, ENC_BIG_ENDIAN);
}
return offset;
}
static int
dissect_lte_rrc_T_warningMessageSegmentType(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_warningMessageSegment(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *warning_msg_seg_tvb = NULL;
gpointer p_dcs;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &warning_msg_seg_tvb);
p_dcs = g_hash_table_lookup(lte_rrc_etws_cmas_dcs_hash, GUINT_TO_POINTER((guint)private_data_get_message_identifier(actx)));
if (warning_msg_seg_tvb && p_dcs) {
proto_tree *subtree;
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_warningMessageSegment);
dissect_lte_rrc_warningMessageSegment(warning_msg_seg_tvb, subtree, actx->pinfo, GPOINTER_TO_UINT(p_dcs));
}
return offset;
}
static int
dissect_lte_rrc_T_dataCodingScheme(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *data_coding_scheme_tvb = NULL;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
1, 1, FALSE, &data_coding_scheme_tvb);
if (data_coding_scheme_tvb) {
proto_tree *subtree;
guint32 dataCodingScheme;
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_dataCodingScheme);
dataCodingScheme = dissect_cbs_data_coding_scheme(data_coding_scheme_tvb, actx->pinfo, subtree, 0);
g_hash_table_insert(lte_rrc_etws_cmas_dcs_hash, GUINT_TO_POINTER((guint)private_data_get_message_identifier(actx)),
GUINT_TO_POINTER(dataCodingScheme));
}
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SIB11 ");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType11, SystemInformationBlockType11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_messageIdentifier_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *msg_id_tvb = NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, -1,
16, 16, FALSE, &msg_id_tvb, NULL);
if (msg_id_tvb) {
private_data_set_message_identifier(actx, tvb_get_ntohs(msg_id_tvb, 0) << 16);
actx->created_item = proto_tree_add_item(tree, hf_index, msg_id_tvb, 0, 2, ENC_BIG_ENDIAN);
}
return offset;
}
static int
dissect_lte_rrc_T_serialNumber_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *serial_nb_tvb = NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
16, 16, FALSE, &serial_nb_tvb, NULL);
if (serial_nb_tvb) {
proto_tree *subtree;
private_data_set_message_identifier(actx,
private_data_get_message_identifier(actx) | tvb_get_ntohs(serial_nb_tvb, 0));
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_serialNumber);
proto_tree_add_item(subtree, hf_lte_rrc_serialNumber_gs, serial_nb_tvb, 0, 2, ENC_BIG_ENDIAN);
proto_tree_add_item(subtree, hf_lte_rrc_serialNumber_msg_code, serial_nb_tvb, 0, 2, ENC_BIG_ENDIAN);
proto_tree_add_item(subtree, hf_lte_rrc_serialNumber_upd_nb, serial_nb_tvb, 0, 2, ENC_BIG_ENDIAN);
}
return offset;
}
static int
dissect_lte_rrc_T_warningMessageSegmentType_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_warningMessageSegment_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *warning_msg_seg_tvb = NULL;
gpointer p_dcs;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &warning_msg_seg_tvb);
p_dcs = g_hash_table_lookup(lte_rrc_etws_cmas_dcs_hash, GUINT_TO_POINTER((guint)private_data_get_message_identifier(actx)));
if (warning_msg_seg_tvb && p_dcs) {
proto_tree *subtree;
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_warningMessageSegment);
dissect_lte_rrc_warningMessageSegment(warning_msg_seg_tvb, subtree, actx->pinfo, GPOINTER_TO_UINT(p_dcs));
}
return offset;
}
static int
dissect_lte_rrc_T_dataCodingScheme_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *data_coding_scheme_tvb = NULL;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
1, 1, FALSE, &data_coding_scheme_tvb);
if (data_coding_scheme_tvb) {
proto_tree *subtree;
guint32 dataCodingScheme;
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_dataCodingScheme);
dataCodingScheme = dissect_cbs_data_coding_scheme(data_coding_scheme_tvb, actx->pinfo, subtree, 0);
g_hash_table_insert(lte_rrc_etws_cmas_dcs_hash, GUINT_TO_POINTER((guint)private_data_get_message_identifier(actx)),
GUINT_TO_POINTER(dataCodingScheme));
}
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType12_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SIB12 ");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType12_r9, SystemInformationBlockType12_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_255(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 255U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_non_MBSFNregionLength(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_mcch_RepetitionPeriod_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 10U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_mcch_ModificationPeriod_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_signallingMCS_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_mcch_Config_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_mcch_Config_r9, T_mcch_Config_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_MBSFN_AreaInfo_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBSFN_AreaInfo_r9, MBSFN_AreaInfo_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_MBSFN_AreaInfoList_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBSFN_AreaInfoList_r9, MBSFN_AreaInfoList_r9_sequence_of,
1, maxMBSFN_Area, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_notificationRepetitionCoeff_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_1_6(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 6U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_MBMS_NotificationConfig_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBMS_NotificationConfig_r9, MBMS_NotificationConfig_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType13_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SIB13 ");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType13_r9, SystemInformationBlockType13_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_eab_Category_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
3, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_EAB_Config_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_EAB_Config_r11, EAB_Config_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_EAB_ConfigPLMN_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_EAB_ConfigPLMN_r11, EAB_ConfigPLMN_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_maxPLMN_r11_OF_EAB_ConfigPLMN_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_maxPLMN_r11_OF_EAB_ConfigPLMN_r11, SEQUENCE_SIZE_1_maxPLMN_r11_OF_EAB_ConfigPLMN_r11_sequence_of,
1, maxPLMN_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_eab_Param_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_eab_Param_r11, T_eab_Param_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType14_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SIB14 ");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType14_r11, SystemInformationBlockType14_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_MBMS_SAI_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 65535U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_MBMS_SAI_List_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBMS_SAI_List_r11, MBMS_SAI_List_r11_sequence_of,
1, maxSAI_MBMS_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_ARFCN_ValueEUTRA_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, maxEARFCN2, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_MBMS_SAI_InterFreq_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBMS_SAI_InterFreq_r11, MBMS_SAI_InterFreq_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_MBMS_SAI_InterFreqList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBMS_SAI_InterFreqList_r11, MBMS_SAI_InterFreqList_r11_sequence_of,
1, maxFreq, FALSE);
return offset;
}
static int
dissect_lte_rrc_FreqBandIndicator_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, maxFBI2, &value, FALSE);
set_freq_band_indicator(value, actx);
return offset;
}
static int
dissect_lte_rrc_MultiBandInfoList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MultiBandInfoList_r11, MultiBandInfoList_r11_sequence_of,
1, maxMultiBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_MBMS_SAI_InterFreq_v1140(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBMS_SAI_InterFreq_v1140, MBMS_SAI_InterFreq_v1140_sequence);
return offset;
}
static int
dissect_lte_rrc_MBMS_SAI_InterFreqList_v1140(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBMS_SAI_InterFreqList_v1140, MBMS_SAI_InterFreqList_v1140_sequence_of,
1, maxFreq, FALSE);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType15_r11_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, SystemInformationBlockType15_r11_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType15_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SIB15 ");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType15_r11, SystemInformationBlockType15_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_timeInfoUTC_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint64 timeInfo;
proto_tree *subtree;
nstime_t ts;
guint32 old_offset = offset;
offset = dissect_per_constrained_integer_64b(tvb, offset, actx, tree, hf_index,
0U, G_GUINT64_CONSTANT(549755813887), &timeInfo, FALSE);
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_timeInfo);
ts.secs = (time_t)(timeInfo/100)-2208988800U;
ts.nsecs = (int)(timeInfo%100)*10000000;
proto_tree_add_time(subtree, hf_lte_rrc_utc_time, tvb, old_offset>>3, (old_offset&0x07) ? 6 : 5, &ts);
proto_tree_add_time(subtree, hf_lte_rrc_local_time, tvb, old_offset>>3, (old_offset&0x07) ? 6 : 5, &ts);
return offset;
}
static int
dissect_lte_rrc_T_dayLightSavingTime_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *daylight_saving_time_tvb = NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, -1,
2, 2, FALSE, &daylight_saving_time_tvb, NULL);
if (daylight_saving_time_tvb) {
actx->created_item = proto_tree_add_uint(tree, hf_index, daylight_saving_time_tvb, 0, 1, tvb_get_bits8(daylight_saving_time_tvb, 0, 2));
}
return offset;
}
static int
dissect_lte_rrc_T_leapSeconds_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-127, 128U, NULL, FALSE);
proto_item_append_text(actx->created_item, "s");
return offset;
}
static int
dissect_lte_rrc_INTEGER_M63_64(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-63, 64U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_timeInfo_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_timeInfo_r11, T_timeInfo_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType16_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SIB16 ");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType16_r11, SystemInformationBlockType16_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_sib_TypeAndInfo_item(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_sib_TypeAndInfo_item, T_sib_TypeAndInfo_item_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_sib_TypeAndInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_sib_TypeAndInfo, T_sib_TypeAndInfo_sequence_of,
1, maxSIB, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_33(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_33, T_nonCriticalExtension_33_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformation_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformation_v8a0_IEs, SystemInformation_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformation_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformation_r8_IEs, SystemInformation_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_30(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_30, T_criticalExtensionsFuture_30_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_30(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_30, T_criticalExtensions_30_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_SystemInformation(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SystemInformation [ ");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformation, SystemInformation_sequence);
col_append_str(actx->pinfo->cinfo, COL_INFO, "]");
return offset;
}
static int
dissect_lte_rrc_MCC_MNC_Digit(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 9U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_MCC(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MCC, MCC_sequence_of,
3, 3, FALSE);
return offset;
}
static int
dissect_lte_rrc_MNC(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MNC, MNC_sequence_of,
2, 3, FALSE);
return offset;
}
static int
dissect_lte_rrc_PLMN_Identity(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PLMN_Identity, PLMN_Identity_sequence);
return offset;
}
static int
dissect_lte_rrc_T_cellReservedForOperatorUse(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PLMN_IdentityInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PLMN_IdentityInfo, PLMN_IdentityInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_PLMN_IdentityList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PLMN_IdentityList, PLMN_IdentityList_sequence_of,
1, maxPLMN_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_TrackingAreaCode(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
16, 16, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_CellIdentity(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
28, 28, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_T_cellBarred(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_intraFreqReselection(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_CSG_Identity(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
27, 27, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_T_cellAccessRelatedInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_cellAccessRelatedInfo, T_cellAccessRelatedInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_T_cellSelectionInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_cellSelectionInfo, T_cellSelectionInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_FreqBandIndicator(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, maxFBI, &value, FALSE);
set_freq_band_indicator(value, actx);
return offset;
}
static int
dissect_lte_rrc_T_si_Periodicity(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
7, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_SIB_Type(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, TRUE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_SIB_MappingInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SIB_MappingInfo, SIB_MappingInfo_sequence_of,
0, maxSIB_1, FALSE);
return offset;
}
static int
dissect_lte_rrc_SchedulingInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SchedulingInfo, SchedulingInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_SchedulingInfoList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SchedulingInfoList, SchedulingInfoList_sequence_of,
1, maxSI_Message, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_subframeAssignment(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
7, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_specialSubframePatterns(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
9, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_TDD_Config(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_TDD_Config, TDD_Config_sequence);
return offset;
}
static int
dissect_lte_rrc_T_si_WindowLength(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
7, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_systemInfoValueTag(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 31U, &value, FALSE);
if (!actx->pinfo->fd->flags.visited) {
if (system_info_value_current_set && (value != system_info_value_current)) {
g_hash_table_insert(lte_rrc_system_info_value_changed_hash, GUINT_TO_POINTER(actx->pinfo->fd->num),
GUINT_TO_POINTER(system_info_value_current+1));
}
system_info_value_current_set = TRUE;
system_info_value_current = value;
}
else {
gpointer p_previous = g_hash_table_lookup(lte_rrc_system_info_value_changed_hash, GUINT_TO_POINTER(actx->pinfo->fd->num));
if (p_previous != NULL) {
guint32 previous = GPOINTER_TO_UINT(p_previous) - 1;
expert_add_info_format(actx->pinfo, actx->created_item, &ei_lte_rrc_si_info_value_changed,
"SI Info Value changed (now %u, was %u)", value, previous);
}
}
return offset;
}
static int
dissect_lte_rrc_T_lateNonCriticalExtension_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string_containing_pdu_new(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, dissect_SystemInformationBlockType1_v8h0_IEs_PDU);
return offset;
}
static int
dissect_lte_rrc_T_ims_EmergencySupport_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_q_QualMinOffset_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 8U, NULL, FALSE);
proto_item_append_text(actx->created_item, "dB");
return offset;
}
static int
dissect_lte_rrc_CellSelectionInfo_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellSelectionInfo_v920, CellSelectionInfo_v920_sequence);
return offset;
}
static int
dissect_lte_rrc_T_specialSubframePatterns_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_TDD_Config_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_TDD_Config_v1130, TDD_Config_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_CellSelectionInfo_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellSelectionInfo_v1130, CellSelectionInfo_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_35(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_35, T_nonCriticalExtension_35_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType1_v1130_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType1_v1130_IEs, SystemInformationBlockType1_v1130_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType1_v920_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType1_v920_IEs, SystemInformationBlockType1_v920_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType1_v890_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType1_v890_IEs, SystemInformationBlockType1_v890_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SystemInformationBlockType1");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType1, SystemInformationBlockType1_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1, T_c1_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_messageClassExtension(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_messageClassExtension, T_messageClassExtension_sequence);
return offset;
}
static int
dissect_lte_rrc_BCCH_DL_SCH_MessageType(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BCCH_DL_SCH_MessageType, BCCH_DL_SCH_MessageType_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_BCCH_DL_SCH_Message(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BCCH_DL_SCH_Message, BCCH_DL_SCH_Message_sequence);
return offset;
}
static int
dissect_lte_rrc_CommonSF_AllocPatternList_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CommonSF_AllocPatternList_r9, CommonSF_AllocPatternList_r9_sequence_of,
1, maxMBSFN_Allocations, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_commonSF_AllocPeriod_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
7, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_1535(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 1535U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_28(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 28U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_mch_SchedulingPeriod_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PMCH_Config_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PMCH_Config_r9, PMCH_Config_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_plmn_Id_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_plmn_Id_r9, T_plmn_Id_r9_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_OCTET_STRING_SIZE_3(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
3, 3, FALSE, NULL);
return offset;
}
static int
dissect_lte_rrc_TMGI_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_TMGI_r9, TMGI_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_OCTET_STRING_SIZE_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
1, 1, FALSE, NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_maxSessionPerPMCH_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, maxSessionPerPMCH_1, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_MBMS_SessionInfo_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBMS_SessionInfo_r9, MBMS_SessionInfo_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_MBMS_SessionInfoList_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBMS_SessionInfoList_r9, MBMS_SessionInfoList_r9_sequence_of,
0, maxSessionPerPMCH, FALSE);
return offset;
}
static int
dissect_lte_rrc_PMCH_Info_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PMCH_Info_r9, PMCH_Info_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_PMCH_InfoList_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PMCH_InfoList_r9, PMCH_InfoList_r9_sequence_of,
0, maxPMCH_PerMBSFN, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_12(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_12, T_nonCriticalExtension_12_sequence);
return offset;
}
static int
dissect_lte_rrc_MBSFNAreaConfiguration_v930_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBSFNAreaConfiguration_v930_IEs, MBSFNAreaConfiguration_v930_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_MBSFNAreaConfiguration_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "MBSFNAreaConfiguration-r9");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBSFNAreaConfiguration_r9, MBSFNAreaConfiguration_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_01, T_c1_01_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_CountingRequestInfo_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CountingRequestInfo_r10, CountingRequestInfo_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_CountingRequestList_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CountingRequestList_r10, CountingRequestList_r10_sequence_of,
1, maxServiceCount, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_09(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_09, T_nonCriticalExtension_09_sequence);
return offset;
}
static int
dissect_lte_rrc_MBMSCountingRequest_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "MBMSCountingRequest-r10");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBMSCountingRequest_r10, MBMSCountingRequest_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c2, T_c2_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_messageClassExtension_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_messageClassExtension_01, T_messageClassExtension_01_sequence);
return offset;
}
static int
dissect_lte_rrc_T_later(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_later, T_later_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_MCCH_MessageType(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MCCH_MessageType, MCCH_MessageType_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_MCCH_Message(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MCCH_Message, MCCH_Message_sequence);
return offset;
}
static int
dissect_lte_rrc_MMEC(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
8, 8, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_32(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
32, 32, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_S_TMSI(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_S_TMSI, S_TMSI_sequence);
return offset;
}
static int
dissect_lte_rrc_IMSI_Digit(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 9U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_IMSI(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IMSI, IMSI_sequence_of,
6, 21, FALSE);
return offset;
}
static int
dissect_lte_rrc_PagingUE_Identity(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PagingUE_Identity, PagingUE_Identity_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_cn_Domain(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PagingRecord(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PagingRecord, PagingRecord_sequence);
return offset;
}
static int
dissect_lte_rrc_PagingRecordList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PagingRecordList, PagingRecordList_sequence_of,
1, maxPageRec, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_pagingRecordList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint16 number_of_records = ((tvb_get_ntohs(tvb, 0) >> 7) & 0x0f) + 1;
offset = dissect_lte_rrc_PagingRecordList(tvb, offset, actx, tree, hf_index);
col_append_fstr(actx->pinfo->cinfo, COL_INFO, " (%u PagingRecords)", number_of_records);
return offset;
}
static int
dissect_lte_rrc_T_systemInfoModification(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
col_append_str(actx->pinfo->cinfo, COL_INFO, " (systemInfoModification)");
expert_add_info(actx->pinfo, actx->created_item, &ei_lte_rrc_sibs_changing);
return offset;
}
static int
dissect_lte_rrc_T_etws_Indication(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
col_append_str(actx->pinfo->cinfo, COL_INFO, " (ETWS)");
expert_add_info(actx->pinfo, actx->created_item, &ei_lte_rrc_earthquake_warning_sys);
return offset;
}
static int
dissect_lte_rrc_T_cmas_Indication_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
col_append_str(actx->pinfo->cinfo, COL_INFO, " (CMAS)");
expert_add_info(actx->pinfo, actx->created_item, &ei_lte_rrc_commercial_mobile_alert_sys);
return offset;
}
static int
dissect_lte_rrc_T_eab_ParamModification_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_16(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_16, T_nonCriticalExtension_16_sequence);
return offset;
}
static int
dissect_lte_rrc_Paging_v1130_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_Paging_v1130_IEs, Paging_v1130_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_Paging_v920_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_Paging_v920_IEs, Paging_v920_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_Paging_v890_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_Paging_v890_IEs, Paging_v890_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_Paging(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "Paging");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_Paging, Paging_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_02(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_02, T_c1_02_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_messageClassExtension_02(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_messageClassExtension_02, T_messageClassExtension_02_sequence);
return offset;
}
static int
dissect_lte_rrc_PCCH_MessageType(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PCCH_MessageType, PCCH_MessageType_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_PCCH_Message(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PCCH_Message, PCCH_Message_sequence);
return offset;
}
static int
dissect_lte_rrc_RRC_TransactionIdentifier(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 3U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_INTEGER_1_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 2U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_PollRetransmit(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
64, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PollPDU(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PollByte(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_maxRetxThreshold(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_UL_AM_RLC(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UL_AM_RLC, UL_AM_RLC_sequence);
return offset;
}
static int
dissect_lte_rrc_T_Reordering(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
32, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_StatusProhibit(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
64, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_DL_AM_RLC(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DL_AM_RLC, DL_AM_RLC_sequence);
return offset;
}
static int
dissect_lte_rrc_T_am(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_am, T_am_sequence);
return offset;
}
static int
dissect_lte_rrc_SN_FieldLength(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drb_mapping_t *mapping = private_data_get_drb_mapping(actx);
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, &value, FALSE, 0, NULL);
if (mapping != NULL) {
mapping->um_sn_length = (value==0) ? 5 : 10;
mapping->um_sn_length_present = TRUE;
}
return offset;
}
static int
dissect_lte_rrc_UL_UM_RLC(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UL_UM_RLC, UL_UM_RLC_sequence);
return offset;
}
static int
dissect_lte_rrc_DL_UM_RLC(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DL_UM_RLC, DL_UM_RLC_sequence);
return offset;
}
static int
dissect_lte_rrc_T_um_Bi_Directional(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_um_Bi_Directional, T_um_Bi_Directional_sequence);
return offset;
}
static int
dissect_lte_rrc_T_um_Uni_Directional_UL(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_um_Uni_Directional_UL, T_um_Uni_Directional_UL_sequence);
return offset;
}
static int
dissect_lte_rrc_T_um_Uni_Directional_DL(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_um_Uni_Directional_DL, T_um_Uni_Directional_DL_sequence);
return offset;
}
static int
dissect_lte_rrc_RLC_Config(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drb_mapping_t *mapping = private_data_get_drb_mapping(actx);
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RLC_Config, RLC_Config_choice,
&value);
if (mapping != NULL) {
mapping->rlcMode = (value==0) ? RLC_AM_MODE : RLC_UM_MODE;
mapping->rlcMode_present = TRUE;
if (mapping->rlcMode == RLC_AM_MODE) {
mapping->pdcp_sn_size = 12;
mapping->pdcp_sn_size_present = TRUE;
}
}
return offset;
}
static int
dissect_lte_rrc_T_rlc_Config(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_rlc_Config, T_rlc_Config_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_priority(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drb_mapping_t *mapping = private_data_get_drb_mapping(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 16U, &value, FALSE);
if (mapping != NULL) {
mapping->ul_priority = value;
mapping->ul_priority_present = TRUE;
}
return offset;
}
static int
dissect_lte_rrc_T_prioritisedBitRate(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_bucketSizeDuration(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_ul_SpecificParameters(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_ul_SpecificParameters, T_ul_SpecificParameters_sequence);
return offset;
}
static int
dissect_lte_rrc_T_logicalChannelSR_Mask_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_LogicalChannelConfig_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, LogicalChannelConfig_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_LogicalChannelConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_LogicalChannelConfig, LogicalChannelConfig_sequence);
return offset;
}
static int
dissect_lte_rrc_T_logicalChannelConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_logicalChannelConfig, T_logicalChannelConfig_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_SRB_ToAddMod(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SRB_ToAddMod, SRB_ToAddMod_sequence);
return offset;
}
static int
dissect_lte_rrc_SRB_ToAddModList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SRB_ToAddModList, SRB_ToAddModList_sequence_of,
1, 2, FALSE);
return offset;
}
static int
dissect_lte_rrc_DRB_Identity(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drb_mapping_t *mapping = private_data_get_drb_mapping(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 32U, &value, FALSE);
if (mapping != NULL) {
mapping->drbid = (guint8)value;
}
return offset;
}
static int
dissect_lte_rrc_T_discardTimer(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_rlc_AM(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_rlc_AM, T_rlc_AM_sequence);
return offset;
}
static int
dissect_lte_rrc_T_pdcp_SN_Size(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drb_mapping_t *mapping = private_data_get_drb_mapping(actx);
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, &value, FALSE, 0, NULL);
if (mapping != NULL) {
mapping->pdcp_sn_size = (value==0) ? 7 : 12;
mapping->pdcp_sn_size_present = TRUE;
}
return offset;
}
static int
dissect_lte_rrc_T_rlc_UM(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_rlc_UM, T_rlc_UM_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_1_16383(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 16383U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_profiles(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_profiles, T_profiles_sequence);
return offset;
}
static int
dissect_lte_rrc_T_rohc(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_rohc, T_rohc_sequence);
return offset;
}
static int
dissect_lte_rrc_T_headerCompression(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_headerCompression, T_headerCompression_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_rn_IntegrityProtection_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PDCP_Config_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, PDCP_Config_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_T_pdcp_SN_Size_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
drb_mapping_t *mapping = private_data_get_drb_mapping(actx);
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
if (mapping != NULL) {
mapping->pdcp_sn_size = 15;
mapping->pdcp_sn_size_present = TRUE;
}
return offset;
}
static int
dissect_lte_rrc_PDCP_Config_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, PDCP_Config_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_PDCP_Config(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PDCP_Config, PDCP_Config_sequence);
return offset;
}
static int
dissect_lte_rrc_T_logicalChannelIdentity(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drb_mapping_t *mapping = private_data_get_drb_mapping(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
3U, 10U, &value, FALSE);
if (mapping != NULL) {
mapping->lcid = (guint8)value;
mapping->lcid_present = TRUE;
}
return offset;
}
static int
dissect_lte_rrc_DRB_ToAddMod(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
struct mac_lte_info *p_mac_lte_info;
drb_mapping_t *drb_mapping = private_data_get_drb_mapping(actx);
memset(drb_mapping, 0, sizeof(*drb_mapping));
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DRB_ToAddMod, DRB_ToAddMod_sequence);
p_mac_lte_info = (mac_lte_info *)p_get_proto_data(wmem_file_scope(), actx->pinfo, proto_mac_lte, 0);
if (p_mac_lte_info == NULL) {
return offset;
}
else {
drb_mapping->ueid = p_mac_lte_info->ueid;
}
set_mac_lte_channel_mapping(drb_mapping);
if (drb_mapping->pdcp_sn_size_present) {
set_rlc_lte_drb_pdcp_seqnum_length(drb_mapping->ueid,
drb_mapping->drbid,
drb_mapping->pdcp_sn_size);
}
memset(&drb_mapping, 0, sizeof(drb_mapping));
return offset;
}
static int
dissect_lte_rrc_DRB_ToAddModList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DRB_ToAddModList, DRB_ToAddModList_sequence_of,
1, maxDRB, FALSE);
return offset;
}
static int
dissect_lte_rrc_DRB_ToReleaseList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DRB_ToReleaseList, DRB_ToReleaseList_sequence_of,
1, maxDRB, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_maxHARQ_Tx(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_periodicBSR_Timer(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_retxBSR_Timer(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_ul_SCH_Config(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_ul_SCH_Config, T_ul_SCH_Config_sequence);
return offset;
}
static int
dissect_lte_rrc_T_release(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
mac_lte_info* p_mac_lte_info;
offset = dissect_per_null(tvb, offset, actx, tree, hf_index);
p_mac_lte_info = (mac_lte_info *)p_get_proto_data(wmem_file_scope(), actx->pinfo, proto_mac_lte, 0);
if (p_mac_lte_info != NULL) {
set_mac_lte_drx_config_release(p_mac_lte_info->ueid, actx->pinfo);
}
return offset;
}
static int
dissect_lte_rrc_T_onDurationTimer(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, &value, FALSE, 0, NULL);
config->configured = TRUE;
config->onDurationTimer = drx_lookup_onDurationTimer(value);
return offset;
}
static int
dissect_lte_rrc_T_drx_InactivityTimer(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
32, &value, FALSE, 0, NULL);
config->inactivityTimer = drx_lookup_inactivityTimer(value);
return offset;
}
static int
dissect_lte_rrc_T_drx_RetransmissionTimer(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, &value, FALSE, 0, NULL);
config->retransmissionTimer = drx_lookup_retransmissionTimer(value);
return offset;
}
static int
dissect_lte_rrc_T_sf10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 9U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_sf20(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 19U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_sf32(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 31U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_sf40(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 39U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_sf64(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 63U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_sf80(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 79U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_sf128(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 127U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_sf160(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 159U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_sf256(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 255U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_sf320(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 319U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_sf512(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 511U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_sf640(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 639U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_sf1024(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 1023U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_sf1280(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 1279U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_sf2048(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 2047U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_sf2560(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 2559U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_longDRX_CycleStartOffset(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_longDRX_CycleStartOffset, T_longDRX_CycleStartOffset_choice,
&value);
config->longCycle = drx_lookup_longCycle(value);
return offset;
}
static int
dissect_lte_rrc_T_shortDRX_Cycle(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, &value, FALSE, 0, NULL);
config->shortCycleConfigured = TRUE;
config->shortCycle = drx_lookup_shortCycle(value);
return offset;
}
static int
dissect_lte_rrc_T_drxShortCycleTimer(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 timer;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 16U, &timer, FALSE);
config->shortCycleTimer = timer;
proto_item_append_text(actx->created_item, " shortDRX-Cycle%s", plurality(timer, "", "s"));
return offset;
}
static int
dissect_lte_rrc_T_shortDRX(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_shortDRX, T_shortDRX_sequence);
return offset;
}
static int
dissect_lte_rrc_T_setup_15(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_15, T_setup_15_sequence);
return offset;
}
static int
dissect_lte_rrc_DRX_Config(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DRX_Config, DRX_Config_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_periodicPHR_Timer(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_prohibitPHR_Timer(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_dl_PathlossChange(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_14(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_14, T_setup_14_sequence);
return offset;
}
static int
dissect_lte_rrc_T_phr_Config(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_phr_Config, T_phr_Config_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_sr_ProhibitTimer_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 timer;
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 7U, &timer, FALSE);
proto_item_append_text(actx->created_item, " SR period%s", (timer > 1) ? "s" : "");
return offset;
}
static int
dissect_lte_rrc_MAC_MainConfig_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, MAC_MainConfig_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_T_sCellDeactivationTimer_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_extendedBSR_Sizes_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
mac_lte_info* p_mac_lte_info;
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
p_mac_lte_info = (mac_lte_info *)p_get_proto_data(wmem_file_scope(), actx->pinfo, proto_mac_lte, 0);
if (p_mac_lte_info != NULL) {
set_mac_lte_extended_bsr_sizes(p_mac_lte_info->ueid, TRUE);
}
return offset;
}
static int
dissect_lte_rrc_T_extendedPHR_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_mac_MainConfig_v1020(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_mac_MainConfig_v1020, T_mac_MainConfig_v1020_sequence);
return offset;
}
static int
dissect_lte_rrc_MAC_MainConfig_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, MAC_MainConfig_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_STAG_Id_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, maxSTAG_r11, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_STAG_ToReleaseList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_STAG_ToReleaseList_r11, STAG_ToReleaseList_r11_sequence_of,
1, maxSTAG_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_STAG_ToAddMod_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_STAG_ToAddMod_r11, STAG_ToAddMod_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_STAG_ToAddModList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_STAG_ToAddModList_r11, STAG_ToAddModList_r11_sequence_of,
1, maxSTAG_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_drx_RetransmissionTimer_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, &value, FALSE, 0, NULL);
config->retransmissionTimer = 0;
return offset;
}
static int
dissect_lte_rrc_T_sf60_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 59U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_sf70_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 69U, &value, FALSE);
config->cycleOffset = value;
return offset;
}
static int
dissect_lte_rrc_T_longDRX_CycleStartOffset_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_longDRX_CycleStartOffset_v1130, T_longDRX_CycleStartOffset_v1130_choice,
&value);
config->longCycle = drx_lookup_longCycle_v1130(value);
return offset;
}
static int
dissect_lte_rrc_T_shortDRX_Cycle_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
drx_config_t* config = private_data_get_drx_config(actx);
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, &value, FALSE, 0, NULL);
config->shortCycle = 4;
return offset;
}
static int
dissect_lte_rrc_DRX_Config_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DRX_Config_v1130, DRX_Config_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_MAC_MainConfig_eag_3(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, MAC_MainConfig_eag_3_sequence);
return offset;
}
static int
dissect_lte_rrc_MAC_MainConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
drx_config_t *drx_config = private_data_get_drx_config(actx);
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MAC_MainConfig, MAC_MainConfig_sequence);
if (drx_config->configured) {
mac_lte_info* p_mac_lte_info;
drx_check_config_sane(drx_config, actx);
p_mac_lte_info = (mac_lte_info *)p_get_proto_data(wmem_file_scope(), actx->pinfo, proto_mac_lte, 0);
if (p_mac_lte_info != NULL) {
set_mac_lte_drx_config(p_mac_lte_info->ueid, drx_config, actx->pinfo);
}
drx_config->configured = FALSE;
}
return offset;
}
static int
dissect_lte_rrc_T_mac_MainConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_mac_MainConfig, T_mac_MainConfig_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_C_RNTI(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
16, 16, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_T_semiPersistSchedIntervalDL(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_N1PUCCH_AN_PersistentList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_N1PUCCH_AN_PersistentList, N1PUCCH_AN_PersistentList_sequence_of,
1, 4, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_setup_31(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_31, T_setup_31_sequence);
return offset;
}
static int
dissect_lte_rrc_T_twoAntennaPortActivated_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_twoAntennaPortActivated_r10, T_twoAntennaPortActivated_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, T_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_T_setup_30(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_30, T_setup_30_sequence);
return offset;
}
static int
dissect_lte_rrc_SPS_ConfigDL(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SPS_ConfigDL, SPS_ConfigDL_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_semiPersistSchedIntervalUL(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_implicitReleaseAfter(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_p0_NominalPUSCH_Persistent(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-126, 24U, NULL, FALSE);
proto_item_append_text(actx->created_item, "dBm");
return offset;
}
static int
dissect_lte_rrc_T_p0_UE_PUSCH_Persistent(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-8, 7U, NULL, FALSE);
proto_item_append_text(actx->created_item, "dB");
return offset;
}
static int
dissect_lte_rrc_T_p0_Persistent(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_p0_Persistent, T_p0_Persistent_sequence);
return offset;
}
static int
dissect_lte_rrc_T_twoIntervalsConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_32(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_32, T_setup_32_sequence);
return offset;
}
static int
dissect_lte_rrc_SPS_ConfigUL(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SPS_ConfigUL, SPS_ConfigUL_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_SPS_Config(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SPS_Config, SPS_Config_sequence);
return offset;
}
static int
dissect_lte_rrc_T_p_a(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PDSCH_ConfigDedicated(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PDSCH_ConfigDedicated, PDSCH_ConfigDedicated_sequence);
return offset;
}
static int
dissect_lte_rrc_T_repetitionFactor(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_18(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_18, T_setup_18_sequence);
return offset;
}
static int
dissect_lte_rrc_T_ackNackRepetition(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_ackNackRepetition, T_ackNackRepetition_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_tdd_AckNackFeedbackMode(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PUCCH_ConfigDedicated(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PUCCH_ConfigDedicated, PUCCH_ConfigDedicated_sequence);
return offset;
}
static int
dissect_lte_rrc_PUSCH_ConfigDedicated(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PUSCH_ConfigDedicated, PUSCH_ConfigDedicated_sequence);
return offset;
}
static int
dissect_lte_rrc_T_p0_UE_PUSCH(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-8, 7U, NULL, FALSE);
proto_item_append_text(actx->created_item, "dB");
return offset;
}
static int
dissect_lte_rrc_T_deltaMCS_Enabled(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_p0_UE_PUCCH(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-8, 7U, NULL, FALSE);
proto_item_append_text(actx->created_item, "dB");
return offset;
}
static int
dissect_lte_rrc_FilterCoefficient(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, TRUE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_UplinkPowerControlDedicated(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UplinkPowerControlDedicated, UplinkPowerControlDedicated_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_1_15(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 15U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_INTEGER_1_31(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 31U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_TPC_Index(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_TPC_Index, TPC_Index_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_33(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_33, T_setup_33_sequence);
return offset;
}
static int
dissect_lte_rrc_TPC_PDCCH_Config(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_TPC_PDCCH_Config, TPC_PDCCH_Config_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportModeAperiodic(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_1185(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 1185U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_1023(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 1023U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_subbandCQI(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_subbandCQI, T_subbandCQI_sequence);
return offset;
}
static int
dissect_lte_rrc_T_cqi_FormatIndicatorPeriodic(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_cqi_FormatIndicatorPeriodic, T_cqi_FormatIndicatorPeriodic_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_03(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_03, T_setup_03_sequence);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportPeriodic(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CQI_ReportPeriodic, CQI_ReportPeriodic_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CQI_ReportConfig, CQI_ReportConfig_sequence);
return offset;
}
static int
dissect_lte_rrc_T_srs_Bandwidth(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_srs_HoppingBandwidth(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_23(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 23U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_duration(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
actx->created_item = proto_tree_add_bits_item(tree, hf_index, tvb, offset, 1, ENC_BIG_ENDIAN);
offset++;
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 1U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_cyclicShift(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_27(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_27, T_setup_27_sequence);
return offset;
}
static int
dissect_lte_rrc_SoundingRS_UL_ConfigDedicated(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SoundingRS_UL_ConfigDedicated, SoundingRS_UL_ConfigDedicated_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_transmissionMode(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
2, 2, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_4(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
4, 4, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_64(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
64, 64, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_T_codebookSubsetRestriction(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_codebookSubsetRestriction, T_codebookSubsetRestriction_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_ue_TransmitAntennaSelection(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_ue_TransmitAntennaSelection, T_ue_TransmitAntennaSelection_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_AntennaInfoDedicated(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AntennaInfoDedicated, AntennaInfoDedicated_sequence);
return offset;
}
static int
dissect_lte_rrc_T_antennaInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_antennaInfo, T_antennaInfo_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_sr_ConfigIndex(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
guint16 periodicity, subframe_offset;
proto_item *ti;
proto_tree *subtree;
gint index_offset = offset;
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 157U, &value, FALSE);
sr_lookup_configindex(value, &periodicity, &subframe_offset);
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_sr_ConfigIndex);
ti = proto_tree_add_uint(subtree, hf_lte_rrc_sr_config_periodicity, tvb, index_offset>>3, 1, periodicity);
PROTO_ITEM_SET_GENERATED(ti);
ti = proto_tree_add_uint(subtree, hf_lte_rrc_sr_config_subframe_offset, tvb, index_offset>>3, 1, subframe_offset);
PROTO_ITEM_SET_GENERATED(ti);
return offset;
}
static int
dissect_lte_rrc_T_dsr_TransMax(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_25(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_25, T_setup_25_sequence);
return offset;
}
static int
dissect_lte_rrc_SchedulingRequestConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SchedulingRequestConfig, SchedulingRequestConfig_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_cqi_Mask_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_pmi_RI_Report_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportConfig_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CQI_ReportConfig_v920, CQI_ReportConfig_v920_sequence);
return offset;
}
static int
dissect_lte_rrc_T_codebookSubsetRestriction_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_codebookSubsetRestriction_v920, T_codebookSubsetRestriction_v920_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_AntennaInfoDedicated_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AntennaInfoDedicated_v920, AntennaInfoDedicated_v920_sequence);
return offset;
}
static int
dissect_lte_rrc_PhysicalConfigDedicated_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, PhysicalConfigDedicated_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_T_transmissionMode_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_ue_TransmitAntennaSelection_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_ue_TransmitAntennaSelection_01, T_ue_TransmitAntennaSelection_01_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_AntennaInfoDedicated_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AntennaInfoDedicated_r10, AntennaInfoDedicated_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_antennaInfo_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_antennaInfo_r10, T_antennaInfo_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_transmissionModeUL_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_fourAntennaPortActivated_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_AntennaInfoUL_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AntennaInfoUL_r10, AntennaInfoUL_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_aperiodicCSI_Trigger_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_aperiodicCSI_Trigger_r10, T_aperiodicCSI_Trigger_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_setup_07(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_07, T_setup_07_sequence);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportAperiodic_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CQI_ReportAperiodic_r10, CQI_ReportAperiodic_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_1184(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 1184U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_csi_ReportMode_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_widebandCQI_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_widebandCQI_r10, T_widebandCQI_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_periodicityFactor_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_subbandCQI_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_subbandCQI_r10, T_subbandCQI_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_cqi_FormatIndicatorPeriodic_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_cqi_FormatIndicatorPeriodic_r10, T_cqi_FormatIndicatorPeriodic_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_cqi_Mask_r9_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_05(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_05, T_setup_05_sequence);
return offset;
}
static int
dissect_lte_rrc_T_csi_ConfigIndex_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_csi_ConfigIndex_r10, T_csi_ConfigIndex_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_04(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_04, T_setup_04_sequence);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportPeriodic_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CQI_ReportPeriodic_r10, CQI_ReportPeriodic_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_pmi_RI_Report_r9_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_40(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
40, 40, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_20(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
20, 20, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_70(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
70, 70, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_60(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
60, 60, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_T_subframePatternTDD_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_subframePatternTDD_r10, T_subframePatternTDD_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_MeasSubframePattern_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasSubframePattern_r10, MeasSubframePattern_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_02(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_02, T_setup_02_sequence);
return offset;
}
static int
dissect_lte_rrc_T_csi_SubframePatternConfig_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_csi_SubframePatternConfig_r10, T_csi_SubframePatternConfig_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportConfig_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CQI_ReportConfig_r10, CQI_ReportConfig_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_antennaPortsCount_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_154(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 154U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_INTEGER_M8_15(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-8, 15U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_setup_08(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_08, T_setup_08_sequence);
return offset;
}
static int
dissect_lte_rrc_T_csi_RS_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_csi_RS_r10, T_csi_RS_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_09(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_09, T_setup_09_sequence);
return offset;
}
static int
dissect_lte_rrc_T_zeroTxPowerCSI_RS_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_zeroTxPowerCSI_RS_r10, T_zeroTxPowerCSI_RS_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_CSI_RS_Config_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSI_RS_Config_r10, CSI_RS_Config_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_549(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 549U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_n3PUCCH_AN_List_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_n3PUCCH_AN_List_r10, T_n3PUCCH_AN_List_r10_sequence_of,
1, 4, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_n3PUCCH_AN_ListP1_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_n3PUCCH_AN_ListP1_r10, T_n3PUCCH_AN_ListP1_r10_sequence_of,
1, 4, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_setup_19(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_19, T_setup_19_sequence);
return offset;
}
static int
dissect_lte_rrc_T_twoAntennaPortActivatedPUCCH_Format3_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_twoAntennaPortActivatedPUCCH_Format3_r10, T_twoAntennaPortActivatedPUCCH_Format3_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_format3_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_format3_r10, T_format3_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_N1PUCCH_AN_CS_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_N1PUCCH_AN_CS_r10, N1PUCCH_AN_CS_r10_sequence_of,
1, 4, FALSE);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_2_OF_N1PUCCH_AN_CS_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_2_OF_N1PUCCH_AN_CS_r10, SEQUENCE_SIZE_1_2_OF_N1PUCCH_AN_CS_r10_sequence_of,
1, 2, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_setup_20(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_20, T_setup_20_sequence);
return offset;
}
static int
dissect_lte_rrc_T_n1PUCCH_AN_CS_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_n1PUCCH_AN_CS_r10, T_n1PUCCH_AN_CS_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_channelSelection_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_channelSelection_r10, T_channelSelection_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_pucch_Format_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_pucch_Format_r10, T_pucch_Format_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_twoAntennaPortActivatedPUCCH_Format1a1b_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_simultaneousPUCCH_PUSCH_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PUCCH_ConfigDedicated_v1020(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PUCCH_ConfigDedicated_v1020, PUCCH_ConfigDedicated_v1020_sequence);
return offset;
}
static int
dissect_lte_rrc_T_betaOffsetMC_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_betaOffsetMC_r10, T_betaOffsetMC_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_groupHoppingDisabled_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_dmrs_WithOCC_Activated_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PUSCH_ConfigDedicated_v1020(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PUSCH_ConfigDedicated_v1020, PUSCH_ConfigDedicated_v1020_sequence);
return offset;
}
static int
dissect_lte_rrc_SchedulingRequestConfig_v1020(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SchedulingRequestConfig_v1020, SchedulingRequestConfig_v1020_sequence);
return offset;
}
static int
dissect_lte_rrc_SRS_AntennaPort(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_SoundingRS_UL_ConfigDedicated_v1020(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SoundingRS_UL_ConfigDedicated_v1020, SoundingRS_UL_ConfigDedicated_v1020_sequence);
return offset;
}
static int
dissect_lte_rrc_T_srs_BandwidthAp_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_cyclicShiftAp_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_SRS_ConfigAp_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SRS_ConfigAp_r10, SRS_ConfigAp_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_3_OF_SRS_ConfigAp_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_3_OF_SRS_ConfigAp_r10, SEQUENCE_SIZE_1_3_OF_SRS_ConfigAp_r10_sequence_of,
1, 3, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_setup_29(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_29, T_setup_29_sequence);
return offset;
}
static int
dissect_lte_rrc_T_srs_ActivateAp_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_srs_ActivateAp_r10, T_srs_ActivateAp_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_28(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_28, T_setup_28_sequence);
return offset;
}
static int
dissect_lte_rrc_SoundingRS_UL_ConfigDedicatedAperiodic_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SoundingRS_UL_ConfigDedicatedAperiodic_r10, SoundingRS_UL_ConfigDedicatedAperiodic_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_deltaTxD_OffsetPUCCH_Format1_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_deltaTxD_OffsetPUCCH_Format1a1b_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_deltaTxD_OffsetPUCCH_Format22a2b_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_deltaTxD_OffsetPUCCH_Format3_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_DeltaTxD_OffsetListPUCCH_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DeltaTxD_OffsetListPUCCH_r10, DeltaTxD_OffsetListPUCCH_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_UplinkPowerControlDedicated_v1020(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UplinkPowerControlDedicated_v1020, UplinkPowerControlDedicated_v1020_sequence);
return offset;
}
static int
dissect_lte_rrc_PhysicalConfigDedicated_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, PhysicalConfigDedicated_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_T_setup_17(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_17, T_setup_17_sequence);
return offset;
}
static int
dissect_lte_rrc_T_additionalSpectrumEmissionCA_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_additionalSpectrumEmissionCA_r10, T_additionalSpectrumEmissionCA_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_PhysicalConfigDedicated_eag_3(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, PhysicalConfigDedicated_eag_3_sequence);
return offset;
}
static int
dissect_lte_rrc_CSI_RS_ConfigNZPId_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, maxCSI_RS_NZP_r11, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_CSI_RS_ConfigNZPToReleaseList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSI_RS_ConfigNZPToReleaseList_r11, CSI_RS_ConfigNZPToReleaseList_r11_sequence_of,
1, maxCSI_RS_NZP_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_antennaPortsCount_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_503(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 503U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_crs_PortsCount_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_10, T_setup_10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_mbsfn_SubframeConfigList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_mbsfn_SubframeConfigList_r11, T_mbsfn_SubframeConfigList_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_qcl_CRS_Info_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_qcl_CRS_Info_r11, T_qcl_CRS_Info_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_CSI_RS_ConfigNZP_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSI_RS_ConfigNZP_r11, CSI_RS_ConfigNZP_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_CSI_RS_ConfigNZPToAddModList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSI_RS_ConfigNZPToAddModList_r11, CSI_RS_ConfigNZPToAddModList_r11_sequence_of,
1, maxCSI_RS_NZP_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_CSI_RS_ConfigZPId_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, maxCSI_RS_ZP_r11, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_CSI_RS_ConfigZPToReleaseList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSI_RS_ConfigZPToReleaseList_r11, CSI_RS_ConfigZPToReleaseList_r11_sequence_of,
1, maxCSI_RS_ZP_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_CSI_RS_ConfigZP_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSI_RS_ConfigZP_r11, CSI_RS_ConfigZP_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_CSI_RS_ConfigZPToAddModList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSI_RS_ConfigZPToAddModList_r11, CSI_RS_ConfigZPToAddModList_r11_sequence_of,
1, maxCSI_RS_ZP_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_setup_13(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_13, T_setup_13_sequence);
return offset;
}
static int
dissect_lte_rrc_T_subframePatternConfig_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_subframePatternConfig_r11, T_subframePatternConfig_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_EPDCCH_SetConfigId_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 1U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_EPDCCH_SetConfigToReleaseList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_EPDCCH_SetConfigToReleaseList_r11, EPDCCH_SetConfigToReleaseList_r11_sequence_of,
1, maxEPDCCH_Set_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_transmissionType_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_numberPRB_Pairs_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
3, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_4_38(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
4, 38, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_T_resourceBlockAssignment_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_resourceBlockAssignment_r11, T_resourceBlockAssignment_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_PDSCH_RE_MappingQCL_ConfigId_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, maxRE_MapQCL_r11, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_EPDCCH_SetConfig_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_EPDCCH_SetConfig_r11, EPDCCH_SetConfig_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_EPDCCH_SetConfigToAddModList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_EPDCCH_SetConfigToAddModList_r11, EPDCCH_SetConfigToAddModList_r11_sequence_of,
1, maxEPDCCH_Set_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_setup_12(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_12, T_setup_12_sequence);
return offset;
}
static int
dissect_lte_rrc_T_config_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_config_r11, T_config_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_EPDCCH_Config_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_EPDCCH_Config_r11, EPDCCH_Config_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_setup_11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_11, T_setup_11_sequence);
return offset;
}
static int
dissect_lte_rrc_DMRS_Config_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DMRS_Config_r11, DMRS_Config_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_qcl_Operation(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_RE_MappingQCLConfigToReleaseList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RE_MappingQCLConfigToReleaseList_r11, RE_MappingQCLConfigToReleaseList_r11_sequence_of,
1, maxRE_MapQCL_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_crs_PortsCount_r11_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_5(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 5U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_setup_16(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_16, T_setup_16_sequence);
return offset;
}
static int
dissect_lte_rrc_T_mbsfn_SubframeConfigList_r11_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_mbsfn_SubframeConfigList_r11_01, T_mbsfn_SubframeConfigList_r11_01_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_pdsch_Start_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
6, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_optionalSetOfFields_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_optionalSetOfFields_r11, T_optionalSetOfFields_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_PDSCH_RE_MappingQCL_Config_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PDSCH_RE_MappingQCL_Config_r11, PDSCH_RE_MappingQCL_Config_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_RE_MappingQCLConfigToAddModList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RE_MappingQCLConfigToAddModList_r11, RE_MappingQCLConfigToAddModList_r11_sequence_of,
1, maxRE_MapQCL_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_PDSCH_ConfigDedicated_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PDSCH_ConfigDedicated_v1130, PDSCH_ConfigDedicated_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_T_simultaneousAckNackAndCQI_Format3_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportPeriodicProcExtId_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, maxCQI_ProcExt_r11, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportPeriodicProcExtToReleaseList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CQI_ReportPeriodicProcExtToReleaseList_r11, CQI_ReportPeriodicProcExtToReleaseList_r11_sequence_of,
1, maxCQI_ProcExt_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_csi_ReportMode_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_widebandCQI_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_widebandCQI_r11, T_widebandCQI_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_periodicityFactor_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_subbandCQI_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_subbandCQI_r11, T_subbandCQI_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_cqi_FormatIndicatorPeriodic_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_cqi_FormatIndicatorPeriodic_r11, T_cqi_FormatIndicatorPeriodic_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_06(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_06, T_setup_06_sequence);
return offset;
}
static int
dissect_lte_rrc_T_csi_ConfigIndex_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_csi_ConfigIndex_r11, T_csi_ConfigIndex_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportPeriodicProcExt_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CQI_ReportPeriodicProcExt_r11, CQI_ReportPeriodicProcExt_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportPeriodicProcExtToAddModList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CQI_ReportPeriodicProcExtToAddModList_r11, CQI_ReportPeriodicProcExtToAddModList_r11_sequence_of,
1, maxCQI_ProcExt_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportPeriodic_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CQI_ReportPeriodic_v1130, CQI_ReportPeriodic_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_CSI_IM_ConfigId_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, maxCSI_IM_r11, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_CSI_IM_ConfigToReleaseList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSI_IM_ConfigToReleaseList_r11, CSI_IM_ConfigToReleaseList_r11_sequence_of,
1, maxCSI_IM_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_CSI_IM_Config_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSI_IM_Config_r11, CSI_IM_Config_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_CSI_IM_ConfigToAddModList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSI_IM_ConfigToAddModList_r11, CSI_IM_ConfigToAddModList_r11_sequence_of,
1, maxCSI_IM_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_CSI_ProcessId_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, maxCSI_Proc_r11, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_CSI_ProcessToReleaseList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSI_ProcessToReleaseList_r11, CSI_ProcessToReleaseList_r11_sequence_of,
1, maxCSI_Proc_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_P_C_AndCBSR_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_P_C_AndCBSR_r11, P_C_AndCBSR_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_2_OF_P_C_AndCBSR_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_2_OF_P_C_AndCBSR_r11, SEQUENCE_SIZE_1_2_OF_P_C_AndCBSR_r11_sequence_of,
1, 2, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_pmi_RI_Report_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportBothProc_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CQI_ReportBothProc_r11, CQI_ReportBothProc_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_maxCQI_ProcExt_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, maxCQI_ProcExt_r11, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportAperiodicProc_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CQI_ReportAperiodicProc_r11, CQI_ReportAperiodicProc_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_CSI_Process_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSI_Process_r11, CSI_Process_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_CSI_ProcessToAddModList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSI_ProcessToAddModList_r11, CSI_ProcessToAddModList_r11_sequence_of,
1, maxCSI_Proc_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportBoth_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CQI_ReportBoth_r11, CQI_ReportBoth_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportConfig_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CQI_ReportConfig_v1130, CQI_ReportConfig_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_T_n1PUCCH_AN_CS_ListP1_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_n1PUCCH_AN_CS_ListP1_r11, T_n1PUCCH_AN_CS_ListP1_r11_sequence_of,
2, 4, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_setup_21(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_21, T_setup_21_sequence);
return offset;
}
static int
dissect_lte_rrc_T_n1PUCCH_AN_CS_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_n1PUCCH_AN_CS_v1130, T_n1PUCCH_AN_CS_v1130_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_22(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_22, T_setup_22_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nPUCCH_Param_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nPUCCH_Param_r11, T_nPUCCH_Param_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_PUCCH_ConfigDedicated_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PUCCH_ConfigDedicated_v1130, PUCCH_ConfigDedicated_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_509(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 509U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_setup_23(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_23, T_setup_23_sequence);
return offset;
}
static int
dissect_lte_rrc_T_pusch_DMRS_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_pusch_DMRS_r11, T_pusch_DMRS_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_PUSCH_ConfigDedicated_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PUSCH_ConfigDedicated_v1130, PUSCH_ConfigDedicated_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_16_31(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
16U, 31U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_deltaTxD_OffsetPUCCH_Format1bCS_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_DeltaTxD_OffsetListPUCCH_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DeltaTxD_OffsetListPUCCH_v1130, DeltaTxD_OffsetListPUCCH_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_UplinkPowerControlDedicated_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UplinkPowerControlDedicated_v1130, UplinkPowerControlDedicated_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_PhysicalConfigDedicated_eag_4(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, PhysicalConfigDedicated_eag_4_sequence);
return offset;
}
static int
dissect_lte_rrc_PhysicalConfigDedicated(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PhysicalConfigDedicated, PhysicalConfigDedicated_sequence);
return offset;
}
static int
dissect_lte_rrc_T_t301_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_t310_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
7, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_n310_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_t311_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
7, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_n311_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_24(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_24, T_setup_24_sequence);
return offset;
}
static int
dissect_lte_rrc_RLF_TimersAndConstants_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RLF_TimersAndConstants_r9, RLF_TimersAndConstants_r9_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_RadioResourceConfigDedicated_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, RadioResourceConfigDedicated_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasSubframePatternPCell_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasSubframePatternPCell_r10, MeasSubframePatternPCell_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_RadioResourceConfigDedicated_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, RadioResourceConfigDedicated_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_T_antennaPortsCount_r11_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_CRS_AssistanceInfo_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CRS_AssistanceInfo_r11, CRS_AssistanceInfo_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_CRS_AssistanceInfoList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CRS_AssistanceInfoList_r11, CRS_AssistanceInfoList_r11_sequence_of,
1, maxCellReport, FALSE);
return offset;
}
static int
dissect_lte_rrc_NeighCellsCRS_Info_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_NeighCellsCRS_Info_r11, NeighCellsCRS_Info_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_RadioResourceConfigDedicated_eag_3(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, RadioResourceConfigDedicated_eag_3_sequence);
return offset;
}
static int
dissect_lte_rrc_RadioResourceConfigDedicated(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RadioResourceConfigDedicated, RadioResourceConfigDedicated_sequence);
return offset;
}
static int
dissect_lte_rrc_NextHopChainingCount(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 7U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_22(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_22, T_nonCriticalExtension_22_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReestablishment_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReestablishment_v8a0_IEs, RRCConnectionReestablishment_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReestablishment_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReestablishment_r8_IEs, RRCConnectionReestablishment_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_21(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_21, T_c1_21_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_18(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_18, T_criticalExtensionsFuture_18_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_18(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_18, T_criticalExtensions_18_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReestablishment(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "RRCConnectionReestablishment");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReestablishment, RRCConnectionReestablishment_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_24(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_24, T_nonCriticalExtension_24_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReestablishmentReject_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReestablishmentReject_v8a0_IEs, RRCConnectionReestablishmentReject_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReestablishmentReject_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReestablishmentReject_r8_IEs, RRCConnectionReestablishmentReject_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_20(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_20, T_criticalExtensionsFuture_20_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_20(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_20, T_criticalExtensions_20_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReestablishmentReject(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "RRCConnectionReestablishmentReject");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReestablishmentReject, RRCConnectionReestablishmentReject_sequence);
return offset;
}
static int
dissect_lte_rrc_T_waitTime(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 16U, NULL, FALSE);
proto_item_append_text(actx->created_item, "s");
return offset;
}
static int
dissect_lte_rrc_T_extendedWaitTime_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 1800U, NULL, FALSE);
proto_item_append_text(actx->created_item, "s");
return offset;
}
static int
dissect_lte_rrc_T_deprioritisationType_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_deprioritisationTimer_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_deprioritisationReq_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_deprioritisationReq_r11, T_deprioritisationReq_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_25(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_25, T_nonCriticalExtension_25_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReject_v1130_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReject_v1130_IEs, RRCConnectionReject_v1130_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReject_v1020_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReject_v1020_IEs, RRCConnectionReject_v1020_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReject_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReject_v8a0_IEs, RRCConnectionReject_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReject_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReject_r8_IEs, RRCConnectionReject_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_22(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_22, T_c1_22_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_22(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_22, T_criticalExtensionsFuture_22_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_22(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_22, T_criticalExtensions_22_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReject(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "RRCConnectionReject");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReject, RRCConnectionReject_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_28(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_28, T_nonCriticalExtension_28_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionSetup_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionSetup_v8a0_IEs, RRCConnectionSetup_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionSetup_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionSetup_r8_IEs, RRCConnectionSetup_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_24(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_24, T_c1_24_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_25(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_25, T_criticalExtensionsFuture_25_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_25(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_25, T_criticalExtensions_25_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionSetup(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
mac_lte_info* p_mac_lte_info;
col_append_str(actx->pinfo->cinfo, COL_INFO, "RRCConnectionSetup");
p_mac_lte_info = (mac_lte_info *)p_get_proto_data(wmem_file_scope(), actx->pinfo, proto_mac_lte, 0);
if (p_mac_lte_info != NULL) {
set_mac_lte_drx_config_release(p_mac_lte_info->ueid, actx->pinfo);
set_mac_lte_extended_bsr_sizes(p_mac_lte_info->ueid, FALSE);
}
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionSetup, RRCConnectionSetup_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_03(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_03, T_c1_03_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_messageClassExtension_03(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_messageClassExtension_03, T_messageClassExtension_03_sequence);
return offset;
}
static int
dissect_lte_rrc_DL_CCCH_MessageType(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DL_CCCH_MessageType, DL_CCCH_MessageType_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_DL_CCCH_Message(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DL_CCCH_Message, DL_CCCH_Message_sequence);
return offset;
}
static int
dissect_lte_rrc_RAND_CDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
32, 32, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_MobilityParametersCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_03(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_03, T_nonCriticalExtension_03_sequence);
return offset;
}
static int
dissect_lte_rrc_CSFBParametersResponseCDMA2000_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSFBParametersResponseCDMA2000_v8a0_IEs, CSFBParametersResponseCDMA2000_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_CSFBParametersResponseCDMA2000_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSFBParametersResponseCDMA2000_r8_IEs, CSFBParametersResponseCDMA2000_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_03(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_03, T_criticalExtensionsFuture_03_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_03(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_03, T_criticalExtensions_03_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_CSFBParametersResponseCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "CSFBParametersResponseCDMA2000");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSFBParametersResponseCDMA2000, CSFBParametersResponseCDMA2000_sequence);
return offset;
}
static int
dissect_lte_rrc_DedicatedInfoNAS(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *nas_eps_tvb=NULL;
proto_tree *subtree;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &nas_eps_tvb);
if ((nas_eps_tvb)&&(nas_eps_handle)) {
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_dedicatedInfoNAS);
call_dissector(nas_eps_handle, nas_eps_tvb, actx->pinfo, subtree);
}
return offset;
}
static int
dissect_lte_rrc_DedicatedInfoCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, NULL);
return offset;
}
static int
dissect_lte_rrc_T_dedicatedInfoType(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_dedicatedInfoType, T_dedicatedInfoType_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_04(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_04, T_nonCriticalExtension_04_sequence);
return offset;
}
static int
dissect_lte_rrc_DLInformationTransfer_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DLInformationTransfer_v8a0_IEs, DLInformationTransfer_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_DLInformationTransfer_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DLInformationTransfer_r8_IEs, DLInformationTransfer_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_08(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_08, T_c1_08_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_04(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_04, T_criticalExtensionsFuture_04_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_04(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_04, T_criticalExtensions_04_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_DLInformationTransfer(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "DLInformationTransfer");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DLInformationTransfer, DLInformationTransfer_sequence);
return offset;
}
static int
dissect_lte_rrc_CDMA2000_Type(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_dualRxTxRedirectIndicator_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_CarrierFreqCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CarrierFreqCDMA2000, CarrierFreqCDMA2000_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_05(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_05, T_nonCriticalExtension_05_sequence);
return offset;
}
static int
dissect_lte_rrc_HandoverFromEUTRAPreparationRequest_v1020_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_HandoverFromEUTRAPreparationRequest_v1020_IEs, HandoverFromEUTRAPreparationRequest_v1020_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_HandoverFromEUTRAPreparationRequest_v920_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_HandoverFromEUTRAPreparationRequest_v920_IEs, HandoverFromEUTRAPreparationRequest_v920_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_HandoverFromEUTRAPreparationRequest_v890_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_HandoverFromEUTRAPreparationRequest_v890_IEs, HandoverFromEUTRAPreparationRequest_v890_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_HandoverFromEUTRAPreparationRequest_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_HandoverFromEUTRAPreparationRequest_r8_IEs, HandoverFromEUTRAPreparationRequest_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_09(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_09, T_c1_09_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_05(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_05, T_criticalExtensionsFuture_05_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_05(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_05, T_criticalExtensions_05_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_HandoverFromEUTRAPreparationRequest(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "HandoverFromEUTRAPreparationRequest");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_HandoverFromEUTRAPreparationRequest, HandoverFromEUTRAPreparationRequest_sequence);
return offset;
}
static int
dissect_lte_rrc_T_targetRAT_Type(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 target_rat_type;
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, &target_rat_type, TRUE, 0, NULL);
private_data_set_rat_target_type(actx, (guint8)target_rat_type);
return offset;
}
static int
dissect_lte_rrc_T_targetRAT_MessageContainer(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *target_rat_msg_cont_tvb = NULL;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &target_rat_msg_cont_tvb);
if(target_rat_msg_cont_tvb){
guint8 byte;
proto_tree *subtree;
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_targetRAT_MessageContainer);
switch (private_data_get_rat_target_type(actx)){
case T_targetRAT_Type_utra:
if (rrc_irat_ho_to_utran_cmd_handle)
call_dissector(rrc_irat_ho_to_utran_cmd_handle, target_rat_msg_cont_tvb, actx->pinfo, subtree);
break;
case T_targetRAT_Type_geran:
byte = tvb_get_guint8(target_rat_msg_cont_tvb, 0);
if (byte == 0x06) {
if (gsm_a_dtap_handle) {
call_dissector(gsm_a_dtap_handle, target_rat_msg_cont_tvb, actx->pinfo, subtree);
}
} else {
if (gsm_rlcmac_dl_handle) {
call_dissector(gsm_rlcmac_dl_handle, target_rat_msg_cont_tvb, actx->pinfo, subtree);
}
}
break;
case T_targetRAT_Type_cdma2000_1XRTT:
break;
case T_targetRAT_Type_cdma2000_HRPD:
break;
default:
break;
}
}
return offset;
}
static int
dissect_lte_rrc_T_nas_SecurityParamFromEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *nas_sec_param_from_eutra_tvb = NULL;
guint32 length;
proto_tree *subtree;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
1, 1, FALSE, &nas_sec_param_from_eutra_tvb);
if (nas_sec_param_from_eutra_tvb) {
length = tvb_reported_length(nas_sec_param_from_eutra_tvb);
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_nas_SecurityParam);
de_emm_sec_par_from_eutra(nas_sec_param_from_eutra_tvb, subtree, actx->pinfo, 0, length, NULL, 0);
}
return offset;
}
static int
dissect_lte_rrc_SystemInfoListGERAN_item(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *sys_info_list_tvb = NULL;
proto_tree *subtree;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
1, 23, FALSE, &sys_info_list_tvb);
if (sys_info_list_tvb) {
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_siPsiSibContainer);
switch (private_data_get_si_or_psi_geran(actx)) {
case SI_OrPSI_GERAN_si:
if (gsm_a_dtap_handle) {
tvbuff_t *si_tvb = tvb_new_composite();
guint8 *pd = (guint8 *) wmem_alloc(actx->pinfo->pool, 1);
pd[0] = 0x06;
tvb_composite_append(si_tvb, tvb_new_real_data(pd, 1, 1));
tvb_composite_append(si_tvb, sys_info_list_tvb);
tvb_composite_finalize(si_tvb);
add_new_data_source(actx->pinfo, si_tvb, "System Information");
call_dissector(gsm_a_dtap_handle, si_tvb, actx->pinfo, subtree);
}
break;
case SI_OrPSI_GERAN_psi:
if (gsm_rlcmac_dl_handle) {
call_dissector(gsm_rlcmac_dl_handle, sys_info_list_tvb, actx->pinfo, subtree);
}
break;
default:
break;
}
}
return offset;
}
static int
dissect_lte_rrc_SystemInfoListGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInfoListGERAN, SystemInfoListGERAN_sequence_of,
1, maxGERAN_SI, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_si(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
private_data_set_si_or_psi_geran(actx, (guint8)SI_OrPSI_GERAN_si);
offset = dissect_lte_rrc_SystemInfoListGERAN(tvb, offset, actx, tree, hf_index);
return offset;
}
static int
dissect_lte_rrc_T_psi(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
private_data_set_si_or_psi_geran(actx, (guint8)SI_OrPSI_GERAN_psi);
offset = dissect_lte_rrc_SystemInfoListGERAN(tvb, offset, actx, tree, hf_index);
return offset;
}
static int
dissect_lte_rrc_SI_OrPSI_GERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SI_OrPSI_GERAN, SI_OrPSI_GERAN_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_Handover(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
private_data_set_rat_target_type(actx, 0xFF);
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_Handover, Handover_sequence);
return offset;
}
static int
dissect_lte_rrc_T_t304(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PhysCellIdGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PhysCellIdGERAN, PhysCellIdGERAN_sequence);
return offset;
}
static int
dissect_lte_rrc_CarrierFreqGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CarrierFreqGERAN, CarrierFreqGERAN_sequence);
return offset;
}
static int
dissect_lte_rrc_T_geran(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_geran, T_geran_sequence);
return offset;
}
static int
dissect_lte_rrc_T_targetRAT_Type_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_targetRAT_Type_01, T_targetRAT_Type_01_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_CellChangeOrder(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellChangeOrder, CellChangeOrder_sequence);
return offset;
}
static int
dissect_lte_rrc_T_purpose(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_purpose, T_purpose_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_14(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_14, T_nonCriticalExtension_14_sequence);
return offset;
}
static int
dissect_lte_rrc_MobilityFromEUTRACommand_v8d0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MobilityFromEUTRACommand_v8d0_IEs, MobilityFromEUTRACommand_v8d0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_MobilityFromEUTRACommand_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MobilityFromEUTRACommand_v8a0_IEs, MobilityFromEUTRACommand_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_MobilityFromEUTRACommand_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MobilityFromEUTRACommand_r8_IEs, MobilityFromEUTRACommand_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_mobilityCDMA2000_HRPD_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_E_CSFB_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_E_CSFB_r9, E_CSFB_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_purpose_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_purpose_01, T_purpose_01_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_15(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_15, T_nonCriticalExtension_15_sequence);
return offset;
}
static int
dissect_lte_rrc_MobilityFromEUTRACommand_v960_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MobilityFromEUTRACommand_v960_IEs, MobilityFromEUTRACommand_v960_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_MobilityFromEUTRACommand_v930_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MobilityFromEUTRACommand_v930_IEs, MobilityFromEUTRACommand_v930_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_MobilityFromEUTRACommand_r9_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MobilityFromEUTRACommand_r9_IEs, MobilityFromEUTRACommand_r9_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_16(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_16, T_c1_16_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_12(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_12, T_criticalExtensionsFuture_12_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_12(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_12, T_criticalExtensions_12_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_MobilityFromEUTRACommand(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "MobilityFromEUTRACommand");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MobilityFromEUTRACommand, MobilityFromEUTRACommand_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasObjectId(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, maxObjectId, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_MeasObjectToRemoveList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasObjectToRemoveList, MeasObjectToRemoveList_sequence_of,
1, maxObjectId, FALSE);
return offset;
}
static int
dissect_lte_rrc_CellIndex(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, maxCellMeas, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_CellIndexList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellIndexList, CellIndexList_sequence_of,
1, maxCellMeas, FALSE);
return offset;
}
static int
dissect_lte_rrc_INTEGER_1_maxCellMeas(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, maxCellMeas, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_CellsToAddMod(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellsToAddMod, CellsToAddMod_sequence);
return offset;
}
static int
dissect_lte_rrc_CellsToAddModList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellsToAddModList, CellsToAddModList_sequence_of,
1, maxCellMeas, FALSE);
return offset;
}
static int
dissect_lte_rrc_BlackCellsToAddMod(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BlackCellsToAddMod, BlackCellsToAddMod_sequence);
return offset;
}
static int
dissect_lte_rrc_BlackCellsToAddModList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BlackCellsToAddModList, BlackCellsToAddModList_sequence_of,
1, maxCellMeas, FALSE);
return offset;
}
static int
dissect_lte_rrc_MeasCycleSCell_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_MeasSubframeCellList_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasSubframeCellList_r10, MeasSubframeCellList_r10_sequence_of,
1, maxCellMeas, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_setup_36(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_36, T_setup_36_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasSubframePatternConfigNeigh_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasSubframePatternConfigNeigh_r10, MeasSubframePatternConfigNeigh_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_MeasObjectEUTRA_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, MeasObjectEUTRA_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasObjectEUTRA_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, MeasObjectEUTRA_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasObjectEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasObjectEUTRA, MeasObjectEUTRA_sequence);
return offset;
}
static int
dissect_lte_rrc_Q_OffsetRangeInterRAT(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-15, 15U, NULL, FALSE);
proto_item_append_text(actx->created_item, "dB");
return offset;
}
static int
dissect_lte_rrc_PhysCellIdUTRA_FDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 511U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_CellsToAddModUTRA_FDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellsToAddModUTRA_FDD, CellsToAddModUTRA_FDD_sequence);
return offset;
}
static int
dissect_lte_rrc_CellsToAddModListUTRA_FDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellsToAddModListUTRA_FDD, CellsToAddModListUTRA_FDD_sequence_of,
1, maxCellMeas, FALSE);
return offset;
}
static int
dissect_lte_rrc_PhysCellIdUTRA_TDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 127U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_CellsToAddModUTRA_TDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellsToAddModUTRA_TDD, CellsToAddModUTRA_TDD_sequence);
return offset;
}
static int
dissect_lte_rrc_CellsToAddModListUTRA_TDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellsToAddModListUTRA_TDD, CellsToAddModListUTRA_TDD_sequence_of,
1, maxCellMeas, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_cellsToAddModList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_cellsToAddModList, T_cellsToAddModList_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_cellForWhichToReportCGI(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_cellForWhichToReportCGI, T_cellForWhichToReportCGI_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_2_512(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
2U, 512U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_PhysCellIdRangeUTRA_FDD_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PhysCellIdRangeUTRA_FDD_r9, PhysCellIdRangeUTRA_FDD_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_PhysCellIdRangeUTRA_FDDList_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PhysCellIdRangeUTRA_FDDList_r9, PhysCellIdRangeUTRA_FDDList_r9_sequence_of,
1, maxPhysCellIdRange_r9, FALSE);
return offset;
}
static int
dissect_lte_rrc_CSG_AllowedReportingCells_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSG_AllowedReportingCells_r9, CSG_AllowedReportingCells_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasObjectUTRA_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, MeasObjectUTRA_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasObjectUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasObjectUTRA, MeasObjectUTRA_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasObjectGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasObjectGERAN, MeasObjectGERAN_sequence);
return offset;
}
static int
dissect_lte_rrc_CellsToAddModCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellsToAddModCDMA2000, CellsToAddModCDMA2000_sequence);
return offset;
}
static int
dissect_lte_rrc_CellsToAddModListCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellsToAddModListCDMA2000, CellsToAddModListCDMA2000_sequence_of,
1, maxCellMeas, FALSE);
return offset;
}
static int
dissect_lte_rrc_MeasObjectCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasObjectCDMA2000, MeasObjectCDMA2000_sequence);
return offset;
}
static int
dissect_lte_rrc_T_measObject(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_measObject, T_measObject_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_MeasObjectToAddMod(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasObjectToAddMod, MeasObjectToAddMod_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasObjectToAddModList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasObjectToAddModList, MeasObjectToAddModList_sequence_of,
1, maxObjectId, FALSE);
return offset;
}
static int
dissect_lte_rrc_ReportConfigId(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, maxReportConfigId, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_ReportConfigToRemoveList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ReportConfigToRemoveList, ReportConfigToRemoveList_sequence_of,
1, maxReportConfigId, FALSE);
return offset;
}
static int
dissect_lte_rrc_RSRP_Range(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 97U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_Threshold_RSRP_Range(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_lte_rrc_RSRP_Range(tvb, offset, actx, tree, hf_index);
return offset;
}
static int
dissect_lte_rrc_RSRQ_Range(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 34U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_Threshold_RSRQ_Range(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_lte_rrc_RSRQ_Range(tvb, offset, actx, tree, hf_index);
return offset;
}
static int
dissect_lte_rrc_ThresholdEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ThresholdEUTRA, ThresholdEUTRA_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_eventA1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_eventA1, T_eventA1_sequence);
return offset;
}
static int
dissect_lte_rrc_T_eventA2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_eventA2, T_eventA2_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_M30_30(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-30, 30U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_eventA3(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_eventA3, T_eventA3_sequence);
return offset;
}
static int
dissect_lte_rrc_T_eventA4(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_eventA4, T_eventA4_sequence);
return offset;
}
static int
dissect_lte_rrc_T_eventA5(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_eventA5, T_eventA5_sequence);
return offset;
}
static int
dissect_lte_rrc_T_eventA6_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_eventA6_r10, T_eventA6_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_eventId(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_eventId, T_eventId_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_Hysteresis(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 30U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_TimeToTrigger(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_event(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_event, T_event_sequence);
return offset;
}
static int
dissect_lte_rrc_T_purpose_02(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_periodical(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_periodical, T_periodical_sequence);
return offset;
}
static int
dissect_lte_rrc_T_triggerType(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_triggerType, T_triggerType_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_triggerQuantity(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_reportQuantity(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_1_maxCellReport(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, maxCellReport, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_ReportInterval(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_reportAmount(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_si_RequestForHO_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_ue_RxTxTimeDiffPeriodical_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_ReportConfigEUTRA_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, ReportConfigEUTRA_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_T_includeLocationInfo_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_reportAddNeighMeas_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_ReportConfigEUTRA_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, ReportConfigEUTRA_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_ReportConfigEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ReportConfigEUTRA, ReportConfigEUTRA_sequence);
return offset;
}
static int
dissect_lte_rrc_Threshold_utra_RSRP(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-5, 91U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_Threshold_utra_EcN0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 49U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_ThresholdUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ThresholdUTRA, ThresholdUTRA_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_ThresholdGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 63U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_ThresholdCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 63U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_b1_Threshold(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_b1_Threshold, T_b1_Threshold_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_eventB1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_eventB1, T_eventB1_sequence);
return offset;
}
static int
dissect_lte_rrc_T_b2_Threshold2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_b2_Threshold2, T_b2_Threshold2_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_eventB2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_eventB2, T_eventB2_sequence);
return offset;
}
static int
dissect_lte_rrc_T_eventId_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_eventId_01, T_eventId_01_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_event_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_event_01, T_event_01_sequence);
return offset;
}
static int
dissect_lte_rrc_T_purpose_03(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
3, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_periodical_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_periodical_01, T_periodical_01_sequence);
return offset;
}
static int
dissect_lte_rrc_T_triggerType_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_triggerType_01, T_triggerType_01_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_reportAmount_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_si_RequestForHO_r9_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_ReportConfigInterRAT_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, ReportConfigInterRAT_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_T_reportQuantityUTRA_FDD_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_ReportConfigInterRAT_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, ReportConfigInterRAT_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_ReportConfigInterRAT_eag_3(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, ReportConfigInterRAT_eag_3_sequence);
return offset;
}
static int
dissect_lte_rrc_ReportConfigInterRAT(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ReportConfigInterRAT, ReportConfigInterRAT_sequence);
return offset;
}
static int
dissect_lte_rrc_T_reportConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_reportConfig, T_reportConfig_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_ReportConfigToAddMod(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ReportConfigToAddMod, ReportConfigToAddMod_sequence);
return offset;
}
static int
dissect_lte_rrc_ReportConfigToAddModList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ReportConfigToAddModList, ReportConfigToAddModList_sequence_of,
1, maxReportConfigId, FALSE);
return offset;
}
static int
dissect_lte_rrc_MeasId(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, maxMeasId, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_MeasIdToRemoveList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasIdToRemoveList, MeasIdToRemoveList_sequence_of,
1, maxMeasId, FALSE);
return offset;
}
static int
dissect_lte_rrc_MeasIdToAddMod(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasIdToAddMod, MeasIdToAddMod_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasIdToAddModList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasIdToAddModList, MeasIdToAddModList_sequence_of,
1, maxMeasId, FALSE);
return offset;
}
static int
dissect_lte_rrc_QuantityConfigEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_QuantityConfigEUTRA, QuantityConfigEUTRA_sequence);
return offset;
}
static int
dissect_lte_rrc_T_measQuantityUTRA_FDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_measQuantityUTRA_TDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_QuantityConfigUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_QuantityConfigUTRA, QuantityConfigUTRA_sequence);
return offset;
}
static int
dissect_lte_rrc_T_measQuantityGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_QuantityConfigGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_QuantityConfigGERAN, QuantityConfigGERAN_sequence);
return offset;
}
static int
dissect_lte_rrc_T_measQuantityCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_QuantityConfigCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_QuantityConfigCDMA2000, QuantityConfigCDMA2000_sequence);
return offset;
}
static int
dissect_lte_rrc_QuantityConfigUTRA_v1020(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_QuantityConfigUTRA_v1020, QuantityConfigUTRA_v1020_sequence);
return offset;
}
static int
dissect_lte_rrc_QuantityConfig_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, QuantityConfig_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_QuantityConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_QuantityConfig, QuantityConfig_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_39(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 39U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_79(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 79U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_gapOffset(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_gapOffset, T_gapOffset_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_35(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_35, T_setup_35_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasGapConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasGapConfig, MeasGapConfig_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_S_Measure(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_lte_rrc_RSRP_Range(tvb, offset, actx, tree, hf_index);
return offset;
}
static int
dissect_lte_rrc_T_setup_34(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_34, T_setup_34_sequence);
return offset;
}
static int
dissect_lte_rrc_T_speedStatePars(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_speedStatePars, T_speedStatePars_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_ARFCN_ValueEUTRA_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
maxEARFCN_Plus1, maxEARFCN2, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_MeasObjectEUTRA_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasObjectEUTRA_v9e0, MeasObjectEUTRA_v9e0_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasObjectToAddMod_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasObjectToAddMod_v9e0, MeasObjectToAddMod_v9e0_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasObjectToAddModList_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasObjectToAddModList_v9e0, MeasObjectToAddModList_v9e0_sequence_of,
1, maxObjectId, FALSE);
return offset;
}
static int
dissect_lte_rrc_MeasConfig_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, MeasConfig_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasConfig, MeasConfig_sequence);
return offset;
}
static int
dissect_lte_rrc_CarrierFreqEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CarrierFreqEUTRA, CarrierFreqEUTRA_sequence);
return offset;
}
static int
dissect_lte_rrc_T_dl_Bandwidth_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_ul_Bandwidth_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_CarrierBandwidthEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CarrierBandwidthEUTRA, CarrierBandwidthEUTRA_sequence);
return offset;
}
static int
dissect_lte_rrc_T_t304_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PRACH_Config(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PRACH_Config, PRACH_Config_sequence);
return offset;
}
static int
dissect_lte_rrc_T_antennaPortsCount(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_AntennaInfoCommon(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AntennaInfoCommon, AntennaInfoCommon_sequence);
return offset;
}
static int
dissect_lte_rrc_RadioResourceConfigCommon_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, RadioResourceConfigCommon_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_RadioResourceConfigCommon_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, RadioResourceConfigCommon_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_RadioResourceConfigCommon(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RadioResourceConfigCommon, RadioResourceConfigCommon_sequence);
return offset;
}
static int
dissect_lte_rrc_RACH_ConfigDedicated(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RACH_ConfigDedicated, RACH_ConfigDedicated_sequence);
return offset;
}
static int
dissect_lte_rrc_CarrierFreqEUTRA_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CarrierFreqEUTRA_v9e0, CarrierFreqEUTRA_v9e0_sequence);
return offset;
}
static int
dissect_lte_rrc_MobilityControlInfo_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, MobilityControlInfo_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_T_drb_ContinueROHC_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_MobilityControlInfo_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, MobilityControlInfo_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_MobilityControlInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MobilityControlInfo, MobilityControlInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_maxDRB_OF_DedicatedInfoNAS(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_maxDRB_OF_DedicatedInfoNAS, SEQUENCE_SIZE_1_maxDRB_OF_DedicatedInfoNAS_sequence_of,
1, maxDRB, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_cipheringAlgorithm(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
pdcp_security_info_t *p_security_algorithms;
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, &value, TRUE, 0, NULL);
p_security_algorithms = private_data_pdcp_security_algorithms(actx);
p_security_algorithms->ciphering = (enum security_ciphering_algorithm_e)value;
return offset;
}
static int
dissect_lte_rrc_T_integrityProtAlgorithm(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
pdcp_security_info_t *p_security_algorithms;
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, &value, TRUE, 0, NULL);
p_security_algorithms = private_data_pdcp_security_algorithms(actx);
p_security_algorithms->integrity = (enum security_integrity_algorithm_e)value;
return offset;
}
static int
dissect_lte_rrc_SecurityAlgorithmConfig(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
pdcp_lte_info *p_pdcp_lte_info;
pdcp_security_info_t *p_security_algorithms;
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SecurityAlgorithmConfig, SecurityAlgorithmConfig_sequence);
p_security_algorithms = private_data_pdcp_security_algorithms(actx);
p_security_algorithms->configuration_frame = actx->pinfo->fd->num;
p_security_algorithms->previous_configuration_frame = 0;
p_security_algorithms->previous_integrity = eia0;
p_security_algorithms->previous_ciphering = eea0;
p_pdcp_lte_info = (pdcp_lte_info *)p_get_proto_data(wmem_file_scope(), actx->pinfo, proto_pdcp_lte, 0);
if (p_pdcp_lte_info != NULL) {
set_pdcp_lte_security_algorithms(p_pdcp_lte_info->ueid, p_security_algorithms);
}
return offset;
}
static int
dissect_lte_rrc_T_intraLTE(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_intraLTE, T_intraLTE_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nas_SecurityParamToEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *nas_sec_param_to_eutra_tvb = NULL;
guint32 length;
proto_tree *subtree;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
6, 6, FALSE, &nas_sec_param_to_eutra_tvb);
if (nas_sec_param_to_eutra_tvb) {
length = tvb_reported_length(nas_sec_param_to_eutra_tvb);
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_nas_SecurityParam);
de_emm_sec_par_to_eutra(nas_sec_param_to_eutra_tvb, subtree, actx->pinfo, 0, length, NULL, 0);
}
return offset;
}
static int
dissect_lte_rrc_T_interRAT(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_interRAT, T_interRAT_sequence);
return offset;
}
static int
dissect_lte_rrc_T_handoverType(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_handoverType, T_handoverType_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_SecurityConfigHO(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SecurityConfigHO, SecurityConfigHO_sequence);
return offset;
}
static int
dissect_lte_rrc_T_proximityIndicationEUTRA_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_proximityIndicationUTRA_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_ReportProximityConfig_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ReportProximityConfig_r9, ReportProximityConfig_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_idc_Indication_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_autonomousDenialSubframes_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_autonomousDenialValidity_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_autonomousDenialParameters_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_autonomousDenialParameters_r11, T_autonomousDenialParameters_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_IDC_Config_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IDC_Config_r11, IDC_Config_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_powerPrefIndicationTimer_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_setup_37(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_setup_37, T_setup_37_sequence);
return offset;
}
static int
dissect_lte_rrc_PowerPrefIndicationConfig_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PowerPrefIndicationConfig_r11, PowerPrefIndicationConfig_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_obtainLocation_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_ObtainLocationConfig_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ObtainLocationConfig_r11, ObtainLocationConfig_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_OtherConfig_r9_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, OtherConfig_r9_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_OtherConfig_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_OtherConfig_r9, OtherConfig_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_fullConfig_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_SCellIndex_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 7U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_SCellToReleaseList_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SCellToReleaseList_r10, SCellToReleaseList_r10_sequence_of,
1, maxSCell_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_cellIdentification_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_cellIdentification_r10, T_cellIdentification_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_dl_Bandwidth_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
6, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonUL_Configuration_r10_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonUL_Configuration_r10_01, T_nonUL_Configuration_r10_01_sequence);
return offset;
}
static int
dissect_lte_rrc_T_ul_Bandwidth_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
6, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_ul_FreqInfo_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_ul_FreqInfo_r10, T_ul_FreqInfo_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_p0_NominalPUSCH_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-126, 24U, NULL, FALSE);
proto_item_append_text(actx->created_item, "dBm");
return offset;
}
static int
dissect_lte_rrc_T_alpha_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_UplinkPowerControlCommonSCell_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UplinkPowerControlCommonSCell_r10, UplinkPowerControlCommonSCell_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_PRACH_ConfigSCell_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PRACH_ConfigSCell_r10, PRACH_ConfigSCell_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_ul_Configuration_r10_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_ul_Configuration_r10_01, T_ul_Configuration_r10_01_sequence);
return offset;
}
static int
dissect_lte_rrc_RadioResourceConfigCommonSCell_r10_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, RadioResourceConfigCommonSCell_r10_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_T_ra_SupervisionInfo_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_ra_SupervisionInfo_r11, T_ra_SupervisionInfo_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_RACH_ConfigCommonSCell_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RACH_ConfigCommonSCell_r11, RACH_ConfigCommonSCell_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_UplinkPowerControlCommonSCell_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UplinkPowerControlCommonSCell_v1130, UplinkPowerControlCommonSCell_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_RadioResourceConfigCommonSCell_r10_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, RadioResourceConfigCommonSCell_r10_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_RadioResourceConfigCommonSCell_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RadioResourceConfigCommonSCell_r10, RadioResourceConfigCommonSCell_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_own_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_own_r10, T_own_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_ServCellIndex_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 7U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_other_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_other_r10, T_other_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_schedulingCellInfo_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_schedulingCellInfo_r10, T_schedulingCellInfo_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_CrossCarrierSchedulingConfig_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CrossCarrierSchedulingConfig_r10, CrossCarrierSchedulingConfig_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonUL_Configuration_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonUL_Configuration_r10, T_nonUL_Configuration_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_groupHoppingDisabled_r10_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_dmrs_WithOCC_Activated_r10_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PUSCH_ConfigDedicatedSCell_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PUSCH_ConfigDedicatedSCell_r10, PUSCH_ConfigDedicatedSCell_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_p0_UE_PUSCH_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-8, 7U, NULL, FALSE);
proto_item_append_text(actx->created_item, "dB");
return offset;
}
static int
dissect_lte_rrc_T_deltaMCS_Enabled_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_pathlossReferenceLinking_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_UplinkPowerControlDedicatedSCell_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UplinkPowerControlDedicatedSCell_r10, UplinkPowerControlDedicatedSCell_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_pmi_RI_Report_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_CQI_ReportConfigSCell_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CQI_ReportConfigSCell_r10, CQI_ReportConfigSCell_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_ul_Configuration_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_ul_Configuration_r10, T_ul_Configuration_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_PhysicalConfigDedicatedSCell_r10_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, PhysicalConfigDedicatedSCell_r10_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_PhysicalConfigDedicatedSCell_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PhysicalConfigDedicatedSCell_r10, PhysicalConfigDedicatedSCell_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_MAC_MainConfigSCell_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MAC_MainConfigSCell_r11, MAC_MainConfigSCell_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_RadioResourceConfigDedicatedSCell_r10_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, RadioResourceConfigDedicatedSCell_r10_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_RadioResourceConfigDedicatedSCell_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RadioResourceConfigDedicatedSCell_r10, RadioResourceConfigDedicatedSCell_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_SCellToAddMod_r10_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, SCellToAddMod_r10_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_SCellToAddMod_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SCellToAddMod_r10, SCellToAddMod_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_SCellToAddModList_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SCellToAddModList_r10, SCellToAddModList_r10_sequence_of,
1, maxSCell_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_systemInfomationBlockType1Dedicated_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string_containing_pdu_new(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, dissect_SystemInformationBlockType1_PDU);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_20(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_20, T_nonCriticalExtension_20_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReconfiguration_v1130_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReconfiguration_v1130_IEs, RRCConnectionReconfiguration_v1130_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReconfiguration_v1020_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReconfiguration_v1020_IEs, RRCConnectionReconfiguration_v1020_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReconfiguration_v920_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReconfiguration_v920_IEs, RRCConnectionReconfiguration_v920_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReconfiguration_v890_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReconfiguration_v890_IEs, RRCConnectionReconfiguration_v890_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReconfiguration_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReconfiguration_r8_IEs, RRCConnectionReconfiguration_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_20(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_20, T_c1_20_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_16(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_16, T_criticalExtensionsFuture_16_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_16(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_16, T_criticalExtensions_16_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReconfiguration(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "RRCConnectionReconfiguration");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReconfiguration, RRCConnectionReconfiguration_sequence);
return offset;
}
static int
dissect_lte_rrc_ReleaseCause(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, &value, FALSE, 0, NULL);
col_append_fstr(actx->pinfo->cinfo, COL_INFO, " [cause=%s]",
val_to_str(value, lte_rrc_ReleaseCause_vals, "Unknown"));
return offset;
}
static int
dissect_lte_rrc_CarrierFreqListUTRA_TDD_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CarrierFreqListUTRA_TDD_r10, CarrierFreqListUTRA_TDD_r10_sequence_of,
1, maxFreqUTRA_TDD_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_RedirectedCarrierInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RedirectedCarrierInfo, RedirectedCarrierInfo_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_FreqPriorityEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_FreqPriorityEUTRA, FreqPriorityEUTRA_sequence);
return offset;
}
static int
dissect_lte_rrc_FreqPriorityListEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_FreqPriorityListEUTRA, FreqPriorityListEUTRA_sequence_of,
1, maxFreq, FALSE);
return offset;
}
static int
dissect_lte_rrc_FreqsPriorityGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_FreqsPriorityGERAN, FreqsPriorityGERAN_sequence);
return offset;
}
static int
dissect_lte_rrc_FreqsPriorityListGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_FreqsPriorityListGERAN, FreqsPriorityListGERAN_sequence_of,
1, maxGNFG, FALSE);
return offset;
}
static int
dissect_lte_rrc_FreqPriorityUTRA_FDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_FreqPriorityUTRA_FDD, FreqPriorityUTRA_FDD_sequence);
return offset;
}
static int
dissect_lte_rrc_FreqPriorityListUTRA_FDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_FreqPriorityListUTRA_FDD, FreqPriorityListUTRA_FDD_sequence_of,
1, maxUTRA_FDD_Carrier, FALSE);
return offset;
}
static int
dissect_lte_rrc_FreqPriorityUTRA_TDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_FreqPriorityUTRA_TDD, FreqPriorityUTRA_TDD_sequence);
return offset;
}
static int
dissect_lte_rrc_FreqPriorityListUTRA_TDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_FreqPriorityListUTRA_TDD, FreqPriorityListUTRA_TDD_sequence_of,
1, maxUTRA_TDD_Carrier, FALSE);
return offset;
}
static int
dissect_lte_rrc_BandClassPriorityHRPD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandClassPriorityHRPD, BandClassPriorityHRPD_sequence);
return offset;
}
static int
dissect_lte_rrc_BandClassPriorityListHRPD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandClassPriorityListHRPD, BandClassPriorityListHRPD_sequence_of,
1, maxCDMA_BandClass, FALSE);
return offset;
}
static int
dissect_lte_rrc_BandClassPriority1XRTT(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandClassPriority1XRTT, BandClassPriority1XRTT_sequence);
return offset;
}
static int
dissect_lte_rrc_BandClassPriorityList1XRTT(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandClassPriorityList1XRTT, BandClassPriorityList1XRTT_sequence_of,
1, maxCDMA_BandClass, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_t320(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_IdleModeMobilityControlInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IdleModeMobilityControlInfo, IdleModeMobilityControlInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_T_lateNonCriticalExtension(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string_containing_pdu_new(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, dissect_RRCConnectionRelease_v9e0_IEs_PDU);
return offset;
}
static int
dissect_lte_rrc_T_systemInformation_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
private_data_set_si_or_psi_geran(actx, (guint8)SI_OrPSI_GERAN_si);
offset = dissect_lte_rrc_SystemInfoListGERAN(tvb, offset, actx, tree, hf_index);
return offset;
}
static int
dissect_lte_rrc_CellInfoGERAN_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellInfoGERAN_r9, CellInfoGERAN_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_CellInfoListGERAN_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellInfoListGERAN_r9, CellInfoListGERAN_r9_sequence_of,
1, maxCellInfoGERAN_r9, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_utra_BCCH_Container_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *utra_bcch_cont_tvb = NULL;
proto_tree *subtree;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &utra_bcch_cont_tvb);
if (utra_bcch_cont_tvb && rrc_sys_info_cont_handle) {
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_siPsiSibContainer);
call_dissector(rrc_sys_info_cont_handle, utra_bcch_cont_tvb, actx->pinfo, subtree);
}
return offset;
}
static int
dissect_lte_rrc_CellInfoUTRA_FDD_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellInfoUTRA_FDD_r9, CellInfoUTRA_FDD_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_CellInfoListUTRA_FDD_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellInfoListUTRA_FDD_r9, CellInfoListUTRA_FDD_r9_sequence_of,
1, maxCellInfoUTRA_r9, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_utra_BCCH_Container_r9_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *utra_bcch_cont_tvb = NULL;
proto_tree *subtree;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &utra_bcch_cont_tvb);
if (utra_bcch_cont_tvb && rrc_sys_info_cont_handle) {
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_siPsiSibContainer);
call_dissector(rrc_sys_info_cont_handle, utra_bcch_cont_tvb, actx->pinfo, subtree);
}
return offset;
}
static int
dissect_lte_rrc_CellInfoUTRA_TDD_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellInfoUTRA_TDD_r9, CellInfoUTRA_TDD_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_CellInfoListUTRA_TDD_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellInfoListUTRA_TDD_r9, CellInfoListUTRA_TDD_r9_sequence_of,
1, maxCellInfoUTRA_r9, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_utra_BCCH_Container_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *utra_bcch_cont_tvb = NULL;
proto_tree *subtree;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &utra_bcch_cont_tvb);
if (utra_bcch_cont_tvb && rrc_sys_info_cont_handle) {
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_siPsiSibContainer);
call_dissector(rrc_sys_info_cont_handle, utra_bcch_cont_tvb, actx->pinfo, subtree);
}
return offset;
}
static int
dissect_lte_rrc_CellInfoUTRA_TDD_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellInfoUTRA_TDD_r10, CellInfoUTRA_TDD_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_CellInfoListUTRA_TDD_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellInfoListUTRA_TDD_r10, CellInfoListUTRA_TDD_r10_sequence_of,
1, maxCellInfoUTRA_r9, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_cellInfoList_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_cellInfoList_r9, T_cellInfoList_r9_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_extendedWaitTime_r10_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 1800U, NULL, FALSE);
proto_item_append_text(actx->created_item, "s");
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_27(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_27, T_nonCriticalExtension_27_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionRelease_v1020_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionRelease_v1020_IEs, RRCConnectionRelease_v1020_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionRelease_v920_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionRelease_v920_IEs, RRCConnectionRelease_v920_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionRelease_v890_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionRelease_v890_IEs, RRCConnectionRelease_v890_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionRelease_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionRelease_r8_IEs, RRCConnectionRelease_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_23(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_23, T_c1_23_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_23(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_23, T_criticalExtensionsFuture_23_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_23(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_23, T_criticalExtensions_23_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionRelease(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "RRCConnectionRelease");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionRelease, RRCConnectionRelease_sequence);
return offset;
}
static int
dissect_lte_rrc_SecurityConfigSMC(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SecurityConfigSMC, SecurityConfigSMC_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_30(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_30, T_nonCriticalExtension_30_sequence);
return offset;
}
static int
dissect_lte_rrc_SecurityModeCommand_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SecurityModeCommand_v8a0_IEs, SecurityModeCommand_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_SecurityModeCommand_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SecurityModeCommand_r8_IEs, SecurityModeCommand_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_26(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_26, T_c1_26_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_27(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_27, T_criticalExtensionsFuture_27_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_27(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_27, T_criticalExtensions_27_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_SecurityModeCommand(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SecurityModeCommand");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SecurityModeCommand, SecurityModeCommand_sequence);
return offset;
}
static int
dissect_lte_rrc_RAT_Type(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 rat_type;
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, &rat_type, TRUE, 0, NULL);
private_data_set_rat_type(actx, (guint8)rat_type);
return offset;
}
static int
dissect_lte_rrc_UE_CapabilityRequest(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_CapabilityRequest, UE_CapabilityRequest_sequence_of,
1, maxRAT_Capabilities, FALSE);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_16_OF_FreqBandIndicator_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_16_OF_FreqBandIndicator_r11, SEQUENCE_SIZE_1_16_OF_FreqBandIndicator_r11_sequence_of,
1, 16, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_37(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_37, T_nonCriticalExtension_37_sequence);
return offset;
}
static int
dissect_lte_rrc_UECapabilityEnquiry_v1180_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UECapabilityEnquiry_v1180_IEs, UECapabilityEnquiry_v1180_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UECapabilityEnquiry_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UECapabilityEnquiry_v8a0_IEs, UECapabilityEnquiry_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UECapabilityEnquiry_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UECapabilityEnquiry_r8_IEs, UECapabilityEnquiry_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_28(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_28, T_c1_28_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_32(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_32, T_criticalExtensionsFuture_32_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_32(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_32, T_criticalExtensions_32_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_UECapabilityEnquiry(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "UECapabilityEnquiry");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UECapabilityEnquiry, UECapabilityEnquiry_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_33554431(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 33554431U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_DRB_CountMSB_Info(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DRB_CountMSB_Info, DRB_CountMSB_Info_sequence);
return offset;
}
static int
dissect_lte_rrc_DRB_CountMSB_InfoList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DRB_CountMSB_InfoList, DRB_CountMSB_InfoList_sequence_of,
1, maxDRB, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension, T_nonCriticalExtension_sequence);
return offset;
}
static int
dissect_lte_rrc_CounterCheck_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CounterCheck_v8a0_IEs, CounterCheck_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_CounterCheck_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CounterCheck_r8_IEs, CounterCheck_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_07(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_07, T_c1_07_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture, T_criticalExtensionsFuture_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions, T_criticalExtensions_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_CounterCheck(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "CounterCheck");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CounterCheck, CounterCheck_sequence);
return offset;
}
static int
dissect_lte_rrc_T_logMeasReportReq_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_connEstFailReportReq_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_39(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_39, T_nonCriticalExtension_39_sequence);
return offset;
}
static int
dissect_lte_rrc_UEInformationRequest_v1130_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UEInformationRequest_v1130_IEs, UEInformationRequest_v1130_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UEInformationRequest_v1020_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UEInformationRequest_v1020_IEs, UEInformationRequest_v1020_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UEInformationRequest_v930_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UEInformationRequest_v930_IEs, UEInformationRequest_v930_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UEInformationRequest_r9_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UEInformationRequest_r9_IEs, UEInformationRequest_r9_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_30(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_30, T_c1_30_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_34(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_34, T_criticalExtensionsFuture_34_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_34(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_34, T_criticalExtensions_34_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_UEInformationRequest_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "UEInformationRequest-r9");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UEInformationRequest_r9, UEInformationRequest_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_TraceReference_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_TraceReference_r10, TraceReference_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_OCTET_STRING_SIZE_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
2, 2, FALSE, NULL);
return offset;
}
static int
dissect_lte_rrc_AbsoluteTimeInfo_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *abs_time_info_tvb = NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
48, 48, FALSE, &abs_time_info_tvb, NULL);
if (abs_time_info_tvb) {
const gchar *str, *hf_str;
proto_tree *subtree;
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_absTimeInfo);
str = tvb_bcd_dig_to_wmem_packet_str(abs_time_info_tvb, 0, 6, NULL, FALSE);
hf_str = wmem_strdup_printf(wmem_packet_scope(), "%c%c-%c%c-%c%c %c%c:%c%c:%c%c", str[0], str[1],
str[2], str[3], str[4], str[5], str[6], str[7], str[8], str[9], str[10], str[11]);
proto_tree_add_string(subtree, hf_lte_rrc_absolute_time, abs_time_info_tvb, 0, 6, hf_str);
}
return offset;
}
static int
dissect_lte_rrc_CellGlobalIdEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellGlobalIdEUTRA, CellGlobalIdEUTRA_sequence);
return offset;
}
static int
dissect_lte_rrc_CellGlobalIdList_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellGlobalIdList_r10, CellGlobalIdList_r10_sequence_of,
1, 32, FALSE);
return offset;
}
static int
dissect_lte_rrc_TrackingAreaCodeList_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_TrackingAreaCodeList_r10, TrackingAreaCodeList_r10_sequence_of,
1, 8, FALSE);
return offset;
}
static int
dissect_lte_rrc_AreaConfiguration_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AreaConfiguration_r10, AreaConfiguration_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_LoggingDuration_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_LoggingInterval_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PLMN_IdentityList3_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PLMN_IdentityList3_r11, PLMN_IdentityList3_r11_sequence_of,
1, 16, FALSE);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_8_OF_PLMN_Identity(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_8_OF_PLMN_Identity, SEQUENCE_SIZE_1_8_OF_PLMN_Identity_sequence_of,
1, 8, FALSE);
return offset;
}
static int
dissect_lte_rrc_TrackingAreaCodeList_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_TrackingAreaCodeList_v1130, TrackingAreaCodeList_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_AreaConfiguration_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AreaConfiguration_v1130, AreaConfiguration_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_08(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_08, T_nonCriticalExtension_08_sequence);
return offset;
}
static int
dissect_lte_rrc_LoggedMeasurementConfiguration_v1130_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_LoggedMeasurementConfiguration_v1130_IEs, LoggedMeasurementConfiguration_v1130_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_LoggedMeasurementConfiguration_v1080_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_LoggedMeasurementConfiguration_v1080_IEs, LoggedMeasurementConfiguration_v1080_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_LoggedMeasurementConfiguration_r10_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_LoggedMeasurementConfiguration_r10_IEs, LoggedMeasurementConfiguration_r10_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_12(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_12, T_c1_12_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_08(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_08, T_criticalExtensionsFuture_08_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_08(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_08, T_criticalExtensions_08_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_LoggedMeasurementConfiguration_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "LoggedMeasurementConfiguration-r10");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_LoggedMeasurementConfiguration_r10, LoggedMeasurementConfiguration_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_systemInformationBlockType1_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string_containing_pdu_new(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, dissect_SystemInformationBlockType1_PDU);
return offset;
}
static int
dissect_lte_rrc_RN_SystemInfo_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RN_SystemInfo_r10, RN_SystemInfo_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_subframeConfigPattern_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_subframeConfigPattern_r10, T_subframeConfigPattern_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_resourceAllocationType_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_13(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
13, 13, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_17(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
17, 17, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_19(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
19, 19, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_25(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
25, 25, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_T_type01_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_type01_r10, T_type01_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
9, 9, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
11, 11, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_T_type2_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_type2_r10, T_type2_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_resourceBlockAssignment_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_resourceBlockAssignment_r10, T_resourceBlockAssignment_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_interleaving_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_noInterleaving_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_demodulationRS_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_demodulationRS_r10, T_demodulationRS_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_1_3(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 3U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_n1PUCCH_AN_List_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_n1PUCCH_AN_List_r10, T_n1PUCCH_AN_List_r10_sequence_of,
1, 4, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_channelSelectionMultiplexingBundling(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_channelSelectionMultiplexingBundling, T_channelSelectionMultiplexingBundling_sequence);
return offset;
}
static int
dissect_lte_rrc_T_fallbackForFormat3(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_fallbackForFormat3, T_fallbackForFormat3_sequence);
return offset;
}
static int
dissect_lte_rrc_T_tdd(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_tdd, T_tdd_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_fdd(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_fdd, T_fdd_sequence);
return offset;
}
static int
dissect_lte_rrc_T_pucch_Config_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_pucch_Config_r10, T_pucch_Config_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_rpdcch_Config_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_rpdcch_Config_r10, T_rpdcch_Config_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_RN_SubframeConfig_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RN_SubframeConfig_r10, RN_SubframeConfig_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_18(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_18, T_nonCriticalExtension_18_sequence);
return offset;
}
static int
dissect_lte_rrc_RNReconfiguration_r10_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RNReconfiguration_r10_IEs, RNReconfiguration_r10_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_18(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_18, T_c1_18_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_14(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_14, T_criticalExtensionsFuture_14_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_14(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_14, T_criticalExtensions_14_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_RNReconfiguration_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "RNReconfiguration-r10");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RNReconfiguration_r10, RNReconfiguration_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_04(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_04, T_c1_04_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_messageClassExtension_04(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_messageClassExtension_04, T_messageClassExtension_04_sequence);
return offset;
}
static int
dissect_lte_rrc_DL_DCCH_MessageType(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DL_DCCH_MessageType, DL_DCCH_MessageType_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_DL_DCCH_Message(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
actx->pinfo->link_dir = P2P_DIR_DL;
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DL_DCCH_Message, DL_DCCH_Message_sequence);
return offset;
}
static int
dissect_lte_rrc_ShortMAC_I(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
16, 16, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_ReestabUE_Identity(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ReestabUE_Identity, ReestabUE_Identity_sequence);
return offset;
}
static int
dissect_lte_rrc_ReestablishmentCause(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReestablishmentRequest_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReestablishmentRequest_r8_IEs, RRCConnectionReestablishmentRequest_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_21(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_21, T_criticalExtensionsFuture_21_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_21(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_21, T_criticalExtensions_21_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReestablishmentRequest(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "RRCConnectionReestablishmentRequest");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReestablishmentRequest, RRCConnectionReestablishmentRequest_sequence);
return offset;
}
static int
dissect_lte_rrc_InitialUE_Identity(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_InitialUE_Identity, InitialUE_Identity_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_EstablishmentCause(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
1, 1, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionRequest_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionRequest_r8_IEs, RRCConnectionRequest_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_24(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_24, T_criticalExtensionsFuture_24_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_24(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_24, T_criticalExtensions_24_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionRequest(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "RRCConnectionRequest");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionRequest, RRCConnectionRequest_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_05(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_05, T_c1_05_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_messageClassExtension_05(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_messageClassExtension_05, T_messageClassExtension_05_sequence);
return offset;
}
static int
dissect_lte_rrc_UL_CCCH_MessageType(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UL_CCCH_MessageType, UL_CCCH_MessageType_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_UL_CCCH_Message(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UL_CCCH_Message, UL_CCCH_Message_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_02(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_02, T_nonCriticalExtension_02_sequence);
return offset;
}
static int
dissect_lte_rrc_CSFBParametersRequestCDMA2000_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSFBParametersRequestCDMA2000_v8a0_IEs, CSFBParametersRequestCDMA2000_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_CSFBParametersRequestCDMA2000_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSFBParametersRequestCDMA2000_r8_IEs, CSFBParametersRequestCDMA2000_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_02(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_02, T_criticalExtensionsFuture_02_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_02(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_02, T_criticalExtensions_02_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_CSFBParametersRequestCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "CSFBParametersRequestCDMA2000");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSFBParametersRequestCDMA2000, CSFBParametersRequestCDMA2000_sequence);
return offset;
}
static int
dissect_lte_rrc_T_measResultPCell(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_measResultPCell, T_measResultPCell_sequence);
return offset;
}
static int
dissect_lte_rrc_PLMN_IdentityList2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PLMN_IdentityList2, PLMN_IdentityList2_sequence_of,
1, 5, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_cgi_Info(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_cgi_Info, T_cgi_Info_sequence);
return offset;
}
static int
dissect_lte_rrc_T_csg_MemberStatus_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_AdditionalSI_Info_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AdditionalSI_Info_r9, AdditionalSI_Info_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_eag_1_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, T_eag_1_01_sequence);
return offset;
}
static int
dissect_lte_rrc_T_measResult(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_measResult, T_measResult_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResultEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultEUTRA, MeasResultEUTRA_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResultListEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultListEUTRA, MeasResultListEUTRA_sequence_of,
1, maxCellReport, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_physCellId(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_physCellId, T_physCellId_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_28(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
28, 28, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_CellGlobalIdUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellGlobalIdUTRA, CellGlobalIdUTRA_sequence);
return offset;
}
static int
dissect_lte_rrc_T_cgi_Info_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_cgi_Info_01, T_cgi_Info_01_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_M5_91(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
-5, 91U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_49(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 49U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_eag_1_02(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, T_eag_1_02_sequence);
return offset;
}
static int
dissect_lte_rrc_T_measResult_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_measResult_01, T_measResult_01_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResultUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultUTRA, MeasResultUTRA_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResultListUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultListUTRA, MeasResultListUTRA_sequence_of,
1, maxCellReport, FALSE);
return offset;
}
static int
dissect_lte_rrc_CellGlobalIdGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellGlobalIdGERAN, CellGlobalIdGERAN_sequence);
return offset;
}
static int
dissect_lte_rrc_T_cgi_Info_02(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_cgi_Info_02, T_cgi_Info_02_sequence);
return offset;
}
static int
dissect_lte_rrc_T_measResult_02(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_measResult_02, T_measResult_02_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResultGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultGERAN, MeasResultGERAN_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResultListGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultListGERAN, MeasResultListGERAN_sequence_of,
1, maxCellReport, FALSE);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_47(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
47, 47, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_128(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
128, 128, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_CellGlobalIdCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CellGlobalIdCDMA2000, CellGlobalIdCDMA2000_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_32767(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 32767U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_measResult_03(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_measResult_03, T_measResult_03_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResultCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultCDMA2000, MeasResultCDMA2000_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResultListCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultListCDMA2000, MeasResultListCDMA2000_sequence_of,
1, maxCellReport, FALSE);
return offset;
}
static int
dissect_lte_rrc_MeasResultsCDMA2000(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultsCDMA2000, MeasResultsCDMA2000_sequence);
return offset;
}
static int
dissect_lte_rrc_T_measResultNeighCells(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_measResultNeighCells, T_measResultNeighCells_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_4095(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 4095U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_MeasResultForECID_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultForECID_r9, MeasResultForECID_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResults_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, MeasResults_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_T_ellipsoid_Point_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *ellipsoid_point_tvb = NULL;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &ellipsoid_point_tvb);
if (ellipsoid_point_tvb) {
dissect_lpp_Ellipsoid_Point_PDU(ellipsoid_point_tvb, actx->pinfo, tree, NULL);
}
return offset;
}
static int
dissect_lte_rrc_T_ellipsoidPointWithAltitude_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *ellipsoid_point_with_alt_tvb = NULL;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &ellipsoid_point_with_alt_tvb);
if (ellipsoid_point_with_alt_tvb) {
dissect_lpp_EllipsoidPointWithAltitude_PDU(ellipsoid_point_with_alt_tvb, actx->pinfo, tree, NULL);
}
return offset;
}
static int
dissect_lte_rrc_T_ellipsoidPointWithUncertaintyCircle_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *ellipsoid_point_with_uncert_circ_tvb = NULL;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &ellipsoid_point_with_uncert_circ_tvb);
if (ellipsoid_point_with_uncert_circ_tvb) {
dissect_lpp_Ellipsoid_PointWithUncertaintyCircle_PDU(ellipsoid_point_with_uncert_circ_tvb, actx->pinfo, tree, NULL);
}
return offset;
}
static int
dissect_lte_rrc_T_ellipsoidPointWithUncertaintyEllipse_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *ellipsoid_point_with_uncert_ellipse_tvb = NULL;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &ellipsoid_point_with_uncert_ellipse_tvb);
if (ellipsoid_point_with_uncert_ellipse_tvb) {
dissect_lpp_EllipsoidPointWithUncertaintyEllipse_PDU(ellipsoid_point_with_uncert_ellipse_tvb, actx->pinfo, tree, NULL);
}
return offset;
}
static int
dissect_lte_rrc_T_ellipsoidPointWithAltitudeAndUncertaintyEllipsoid_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *ellipsoid_point_with_alt_uncert_ellipsoid_tvb = NULL;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &ellipsoid_point_with_alt_uncert_ellipsoid_tvb);
if (ellipsoid_point_with_alt_uncert_ellipsoid_tvb) {
dissect_lpp_EllipsoidPointWithAltitudeAndUncertaintyEllipsoid_PDU(ellipsoid_point_with_alt_uncert_ellipsoid_tvb, actx->pinfo, tree, NULL);
}
return offset;
}
static int
dissect_lte_rrc_T_ellipsoidArc_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *ellipsoid_arc_tvb = NULL;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &ellipsoid_arc_tvb);
if (ellipsoid_arc_tvb) {
dissect_lpp_EllipsoidArc_PDU(ellipsoid_arc_tvb, actx->pinfo, tree, NULL);
}
return offset;
}
static int
dissect_lte_rrc_T_polygon_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *polygon_tvb = NULL;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &polygon_tvb);
if (polygon_tvb) {
dissect_lpp_Polygon_PDU(polygon_tvb, actx->pinfo, tree, NULL);
}
return offset;
}
static int
dissect_lte_rrc_T_locationCoordinates_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_locationCoordinates_r10, T_locationCoordinates_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_horizontalVelocity_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *horizontal_velocity_tvb = NULL;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &horizontal_velocity_tvb);
if (horizontal_velocity_tvb) {
dissect_lpp_HorizontalVelocity_PDU(horizontal_velocity_tvb, actx->pinfo, tree, NULL);
}
return offset;
}
static int
dissect_lte_rrc_T_gnss_TOD_msec_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *gnss_tod_msec_tvb = NULL;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &gnss_tod_msec_tvb);
if (gnss_tod_msec_tvb) {
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, actx->pinfo);
dissect_per_constrained_integer(gnss_tod_msec_tvb, 0, &asn1_ctx, tree, hf_index, 0U, 3599999U, NULL, FALSE);
}
return offset;
}
static int
dissect_lte_rrc_LocationInfo_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_LocationInfo_r10, LocationInfo_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_measResultSCell_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_measResultSCell_r10, T_measResultSCell_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_measResultBestNeighCell_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_measResultBestNeighCell_r10, T_measResultBestNeighCell_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResultServFreq_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultServFreq_r10, MeasResultServFreq_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResultServFreqList_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultServFreqList_r10, MeasResultServFreqList_r10_sequence_of,
1, maxServCell_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_MeasResults_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, MeasResults_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResults(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResults, MeasResults_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_13(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_13, T_nonCriticalExtension_13_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasurementReport_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasurementReport_v8a0_IEs, MeasurementReport_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasurementReport_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasurementReport_r8_IEs, MeasurementReport_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_15(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_15, T_c1_15_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_11, T_criticalExtensionsFuture_11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_11, T_criticalExtensions_11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_MeasurementReport(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "MeasurementReport");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasurementReport, MeasurementReport_sequence);
return offset;
}
static int
dissect_lte_rrc_T_rlf_InfoAvailable_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_logMeasAvailable_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_connEstFailInfoAvailable_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_21(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_21, T_nonCriticalExtension_21_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReconfigurationComplete_v1130_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReconfigurationComplete_v1130_IEs, RRCConnectionReconfigurationComplete_v1130_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReconfigurationComplete_v1020_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReconfigurationComplete_v1020_IEs, RRCConnectionReconfigurationComplete_v1020_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReconfigurationComplete_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReconfigurationComplete_v8a0_IEs, RRCConnectionReconfigurationComplete_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReconfigurationComplete_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReconfigurationComplete_r8_IEs, RRCConnectionReconfigurationComplete_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_17(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_17, T_criticalExtensionsFuture_17_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_17(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_17, T_criticalExtensions_17_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReconfigurationComplete(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "RRCConnectionReconfigurationComplete");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReconfigurationComplete, RRCConnectionReconfigurationComplete_sequence);
return offset;
}
static int
dissect_lte_rrc_T_rlf_InfoAvailable_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_logMeasAvailable_r10_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_connEstFailInfoAvailable_r11_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_23(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_23, T_nonCriticalExtension_23_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReestablishmentComplete_v1130_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReestablishmentComplete_v1130_IEs, RRCConnectionReestablishmentComplete_v1130_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReestablishmentComplete_v1020_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReestablishmentComplete_v1020_IEs, RRCConnectionReestablishmentComplete_v1020_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReestablishmentComplete_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReestablishmentComplete_v8a0_IEs, RRCConnectionReestablishmentComplete_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReestablishmentComplete_v920_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReestablishmentComplete_v920_IEs, RRCConnectionReestablishmentComplete_v920_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReestablishmentComplete_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReestablishmentComplete_r8_IEs, RRCConnectionReestablishmentComplete_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_19(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_19, T_criticalExtensionsFuture_19_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_19(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_19, T_criticalExtensions_19_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionReestablishmentComplete(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "RRCConnectionReestablishmentComplete");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionReestablishmentComplete, RRCConnectionReestablishmentComplete_sequence);
return offset;
}
static int
dissect_lte_rrc_RegisteredMME(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RegisteredMME, RegisteredMME_sequence);
return offset;
}
static int
dissect_lte_rrc_T_gummei_Type_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_rlf_InfoAvailable_r10_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_logMeasAvailable_r10_02(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_rn_SubframeConfigReq_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_connEstFailInfoAvailable_r11_02(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_29(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_29, T_nonCriticalExtension_29_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionSetupComplete_v1130_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionSetupComplete_v1130_IEs, RRCConnectionSetupComplete_v1130_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionSetupComplete_v1020_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionSetupComplete_v1020_IEs, RRCConnectionSetupComplete_v1020_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionSetupComplete_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionSetupComplete_v8a0_IEs, RRCConnectionSetupComplete_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionSetupComplete_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionSetupComplete_r8_IEs, RRCConnectionSetupComplete_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_25(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_25, T_c1_25_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_26(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_26, T_criticalExtensionsFuture_26_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_26(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_26, T_criticalExtensions_26_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionSetupComplete(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "RRCConnectionSetupComplete");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionSetupComplete, RRCConnectionSetupComplete_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_31(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_31, T_nonCriticalExtension_31_sequence);
return offset;
}
static int
dissect_lte_rrc_SecurityModeComplete_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SecurityModeComplete_v8a0_IEs, SecurityModeComplete_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_SecurityModeComplete_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SecurityModeComplete_r8_IEs, SecurityModeComplete_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_28(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_28, T_criticalExtensionsFuture_28_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_28(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_28, T_criticalExtensions_28_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_SecurityModeComplete(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "SecurityModeComplete");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SecurityModeComplete, SecurityModeComplete_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_32(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_32, T_nonCriticalExtension_32_sequence);
return offset;
}
static int
dissect_lte_rrc_SecurityModeFailure_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SecurityModeFailure_v8a0_IEs, SecurityModeFailure_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_SecurityModeFailure_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SecurityModeFailure_r8_IEs, SecurityModeFailure_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_29(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_29, T_criticalExtensionsFuture_29_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_29(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_29, T_criticalExtensions_29_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_SecurityModeFailure(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
mac_lte_info *p_mac_lte_info;
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SecurityModeFailure, SecurityModeFailure_sequence);
p_mac_lte_info = (mac_lte_info *)p_get_proto_data(wmem_file_scope(), actx->pinfo, proto_mac_lte, 0);
if (p_mac_lte_info != NULL) {
set_pdcp_lte_security_algorithms_failed(p_mac_lte_info->ueid);
}
col_append_str(actx->pinfo->cinfo, COL_INFO, "SecurityModeFailure");
return offset;
}
static int
dissect_lte_rrc_T_ueCapabilityRAT_Container(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *ue_cap_tvb=NULL;
offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, &ue_cap_tvb);
if(ue_cap_tvb){
guint32 length;
proto_item *item;
proto_tree *subtree, *subtree2;
guint8 byte;
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_UE_CapabilityRAT_Container);
switch(private_data_get_rat_type(actx)){
case RAT_Type_eutra:
dissect_lte_rrc_UE_EUTRA_Capability_PDU(ue_cap_tvb, actx->pinfo, subtree, NULL);
break;
case RAT_Type_utra:
dissect_rrc_InterRATHandoverInfo_PDU(ue_cap_tvb, actx->pinfo, subtree, NULL);
break;
case RAT_Type_geran_cs:
subtree2 = proto_tree_add_subtree(subtree, ue_cap_tvb, 0, 5,
ett_lte_rrc_UE_CapabilityRAT_Container, &item, "Mobile Station Classmark 2");
byte = tvb_get_guint8(ue_cap_tvb, 0);
if (byte != 0x33) {
expert_add_info_format(actx->pinfo, item, &ei_lte_rrc_unexpected_type_value,
"Unexpected type value (found 0x%02X)", byte);
}
byte = tvb_get_guint8(ue_cap_tvb, 1);
if (byte != 0x03) {
expert_add_info_format(actx->pinfo, item, &ei_lte_rrc_unexpected_length_value,
"Unexpected length value (found %d)", byte);
}
de_ms_cm_2(ue_cap_tvb, subtree2, actx->pinfo, 2, 3, NULL, 0);
length = tvb_reported_length_remaining(ue_cap_tvb, 5);
subtree2 = proto_tree_add_subtree(subtree, ue_cap_tvb, 5, length,
ett_lte_rrc_UE_CapabilityRAT_Container, NULL, "Mobile Station Classmark 3");
de_ms_cm_3(ue_cap_tvb, subtree2, actx->pinfo, 5, length, NULL, 0);
break;
case RAT_Type_geran_ps:
length = tvb_reported_length(ue_cap_tvb);
subtree2 = proto_tree_add_subtree(subtree, ue_cap_tvb, 0, length,
ett_lte_rrc_UE_CapabilityRAT_Container, NULL, "MS Radio Access Capability");
de_gmm_ms_radio_acc_cap(ue_cap_tvb, subtree2, actx->pinfo, 0, length, NULL, 0);
break;
case RAT_Type_cdma2000_1XRTT:
break;
default:
break;
}
}
return offset;
}
static int
dissect_lte_rrc_UE_CapabilityRAT_Container(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
private_data_set_rat_type(actx, 0xFF);
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_CapabilityRAT_Container, UE_CapabilityRAT_Container_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_CapabilityRAT_ContainerList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_CapabilityRAT_ContainerList, UE_CapabilityRAT_ContainerList_sequence_of,
0, maxRAT_Capabilities, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_38(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_38, T_nonCriticalExtension_38_sequence);
return offset;
}
static int
dissect_lte_rrc_UECapabilityInformation_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UECapabilityInformation_v8a0_IEs, UECapabilityInformation_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UECapabilityInformation_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UECapabilityInformation_r8_IEs, UECapabilityInformation_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_29(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_29, T_c1_29_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_33(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_33, T_criticalExtensionsFuture_33_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_33(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_33, T_criticalExtensions_33_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_UECapabilityInformation(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "UECapabilityInformation");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UECapabilityInformation, UECapabilityInformation_sequence);
return offset;
}
static int
dissect_lte_rrc_BIT_STRING_SIZE_56(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
56, 56, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_42(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_42, T_nonCriticalExtension_42_sequence);
return offset;
}
static int
dissect_lte_rrc_ULHandoverPreparationTransfer_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ULHandoverPreparationTransfer_v8a0_IEs, ULHandoverPreparationTransfer_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_ULHandoverPreparationTransfer_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ULHandoverPreparationTransfer_r8_IEs, ULHandoverPreparationTransfer_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_32(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_32, T_c1_32_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_36(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_36, T_criticalExtensionsFuture_36_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_36(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_36, T_criticalExtensions_36_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_ULHandoverPreparationTransfer(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "ULHandoverPreparationTransfer");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ULHandoverPreparationTransfer, ULHandoverPreparationTransfer_sequence);
return offset;
}
static int
dissect_lte_rrc_T_dedicatedInfoType_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_dedicatedInfoType_01, T_dedicatedInfoType_01_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_43(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_43, T_nonCriticalExtension_43_sequence);
return offset;
}
static int
dissect_lte_rrc_ULInformationTransfer_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ULInformationTransfer_v8a0_IEs, ULInformationTransfer_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_ULInformationTransfer_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ULInformationTransfer_r8_IEs, ULInformationTransfer_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_33(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_33, T_c1_33_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_37(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_37, T_criticalExtensionsFuture_37_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_37(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_37, T_criticalExtensions_37_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_ULInformationTransfer(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "ULInformationTransfer");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ULInformationTransfer, ULInformationTransfer_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_4294967295(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 4294967295U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_DRB_CountInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DRB_CountInfo, DRB_CountInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_DRB_CountInfoList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_DRB_CountInfoList, DRB_CountInfoList_sequence_of,
0, maxDRB, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_01, T_nonCriticalExtension_01_sequence);
return offset;
}
static int
dissect_lte_rrc_CounterCheckResponse_v8a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CounterCheckResponse_v8a0_IEs, CounterCheckResponse_v8a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_CounterCheckResponse_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CounterCheckResponse_r8_IEs, CounterCheckResponse_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_01, T_criticalExtensionsFuture_01_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_01, T_criticalExtensions_01_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_CounterCheckResponse(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "CounterCheckResponse");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CounterCheckResponse, CounterCheckResponse_sequence);
return offset;
}
static int
dissect_lte_rrc_NumberOfPreamblesSent_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 200U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_rach_Report_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_rach_Report_r9, T_rach_Report_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_measResultLastServCell_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_measResultLastServCell_r9, T_measResultLastServCell_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResult2EUTRA_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResult2EUTRA_r9, MeasResult2EUTRA_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResultList2EUTRA_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultList2EUTRA_r9, MeasResultList2EUTRA_r9_sequence_of,
1, maxFreq, FALSE);
return offset;
}
static int
dissect_lte_rrc_MeasResult2UTRA_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResult2UTRA_r9, MeasResult2UTRA_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResultList2UTRA_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultList2UTRA_r9, MeasResultList2UTRA_r9_sequence_of,
1, maxFreq, FALSE);
return offset;
}
static int
dissect_lte_rrc_MeasResult2CDMA2000_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResult2CDMA2000_r9, MeasResult2CDMA2000_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResultList2CDMA2000_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultList2CDMA2000_r9, MeasResultList2CDMA2000_r9_sequence_of,
1, maxFreq, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_measResultNeighCells_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_measResultNeighCells_r9, T_measResultNeighCells_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_pci_arfcn_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_pci_arfcn_r10, T_pci_arfcn_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_failedPCellId_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_failedPCellId_r10, T_failedPCellId_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_connectionFailureType_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_RLF_Report_r9_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, RLF_Report_r9_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_T_failedPCellId_v1090(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_failedPCellId_v1090, T_failedPCellId_v1090_sequence);
return offset;
}
static int
dissect_lte_rrc_RLF_Report_r9_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, RLF_Report_r9_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_T_rlf_Cause_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_TimeSinceFailure_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 172800U, NULL, FALSE);
proto_item_append_text(actx->created_item, "s");
return offset;
}
static int
dissect_lte_rrc_T_basicFields_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_basicFields_r11, T_basicFields_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_physCellId_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_physCellId_r11, T_physCellId_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_previousUTRA_CellId_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_previousUTRA_CellId_r11, T_previousUTRA_CellId_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_physCellId_r11_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_physCellId_r11_01, T_physCellId_r11_01_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_selectedUTRA_CellId_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_selectedUTRA_CellId_r11, T_selectedUTRA_CellId_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_RLF_Report_r9_eag_3(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, RLF_Report_r9_eag_3_sequence);
return offset;
}
static int
dissect_lte_rrc_RLF_Report_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RLF_Report_r9, RLF_Report_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_lateNonCriticalExtension_02(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string_containing_pdu_new(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, dissect_UEInformationResponse_v9e0_IEs_PDU);
return offset;
}
static int
dissect_lte_rrc_T_relativeTimeStamp_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, 7200U, NULL, FALSE);
proto_item_append_text(actx->created_item, "s");
return offset;
}
static int
dissect_lte_rrc_T_measResultServCell_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_measResultServCell_r10, T_measResultServCell_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResultList2GERAN_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultList2GERAN_r10, MeasResultList2GERAN_r10_sequence_of,
1, maxCellListGERAN, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_measResultNeighCells_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_measResultNeighCells_r10, T_measResultNeighCells_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResult2EUTRA_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResult2EUTRA_v9e0, MeasResult2EUTRA_v9e0_sequence);
return offset;
}
static int
dissect_lte_rrc_MeasResultList2EUTRA_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasResultList2EUTRA_v9e0, MeasResultList2EUTRA_v9e0_sequence_of,
1, maxFreq, FALSE);
return offset;
}
static int
dissect_lte_rrc_LogMeasInfo_r10_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, LogMeasInfo_r10_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_LogMeasInfo_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_LogMeasInfo_r10, LogMeasInfo_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_LogMeasInfoList_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_LogMeasInfoList_r10, LogMeasInfoList_r10_sequence_of,
1, maxLogMeasReport_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_logMeasAvailable_r10_03(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_LogMeasReport_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_LogMeasReport_r10, LogMeasReport_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_measResultFailedCell_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_measResultFailedCell_r11, T_measResultFailedCell_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_measResultNeighCells_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_measResultNeighCells_r11, T_measResultNeighCells_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_ConnEstFailReport_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ConnEstFailReport_r11, ConnEstFailReport_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_41(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_41, T_nonCriticalExtension_41_sequence);
return offset;
}
static int
dissect_lte_rrc_UEInformationResponse_v1130_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UEInformationResponse_v1130_IEs, UEInformationResponse_v1130_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UEInformationResponse_v1020_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UEInformationResponse_v1020_IEs, UEInformationResponse_v1020_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UEInformationResponse_v930_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UEInformationResponse_v930_IEs, UEInformationResponse_v930_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UEInformationResponse_r9_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UEInformationResponse_r9_IEs, UEInformationResponse_r9_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_31(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_31, T_c1_31_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_35(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_35, T_criticalExtensionsFuture_35_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_35(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_35, T_criticalExtensions_35_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_UEInformationResponse_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "UEInformationResponse-r9");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UEInformationResponse_r9, UEInformationResponse_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_type_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_carrierFreq_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_carrierFreq_r9, T_carrierFreq_r9_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_17(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_17, T_nonCriticalExtension_17_sequence);
return offset;
}
static int
dissect_lte_rrc_ProximityIndication_v930_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ProximityIndication_v930_IEs, ProximityIndication_v930_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_ProximityIndication_r9_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ProximityIndication_r9_IEs, ProximityIndication_r9_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_17(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_17, T_c1_17_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_13(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_13, T_criticalExtensionsFuture_13_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_13(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_13, T_criticalExtensions_13_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_ProximityIndication_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "ProximityIndication-r9");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ProximityIndication_r9, ProximityIndication_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_19(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_19, T_nonCriticalExtension_19_sequence);
return offset;
}
static int
dissect_lte_rrc_RNReconfigurationComplete_r10_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RNReconfigurationComplete_r10_IEs, RNReconfigurationComplete_r10_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_19(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_19, T_c1_19_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_15(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_15, T_criticalExtensionsFuture_15_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_15(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_15, T_criticalExtensions_15_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_RNReconfigurationComplete_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "RNReconfigurationComplete-r10");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RNReconfigurationComplete_r10, RNReconfigurationComplete_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_maxMBSFN_Area_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, maxMBSFN_Area_1, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_INTEGER_0_maxServiceCount_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
0U, maxServiceCount_1, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_CountingResponseInfo_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CountingResponseInfo_r10, CountingResponseInfo_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_CountingResponseList_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CountingResponseList_r10, CountingResponseList_r10_sequence_of,
1, maxServiceCount, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_10, T_nonCriticalExtension_10_sequence);
return offset;
}
static int
dissect_lte_rrc_MBMSCountingResponse_r10_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBMSCountingResponse_r10_IEs, MBMSCountingResponse_r10_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_13(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_13, T_c1_13_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_09(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_09, T_criticalExtensionsFuture_09_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_09(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_09, T_criticalExtensions_09_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_MBMSCountingResponse_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "MBMSCountingResponse-r10");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBMSCountingResponse_r10, MBMSCountingResponse_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_RSTD_InterFreqInfo_r10_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, RSTD_InterFreqInfo_r10_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_RSTD_InterFreqInfo_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RSTD_InterFreqInfo_r10, RSTD_InterFreqInfo_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_RSTD_InterFreqInfoList_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RSTD_InterFreqInfoList_r10, RSTD_InterFreqInfoList_r10_sequence_of,
1, maxRSTD_Freq_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_start(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_start, T_start_sequence);
return offset;
}
static int
dissect_lte_rrc_T_rstd_InterFreqIndication_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_rstd_InterFreqIndication_r10, T_rstd_InterFreqIndication_r10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_07(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_07, T_nonCriticalExtension_07_sequence);
return offset;
}
static int
dissect_lte_rrc_InterFreqRSTDMeasurementIndication_r10_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_InterFreqRSTDMeasurementIndication_r10_IEs, InterFreqRSTDMeasurementIndication_r10_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_11, T_c1_11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_07(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_07, T_criticalExtensionsFuture_07_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_07(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_07, T_criticalExtensions_07_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_InterFreqRSTDMeasurementIndication_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "InterFreqRSTDMeasurementIndication-r10");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_InterFreqRSTDMeasurementIndication_r10, InterFreqRSTDMeasurementIndication_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_06(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_06, T_c1_06_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_powerPrefIndication_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_36(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_36, T_nonCriticalExtension_36_sequence);
return offset;
}
static int
dissect_lte_rrc_UEAssistanceInformation_r11_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UEAssistanceInformation_r11_IEs, UEAssistanceInformation_r11_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_27(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_27, T_c1_27_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_31(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_31, T_criticalExtensionsFuture_31_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_31(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_31, T_criticalExtensions_31_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_UEAssistanceInformation_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "UEAssistanceInformation-r11");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UEAssistanceInformation_r11, UEAssistanceInformation_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_interferenceDirection_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
4, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_AffectedCarrierFreq_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AffectedCarrierFreq_r11, AffectedCarrierFreq_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_AffectedCarrierFreqList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AffectedCarrierFreqList_r11, AffectedCarrierFreqList_r11_sequence_of,
1, maxFreqIDC_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_drx_CycleLength_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_drx_ActiveTime_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_drx_AssistanceInfo_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_drx_AssistanceInfo_r11, T_drx_AssistanceInfo_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_subframePatternTDD_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_subframePatternTDD_r11, T_subframePatternTDD_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_IDC_SubframePattern_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IDC_SubframePattern_r11, IDC_SubframePattern_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_IDC_SubframePatternList_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IDC_SubframePatternList_r11, IDC_SubframePatternList_r11_sequence_of,
1, maxSubframePatternIDC_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_TDM_AssistanceInfo_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_TDM_AssistanceInfo_r11, TDM_AssistanceInfo_r11_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_06(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_06, T_nonCriticalExtension_06_sequence);
return offset;
}
static int
dissect_lte_rrc_InDeviceCoexIndication_r11_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_InDeviceCoexIndication_r11_IEs, InDeviceCoexIndication_r11_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_10, T_c1_10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_06(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_06, T_criticalExtensionsFuture_06_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_06(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_06, T_criticalExtensions_06_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_InDeviceCoexIndication_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "InDeviceCoexIndication-r11");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_InDeviceCoexIndication_r11, InDeviceCoexIndication_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_CarrierFreqListMBMS_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CarrierFreqListMBMS_r11, CarrierFreqListMBMS_r11_sequence_of,
1, maxFreqMBMS_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_mbms_Priority_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_11, T_nonCriticalExtension_11_sequence);
return offset;
}
static int
dissect_lte_rrc_MBMSInterestIndication_r11_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBMSInterestIndication_r11_IEs, MBMSInterestIndication_r11_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_14(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_14, T_c1_14_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_10, T_criticalExtensionsFuture_10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_10, T_criticalExtensions_10_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_MBMSInterestIndication_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
col_append_str(actx->pinfo->cinfo, COL_INFO, "MBMSInterestIndication-r11");
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBMSInterestIndication_r11, MBMSInterestIndication_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c2_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c2_01, T_c2_01_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_messageClassExtensionFuture_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_messageClassExtensionFuture_r11, T_messageClassExtensionFuture_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_T_messageClassExtension_06(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_messageClassExtension_06, T_messageClassExtension_06_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_UL_DCCH_MessageType(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UL_DCCH_MessageType, UL_DCCH_MessageType_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_UL_DCCH_Message(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
actx->pinfo->link_dir = P2P_DIR_UL;
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UL_DCCH_Message, UL_DCCH_Message_sequence);
return offset;
}
static int
dissect_lte_rrc_RedirectedCarrierInfo_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RedirectedCarrierInfo_v9e0, RedirectedCarrierInfo_v9e0_sequence);
return offset;
}
static int
dissect_lte_rrc_FreqPriorityEUTRA_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_FreqPriorityEUTRA_v9e0, FreqPriorityEUTRA_v9e0_sequence);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_maxFreq_OF_FreqPriorityEUTRA_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_maxFreq_OF_FreqPriorityEUTRA_v9e0, SEQUENCE_SIZE_1_maxFreq_OF_FreqPriorityEUTRA_v9e0_sequence_of,
1, maxFreq, FALSE);
return offset;
}
static int
dissect_lte_rrc_IdleModeMobilityControlInfo_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IdleModeMobilityControlInfo_v9e0, IdleModeMobilityControlInfo_v9e0_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_26(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_26, T_nonCriticalExtension_26_sequence);
return offset;
}
static int
dissect_lte_rrc_RRCConnectionRelease_v9e0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRCConnectionRelease_v9e0_IEs, RRCConnectionRelease_v9e0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_MultiBandInfoList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MultiBandInfoList, MultiBandInfoList_sequence_of,
1, maxMultiBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_FreqBandIndicator_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
guint32 value;
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
maxFBI_Plus1, maxFBI2, &value, FALSE);
set_freq_band_indicator(value, actx);
return offset;
}
static int
dissect_lte_rrc_MultiBandInfo_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MultiBandInfo_v9e0, MultiBandInfo_v9e0_sequence);
return offset;
}
static int
dissect_lte_rrc_MultiBandInfoList_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MultiBandInfoList_v9e0, MultiBandInfoList_v9e0_sequence_of,
1, maxMultiBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_34(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_34, T_nonCriticalExtension_34_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType1_v9e0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType1_v9e0_IEs, SystemInformationBlockType1_v9e0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType1_v8h0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType1_v8h0_IEs, SystemInformationBlockType1_v8h0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_RLF_Report_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RLF_Report_v9e0, RLF_Report_v9e0_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_40(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_40, T_nonCriticalExtension_40_sequence);
return offset;
}
static int
dissect_lte_rrc_UEInformationResponse_v9e0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UEInformationResponse_v9e0_IEs, UEInformationResponse_v9e0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_maxMultiBands_OF_AdditionalSpectrumEmission(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_maxMultiBands_OF_AdditionalSpectrumEmission, SEQUENCE_SIZE_1_maxMultiBands_OF_AdditionalSpectrumEmission_sequence_of,
1, maxMultiBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_44(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_44, T_nonCriticalExtension_44_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType2_v9e0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType2_v9e0_IEs, SystemInformationBlockType2_v9e0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType2_v8h0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType2_v8h0_IEs, SystemInformationBlockType2_v8h0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_InterFreqCarrierFreqInfo_v8h0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_InterFreqCarrierFreqInfo_v8h0, InterFreqCarrierFreqInfo_v8h0_sequence);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_maxFreq_OF_InterFreqCarrierFreqInfo_v8h0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_maxFreq_OF_InterFreqCarrierFreqInfo_v8h0, SEQUENCE_SIZE_1_maxFreq_OF_InterFreqCarrierFreqInfo_v8h0_sequence_of,
1, maxFreq, FALSE);
return offset;
}
static int
dissect_lte_rrc_InterFreqCarrierFreqInfo_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_InterFreqCarrierFreqInfo_v9e0, InterFreqCarrierFreqInfo_v9e0_sequence);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_maxFreq_OF_InterFreqCarrierFreqInfo_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_maxFreq_OF_InterFreqCarrierFreqInfo_v9e0, SEQUENCE_SIZE_1_maxFreq_OF_InterFreqCarrierFreqInfo_v9e0_sequence_of,
1, maxFreq, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_45(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_45, T_nonCriticalExtension_45_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType5_v9e0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType5_v9e0_IEs, SystemInformationBlockType5_v9e0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType5_v8h0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType5_v8h0_IEs, SystemInformationBlockType5_v8h0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_FreqBandIndicator_UTRA_FDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 86U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_maxMultiBands_OF_FreqBandIndicator_UTRA_FDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_maxMultiBands_OF_FreqBandIndicator_UTRA_FDD, SEQUENCE_SIZE_1_maxMultiBands_OF_FreqBandIndicator_UTRA_FDD_sequence_of,
1, maxMultiBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_CarrierFreqInfoUTRA_FDD_v8h0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CarrierFreqInfoUTRA_FDD_v8h0, CarrierFreqInfoUTRA_FDD_v8h0_sequence);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_maxUTRA_FDD_Carrier_OF_CarrierFreqInfoUTRA_FDD_v8h0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_maxUTRA_FDD_Carrier_OF_CarrierFreqInfoUTRA_FDD_v8h0, SEQUENCE_SIZE_1_maxUTRA_FDD_Carrier_OF_CarrierFreqInfoUTRA_FDD_v8h0_sequence_of,
1, maxUTRA_FDD_Carrier, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_46(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_46, T_nonCriticalExtension_46_sequence);
return offset;
}
static int
dissect_lte_rrc_SystemInformationBlockType6_v8h0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SystemInformationBlockType6_v8h0_IEs, SystemInformationBlockType6_v8h0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_AccessStratumRelease(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, TRUE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_INTEGER_1_5(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
1U, 5U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_supportedROHC_Profiles(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_supportedROHC_Profiles, T_supportedROHC_Profiles_sequence);
return offset;
}
static int
dissect_lte_rrc_T_maxNumberROHC_ContextSessions(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PDCP_Parameters(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PDCP_Parameters, PDCP_Parameters_sequence);
return offset;
}
static int
dissect_lte_rrc_PhyLayerParameters(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PhyLayerParameters, PhyLayerParameters_sequence);
return offset;
}
static int
dissect_lte_rrc_SupportedBandEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SupportedBandEUTRA, SupportedBandEUTRA_sequence);
return offset;
}
static int
dissect_lte_rrc_SupportedBandListEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SupportedBandListEUTRA, SupportedBandListEUTRA_sequence_of,
1, maxBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_RF_Parameters(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RF_Parameters, RF_Parameters_sequence);
return offset;
}
static int
dissect_lte_rrc_InterFreqBandInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
meas_capabilities_item_band_mappings_t *mappings;
proto_item *it;
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_InterFreqBandInfo, InterFreqBandInfo_sequence);
mappings = private_data_meas_capabilities_item_band_mappings(actx);
if (mappings->number_of_interfreq_serving_read <= mappings->number_of_bands_set) {
guint16 serving_band = mappings->band_by_item[mappings->number_of_interfreq_serving_read];
guint16 target_band = mappings->band_by_item[mappings->number_of_interfreq_target_read++];
if (mappings->number_of_interfreq_target_read == mappings->number_of_bands_set) {
mappings->number_of_interfreq_target_read = 0;
mappings->number_of_interfreq_serving_read++;
}
it = proto_tree_add_uint(tree, hf_lte_rrc_bandEUTRA, tvb, 0, 0, serving_band);
proto_item_append_text(it, " -> band %u", target_band);
PROTO_ITEM_SET_GENERATED(it);
}
return offset;
}
static int
dissect_lte_rrc_InterFreqBandList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_InterFreqBandList, InterFreqBandList_sequence_of,
1, maxBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_InterRAT_BandInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_InterRAT_BandInfo, InterRAT_BandInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_InterRAT_BandList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_InterRAT_BandList, InterRAT_BandList_sequence_of,
1, maxBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_BandInfoEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandInfoEUTRA, BandInfoEUTRA_sequence);
return offset;
}
static int
dissect_lte_rrc_BandListEUTRA(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandListEUTRA, BandListEUTRA_sequence_of,
1, maxBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_MeasParameters(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasParameters, MeasParameters_sequence);
return offset;
}
static int
dissect_lte_rrc_T_featureGroupIndicators(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *featureGroupIndicators_tvb=NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
32, 32, FALSE, &featureGroupIndicators_tvb, NULL);
if(featureGroupIndicators_tvb){
dissect_lte_rrc_featureGroupIndicators(featureGroupIndicators_tvb, actx);
}
return offset;
}
static int
dissect_lte_rrc_SupportedBandUTRA_FDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, TRUE, 16, NULL);
return offset;
}
static int
dissect_lte_rrc_SupportedBandListUTRA_FDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SupportedBandListUTRA_FDD, SupportedBandListUTRA_FDD_sequence_of,
1, maxBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_IRAT_ParametersUTRA_FDD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IRAT_ParametersUTRA_FDD, IRAT_ParametersUTRA_FDD_sequence);
return offset;
}
static int
dissect_lte_rrc_SupportedBandUTRA_TDD128(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, TRUE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_SupportedBandListUTRA_TDD128(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SupportedBandListUTRA_TDD128, SupportedBandListUTRA_TDD128_sequence_of,
1, maxBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_IRAT_ParametersUTRA_TDD128(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IRAT_ParametersUTRA_TDD128, IRAT_ParametersUTRA_TDD128_sequence);
return offset;
}
static int
dissect_lte_rrc_SupportedBandUTRA_TDD384(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, TRUE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_SupportedBandListUTRA_TDD384(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SupportedBandListUTRA_TDD384, SupportedBandListUTRA_TDD384_sequence_of,
1, maxBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_IRAT_ParametersUTRA_TDD384(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IRAT_ParametersUTRA_TDD384, IRAT_ParametersUTRA_TDD384_sequence);
return offset;
}
static int
dissect_lte_rrc_SupportedBandUTRA_TDD768(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, TRUE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_SupportedBandListUTRA_TDD768(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SupportedBandListUTRA_TDD768, SupportedBandListUTRA_TDD768_sequence_of,
1, maxBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_IRAT_ParametersUTRA_TDD768(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IRAT_ParametersUTRA_TDD768, IRAT_ParametersUTRA_TDD768_sequence);
return offset;
}
static int
dissect_lte_rrc_SupportedBandGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
16, NULL, TRUE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_SupportedBandListGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SupportedBandListGERAN, SupportedBandListGERAN_sequence_of,
1, maxBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_IRAT_ParametersGERAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IRAT_ParametersGERAN, IRAT_ParametersGERAN_sequence);
return offset;
}
static int
dissect_lte_rrc_SupportedBandListHRPD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SupportedBandListHRPD, SupportedBandListHRPD_sequence_of,
1, maxCDMA_BandClass, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_tx_ConfigHRPD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_rx_ConfigHRPD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_IRAT_ParametersCDMA2000_HRPD(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IRAT_ParametersCDMA2000_HRPD, IRAT_ParametersCDMA2000_HRPD_sequence);
return offset;
}
static int
dissect_lte_rrc_SupportedBandList1XRTT(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SupportedBandList1XRTT, SupportedBandList1XRTT_sequence_of,
1, maxCDMA_BandClass, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_tx_Config1XRTT(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_rx_Config1XRTT(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_IRAT_ParametersCDMA2000_1XRTT(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IRAT_ParametersCDMA2000_1XRTT, IRAT_ParametersCDMA2000_1XRTT_sequence);
return offset;
}
static int
dissect_lte_rrc_T_interRAT_Parameters(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_interRAT_Parameters, T_interRAT_Parameters_sequence);
return offset;
}
static int
dissect_lte_rrc_T_enhancedDualLayerFDD_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_enhancedDualLayerTDD_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PhyLayerParameters_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PhyLayerParameters_v920, PhyLayerParameters_v920_sequence);
return offset;
}
static int
dissect_lte_rrc_T_dtm_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_e_RedirectionGERAN_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_IRAT_ParametersGERAN_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IRAT_ParametersGERAN_v920, IRAT_ParametersGERAN_v920_sequence);
return offset;
}
static int
dissect_lte_rrc_T_e_RedirectionUTRA_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_IRAT_ParametersUTRA_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IRAT_ParametersUTRA_v920, IRAT_ParametersUTRA_v920_sequence);
return offset;
}
static int
dissect_lte_rrc_T_e_CSFB_1XRTT_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_e_CSFB_ConcPS_Mob1XRTT_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_IRAT_ParametersCDMA2000_1XRTT_v920(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IRAT_ParametersCDMA2000_1XRTT_v920, IRAT_ParametersCDMA2000_1XRTT_v920_sequence);
return offset;
}
static int
dissect_lte_rrc_T_deviceType_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_intraFreqProximityIndication_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_interFreqProximityIndication_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_utran_ProximityIndication_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_CSG_ProximityIndicationParameters_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CSG_ProximityIndicationParameters_r9, CSG_ProximityIndicationParameters_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_intraFreqSI_AcquisitionForHO_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_interFreqSI_AcquisitionForHO_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_utran_SI_AcquisitionForHO_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_NeighCellSI_AcquisitionParameters_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_NeighCellSI_AcquisitionParameters_r9, NeighCellSI_AcquisitionParameters_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_rach_Report_r9_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_SON_Parameters_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SON_Parameters_r9, SON_Parameters_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_lateNonCriticalExtension_06(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string_containing_pdu_new(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, dissect_UE_EUTRA_Capability_v9a0_IEs_PDU);
return offset;
}
static int
dissect_lte_rrc_INTEGER_6_8(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
6U, 8U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_twoAntennaPortsForPUCCH_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_tm9_With_8Tx_FDD_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_pmi_Disabling_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_crossCarrierScheduling_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_simultaneousPUCCH_PUSCH_r10_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_multiClusterPUSCH_WithinCC_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_nonContiguousUL_RA_WithinCC_Info_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_NonContiguousUL_RA_WithinCC_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_NonContiguousUL_RA_WithinCC_r10, NonContiguousUL_RA_WithinCC_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_NonContiguousUL_RA_WithinCC_List_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_NonContiguousUL_RA_WithinCC_List_r10, NonContiguousUL_RA_WithinCC_List_r10_sequence_of,
1, maxBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_PhyLayerParameters_v1020(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PhyLayerParameters_v1020, PhyLayerParameters_v1020_sequence);
return offset;
}
static int
dissect_lte_rrc_CA_BandwidthClass_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
6, NULL, TRUE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_MIMO_CapabilityUL_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
2, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_CA_MIMO_ParametersUL_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CA_MIMO_ParametersUL_r10, CA_MIMO_ParametersUL_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_BandParametersUL_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandParametersUL_r10, BandParametersUL_r10_sequence_of,
1, maxBandwidthClass_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_MIMO_CapabilityDL_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
3, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_CA_MIMO_ParametersDL_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CA_MIMO_ParametersDL_r10, CA_MIMO_ParametersDL_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_BandParametersDL_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandParametersDL_r10, BandParametersDL_r10_sequence_of,
1, maxBandwidthClass_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_BandParameters_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandParameters_r10, BandParameters_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_BandCombinationParameters_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandCombinationParameters_r10, BandCombinationParameters_r10_sequence_of,
1, maxSimultaneousBands_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_SupportedBandCombination_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SupportedBandCombination_r10, SupportedBandCombination_r10_sequence_of,
1, maxBandComb_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_RF_Parameters_v1020(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RF_Parameters_v1020, RF_Parameters_v1020_sequence);
return offset;
}
static int
dissect_lte_rrc_BandCombinationListEUTRA_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandCombinationListEUTRA_r10, BandCombinationListEUTRA_r10_sequence_of,
1, maxBandComb_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_MeasParameters_v1020(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasParameters_v1020, MeasParameters_v1020_sequence);
return offset;
}
static int
dissect_lte_rrc_T_featureGroupIndRel10_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *featureGroupIndRel10_tvb=NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
32, 32, FALSE, &featureGroupIndRel10_tvb, NULL);
if(featureGroupIndRel10_tvb){
dissect_lte_rrc_featureGroupIndRel10(featureGroupIndRel10_tvb, actx);
}
return offset;
}
static int
dissect_lte_rrc_T_e_CSFB_dual_1XRTT_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_IRAT_ParametersCDMA2000_1XRTT_v1020(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IRAT_ParametersCDMA2000_1XRTT_v1020, IRAT_ParametersCDMA2000_1XRTT_v1020_sequence);
return offset;
}
static int
dissect_lte_rrc_T_loggedMeasurementsIdle_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_standaloneGNSS_Location_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_UE_BasedNetwPerfMeasParameters_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_BasedNetwPerfMeasParameters_r10, UE_BasedNetwPerfMeasParameters_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_T_e_RedirectionUTRA_TDD_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_IRAT_ParametersUTRA_TDD_v1020(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IRAT_ParametersUTRA_TDD_v1020, IRAT_ParametersUTRA_TDD_v1020_sequence);
return offset;
}
static int
dissect_lte_rrc_T_featureGroupIndRel10_v1060(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *featureGroupIndRel10_tvb=NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
32, 32, FALSE, &featureGroupIndRel10_tvb, NULL);
if(featureGroupIndRel10_tvb){
dissect_lte_rrc_featureGroupIndRel10(featureGroupIndRel10_tvb, actx);
}
return offset;
}
static int
dissect_lte_rrc_T_otdoa_UE_Assisted_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_interFreqRSTD_Measurement_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_OTDOA_PositioningCapabilities_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_OTDOA_PositioningCapabilities_r10, OTDOA_PositioningCapabilities_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_CapabilityAddXDD_Mode_v1060_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, UE_EUTRA_CapabilityAddXDD_Mode_v1060_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_CapabilityAddXDD_Mode_v1060(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_CapabilityAddXDD_Mode_v1060, UE_EUTRA_CapabilityAddXDD_Mode_v1060_sequence);
return offset;
}
static int
dissect_lte_rrc_SupportedBandwidthCombinationSet_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
1, maxBandwidthCombSet_r10, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_BandCombinationParametersExt_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandCombinationParametersExt_r10, BandCombinationParametersExt_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_SupportedBandCombinationExt_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SupportedBandCombinationExt_r10, SupportedBandCombinationExt_r10_sequence_of,
1, maxBandComb_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_RF_Parameters_v1060(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RF_Parameters_v1060, RF_Parameters_v1060_sequence);
return offset;
}
static int
dissect_lte_rrc_BandParameters_v1090(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandParameters_v1090, BandParameters_v1090_sequence);
return offset;
}
static int
dissect_lte_rrc_BandCombinationParameters_v1090(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandCombinationParameters_v1090, BandCombinationParameters_v1090_sequence_of,
1, maxSimultaneousBands_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_SupportedBandCombination_v1090(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SupportedBandCombination_v1090, SupportedBandCombination_v1090_sequence_of,
1, maxBandComb_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_RF_Parameters_v1090(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RF_Parameters_v1090, RF_Parameters_v1090_sequence);
return offset;
}
static int
dissect_lte_rrc_T_pdcp_SN_Extension_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_supportRohcContextContinue_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PDCP_Parameters_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PDCP_Parameters_v1130, PDCP_Parameters_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_T_crs_InterfHandl_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_ePDCCH_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_multiACK_CSI_Reporting_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_ss_CCH_InterfHandl_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_tdd_SpecialSubframe_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_txDiv_PUCCH1b_ChSelect_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_ul_CoMP_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PhyLayerParameters_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PhyLayerParameters_v1130, PhyLayerParameters_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_T_multipleTimingAdvance_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_simultaneousRx_Tx_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_supportedCSI_Proc_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
3, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_BandParameters_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandParameters_v1130, BandParameters_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_maxSimultaneousBands_r10_OF_BandParameters_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_maxSimultaneousBands_r10_OF_BandParameters_v1130, SEQUENCE_SIZE_1_maxSimultaneousBands_r10_OF_BandParameters_v1130_sequence_of,
1, maxSimultaneousBands_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_BandCombinationParameters_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandCombinationParameters_v1130, BandCombinationParameters_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_SupportedBandCombination_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SupportedBandCombination_v1130, SupportedBandCombination_v1130_sequence_of,
1, maxBandComb_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_RF_Parameters_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RF_Parameters_v1130, RF_Parameters_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_T_rsrqMeasWideband_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_MeasParameters_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MeasParameters_v1130, MeasParameters_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_T_cdma2000_NW_Sharing_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_IRAT_ParametersCDMA2000_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IRAT_ParametersCDMA2000_v1130, IRAT_ParametersCDMA2000_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_T_inDeviceCoexInd_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_powerPrefInd_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_ue_Rx_TxTimeDiffMeasurements_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_Other_Parameters_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_Other_Parameters_r11, Other_Parameters_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_CapabilityAddXDD_Mode_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_CapabilityAddXDD_Mode_v1130, UE_EUTRA_CapabilityAddXDD_Mode_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_T_interBandTDD_CA_WithDifferentConfig_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *inter_band_tdd_ca_tvb=NULL;
proto_tree *subtree;
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
2, 2, FALSE, &inter_band_tdd_ca_tvb, NULL);
if (inter_band_tdd_ca_tvb) {
subtree = proto_item_add_subtree(actx->created_item, ett_lte_rrc_interBandTDD_CA_WithDifferentConfig);
proto_tree_add_bits_item(subtree, hf_lte_rrc_interBandTDD_CA_WithDifferentConfig_bit1, inter_band_tdd_ca_tvb, 0, 1, ENC_BIG_ENDIAN);
proto_tree_add_bits_item(subtree, hf_lte_rrc_interBandTDD_CA_WithDifferentConfig_bit2, inter_band_tdd_ca_tvb, 1, 1, ENC_BIG_ENDIAN);
}
return offset;
}
static int
dissect_lte_rrc_PhyLayerParameters_v1170(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PhyLayerParameters_v1170, PhyLayerParameters_v1170_sequence);
return offset;
}
static int
dissect_lte_rrc_INTEGER_9_10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
9U, 10U, NULL, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_freqBandRetrieval_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_maxBands_OF_FreqBandIndicator_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_maxBands_OF_FreqBandIndicator_r11, SEQUENCE_SIZE_1_maxBands_OF_FreqBandIndicator_r11_sequence_of,
1, maxBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_supportedCSI_Proc_r11_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
3, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_BandParameters_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandParameters_r11, BandParameters_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_SEQUENCE_SIZE_1_maxSimultaneousBands_r10_OF_BandParameters_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SEQUENCE_SIZE_1_maxSimultaneousBands_r10_OF_BandParameters_r11, SEQUENCE_SIZE_1_maxSimultaneousBands_r10_OF_BandParameters_r11_sequence_of,
1, maxSimultaneousBands_r10, FALSE);
return offset;
}
static int
dissect_lte_rrc_T_multipleTimingAdvance_r11_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_simultaneousRx_Tx_r11_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_BandCombinationParameters_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_BandCombinationParameters_r11, BandCombinationParameters_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_SupportedBandCombinationAdd_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SupportedBandCombinationAdd_r11, SupportedBandCombinationAdd_r11_sequence_of,
1, maxBandComb_r11, FALSE);
return offset;
}
static int
dissect_lte_rrc_RF_Parameters_v1180(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RF_Parameters_v1180, RF_Parameters_v1180_sequence);
return offset;
}
static int
dissect_lte_rrc_T_mbms_SCell_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_mbms_NonServingCell_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_MBMS_Parameters_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_MBMS_Parameters_r11, MBMS_Parameters_r11_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_CapabilityAddXDD_Mode_v1180(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_CapabilityAddXDD_Mode_v1180, UE_EUTRA_CapabilityAddXDD_Mode_v1180_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_48(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_48, T_nonCriticalExtension_48_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_Capability_v1180_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_Capability_v1180_IEs, UE_EUTRA_Capability_v1180_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_Capability_v1170_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_Capability_v1170_IEs, UE_EUTRA_Capability_v1170_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_Capability_v1130_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_Capability_v1130_IEs, UE_EUTRA_Capability_v1130_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_Capability_v1090_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_Capability_v1090_IEs, UE_EUTRA_Capability_v1090_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_Capability_v1060_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_Capability_v1060_IEs, UE_EUTRA_Capability_v1060_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_Capability_v1020_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_Capability_v1020_IEs, UE_EUTRA_Capability_v1020_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_Capability_v940_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_Capability_v940_IEs, UE_EUTRA_Capability_v940_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_Capability_v920_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_Capability_v920_IEs, UE_EUTRA_Capability_v920_IEs_sequence);
return offset;
}
int
dissect_lte_rrc_UE_EUTRA_Capability(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_Capability, UE_EUTRA_Capability_sequence);
return offset;
}
static int
dissect_lte_rrc_T_featureGroupIndRel9Add_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *featureGroupIndRel9Add_tvb=NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
32, 32, FALSE, &featureGroupIndRel9Add_tvb, NULL);
if(featureGroupIndRel9Add_tvb){
dissect_lte_rrc_featureGroupIndRel9Add(featureGroupIndRel9Add_tvb, actx);
}
return offset;
}
static int
dissect_lte_rrc_T_featureGroupIndicators_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *featureGroupIndicators_tvb=NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
32, 32, FALSE, &featureGroupIndicators_tvb, NULL);
if(featureGroupIndicators_tvb){
dissect_lte_rrc_featureGroupIndicators(featureGroupIndicators_tvb, actx);
}
return offset;
}
static int
dissect_lte_rrc_T_featureGroupIndRel9Add_r9_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
tvbuff_t *featureGroupIndRel9Add_tvb=NULL;
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
32, 32, FALSE, &featureGroupIndRel9Add_tvb, NULL);
if(featureGroupIndRel9Add_tvb){
dissect_lte_rrc_featureGroupIndRel9Add(featureGroupIndRel9Add_tvb, actx);
}
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_CapabilityAddXDD_Mode_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_CapabilityAddXDD_Mode_r9, UE_EUTRA_CapabilityAddXDD_Mode_r9_sequence);
return offset;
}
static int
dissect_lte_rrc_T_voiceOverPS_HS_UTRA_FDD_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_voiceOverPS_HS_UTRA_TDD128_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_srvcc_FromUTRA_FDD_ToUTRA_FDD_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_srvcc_FromUTRA_FDD_ToGERAN_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_srvcc_FromUTRA_TDD128_ToUTRA_TDD128_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_srvcc_FromUTRA_TDD128_ToGERAN_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_IRAT_ParametersUTRA_v9c0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IRAT_ParametersUTRA_v9c0, IRAT_ParametersUTRA_v9c0_sequence);
return offset;
}
static int
dissect_lte_rrc_T_tm5_FDD_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_T_tm5_TDD_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_PhyLayerParameters_v9d0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_PhyLayerParameters_v9d0, PhyLayerParameters_v9d0_sequence);
return offset;
}
static int
dissect_lte_rrc_SupportedBandEUTRA_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SupportedBandEUTRA_v9e0, SupportedBandEUTRA_v9e0_sequence);
return offset;
}
static int
dissect_lte_rrc_SupportedBandListEUTRA_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_SupportedBandListEUTRA_v9e0, SupportedBandListEUTRA_v9e0_sequence_of,
1, maxBands, FALSE);
return offset;
}
static int
dissect_lte_rrc_RF_Parameters_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RF_Parameters_v9e0, RF_Parameters_v9e0_sequence);
return offset;
}
static int
dissect_lte_rrc_T_mfbi_UTRA_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
1, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_IRAT_ParametersUTRA_v9h0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_IRAT_ParametersUTRA_v9h0, IRAT_ParametersUTRA_v9h0_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_47(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_47, T_nonCriticalExtension_47_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_Capability_v10c0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_Capability_v10c0_IEs, UE_EUTRA_Capability_v10c0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_Capability_v9h0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_Capability_v9h0_IEs, UE_EUTRA_Capability_v9h0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_Capability_v9e0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_Capability_v9e0_IEs, UE_EUTRA_Capability_v9e0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_Capability_v9d0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_Capability_v9d0_IEs, UE_EUTRA_Capability_v9d0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_Capability_v9c0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_Capability_v9c0_IEs, UE_EUTRA_Capability_v9c0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_UE_EUTRA_Capability_v9a0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UE_EUTRA_Capability_v9a0_IEs, UE_EUTRA_Capability_v9a0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_handoverCommandMessage(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string_containing_pdu_new(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, dissect_DL_DCCH_Message_PDU);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_49(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_49, T_nonCriticalExtension_49_sequence);
return offset;
}
static int
dissect_lte_rrc_HandoverCommand_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_HandoverCommand_r8_IEs, HandoverCommand_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_34(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_34, T_c1_34_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_38(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_38, T_criticalExtensionsFuture_38_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_38(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_38, T_criticalExtensions_38_choice,
NULL);
return offset;
}
int
dissect_lte_rrc_HandoverCommand(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_HandoverCommand, HandoverCommand_sequence);
return offset;
}
static int
dissect_lte_rrc_T_sourceSystemInformationBlockType1Ext(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string_containing_pdu_new(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, dissect_SystemInformationBlockType1_v890_IEs_PDU);
return offset;
}
static int
dissect_lte_rrc_AS_Config_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, AS_Config_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_AS_Config_eag_2(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, AS_Config_eag_2_sequence);
return offset;
}
static int
dissect_lte_rrc_AS_Config(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AS_Config, AS_Config_sequence);
return offset;
}
static int
dissect_lte_rrc_T_ue_InactiveTime(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
64, NULL, FALSE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_CandidateCellInfo_r10_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, CandidateCellInfo_r10_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_CandidateCellInfo_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CandidateCellInfo_r10, CandidateCellInfo_r10_sequence);
return offset;
}
static int
dissect_lte_rrc_CandidateCellInfoList_r10(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_CandidateCellInfoList_r10, CandidateCellInfoList_r10_sequence_of,
1, maxFreq, FALSE);
return offset;
}
static int
dissect_lte_rrc_RRM_Config_eag_1(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence_eag(tvb, offset, actx, tree, RRM_Config_eag_1_sequence);
return offset;
}
static int
dissect_lte_rrc_RRM_Config(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_RRM_Config, RRM_Config_sequence);
return offset;
}
static int
dissect_lte_rrc_Key_eNodeB_Star(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_bit_string(tvb, offset, actx, tree, hf_index,
256, 256, FALSE, NULL, NULL);
return offset;
}
static int
dissect_lte_rrc_AdditionalReestabInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AdditionalReestabInfo, AdditionalReestabInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_AdditionalReestabInfoList(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_constrained_sequence_of(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AdditionalReestabInfoList, AdditionalReestabInfoList_sequence_of,
1, maxReestabInfo, FALSE);
return offset;
}
static int
dissect_lte_rrc_ReestablishmentInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_ReestablishmentInfo, ReestablishmentInfo_sequence);
return offset;
}
static int
dissect_lte_rrc_AS_Context(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AS_Context, AS_Context_sequence);
return offset;
}
static int
dissect_lte_rrc_T_ue_ConfigRelease_r9(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
8, NULL, TRUE, 0, NULL);
return offset;
}
static int
dissect_lte_rrc_AS_Config_v9e0(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AS_Config_v9e0, AS_Config_v9e0_sequence);
return offset;
}
static int
dissect_lte_rrc_T_idc_Indication_r11_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string_containing_pdu_new(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, dissect_InDeviceCoexIndication_r11_PDU);
return offset;
}
static int
dissect_lte_rrc_T_mbmsInterestIndication_r11(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string_containing_pdu_new(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, dissect_MBMSInterestIndication_r11_PDU);
return offset;
}
static int
dissect_lte_rrc_T_powerPrefIndication_r11_01(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string_containing_pdu_new(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, dissect_UEAssistanceInformation_r11_PDU);
return offset;
}
static int
dissect_lte_rrc_AS_Context_v1130(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_AS_Context_v1130, AS_Context_v1130_sequence);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_50(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_50, T_nonCriticalExtension_50_sequence);
return offset;
}
static int
dissect_lte_rrc_HandoverPreparationInformation_v1130_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_HandoverPreparationInformation_v1130_IEs, HandoverPreparationInformation_v1130_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_HandoverPreparationInformation_v9e0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_HandoverPreparationInformation_v9e0_IEs, HandoverPreparationInformation_v9e0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_HandoverPreparationInformation_v9d0_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_HandoverPreparationInformation_v9d0_IEs, HandoverPreparationInformation_v9d0_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_HandoverPreparationInformation_v920_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_HandoverPreparationInformation_v920_IEs, HandoverPreparationInformation_v920_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_HandoverPreparationInformation_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_HandoverPreparationInformation_r8_IEs, HandoverPreparationInformation_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_35(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_35, T_c1_35_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_39(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_39, T_criticalExtensionsFuture_39_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_39(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_39, T_criticalExtensions_39_choice,
NULL);
return offset;
}
int
dissect_lte_rrc_HandoverPreparationInformation(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_HandoverPreparationInformation, HandoverPreparationInformation_sequence);
return offset;
}
static int
dissect_lte_rrc_T_ue_RadioAccessCapabilityInfo(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_octet_string_containing_pdu_new(tvb, offset, actx, tree, hf_index,
NO_BOUND, NO_BOUND, FALSE, dissect_UECapabilityInformation_PDU);
return offset;
}
static int
dissect_lte_rrc_T_nonCriticalExtension_51(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_nonCriticalExtension_51, T_nonCriticalExtension_51_sequence);
return offset;
}
static int
dissect_lte_rrc_UERadioAccessCapabilityInformation_r8_IEs(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UERadioAccessCapabilityInformation_r8_IEs, UERadioAccessCapabilityInformation_r8_IEs_sequence);
return offset;
}
static int
dissect_lte_rrc_T_c1_36(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_c1_36, T_c1_36_choice,
NULL);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensionsFuture_40(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensionsFuture_40, T_criticalExtensionsFuture_40_sequence);
return offset;
}
static int
dissect_lte_rrc_T_criticalExtensions_40(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_T_criticalExtensions_40, T_criticalExtensions_40_choice,
NULL);
return offset;
}
int
dissect_lte_rrc_UERadioAccessCapabilityInformation(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
ett_lte_rrc_UERadioAccessCapabilityInformation, UERadioAccessCapabilityInformation_sequence);
return offset;
}
static int dissect_BCCH_BCH_Message_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_BCCH_BCH_Message(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_BCCH_BCH_Message_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_BCCH_DL_SCH_Message_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_BCCH_DL_SCH_Message(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_BCCH_DL_SCH_Message_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_MCCH_Message_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_MCCH_Message(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_MCCH_Message_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_PCCH_Message_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_PCCH_Message(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_PCCH_Message_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_DL_CCCH_Message_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_DL_CCCH_Message(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_DL_CCCH_Message_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_DL_DCCH_Message_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_DL_DCCH_Message(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_DL_DCCH_Message_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_UL_CCCH_Message_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_UL_CCCH_Message(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_UL_CCCH_Message_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_UL_DCCH_Message_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_UL_DCCH_Message(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_UL_DCCH_Message_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_UECapabilityInformation_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_UECapabilityInformation(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_UECapabilityInformation_PDU);
offset += 7; offset >>= 3;
return offset;
}
int dissect_lte_rrc_UE_EUTRA_Capability_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_UE_EUTRA_Capability(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_lte_rrc_UE_EUTRA_Capability_PDU);
offset += 7; offset >>= 3;
return offset;
}
int dissect_lte_rrc_HandoverCommand_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_HandoverCommand(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_lte_rrc_HandoverCommand_PDU);
offset += 7; offset >>= 3;
return offset;
}
int dissect_lte_rrc_HandoverPreparationInformation_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_HandoverPreparationInformation(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_lte_rrc_HandoverPreparationInformation_PDU);
offset += 7; offset >>= 3;
return offset;
}
int dissect_lte_rrc_UERadioAccessCapabilityInformation_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_UERadioAccessCapabilityInformation(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_lte_rrc_UERadioAccessCapabilityInformation_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_SystemInformationBlockType1_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_SystemInformationBlockType1(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_SystemInformationBlockType1_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_RRCConnectionRelease_v9e0_IEs_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_RRCConnectionRelease_v9e0_IEs(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_RRCConnectionRelease_v9e0_IEs_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_SystemInformationBlockType1_v8h0_IEs_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_SystemInformationBlockType1_v8h0_IEs(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_SystemInformationBlockType1_v8h0_IEs_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_UEInformationResponse_v9e0_IEs_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_UEInformationResponse_v9e0_IEs(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_UEInformationResponse_v9e0_IEs_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_SystemInformationBlockType2_v8h0_IEs_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_SystemInformationBlockType2_v8h0_IEs(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_SystemInformationBlockType2_v8h0_IEs_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_SystemInformationBlockType5_v8h0_IEs_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_SystemInformationBlockType5_v8h0_IEs(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_SystemInformationBlockType5_v8h0_IEs_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_SystemInformationBlockType6_v8h0_IEs_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_SystemInformationBlockType6_v8h0_IEs(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_SystemInformationBlockType6_v8h0_IEs_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_UE_EUTRA_Capability_v9a0_IEs_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_UE_EUTRA_Capability_v9a0_IEs(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_UE_EUTRA_Capability_v9a0_IEs_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_SystemInformationBlockType1_v890_IEs_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_SystemInformationBlockType1_v890_IEs(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_SystemInformationBlockType1_v890_IEs_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_InDeviceCoexIndication_r11_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_InDeviceCoexIndication_r11(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_InDeviceCoexIndication_r11_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_MBMSInterestIndication_r11_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_MBMSInterestIndication_r11(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_MBMSInterestIndication_r11_PDU);
offset += 7; offset >>= 3;
return offset;
}
static int dissect_UEAssistanceInformation_r11_PDU(tvbuff_t *tvb _U_, packet_info *pinfo _U_, proto_tree *tree _U_, void *data _U_) {
int offset = 0;
asn1_ctx_t asn1_ctx;
asn1_ctx_init(&asn1_ctx, ASN1_ENC_PER, FALSE, pinfo);
offset = dissect_lte_rrc_UEAssistanceInformation_r11(tvb, offset, &asn1_ctx, tree, hf_lte_rrc_UEAssistanceInformation_r11_PDU);
offset += 7; offset >>= 3;
return offset;
}
static void
dissect_lte_rrc_DL_CCCH(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
proto_item *ti;
proto_tree *lte_rrc_tree;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "LTE RRC DL_CCCH");
col_clear(pinfo->cinfo, COL_INFO);
if (tree) {
ti = proto_tree_add_item(tree, proto_lte_rrc, tvb, 0, -1, ENC_NA);
lte_rrc_tree = proto_item_add_subtree(ti, ett_lte_rrc);
dissect_DL_CCCH_Message_PDU(tvb, pinfo, lte_rrc_tree, NULL);
}
}
static void
dissect_lte_rrc_DL_DCCH(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
proto_item *ti;
proto_tree *lte_rrc_tree;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "LTE RRC DL_DCCH");
col_clear(pinfo->cinfo, COL_INFO);
ti = proto_tree_add_item(tree, proto_lte_rrc, tvb, 0, -1, ENC_NA);
lte_rrc_tree = proto_item_add_subtree(ti, ett_lte_rrc);
dissect_DL_DCCH_Message_PDU(tvb, pinfo, lte_rrc_tree, NULL);
}
static void
dissect_lte_rrc_UL_CCCH(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
proto_item *ti;
proto_tree *lte_rrc_tree;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "LTE RRC UL_CCCH");
col_clear(pinfo->cinfo, COL_INFO);
if (tree) {
ti = proto_tree_add_item(tree, proto_lte_rrc, tvb, 0, -1, ENC_NA);
lte_rrc_tree = proto_item_add_subtree(ti, ett_lte_rrc);
dissect_UL_CCCH_Message_PDU(tvb, pinfo, lte_rrc_tree, NULL);
}
}
static void
dissect_lte_rrc_UL_DCCH(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
proto_item *ti;
proto_tree *lte_rrc_tree;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "LTE RRC UL_DCCH");
col_clear(pinfo->cinfo, COL_INFO);
if (tree) {
ti = proto_tree_add_item(tree, proto_lte_rrc, tvb, 0, -1, ENC_NA);
lte_rrc_tree = proto_item_add_subtree(ti, ett_lte_rrc);
dissect_UL_DCCH_Message_PDU(tvb, pinfo, lte_rrc_tree, NULL);
}
}
static void
dissect_lte_rrc_BCCH_BCH(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
proto_item *ti;
proto_tree *lte_rrc_tree;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "LTE RRC BCCH_BCH");
col_clear(pinfo->cinfo, COL_INFO);
if (tree) {
ti = proto_tree_add_item(tree, proto_lte_rrc, tvb, 0, -1, ENC_NA);
lte_rrc_tree = proto_item_add_subtree(ti, ett_lte_rrc);
dissect_BCCH_BCH_Message_PDU(tvb, pinfo, lte_rrc_tree, NULL);
}
}
static void
dissect_lte_rrc_BCCH_DL_SCH(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
proto_item *ti;
proto_tree *lte_rrc_tree;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "LTE RRC DL_SCH");
col_clear(pinfo->cinfo, COL_INFO);
ti = proto_tree_add_item(tree, proto_lte_rrc, tvb, 0, -1, ENC_NA);
lte_rrc_tree = proto_item_add_subtree(ti, ett_lte_rrc);
dissect_BCCH_DL_SCH_Message_PDU(tvb, pinfo, lte_rrc_tree, NULL);
}
static void
dissect_lte_rrc_PCCH(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
proto_item *ti;
proto_tree *lte_rrc_tree;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "LTE RRC PCCH");
col_clear(pinfo->cinfo, COL_INFO);
ti = proto_tree_add_item(tree, proto_lte_rrc, tvb, 0, -1, ENC_NA);
lte_rrc_tree = proto_item_add_subtree(ti, ett_lte_rrc);
dissect_PCCH_Message_PDU(tvb, pinfo, lte_rrc_tree, NULL);
}
static void
dissect_lte_rrc_MCCH(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
proto_item *ti;
proto_tree *lte_rrc_tree;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "LTE RRC MCCH");
col_clear(pinfo->cinfo, COL_INFO);
if (tree) {
ti = proto_tree_add_item(tree, proto_lte_rrc, tvb, 0, -1, ENC_NA);
lte_rrc_tree = proto_item_add_subtree(ti, ett_lte_rrc);
dissect_MCCH_Message_PDU(tvb, pinfo, lte_rrc_tree, NULL);
}
}
static void
dissect_lte_rrc_Handover_Preparation_Info(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
proto_item *ti;
proto_tree *lte_rrc_tree;
col_set_str(pinfo->cinfo, COL_PROTOCOL, "LTE_HO_Prep_Info");
col_clear(pinfo->cinfo, COL_INFO);
col_set_str(pinfo->cinfo, COL_INFO, "HandoverPreparationInformation");
col_set_writable(pinfo->cinfo, FALSE);
if (tree) {
ti = proto_tree_add_item(tree, proto_lte_rrc, tvb, 0, -1, ENC_NA);
lte_rrc_tree = proto_item_add_subtree(ti, ett_lte_rrc);
dissect_lte_rrc_HandoverPreparationInformation_PDU(tvb, pinfo, lte_rrc_tree, NULL);
}
col_set_writable(pinfo->cinfo, TRUE);
}
static void
lte_rrc_init_protocol(void)
{
if (lte_rrc_etws_cmas_dcs_hash) {
g_hash_table_destroy(lte_rrc_etws_cmas_dcs_hash);
}
if (lte_rrc_system_info_value_changed_hash) {
g_hash_table_destroy(lte_rrc_system_info_value_changed_hash);
}
lte_rrc_etws_cmas_dcs_hash = g_hash_table_new(g_direct_hash, g_direct_equal);
lte_rrc_system_info_value_changed_hash = g_hash_table_new(g_direct_hash, g_direct_equal);
}
void proto_register_lte_rrc(void) {
static hf_register_info hf[] = {
#line 1 "../../asn1/lte-rrc/packet-lte-rrc-hfarr.c"
{ &hf_lte_rrc_BCCH_BCH_Message_PDU,
{ "BCCH-BCH-Message", "lte-rrc.BCCH_BCH_Message_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_BCCH_DL_SCH_Message_PDU,
{ "BCCH-DL-SCH-Message", "lte-rrc.BCCH_DL_SCH_Message_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MCCH_Message_PDU,
{ "MCCH-Message", "lte-rrc.MCCH_Message_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_PCCH_Message_PDU,
{ "PCCH-Message", "lte-rrc.PCCH_Message_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_DL_CCCH_Message_PDU,
{ "DL-CCCH-Message", "lte-rrc.DL_CCCH_Message_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_DL_DCCH_Message_PDU,
{ "DL-DCCH-Message", "lte-rrc.DL_DCCH_Message_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_UL_CCCH_Message_PDU,
{ "UL-CCCH-Message", "lte-rrc.UL_CCCH_Message_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_UL_DCCH_Message_PDU,
{ "UL-DCCH-Message", "lte-rrc.UL_DCCH_Message_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_UECapabilityInformation_PDU,
{ "UECapabilityInformation", "lte-rrc.UECapabilityInformation_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_lte_rrc_UE_EUTRA_Capability_PDU,
{ "UE-EUTRA-Capability", "lte-rrc.UE_EUTRA_Capability_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_lte_rrc_HandoverCommand_PDU,
{ "HandoverCommand", "lte-rrc.HandoverCommand_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_lte_rrc_HandoverPreparationInformation_PDU,
{ "HandoverPreparationInformation", "lte-rrc.HandoverPreparationInformation_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_lte_rrc_UERadioAccessCapabilityInformation_PDU,
{ "UERadioAccessCapabilityInformation", "lte-rrc.UERadioAccessCapabilityInformation_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SystemInformationBlockType1_PDU,
{ "SystemInformationBlockType1", "lte-rrc.SystemInformationBlockType1_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_RRCConnectionRelease_v9e0_IEs_PDU,
{ "RRCConnectionRelease-v9e0-IEs", "lte-rrc.RRCConnectionRelease_v9e0_IEs_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SystemInformationBlockType1_v8h0_IEs_PDU,
{ "SystemInformationBlockType1-v8h0-IEs", "lte-rrc.SystemInformationBlockType1_v8h0_IEs_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_UEInformationResponse_v9e0_IEs_PDU,
{ "UEInformationResponse-v9e0-IEs", "lte-rrc.UEInformationResponse_v9e0_IEs_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SystemInformationBlockType2_v8h0_IEs_PDU,
{ "SystemInformationBlockType2-v8h0-IEs", "lte-rrc.SystemInformationBlockType2_v8h0_IEs_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SystemInformationBlockType5_v8h0_IEs_PDU,
{ "SystemInformationBlockType5-v8h0-IEs", "lte-rrc.SystemInformationBlockType5_v8h0_IEs_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SystemInformationBlockType6_v8h0_IEs_PDU,
{ "SystemInformationBlockType6-v8h0-IEs", "lte-rrc.SystemInformationBlockType6_v8h0_IEs_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_UE_EUTRA_Capability_v9a0_IEs_PDU,
{ "UE-EUTRA-Capability-v9a0-IEs", "lte-rrc.UE_EUTRA_Capability_v9a0_IEs_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SystemInformationBlockType1_v890_IEs_PDU,
{ "SystemInformationBlockType1-v890-IEs", "lte-rrc.SystemInformationBlockType1_v890_IEs_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_InDeviceCoexIndication_r11_PDU,
{ "InDeviceCoexIndication-r11", "lte-rrc.InDeviceCoexIndication_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MBMSInterestIndication_r11_PDU,
{ "MBMSInterestIndication-r11", "lte-rrc.MBMSInterestIndication_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_UEAssistanceInformation_r11_PDU,
{ "UEAssistanceInformation-r11", "lte-rrc.UEAssistanceInformation_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_message,
{ "message", "lte-rrc.message_element",
FT_NONE, BASE_NONE, NULL, 0,
"BCCH_BCH_MessageType", HFILL }},
{ &hf_lte_rrc_message_01,
{ "message", "lte-rrc.message",
FT_UINT32, BASE_DEC, VALS(lte_rrc_BCCH_DL_SCH_MessageType_vals), 0,
"BCCH_DL_SCH_MessageType", HFILL }},
{ &hf_lte_rrc_c1,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_systemInformation,
{ "systemInformation", "lte-rrc.systemInformation_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_systemInformationBlockType1,
{ "systemInformationBlockType1", "lte-rrc.systemInformationBlockType1_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_messageClassExtension,
{ "messageClassExtension", "lte-rrc.messageClassExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_message_02,
{ "message", "lte-rrc.message",
FT_UINT32, BASE_DEC, VALS(lte_rrc_MCCH_MessageType_vals), 0,
"MCCH_MessageType", HFILL }},
{ &hf_lte_rrc_c1_01,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_01_vals), 0,
"T_c1_01", HFILL }},
{ &hf_lte_rrc_mbsfnAreaConfiguration_r9,
{ "mbsfnAreaConfiguration-r9", "lte-rrc.mbsfnAreaConfiguration_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_later,
{ "later", "lte-rrc.later",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_later_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_c2,
{ "c2", "lte-rrc.c2",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c2_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_mbmsCountingRequest_r10,
{ "mbmsCountingRequest-r10", "lte-rrc.mbmsCountingRequest_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_messageClassExtension_01,
{ "messageClassExtension", "lte-rrc.messageClassExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_messageClassExtension_01", HFILL }},
{ &hf_lte_rrc_message_03,
{ "message", "lte-rrc.message",
FT_UINT32, BASE_DEC, VALS(lte_rrc_PCCH_MessageType_vals), 0,
"PCCH_MessageType", HFILL }},
{ &hf_lte_rrc_c1_02,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_02_vals), 0,
"T_c1_02", HFILL }},
{ &hf_lte_rrc_paging,
{ "paging", "lte-rrc.paging_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_messageClassExtension_02,
{ "messageClassExtension", "lte-rrc.messageClassExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_messageClassExtension_02", HFILL }},
{ &hf_lte_rrc_message_04,
{ "message", "lte-rrc.message",
FT_UINT32, BASE_DEC, VALS(lte_rrc_DL_CCCH_MessageType_vals), 0,
"DL_CCCH_MessageType", HFILL }},
{ &hf_lte_rrc_c1_03,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_03_vals), 0,
"T_c1_03", HFILL }},
{ &hf_lte_rrc_rrcConnectionReestablishment,
{ "rrcConnectionReestablishment", "lte-rrc.rrcConnectionReestablishment_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rrcConnectionReestablishmentReject,
{ "rrcConnectionReestablishmentReject", "lte-rrc.rrcConnectionReestablishmentReject_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rrcConnectionReject,
{ "rrcConnectionReject", "lte-rrc.rrcConnectionReject_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rrcConnectionSetup,
{ "rrcConnectionSetup", "lte-rrc.rrcConnectionSetup_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_messageClassExtension_03,
{ "messageClassExtension", "lte-rrc.messageClassExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_messageClassExtension_03", HFILL }},
{ &hf_lte_rrc_message_05,
{ "message", "lte-rrc.message",
FT_UINT32, BASE_DEC, VALS(lte_rrc_DL_DCCH_MessageType_vals), 0,
"DL_DCCH_MessageType", HFILL }},
{ &hf_lte_rrc_c1_04,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_04_vals), 0,
"T_c1_04", HFILL }},
{ &hf_lte_rrc_csfbParametersResponseCDMA2000,
{ "csfbParametersResponseCDMA2000", "lte-rrc.csfbParametersResponseCDMA2000_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_dlInformationTransfer,
{ "dlInformationTransfer", "lte-rrc.dlInformationTransfer_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_handoverFromEUTRAPreparationRequest,
{ "handoverFromEUTRAPreparationRequest", "lte-rrc.handoverFromEUTRAPreparationRequest_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mobilityFromEUTRACommand,
{ "mobilityFromEUTRACommand", "lte-rrc.mobilityFromEUTRACommand_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rrcConnectionReconfiguration,
{ "rrcConnectionReconfiguration", "lte-rrc.rrcConnectionReconfiguration_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rrcConnectionRelease,
{ "rrcConnectionRelease", "lte-rrc.rrcConnectionRelease_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_securityModeCommand,
{ "securityModeCommand", "lte-rrc.securityModeCommand_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ueCapabilityEnquiry,
{ "ueCapabilityEnquiry", "lte-rrc.ueCapabilityEnquiry_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_counterCheck,
{ "counterCheck", "lte-rrc.counterCheck_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ueInformationRequest_r9,
{ "ueInformationRequest-r9", "lte-rrc.ueInformationRequest_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_loggedMeasurementConfiguration_r10,
{ "loggedMeasurementConfiguration-r10", "lte-rrc.loggedMeasurementConfiguration_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rnReconfiguration_r10,
{ "rnReconfiguration-r10", "lte-rrc.rnReconfiguration_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_spare4,
{ "spare4", "lte-rrc.spare4_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_spare3,
{ "spare3", "lte-rrc.spare3_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_spare2,
{ "spare2", "lte-rrc.spare2_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_spare1,
{ "spare1", "lte-rrc.spare1_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_messageClassExtension_04,
{ "messageClassExtension", "lte-rrc.messageClassExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_messageClassExtension_04", HFILL }},
{ &hf_lte_rrc_message_06,
{ "message", "lte-rrc.message",
FT_UINT32, BASE_DEC, VALS(lte_rrc_UL_CCCH_MessageType_vals), 0,
"UL_CCCH_MessageType", HFILL }},
{ &hf_lte_rrc_c1_05,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_05_vals), 0,
"T_c1_05", HFILL }},
{ &hf_lte_rrc_rrcConnectionReestablishmentRequest,
{ "rrcConnectionReestablishmentRequest", "lte-rrc.rrcConnectionReestablishmentRequest_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rrcConnectionRequest,
{ "rrcConnectionRequest", "lte-rrc.rrcConnectionRequest_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_messageClassExtension_05,
{ "messageClassExtension", "lte-rrc.messageClassExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_messageClassExtension_05", HFILL }},
{ &hf_lte_rrc_message_07,
{ "message", "lte-rrc.message",
FT_UINT32, BASE_DEC, VALS(lte_rrc_UL_DCCH_MessageType_vals), 0,
"UL_DCCH_MessageType", HFILL }},
{ &hf_lte_rrc_c1_06,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_06_vals), 0,
"T_c1_06", HFILL }},
{ &hf_lte_rrc_csfbParametersRequestCDMA2000,
{ "csfbParametersRequestCDMA2000", "lte-rrc.csfbParametersRequestCDMA2000_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measurementReport,
{ "measurementReport", "lte-rrc.measurementReport_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rrcConnectionReconfigurationComplete,
{ "rrcConnectionReconfigurationComplete", "lte-rrc.rrcConnectionReconfigurationComplete_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rrcConnectionReestablishmentComplete,
{ "rrcConnectionReestablishmentComplete", "lte-rrc.rrcConnectionReestablishmentComplete_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rrcConnectionSetupComplete,
{ "rrcConnectionSetupComplete", "lte-rrc.rrcConnectionSetupComplete_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_securityModeComplete,
{ "securityModeComplete", "lte-rrc.securityModeComplete_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_securityModeFailure,
{ "securityModeFailure", "lte-rrc.securityModeFailure_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ueCapabilityInformation,
{ "ueCapabilityInformation", "lte-rrc.ueCapabilityInformation_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ulHandoverPreparationTransfer,
{ "ulHandoverPreparationTransfer", "lte-rrc.ulHandoverPreparationTransfer_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ulInformationTransfer,
{ "ulInformationTransfer", "lte-rrc.ulInformationTransfer_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_counterCheckResponse,
{ "counterCheckResponse", "lte-rrc.counterCheckResponse_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ueInformationResponse_r9,
{ "ueInformationResponse-r9", "lte-rrc.ueInformationResponse_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_proximityIndication_r9,
{ "proximityIndication-r9", "lte-rrc.proximityIndication_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rnReconfigurationComplete_r10,
{ "rnReconfigurationComplete-r10", "lte-rrc.rnReconfigurationComplete_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mbmsCountingResponse_r10,
{ "mbmsCountingResponse-r10", "lte-rrc.mbmsCountingResponse_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_interFreqRSTDMeasurementIndication_r10,
{ "interFreqRSTDMeasurementIndication-r10", "lte-rrc.interFreqRSTDMeasurementIndication_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_messageClassExtension_06,
{ "messageClassExtension", "lte-rrc.messageClassExtension",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_messageClassExtension_06_vals), 0,
"T_messageClassExtension_06", HFILL }},
{ &hf_lte_rrc_c2_01,
{ "c2", "lte-rrc.c2",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c2_01_vals), 0,
"T_c2_01", HFILL }},
{ &hf_lte_rrc_ueAssistanceInformation_r11,
{ "ueAssistanceInformation-r11", "lte-rrc.ueAssistanceInformation_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_inDeviceCoexIndication_r11,
{ "inDeviceCoexIndication-r11", "lte-rrc.inDeviceCoexIndication_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mbmsInterestIndication_r11,
{ "mbmsInterestIndication-r11", "lte-rrc.mbmsInterestIndication_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_spare13,
{ "spare13", "lte-rrc.spare13_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_spare12,
{ "spare12", "lte-rrc.spare12_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_spare11,
{ "spare11", "lte-rrc.spare11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_spare10,
{ "spare10", "lte-rrc.spare10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_spare9,
{ "spare9", "lte-rrc.spare9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_spare8,
{ "spare8", "lte-rrc.spare8_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_spare7,
{ "spare7", "lte-rrc.spare7_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_spare6,
{ "spare6", "lte-rrc.spare6_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_spare5,
{ "spare5", "lte-rrc.spare5_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_messageClassExtensionFuture_r11,
{ "messageClassExtensionFuture-r11", "lte-rrc.messageClassExtensionFuture_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rrc_TransactionIdentifier,
{ "rrc-TransactionIdentifier", "lte-rrc.rrc_TransactionIdentifier",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_criticalExtensions,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_c1_07,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_07_vals), 0,
"T_c1_07", HFILL }},
{ &hf_lte_rrc_counterCheck_r8,
{ "counterCheck-r8", "lte-rrc.counterCheck_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"CounterCheck_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_drb_CountMSB_InfoList,
{ "drb-CountMSB-InfoList", "lte-rrc.drb_CountMSB_InfoList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"CounterCheck_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_lateNonCriticalExtension,
{ "lateNonCriticalExtension", "lte-rrc.lateNonCriticalExtension",
FT_BYTES, BASE_NONE, NULL, 0,
"OCTET_STRING", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_01,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_DRB_CountMSB_InfoList_item,
{ "DRB-CountMSB-Info", "lte-rrc.DRB_CountMSB_Info_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_drb_Identity,
{ "drb-Identity", "lte-rrc.drb_Identity",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_countMSB_Uplink,
{ "countMSB-Uplink", "lte-rrc.countMSB_Uplink",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_33554431", HFILL }},
{ &hf_lte_rrc_countMSB_Downlink,
{ "countMSB-Downlink", "lte-rrc.countMSB_Downlink",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_33554431", HFILL }},
{ &hf_lte_rrc_criticalExtensions_01,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_01_vals), 0,
"T_criticalExtensions_01", HFILL }},
{ &hf_lte_rrc_counterCheckResponse_r8,
{ "counterCheckResponse-r8", "lte-rrc.counterCheckResponse_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"CounterCheckResponse_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_01,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_01", HFILL }},
{ &hf_lte_rrc_drb_CountInfoList,
{ "drb-CountInfoList", "lte-rrc.drb_CountInfoList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_02,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"CounterCheckResponse_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_03,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_01", HFILL }},
{ &hf_lte_rrc_DRB_CountInfoList_item,
{ "DRB-CountInfo", "lte-rrc.DRB_CountInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_count_Uplink,
{ "count-Uplink", "lte-rrc.count_Uplink",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_4294967295", HFILL }},
{ &hf_lte_rrc_count_Downlink,
{ "count-Downlink", "lte-rrc.count_Downlink",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_4294967295", HFILL }},
{ &hf_lte_rrc_criticalExtensions_02,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_02_vals), 0,
"T_criticalExtensions_02", HFILL }},
{ &hf_lte_rrc_csfbParametersRequestCDMA2000_r8,
{ "csfbParametersRequestCDMA2000-r8", "lte-rrc.csfbParametersRequestCDMA2000_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"CSFBParametersRequestCDMA2000_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_02,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_02", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_04,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"CSFBParametersRequestCDMA2000_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_05,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_02", HFILL }},
{ &hf_lte_rrc_criticalExtensions_03,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_03_vals), 0,
"T_criticalExtensions_03", HFILL }},
{ &hf_lte_rrc_csfbParametersResponseCDMA2000_r8,
{ "csfbParametersResponseCDMA2000-r8", "lte-rrc.csfbParametersResponseCDMA2000_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"CSFBParametersResponseCDMA2000_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_03,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_03", HFILL }},
{ &hf_lte_rrc_rand,
{ "rand", "lte-rrc.rand",
FT_BYTES, BASE_NONE, NULL, 0,
"RAND_CDMA2000", HFILL }},
{ &hf_lte_rrc_mobilityParameters,
{ "mobilityParameters", "lte-rrc.mobilityParameters",
FT_BYTES, BASE_NONE, NULL, 0,
"MobilityParametersCDMA2000", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_06,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"CSFBParametersResponseCDMA2000_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_07,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_03", HFILL }},
{ &hf_lte_rrc_criticalExtensions_04,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_04_vals), 0,
"T_criticalExtensions_04", HFILL }},
{ &hf_lte_rrc_c1_08,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_08_vals), 0,
"T_c1_08", HFILL }},
{ &hf_lte_rrc_dlInformationTransfer_r8,
{ "dlInformationTransfer-r8", "lte-rrc.dlInformationTransfer_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"DLInformationTransfer_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_04,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_04", HFILL }},
{ &hf_lte_rrc_dedicatedInfoType,
{ "dedicatedInfoType", "lte-rrc.dedicatedInfoType",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_dedicatedInfoType_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_dedicatedInfoNAS,
{ "dedicatedInfoNAS", "lte-rrc.dedicatedInfoNAS",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_dedicatedInfoCDMA2000_1XRTT,
{ "dedicatedInfoCDMA2000-1XRTT", "lte-rrc.dedicatedInfoCDMA2000_1XRTT",
FT_BYTES, BASE_NONE, NULL, 0,
"DedicatedInfoCDMA2000", HFILL }},
{ &hf_lte_rrc_dedicatedInfoCDMA2000_HRPD,
{ "dedicatedInfoCDMA2000-HRPD", "lte-rrc.dedicatedInfoCDMA2000_HRPD",
FT_BYTES, BASE_NONE, NULL, 0,
"DedicatedInfoCDMA2000", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_08,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"DLInformationTransfer_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_09,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_04", HFILL }},
{ &hf_lte_rrc_criticalExtensions_05,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_05_vals), 0,
"T_criticalExtensions_05", HFILL }},
{ &hf_lte_rrc_c1_09,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_09_vals), 0,
"T_c1_09", HFILL }},
{ &hf_lte_rrc_handoverFromEUTRAPreparationRequest_r8,
{ "handoverFromEUTRAPreparationRequest-r8", "lte-rrc.handoverFromEUTRAPreparationRequest_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"HandoverFromEUTRAPreparationRequest_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_05,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_05", HFILL }},
{ &hf_lte_rrc_cdma2000_Type,
{ "cdma2000-Type", "lte-rrc.cdma2000_Type",
FT_UINT32, BASE_DEC, VALS(lte_rrc_CDMA2000_Type_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_10,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"HandoverFromEUTRAPreparationRequest_v890_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_11,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"HandoverFromEUTRAPreparationRequest_v920_IEs", HFILL }},
{ &hf_lte_rrc_concurrPrepCDMA2000_HRPD_r9,
{ "concurrPrepCDMA2000-HRPD-r9", "lte-rrc.concurrPrepCDMA2000_HRPD_r9",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_12,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"HandoverFromEUTRAPreparationRequest_v1020_IEs", HFILL }},
{ &hf_lte_rrc_dualRxTxRedirectIndicator_r10,
{ "dualRxTxRedirectIndicator-r10", "lte-rrc.dualRxTxRedirectIndicator_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_dualRxTxRedirectIndicator_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_redirectCarrierCDMA2000_1XRTT_r10,
{ "redirectCarrierCDMA2000-1XRTT-r10", "lte-rrc.redirectCarrierCDMA2000_1XRTT_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"CarrierFreqCDMA2000", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_13,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_05", HFILL }},
{ &hf_lte_rrc_criticalExtensions_06,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_06_vals), 0,
"T_criticalExtensions_06", HFILL }},
{ &hf_lte_rrc_c1_10,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_10_vals), 0,
"T_c1_10", HFILL }},
{ &hf_lte_rrc_inDeviceCoexIndication_r11_01,
{ "inDeviceCoexIndication-r11", "lte-rrc.inDeviceCoexIndication_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"InDeviceCoexIndication_r11_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_06,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_06", HFILL }},
{ &hf_lte_rrc_affectedCarrierFreqList_r11,
{ "affectedCarrierFreqList-r11", "lte-rrc.affectedCarrierFreqList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_tdm_AssistanceInfo_r11,
{ "tdm-AssistanceInfo-r11", "lte-rrc.tdm_AssistanceInfo_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_TDM_AssistanceInfo_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_14,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_06", HFILL }},
{ &hf_lte_rrc_AffectedCarrierFreqList_r11_item,
{ "AffectedCarrierFreq-r11", "lte-rrc.AffectedCarrierFreq_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_carrierFreq_r11,
{ "carrierFreq-r11", "lte-rrc.carrierFreq_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasObjectId", HFILL }},
{ &hf_lte_rrc_interferenceDirection_r11,
{ "interferenceDirection-r11", "lte-rrc.interferenceDirection_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_interferenceDirection_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_drx_AssistanceInfo_r11,
{ "drx-AssistanceInfo-r11", "lte-rrc.drx_AssistanceInfo_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_drx_CycleLength_r11,
{ "drx-CycleLength-r11", "lte-rrc.drx_CycleLength_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_drx_CycleLength_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_drx_Offset_r11,
{ "drx-Offset-r11", "lte-rrc.drx_Offset_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_255", HFILL }},
{ &hf_lte_rrc_drx_ActiveTime_r11,
{ "drx-ActiveTime-r11", "lte-rrc.drx_ActiveTime_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_drx_ActiveTime_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_idc_SubframePatternList_r11,
{ "idc-SubframePatternList-r11", "lte-rrc.idc_SubframePatternList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_IDC_SubframePatternList_r11_item,
{ "IDC-SubframePattern-r11", "lte-rrc.IDC_SubframePattern_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_IDC_SubframePattern_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_subframePatternFDD_r11,
{ "subframePatternFDD-r11", "lte-rrc.subframePatternFDD_r11",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_4", HFILL }},
{ &hf_lte_rrc_subframePatternTDD_r11,
{ "subframePatternTDD-r11", "lte-rrc.subframePatternTDD_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_subframePatternTDD_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_subframeConfig0_r11,
{ "subframeConfig0-r11", "lte-rrc.subframeConfig0_r11",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_70", HFILL }},
{ &hf_lte_rrc_subframeConfig1_5_r11,
{ "subframeConfig1-5-r11", "lte-rrc.subframeConfig1_5_r11",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_10", HFILL }},
{ &hf_lte_rrc_subframeConfig6_r11,
{ "subframeConfig6-r11", "lte-rrc.subframeConfig6_r11",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_60", HFILL }},
{ &hf_lte_rrc_criticalExtensions_07,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_07_vals), 0,
"T_criticalExtensions_07", HFILL }},
{ &hf_lte_rrc_c1_11,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_11_vals), 0,
"T_c1_11", HFILL }},
{ &hf_lte_rrc_interFreqRSTDMeasurementIndication_r10_01,
{ "interFreqRSTDMeasurementIndication-r10", "lte-rrc.interFreqRSTDMeasurementIndication_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"InterFreqRSTDMeasurementIndication_r10_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_07,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_07", HFILL }},
{ &hf_lte_rrc_rstd_InterFreqIndication_r10,
{ "rstd-InterFreqIndication-r10", "lte-rrc.rstd_InterFreqIndication_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_rstd_InterFreqIndication_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_start,
{ "start", "lte-rrc.start_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rstd_InterFreqInfoList_r10,
{ "rstd-InterFreqInfoList-r10", "lte-rrc.rstd_InterFreqInfoList_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_stop,
{ "stop", "lte-rrc.stop_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_15,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_07", HFILL }},
{ &hf_lte_rrc_RSTD_InterFreqInfoList_r10_item,
{ "RSTD-InterFreqInfo-r10", "lte-rrc.RSTD_InterFreqInfo_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_carrierFreq_r10,
{ "carrierFreq-r10", "lte-rrc.carrierFreq_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA", HFILL }},
{ &hf_lte_rrc_measPRS_Offset_r10,
{ "measPRS-Offset-r10", "lte-rrc.measPRS_Offset_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_39", HFILL }},
{ &hf_lte_rrc_carrierFreq_v1090,
{ "carrierFreq-v1090", "lte-rrc.carrierFreq_v1090",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA_v9e0", HFILL }},
{ &hf_lte_rrc_criticalExtensions_08,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_08_vals), 0,
"T_criticalExtensions_08", HFILL }},
{ &hf_lte_rrc_c1_12,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_12_vals), 0,
"T_c1_12", HFILL }},
{ &hf_lte_rrc_loggedMeasurementConfiguration_r10_01,
{ "loggedMeasurementConfiguration-r10", "lte-rrc.loggedMeasurementConfiguration_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"LoggedMeasurementConfiguration_r10_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_08,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_08", HFILL }},
{ &hf_lte_rrc_traceReference_r10,
{ "traceReference-r10", "lte-rrc.traceReference_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_traceRecordingSessionRef_r10,
{ "traceRecordingSessionRef-r10", "lte-rrc.traceRecordingSessionRef_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"OCTET_STRING_SIZE_2", HFILL }},
{ &hf_lte_rrc_tce_Id_r10,
{ "tce-Id-r10", "lte-rrc.tce_Id_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"OCTET_STRING_SIZE_1", HFILL }},
{ &hf_lte_rrc_absoluteTimeInfo_r10,
{ "absoluteTimeInfo-r10", "lte-rrc.absoluteTimeInfo_r10",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_areaConfiguration_r10,
{ "areaConfiguration-r10", "lte-rrc.areaConfiguration_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_AreaConfiguration_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_loggingDuration_r10,
{ "loggingDuration-r10", "lte-rrc.loggingDuration_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_LoggingDuration_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_loggingInterval_r10,
{ "loggingInterval-r10", "lte-rrc.loggingInterval_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_LoggingInterval_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_16,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"LoggedMeasurementConfiguration_v1080_IEs", HFILL }},
{ &hf_lte_rrc_lateNonCriticalExtension_r10,
{ "lateNonCriticalExtension-r10", "lte-rrc.lateNonCriticalExtension_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"OCTET_STRING", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_17,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"LoggedMeasurementConfiguration_v1130_IEs", HFILL }},
{ &hf_lte_rrc_plmn_IdentityList_r11,
{ "plmn-IdentityList-r11", "lte-rrc.plmn_IdentityList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"PLMN_IdentityList3_r11", HFILL }},
{ &hf_lte_rrc_areaConfiguration_v1130,
{ "areaConfiguration-v1130", "lte-rrc.areaConfiguration_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_18,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_08", HFILL }},
{ &hf_lte_rrc_dl_Bandwidth,
{ "dl-Bandwidth", "lte-rrc.dl_Bandwidth",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_dl_Bandwidth_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_phich_Config,
{ "phich-Config", "lte-rrc.phich_Config_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_systemFrameNumber,
{ "systemFrameNumber", "lte-rrc.systemFrameNumber",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_spare,
{ "spare", "lte-rrc.spare",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_10", HFILL }},
{ &hf_lte_rrc_countingRequestList_r10,
{ "countingRequestList-r10", "lte-rrc.countingRequestList_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_19,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_09", HFILL }},
{ &hf_lte_rrc_CountingRequestList_r10_item,
{ "CountingRequestInfo-r10", "lte-rrc.CountingRequestInfo_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_tmgi_r10,
{ "tmgi-r10", "lte-rrc.tmgi_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"TMGI_r9", HFILL }},
{ &hf_lte_rrc_criticalExtensions_09,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_09_vals), 0,
"T_criticalExtensions_09", HFILL }},
{ &hf_lte_rrc_c1_13,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_13_vals), 0,
"T_c1_13", HFILL }},
{ &hf_lte_rrc_countingResponse_r10,
{ "countingResponse-r10", "lte-rrc.countingResponse_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"MBMSCountingResponse_r10_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_09,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_09", HFILL }},
{ &hf_lte_rrc_mbsfn_AreaIndex_r10,
{ "mbsfn-AreaIndex-r10", "lte-rrc.mbsfn_AreaIndex_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_maxMBSFN_Area_1", HFILL }},
{ &hf_lte_rrc_countingResponseList_r10,
{ "countingResponseList-r10", "lte-rrc.countingResponseList_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_20,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_10", HFILL }},
{ &hf_lte_rrc_CountingResponseList_r10_item,
{ "CountingResponseInfo-r10", "lte-rrc.CountingResponseInfo_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_countingResponseService_r10,
{ "countingResponseService-r10", "lte-rrc.countingResponseService_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_maxServiceCount_1", HFILL }},
{ &hf_lte_rrc_criticalExtensions_10,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_10_vals), 0,
"T_criticalExtensions_10", HFILL }},
{ &hf_lte_rrc_c1_14,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_14_vals), 0,
"T_c1_14", HFILL }},
{ &hf_lte_rrc_interestIndication_r11,
{ "interestIndication-r11", "lte-rrc.interestIndication_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"MBMSInterestIndication_r11_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_10,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_10", HFILL }},
{ &hf_lte_rrc_mbms_FreqList_r11,
{ "mbms-FreqList-r11", "lte-rrc.mbms_FreqList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"CarrierFreqListMBMS_r11", HFILL }},
{ &hf_lte_rrc_mbms_Priority_r11,
{ "mbms-Priority-r11", "lte-rrc.mbms_Priority_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_mbms_Priority_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_21,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_11", HFILL }},
{ &hf_lte_rrc_commonSF_Alloc_r9,
{ "commonSF-Alloc-r9", "lte-rrc.commonSF_Alloc_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"CommonSF_AllocPatternList_r9", HFILL }},
{ &hf_lte_rrc_commonSF_AllocPeriod_r9,
{ "commonSF-AllocPeriod-r9", "lte-rrc.commonSF_AllocPeriod_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_commonSF_AllocPeriod_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_pmch_InfoList_r9,
{ "pmch-InfoList-r9", "lte-rrc.pmch_InfoList_r9",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_22,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"MBSFNAreaConfiguration_v930_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_23,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_12", HFILL }},
{ &hf_lte_rrc_CommonSF_AllocPatternList_r9_item,
{ "MBSFN-SubframeConfig", "lte-rrc.MBSFN_SubframeConfig_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_criticalExtensions_11,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_11_vals), 0,
"T_criticalExtensions_11", HFILL }},
{ &hf_lte_rrc_c1_15,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_15_vals), 0,
"T_c1_15", HFILL }},
{ &hf_lte_rrc_measurementReport_r8,
{ "measurementReport-r8", "lte-rrc.measurementReport_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"MeasurementReport_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_11,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_11", HFILL }},
{ &hf_lte_rrc_measResults,
{ "measResults", "lte-rrc.measResults_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_24,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"MeasurementReport_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_25,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_13", HFILL }},
{ &hf_lte_rrc_criticalExtensions_12,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_12_vals), 0,
"T_criticalExtensions_12", HFILL }},
{ &hf_lte_rrc_c1_16,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_16_vals), 0,
"T_c1_16", HFILL }},
{ &hf_lte_rrc_mobilityFromEUTRACommand_r8,
{ "mobilityFromEUTRACommand-r8", "lte-rrc.mobilityFromEUTRACommand_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"MobilityFromEUTRACommand_r8_IEs", HFILL }},
{ &hf_lte_rrc_mobilityFromEUTRACommand_r9,
{ "mobilityFromEUTRACommand-r9", "lte-rrc.mobilityFromEUTRACommand_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"MobilityFromEUTRACommand_r9_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_12,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_12", HFILL }},
{ &hf_lte_rrc_cs_FallbackIndicator,
{ "cs-FallbackIndicator", "lte-rrc.cs_FallbackIndicator",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_purpose,
{ "purpose", "lte-rrc.purpose",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_purpose_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_handover,
{ "handover", "lte-rrc.handover_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellChangeOrder,
{ "cellChangeOrder", "lte-rrc.cellChangeOrder_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_26,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"MobilityFromEUTRACommand_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_27,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"MobilityFromEUTRACommand_v8d0_IEs", HFILL }},
{ &hf_lte_rrc_bandIndicator,
{ "bandIndicator", "lte-rrc.bandIndicator",
FT_UINT32, BASE_DEC, VALS(lte_rrc_BandIndicatorGERAN_vals), 0,
"BandIndicatorGERAN", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_28,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_14", HFILL }},
{ &hf_lte_rrc_purpose_01,
{ "purpose", "lte-rrc.purpose",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_purpose_01_vals), 0,
"T_purpose_01", HFILL }},
{ &hf_lte_rrc_e_CSFB_r9,
{ "e-CSFB-r9", "lte-rrc.e_CSFB_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_29,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"MobilityFromEUTRACommand_v930_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_30,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"MobilityFromEUTRACommand_v960_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_31,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_15", HFILL }},
{ &hf_lte_rrc_targetRAT_Type,
{ "targetRAT-Type", "lte-rrc.targetRAT_Type",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_targetRAT_Type_vals), 0,
"T_targetRAT_Type", HFILL }},
{ &hf_lte_rrc_targetRAT_MessageContainer,
{ "targetRAT-MessageContainer", "lte-rrc.targetRAT_MessageContainer",
FT_BYTES, BASE_NONE, NULL, 0,
"T_targetRAT_MessageContainer", HFILL }},
{ &hf_lte_rrc_nas_SecurityParamFromEUTRA,
{ "nas-SecurityParamFromEUTRA", "lte-rrc.nas_SecurityParamFromEUTRA",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_systemInformation_01,
{ "systemInformation", "lte-rrc.systemInformation",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SI_OrPSI_GERAN_vals), 0,
"SI_OrPSI_GERAN", HFILL }},
{ &hf_lte_rrc_t304,
{ "t304", "lte-rrc.t304",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_t304_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_targetRAT_Type_01,
{ "targetRAT-Type", "lte-rrc.targetRAT_Type",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_targetRAT_Type_01_vals), 0,
"T_targetRAT_Type_01", HFILL }},
{ &hf_lte_rrc_geran,
{ "geran", "lte-rrc.geran_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_physCellId,
{ "physCellId", "lte-rrc.physCellId_element",
FT_NONE, BASE_NONE, NULL, 0,
"PhysCellIdGERAN", HFILL }},
{ &hf_lte_rrc_carrierFreq,
{ "carrierFreq", "lte-rrc.carrierFreq_element",
FT_NONE, BASE_NONE, NULL, 0,
"CarrierFreqGERAN", HFILL }},
{ &hf_lte_rrc_networkControlOrder,
{ "networkControlOrder", "lte-rrc.networkControlOrder",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_2", HFILL }},
{ &hf_lte_rrc_si,
{ "si", "lte-rrc.si",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_psi,
{ "psi", "lte-rrc.psi",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_messageContCDMA2000_1XRTT_r9,
{ "messageContCDMA2000-1XRTT-r9", "lte-rrc.messageContCDMA2000_1XRTT_r9",
FT_BYTES, BASE_NONE, NULL, 0,
"OCTET_STRING", HFILL }},
{ &hf_lte_rrc_mobilityCDMA2000_HRPD_r9,
{ "mobilityCDMA2000-HRPD-r9", "lte-rrc.mobilityCDMA2000_HRPD_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_mobilityCDMA2000_HRPD_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_messageContCDMA2000_HRPD_r9,
{ "messageContCDMA2000-HRPD-r9", "lte-rrc.messageContCDMA2000_HRPD_r9",
FT_BYTES, BASE_NONE, NULL, 0,
"OCTET_STRING", HFILL }},
{ &hf_lte_rrc_redirectCarrierCDMA2000_HRPD_r9,
{ "redirectCarrierCDMA2000-HRPD-r9", "lte-rrc.redirectCarrierCDMA2000_HRPD_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"CarrierFreqCDMA2000", HFILL }},
{ &hf_lte_rrc_pagingRecordList,
{ "pagingRecordList", "lte-rrc.pagingRecordList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_systemInfoModification,
{ "systemInfoModification", "lte-rrc.systemInfoModification",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_systemInfoModification_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_etws_Indication,
{ "etws-Indication", "lte-rrc.etws_Indication",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_etws_Indication_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_32,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"Paging_v890_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_33,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"Paging_v920_IEs", HFILL }},
{ &hf_lte_rrc_cmas_Indication_r9,
{ "cmas-Indication-r9", "lte-rrc.cmas_Indication_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cmas_Indication_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_34,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"Paging_v1130_IEs", HFILL }},
{ &hf_lte_rrc_eab_ParamModification_r11,
{ "eab-ParamModification-r11", "lte-rrc.eab_ParamModification_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_eab_ParamModification_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_35,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_16", HFILL }},
{ &hf_lte_rrc_PagingRecordList_item,
{ "PagingRecord", "lte-rrc.PagingRecord_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ue_Identity,
{ "ue-Identity", "lte-rrc.ue_Identity",
FT_UINT32, BASE_DEC, VALS(lte_rrc_PagingUE_Identity_vals), 0,
"PagingUE_Identity", HFILL }},
{ &hf_lte_rrc_cn_Domain,
{ "cn-Domain", "lte-rrc.cn_Domain",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cn_Domain_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_s_TMSI,
{ "s-TMSI", "lte-rrc.s_TMSI_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_imsi,
{ "imsi", "lte-rrc.imsi",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_IMSI_item,
{ "IMSI-Digit", "lte-rrc.IMSI_Digit",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_criticalExtensions_13,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_13_vals), 0,
"T_criticalExtensions_13", HFILL }},
{ &hf_lte_rrc_c1_17,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_17_vals), 0,
"T_c1_17", HFILL }},
{ &hf_lte_rrc_proximityIndication_r9_01,
{ "proximityIndication-r9", "lte-rrc.proximityIndication_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"ProximityIndication_r9_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_13,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_13", HFILL }},
{ &hf_lte_rrc_type_r9,
{ "type-r9", "lte-rrc.type_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_type_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_carrierFreq_r9,
{ "carrierFreq-r9", "lte-rrc.carrierFreq_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_carrierFreq_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_eutra_r9,
{ "eutra-r9", "lte-rrc.eutra_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA", HFILL }},
{ &hf_lte_rrc_utra_r9,
{ "utra-r9", "lte-rrc.utra_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueUTRA", HFILL }},
{ &hf_lte_rrc_eutra2_v9e0,
{ "eutra2-v9e0", "lte-rrc.eutra2_v9e0",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA_v9e0", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_36,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"ProximityIndication_v930_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_37,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_17", HFILL }},
{ &hf_lte_rrc_criticalExtensions_14,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_14_vals), 0,
"T_criticalExtensions_14", HFILL }},
{ &hf_lte_rrc_c1_18,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_18_vals), 0,
"T_c1_18", HFILL }},
{ &hf_lte_rrc_rnReconfiguration_r10_01,
{ "rnReconfiguration-r10", "lte-rrc.rnReconfiguration_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"RNReconfiguration_r10_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_14,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_14", HFILL }},
{ &hf_lte_rrc_rn_SystemInfo_r10,
{ "rn-SystemInfo-r10", "lte-rrc.rn_SystemInfo_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rn_SubframeConfig_r10,
{ "rn-SubframeConfig-r10", "lte-rrc.rn_SubframeConfig_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_38,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_18", HFILL }},
{ &hf_lte_rrc_systemInformationBlockType1_r10,
{ "systemInformationBlockType1-r10", "lte-rrc.systemInformationBlockType1_r10",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_systemInformationBlockType2_r10,
{ "systemInformationBlockType2-r10", "lte-rrc.systemInformationBlockType2_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType2", HFILL }},
{ &hf_lte_rrc_criticalExtensions_15,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_15_vals), 0,
"T_criticalExtensions_15", HFILL }},
{ &hf_lte_rrc_c1_19,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_19_vals), 0,
"T_c1_19", HFILL }},
{ &hf_lte_rrc_rnReconfigurationComplete_r10_01,
{ "rnReconfigurationComplete-r10", "lte-rrc.rnReconfigurationComplete_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"RNReconfigurationComplete_r10_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_15,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_15", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_39,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_19", HFILL }},
{ &hf_lte_rrc_criticalExtensions_16,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_16_vals), 0,
"T_criticalExtensions_16", HFILL }},
{ &hf_lte_rrc_c1_20,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_20_vals), 0,
"T_c1_20", HFILL }},
{ &hf_lte_rrc_rrcConnectionReconfiguration_r8,
{ "rrcConnectionReconfiguration-r8", "lte-rrc.rrcConnectionReconfiguration_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReconfiguration_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_16,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_16", HFILL }},
{ &hf_lte_rrc_measConfig,
{ "measConfig", "lte-rrc.measConfig_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mobilityControlInfo,
{ "mobilityControlInfo", "lte-rrc.mobilityControlInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_dedicatedInfoNASList,
{ "dedicatedInfoNASList", "lte-rrc.dedicatedInfoNASList",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_maxDRB_OF_DedicatedInfoNAS", HFILL }},
{ &hf_lte_rrc_dedicatedInfoNASList_item,
{ "DedicatedInfoNAS", "lte-rrc.DedicatedInfoNAS",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_radioResourceConfigDedicated,
{ "radioResourceConfigDedicated", "lte-rrc.radioResourceConfigDedicated_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_securityConfigHO,
{ "securityConfigHO", "lte-rrc.securityConfigHO_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_40,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReconfiguration_v890_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_41,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReconfiguration_v920_IEs", HFILL }},
{ &hf_lte_rrc_otherConfig_r9,
{ "otherConfig-r9", "lte-rrc.otherConfig_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_fullConfig_r9,
{ "fullConfig-r9", "lte-rrc.fullConfig_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_fullConfig_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_42,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReconfiguration_v1020_IEs", HFILL }},
{ &hf_lte_rrc_sCellToReleaseList_r10,
{ "sCellToReleaseList-r10", "lte-rrc.sCellToReleaseList_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sCellToAddModList_r10,
{ "sCellToAddModList-r10", "lte-rrc.sCellToAddModList_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_43,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReconfiguration_v1130_IEs", HFILL }},
{ &hf_lte_rrc_systemInfomationBlockType1Dedicated_r11,
{ "systemInfomationBlockType1Dedicated-r11", "lte-rrc.systemInfomationBlockType1Dedicated_r11",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_44,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_20", HFILL }},
{ &hf_lte_rrc_SCellToAddModList_r10_item,
{ "SCellToAddMod-r10", "lte-rrc.SCellToAddMod_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sCellIndex_r10,
{ "sCellIndex-r10", "lte-rrc.sCellIndex_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellIdentification_r10,
{ "cellIdentification-r10", "lte-rrc.cellIdentification_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_physCellId_r10,
{ "physCellId-r10", "lte-rrc.physCellId_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellId", HFILL }},
{ &hf_lte_rrc_dl_CarrierFreq_r10,
{ "dl-CarrierFreq-r10", "lte-rrc.dl_CarrierFreq_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA", HFILL }},
{ &hf_lte_rrc_radioResourceConfigCommonSCell_r10,
{ "radioResourceConfigCommonSCell-r10", "lte-rrc.radioResourceConfigCommonSCell_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_radioResourceConfigDedicatedSCell_r10,
{ "radioResourceConfigDedicatedSCell-r10", "lte-rrc.radioResourceConfigDedicatedSCell_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_dl_CarrierFreq_v1090,
{ "dl-CarrierFreq-v1090", "lte-rrc.dl_CarrierFreq_v1090",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA_v9e0", HFILL }},
{ &hf_lte_rrc_SCellToReleaseList_r10_item,
{ "SCellIndex-r10", "lte-rrc.SCellIndex_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_handoverType,
{ "handoverType", "lte-rrc.handoverType",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_handoverType_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_intraLTE,
{ "intraLTE", "lte-rrc.intraLTE_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_securityAlgorithmConfig,
{ "securityAlgorithmConfig", "lte-rrc.securityAlgorithmConfig_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_keyChangeIndicator,
{ "keyChangeIndicator", "lte-rrc.keyChangeIndicator",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_nextHopChainingCount,
{ "nextHopChainingCount", "lte-rrc.nextHopChainingCount",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_interRAT,
{ "interRAT", "lte-rrc.interRAT_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nas_SecurityParamToEUTRA,
{ "nas-SecurityParamToEUTRA", "lte-rrc.nas_SecurityParamToEUTRA",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_criticalExtensions_17,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_17_vals), 0,
"T_criticalExtensions_17", HFILL }},
{ &hf_lte_rrc_rrcConnectionReconfigurationComplete_r8,
{ "rrcConnectionReconfigurationComplete-r8", "lte-rrc.rrcConnectionReconfigurationComplete_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReconfigurationComplete_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_17,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_17", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_45,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReconfigurationComplete_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_46,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReconfigurationComplete_v1020_IEs", HFILL }},
{ &hf_lte_rrc_rlf_InfoAvailable_r10,
{ "rlf-InfoAvailable-r10", "lte-rrc.rlf_InfoAvailable_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_rlf_InfoAvailable_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_logMeasAvailable_r10,
{ "logMeasAvailable-r10", "lte-rrc.logMeasAvailable_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_logMeasAvailable_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_47,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReconfigurationComplete_v1130_IEs", HFILL }},
{ &hf_lte_rrc_connEstFailInfoAvailable_r11,
{ "connEstFailInfoAvailable-r11", "lte-rrc.connEstFailInfoAvailable_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_connEstFailInfoAvailable_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_48,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_21", HFILL }},
{ &hf_lte_rrc_criticalExtensions_18,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_18_vals), 0,
"T_criticalExtensions_18", HFILL }},
{ &hf_lte_rrc_c1_21,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_21_vals), 0,
"T_c1_21", HFILL }},
{ &hf_lte_rrc_rrcConnectionReestablishment_r8,
{ "rrcConnectionReestablishment-r8", "lte-rrc.rrcConnectionReestablishment_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReestablishment_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_18,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_18", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_49,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReestablishment_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_50,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_22", HFILL }},
{ &hf_lte_rrc_criticalExtensions_19,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_19_vals), 0,
"T_criticalExtensions_19", HFILL }},
{ &hf_lte_rrc_rrcConnectionReestablishmentComplete_r8,
{ "rrcConnectionReestablishmentComplete-r8", "lte-rrc.rrcConnectionReestablishmentComplete_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReestablishmentComplete_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_19,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_19", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_51,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReestablishmentComplete_v920_IEs", HFILL }},
{ &hf_lte_rrc_rlf_InfoAvailable_r9,
{ "rlf-InfoAvailable-r9", "lte-rrc.rlf_InfoAvailable_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_rlf_InfoAvailable_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_52,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReestablishmentComplete_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_53,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReestablishmentComplete_v1020_IEs", HFILL }},
{ &hf_lte_rrc_logMeasAvailable_r10_01,
{ "logMeasAvailable-r10", "lte-rrc.logMeasAvailable_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_logMeasAvailable_r10_01_vals), 0,
"T_logMeasAvailable_r10_01", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_54,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReestablishmentComplete_v1130_IEs", HFILL }},
{ &hf_lte_rrc_connEstFailInfoAvailable_r11_01,
{ "connEstFailInfoAvailable-r11", "lte-rrc.connEstFailInfoAvailable_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_connEstFailInfoAvailable_r11_01_vals), 0,
"T_connEstFailInfoAvailable_r11_01", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_55,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_23", HFILL }},
{ &hf_lte_rrc_criticalExtensions_20,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_20_vals), 0,
"T_criticalExtensions_20", HFILL }},
{ &hf_lte_rrc_rrcConnectionReestablishmentReject_r8,
{ "rrcConnectionReestablishmentReject-r8", "lte-rrc.rrcConnectionReestablishmentReject_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReestablishmentReject_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_20,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_20", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_56,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReestablishmentReject_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_57,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_24", HFILL }},
{ &hf_lte_rrc_criticalExtensions_21,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_21_vals), 0,
"T_criticalExtensions_21", HFILL }},
{ &hf_lte_rrc_rrcConnectionReestablishmentRequest_r8,
{ "rrcConnectionReestablishmentRequest-r8", "lte-rrc.rrcConnectionReestablishmentRequest_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReestablishmentRequest_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_21,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_21", HFILL }},
{ &hf_lte_rrc_ue_Identity_01,
{ "ue-Identity", "lte-rrc.ue_Identity_element",
FT_NONE, BASE_NONE, NULL, 0,
"ReestabUE_Identity", HFILL }},
{ &hf_lte_rrc_reestablishmentCause,
{ "reestablishmentCause", "lte-rrc.reestablishmentCause",
FT_UINT32, BASE_DEC, VALS(lte_rrc_ReestablishmentCause_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_spare_01,
{ "spare", "lte-rrc.spare",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_2", HFILL }},
{ &hf_lte_rrc_c_RNTI,
{ "c-RNTI", "lte-rrc.c_RNTI",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_physCellId_01,
{ "physCellId", "lte-rrc.physCellId",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_shortMAC_I,
{ "shortMAC-I", "lte-rrc.shortMAC_I",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_criticalExtensions_22,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_22_vals), 0,
"T_criticalExtensions_22", HFILL }},
{ &hf_lte_rrc_c1_22,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_22_vals), 0,
"T_c1_22", HFILL }},
{ &hf_lte_rrc_rrcConnectionReject_r8,
{ "rrcConnectionReject-r8", "lte-rrc.rrcConnectionReject_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReject_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_22,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_22", HFILL }},
{ &hf_lte_rrc_waitTime,
{ "waitTime", "lte-rrc.waitTime",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_58,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReject_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_59,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReject_v1020_IEs", HFILL }},
{ &hf_lte_rrc_extendedWaitTime_r10,
{ "extendedWaitTime-r10", "lte-rrc.extendedWaitTime_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_60,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionReject_v1130_IEs", HFILL }},
{ &hf_lte_rrc_deprioritisationReq_r11,
{ "deprioritisationReq-r11", "lte-rrc.deprioritisationReq_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_deprioritisationType_r11,
{ "deprioritisationType-r11", "lte-rrc.deprioritisationType_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deprioritisationType_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_deprioritisationTimer_r11,
{ "deprioritisationTimer-r11", "lte-rrc.deprioritisationTimer_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deprioritisationTimer_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_61,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_25", HFILL }},
{ &hf_lte_rrc_criticalExtensions_23,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_23_vals), 0,
"T_criticalExtensions_23", HFILL }},
{ &hf_lte_rrc_c1_23,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_23_vals), 0,
"T_c1_23", HFILL }},
{ &hf_lte_rrc_rrcConnectionRelease_r8,
{ "rrcConnectionRelease-r8", "lte-rrc.rrcConnectionRelease_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionRelease_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_23,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_23", HFILL }},
{ &hf_lte_rrc_releaseCause,
{ "releaseCause", "lte-rrc.releaseCause",
FT_UINT32, BASE_DEC, VALS(lte_rrc_ReleaseCause_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_redirectedCarrierInfo,
{ "redirectedCarrierInfo", "lte-rrc.redirectedCarrierInfo",
FT_UINT32, BASE_DEC, VALS(lte_rrc_RedirectedCarrierInfo_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_idleModeMobilityControlInfo,
{ "idleModeMobilityControlInfo", "lte-rrc.idleModeMobilityControlInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_62,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionRelease_v890_IEs", HFILL }},
{ &hf_lte_rrc_lateNonCriticalExtension_01,
{ "lateNonCriticalExtension", "lte-rrc.lateNonCriticalExtension",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_63,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionRelease_v920_IEs", HFILL }},
{ &hf_lte_rrc_redirectedCarrierInfo_v9e0,
{ "redirectedCarrierInfo-v9e0", "lte-rrc.redirectedCarrierInfo_v9e0_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_idleModeMobilityControlInfo_v9e0,
{ "idleModeMobilityControlInfo-v9e0", "lte-rrc.idleModeMobilityControlInfo_v9e0_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_64,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_26", HFILL }},
{ &hf_lte_rrc_cellInfoList_r9,
{ "cellInfoList-r9", "lte-rrc.cellInfoList_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cellInfoList_r9_vals), 0,
"T_cellInfoList_r9", HFILL }},
{ &hf_lte_rrc_geran_r9,
{ "geran-r9", "lte-rrc.geran_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"CellInfoListGERAN_r9", HFILL }},
{ &hf_lte_rrc_utra_FDD_r9,
{ "utra-FDD-r9", "lte-rrc.utra_FDD_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"CellInfoListUTRA_FDD_r9", HFILL }},
{ &hf_lte_rrc_utra_TDD_r9,
{ "utra-TDD-r9", "lte-rrc.utra_TDD_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"CellInfoListUTRA_TDD_r9", HFILL }},
{ &hf_lte_rrc_utra_TDD_r10,
{ "utra-TDD-r10", "lte-rrc.utra_TDD_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"CellInfoListUTRA_TDD_r10", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_65,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionRelease_v1020_IEs", HFILL }},
{ &hf_lte_rrc_extendedWaitTime_r10_01,
{ "extendedWaitTime-r10", "lte-rrc.extendedWaitTime_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"T_extendedWaitTime_r10_01", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_66,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_27", HFILL }},
{ &hf_lte_rrc_eutra,
{ "eutra", "lte-rrc.eutra",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA", HFILL }},
{ &hf_lte_rrc_geran_01,
{ "geran", "lte-rrc.geran_element",
FT_NONE, BASE_NONE, NULL, 0,
"CarrierFreqsGERAN", HFILL }},
{ &hf_lte_rrc_utra_FDD,
{ "utra-FDD", "lte-rrc.utra_FDD",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueUTRA", HFILL }},
{ &hf_lte_rrc_utra_TDD,
{ "utra-TDD", "lte-rrc.utra_TDD",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueUTRA", HFILL }},
{ &hf_lte_rrc_cdma2000_HRPD,
{ "cdma2000-HRPD", "lte-rrc.cdma2000_HRPD_element",
FT_NONE, BASE_NONE, NULL, 0,
"CarrierFreqCDMA2000", HFILL }},
{ &hf_lte_rrc_cdma2000_1xRTT,
{ "cdma2000-1xRTT", "lte-rrc.cdma2000_1xRTT_element",
FT_NONE, BASE_NONE, NULL, 0,
"CarrierFreqCDMA2000", HFILL }},
{ &hf_lte_rrc_utra_TDD_r10_01,
{ "utra-TDD-r10", "lte-rrc.utra_TDD_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"CarrierFreqListUTRA_TDD_r10", HFILL }},
{ &hf_lte_rrc_eutra_v9e0,
{ "eutra-v9e0", "lte-rrc.eutra_v9e0",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA_v9e0", HFILL }},
{ &hf_lte_rrc_CarrierFreqListUTRA_TDD_r10_item,
{ "ARFCN-ValueUTRA", "lte-rrc.ARFCN_ValueUTRA",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_freqPriorityListEUTRA,
{ "freqPriorityListEUTRA", "lte-rrc.freqPriorityListEUTRA",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_freqPriorityListGERAN,
{ "freqPriorityListGERAN", "lte-rrc.freqPriorityListGERAN",
FT_UINT32, BASE_DEC, NULL, 0,
"FreqsPriorityListGERAN", HFILL }},
{ &hf_lte_rrc_freqPriorityListUTRA_FDD,
{ "freqPriorityListUTRA-FDD", "lte-rrc.freqPriorityListUTRA_FDD",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_freqPriorityListUTRA_TDD,
{ "freqPriorityListUTRA-TDD", "lte-rrc.freqPriorityListUTRA_TDD",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_bandClassPriorityListHRPD,
{ "bandClassPriorityListHRPD", "lte-rrc.bandClassPriorityListHRPD",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_bandClassPriorityList1XRTT,
{ "bandClassPriorityList1XRTT", "lte-rrc.bandClassPriorityList1XRTT",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_t320,
{ "t320", "lte-rrc.t320",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_t320_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_freqPriorityListEUTRA_v9e0,
{ "freqPriorityListEUTRA-v9e0", "lte-rrc.freqPriorityListEUTRA_v9e0",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_maxFreq_OF_FreqPriorityEUTRA_v9e0", HFILL }},
{ &hf_lte_rrc_freqPriorityListEUTRA_v9e0_item,
{ "FreqPriorityEUTRA-v9e0", "lte-rrc.FreqPriorityEUTRA_v9e0_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_FreqPriorityListEUTRA_item,
{ "FreqPriorityEUTRA", "lte-rrc.FreqPriorityEUTRA_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_carrierFreq_01,
{ "carrierFreq", "lte-rrc.carrierFreq",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA", HFILL }},
{ &hf_lte_rrc_cellReselectionPriority,
{ "cellReselectionPriority", "lte-rrc.cellReselectionPriority",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_carrierFreq_v9e0,
{ "carrierFreq-v9e0", "lte-rrc.carrierFreq_v9e0",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA_v9e0", HFILL }},
{ &hf_lte_rrc_FreqsPriorityListGERAN_item,
{ "FreqsPriorityGERAN", "lte-rrc.FreqsPriorityGERAN_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_carrierFreqs,
{ "carrierFreqs", "lte-rrc.carrierFreqs_element",
FT_NONE, BASE_NONE, NULL, 0,
"CarrierFreqsGERAN", HFILL }},
{ &hf_lte_rrc_FreqPriorityListUTRA_FDD_item,
{ "FreqPriorityUTRA-FDD", "lte-rrc.FreqPriorityUTRA_FDD_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_carrierFreq_02,
{ "carrierFreq", "lte-rrc.carrierFreq",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueUTRA", HFILL }},
{ &hf_lte_rrc_FreqPriorityListUTRA_TDD_item,
{ "FreqPriorityUTRA-TDD", "lte-rrc.FreqPriorityUTRA_TDD_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_BandClassPriorityListHRPD_item,
{ "BandClassPriorityHRPD", "lte-rrc.BandClassPriorityHRPD_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_bandClass,
{ "bandClass", "lte-rrc.bandClass",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_BandclassCDMA2000_vals_ext, 0,
"BandclassCDMA2000", HFILL }},
{ &hf_lte_rrc_BandClassPriorityList1XRTT_item,
{ "BandClassPriority1XRTT", "lte-rrc.BandClassPriority1XRTT_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CellInfoListGERAN_r9_item,
{ "CellInfoGERAN-r9", "lte-rrc.CellInfoGERAN_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_physCellId_r9,
{ "physCellId-r9", "lte-rrc.physCellId_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"PhysCellIdGERAN", HFILL }},
{ &hf_lte_rrc_carrierFreq_r9_01,
{ "carrierFreq-r9", "lte-rrc.carrierFreq_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"CarrierFreqGERAN", HFILL }},
{ &hf_lte_rrc_systemInformation_r9,
{ "systemInformation-r9", "lte-rrc.systemInformation_r9",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CellInfoListUTRA_FDD_r9_item,
{ "CellInfoUTRA-FDD-r9", "lte-rrc.CellInfoUTRA_FDD_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_physCellId_r9_01,
{ "physCellId-r9", "lte-rrc.physCellId_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellIdUTRA_FDD", HFILL }},
{ &hf_lte_rrc_utra_BCCH_Container_r9,
{ "utra-BCCH-Container-r9", "lte-rrc.utra_BCCH_Container_r9",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CellInfoListUTRA_TDD_r9_item,
{ "CellInfoUTRA-TDD-r9", "lte-rrc.CellInfoUTRA_TDD_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_physCellId_r9_02,
{ "physCellId-r9", "lte-rrc.physCellId_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellIdUTRA_TDD", HFILL }},
{ &hf_lte_rrc_utra_BCCH_Container_r9_01,
{ "utra-BCCH-Container-r9", "lte-rrc.utra_BCCH_Container_r9",
FT_BYTES, BASE_NONE, NULL, 0,
"T_utra_BCCH_Container_r9_01", HFILL }},
{ &hf_lte_rrc_CellInfoListUTRA_TDD_r10_item,
{ "CellInfoUTRA-TDD-r10", "lte-rrc.CellInfoUTRA_TDD_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_physCellId_r10_01,
{ "physCellId-r10", "lte-rrc.physCellId_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellIdUTRA_TDD", HFILL }},
{ &hf_lte_rrc_carrierFreq_r10_01,
{ "carrierFreq-r10", "lte-rrc.carrierFreq_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueUTRA", HFILL }},
{ &hf_lte_rrc_utra_BCCH_Container_r10,
{ "utra-BCCH-Container-r10", "lte-rrc.utra_BCCH_Container_r10",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_criticalExtensions_24,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_24_vals), 0,
"T_criticalExtensions_24", HFILL }},
{ &hf_lte_rrc_rrcConnectionRequest_r8,
{ "rrcConnectionRequest-r8", "lte-rrc.rrcConnectionRequest_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionRequest_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_24,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_24", HFILL }},
{ &hf_lte_rrc_ue_Identity_02,
{ "ue-Identity", "lte-rrc.ue_Identity",
FT_UINT32, BASE_DEC, VALS(lte_rrc_InitialUE_Identity_vals), 0,
"InitialUE_Identity", HFILL }},
{ &hf_lte_rrc_establishmentCause,
{ "establishmentCause", "lte-rrc.establishmentCause",
FT_UINT32, BASE_DEC, VALS(lte_rrc_EstablishmentCause_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_spare_02,
{ "spare", "lte-rrc.spare",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_1", HFILL }},
{ &hf_lte_rrc_randomValue,
{ "randomValue", "lte-rrc.randomValue",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_40", HFILL }},
{ &hf_lte_rrc_criticalExtensions_25,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_25_vals), 0,
"T_criticalExtensions_25", HFILL }},
{ &hf_lte_rrc_c1_24,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_24_vals), 0,
"T_c1_24", HFILL }},
{ &hf_lte_rrc_rrcConnectionSetup_r8,
{ "rrcConnectionSetup-r8", "lte-rrc.rrcConnectionSetup_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionSetup_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_25,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_25", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_67,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionSetup_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_68,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_28", HFILL }},
{ &hf_lte_rrc_criticalExtensions_26,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_26_vals), 0,
"T_criticalExtensions_26", HFILL }},
{ &hf_lte_rrc_c1_25,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_25_vals), 0,
"T_c1_25", HFILL }},
{ &hf_lte_rrc_rrcConnectionSetupComplete_r8,
{ "rrcConnectionSetupComplete-r8", "lte-rrc.rrcConnectionSetupComplete_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionSetupComplete_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_26,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_26", HFILL }},
{ &hf_lte_rrc_selectedPLMN_Identity,
{ "selectedPLMN-Identity", "lte-rrc.selectedPLMN_Identity",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_maxPLMN_r11", HFILL }},
{ &hf_lte_rrc_registeredMME,
{ "registeredMME", "lte-rrc.registeredMME_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_69,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionSetupComplete_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_70,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionSetupComplete_v1020_IEs", HFILL }},
{ &hf_lte_rrc_gummei_Type_r10,
{ "gummei-Type-r10", "lte-rrc.gummei_Type_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_gummei_Type_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_rlf_InfoAvailable_r10_01,
{ "rlf-InfoAvailable-r10", "lte-rrc.rlf_InfoAvailable_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_rlf_InfoAvailable_r10_01_vals), 0,
"T_rlf_InfoAvailable_r10_01", HFILL }},
{ &hf_lte_rrc_logMeasAvailable_r10_02,
{ "logMeasAvailable-r10", "lte-rrc.logMeasAvailable_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_logMeasAvailable_r10_02_vals), 0,
"T_logMeasAvailable_r10_02", HFILL }},
{ &hf_lte_rrc_rn_SubframeConfigReq_r10,
{ "rn-SubframeConfigReq-r10", "lte-rrc.rn_SubframeConfigReq_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_rn_SubframeConfigReq_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_71,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"RRCConnectionSetupComplete_v1130_IEs", HFILL }},
{ &hf_lte_rrc_connEstFailInfoAvailable_r11_02,
{ "connEstFailInfoAvailable-r11", "lte-rrc.connEstFailInfoAvailable_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_connEstFailInfoAvailable_r11_02_vals), 0,
"T_connEstFailInfoAvailable_r11_02", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_72,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_29", HFILL }},
{ &hf_lte_rrc_plmn_Identity,
{ "plmn-Identity", "lte-rrc.plmn_Identity_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mmegi,
{ "mmegi", "lte-rrc.mmegi",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_16", HFILL }},
{ &hf_lte_rrc_mmec,
{ "mmec", "lte-rrc.mmec",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_criticalExtensions_27,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_27_vals), 0,
"T_criticalExtensions_27", HFILL }},
{ &hf_lte_rrc_c1_26,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_26_vals), 0,
"T_c1_26", HFILL }},
{ &hf_lte_rrc_securityModeCommand_r8,
{ "securityModeCommand-r8", "lte-rrc.securityModeCommand_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"SecurityModeCommand_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_27,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_27", HFILL }},
{ &hf_lte_rrc_securityConfigSMC,
{ "securityConfigSMC", "lte-rrc.securityConfigSMC_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_73,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"SecurityModeCommand_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_74,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_30", HFILL }},
{ &hf_lte_rrc_criticalExtensions_28,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_28_vals), 0,
"T_criticalExtensions_28", HFILL }},
{ &hf_lte_rrc_securityModeComplete_r8,
{ "securityModeComplete-r8", "lte-rrc.securityModeComplete_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"SecurityModeComplete_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_28,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_28", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_75,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"SecurityModeComplete_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_76,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_31", HFILL }},
{ &hf_lte_rrc_criticalExtensions_29,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_29_vals), 0,
"T_criticalExtensions_29", HFILL }},
{ &hf_lte_rrc_securityModeFailure_r8,
{ "securityModeFailure-r8", "lte-rrc.securityModeFailure_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"SecurityModeFailure_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_29,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_29", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_77,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"SecurityModeFailure_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_78,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_32", HFILL }},
{ &hf_lte_rrc_criticalExtensions_30,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_30_vals), 0,
"T_criticalExtensions_30", HFILL }},
{ &hf_lte_rrc_systemInformation_r8,
{ "systemInformation-r8", "lte-rrc.systemInformation_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformation_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_30,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_30", HFILL }},
{ &hf_lte_rrc_sib_TypeAndInfo,
{ "sib-TypeAndInfo", "lte-rrc.sib_TypeAndInfo",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sib_TypeAndInfo_item,
{ "sib-TypeAndInfo item", "lte-rrc.sib_TypeAndInfo_item",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_sib_TypeAndInfo_item_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_sib2,
{ "sib2", "lte-rrc.sib2_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType2", HFILL }},
{ &hf_lte_rrc_sib3,
{ "sib3", "lte-rrc.sib3_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType3", HFILL }},
{ &hf_lte_rrc_sib4,
{ "sib4", "lte-rrc.sib4_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType4", HFILL }},
{ &hf_lte_rrc_sib5,
{ "sib5", "lte-rrc.sib5_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType5", HFILL }},
{ &hf_lte_rrc_sib6,
{ "sib6", "lte-rrc.sib6_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType6", HFILL }},
{ &hf_lte_rrc_sib7,
{ "sib7", "lte-rrc.sib7_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType7", HFILL }},
{ &hf_lte_rrc_sib8,
{ "sib8", "lte-rrc.sib8_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType8", HFILL }},
{ &hf_lte_rrc_sib9,
{ "sib9", "lte-rrc.sib9_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType9", HFILL }},
{ &hf_lte_rrc_sib10,
{ "sib10", "lte-rrc.sib10_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType10", HFILL }},
{ &hf_lte_rrc_sib11,
{ "sib11", "lte-rrc.sib11_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType11", HFILL }},
{ &hf_lte_rrc_sib12_v920,
{ "sib12-v920", "lte-rrc.sib12_v920_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType12_r9", HFILL }},
{ &hf_lte_rrc_sib13_v920,
{ "sib13-v920", "lte-rrc.sib13_v920_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType13_r9", HFILL }},
{ &hf_lte_rrc_sib14_v1130,
{ "sib14-v1130", "lte-rrc.sib14_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType14_r11", HFILL }},
{ &hf_lte_rrc_sib15_v1130,
{ "sib15-v1130", "lte-rrc.sib15_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType15_r11", HFILL }},
{ &hf_lte_rrc_sib16_v1130,
{ "sib16-v1130", "lte-rrc.sib16_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType16_r11", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_79,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformation_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_80,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_33", HFILL }},
{ &hf_lte_rrc_cellAccessRelatedInfo,
{ "cellAccessRelatedInfo", "lte-rrc.cellAccessRelatedInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_plmn_IdentityList,
{ "plmn-IdentityList", "lte-rrc.plmn_IdentityList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_trackingAreaCode,
{ "trackingAreaCode", "lte-rrc.trackingAreaCode",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellIdentity,
{ "cellIdentity", "lte-rrc.cellIdentity",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellBarred,
{ "cellBarred", "lte-rrc.cellBarred",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cellBarred_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_intraFreqReselection,
{ "intraFreqReselection", "lte-rrc.intraFreqReselection",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_intraFreqReselection_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_csg_Indication,
{ "csg-Indication", "lte-rrc.csg_Indication",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_csg_Identity,
{ "csg-Identity", "lte-rrc.csg_Identity",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellSelectionInfo,
{ "cellSelectionInfo", "lte-rrc.cellSelectionInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_q_RxLevMin,
{ "q-RxLevMin", "lte-rrc.q_RxLevMin",
FT_INT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_q_RxLevMin_vals_ext, 0,
NULL, HFILL }},
{ &hf_lte_rrc_q_RxLevMinOffset,
{ "q-RxLevMinOffset", "lte-rrc.q_RxLevMinOffset",
FT_UINT32, BASE_DEC, VALS(lte_rrc_q_RxLevMinOffset_vals), 0,
"INTEGER_1_8", HFILL }},
{ &hf_lte_rrc_p_Max,
{ "p-Max", "lte-rrc.p_Max",
FT_INT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_freqBandIndicator,
{ "freqBandIndicator", "lte-rrc.freqBandIndicator",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_schedulingInfoList,
{ "schedulingInfoList", "lte-rrc.schedulingInfoList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_tdd_Config,
{ "tdd-Config", "lte-rrc.tdd_Config_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_si_WindowLength,
{ "si-WindowLength", "lte-rrc.si_WindowLength",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_si_WindowLength_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_systemInfoValueTag,
{ "systemInfoValueTag", "lte-rrc.systemInfoValueTag",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_81,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType1_v890_IEs", HFILL }},
{ &hf_lte_rrc_lateNonCriticalExtension_02,
{ "lateNonCriticalExtension", "lte-rrc.lateNonCriticalExtension",
FT_BYTES, BASE_NONE, NULL, 0,
"T_lateNonCriticalExtension_01", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_82,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType1_v920_IEs", HFILL }},
{ &hf_lte_rrc_multiBandInfoList,
{ "multiBandInfoList", "lte-rrc.multiBandInfoList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_83,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType1_v9e0_IEs", HFILL }},
{ &hf_lte_rrc_freqBandIndicator_v9e0,
{ "freqBandIndicator-v9e0", "lte-rrc.freqBandIndicator_v9e0",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_multiBandInfoList_v9e0,
{ "multiBandInfoList-v9e0", "lte-rrc.multiBandInfoList_v9e0",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_84,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_34", HFILL }},
{ &hf_lte_rrc_ims_EmergencySupport_r9,
{ "ims-EmergencySupport-r9", "lte-rrc.ims_EmergencySupport_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_ims_EmergencySupport_r9_vals), 0,
"T_ims_EmergencySupport_r9", HFILL }},
{ &hf_lte_rrc_cellSelectionInfo_v920,
{ "cellSelectionInfo-v920", "lte-rrc.cellSelectionInfo_v920_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_85,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType1_v1130_IEs", HFILL }},
{ &hf_lte_rrc_tdd_Config_v1130,
{ "tdd-Config-v1130", "lte-rrc.tdd_Config_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellSelectionInfo_v1130,
{ "cellSelectionInfo-v1130", "lte-rrc.cellSelectionInfo_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_86,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_35", HFILL }},
{ &hf_lte_rrc_PLMN_IdentityList_item,
{ "PLMN-IdentityInfo", "lte-rrc.PLMN_IdentityInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellReservedForOperatorUse,
{ "cellReservedForOperatorUse", "lte-rrc.cellReservedForOperatorUse",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cellReservedForOperatorUse_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_SchedulingInfoList_item,
{ "SchedulingInfo", "lte-rrc.SchedulingInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_si_Periodicity,
{ "si-Periodicity", "lte-rrc.si_Periodicity",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_si_Periodicity_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_sib_MappingInfo,
{ "sib-MappingInfo", "lte-rrc.sib_MappingInfo",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SIB_MappingInfo_item,
{ "SIB-Type", "lte-rrc.SIB_Type",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SIB_Type_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_q_QualMin_r9,
{ "q-QualMin-r9", "lte-rrc.q_QualMin_r9",
FT_INT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_q_QualMinOffset_r9,
{ "q-QualMinOffset-r9", "lte-rrc.q_QualMinOffset_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"T_q_QualMinOffset_r9", HFILL }},
{ &hf_lte_rrc_q_QualMinWB_r11,
{ "q-QualMinWB-r11", "lte-rrc.q_QualMinWB_r11",
FT_INT32, BASE_DEC, NULL, 0,
"Q_QualMin_r9", HFILL }},
{ &hf_lte_rrc_criticalExtensions_31,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_31_vals), 0,
"T_criticalExtensions_31", HFILL }},
{ &hf_lte_rrc_c1_27,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_27_vals), 0,
"T_c1_27", HFILL }},
{ &hf_lte_rrc_ueAssistanceInformation_r11_01,
{ "ueAssistanceInformation-r11", "lte-rrc.ueAssistanceInformation_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"UEAssistanceInformation_r11_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_31,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_31", HFILL }},
{ &hf_lte_rrc_powerPrefIndication_r11,
{ "powerPrefIndication-r11", "lte-rrc.powerPrefIndication_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_powerPrefIndication_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_87,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_36", HFILL }},
{ &hf_lte_rrc_criticalExtensions_32,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_32_vals), 0,
"T_criticalExtensions_32", HFILL }},
{ &hf_lte_rrc_c1_28,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_28_vals), 0,
"T_c1_28", HFILL }},
{ &hf_lte_rrc_ueCapabilityEnquiry_r8,
{ "ueCapabilityEnquiry-r8", "lte-rrc.ueCapabilityEnquiry_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"UECapabilityEnquiry_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_32,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_32", HFILL }},
{ &hf_lte_rrc_ue_CapabilityRequest,
{ "ue-CapabilityRequest", "lte-rrc.ue_CapabilityRequest",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_88,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UECapabilityEnquiry_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_89,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UECapabilityEnquiry_v1180_IEs", HFILL }},
{ &hf_lte_rrc_requestedFrequencyBands_r11,
{ "requestedFrequencyBands-r11", "lte-rrc.requestedFrequencyBands_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_16_OF_FreqBandIndicator_r11", HFILL }},
{ &hf_lte_rrc_requestedFrequencyBands_r11_item,
{ "FreqBandIndicator-r11", "lte-rrc.FreqBandIndicator_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_90,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_37", HFILL }},
{ &hf_lte_rrc_UE_CapabilityRequest_item,
{ "RAT-Type", "lte-rrc.RAT_Type",
FT_UINT32, BASE_DEC, VALS(lte_rrc_RAT_Type_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_criticalExtensions_33,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_33_vals), 0,
"T_criticalExtensions_33", HFILL }},
{ &hf_lte_rrc_c1_29,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_29_vals), 0,
"T_c1_29", HFILL }},
{ &hf_lte_rrc_ueCapabilityInformation_r8,
{ "ueCapabilityInformation-r8", "lte-rrc.ueCapabilityInformation_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"UECapabilityInformation_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_33,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_33", HFILL }},
{ &hf_lte_rrc_ue_CapabilityRAT_ContainerList,
{ "ue-CapabilityRAT-ContainerList", "lte-rrc.ue_CapabilityRAT_ContainerList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_91,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UECapabilityInformation_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_92,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_38", HFILL }},
{ &hf_lte_rrc_criticalExtensions_34,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_34_vals), 0,
"T_criticalExtensions_34", HFILL }},
{ &hf_lte_rrc_c1_30,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_30_vals), 0,
"T_c1_30", HFILL }},
{ &hf_lte_rrc_ueInformationRequest_r9_01,
{ "ueInformationRequest-r9", "lte-rrc.ueInformationRequest_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"UEInformationRequest_r9_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_34,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_34", HFILL }},
{ &hf_lte_rrc_rach_ReportReq_r9,
{ "rach-ReportReq-r9", "lte-rrc.rach_ReportReq_r9",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_rlf_ReportReq_r9,
{ "rlf-ReportReq-r9", "lte-rrc.rlf_ReportReq_r9",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_93,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UEInformationRequest_v930_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_94,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UEInformationRequest_v1020_IEs", HFILL }},
{ &hf_lte_rrc_logMeasReportReq_r10,
{ "logMeasReportReq-r10", "lte-rrc.logMeasReportReq_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_logMeasReportReq_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_95,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UEInformationRequest_v1130_IEs", HFILL }},
{ &hf_lte_rrc_connEstFailReportReq_r11,
{ "connEstFailReportReq-r11", "lte-rrc.connEstFailReportReq_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_connEstFailReportReq_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_96,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_39", HFILL }},
{ &hf_lte_rrc_criticalExtensions_35,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_35_vals), 0,
"T_criticalExtensions_35", HFILL }},
{ &hf_lte_rrc_c1_31,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_31_vals), 0,
"T_c1_31", HFILL }},
{ &hf_lte_rrc_ueInformationResponse_r9_01,
{ "ueInformationResponse-r9", "lte-rrc.ueInformationResponse_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"UEInformationResponse_r9_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_35,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_35", HFILL }},
{ &hf_lte_rrc_rach_Report_r9,
{ "rach-Report-r9", "lte-rrc.rach_Report_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_rach_Report_r9", HFILL }},
{ &hf_lte_rrc_numberOfPreamblesSent_r9,
{ "numberOfPreamblesSent-r9", "lte-rrc.numberOfPreamblesSent_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"NumberOfPreamblesSent_r11", HFILL }},
{ &hf_lte_rrc_contentionDetected_r9,
{ "contentionDetected-r9", "lte-rrc.contentionDetected_r9",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_rlf_Report_r9,
{ "rlf-Report-r9", "lte-rrc.rlf_Report_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_97,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UEInformationResponse_v930_IEs", HFILL }},
{ &hf_lte_rrc_rlf_Report_v9e0,
{ "rlf-Report-v9e0", "lte-rrc.rlf_Report_v9e0_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_98,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_40", HFILL }},
{ &hf_lte_rrc_lateNonCriticalExtension_03,
{ "lateNonCriticalExtension", "lte-rrc.lateNonCriticalExtension",
FT_BYTES, BASE_NONE, NULL, 0,
"T_lateNonCriticalExtension_02", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_99,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UEInformationResponse_v1020_IEs", HFILL }},
{ &hf_lte_rrc_logMeasReport_r10,
{ "logMeasReport-r10", "lte-rrc.logMeasReport_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_100,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UEInformationResponse_v1130_IEs", HFILL }},
{ &hf_lte_rrc_connEstFailReport_r11,
{ "connEstFailReport-r11", "lte-rrc.connEstFailReport_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_101,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_41", HFILL }},
{ &hf_lte_rrc_measResultLastServCell_r9,
{ "measResultLastServCell-r9", "lte-rrc.measResultLastServCell_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rsrpResult_r9,
{ "rsrpResult-r9", "lte-rrc.rsrpResult_r9",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_RSRP_Range_vals_ext, 0,
"RSRP_Range", HFILL }},
{ &hf_lte_rrc_rsrqResult_r9,
{ "rsrqResult-r9", "lte-rrc.rsrqResult_r9",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_RSRQ_Range_vals_ext, 0,
"RSRQ_Range", HFILL }},
{ &hf_lte_rrc_measResultNeighCells_r9,
{ "measResultNeighCells-r9", "lte-rrc.measResultNeighCells_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measResultListEUTRA_r9,
{ "measResultListEUTRA-r9", "lte-rrc.measResultListEUTRA_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultList2EUTRA_r9", HFILL }},
{ &hf_lte_rrc_measResultListUTRA_r9,
{ "measResultListUTRA-r9", "lte-rrc.measResultListUTRA_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultList2UTRA_r9", HFILL }},
{ &hf_lte_rrc_measResultListGERAN_r9,
{ "measResultListGERAN-r9", "lte-rrc.measResultListGERAN_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultListGERAN", HFILL }},
{ &hf_lte_rrc_measResultsCDMA2000_r9,
{ "measResultsCDMA2000-r9", "lte-rrc.measResultsCDMA2000_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultList2CDMA2000_r9", HFILL }},
{ &hf_lte_rrc_locationInfo_r10,
{ "locationInfo-r10", "lte-rrc.locationInfo_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_failedPCellId_r10,
{ "failedPCellId-r10", "lte-rrc.failedPCellId_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_failedPCellId_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellGlobalId_r10,
{ "cellGlobalId-r10", "lte-rrc.cellGlobalId_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"CellGlobalIdEUTRA", HFILL }},
{ &hf_lte_rrc_pci_arfcn_r10,
{ "pci-arfcn-r10", "lte-rrc.pci_arfcn_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_reestablishmentCellId_r10,
{ "reestablishmentCellId-r10", "lte-rrc.reestablishmentCellId_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"CellGlobalIdEUTRA", HFILL }},
{ &hf_lte_rrc_timeConnFailure_r10,
{ "timeConnFailure-r10", "lte-rrc.timeConnFailure_r10",
FT_UINT32, BASE_CUSTOM, &lte_rrc_timeConnFailure_r10_fmt, 0,
"INTEGER_0_1023", HFILL }},
{ &hf_lte_rrc_connectionFailureType_r10,
{ "connectionFailureType-r10", "lte-rrc.connectionFailureType_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_connectionFailureType_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_previousPCellId_r10,
{ "previousPCellId-r10", "lte-rrc.previousPCellId_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"CellGlobalIdEUTRA", HFILL }},
{ &hf_lte_rrc_failedPCellId_v1090,
{ "failedPCellId-v1090", "lte-rrc.failedPCellId_v1090_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_basicFields_r11,
{ "basicFields-r11", "lte-rrc.basicFields_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_c_RNTI_r11,
{ "c-RNTI-r11", "lte-rrc.c_RNTI_r11",
FT_BYTES, BASE_NONE, NULL, 0,
"C_RNTI", HFILL }},
{ &hf_lte_rrc_rlf_Cause_r11,
{ "rlf-Cause-r11", "lte-rrc.rlf_Cause_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_rlf_Cause_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_timeSinceFailure_r11,
{ "timeSinceFailure-r11", "lte-rrc.timeSinceFailure_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_previousUTRA_CellId_r11,
{ "previousUTRA-CellId-r11", "lte-rrc.previousUTRA_CellId_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_carrierFreq_r11_01,
{ "carrierFreq-r11", "lte-rrc.carrierFreq_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueUTRA", HFILL }},
{ &hf_lte_rrc_physCellId_r11,
{ "physCellId-r11", "lte-rrc.physCellId_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_physCellId_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_fdd_r11,
{ "fdd-r11", "lte-rrc.fdd_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellIdUTRA_FDD", HFILL }},
{ &hf_lte_rrc_tdd_r11,
{ "tdd-r11", "lte-rrc.tdd_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellIdUTRA_TDD", HFILL }},
{ &hf_lte_rrc_cellGlobalId_r11,
{ "cellGlobalId-r11", "lte-rrc.cellGlobalId_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"CellGlobalIdUTRA", HFILL }},
{ &hf_lte_rrc_selectedUTRA_CellId_r11,
{ "selectedUTRA-CellId-r11", "lte-rrc.selectedUTRA_CellId_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_physCellId_r11_01,
{ "physCellId-r11", "lte-rrc.physCellId_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_physCellId_r11_01_vals), 0,
"T_physCellId_r11_01", HFILL }},
{ &hf_lte_rrc_measResultListEUTRA_v9e0,
{ "measResultListEUTRA-v9e0", "lte-rrc.measResultListEUTRA_v9e0",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultList2EUTRA_v9e0", HFILL }},
{ &hf_lte_rrc_MeasResultList2EUTRA_r9_item,
{ "MeasResult2EUTRA-r9", "lte-rrc.MeasResult2EUTRA_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MeasResultList2EUTRA_v9e0_item,
{ "MeasResult2EUTRA-v9e0", "lte-rrc.MeasResult2EUTRA_v9e0_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_carrierFreq_r9_02,
{ "carrierFreq-r9", "lte-rrc.carrierFreq_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA", HFILL }},
{ &hf_lte_rrc_measResultList_r9,
{ "measResultList-r9", "lte-rrc.measResultList_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultListEUTRA", HFILL }},
{ &hf_lte_rrc_MeasResultList2UTRA_r9_item,
{ "MeasResult2UTRA-r9", "lte-rrc.MeasResult2UTRA_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_carrierFreq_r9_03,
{ "carrierFreq-r9", "lte-rrc.carrierFreq_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueUTRA", HFILL }},
{ &hf_lte_rrc_measResultList_r9_01,
{ "measResultList-r9", "lte-rrc.measResultList_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultListUTRA", HFILL }},
{ &hf_lte_rrc_MeasResultList2CDMA2000_r9_item,
{ "MeasResult2CDMA2000-r9", "lte-rrc.MeasResult2CDMA2000_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_carrierFreq_r9_04,
{ "carrierFreq-r9", "lte-rrc.carrierFreq_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"CarrierFreqCDMA2000", HFILL }},
{ &hf_lte_rrc_measResultList_r9_02,
{ "measResultList-r9", "lte-rrc.measResultList_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"MeasResultsCDMA2000", HFILL }},
{ &hf_lte_rrc_absoluteTimeStamp_r10,
{ "absoluteTimeStamp-r10", "lte-rrc.absoluteTimeStamp_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"AbsoluteTimeInfo_r10", HFILL }},
{ &hf_lte_rrc_logMeasInfoList_r10,
{ "logMeasInfoList-r10", "lte-rrc.logMeasInfoList_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_logMeasAvailable_r10_03,
{ "logMeasAvailable-r10", "lte-rrc.logMeasAvailable_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_logMeasAvailable_r10_03_vals), 0,
"T_logMeasAvailable_r10_03", HFILL }},
{ &hf_lte_rrc_LogMeasInfoList_r10_item,
{ "LogMeasInfo-r10", "lte-rrc.LogMeasInfo_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_relativeTimeStamp_r10,
{ "relativeTimeStamp-r10", "lte-rrc.relativeTimeStamp_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_servCellIdentity_r10,
{ "servCellIdentity-r10", "lte-rrc.servCellIdentity_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"CellGlobalIdEUTRA", HFILL }},
{ &hf_lte_rrc_measResultServCell_r10,
{ "measResultServCell-r10", "lte-rrc.measResultServCell_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rsrpResult_r10,
{ "rsrpResult-r10", "lte-rrc.rsrpResult_r10",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_RSRP_Range_vals_ext, 0,
"RSRP_Range", HFILL }},
{ &hf_lte_rrc_rsrqResult_r10,
{ "rsrqResult-r10", "lte-rrc.rsrqResult_r10",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_RSRQ_Range_vals_ext, 0,
"RSRQ_Range", HFILL }},
{ &hf_lte_rrc_measResultNeighCells_r10,
{ "measResultNeighCells-r10", "lte-rrc.measResultNeighCells_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measResultListEUTRA_r10,
{ "measResultListEUTRA-r10", "lte-rrc.measResultListEUTRA_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultList2EUTRA_r9", HFILL }},
{ &hf_lte_rrc_measResultListUTRA_r10,
{ "measResultListUTRA-r10", "lte-rrc.measResultListUTRA_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultList2UTRA_r9", HFILL }},
{ &hf_lte_rrc_measResultListGERAN_r10,
{ "measResultListGERAN-r10", "lte-rrc.measResultListGERAN_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultList2GERAN_r10", HFILL }},
{ &hf_lte_rrc_measResultListCDMA2000_r10,
{ "measResultListCDMA2000-r10", "lte-rrc.measResultListCDMA2000_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultList2CDMA2000_r9", HFILL }},
{ &hf_lte_rrc_measResultListEUTRA_v1090,
{ "measResultListEUTRA-v1090", "lte-rrc.measResultListEUTRA_v1090",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultList2EUTRA_v9e0", HFILL }},
{ &hf_lte_rrc_MeasResultList2GERAN_r10_item,
{ "MeasResultListGERAN", "lte-rrc.MeasResultListGERAN",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_failedCellId_r11,
{ "failedCellId-r11", "lte-rrc.failedCellId_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"CellGlobalIdEUTRA", HFILL }},
{ &hf_lte_rrc_locationInfo_r11,
{ "locationInfo-r11", "lte-rrc.locationInfo_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"LocationInfo_r10", HFILL }},
{ &hf_lte_rrc_measResultFailedCell_r11,
{ "measResultFailedCell-r11", "lte-rrc.measResultFailedCell_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rsrpResult_r11,
{ "rsrpResult-r11", "lte-rrc.rsrpResult_r11",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_RSRP_Range_vals_ext, 0,
"RSRP_Range", HFILL }},
{ &hf_lte_rrc_rsrqResult_r11,
{ "rsrqResult-r11", "lte-rrc.rsrqResult_r11",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_RSRQ_Range_vals_ext, 0,
"RSRQ_Range", HFILL }},
{ &hf_lte_rrc_measResultNeighCells_r11,
{ "measResultNeighCells-r11", "lte-rrc.measResultNeighCells_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measResultListEUTRA_r11,
{ "measResultListEUTRA-r11", "lte-rrc.measResultListEUTRA_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultList2EUTRA_r9", HFILL }},
{ &hf_lte_rrc_measResultListUTRA_r11,
{ "measResultListUTRA-r11", "lte-rrc.measResultListUTRA_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultList2UTRA_r9", HFILL }},
{ &hf_lte_rrc_measResultListGERAN_r11,
{ "measResultListGERAN-r11", "lte-rrc.measResultListGERAN_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultListGERAN", HFILL }},
{ &hf_lte_rrc_measResultsCDMA2000_r11,
{ "measResultsCDMA2000-r11", "lte-rrc.measResultsCDMA2000_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultList2CDMA2000_r9", HFILL }},
{ &hf_lte_rrc_numberOfPreamblesSent_r11,
{ "numberOfPreamblesSent-r11", "lte-rrc.numberOfPreamblesSent_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_contentionDetected_r11,
{ "contentionDetected-r11", "lte-rrc.contentionDetected_r11",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_maxTxPowerReached_r11,
{ "maxTxPowerReached-r11", "lte-rrc.maxTxPowerReached_r11",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_measResultListEUTRA_v1130,
{ "measResultListEUTRA-v1130", "lte-rrc.measResultListEUTRA_v1130",
FT_UINT32, BASE_DEC, NULL, 0,
"MeasResultList2EUTRA_v9e0", HFILL }},
{ &hf_lte_rrc_criticalExtensions_36,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_36_vals), 0,
"T_criticalExtensions_36", HFILL }},
{ &hf_lte_rrc_c1_32,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_32_vals), 0,
"T_c1_32", HFILL }},
{ &hf_lte_rrc_ulHandoverPreparationTransfer_r8,
{ "ulHandoverPreparationTransfer-r8", "lte-rrc.ulHandoverPreparationTransfer_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"ULHandoverPreparationTransfer_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_36,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_36", HFILL }},
{ &hf_lte_rrc_meid,
{ "meid", "lte-rrc.meid",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_56", HFILL }},
{ &hf_lte_rrc_dedicatedInfo,
{ "dedicatedInfo", "lte-rrc.dedicatedInfo",
FT_BYTES, BASE_NONE, NULL, 0,
"DedicatedInfoCDMA2000", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_102,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"ULHandoverPreparationTransfer_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_103,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_42", HFILL }},
{ &hf_lte_rrc_criticalExtensions_37,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_37_vals), 0,
"T_criticalExtensions_37", HFILL }},
{ &hf_lte_rrc_c1_33,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_33_vals), 0,
"T_c1_33", HFILL }},
{ &hf_lte_rrc_ulInformationTransfer_r8,
{ "ulInformationTransfer-r8", "lte-rrc.ulInformationTransfer_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"ULInformationTransfer_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_37,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_37", HFILL }},
{ &hf_lte_rrc_dedicatedInfoType_01,
{ "dedicatedInfoType", "lte-rrc.dedicatedInfoType",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_dedicatedInfoType_01_vals), 0,
"T_dedicatedInfoType_01", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_104,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"ULInformationTransfer_v8a0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_105,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_43", HFILL }},
{ &hf_lte_rrc_ac_BarringInfo,
{ "ac-BarringInfo", "lte-rrc.ac_BarringInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ac_BarringForEmergency,
{ "ac-BarringForEmergency", "lte-rrc.ac_BarringForEmergency",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_ac_BarringForMO_Signalling,
{ "ac-BarringForMO-Signalling", "lte-rrc.ac_BarringForMO_Signalling_element",
FT_NONE, BASE_NONE, NULL, 0,
"AC_BarringConfig", HFILL }},
{ &hf_lte_rrc_ac_BarringForMO_Data,
{ "ac-BarringForMO-Data", "lte-rrc.ac_BarringForMO_Data_element",
FT_NONE, BASE_NONE, NULL, 0,
"AC_BarringConfig", HFILL }},
{ &hf_lte_rrc_radioResourceConfigCommon,
{ "radioResourceConfigCommon", "lte-rrc.radioResourceConfigCommon_element",
FT_NONE, BASE_NONE, NULL, 0,
"RadioResourceConfigCommonSIB", HFILL }},
{ &hf_lte_rrc_ue_TimersAndConstants,
{ "ue-TimersAndConstants", "lte-rrc.ue_TimersAndConstants_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_freqInfo,
{ "freqInfo", "lte-rrc.freqInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ul_CarrierFreq,
{ "ul-CarrierFreq", "lte-rrc.ul_CarrierFreq",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA", HFILL }},
{ &hf_lte_rrc_ul_Bandwidth,
{ "ul-Bandwidth", "lte-rrc.ul_Bandwidth",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_ul_Bandwidth_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_additionalSpectrumEmission,
{ "additionalSpectrumEmission", "lte-rrc.additionalSpectrumEmission",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mbsfn_SubframeConfigList,
{ "mbsfn-SubframeConfigList", "lte-rrc.mbsfn_SubframeConfigList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_timeAlignmentTimerCommon,
{ "timeAlignmentTimerCommon", "lte-rrc.timeAlignmentTimerCommon",
FT_UINT32, BASE_DEC, VALS(lte_rrc_TimeAlignmentTimer_vals), 0,
"TimeAlignmentTimer", HFILL }},
{ &hf_lte_rrc_lateNonCriticalExtension_04,
{ "lateNonCriticalExtension", "lte-rrc.lateNonCriticalExtension",
FT_BYTES, BASE_NONE, NULL, 0,
"T_lateNonCriticalExtension_03", HFILL }},
{ &hf_lte_rrc_ssac_BarringForMMTEL_Voice_r9,
{ "ssac-BarringForMMTEL-Voice-r9", "lte-rrc.ssac_BarringForMMTEL_Voice_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"AC_BarringConfig", HFILL }},
{ &hf_lte_rrc_ssac_BarringForMMTEL_Video_r9,
{ "ssac-BarringForMMTEL-Video-r9", "lte-rrc.ssac_BarringForMMTEL_Video_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"AC_BarringConfig", HFILL }},
{ &hf_lte_rrc_ac_BarringForCSFB_r10,
{ "ac-BarringForCSFB-r10", "lte-rrc.ac_BarringForCSFB_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"AC_BarringConfig", HFILL }},
{ &hf_lte_rrc_multiBandInfoList_01,
{ "multiBandInfoList", "lte-rrc.multiBandInfoList",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_maxMultiBands_OF_AdditionalSpectrumEmission", HFILL }},
{ &hf_lte_rrc_multiBandInfoList_item,
{ "AdditionalSpectrumEmission", "lte-rrc.AdditionalSpectrumEmission",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_106,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType2_v9e0_IEs", HFILL }},
{ &hf_lte_rrc_ul_CarrierFreq_v9e0,
{ "ul-CarrierFreq-v9e0", "lte-rrc.ul_CarrierFreq_v9e0",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA_v9e0", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_107,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_44", HFILL }},
{ &hf_lte_rrc_ac_BarringFactor,
{ "ac-BarringFactor", "lte-rrc.ac_BarringFactor",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_ac_BarringFactor_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_ac_BarringTime,
{ "ac-BarringTime", "lte-rrc.ac_BarringTime",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_ac_BarringTime_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_ac_BarringForSpecialAC,
{ "ac-BarringForSpecialAC", "lte-rrc.ac_BarringForSpecialAC",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_5", HFILL }},
{ &hf_lte_rrc_MBSFN_SubframeConfigList_item,
{ "MBSFN-SubframeConfig", "lte-rrc.MBSFN_SubframeConfig_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellReselectionInfoCommon,
{ "cellReselectionInfoCommon", "lte-rrc.cellReselectionInfoCommon_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_q_Hyst,
{ "q-Hyst", "lte-rrc.q_Hyst",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_q_Hyst_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_speedStateReselectionPars,
{ "speedStateReselectionPars", "lte-rrc.speedStateReselectionPars_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mobilityStateParameters,
{ "mobilityStateParameters", "lte-rrc.mobilityStateParameters_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_q_HystSF,
{ "q-HystSF", "lte-rrc.q_HystSF_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf_Medium,
{ "sf-Medium", "lte-rrc.sf_Medium",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_sf_Medium_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf_High,
{ "sf-High", "lte-rrc.sf_High",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_sf_High_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellReselectionServingFreqInfo,
{ "cellReselectionServingFreqInfo", "lte-rrc.cellReselectionServingFreqInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_s_NonIntraSearch,
{ "s-NonIntraSearch", "lte-rrc.s_NonIntraSearch",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_ReselectionThreshold_vals_ext, 0,
"ReselectionThreshold", HFILL }},
{ &hf_lte_rrc_threshServingLow,
{ "threshServingLow", "lte-rrc.threshServingLow",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_ReselectionThreshold_vals_ext, 0,
"ReselectionThreshold", HFILL }},
{ &hf_lte_rrc_intraFreqCellReselectionInfo,
{ "intraFreqCellReselectionInfo", "lte-rrc.intraFreqCellReselectionInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_s_IntraSearch,
{ "s-IntraSearch", "lte-rrc.s_IntraSearch",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_ReselectionThreshold_vals_ext, 0,
"ReselectionThreshold", HFILL }},
{ &hf_lte_rrc_allowedMeasBandwidth,
{ "allowedMeasBandwidth", "lte-rrc.allowedMeasBandwidth",
FT_UINT32, BASE_DEC, VALS(lte_rrc_AllowedMeasBandwidth_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_presenceAntennaPort1,
{ "presenceAntennaPort1", "lte-rrc.presenceAntennaPort1",
FT_BOOLEAN, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_neighCellConfig,
{ "neighCellConfig", "lte-rrc.neighCellConfig",
FT_UINT8, BASE_DEC, VALS(lte_rrc_neighCellConfig_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_t_ReselectionEUTRA,
{ "t-ReselectionEUTRA", "lte-rrc.t_ReselectionEUTRA",
FT_UINT32, BASE_DEC, NULL, 0,
"T_Reselection", HFILL }},
{ &hf_lte_rrc_t_ReselectionEUTRA_SF,
{ "t-ReselectionEUTRA-SF", "lte-rrc.t_ReselectionEUTRA_SF_element",
FT_NONE, BASE_NONE, NULL, 0,
"SpeedStateScaleFactors", HFILL }},
{ &hf_lte_rrc_s_IntraSearch_v920,
{ "s-IntraSearch-v920", "lte-rrc.s_IntraSearch_v920_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_s_IntraSearchP_r9,
{ "s-IntraSearchP-r9", "lte-rrc.s_IntraSearchP_r9",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_ReselectionThreshold_vals_ext, 0,
"ReselectionThreshold", HFILL }},
{ &hf_lte_rrc_s_IntraSearchQ_r9,
{ "s-IntraSearchQ-r9", "lte-rrc.s_IntraSearchQ_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"ReselectionThresholdQ_r9", HFILL }},
{ &hf_lte_rrc_s_NonIntraSearch_v920,
{ "s-NonIntraSearch-v920", "lte-rrc.s_NonIntraSearch_v920_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_s_NonIntraSearchP_r9,
{ "s-NonIntraSearchP-r9", "lte-rrc.s_NonIntraSearchP_r9",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_ReselectionThreshold_vals_ext, 0,
"ReselectionThreshold", HFILL }},
{ &hf_lte_rrc_s_NonIntraSearchQ_r9,
{ "s-NonIntraSearchQ-r9", "lte-rrc.s_NonIntraSearchQ_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"ReselectionThresholdQ_r9", HFILL }},
{ &hf_lte_rrc_threshServingLowQ_r9,
{ "threshServingLowQ-r9", "lte-rrc.threshServingLowQ_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"ReselectionThresholdQ_r9", HFILL }},
{ &hf_lte_rrc_intraFreqNeighCellList,
{ "intraFreqNeighCellList", "lte-rrc.intraFreqNeighCellList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_intraFreqBlackCellList,
{ "intraFreqBlackCellList", "lte-rrc.intraFreqBlackCellList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_csg_PhysCellIdRange,
{ "csg-PhysCellIdRange", "lte-rrc.csg_PhysCellIdRange_element",
FT_NONE, BASE_NONE, NULL, 0,
"PhysCellIdRange", HFILL }},
{ &hf_lte_rrc_IntraFreqNeighCellList_item,
{ "IntraFreqNeighCellInfo", "lte-rrc.IntraFreqNeighCellInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_q_OffsetCell,
{ "q-OffsetCell", "lte-rrc.q_OffsetCell",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_Q_OffsetRange_vals_ext, 0,
"Q_OffsetRange", HFILL }},
{ &hf_lte_rrc_IntraFreqBlackCellList_item,
{ "PhysCellIdRange", "lte-rrc.PhysCellIdRange_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_interFreqCarrierFreqList,
{ "interFreqCarrierFreqList", "lte-rrc.interFreqCarrierFreqList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_lateNonCriticalExtension_05,
{ "lateNonCriticalExtension", "lte-rrc.lateNonCriticalExtension",
FT_BYTES, BASE_NONE, NULL, 0,
"T_lateNonCriticalExtension_04", HFILL }},
{ &hf_lte_rrc_interFreqCarrierFreqList_v8h0,
{ "interFreqCarrierFreqList-v8h0", "lte-rrc.interFreqCarrierFreqList_v8h0",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_maxFreq_OF_InterFreqCarrierFreqInfo_v8h0", HFILL }},
{ &hf_lte_rrc_interFreqCarrierFreqList_v8h0_item,
{ "InterFreqCarrierFreqInfo-v8h0", "lte-rrc.InterFreqCarrierFreqInfo_v8h0_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_108,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType5_v9e0_IEs", HFILL }},
{ &hf_lte_rrc_interFreqCarrierFreqList_v9e0,
{ "interFreqCarrierFreqList-v9e0", "lte-rrc.interFreqCarrierFreqList_v9e0",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_maxFreq_OF_InterFreqCarrierFreqInfo_v9e0", HFILL }},
{ &hf_lte_rrc_interFreqCarrierFreqList_v9e0_item,
{ "InterFreqCarrierFreqInfo-v9e0", "lte-rrc.InterFreqCarrierFreqInfo_v9e0_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_109,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_45", HFILL }},
{ &hf_lte_rrc_InterFreqCarrierFreqList_item,
{ "InterFreqCarrierFreqInfo", "lte-rrc.InterFreqCarrierFreqInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_dl_CarrierFreq,
{ "dl-CarrierFreq", "lte-rrc.dl_CarrierFreq",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA", HFILL }},
{ &hf_lte_rrc_threshX_High,
{ "threshX-High", "lte-rrc.threshX_High",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_ReselectionThreshold_vals_ext, 0,
"ReselectionThreshold", HFILL }},
{ &hf_lte_rrc_threshX_Low,
{ "threshX-Low", "lte-rrc.threshX_Low",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_ReselectionThreshold_vals_ext, 0,
"ReselectionThreshold", HFILL }},
{ &hf_lte_rrc_q_OffsetFreq,
{ "q-OffsetFreq", "lte-rrc.q_OffsetFreq",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_Q_OffsetRange_vals_ext, 0,
"Q_OffsetRange", HFILL }},
{ &hf_lte_rrc_interFreqNeighCellList,
{ "interFreqNeighCellList", "lte-rrc.interFreqNeighCellList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_interFreqBlackCellList,
{ "interFreqBlackCellList", "lte-rrc.interFreqBlackCellList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_threshX_Q_r9,
{ "threshX-Q-r9", "lte-rrc.threshX_Q_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_threshX_HighQ_r9,
{ "threshX-HighQ-r9", "lte-rrc.threshX_HighQ_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"ReselectionThresholdQ_r9", HFILL }},
{ &hf_lte_rrc_threshX_LowQ_r9,
{ "threshX-LowQ-r9", "lte-rrc.threshX_LowQ_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"ReselectionThresholdQ_r9", HFILL }},
{ &hf_lte_rrc_dl_CarrierFreq_v9e0,
{ "dl-CarrierFreq-v9e0", "lte-rrc.dl_CarrierFreq_v9e0",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA_v9e0", HFILL }},
{ &hf_lte_rrc_InterFreqNeighCellList_item,
{ "InterFreqNeighCellInfo", "lte-rrc.InterFreqNeighCellInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_InterFreqBlackCellList_item,
{ "PhysCellIdRange", "lte-rrc.PhysCellIdRange_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_carrierFreqListUTRA_FDD,
{ "carrierFreqListUTRA-FDD", "lte-rrc.carrierFreqListUTRA_FDD",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_carrierFreqListUTRA_TDD,
{ "carrierFreqListUTRA-TDD", "lte-rrc.carrierFreqListUTRA_TDD",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_t_ReselectionUTRA,
{ "t-ReselectionUTRA", "lte-rrc.t_ReselectionUTRA",
FT_UINT32, BASE_DEC, NULL, 0,
"T_Reselection", HFILL }},
{ &hf_lte_rrc_t_ReselectionUTRA_SF,
{ "t-ReselectionUTRA-SF", "lte-rrc.t_ReselectionUTRA_SF_element",
FT_NONE, BASE_NONE, NULL, 0,
"SpeedStateScaleFactors", HFILL }},
{ &hf_lte_rrc_lateNonCriticalExtension_06,
{ "lateNonCriticalExtension", "lte-rrc.lateNonCriticalExtension",
FT_BYTES, BASE_NONE, NULL, 0,
"T_lateNonCriticalExtension_05", HFILL }},
{ &hf_lte_rrc_carrierFreqListUTRA_FDD_v8h0,
{ "carrierFreqListUTRA-FDD-v8h0", "lte-rrc.carrierFreqListUTRA_FDD_v8h0",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_maxUTRA_FDD_Carrier_OF_CarrierFreqInfoUTRA_FDD_v8h0", HFILL }},
{ &hf_lte_rrc_carrierFreqListUTRA_FDD_v8h0_item,
{ "CarrierFreqInfoUTRA-FDD-v8h0", "lte-rrc.CarrierFreqInfoUTRA_FDD_v8h0_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_110,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_46", HFILL }},
{ &hf_lte_rrc_CarrierFreqListUTRA_FDD_item,
{ "CarrierFreqUTRA-FDD", "lte-rrc.CarrierFreqUTRA_FDD_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_q_RxLevMin_01,
{ "q-RxLevMin", "lte-rrc.q_RxLevMin",
FT_INT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_utra_q_RxLevMin_vals_ext, 0,
"INTEGER_M60_M13", HFILL }},
{ &hf_lte_rrc_p_MaxUTRA,
{ "p-MaxUTRA", "lte-rrc.p_MaxUTRA",
FT_INT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_q_QualMin,
{ "q-QualMin", "lte-rrc.q_QualMin",
FT_INT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_threshX_Q_r9_01,
{ "threshX-Q-r9", "lte-rrc.threshX_Q_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_threshX_Q_r9_01", HFILL }},
{ &hf_lte_rrc_multiBandInfoList_02,
{ "multiBandInfoList", "lte-rrc.multiBandInfoList",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_maxMultiBands_OF_FreqBandIndicator_UTRA_FDD", HFILL }},
{ &hf_lte_rrc_multiBandInfoList_item_01,
{ "FreqBandIndicator-UTRA-FDD", "lte-rrc.FreqBandIndicator_UTRA_FDD",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CarrierFreqListUTRA_TDD_item,
{ "CarrierFreqUTRA-TDD", "lte-rrc.CarrierFreqUTRA_TDD_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_p_MaxUTRA_01,
{ "p-MaxUTRA", "lte-rrc.p_MaxUTRA",
FT_INT32, BASE_DEC, NULL, 0,
"T_p_MaxUTRA_01", HFILL }},
{ &hf_lte_rrc_t_ReselectionGERAN,
{ "t-ReselectionGERAN", "lte-rrc.t_ReselectionGERAN",
FT_UINT32, BASE_DEC, NULL, 0,
"T_Reselection", HFILL }},
{ &hf_lte_rrc_t_ReselectionGERAN_SF,
{ "t-ReselectionGERAN-SF", "lte-rrc.t_ReselectionGERAN_SF_element",
FT_NONE, BASE_NONE, NULL, 0,
"SpeedStateScaleFactors", HFILL }},
{ &hf_lte_rrc_carrierFreqsInfoList,
{ "carrierFreqsInfoList", "lte-rrc.carrierFreqsInfoList",
FT_UINT32, BASE_DEC, NULL, 0,
"CarrierFreqsInfoListGERAN", HFILL }},
{ &hf_lte_rrc_CarrierFreqsInfoListGERAN_item,
{ "CarrierFreqsInfoGERAN", "lte-rrc.CarrierFreqsInfoGERAN_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_commonInfo,
{ "commonInfo", "lte-rrc.commonInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ncc_Permitted,
{ "ncc-Permitted", "lte-rrc.ncc_Permitted",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_8", HFILL }},
{ &hf_lte_rrc_q_RxLevMin_02,
{ "q-RxLevMin", "lte-rrc.q_RxLevMin",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_geran_q_RxLevMin_vals_ext, 0,
"INTEGER_0_45", HFILL }},
{ &hf_lte_rrc_p_MaxGERAN,
{ "p-MaxGERAN", "lte-rrc.p_MaxGERAN",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_systemTimeInfo,
{ "systemTimeInfo", "lte-rrc.systemTimeInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemTimeInfoCDMA2000", HFILL }},
{ &hf_lte_rrc_searchWindowSize,
{ "searchWindowSize", "lte-rrc.searchWindowSize",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_15", HFILL }},
{ &hf_lte_rrc_parametersHRPD,
{ "parametersHRPD", "lte-rrc.parametersHRPD_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_preRegistrationInfoHRPD,
{ "preRegistrationInfoHRPD", "lte-rrc.preRegistrationInfoHRPD_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellReselectionParametersHRPD,
{ "cellReselectionParametersHRPD", "lte-rrc.cellReselectionParametersHRPD_element",
FT_NONE, BASE_NONE, NULL, 0,
"CellReselectionParametersCDMA2000", HFILL }},
{ &hf_lte_rrc_parameters1XRTT,
{ "parameters1XRTT", "lte-rrc.parameters1XRTT_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_csfb_RegistrationParam1XRTT,
{ "csfb-RegistrationParam1XRTT", "lte-rrc.csfb_RegistrationParam1XRTT_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_longCodeState1XRTT,
{ "longCodeState1XRTT", "lte-rrc.longCodeState1XRTT",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_42", HFILL }},
{ &hf_lte_rrc_cellReselectionParameters1XRTT,
{ "cellReselectionParameters1XRTT", "lte-rrc.cellReselectionParameters1XRTT_element",
FT_NONE, BASE_NONE, NULL, 0,
"CellReselectionParametersCDMA2000", HFILL }},
{ &hf_lte_rrc_csfb_SupportForDualRxUEs_r9,
{ "csfb-SupportForDualRxUEs-r9", "lte-rrc.csfb_SupportForDualRxUEs_r9",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_cellReselectionParametersHRPD_v920,
{ "cellReselectionParametersHRPD-v920", "lte-rrc.cellReselectionParametersHRPD_v920_element",
FT_NONE, BASE_NONE, NULL, 0,
"CellReselectionParametersCDMA2000_v920", HFILL }},
{ &hf_lte_rrc_cellReselectionParameters1XRTT_v920,
{ "cellReselectionParameters1XRTT-v920", "lte-rrc.cellReselectionParameters1XRTT_v920_element",
FT_NONE, BASE_NONE, NULL, 0,
"CellReselectionParametersCDMA2000_v920", HFILL }},
{ &hf_lte_rrc_csfb_RegistrationParam1XRTT_v920,
{ "csfb-RegistrationParam1XRTT-v920", "lte-rrc.csfb_RegistrationParam1XRTT_v920_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ac_BarringConfig1XRTT_r9,
{ "ac-BarringConfig1XRTT-r9", "lte-rrc.ac_BarringConfig1XRTT_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_csfb_DualRxTxSupport_r10,
{ "csfb-DualRxTxSupport-r10", "lte-rrc.csfb_DualRxTxSupport_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_csfb_DualRxTxSupport_r10_vals), 0,
"T_csfb_DualRxTxSupport_r10", HFILL }},
{ &hf_lte_rrc_sib8_PerPLMN_List_r11,
{ "sib8-PerPLMN-List-r11", "lte-rrc.sib8_PerPLMN_List_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_bandClassList,
{ "bandClassList", "lte-rrc.bandClassList",
FT_UINT32, BASE_DEC, NULL, 0,
"BandClassListCDMA2000", HFILL }},
{ &hf_lte_rrc_neighCellList,
{ "neighCellList", "lte-rrc.neighCellList",
FT_UINT32, BASE_DEC, NULL, 0,
"NeighCellListCDMA2000", HFILL }},
{ &hf_lte_rrc_t_ReselectionCDMA2000,
{ "t-ReselectionCDMA2000", "lte-rrc.t_ReselectionCDMA2000",
FT_UINT32, BASE_DEC, NULL, 0,
"T_Reselection", HFILL }},
{ &hf_lte_rrc_t_ReselectionCDMA2000_SF,
{ "t-ReselectionCDMA2000-SF", "lte-rrc.t_ReselectionCDMA2000_SF_element",
FT_NONE, BASE_NONE, NULL, 0,
"SpeedStateScaleFactors", HFILL }},
{ &hf_lte_rrc_neighCellList_r11,
{ "neighCellList-r11", "lte-rrc.neighCellList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_16_OF_NeighCellCDMA2000_r11", HFILL }},
{ &hf_lte_rrc_neighCellList_r11_item,
{ "NeighCellCDMA2000-r11", "lte-rrc.NeighCellCDMA2000_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_neighCellList_v920,
{ "neighCellList-v920", "lte-rrc.neighCellList_v920",
FT_UINT32, BASE_DEC, NULL, 0,
"NeighCellListCDMA2000_v920", HFILL }},
{ &hf_lte_rrc_NeighCellListCDMA2000_item,
{ "NeighCellCDMA2000", "lte-rrc.NeighCellCDMA2000_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_neighCellsPerFreqList,
{ "neighCellsPerFreqList", "lte-rrc.neighCellsPerFreqList",
FT_UINT32, BASE_DEC, NULL, 0,
"NeighCellsPerBandclassListCDMA2000", HFILL }},
{ &hf_lte_rrc_neighFreqInfoList_r11,
{ "neighFreqInfoList-r11", "lte-rrc.neighFreqInfoList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_16_OF_NeighCellsPerBandclassCDMA2000_r11", HFILL }},
{ &hf_lte_rrc_neighFreqInfoList_r11_item,
{ "NeighCellsPerBandclassCDMA2000-r11", "lte-rrc.NeighCellsPerBandclassCDMA2000_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_NeighCellsPerBandclassListCDMA2000_item,
{ "NeighCellsPerBandclassCDMA2000", "lte-rrc.NeighCellsPerBandclassCDMA2000_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_arfcn,
{ "arfcn", "lte-rrc.arfcn",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueCDMA2000", HFILL }},
{ &hf_lte_rrc_physCellIdList,
{ "physCellIdList", "lte-rrc.physCellIdList",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellIdListCDMA2000", HFILL }},
{ &hf_lte_rrc_physCellIdList_r11,
{ "physCellIdList-r11", "lte-rrc.physCellIdList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_40_OF_PhysCellIdCDMA2000", HFILL }},
{ &hf_lte_rrc_physCellIdList_r11_item,
{ "PhysCellIdCDMA2000", "lte-rrc.PhysCellIdCDMA2000",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_NeighCellListCDMA2000_v920_item,
{ "NeighCellCDMA2000-v920", "lte-rrc.NeighCellCDMA2000_v920_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_neighCellsPerFreqList_v920,
{ "neighCellsPerFreqList-v920", "lte-rrc.neighCellsPerFreqList_v920",
FT_UINT32, BASE_DEC, NULL, 0,
"NeighCellsPerBandclassListCDMA2000_v920", HFILL }},
{ &hf_lte_rrc_NeighCellsPerBandclassListCDMA2000_v920_item,
{ "NeighCellsPerBandclassCDMA2000-v920", "lte-rrc.NeighCellsPerBandclassCDMA2000_v920_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_physCellIdList_v920,
{ "physCellIdList-v920", "lte-rrc.physCellIdList_v920",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellIdListCDMA2000_v920", HFILL }},
{ &hf_lte_rrc_PhysCellIdListCDMA2000_item,
{ "PhysCellIdCDMA2000", "lte-rrc.PhysCellIdCDMA2000",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_PhysCellIdListCDMA2000_v920_item,
{ "PhysCellIdCDMA2000", "lte-rrc.PhysCellIdCDMA2000",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_BandClassListCDMA2000_item,
{ "BandClassInfoCDMA2000", "lte-rrc.BandClassInfoCDMA2000_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_threshX_High_01,
{ "threshX-High", "lte-rrc.threshX_High",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_63", HFILL }},
{ &hf_lte_rrc_threshX_Low_01,
{ "threshX-Low", "lte-rrc.threshX_Low",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_63", HFILL }},
{ &hf_lte_rrc_ac_Barring0to9_r9,
{ "ac-Barring0to9-r9", "lte-rrc.ac_Barring0to9_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_63", HFILL }},
{ &hf_lte_rrc_ac_Barring10_r9,
{ "ac-Barring10-r9", "lte-rrc.ac_Barring10_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_7", HFILL }},
{ &hf_lte_rrc_ac_Barring11_r9,
{ "ac-Barring11-r9", "lte-rrc.ac_Barring11_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_7", HFILL }},
{ &hf_lte_rrc_ac_Barring12_r9,
{ "ac-Barring12-r9", "lte-rrc.ac_Barring12_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_7", HFILL }},
{ &hf_lte_rrc_ac_Barring13_r9,
{ "ac-Barring13-r9", "lte-rrc.ac_Barring13_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_7", HFILL }},
{ &hf_lte_rrc_ac_Barring14_r9,
{ "ac-Barring14-r9", "lte-rrc.ac_Barring14_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_7", HFILL }},
{ &hf_lte_rrc_ac_Barring15_r9,
{ "ac-Barring15-r9", "lte-rrc.ac_Barring15_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_7", HFILL }},
{ &hf_lte_rrc_ac_BarringMsg_r9,
{ "ac-BarringMsg-r9", "lte-rrc.ac_BarringMsg_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_7", HFILL }},
{ &hf_lte_rrc_ac_BarringReg_r9,
{ "ac-BarringReg-r9", "lte-rrc.ac_BarringReg_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_7", HFILL }},
{ &hf_lte_rrc_ac_BarringEmg_r9,
{ "ac-BarringEmg-r9", "lte-rrc.ac_BarringEmg_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_7", HFILL }},
{ &hf_lte_rrc_SIB8_PerPLMN_List_r11_item,
{ "SIB8-PerPLMN-r11", "lte-rrc.SIB8_PerPLMN_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_plmn_Identity_r11,
{ "plmn-Identity-r11", "lte-rrc.plmn_Identity_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_maxPLMN_r11", HFILL }},
{ &hf_lte_rrc_parametersCDMA2000_r11,
{ "parametersCDMA2000-r11", "lte-rrc.parametersCDMA2000_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_parametersCDMA2000_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_explicitValue,
{ "explicitValue", "lte-rrc.explicitValue_element",
FT_NONE, BASE_NONE, NULL, 0,
"ParametersCDMA2000_r11", HFILL }},
{ &hf_lte_rrc_defaultValue,
{ "defaultValue", "lte-rrc.defaultValue_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_systemTimeInfo_r11,
{ "systemTimeInfo-r11", "lte-rrc.systemTimeInfo_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_systemTimeInfo_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_explicitValue_01,
{ "explicitValue", "lte-rrc.explicitValue_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemTimeInfoCDMA2000", HFILL }},
{ &hf_lte_rrc_searchWindowSize_r11,
{ "searchWindowSize-r11", "lte-rrc.searchWindowSize_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_15", HFILL }},
{ &hf_lte_rrc_parametersHRPD_r11,
{ "parametersHRPD-r11", "lte-rrc.parametersHRPD_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_preRegistrationInfoHRPD_r11,
{ "preRegistrationInfoHRPD-r11", "lte-rrc.preRegistrationInfoHRPD_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"PreRegistrationInfoHRPD", HFILL }},
{ &hf_lte_rrc_cellReselectionParametersHRPD_r11,
{ "cellReselectionParametersHRPD-r11", "lte-rrc.cellReselectionParametersHRPD_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"CellReselectionParametersCDMA2000_r11", HFILL }},
{ &hf_lte_rrc_parameters1XRTT_r11,
{ "parameters1XRTT-r11", "lte-rrc.parameters1XRTT_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_parameters1XRTT_r11", HFILL }},
{ &hf_lte_rrc_csfb_RegistrationParam1XRTT_r11,
{ "csfb-RegistrationParam1XRTT-r11", "lte-rrc.csfb_RegistrationParam1XRTT_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"CSFB_RegistrationParam1XRTT", HFILL }},
{ &hf_lte_rrc_csfb_RegistrationParam1XRTT_Ext_r11,
{ "csfb-RegistrationParam1XRTT-Ext-r11", "lte-rrc.csfb_RegistrationParam1XRTT_Ext_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"CSFB_RegistrationParam1XRTT_v920", HFILL }},
{ &hf_lte_rrc_longCodeState1XRTT_r11,
{ "longCodeState1XRTT-r11", "lte-rrc.longCodeState1XRTT_r11",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_42", HFILL }},
{ &hf_lte_rrc_cellReselectionParameters1XRTT_r11,
{ "cellReselectionParameters1XRTT-r11", "lte-rrc.cellReselectionParameters1XRTT_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"CellReselectionParametersCDMA2000_r11", HFILL }},
{ &hf_lte_rrc_ac_BarringConfig1XRTT_r11,
{ "ac-BarringConfig1XRTT-r11", "lte-rrc.ac_BarringConfig1XRTT_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"AC_BarringConfig1XRTT_r9", HFILL }},
{ &hf_lte_rrc_csfb_SupportForDualRxUEs_r11,
{ "csfb-SupportForDualRxUEs-r11", "lte-rrc.csfb_SupportForDualRxUEs_r11",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_csfb_DualRxTxSupport_r11,
{ "csfb-DualRxTxSupport-r11", "lte-rrc.csfb_DualRxTxSupport_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_csfb_DualRxTxSupport_r11_vals), 0,
"T_csfb_DualRxTxSupport_r11", HFILL }},
{ &hf_lte_rrc_hnb_Name,
{ "hnb-Name", "lte-rrc.hnb_Name",
FT_STRING, STR_UNICODE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_messageIdentifier,
{ "messageIdentifier", "lte-rrc.messageIdentifier",
FT_UINT16, BASE_DEC|BASE_EXT_STRING, &lte_rrc_messageIdentifier_vals_ext, 0,
NULL, HFILL }},
{ &hf_lte_rrc_serialNumber,
{ "serialNumber", "lte-rrc.serialNumber",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_warningType,
{ "warningType", "lte-rrc.warningType",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_dummy,
{ "dummy", "lte-rrc.dummy",
FT_BYTES, BASE_NONE, NULL, 0,
"OCTET_STRING_SIZE_50", HFILL }},
{ &hf_lte_rrc_messageIdentifier_01,
{ "messageIdentifier", "lte-rrc.messageIdentifier",
FT_UINT16, BASE_DEC|BASE_EXT_STRING, &lte_rrc_messageIdentifier_vals_ext, 0,
"T_messageIdentifier_01", HFILL }},
{ &hf_lte_rrc_serialNumber_01,
{ "serialNumber", "lte-rrc.serialNumber",
FT_BYTES, BASE_NONE, NULL, 0,
"T_serialNumber_01", HFILL }},
{ &hf_lte_rrc_warningMessageSegmentType,
{ "warningMessageSegmentType", "lte-rrc.warningMessageSegmentType",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_warningMessageSegmentType_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_warningMessageSegmentNumber,
{ "warningMessageSegmentNumber", "lte-rrc.warningMessageSegmentNumber",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_63", HFILL }},
{ &hf_lte_rrc_warningMessageSegment,
{ "warningMessageSegment", "lte-rrc.warningMessageSegment",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_dataCodingScheme,
{ "dataCodingScheme", "lte-rrc.dataCodingScheme",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_messageIdentifier_r9,
{ "messageIdentifier-r9", "lte-rrc.messageIdentifier_r9",
FT_UINT16, BASE_DEC|BASE_EXT_STRING, &lte_rrc_messageIdentifier_vals_ext, 0,
NULL, HFILL }},
{ &hf_lte_rrc_serialNumber_r9,
{ "serialNumber-r9", "lte-rrc.serialNumber_r9",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_warningMessageSegmentType_r9,
{ "warningMessageSegmentType-r9", "lte-rrc.warningMessageSegmentType_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_warningMessageSegmentType_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_warningMessageSegmentNumber_r9,
{ "warningMessageSegmentNumber-r9", "lte-rrc.warningMessageSegmentNumber_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_63", HFILL }},
{ &hf_lte_rrc_warningMessageSegment_r9,
{ "warningMessageSegment-r9", "lte-rrc.warningMessageSegment_r9",
FT_BYTES, BASE_NONE, NULL, 0,
"T_warningMessageSegment_r9", HFILL }},
{ &hf_lte_rrc_dataCodingScheme_r9,
{ "dataCodingScheme-r9", "lte-rrc.dataCodingScheme_r9",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mbsfn_AreaInfoList_r9,
{ "mbsfn-AreaInfoList-r9", "lte-rrc.mbsfn_AreaInfoList_r9",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_notificationConfig_r9,
{ "notificationConfig-r9", "lte-rrc.notificationConfig_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"MBMS_NotificationConfig_r9", HFILL }},
{ &hf_lte_rrc_eab_Param_r11,
{ "eab-Param-r11", "lte-rrc.eab_Param_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_eab_Param_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_eab_Common_r11,
{ "eab-Common-r11", "lte-rrc.eab_Common_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"EAB_Config_r11", HFILL }},
{ &hf_lte_rrc_eab_PerPLMN_List_r11,
{ "eab-PerPLMN-List-r11", "lte-rrc.eab_PerPLMN_List_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_maxPLMN_r11_OF_EAB_ConfigPLMN_r11", HFILL }},
{ &hf_lte_rrc_eab_PerPLMN_List_r11_item,
{ "EAB-ConfigPLMN-r11", "lte-rrc.EAB_ConfigPLMN_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_eab_Config_r11,
{ "eab-Config-r11", "lte-rrc.eab_Config_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_eab_Category_r11,
{ "eab-Category-r11", "lte-rrc.eab_Category_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_eab_Category_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_eab_BarringBitmap_r11,
{ "eab-BarringBitmap-r11", "lte-rrc.eab_BarringBitmap_r11",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_10", HFILL }},
{ &hf_lte_rrc_mbms_SAI_IntraFreq_r11,
{ "mbms-SAI-IntraFreq-r11", "lte-rrc.mbms_SAI_IntraFreq_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"MBMS_SAI_List_r11", HFILL }},
{ &hf_lte_rrc_mbms_SAI_InterFreqList_r11,
{ "mbms-SAI-InterFreqList-r11", "lte-rrc.mbms_SAI_InterFreqList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mbms_SAI_InterFreqList_v1140,
{ "mbms-SAI-InterFreqList-v1140", "lte-rrc.mbms_SAI_InterFreqList_v1140",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MBMS_SAI_List_r11_item,
{ "MBMS-SAI-r11", "lte-rrc.MBMS_SAI_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MBMS_SAI_InterFreqList_r11_item,
{ "MBMS-SAI-InterFreq-r11", "lte-rrc.MBMS_SAI_InterFreq_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MBMS_SAI_InterFreqList_v1140_item,
{ "MBMS-SAI-InterFreq-v1140", "lte-rrc.MBMS_SAI_InterFreq_v1140_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_dl_CarrierFreq_r11,
{ "dl-CarrierFreq-r11", "lte-rrc.dl_CarrierFreq_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA_r9", HFILL }},
{ &hf_lte_rrc_mbms_SAI_List_r11,
{ "mbms-SAI-List-r11", "lte-rrc.mbms_SAI_List_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_multiBandInfoList_r11,
{ "multiBandInfoList-r11", "lte-rrc.multiBandInfoList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_timeInfo_r11,
{ "timeInfo-r11", "lte-rrc.timeInfo_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_timeInfoUTC_r11,
{ "timeInfoUTC-r11", "lte-rrc.timeInfoUTC_r11",
FT_UINT64, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_dayLightSavingTime_r11,
{ "dayLightSavingTime-r11", "lte-rrc.dayLightSavingTime_r11",
FT_UINT8, BASE_DEC, VALS(lte_rrc_daylightSavingTime_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_leapSeconds_r11,
{ "leapSeconds-r11", "lte-rrc.leapSeconds_r11",
FT_INT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_localTimeOffset_r11,
{ "localTimeOffset-r11", "lte-rrc.localTimeOffset_r11",
FT_INT32, BASE_CUSTOM, &lte_rrc_localTimeOffset_fmt, 0,
"INTEGER_M63_64", HFILL }},
{ &hf_lte_rrc_antennaPortsCount,
{ "antennaPortsCount", "lte-rrc.antennaPortsCount",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_antennaPortsCount_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_transmissionMode,
{ "transmissionMode", "lte-rrc.transmissionMode",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_transmissionMode_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_codebookSubsetRestriction,
{ "codebookSubsetRestriction", "lte-rrc.codebookSubsetRestriction",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_codebookSubsetRestriction_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_n2TxAntenna_tm3,
{ "n2TxAntenna-tm3", "lte-rrc.n2TxAntenna_tm3",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_2", HFILL }},
{ &hf_lte_rrc_n4TxAntenna_tm3,
{ "n4TxAntenna-tm3", "lte-rrc.n4TxAntenna_tm3",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_4", HFILL }},
{ &hf_lte_rrc_n2TxAntenna_tm4,
{ "n2TxAntenna-tm4", "lte-rrc.n2TxAntenna_tm4",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_6", HFILL }},
{ &hf_lte_rrc_n4TxAntenna_tm4,
{ "n4TxAntenna-tm4", "lte-rrc.n4TxAntenna_tm4",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_64", HFILL }},
{ &hf_lte_rrc_n2TxAntenna_tm5,
{ "n2TxAntenna-tm5", "lte-rrc.n2TxAntenna_tm5",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_4", HFILL }},
{ &hf_lte_rrc_n4TxAntenna_tm5,
{ "n4TxAntenna-tm5", "lte-rrc.n4TxAntenna_tm5",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_16", HFILL }},
{ &hf_lte_rrc_n2TxAntenna_tm6,
{ "n2TxAntenna-tm6", "lte-rrc.n2TxAntenna_tm6",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_4", HFILL }},
{ &hf_lte_rrc_n4TxAntenna_tm6,
{ "n4TxAntenna-tm6", "lte-rrc.n4TxAntenna_tm6",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_16", HFILL }},
{ &hf_lte_rrc_ue_TransmitAntennaSelection,
{ "ue-TransmitAntennaSelection", "lte-rrc.ue_TransmitAntennaSelection",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_ue_TransmitAntennaSelection_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_release,
{ "release", "lte-rrc.release_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup,
{ "setup", "lte-rrc.setup",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_setup_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_codebookSubsetRestriction_v920,
{ "codebookSubsetRestriction-v920", "lte-rrc.codebookSubsetRestriction_v920",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_codebookSubsetRestriction_v920_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_n2TxAntenna_tm8_r9,
{ "n2TxAntenna-tm8-r9", "lte-rrc.n2TxAntenna_tm8_r9",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_6", HFILL }},
{ &hf_lte_rrc_n4TxAntenna_tm8_r9,
{ "n4TxAntenna-tm8-r9", "lte-rrc.n4TxAntenna_tm8_r9",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_32", HFILL }},
{ &hf_lte_rrc_transmissionMode_r10,
{ "transmissionMode-r10", "lte-rrc.transmissionMode_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_transmissionMode_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_codebookSubsetRestriction_r10,
{ "codebookSubsetRestriction-r10", "lte-rrc.codebookSubsetRestriction_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING", HFILL }},
{ &hf_lte_rrc_ue_TransmitAntennaSelection_01,
{ "ue-TransmitAntennaSelection", "lte-rrc.ue_TransmitAntennaSelection",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_ue_TransmitAntennaSelection_01_vals), 0,
"T_ue_TransmitAntennaSelection_01", HFILL }},
{ &hf_lte_rrc_setup_01,
{ "setup", "lte-rrc.setup",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_setup_01_vals), 0,
"T_setup_01", HFILL }},
{ &hf_lte_rrc_transmissionModeUL_r10,
{ "transmissionModeUL-r10", "lte-rrc.transmissionModeUL_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_transmissionModeUL_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_fourAntennaPortActivated_r10,
{ "fourAntennaPortActivated-r10", "lte-rrc.fourAntennaPortActivated_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_fourAntennaPortActivated_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_cqi_ReportModeAperiodic,
{ "cqi-ReportModeAperiodic", "lte-rrc.cqi_ReportModeAperiodic",
FT_UINT32, BASE_DEC, VALS(lte_rrc_CQI_ReportModeAperiodic_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nomPDSCH_RS_EPRE_Offset,
{ "nomPDSCH-RS-EPRE-Offset", "lte-rrc.nomPDSCH_RS_EPRE_Offset",
FT_INT32, BASE_DEC, VALS(lte_rrc_nomPDSCH_RS_EPRE_Offset_vals), 0,
"INTEGER_M1_6", HFILL }},
{ &hf_lte_rrc_cqi_ReportPeriodic,
{ "cqi-ReportPeriodic", "lte-rrc.cqi_ReportPeriodic",
FT_UINT32, BASE_DEC, VALS(lte_rrc_CQI_ReportPeriodic_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_cqi_Mask_r9,
{ "cqi-Mask-r9", "lte-rrc.cqi_Mask_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cqi_Mask_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_pmi_RI_Report_r9,
{ "pmi-RI-Report-r9", "lte-rrc.pmi_RI_Report_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_pmi_RI_Report_r9_vals), 0,
"T_pmi_RI_Report_r9", HFILL }},
{ &hf_lte_rrc_cqi_ReportAperiodic_r10,
{ "cqi-ReportAperiodic-r10", "lte-rrc.cqi_ReportAperiodic_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_CQI_ReportAperiodic_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_cqi_ReportPeriodic_r10,
{ "cqi-ReportPeriodic-r10", "lte-rrc.cqi_ReportPeriodic_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_CQI_ReportPeriodic_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_pmi_RI_Report_r9_01,
{ "pmi-RI-Report-r9", "lte-rrc.pmi_RI_Report_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_pmi_RI_Report_r9_01_vals), 0,
"T_pmi_RI_Report_r9_01", HFILL }},
{ &hf_lte_rrc_csi_SubframePatternConfig_r10,
{ "csi-SubframePatternConfig-r10", "lte-rrc.csi_SubframePatternConfig_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_csi_SubframePatternConfig_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_02,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_02", HFILL }},
{ &hf_lte_rrc_csi_MeasSubframeSet1_r10,
{ "csi-MeasSubframeSet1-r10", "lte-rrc.csi_MeasSubframeSet1_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_MeasSubframePattern_r10_vals), 0,
"MeasSubframePattern_r10", HFILL }},
{ &hf_lte_rrc_csi_MeasSubframeSet2_r10,
{ "csi-MeasSubframeSet2-r10", "lte-rrc.csi_MeasSubframeSet2_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_MeasSubframePattern_r10_vals), 0,
"MeasSubframePattern_r10", HFILL }},
{ &hf_lte_rrc_cqi_ReportPeriodic_v1130,
{ "cqi-ReportPeriodic-v1130", "lte-rrc.cqi_ReportPeriodic_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cqi_ReportBoth_r11,
{ "cqi-ReportBoth-r11", "lte-rrc.cqi_ReportBoth_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cqi_ReportModeAperiodic_r10,
{ "cqi-ReportModeAperiodic-r10", "lte-rrc.cqi_ReportModeAperiodic_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_CQI_ReportModeAperiodic_vals), 0,
"CQI_ReportModeAperiodic", HFILL }},
{ &hf_lte_rrc_nomPDSCH_RS_EPRE_Offset_r10,
{ "nomPDSCH-RS-EPRE-Offset-r10", "lte-rrc.nomPDSCH_RS_EPRE_Offset_r10",
FT_INT32, BASE_DEC, VALS(lte_rrc_nomPDSCH_RS_EPRE_Offset_vals), 0,
"INTEGER_M1_6", HFILL }},
{ &hf_lte_rrc_cqi_ReportPeriodicSCell_r10,
{ "cqi-ReportPeriodicSCell-r10", "lte-rrc.cqi_ReportPeriodicSCell_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_CQI_ReportPeriodic_r10_vals), 0,
"CQI_ReportPeriodic_r10", HFILL }},
{ &hf_lte_rrc_pmi_RI_Report_r10,
{ "pmi-RI-Report-r10", "lte-rrc.pmi_RI_Report_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_pmi_RI_Report_r10_vals), 0,
"T_pmi_RI_Report_r10", HFILL }},
{ &hf_lte_rrc_setup_03,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_03", HFILL }},
{ &hf_lte_rrc_cqi_PUCCH_ResourceIndex,
{ "cqi-PUCCH-ResourceIndex", "lte-rrc.cqi_PUCCH_ResourceIndex",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_1185", HFILL }},
{ &hf_lte_rrc_cqi_pmi_ConfigIndex,
{ "cqi-pmi-ConfigIndex", "lte-rrc.cqi_pmi_ConfigIndex",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_1023", HFILL }},
{ &hf_lte_rrc_cqi_FormatIndicatorPeriodic,
{ "cqi-FormatIndicatorPeriodic", "lte-rrc.cqi_FormatIndicatorPeriodic",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cqi_FormatIndicatorPeriodic_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_widebandCQI,
{ "widebandCQI", "lte-rrc.widebandCQI_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_subbandCQI,
{ "subbandCQI", "lte-rrc.subbandCQI_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_k,
{ "k", "lte-rrc.k",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_4", HFILL }},
{ &hf_lte_rrc_ri_ConfigIndex,
{ "ri-ConfigIndex", "lte-rrc.ri_ConfigIndex",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_1023", HFILL }},
{ &hf_lte_rrc_simultaneousAckNackAndCQI,
{ "simultaneousAckNackAndCQI", "lte-rrc.simultaneousAckNackAndCQI",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_setup_04,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_04", HFILL }},
{ &hf_lte_rrc_cqi_PUCCH_ResourceIndex_r10,
{ "cqi-PUCCH-ResourceIndex-r10", "lte-rrc.cqi_PUCCH_ResourceIndex_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_1184", HFILL }},
{ &hf_lte_rrc_cqi_PUCCH_ResourceIndexP1_r10,
{ "cqi-PUCCH-ResourceIndexP1-r10", "lte-rrc.cqi_PUCCH_ResourceIndexP1_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_1184", HFILL }},
{ &hf_lte_rrc_cqi_FormatIndicatorPeriodic_r10,
{ "cqi-FormatIndicatorPeriodic-r10", "lte-rrc.cqi_FormatIndicatorPeriodic_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cqi_FormatIndicatorPeriodic_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_widebandCQI_r10,
{ "widebandCQI-r10", "lte-rrc.widebandCQI_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_csi_ReportMode_r10,
{ "csi-ReportMode-r10", "lte-rrc.csi_ReportMode_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_csi_ReportMode_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_subbandCQI_r10,
{ "subbandCQI-r10", "lte-rrc.subbandCQI_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_periodicityFactor_r10,
{ "periodicityFactor-r10", "lte-rrc.periodicityFactor_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_periodicityFactor_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_cqi_Mask_r9_01,
{ "cqi-Mask-r9", "lte-rrc.cqi_Mask_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cqi_Mask_r9_01_vals), 0,
"T_cqi_Mask_r9_01", HFILL }},
{ &hf_lte_rrc_csi_ConfigIndex_r10,
{ "csi-ConfigIndex-r10", "lte-rrc.csi_ConfigIndex_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_csi_ConfigIndex_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_05,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_05", HFILL }},
{ &hf_lte_rrc_cqi_pmi_ConfigIndex2_r10,
{ "cqi-pmi-ConfigIndex2-r10", "lte-rrc.cqi_pmi_ConfigIndex2_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_1023", HFILL }},
{ &hf_lte_rrc_ri_ConfigIndex2_r10,
{ "ri-ConfigIndex2-r10", "lte-rrc.ri_ConfigIndex2_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_1023", HFILL }},
{ &hf_lte_rrc_simultaneousAckNackAndCQI_Format3_r11,
{ "simultaneousAckNackAndCQI-Format3-r11", "lte-rrc.simultaneousAckNackAndCQI_Format3_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_simultaneousAckNackAndCQI_Format3_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_cqi_ReportPeriodicProcExtToReleaseList_r11,
{ "cqi-ReportPeriodicProcExtToReleaseList-r11", "lte-rrc.cqi_ReportPeriodicProcExtToReleaseList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cqi_ReportPeriodicProcExtToAddModList_r11,
{ "cqi-ReportPeriodicProcExtToAddModList-r11", "lte-rrc.cqi_ReportPeriodicProcExtToAddModList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CQI_ReportPeriodicProcExtToAddModList_r11_item,
{ "CQI-ReportPeriodicProcExt-r11", "lte-rrc.CQI_ReportPeriodicProcExt_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CQI_ReportPeriodicProcExtToReleaseList_r11_item,
{ "CQI-ReportPeriodicProcExtId-r11", "lte-rrc.CQI_ReportPeriodicProcExtId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cqi_ReportPeriodicProcExtId_r11,
{ "cqi-ReportPeriodicProcExtId-r11", "lte-rrc.cqi_ReportPeriodicProcExtId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cqi_pmi_ConfigIndex_r11,
{ "cqi-pmi-ConfigIndex-r11", "lte-rrc.cqi_pmi_ConfigIndex_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_1023", HFILL }},
{ &hf_lte_rrc_cqi_FormatIndicatorPeriodic_r11,
{ "cqi-FormatIndicatorPeriodic-r11", "lte-rrc.cqi_FormatIndicatorPeriodic_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cqi_FormatIndicatorPeriodic_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_widebandCQI_r11,
{ "widebandCQI-r11", "lte-rrc.widebandCQI_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_csi_ReportMode_r11,
{ "csi-ReportMode-r11", "lte-rrc.csi_ReportMode_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_csi_ReportMode_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_subbandCQI_r11,
{ "subbandCQI-r11", "lte-rrc.subbandCQI_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_periodicityFactor_r11,
{ "periodicityFactor-r11", "lte-rrc.periodicityFactor_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_periodicityFactor_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_ri_ConfigIndex_r11,
{ "ri-ConfigIndex-r11", "lte-rrc.ri_ConfigIndex_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_1023", HFILL }},
{ &hf_lte_rrc_csi_ConfigIndex_r11,
{ "csi-ConfigIndex-r11", "lte-rrc.csi_ConfigIndex_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_csi_ConfigIndex_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_06,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_06", HFILL }},
{ &hf_lte_rrc_cqi_pmi_ConfigIndex2_r11,
{ "cqi-pmi-ConfigIndex2-r11", "lte-rrc.cqi_pmi_ConfigIndex2_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_1023", HFILL }},
{ &hf_lte_rrc_ri_ConfigIndex2_r11,
{ "ri-ConfigIndex2-r11", "lte-rrc.ri_ConfigIndex2_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_1023", HFILL }},
{ &hf_lte_rrc_setup_07,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_07", HFILL }},
{ &hf_lte_rrc_aperiodicCSI_Trigger_r10,
{ "aperiodicCSI-Trigger-r10", "lte-rrc.aperiodicCSI_Trigger_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_trigger1_r10,
{ "trigger1-r10", "lte-rrc.trigger1_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_8", HFILL }},
{ &hf_lte_rrc_trigger2_r10,
{ "trigger2-r10", "lte-rrc.trigger2_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_8", HFILL }},
{ &hf_lte_rrc_cqi_ReportModeAperiodic_r11,
{ "cqi-ReportModeAperiodic-r11", "lte-rrc.cqi_ReportModeAperiodic_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_CQI_ReportModeAperiodic_vals), 0,
"CQI_ReportModeAperiodic", HFILL }},
{ &hf_lte_rrc_trigger01_r11,
{ "trigger01-r11", "lte-rrc.trigger01_r11",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_trigger10_r11,
{ "trigger10-r11", "lte-rrc.trigger10_r11",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_trigger11_r11,
{ "trigger11-r11", "lte-rrc.trigger11_r11",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_csi_IM_ConfigToReleaseList_r11,
{ "csi-IM-ConfigToReleaseList-r11", "lte-rrc.csi_IM_ConfigToReleaseList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_csi_IM_ConfigToAddModList_r11,
{ "csi-IM-ConfigToAddModList-r11", "lte-rrc.csi_IM_ConfigToAddModList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_csi_ProcessToReleaseList_r11,
{ "csi-ProcessToReleaseList-r11", "lte-rrc.csi_ProcessToReleaseList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_csi_ProcessToAddModList_r11,
{ "csi-ProcessToAddModList-r11", "lte-rrc.csi_ProcessToAddModList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CSI_IM_ConfigToAddModList_r11_item,
{ "CSI-IM-Config-r11", "lte-rrc.CSI_IM_Config_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CSI_IM_ConfigToReleaseList_r11_item,
{ "CSI-IM-ConfigId-r11", "lte-rrc.CSI_IM_ConfigId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CSI_ProcessToAddModList_r11_item,
{ "CSI-Process-r11", "lte-rrc.CSI_Process_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CSI_ProcessToReleaseList_r11_item,
{ "CSI-ProcessId-r11", "lte-rrc.CSI_ProcessId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ri_Ref_CSI_ProcessId_r11,
{ "ri-Ref-CSI-ProcessId-r11", "lte-rrc.ri_Ref_CSI_ProcessId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"CSI_ProcessId_r11", HFILL }},
{ &hf_lte_rrc_pmi_RI_Report_r11,
{ "pmi-RI-Report-r11", "lte-rrc.pmi_RI_Report_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_pmi_RI_Report_r11_vals), 0,
"T_pmi_RI_Report_r11", HFILL }},
{ &hf_lte_rrc_schedulingCellInfo_r10,
{ "schedulingCellInfo-r10", "lte-rrc.schedulingCellInfo_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_schedulingCellInfo_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_own_r10,
{ "own-r10", "lte-rrc.own_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cif_Presence_r10,
{ "cif-Presence-r10", "lte-rrc.cif_Presence_r10",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_other_r10,
{ "other-r10", "lte-rrc.other_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_schedulingCellId_r10,
{ "schedulingCellId-r10", "lte-rrc.schedulingCellId_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"ServCellIndex_r10", HFILL }},
{ &hf_lte_rrc_pdsch_Start_r10,
{ "pdsch-Start-r10", "lte-rrc.pdsch_Start_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_4", HFILL }},
{ &hf_lte_rrc_csi_IM_ConfigId_r11,
{ "csi-IM-ConfigId-r11", "lte-rrc.csi_IM_ConfigId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_resourceConfig_r11,
{ "resourceConfig-r11", "lte-rrc.resourceConfig_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_31", HFILL }},
{ &hf_lte_rrc_subframeConfig_r11,
{ "subframeConfig-r11", "lte-rrc.subframeConfig_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_154", HFILL }},
{ &hf_lte_rrc_csi_ProcessId_r11,
{ "csi-ProcessId-r11", "lte-rrc.csi_ProcessId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_csi_RS_ConfigNZPId_r11,
{ "csi-RS-ConfigNZPId-r11", "lte-rrc.csi_RS_ConfigNZPId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_p_C_AndCBSRList_r11,
{ "p-C-AndCBSRList-r11", "lte-rrc.p_C_AndCBSRList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_2_OF_P_C_AndCBSR_r11", HFILL }},
{ &hf_lte_rrc_p_C_AndCBSRList_r11_item,
{ "P-C-AndCBSR-r11", "lte-rrc.P_C_AndCBSR_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cqi_ReportBothProc_r11,
{ "cqi-ReportBothProc-r11", "lte-rrc.cqi_ReportBothProc_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cqi_ReportPeriodicProcId_r11,
{ "cqi-ReportPeriodicProcId-r11", "lte-rrc.cqi_ReportPeriodicProcId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_maxCQI_ProcExt_r11", HFILL }},
{ &hf_lte_rrc_cqi_ReportAperiodicProc_r11,
{ "cqi-ReportAperiodicProc-r11", "lte-rrc.cqi_ReportAperiodicProc_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_p_C_r11,
{ "p-C-r11", "lte-rrc.p_C_r11",
FT_INT32, BASE_DEC, NULL, 0,
"INTEGER_M8_15", HFILL }},
{ &hf_lte_rrc_codebookSubsetRestriction_r11,
{ "codebookSubsetRestriction-r11", "lte-rrc.codebookSubsetRestriction_r11",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING", HFILL }},
{ &hf_lte_rrc_csi_RS_r10,
{ "csi-RS-r10", "lte-rrc.csi_RS_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_csi_RS_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_08,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_08", HFILL }},
{ &hf_lte_rrc_antennaPortsCount_r10,
{ "antennaPortsCount-r10", "lte-rrc.antennaPortsCount_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_antennaPortsCount_r10_vals), 0,
"T_antennaPortsCount_r10", HFILL }},
{ &hf_lte_rrc_resourceConfig_r10,
{ "resourceConfig-r10", "lte-rrc.resourceConfig_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_31", HFILL }},
{ &hf_lte_rrc_subframeConfig_r10,
{ "subframeConfig-r10", "lte-rrc.subframeConfig_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_154", HFILL }},
{ &hf_lte_rrc_p_C_r10,
{ "p-C-r10", "lte-rrc.p_C_r10",
FT_INT32, BASE_DEC, NULL, 0,
"INTEGER_M8_15", HFILL }},
{ &hf_lte_rrc_zeroTxPowerCSI_RS_r10,
{ "zeroTxPowerCSI-RS-r10", "lte-rrc.zeroTxPowerCSI_RS_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_zeroTxPowerCSI_RS_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_09,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_09", HFILL }},
{ &hf_lte_rrc_zeroTxPowerResourceConfigList_r10,
{ "zeroTxPowerResourceConfigList-r10", "lte-rrc.zeroTxPowerResourceConfigList_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_16", HFILL }},
{ &hf_lte_rrc_zeroTxPowerSubframeConfig_r10,
{ "zeroTxPowerSubframeConfig-r10", "lte-rrc.zeroTxPowerSubframeConfig_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_154", HFILL }},
{ &hf_lte_rrc_antennaPortsCount_r11,
{ "antennaPortsCount-r11", "lte-rrc.antennaPortsCount_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_antennaPortsCount_r11_vals), 0,
"T_antennaPortsCount_r11", HFILL }},
{ &hf_lte_rrc_scramblingIdentity_r11,
{ "scramblingIdentity-r11", "lte-rrc.scramblingIdentity_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_503", HFILL }},
{ &hf_lte_rrc_qcl_CRS_Info_r11,
{ "qcl-CRS-Info-r11", "lte-rrc.qcl_CRS_Info_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_qcl_ScramblingIdentity_r11,
{ "qcl-ScramblingIdentity-r11", "lte-rrc.qcl_ScramblingIdentity_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_503", HFILL }},
{ &hf_lte_rrc_crs_PortsCount_r11,
{ "crs-PortsCount-r11", "lte-rrc.crs_PortsCount_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_crs_PortsCount_r11_vals), 0,
"T_crs_PortsCount_r11", HFILL }},
{ &hf_lte_rrc_mbsfn_SubframeConfigList_r11,
{ "mbsfn-SubframeConfigList-r11", "lte-rrc.mbsfn_SubframeConfigList_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_mbsfn_SubframeConfigList_r11_vals), 0,
"T_mbsfn_SubframeConfigList_r11", HFILL }},
{ &hf_lte_rrc_setup_10,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_10", HFILL }},
{ &hf_lte_rrc_subframeConfigList,
{ "subframeConfigList", "lte-rrc.subframeConfigList",
FT_UINT32, BASE_DEC, NULL, 0,
"MBSFN_SubframeConfigList", HFILL }},
{ &hf_lte_rrc_csi_RS_ConfigZPId_r11,
{ "csi-RS-ConfigZPId-r11", "lte-rrc.csi_RS_ConfigZPId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_resourceConfigList_r11,
{ "resourceConfigList-r11", "lte-rrc.resourceConfigList_r11",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_16", HFILL }},
{ &hf_lte_rrc_setup_11,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_11", HFILL }},
{ &hf_lte_rrc_scramblingIdentity2_r11,
{ "scramblingIdentity2-r11", "lte-rrc.scramblingIdentity2_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_503", HFILL }},
{ &hf_lte_rrc_config_r11,
{ "config-r11", "lte-rrc.config_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_config_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_12,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_12", HFILL }},
{ &hf_lte_rrc_subframePatternConfig_r11,
{ "subframePatternConfig-r11", "lte-rrc.subframePatternConfig_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_subframePatternConfig_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_13,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_13", HFILL }},
{ &hf_lte_rrc_subframePattern_r11,
{ "subframePattern-r11", "lte-rrc.subframePattern_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_MeasSubframePattern_r10_vals), 0,
"MeasSubframePattern_r10", HFILL }},
{ &hf_lte_rrc_startSymbol_r11,
{ "startSymbol-r11", "lte-rrc.startSymbol_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_4", HFILL }},
{ &hf_lte_rrc_setConfigToReleaseList_r11,
{ "setConfigToReleaseList-r11", "lte-rrc.setConfigToReleaseList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"EPDCCH_SetConfigToReleaseList_r11", HFILL }},
{ &hf_lte_rrc_setConfigToAddModList_r11,
{ "setConfigToAddModList-r11", "lte-rrc.setConfigToAddModList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"EPDCCH_SetConfigToAddModList_r11", HFILL }},
{ &hf_lte_rrc_EPDCCH_SetConfigToAddModList_r11_item,
{ "EPDCCH-SetConfig-r11", "lte-rrc.EPDCCH_SetConfig_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_EPDCCH_SetConfigToReleaseList_r11_item,
{ "EPDCCH-SetConfigId-r11", "lte-rrc.EPDCCH_SetConfigId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_setConfigId_r11,
{ "setConfigId-r11", "lte-rrc.setConfigId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"EPDCCH_SetConfigId_r11", HFILL }},
{ &hf_lte_rrc_transmissionType_r11,
{ "transmissionType-r11", "lte-rrc.transmissionType_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_transmissionType_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_resourceBlockAssignment_r11,
{ "resourceBlockAssignment-r11", "lte-rrc.resourceBlockAssignment_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_resourceBlockAssignment_r11", HFILL }},
{ &hf_lte_rrc_numberPRB_Pairs_r11,
{ "numberPRB-Pairs-r11", "lte-rrc.numberPRB_Pairs_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_numberPRB_Pairs_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_resourceBlockAssignment_r11_01,
{ "resourceBlockAssignment-r11", "lte-rrc.resourceBlockAssignment_r11",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_4_38", HFILL }},
{ &hf_lte_rrc_dmrs_ScramblingSequenceInt_r11,
{ "dmrs-ScramblingSequenceInt-r11", "lte-rrc.dmrs_ScramblingSequenceInt_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_503", HFILL }},
{ &hf_lte_rrc_pucch_ResourceStartOffset_r11,
{ "pucch-ResourceStartOffset-r11", "lte-rrc.pucch_ResourceStartOffset_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_2047", HFILL }},
{ &hf_lte_rrc_re_MappingQCL_ConfigId_r11,
{ "re-MappingQCL-ConfigId-r11", "lte-rrc.re_MappingQCL_ConfigId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"PDSCH_RE_MappingQCL_ConfigId_r11", HFILL }},
{ &hf_lte_rrc_ul_SpecificParameters,
{ "ul-SpecificParameters", "lte-rrc.ul_SpecificParameters_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_priority,
{ "priority", "lte-rrc.priority",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_prioritisedBitRate,
{ "prioritisedBitRate", "lte-rrc.prioritisedBitRate",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_prioritisedBitRate_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_bucketSizeDuration,
{ "bucketSizeDuration", "lte-rrc.bucketSizeDuration",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_bucketSizeDuration_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_logicalChannelGroup,
{ "logicalChannelGroup", "lte-rrc.logicalChannelGroup",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_3", HFILL }},
{ &hf_lte_rrc_logicalChannelSR_Mask_r9,
{ "logicalChannelSR-Mask-r9", "lte-rrc.logicalChannelSR_Mask_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_logicalChannelSR_Mask_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_ul_SCH_Config,
{ "ul-SCH-Config", "lte-rrc.ul_SCH_Config_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_maxHARQ_Tx,
{ "maxHARQ-Tx", "lte-rrc.maxHARQ_Tx",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_maxHARQ_Tx_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_periodicBSR_Timer,
{ "periodicBSR-Timer", "lte-rrc.periodicBSR_Timer",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_periodicBSR_Timer_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_retxBSR_Timer,
{ "retxBSR-Timer", "lte-rrc.retxBSR_Timer",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_retxBSR_Timer_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_ttiBundling,
{ "ttiBundling", "lte-rrc.ttiBundling",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_drx_Config,
{ "drx-Config", "lte-rrc.drx_Config",
FT_UINT32, BASE_DEC, VALS(lte_rrc_DRX_Config_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_timeAlignmentTimerDedicated,
{ "timeAlignmentTimerDedicated", "lte-rrc.timeAlignmentTimerDedicated",
FT_UINT32, BASE_DEC, VALS(lte_rrc_TimeAlignmentTimer_vals), 0,
"TimeAlignmentTimer", HFILL }},
{ &hf_lte_rrc_phr_Config,
{ "phr-Config", "lte-rrc.phr_Config",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_phr_Config_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_14,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_14", HFILL }},
{ &hf_lte_rrc_periodicPHR_Timer,
{ "periodicPHR-Timer", "lte-rrc.periodicPHR_Timer",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_periodicPHR_Timer_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_prohibitPHR_Timer,
{ "prohibitPHR-Timer", "lte-rrc.prohibitPHR_Timer",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_prohibitPHR_Timer_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_dl_PathlossChange,
{ "dl-PathlossChange", "lte-rrc.dl_PathlossChange",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_dl_PathlossChange_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_sr_ProhibitTimer_r9,
{ "sr-ProhibitTimer-r9", "lte-rrc.sr_ProhibitTimer_r9",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mac_MainConfig_v1020,
{ "mac-MainConfig-v1020", "lte-rrc.mac_MainConfig_v1020_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sCellDeactivationTimer_r10,
{ "sCellDeactivationTimer-r10", "lte-rrc.sCellDeactivationTimer_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_sCellDeactivationTimer_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_extendedBSR_Sizes_r10,
{ "extendedBSR-Sizes-r10", "lte-rrc.extendedBSR_Sizes_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_extendedBSR_Sizes_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_extendedPHR_r10,
{ "extendedPHR-r10", "lte-rrc.extendedPHR_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_extendedPHR_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_stag_ToReleaseList_r11,
{ "stag-ToReleaseList-r11", "lte-rrc.stag_ToReleaseList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_stag_ToAddModList_r11,
{ "stag-ToAddModList-r11", "lte-rrc.stag_ToAddModList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_drx_Config_v1130,
{ "drx-Config-v1130", "lte-rrc.drx_Config_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_stag_Id_r11,
{ "stag-Id-r11", "lte-rrc.stag_Id_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_release_01,
{ "release", "lte-rrc.release_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_15,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_15", HFILL }},
{ &hf_lte_rrc_onDurationTimer,
{ "onDurationTimer", "lte-rrc.onDurationTimer",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_onDurationTimer_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_drx_InactivityTimer,
{ "drx-InactivityTimer", "lte-rrc.drx_InactivityTimer",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_T_drx_InactivityTimer_vals_ext, 0,
NULL, HFILL }},
{ &hf_lte_rrc_drx_RetransmissionTimer,
{ "drx-RetransmissionTimer", "lte-rrc.drx_RetransmissionTimer",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_drx_RetransmissionTimer_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_longDRX_CycleStartOffset,
{ "longDRX-CycleStartOffset", "lte-rrc.longDRX_CycleStartOffset",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_longDRX_CycleStartOffset_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf10,
{ "sf10", "lte-rrc.sf10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf20,
{ "sf20", "lte-rrc.sf20",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf32,
{ "sf32", "lte-rrc.sf32",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf40,
{ "sf40", "lte-rrc.sf40",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf64,
{ "sf64", "lte-rrc.sf64",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf80,
{ "sf80", "lte-rrc.sf80",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf128,
{ "sf128", "lte-rrc.sf128",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf160,
{ "sf160", "lte-rrc.sf160",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf256,
{ "sf256", "lte-rrc.sf256",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf320,
{ "sf320", "lte-rrc.sf320",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf512,
{ "sf512", "lte-rrc.sf512",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf640,
{ "sf640", "lte-rrc.sf640",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf1024,
{ "sf1024", "lte-rrc.sf1024",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf1280,
{ "sf1280", "lte-rrc.sf1280",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf2048,
{ "sf2048", "lte-rrc.sf2048",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf2560,
{ "sf2560", "lte-rrc.sf2560",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_shortDRX,
{ "shortDRX", "lte-rrc.shortDRX_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_shortDRX_Cycle,
{ "shortDRX-Cycle", "lte-rrc.shortDRX_Cycle",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_shortDRX_Cycle_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_drxShortCycleTimer,
{ "drxShortCycleTimer", "lte-rrc.drxShortCycleTimer",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_drx_RetransmissionTimer_v1130,
{ "drx-RetransmissionTimer-v1130", "lte-rrc.drx_RetransmissionTimer_v1130",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_drx_RetransmissionTimer_v1130_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_longDRX_CycleStartOffset_v1130,
{ "longDRX-CycleStartOffset-v1130", "lte-rrc.longDRX_CycleStartOffset_v1130",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_longDRX_CycleStartOffset_v1130_vals), 0,
"T_longDRX_CycleStartOffset_v1130", HFILL }},
{ &hf_lte_rrc_sf60_v1130,
{ "sf60-v1130", "lte-rrc.sf60_v1130",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf70_v1130,
{ "sf70-v1130", "lte-rrc.sf70_v1130",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_shortDRX_Cycle_v1130,
{ "shortDRX-Cycle-v1130", "lte-rrc.shortDRX_Cycle_v1130",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_shortDRX_Cycle_v1130_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_STAG_ToReleaseList_r11_item,
{ "STAG-Id-r11", "lte-rrc.STAG_Id_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_STAG_ToAddModList_r11_item,
{ "STAG-ToAddMod-r11", "lte-rrc.STAG_ToAddMod_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_timeAlignmentTimerSTAG_r11,
{ "timeAlignmentTimerSTAG-r11", "lte-rrc.timeAlignmentTimerSTAG_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_TimeAlignmentTimer_vals), 0,
"TimeAlignmentTimer", HFILL }},
{ &hf_lte_rrc_discardTimer,
{ "discardTimer", "lte-rrc.discardTimer",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_discardTimer_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_rlc_AM,
{ "rlc-AM", "lte-rrc.rlc_AM_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_statusReportRequired,
{ "statusReportRequired", "lte-rrc.statusReportRequired",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_rlc_UM,
{ "rlc-UM", "lte-rrc.rlc_UM_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pdcp_SN_Size,
{ "pdcp-SN-Size", "lte-rrc.pdcp_SN_Size",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_pdcp_SN_Size_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_headerCompression,
{ "headerCompression", "lte-rrc.headerCompression",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_headerCompression_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_notUsed,
{ "notUsed", "lte-rrc.notUsed_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rohc,
{ "rohc", "lte-rrc.rohc_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_maxCID,
{ "maxCID", "lte-rrc.maxCID",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_16383", HFILL }},
{ &hf_lte_rrc_profiles,
{ "profiles", "lte-rrc.profiles_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_profile0x0001,
{ "profile0x0001", "lte-rrc.profile0x0001",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_profile0x0002,
{ "profile0x0002", "lte-rrc.profile0x0002",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_profile0x0003,
{ "profile0x0003", "lte-rrc.profile0x0003",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_profile0x0004,
{ "profile0x0004", "lte-rrc.profile0x0004",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_profile0x0006,
{ "profile0x0006", "lte-rrc.profile0x0006",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_profile0x0101,
{ "profile0x0101", "lte-rrc.profile0x0101",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_profile0x0102,
{ "profile0x0102", "lte-rrc.profile0x0102",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_profile0x0103,
{ "profile0x0103", "lte-rrc.profile0x0103",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_profile0x0104,
{ "profile0x0104", "lte-rrc.profile0x0104",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_rn_IntegrityProtection_r10,
{ "rn-IntegrityProtection-r10", "lte-rrc.rn_IntegrityProtection_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_rn_IntegrityProtection_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_pdcp_SN_Size_v1130,
{ "pdcp-SN-Size-v1130", "lte-rrc.pdcp_SN_Size_v1130",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_pdcp_SN_Size_v1130_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_referenceSignalPower,
{ "referenceSignalPower", "lte-rrc.referenceSignalPower",
FT_INT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_p_b,
{ "p-b", "lte-rrc.p_b",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_3", HFILL }},
{ &hf_lte_rrc_p_a,
{ "p-a", "lte-rrc.p_a",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_p_a_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_dmrs_ConfigPDSCH_r11,
{ "dmrs-ConfigPDSCH-r11", "lte-rrc.dmrs_ConfigPDSCH_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_DMRS_Config_r11_vals), 0,
"DMRS_Config_r11", HFILL }},
{ &hf_lte_rrc_qcl_Operation,
{ "qcl-Operation", "lte-rrc.qcl_Operation",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_qcl_Operation_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_re_MappingQCLConfigToReleaseList_r11,
{ "re-MappingQCLConfigToReleaseList-r11", "lte-rrc.re_MappingQCLConfigToReleaseList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_re_MappingQCLConfigToAddModList_r11,
{ "re-MappingQCLConfigToAddModList-r11", "lte-rrc.re_MappingQCLConfigToAddModList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_RE_MappingQCLConfigToAddModList_r11_item,
{ "PDSCH-RE-MappingQCL-Config-r11", "lte-rrc.PDSCH_RE_MappingQCL_Config_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_RE_MappingQCLConfigToReleaseList_r11_item,
{ "PDSCH-RE-MappingQCL-ConfigId-r11", "lte-rrc.PDSCH_RE_MappingQCL_ConfigId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pdsch_RE_MappingQCL_ConfigId_r11,
{ "pdsch-RE-MappingQCL-ConfigId-r11", "lte-rrc.pdsch_RE_MappingQCL_ConfigId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_optionalSetOfFields_r11,
{ "optionalSetOfFields-r11", "lte-rrc.optionalSetOfFields_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_crs_PortsCount_r11_01,
{ "crs-PortsCount-r11", "lte-rrc.crs_PortsCount_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_crs_PortsCount_r11_01_vals), 0,
"T_crs_PortsCount_r11_01", HFILL }},
{ &hf_lte_rrc_crs_FreqShift_r11,
{ "crs-FreqShift-r11", "lte-rrc.crs_FreqShift_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_5", HFILL }},
{ &hf_lte_rrc_mbsfn_SubframeConfigList_r11_01,
{ "mbsfn-SubframeConfigList-r11", "lte-rrc.mbsfn_SubframeConfigList_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_mbsfn_SubframeConfigList_r11_01_vals), 0,
"T_mbsfn_SubframeConfigList_r11_01", HFILL }},
{ &hf_lte_rrc_setup_16,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_16", HFILL }},
{ &hf_lte_rrc_pdsch_Start_r11,
{ "pdsch-Start-r11", "lte-rrc.pdsch_Start_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_pdsch_Start_r11_vals), 0,
"T_pdsch_Start_r11", HFILL }},
{ &hf_lte_rrc_qcl_CSI_RS_ConfigNZPId_r11,
{ "qcl-CSI-RS-ConfigNZPId-r11", "lte-rrc.qcl_CSI_RS_ConfigNZPId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"CSI_RS_ConfigNZPId_r11", HFILL }},
{ &hf_lte_rrc_phich_Duration,
{ "phich-Duration", "lte-rrc.phich_Duration",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_phich_Duration_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_phich_Resource,
{ "phich-Resource", "lte-rrc.phich_Resource",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_phich_Resource_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_pdsch_ConfigDedicated,
{ "pdsch-ConfigDedicated", "lte-rrc.pdsch_ConfigDedicated_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pucch_ConfigDedicated,
{ "pucch-ConfigDedicated", "lte-rrc.pucch_ConfigDedicated_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pusch_ConfigDedicated,
{ "pusch-ConfigDedicated", "lte-rrc.pusch_ConfigDedicated_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_uplinkPowerControlDedicated,
{ "uplinkPowerControlDedicated", "lte-rrc.uplinkPowerControlDedicated_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_tpc_PDCCH_ConfigPUCCH,
{ "tpc-PDCCH-ConfigPUCCH", "lte-rrc.tpc_PDCCH_ConfigPUCCH",
FT_UINT32, BASE_DEC, VALS(lte_rrc_TPC_PDCCH_Config_vals), 0,
"TPC_PDCCH_Config", HFILL }},
{ &hf_lte_rrc_tpc_PDCCH_ConfigPUSCH,
{ "tpc-PDCCH-ConfigPUSCH", "lte-rrc.tpc_PDCCH_ConfigPUSCH",
FT_UINT32, BASE_DEC, VALS(lte_rrc_TPC_PDCCH_Config_vals), 0,
"TPC_PDCCH_Config", HFILL }},
{ &hf_lte_rrc_cqi_ReportConfig,
{ "cqi-ReportConfig", "lte-rrc.cqi_ReportConfig_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_soundingRS_UL_ConfigDedicated,
{ "soundingRS-UL-ConfigDedicated", "lte-rrc.soundingRS_UL_ConfigDedicated",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SoundingRS_UL_ConfigDedicated_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_antennaInfo,
{ "antennaInfo", "lte-rrc.antennaInfo",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_antennaInfo_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_explicitValue_02,
{ "explicitValue", "lte-rrc.explicitValue_element",
FT_NONE, BASE_NONE, NULL, 0,
"AntennaInfoDedicated", HFILL }},
{ &hf_lte_rrc_schedulingRequestConfig,
{ "schedulingRequestConfig", "lte-rrc.schedulingRequestConfig",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SchedulingRequestConfig_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_cqi_ReportConfig_v920,
{ "cqi-ReportConfig-v920", "lte-rrc.cqi_ReportConfig_v920_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_antennaInfo_v920,
{ "antennaInfo-v920", "lte-rrc.antennaInfo_v920_element",
FT_NONE, BASE_NONE, NULL, 0,
"AntennaInfoDedicated_v920", HFILL }},
{ &hf_lte_rrc_antennaInfo_r10,
{ "antennaInfo-r10", "lte-rrc.antennaInfo_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_antennaInfo_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_explicitValue_r10,
{ "explicitValue-r10", "lte-rrc.explicitValue_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"AntennaInfoDedicated_r10", HFILL }},
{ &hf_lte_rrc_antennaInfoUL_r10,
{ "antennaInfoUL-r10", "lte-rrc.antennaInfoUL_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cqi_ReportConfig_r10,
{ "cqi-ReportConfig-r10", "lte-rrc.cqi_ReportConfig_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_csi_RS_Config_r10,
{ "csi-RS-Config-r10", "lte-rrc.csi_RS_Config_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pucch_ConfigDedicated_v1020,
{ "pucch-ConfigDedicated-v1020", "lte-rrc.pucch_ConfigDedicated_v1020_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pusch_ConfigDedicated_v1020,
{ "pusch-ConfigDedicated-v1020", "lte-rrc.pusch_ConfigDedicated_v1020_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_schedulingRequestConfig_v1020,
{ "schedulingRequestConfig-v1020", "lte-rrc.schedulingRequestConfig_v1020_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_soundingRS_UL_ConfigDedicated_v1020,
{ "soundingRS-UL-ConfigDedicated-v1020", "lte-rrc.soundingRS_UL_ConfigDedicated_v1020_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_soundingRS_UL_ConfigDedicatedAperiodic_r10,
{ "soundingRS-UL-ConfigDedicatedAperiodic-r10", "lte-rrc.soundingRS_UL_ConfigDedicatedAperiodic_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SoundingRS_UL_ConfigDedicatedAperiodic_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_uplinkPowerControlDedicated_v1020,
{ "uplinkPowerControlDedicated-v1020", "lte-rrc.uplinkPowerControlDedicated_v1020_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_additionalSpectrumEmissionCA_r10,
{ "additionalSpectrumEmissionCA-r10", "lte-rrc.additionalSpectrumEmissionCA_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_additionalSpectrumEmissionCA_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_17,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_17", HFILL }},
{ &hf_lte_rrc_additionalSpectrumEmissionPCell_r10,
{ "additionalSpectrumEmissionPCell-r10", "lte-rrc.additionalSpectrumEmissionPCell_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"AdditionalSpectrumEmission", HFILL }},
{ &hf_lte_rrc_csi_RS_ConfigNZPToReleaseList_r11,
{ "csi-RS-ConfigNZPToReleaseList-r11", "lte-rrc.csi_RS_ConfigNZPToReleaseList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_csi_RS_ConfigNZPToAddModList_r11,
{ "csi-RS-ConfigNZPToAddModList-r11", "lte-rrc.csi_RS_ConfigNZPToAddModList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_csi_RS_ConfigZPToReleaseList_r11,
{ "csi-RS-ConfigZPToReleaseList-r11", "lte-rrc.csi_RS_ConfigZPToReleaseList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_csi_RS_ConfigZPToAddModList_r11,
{ "csi-RS-ConfigZPToAddModList-r11", "lte-rrc.csi_RS_ConfigZPToAddModList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_epdcch_Config_r11,
{ "epdcch-Config-r11", "lte-rrc.epdcch_Config_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pdsch_ConfigDedicated_v1130,
{ "pdsch-ConfigDedicated-v1130", "lte-rrc.pdsch_ConfigDedicated_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cqi_ReportConfig_v1130,
{ "cqi-ReportConfig-v1130", "lte-rrc.cqi_ReportConfig_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pucch_ConfigDedicated_v1130,
{ "pucch-ConfigDedicated-v1130", "lte-rrc.pucch_ConfigDedicated_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pusch_ConfigDedicated_v1130,
{ "pusch-ConfigDedicated-v1130", "lte-rrc.pusch_ConfigDedicated_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_uplinkPowerControlDedicated_v1130,
{ "uplinkPowerControlDedicated-v1130", "lte-rrc.uplinkPowerControlDedicated_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonUL_Configuration_r10,
{ "nonUL-Configuration-r10", "lte-rrc.nonUL_Configuration_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_antennaInfo_r10_01,
{ "antennaInfo-r10", "lte-rrc.antennaInfo_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"AntennaInfoDedicated_r10", HFILL }},
{ &hf_lte_rrc_crossCarrierSchedulingConfig_r10,
{ "crossCarrierSchedulingConfig-r10", "lte-rrc.crossCarrierSchedulingConfig_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pdsch_ConfigDedicated_r10,
{ "pdsch-ConfigDedicated-r10", "lte-rrc.pdsch_ConfigDedicated_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"PDSCH_ConfigDedicated", HFILL }},
{ &hf_lte_rrc_ul_Configuration_r10,
{ "ul-Configuration-r10", "lte-rrc.ul_Configuration_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pusch_ConfigDedicatedSCell_r10,
{ "pusch-ConfigDedicatedSCell-r10", "lte-rrc.pusch_ConfigDedicatedSCell_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_uplinkPowerControlDedicatedSCell_r10,
{ "uplinkPowerControlDedicatedSCell-r10", "lte-rrc.uplinkPowerControlDedicatedSCell_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cqi_ReportConfigSCell_r10,
{ "cqi-ReportConfigSCell-r10", "lte-rrc.cqi_ReportConfigSCell_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_soundingRS_UL_ConfigDedicated_r10,
{ "soundingRS-UL-ConfigDedicated-r10", "lte-rrc.soundingRS_UL_ConfigDedicated_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SoundingRS_UL_ConfigDedicated_vals), 0,
"SoundingRS_UL_ConfigDedicated", HFILL }},
{ &hf_lte_rrc_uplinkPowerControlDedicatedSCell_v1130,
{ "uplinkPowerControlDedicatedSCell-v1130", "lte-rrc.uplinkPowerControlDedicatedSCell_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
"UplinkPowerControlDedicated_v1130", HFILL }},
{ &hf_lte_rrc_CSI_RS_ConfigNZPToAddModList_r11_item,
{ "CSI-RS-ConfigNZP-r11", "lte-rrc.CSI_RS_ConfigNZP_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CSI_RS_ConfigNZPToReleaseList_r11_item,
{ "CSI-RS-ConfigNZPId-r11", "lte-rrc.CSI_RS_ConfigNZPId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CSI_RS_ConfigZPToAddModList_r11_item,
{ "CSI-RS-ConfigZP-r11", "lte-rrc.CSI_RS_ConfigZP_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CSI_RS_ConfigZPToReleaseList_r11_item,
{ "CSI-RS-ConfigZPId-r11", "lte-rrc.CSI_RS_ConfigZPId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rootSequenceIndex,
{ "rootSequenceIndex", "lte-rrc.rootSequenceIndex",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_837", HFILL }},
{ &hf_lte_rrc_prach_ConfigInfo,
{ "prach-ConfigInfo", "lte-rrc.prach_ConfigInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_prach_ConfigIndex_r10,
{ "prach-ConfigIndex-r10", "lte-rrc.prach_ConfigIndex_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_63", HFILL }},
{ &hf_lte_rrc_prach_ConfigIndex,
{ "prach-ConfigIndex", "lte-rrc.prach_ConfigIndex",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_63", HFILL }},
{ &hf_lte_rrc_highSpeedFlag,
{ "highSpeedFlag", "lte-rrc.highSpeedFlag",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_zeroCorrelationZoneConfig,
{ "zeroCorrelationZoneConfig", "lte-rrc.zeroCorrelationZoneConfig",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_15", HFILL }},
{ &hf_lte_rrc_prach_FreqOffset,
{ "prach-FreqOffset", "lte-rrc.prach_FreqOffset",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_94", HFILL }},
{ &hf_lte_rrc_deltaPUCCH_Shift,
{ "deltaPUCCH-Shift", "lte-rrc.deltaPUCCH_Shift",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deltaPUCCH_Shift_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nRB_CQI,
{ "nRB-CQI", "lte-rrc.nRB_CQI",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_98", HFILL }},
{ &hf_lte_rrc_nCS_AN,
{ "nCS-AN", "lte-rrc.nCS_AN",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_7", HFILL }},
{ &hf_lte_rrc_n1PUCCH_AN,
{ "n1PUCCH-AN", "lte-rrc.n1PUCCH_AN",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_2047", HFILL }},
{ &hf_lte_rrc_ackNackRepetition,
{ "ackNackRepetition", "lte-rrc.ackNackRepetition",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_ackNackRepetition_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_18,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_18", HFILL }},
{ &hf_lte_rrc_repetitionFactor,
{ "repetitionFactor", "lte-rrc.repetitionFactor",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_repetitionFactor_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_n1PUCCH_AN_Rep,
{ "n1PUCCH-AN-Rep", "lte-rrc.n1PUCCH_AN_Rep",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_2047", HFILL }},
{ &hf_lte_rrc_tdd_AckNackFeedbackMode,
{ "tdd-AckNackFeedbackMode", "lte-rrc.tdd_AckNackFeedbackMode",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_tdd_AckNackFeedbackMode_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_pucch_Format_r10,
{ "pucch-Format-r10", "lte-rrc.pucch_Format_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_pucch_Format_r10_vals), 0,
"T_pucch_Format_r10", HFILL }},
{ &hf_lte_rrc_format3_r10,
{ "format3-r10", "lte-rrc.format3_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_n3PUCCH_AN_List_r10,
{ "n3PUCCH-AN-List-r10", "lte-rrc.n3PUCCH_AN_List_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"T_n3PUCCH_AN_List_r10", HFILL }},
{ &hf_lte_rrc_n3PUCCH_AN_List_r10_item,
{ "n3PUCCH-AN-List-r10 item", "lte-rrc.n3PUCCH_AN_List_r10_item",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_549", HFILL }},
{ &hf_lte_rrc_twoAntennaPortActivatedPUCCH_Format3_r10,
{ "twoAntennaPortActivatedPUCCH-Format3-r10", "lte-rrc.twoAntennaPortActivatedPUCCH_Format3_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_twoAntennaPortActivatedPUCCH_Format3_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_19,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_19", HFILL }},
{ &hf_lte_rrc_n3PUCCH_AN_ListP1_r10,
{ "n3PUCCH-AN-ListP1-r10", "lte-rrc.n3PUCCH_AN_ListP1_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_n3PUCCH_AN_ListP1_r10_item,
{ "n3PUCCH-AN-ListP1-r10 item", "lte-rrc.n3PUCCH_AN_ListP1_r10_item",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_549", HFILL }},
{ &hf_lte_rrc_channelSelection_r10,
{ "channelSelection-r10", "lte-rrc.channelSelection_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_n1PUCCH_AN_CS_r10,
{ "n1PUCCH-AN-CS-r10", "lte-rrc.n1PUCCH_AN_CS_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_n1PUCCH_AN_CS_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_20,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_20", HFILL }},
{ &hf_lte_rrc_n1PUCCH_AN_CS_List_r10,
{ "n1PUCCH-AN-CS-List-r10", "lte-rrc.n1PUCCH_AN_CS_List_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_2_OF_N1PUCCH_AN_CS_r10", HFILL }},
{ &hf_lte_rrc_n1PUCCH_AN_CS_List_r10_item,
{ "N1PUCCH-AN-CS-r10", "lte-rrc.N1PUCCH_AN_CS_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_twoAntennaPortActivatedPUCCH_Format1a1b_r10,
{ "twoAntennaPortActivatedPUCCH-Format1a1b-r10", "lte-rrc.twoAntennaPortActivatedPUCCH_Format1a1b_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_twoAntennaPortActivatedPUCCH_Format1a1b_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_simultaneousPUCCH_PUSCH_r10,
{ "simultaneousPUCCH-PUSCH-r10", "lte-rrc.simultaneousPUCCH_PUSCH_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_simultaneousPUCCH_PUSCH_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_n1PUCCH_AN_RepP1_r10,
{ "n1PUCCH-AN-RepP1-r10", "lte-rrc.n1PUCCH_AN_RepP1_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_2047", HFILL }},
{ &hf_lte_rrc_n1PUCCH_AN_CS_v1130,
{ "n1PUCCH-AN-CS-v1130", "lte-rrc.n1PUCCH_AN_CS_v1130",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_n1PUCCH_AN_CS_v1130_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_21,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_21", HFILL }},
{ &hf_lte_rrc_n1PUCCH_AN_CS_ListP1_r11,
{ "n1PUCCH-AN-CS-ListP1-r11", "lte-rrc.n1PUCCH_AN_CS_ListP1_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_n1PUCCH_AN_CS_ListP1_r11_item,
{ "n1PUCCH-AN-CS-ListP1-r11 item", "lte-rrc.n1PUCCH_AN_CS_ListP1_r11_item",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_2047", HFILL }},
{ &hf_lte_rrc_nPUCCH_Param_r11,
{ "nPUCCH-Param-r11", "lte-rrc.nPUCCH_Param_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_nPUCCH_Param_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_22,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_22", HFILL }},
{ &hf_lte_rrc_nPUCCH_Identity_r11,
{ "nPUCCH-Identity-r11", "lte-rrc.nPUCCH_Identity_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_503", HFILL }},
{ &hf_lte_rrc_n1PUCCH_AN_r11,
{ "n1PUCCH-AN-r11", "lte-rrc.n1PUCCH_AN_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_2047", HFILL }},
{ &hf_lte_rrc_N1PUCCH_AN_CS_r10_item,
{ "N1PUCCH-AN-CS-r10 item", "lte-rrc.N1PUCCH_AN_CS_r10_item",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_2047", HFILL }},
{ &hf_lte_rrc_pusch_ConfigBasic,
{ "pusch-ConfigBasic", "lte-rrc.pusch_ConfigBasic_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_n_SB,
{ "n-SB", "lte-rrc.n_SB",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_4", HFILL }},
{ &hf_lte_rrc_hoppingMode,
{ "hoppingMode", "lte-rrc.hoppingMode",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_hoppingMode_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_pusch_HoppingOffset,
{ "pusch-HoppingOffset", "lte-rrc.pusch_HoppingOffset",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_98", HFILL }},
{ &hf_lte_rrc_enable64QAM,
{ "enable64QAM", "lte-rrc.enable64QAM",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_ul_ReferenceSignalsPUSCH,
{ "ul-ReferenceSignalsPUSCH", "lte-rrc.ul_ReferenceSignalsPUSCH_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_betaOffset_ACK_Index,
{ "betaOffset-ACK-Index", "lte-rrc.betaOffset_ACK_Index",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_15", HFILL }},
{ &hf_lte_rrc_betaOffset_RI_Index,
{ "betaOffset-RI-Index", "lte-rrc.betaOffset_RI_Index",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_15", HFILL }},
{ &hf_lte_rrc_betaOffset_CQI_Index,
{ "betaOffset-CQI-Index", "lte-rrc.betaOffset_CQI_Index",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_15", HFILL }},
{ &hf_lte_rrc_betaOffsetMC_r10,
{ "betaOffsetMC-r10", "lte-rrc.betaOffsetMC_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_betaOffset_ACK_Index_MC_r10,
{ "betaOffset-ACK-Index-MC-r10", "lte-rrc.betaOffset_ACK_Index_MC_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_15", HFILL }},
{ &hf_lte_rrc_betaOffset_RI_Index_MC_r10,
{ "betaOffset-RI-Index-MC-r10", "lte-rrc.betaOffset_RI_Index_MC_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_15", HFILL }},
{ &hf_lte_rrc_betaOffset_CQI_Index_MC_r10,
{ "betaOffset-CQI-Index-MC-r10", "lte-rrc.betaOffset_CQI_Index_MC_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_15", HFILL }},
{ &hf_lte_rrc_groupHoppingDisabled_r10,
{ "groupHoppingDisabled-r10", "lte-rrc.groupHoppingDisabled_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_groupHoppingDisabled_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_dmrs_WithOCC_Activated_r10,
{ "dmrs-WithOCC-Activated-r10", "lte-rrc.dmrs_WithOCC_Activated_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_dmrs_WithOCC_Activated_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_pusch_DMRS_r11,
{ "pusch-DMRS-r11", "lte-rrc.pusch_DMRS_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_pusch_DMRS_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_23,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_23", HFILL }},
{ &hf_lte_rrc_nPUSCH_Identity_r11,
{ "nPUSCH-Identity-r11", "lte-rrc.nPUSCH_Identity_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_509", HFILL }},
{ &hf_lte_rrc_nDMRS_CSH_Identity_r11,
{ "nDMRS-CSH-Identity-r11", "lte-rrc.nDMRS_CSH_Identity_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_509", HFILL }},
{ &hf_lte_rrc_groupHoppingDisabled_r10_01,
{ "groupHoppingDisabled-r10", "lte-rrc.groupHoppingDisabled_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_groupHoppingDisabled_r10_01_vals), 0,
"T_groupHoppingDisabled_r10_01", HFILL }},
{ &hf_lte_rrc_dmrs_WithOCC_Activated_r10_01,
{ "dmrs-WithOCC-Activated-r10", "lte-rrc.dmrs_WithOCC_Activated_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_dmrs_WithOCC_Activated_r10_01_vals), 0,
"T_dmrs_WithOCC_Activated_r10_01", HFILL }},
{ &hf_lte_rrc_groupHoppingEnabled,
{ "groupHoppingEnabled", "lte-rrc.groupHoppingEnabled",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_groupAssignmentPUSCH,
{ "groupAssignmentPUSCH", "lte-rrc.groupAssignmentPUSCH",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_29", HFILL }},
{ &hf_lte_rrc_sequenceHoppingEnabled,
{ "sequenceHoppingEnabled", "lte-rrc.sequenceHoppingEnabled",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_cyclicShift,
{ "cyclicShift", "lte-rrc.cyclicShift",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_7", HFILL }},
{ &hf_lte_rrc_preambleInfo,
{ "preambleInfo", "lte-rrc.preambleInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_numberOfRA_Preambles,
{ "numberOfRA-Preambles", "lte-rrc.numberOfRA_Preambles",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_numberOfRA_Preambles_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_preamblesGroupAConfig,
{ "preamblesGroupAConfig", "lte-rrc.preamblesGroupAConfig_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sizeOfRA_PreamblesGroupA,
{ "sizeOfRA-PreamblesGroupA", "lte-rrc.sizeOfRA_PreamblesGroupA",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_sizeOfRA_PreamblesGroupA_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_messageSizeGroupA,
{ "messageSizeGroupA", "lte-rrc.messageSizeGroupA",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_messageSizeGroupA_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_messagePowerOffsetGroupB,
{ "messagePowerOffsetGroupB", "lte-rrc.messagePowerOffsetGroupB",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_messagePowerOffsetGroupB_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_powerRampingParameters,
{ "powerRampingParameters", "lte-rrc.powerRampingParameters_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ra_SupervisionInfo,
{ "ra-SupervisionInfo", "lte-rrc.ra_SupervisionInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_preambleTransMax,
{ "preambleTransMax", "lte-rrc.preambleTransMax",
FT_UINT32, BASE_DEC, VALS(lte_rrc_PreambleTransMax_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_ra_ResponseWindowSize,
{ "ra-ResponseWindowSize", "lte-rrc.ra_ResponseWindowSize",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_ra_ResponseWindowSize_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_mac_ContentionResolutionTimer,
{ "mac-ContentionResolutionTimer", "lte-rrc.mac_ContentionResolutionTimer",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_mac_ContentionResolutionTimer_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_maxHARQ_Msg3Tx,
{ "maxHARQ-Msg3Tx", "lte-rrc.maxHARQ_Msg3Tx",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_8", HFILL }},
{ &hf_lte_rrc_powerRampingParameters_r11,
{ "powerRampingParameters-r11", "lte-rrc.powerRampingParameters_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"PowerRampingParameters", HFILL }},
{ &hf_lte_rrc_ra_SupervisionInfo_r11,
{ "ra-SupervisionInfo-r11", "lte-rrc.ra_SupervisionInfo_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_preambleTransMax_r11,
{ "preambleTransMax-r11", "lte-rrc.preambleTransMax_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_PreambleTransMax_vals), 0,
"PreambleTransMax", HFILL }},
{ &hf_lte_rrc_powerRampingStep,
{ "powerRampingStep", "lte-rrc.powerRampingStep",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_powerRampingStep_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_preambleInitialReceivedTargetPower,
{ "preambleInitialReceivedTargetPower", "lte-rrc.preambleInitialReceivedTargetPower",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_preambleInitialReceivedTargetPower_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_ra_PreambleIndex,
{ "ra-PreambleIndex", "lte-rrc.ra_PreambleIndex",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_63", HFILL }},
{ &hf_lte_rrc_ra_PRACH_MaskIndex,
{ "ra-PRACH-MaskIndex", "lte-rrc.ra_PRACH_MaskIndex",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_15", HFILL }},
{ &hf_lte_rrc_rach_ConfigCommon,
{ "rach-ConfigCommon", "lte-rrc.rach_ConfigCommon_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_bcch_Config,
{ "bcch-Config", "lte-rrc.bcch_Config_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pcch_Config,
{ "pcch-Config", "lte-rrc.pcch_Config_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_prach_Config,
{ "prach-Config", "lte-rrc.prach_Config_element",
FT_NONE, BASE_NONE, NULL, 0,
"PRACH_ConfigSIB", HFILL }},
{ &hf_lte_rrc_pdsch_ConfigCommon,
{ "pdsch-ConfigCommon", "lte-rrc.pdsch_ConfigCommon_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pusch_ConfigCommon,
{ "pusch-ConfigCommon", "lte-rrc.pusch_ConfigCommon_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pucch_ConfigCommon,
{ "pucch-ConfigCommon", "lte-rrc.pucch_ConfigCommon_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_soundingRS_UL_ConfigCommon,
{ "soundingRS-UL-ConfigCommon", "lte-rrc.soundingRS_UL_ConfigCommon",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SoundingRS_UL_ConfigCommon_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_uplinkPowerControlCommon,
{ "uplinkPowerControlCommon", "lte-rrc.uplinkPowerControlCommon_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ul_CyclicPrefixLength,
{ "ul-CyclicPrefixLength", "lte-rrc.ul_CyclicPrefixLength",
FT_UINT32, BASE_DEC, VALS(lte_rrc_UL_CyclicPrefixLength_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_uplinkPowerControlCommon_v1020,
{ "uplinkPowerControlCommon-v1020", "lte-rrc.uplinkPowerControlCommon_v1020_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_prach_Config_01,
{ "prach-Config", "lte-rrc.prach_Config_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_antennaInfoCommon,
{ "antennaInfoCommon", "lte-rrc.antennaInfoCommon_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonUL_Configuration_r10_01,
{ "nonUL-Configuration-r10", "lte-rrc.nonUL_Configuration_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonUL_Configuration_r10_01", HFILL }},
{ &hf_lte_rrc_dl_Bandwidth_r10,
{ "dl-Bandwidth-r10", "lte-rrc.dl_Bandwidth_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_dl_Bandwidth_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_antennaInfoCommon_r10,
{ "antennaInfoCommon-r10", "lte-rrc.antennaInfoCommon_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"AntennaInfoCommon", HFILL }},
{ &hf_lte_rrc_mbsfn_SubframeConfigList_r10,
{ "mbsfn-SubframeConfigList-r10", "lte-rrc.mbsfn_SubframeConfigList_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"MBSFN_SubframeConfigList", HFILL }},
{ &hf_lte_rrc_phich_Config_r10,
{ "phich-Config-r10", "lte-rrc.phich_Config_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"PHICH_Config", HFILL }},
{ &hf_lte_rrc_pdsch_ConfigCommon_r10,
{ "pdsch-ConfigCommon-r10", "lte-rrc.pdsch_ConfigCommon_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"PDSCH_ConfigCommon", HFILL }},
{ &hf_lte_rrc_tdd_Config_r10,
{ "tdd-Config-r10", "lte-rrc.tdd_Config_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"TDD_Config", HFILL }},
{ &hf_lte_rrc_ul_Configuration_r10_01,
{ "ul-Configuration-r10", "lte-rrc.ul_Configuration_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_ul_Configuration_r10_01", HFILL }},
{ &hf_lte_rrc_ul_FreqInfo_r10,
{ "ul-FreqInfo-r10", "lte-rrc.ul_FreqInfo_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ul_CarrierFreq_r10,
{ "ul-CarrierFreq-r10", "lte-rrc.ul_CarrierFreq_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA", HFILL }},
{ &hf_lte_rrc_ul_Bandwidth_r10,
{ "ul-Bandwidth-r10", "lte-rrc.ul_Bandwidth_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_ul_Bandwidth_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_additionalSpectrumEmissionSCell_r10,
{ "additionalSpectrumEmissionSCell-r10", "lte-rrc.additionalSpectrumEmissionSCell_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"AdditionalSpectrumEmission", HFILL }},
{ &hf_lte_rrc_p_Max_r10,
{ "p-Max-r10", "lte-rrc.p_Max_r10",
FT_INT32, BASE_DEC, NULL, 0,
"P_Max", HFILL }},
{ &hf_lte_rrc_uplinkPowerControlCommonSCell_r10,
{ "uplinkPowerControlCommonSCell-r10", "lte-rrc.uplinkPowerControlCommonSCell_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_soundingRS_UL_ConfigCommon_r10,
{ "soundingRS-UL-ConfigCommon-r10", "lte-rrc.soundingRS_UL_ConfigCommon_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SoundingRS_UL_ConfigCommon_vals), 0,
"SoundingRS_UL_ConfigCommon", HFILL }},
{ &hf_lte_rrc_ul_CyclicPrefixLength_r10,
{ "ul-CyclicPrefixLength-r10", "lte-rrc.ul_CyclicPrefixLength_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_UL_CyclicPrefixLength_vals), 0,
"UL_CyclicPrefixLength", HFILL }},
{ &hf_lte_rrc_prach_ConfigSCell_r10,
{ "prach-ConfigSCell-r10", "lte-rrc.prach_ConfigSCell_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pusch_ConfigCommon_r10,
{ "pusch-ConfigCommon-r10", "lte-rrc.pusch_ConfigCommon_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"PUSCH_ConfigCommon", HFILL }},
{ &hf_lte_rrc_ul_CarrierFreq_v1090,
{ "ul-CarrierFreq-v1090", "lte-rrc.ul_CarrierFreq_v1090",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA_v9e0", HFILL }},
{ &hf_lte_rrc_rach_ConfigCommonSCell_r11,
{ "rach-ConfigCommonSCell-r11", "lte-rrc.rach_ConfigCommonSCell_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_prach_ConfigSCell_r11,
{ "prach-ConfigSCell-r11", "lte-rrc.prach_ConfigSCell_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"PRACH_Config", HFILL }},
{ &hf_lte_rrc_uplinkPowerControlCommonSCell_v1130,
{ "uplinkPowerControlCommonSCell-v1130", "lte-rrc.uplinkPowerControlCommonSCell_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_modificationPeriodCoeff,
{ "modificationPeriodCoeff", "lte-rrc.modificationPeriodCoeff",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_modificationPeriodCoeff_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_defaultPagingCycle,
{ "defaultPagingCycle", "lte-rrc.defaultPagingCycle",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_defaultPagingCycle_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nB,
{ "nB", "lte-rrc.nB",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_nB_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_srb_ToAddModList,
{ "srb-ToAddModList", "lte-rrc.srb_ToAddModList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_drb_ToAddModList,
{ "drb-ToAddModList", "lte-rrc.drb_ToAddModList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_drb_ToReleaseList,
{ "drb-ToReleaseList", "lte-rrc.drb_ToReleaseList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mac_MainConfig,
{ "mac-MainConfig", "lte-rrc.mac_MainConfig",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_mac_MainConfig_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_explicitValue_03,
{ "explicitValue", "lte-rrc.explicitValue_element",
FT_NONE, BASE_NONE, NULL, 0,
"MAC_MainConfig", HFILL }},
{ &hf_lte_rrc_sps_Config,
{ "sps-Config", "lte-rrc.sps_Config_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_physicalConfigDedicated,
{ "physicalConfigDedicated", "lte-rrc.physicalConfigDedicated_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rlf_TimersAndConstants_r9,
{ "rlf-TimersAndConstants-r9", "lte-rrc.rlf_TimersAndConstants_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_RLF_TimersAndConstants_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_measSubframePatternPCell_r10,
{ "measSubframePatternPCell-r10", "lte-rrc.measSubframePatternPCell_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_MeasSubframePatternPCell_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_neighCellsCRS_Info_r11,
{ "neighCellsCRS-Info-r11", "lte-rrc.neighCellsCRS_Info_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_NeighCellsCRS_Info_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_physicalConfigDedicatedSCell_r10,
{ "physicalConfigDedicatedSCell-r10", "lte-rrc.physicalConfigDedicatedSCell_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mac_MainConfigSCell_r11,
{ "mac-MainConfigSCell-r11", "lte-rrc.mac_MainConfigSCell_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SRB_ToAddModList_item,
{ "SRB-ToAddMod", "lte-rrc.SRB_ToAddMod_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_srb_Identity,
{ "srb-Identity", "lte-rrc.srb_Identity",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_2", HFILL }},
{ &hf_lte_rrc_rlc_Config,
{ "rlc-Config", "lte-rrc.rlc_Config",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_rlc_Config_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_explicitValue_04,
{ "explicitValue", "lte-rrc.explicitValue",
FT_UINT32, BASE_DEC, VALS(lte_rrc_RLC_Config_vals), 0,
"RLC_Config", HFILL }},
{ &hf_lte_rrc_logicalChannelConfig,
{ "logicalChannelConfig", "lte-rrc.logicalChannelConfig",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_logicalChannelConfig_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_explicitValue_05,
{ "explicitValue", "lte-rrc.explicitValue_element",
FT_NONE, BASE_NONE, NULL, 0,
"LogicalChannelConfig", HFILL }},
{ &hf_lte_rrc_DRB_ToAddModList_item,
{ "DRB-ToAddMod", "lte-rrc.DRB_ToAddMod_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_eps_BearerIdentity,
{ "eps-BearerIdentity", "lte-rrc.eps_BearerIdentity",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_15", HFILL }},
{ &hf_lte_rrc_pdcp_Config,
{ "pdcp-Config", "lte-rrc.pdcp_Config_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rlc_Config_01,
{ "rlc-Config", "lte-rrc.rlc_Config",
FT_UINT32, BASE_DEC, VALS(lte_rrc_RLC_Config_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_logicalChannelIdentity,
{ "logicalChannelIdentity", "lte-rrc.logicalChannelIdentity",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_logicalChannelConfig_01,
{ "logicalChannelConfig", "lte-rrc.logicalChannelConfig_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_DRB_ToReleaseList_item,
{ "DRB-Identity", "lte-rrc.DRB_Identity",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_24,
{ "setup", "lte-rrc.setup",
FT_UINT32, BASE_DEC, VALS(lte_rrc_MeasSubframePattern_r10_vals), 0,
"MeasSubframePattern_r10", HFILL }},
{ &hf_lte_rrc_setup_25,
{ "setup", "lte-rrc.setup",
FT_UINT32, BASE_DEC, NULL, 0,
"CRS_AssistanceInfoList_r11", HFILL }},
{ &hf_lte_rrc_CRS_AssistanceInfoList_r11_item,
{ "CRS-AssistanceInfo-r11", "lte-rrc.CRS_AssistanceInfo_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_physCellId_r11_02,
{ "physCellId-r11", "lte-rrc.physCellId_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellId", HFILL }},
{ &hf_lte_rrc_antennaPortsCount_r11_01,
{ "antennaPortsCount-r11", "lte-rrc.antennaPortsCount_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_antennaPortsCount_r11_01_vals), 0,
"T_antennaPortsCount_r11_01", HFILL }},
{ &hf_lte_rrc_mbsfn_SubframeConfigList_r11_02,
{ "mbsfn-SubframeConfigList-r11", "lte-rrc.mbsfn_SubframeConfigList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"MBSFN_SubframeConfigList", HFILL }},
{ &hf_lte_rrc_am,
{ "am", "lte-rrc.am_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ul_AM_RLC,
{ "ul-AM-RLC", "lte-rrc.ul_AM_RLC_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_dl_AM_RLC,
{ "dl-AM-RLC", "lte-rrc.dl_AM_RLC_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_um_Bi_Directional,
{ "um-Bi-Directional", "lte-rrc.um_Bi_Directional_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ul_UM_RLC,
{ "ul-UM-RLC", "lte-rrc.ul_UM_RLC_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_dl_UM_RLC,
{ "dl-UM-RLC", "lte-rrc.dl_UM_RLC_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_um_Uni_Directional_UL,
{ "um-Uni-Directional-UL", "lte-rrc.um_Uni_Directional_UL_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_um_Uni_Directional_DL,
{ "um-Uni-Directional-DL", "lte-rrc.um_Uni_Directional_DL_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_t_PollRetransmit,
{ "t-PollRetransmit", "lte-rrc.t_PollRetransmit",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_T_PollRetransmit_vals_ext, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pollPDU,
{ "pollPDU", "lte-rrc.pollPDU",
FT_UINT32, BASE_DEC, VALS(lte_rrc_PollPDU_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_pollByte,
{ "pollByte", "lte-rrc.pollByte",
FT_UINT32, BASE_DEC, VALS(lte_rrc_PollByte_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_maxRetxThreshold,
{ "maxRetxThreshold", "lte-rrc.maxRetxThreshold",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_maxRetxThreshold_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_t_Reordering,
{ "t-Reordering", "lte-rrc.t_Reordering",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_T_Reordering_vals_ext, 0,
NULL, HFILL }},
{ &hf_lte_rrc_t_StatusProhibit,
{ "t-StatusProhibit", "lte-rrc.t_StatusProhibit",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_T_StatusProhibit_vals_ext, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sn_FieldLength,
{ "sn-FieldLength", "lte-rrc.sn_FieldLength",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SN_FieldLength_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_26,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_24", HFILL }},
{ &hf_lte_rrc_t301_r9,
{ "t301-r9", "lte-rrc.t301_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_t301_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_t310_r9,
{ "t310-r9", "lte-rrc.t310_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_t310_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_n310_r9,
{ "n310-r9", "lte-rrc.n310_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_n310_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_t311_r9,
{ "t311-r9", "lte-rrc.t311_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_t311_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_n311_r9,
{ "n311-r9", "lte-rrc.n311_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_n311_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_subframeConfigPattern_r10,
{ "subframeConfigPattern-r10", "lte-rrc.subframeConfigPattern_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_subframeConfigPattern_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_subframeConfigPatternFDD_r10,
{ "subframeConfigPatternFDD-r10", "lte-rrc.subframeConfigPatternFDD_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_8", HFILL }},
{ &hf_lte_rrc_subframeConfigPatternTDD_r10,
{ "subframeConfigPatternTDD-r10", "lte-rrc.subframeConfigPatternTDD_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_31", HFILL }},
{ &hf_lte_rrc_rpdcch_Config_r10,
{ "rpdcch-Config-r10", "lte-rrc.rpdcch_Config_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_resourceAllocationType_r10,
{ "resourceAllocationType-r10", "lte-rrc.resourceAllocationType_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_resourceAllocationType_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_resourceBlockAssignment_r10,
{ "resourceBlockAssignment-r10", "lte-rrc.resourceBlockAssignment_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_resourceBlockAssignment_r10_vals), 0,
"T_resourceBlockAssignment_r10", HFILL }},
{ &hf_lte_rrc_type01_r10,
{ "type01-r10", "lte-rrc.type01_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_type01_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nrb6_r10,
{ "nrb6-r10", "lte-rrc.nrb6_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_6", HFILL }},
{ &hf_lte_rrc_nrb15_r10,
{ "nrb15-r10", "lte-rrc.nrb15_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_8", HFILL }},
{ &hf_lte_rrc_nrb25_r10,
{ "nrb25-r10", "lte-rrc.nrb25_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_13", HFILL }},
{ &hf_lte_rrc_nrb50_r10,
{ "nrb50-r10", "lte-rrc.nrb50_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_17", HFILL }},
{ &hf_lte_rrc_nrb75_r10,
{ "nrb75-r10", "lte-rrc.nrb75_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_19", HFILL }},
{ &hf_lte_rrc_nrb100_r10,
{ "nrb100-r10", "lte-rrc.nrb100_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_25", HFILL }},
{ &hf_lte_rrc_type2_r10,
{ "type2-r10", "lte-rrc.type2_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_type2_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nrb6_r10_01,
{ "nrb6-r10", "lte-rrc.nrb6_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_5", HFILL }},
{ &hf_lte_rrc_nrb15_r10_01,
{ "nrb15-r10", "lte-rrc.nrb15_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_7", HFILL }},
{ &hf_lte_rrc_nrb25_r10_01,
{ "nrb25-r10", "lte-rrc.nrb25_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_9", HFILL }},
{ &hf_lte_rrc_nrb50_r10_01,
{ "nrb50-r10", "lte-rrc.nrb50_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_11", HFILL }},
{ &hf_lte_rrc_nrb75_r10_01,
{ "nrb75-r10", "lte-rrc.nrb75_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_12", HFILL }},
{ &hf_lte_rrc_nrb100_r10_01,
{ "nrb100-r10", "lte-rrc.nrb100_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_13", HFILL }},
{ &hf_lte_rrc_demodulationRS_r10,
{ "demodulationRS-r10", "lte-rrc.demodulationRS_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_demodulationRS_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_interleaving_r10,
{ "interleaving-r10", "lte-rrc.interleaving_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_interleaving_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_noInterleaving_r10,
{ "noInterleaving-r10", "lte-rrc.noInterleaving_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_noInterleaving_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_pdsch_Start_r10_01,
{ "pdsch-Start-r10", "lte-rrc.pdsch_Start_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_3", HFILL }},
{ &hf_lte_rrc_pucch_Config_r10,
{ "pucch-Config-r10", "lte-rrc.pucch_Config_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_pucch_Config_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_tdd,
{ "tdd", "lte-rrc.tdd",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_tdd_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_channelSelectionMultiplexingBundling,
{ "channelSelectionMultiplexingBundling", "lte-rrc.channelSelectionMultiplexingBundling_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_n1PUCCH_AN_List_r10,
{ "n1PUCCH-AN-List-r10", "lte-rrc.n1PUCCH_AN_List_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"T_n1PUCCH_AN_List_r10", HFILL }},
{ &hf_lte_rrc_n1PUCCH_AN_List_r10_item,
{ "n1PUCCH-AN-List-r10 item", "lte-rrc.n1PUCCH_AN_List_r10_item",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_2047", HFILL }},
{ &hf_lte_rrc_fallbackForFormat3,
{ "fallbackForFormat3", "lte-rrc.fallbackForFormat3_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_n1PUCCH_AN_P0_r10,
{ "n1PUCCH-AN-P0-r10", "lte-rrc.n1PUCCH_AN_P0_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_2047", HFILL }},
{ &hf_lte_rrc_n1PUCCH_AN_P1_r10,
{ "n1PUCCH-AN-P1-r10", "lte-rrc.n1PUCCH_AN_P1_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_2047", HFILL }},
{ &hf_lte_rrc_fdd,
{ "fdd", "lte-rrc.fdd_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_27,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_25", HFILL }},
{ &hf_lte_rrc_sr_PUCCH_ResourceIndex,
{ "sr-PUCCH-ResourceIndex", "lte-rrc.sr_PUCCH_ResourceIndex",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_2047", HFILL }},
{ &hf_lte_rrc_sr_ConfigIndex,
{ "sr-ConfigIndex", "lte-rrc.sr_ConfigIndex",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_dsr_TransMax,
{ "dsr-TransMax", "lte-rrc.dsr_TransMax",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_dsr_TransMax_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_sr_PUCCH_ResourceIndexP1_r10,
{ "sr-PUCCH-ResourceIndexP1-r10", "lte-rrc.sr_PUCCH_ResourceIndexP1_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_2047", HFILL }},
{ &hf_lte_rrc_setup_28,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_26", HFILL }},
{ &hf_lte_rrc_srs_BandwidthConfig,
{ "srs-BandwidthConfig", "lte-rrc.srs_BandwidthConfig",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_srs_BandwidthConfig_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_srs_SubframeConfig,
{ "srs-SubframeConfig", "lte-rrc.srs_SubframeConfig",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_srs_SubframeConfig_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_ackNackSRS_SimultaneousTransmission,
{ "ackNackSRS-SimultaneousTransmission", "lte-rrc.ackNackSRS_SimultaneousTransmission",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_srs_MaxUpPts,
{ "srs-MaxUpPts", "lte-rrc.srs_MaxUpPts",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_srs_MaxUpPts_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_29,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_27", HFILL }},
{ &hf_lte_rrc_srs_Bandwidth,
{ "srs-Bandwidth", "lte-rrc.srs_Bandwidth",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_srs_Bandwidth_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_srs_HoppingBandwidth,
{ "srs-HoppingBandwidth", "lte-rrc.srs_HoppingBandwidth",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_srs_HoppingBandwidth_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_freqDomainPosition,
{ "freqDomainPosition", "lte-rrc.freqDomainPosition",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_23", HFILL }},
{ &hf_lte_rrc_duration,
{ "duration", "lte-rrc.duration",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_duration_val), 0,
NULL, HFILL }},
{ &hf_lte_rrc_srs_ConfigIndex,
{ "srs-ConfigIndex", "lte-rrc.srs_ConfigIndex",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_1023", HFILL }},
{ &hf_lte_rrc_transmissionComb,
{ "transmissionComb", "lte-rrc.transmissionComb",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_1", HFILL }},
{ &hf_lte_rrc_cyclicShift_01,
{ "cyclicShift", "lte-rrc.cyclicShift",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cyclicShift_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_srs_AntennaPort_r10,
{ "srs-AntennaPort-r10", "lte-rrc.srs_AntennaPort_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SRS_AntennaPort_vals), 0,
"SRS_AntennaPort", HFILL }},
{ &hf_lte_rrc_setup_30,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_28", HFILL }},
{ &hf_lte_rrc_srs_ConfigIndexAp_r10,
{ "srs-ConfigIndexAp-r10", "lte-rrc.srs_ConfigIndexAp_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_31", HFILL }},
{ &hf_lte_rrc_srs_ConfigApDCI_Format4_r10,
{ "srs-ConfigApDCI-Format4-r10", "lte-rrc.srs_ConfigApDCI_Format4_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_3_OF_SRS_ConfigAp_r10", HFILL }},
{ &hf_lte_rrc_srs_ConfigApDCI_Format4_r10_item,
{ "SRS-ConfigAp-r10", "lte-rrc.SRS_ConfigAp_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_srs_ActivateAp_r10,
{ "srs-ActivateAp-r10", "lte-rrc.srs_ActivateAp_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_srs_ActivateAp_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_31,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_29", HFILL }},
{ &hf_lte_rrc_srs_ConfigApDCI_Format0_r10,
{ "srs-ConfigApDCI-Format0-r10", "lte-rrc.srs_ConfigApDCI_Format0_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"SRS_ConfigAp_r10", HFILL }},
{ &hf_lte_rrc_srs_ConfigApDCI_Format1a2b2c_r10,
{ "srs-ConfigApDCI-Format1a2b2c-r10", "lte-rrc.srs_ConfigApDCI_Format1a2b2c_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"SRS_ConfigAp_r10", HFILL }},
{ &hf_lte_rrc_srs_AntennaPortAp_r10,
{ "srs-AntennaPortAp-r10", "lte-rrc.srs_AntennaPortAp_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SRS_AntennaPort_vals), 0,
"SRS_AntennaPort", HFILL }},
{ &hf_lte_rrc_srs_BandwidthAp_r10,
{ "srs-BandwidthAp-r10", "lte-rrc.srs_BandwidthAp_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_srs_BandwidthAp_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_freqDomainPositionAp_r10,
{ "freqDomainPositionAp-r10", "lte-rrc.freqDomainPositionAp_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_23", HFILL }},
{ &hf_lte_rrc_transmissionCombAp_r10,
{ "transmissionCombAp-r10", "lte-rrc.transmissionCombAp_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_1", HFILL }},
{ &hf_lte_rrc_cyclicShiftAp_r10,
{ "cyclicShiftAp-r10", "lte-rrc.cyclicShiftAp_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cyclicShiftAp_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_semiPersistSchedC_RNTI,
{ "semiPersistSchedC-RNTI", "lte-rrc.semiPersistSchedC_RNTI",
FT_BYTES, BASE_NONE, NULL, 0,
"C_RNTI", HFILL }},
{ &hf_lte_rrc_sps_ConfigDL,
{ "sps-ConfigDL", "lte-rrc.sps_ConfigDL",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SPS_ConfigDL_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_sps_ConfigUL,
{ "sps-ConfigUL", "lte-rrc.sps_ConfigUL",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SPS_ConfigUL_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_32,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_30", HFILL }},
{ &hf_lte_rrc_semiPersistSchedIntervalDL,
{ "semiPersistSchedIntervalDL", "lte-rrc.semiPersistSchedIntervalDL",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_semiPersistSchedIntervalDL_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_numberOfConfSPS_Processes,
{ "numberOfConfSPS-Processes", "lte-rrc.numberOfConfSPS_Processes",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_8", HFILL }},
{ &hf_lte_rrc_n1PUCCH_AN_PersistentList,
{ "n1PUCCH-AN-PersistentList", "lte-rrc.n1PUCCH_AN_PersistentList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_twoAntennaPortActivated_r10,
{ "twoAntennaPortActivated-r10", "lte-rrc.twoAntennaPortActivated_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_twoAntennaPortActivated_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_33,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_31", HFILL }},
{ &hf_lte_rrc_n1PUCCH_AN_PersistentListP1_r10,
{ "n1PUCCH-AN-PersistentListP1-r10", "lte-rrc.n1PUCCH_AN_PersistentListP1_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"N1PUCCH_AN_PersistentList", HFILL }},
{ &hf_lte_rrc_setup_34,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_32", HFILL }},
{ &hf_lte_rrc_semiPersistSchedIntervalUL,
{ "semiPersistSchedIntervalUL", "lte-rrc.semiPersistSchedIntervalUL",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_semiPersistSchedIntervalUL_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_implicitReleaseAfter,
{ "implicitReleaseAfter", "lte-rrc.implicitReleaseAfter",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_implicitReleaseAfter_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_p0_Persistent,
{ "p0-Persistent", "lte-rrc.p0_Persistent_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_p0_NominalPUSCH_Persistent,
{ "p0-NominalPUSCH-Persistent", "lte-rrc.p0_NominalPUSCH_Persistent",
FT_INT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_p0_UE_PUSCH_Persistent,
{ "p0-UE-PUSCH-Persistent", "lte-rrc.p0_UE_PUSCH_Persistent",
FT_INT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_twoIntervalsConfig,
{ "twoIntervalsConfig", "lte-rrc.twoIntervalsConfig",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_twoIntervalsConfig_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_N1PUCCH_AN_PersistentList_item,
{ "N1PUCCH-AN-PersistentList item", "lte-rrc.N1PUCCH_AN_PersistentList_item",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_2047", HFILL }},
{ &hf_lte_rrc_subframeAssignment,
{ "subframeAssignment", "lte-rrc.subframeAssignment",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_subframeAssignment_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_specialSubframePatterns,
{ "specialSubframePatterns", "lte-rrc.specialSubframePatterns",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_specialSubframePatterns_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_specialSubframePatterns_v1130,
{ "specialSubframePatterns-v1130", "lte-rrc.specialSubframePatterns_v1130",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_specialSubframePatterns_v1130_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_35,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_33", HFILL }},
{ &hf_lte_rrc_tpc_RNTI,
{ "tpc-RNTI", "lte-rrc.tpc_RNTI",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_16", HFILL }},
{ &hf_lte_rrc_tpc_Index,
{ "tpc-Index", "lte-rrc.tpc_Index",
FT_UINT32, BASE_DEC, VALS(lte_rrc_TPC_Index_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_indexOfFormat3,
{ "indexOfFormat3", "lte-rrc.indexOfFormat3",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_15", HFILL }},
{ &hf_lte_rrc_indexOfFormat3A,
{ "indexOfFormat3A", "lte-rrc.indexOfFormat3A",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_31", HFILL }},
{ &hf_lte_rrc_p0_NominalPUSCH,
{ "p0-NominalPUSCH", "lte-rrc.p0_NominalPUSCH",
FT_INT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_alpha,
{ "alpha", "lte-rrc.alpha",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_alpha_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_p0_NominalPUCCH,
{ "p0-NominalPUCCH", "lte-rrc.p0_NominalPUCCH",
FT_INT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaFList_PUCCH,
{ "deltaFList-PUCCH", "lte-rrc.deltaFList_PUCCH_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaPreambleMsg3,
{ "deltaPreambleMsg3", "lte-rrc.deltaPreambleMsg3",
FT_INT32, BASE_DEC, VALS(lte_rrc_deltaPreambleMsg3_vals), 0,
"INTEGER_M1_6", HFILL }},
{ &hf_lte_rrc_deltaF_PUCCH_Format3_r10,
{ "deltaF-PUCCH-Format3-r10", "lte-rrc.deltaF_PUCCH_Format3_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deltaF_PUCCH_Format3_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaF_PUCCH_Format1bCS_r10,
{ "deltaF-PUCCH-Format1bCS-r10", "lte-rrc.deltaF_PUCCH_Format1bCS_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deltaF_PUCCH_Format1bCS_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_p0_NominalPUSCH_r10,
{ "p0-NominalPUSCH-r10", "lte-rrc.p0_NominalPUSCH_r10",
FT_INT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_alpha_r10,
{ "alpha-r10", "lte-rrc.alpha_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_alpha_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaPreambleMsg3_r11,
{ "deltaPreambleMsg3-r11", "lte-rrc.deltaPreambleMsg3_r11",
FT_INT32, BASE_DEC, NULL, 0,
"INTEGER_M1_6", HFILL }},
{ &hf_lte_rrc_p0_UE_PUSCH,
{ "p0-UE-PUSCH", "lte-rrc.p0_UE_PUSCH",
FT_INT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaMCS_Enabled,
{ "deltaMCS-Enabled", "lte-rrc.deltaMCS_Enabled",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deltaMCS_Enabled_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_accumulationEnabled,
{ "accumulationEnabled", "lte-rrc.accumulationEnabled",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_p0_UE_PUCCH,
{ "p0-UE-PUCCH", "lte-rrc.p0_UE_PUCCH",
FT_INT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pSRS_Offset,
{ "pSRS-Offset", "lte-rrc.pSRS_Offset",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_15", HFILL }},
{ &hf_lte_rrc_filterCoefficient,
{ "filterCoefficient", "lte-rrc.filterCoefficient",
FT_UINT32, BASE_DEC, VALS(lte_rrc_FilterCoefficient_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaTxD_OffsetListPUCCH_r10,
{ "deltaTxD-OffsetListPUCCH-r10", "lte-rrc.deltaTxD_OffsetListPUCCH_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pSRS_OffsetAp_r10,
{ "pSRS-OffsetAp-r10", "lte-rrc.pSRS_OffsetAp_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_15", HFILL }},
{ &hf_lte_rrc_pSRS_Offset_v1130,
{ "pSRS-Offset-v1130", "lte-rrc.pSRS_Offset_v1130",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_16_31", HFILL }},
{ &hf_lte_rrc_pSRS_OffsetAp_v1130,
{ "pSRS-OffsetAp-v1130", "lte-rrc.pSRS_OffsetAp_v1130",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_16_31", HFILL }},
{ &hf_lte_rrc_deltaTxD_OffsetListPUCCH_v1130,
{ "deltaTxD-OffsetListPUCCH-v1130", "lte-rrc.deltaTxD_OffsetListPUCCH_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_p0_UE_PUSCH_r10,
{ "p0-UE-PUSCH-r10", "lte-rrc.p0_UE_PUSCH_r10",
FT_INT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaMCS_Enabled_r10,
{ "deltaMCS-Enabled-r10", "lte-rrc.deltaMCS_Enabled_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deltaMCS_Enabled_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_accumulationEnabled_r10,
{ "accumulationEnabled-r10", "lte-rrc.accumulationEnabled_r10",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_pSRS_Offset_r10,
{ "pSRS-Offset-r10", "lte-rrc.pSRS_Offset_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_15", HFILL }},
{ &hf_lte_rrc_filterCoefficient_r10,
{ "filterCoefficient-r10", "lte-rrc.filterCoefficient_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_FilterCoefficient_vals), 0,
"FilterCoefficient", HFILL }},
{ &hf_lte_rrc_pathlossReferenceLinking_r10,
{ "pathlossReferenceLinking-r10", "lte-rrc.pathlossReferenceLinking_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_pathlossReferenceLinking_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaF_PUCCH_Format1,
{ "deltaF-PUCCH-Format1", "lte-rrc.deltaF_PUCCH_Format1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deltaF_PUCCH_Format1_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaF_PUCCH_Format1b,
{ "deltaF-PUCCH-Format1b", "lte-rrc.deltaF_PUCCH_Format1b",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deltaF_PUCCH_Format1b_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaF_PUCCH_Format2,
{ "deltaF-PUCCH-Format2", "lte-rrc.deltaF_PUCCH_Format2",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deltaF_PUCCH_Format2_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaF_PUCCH_Format2a,
{ "deltaF-PUCCH-Format2a", "lte-rrc.deltaF_PUCCH_Format2a",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deltaF_PUCCH_Format2a_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaF_PUCCH_Format2b,
{ "deltaF-PUCCH-Format2b", "lte-rrc.deltaF_PUCCH_Format2b",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deltaF_PUCCH_Format2b_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaTxD_OffsetPUCCH_Format1_r10,
{ "deltaTxD-OffsetPUCCH-Format1-r10", "lte-rrc.deltaTxD_OffsetPUCCH_Format1_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deltaTxD_OffsetPUCCH_Format1_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaTxD_OffsetPUCCH_Format1a1b_r10,
{ "deltaTxD-OffsetPUCCH-Format1a1b-r10", "lte-rrc.deltaTxD_OffsetPUCCH_Format1a1b_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deltaTxD_OffsetPUCCH_Format1a1b_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaTxD_OffsetPUCCH_Format22a2b_r10,
{ "deltaTxD-OffsetPUCCH-Format22a2b-r10", "lte-rrc.deltaTxD_OffsetPUCCH_Format22a2b_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deltaTxD_OffsetPUCCH_Format22a2b_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaTxD_OffsetPUCCH_Format3_r10,
{ "deltaTxD-OffsetPUCCH-Format3-r10", "lte-rrc.deltaTxD_OffsetPUCCH_Format3_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deltaTxD_OffsetPUCCH_Format3_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_deltaTxD_OffsetPUCCH_Format1bCS_r11,
{ "deltaTxD-OffsetPUCCH-Format1bCS-r11", "lte-rrc.deltaTxD_OffsetPUCCH_Format1bCS_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deltaTxD_OffsetPUCCH_Format1bCS_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_cipheringAlgorithm,
{ "cipheringAlgorithm", "lte-rrc.cipheringAlgorithm",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cipheringAlgorithm_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_integrityProtAlgorithm,
{ "integrityProtAlgorithm", "lte-rrc.integrityProtAlgorithm",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_integrityProtAlgorithm_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_arfcn_01,
{ "arfcn", "lte-rrc.arfcn",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueGERAN", HFILL }},
{ &hf_lte_rrc_startingARFCN,
{ "startingARFCN", "lte-rrc.startingARFCN",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueGERAN", HFILL }},
{ &hf_lte_rrc_followingARFCNs,
{ "followingARFCNs", "lte-rrc.followingARFCNs",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_followingARFCNs_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_explicitListOfARFCNs,
{ "explicitListOfARFCNs", "lte-rrc.explicitListOfARFCNs",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_equallySpacedARFCNs,
{ "equallySpacedARFCNs", "lte-rrc.equallySpacedARFCNs_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_arfcn_Spacing,
{ "arfcn-Spacing", "lte-rrc.arfcn_Spacing",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_8", HFILL }},
{ &hf_lte_rrc_numberOfFollowingARFCNs,
{ "numberOfFollowingARFCNs", "lte-rrc.numberOfFollowingARFCNs",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_31", HFILL }},
{ &hf_lte_rrc_variableBitMapOfARFCNs,
{ "variableBitMapOfARFCNs", "lte-rrc.variableBitMapOfARFCNs",
FT_BYTES, BASE_NONE, NULL, 0,
"OCTET_STRING_SIZE_1_16", HFILL }},
{ &hf_lte_rrc_ExplicitListOfARFCNs_item,
{ "ARFCN-ValueGERAN", "lte-rrc.ARFCN_ValueGERAN",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CarrierFreqListMBMS_r11_item,
{ "ARFCN-ValueEUTRA-r9", "lte-rrc.ARFCN_ValueEUTRA_r9",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CellIndexList_item,
{ "CellIndex", "lte-rrc.CellIndex",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sid,
{ "sid", "lte-rrc.sid",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_15", HFILL }},
{ &hf_lte_rrc_nid,
{ "nid", "lte-rrc.nid",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_16", HFILL }},
{ &hf_lte_rrc_multipleSID,
{ "multipleSID", "lte-rrc.multipleSID",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_multipleNID,
{ "multipleNID", "lte-rrc.multipleNID",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_homeReg,
{ "homeReg", "lte-rrc.homeReg",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_foreignSIDReg,
{ "foreignSIDReg", "lte-rrc.foreignSIDReg",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_foreignNIDReg,
{ "foreignNIDReg", "lte-rrc.foreignNIDReg",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_parameterReg,
{ "parameterReg", "lte-rrc.parameterReg",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_powerUpReg,
{ "powerUpReg", "lte-rrc.powerUpReg",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_registrationPeriod,
{ "registrationPeriod", "lte-rrc.registrationPeriod",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_7", HFILL }},
{ &hf_lte_rrc_registrationZone,
{ "registrationZone", "lte-rrc.registrationZone",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_12", HFILL }},
{ &hf_lte_rrc_totalZone,
{ "totalZone", "lte-rrc.totalZone",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_3", HFILL }},
{ &hf_lte_rrc_zoneTimer,
{ "zoneTimer", "lte-rrc.zoneTimer",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_3", HFILL }},
{ &hf_lte_rrc_powerDownReg_r9,
{ "powerDownReg-r9", "lte-rrc.powerDownReg_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_powerDownReg_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellIdentity_01,
{ "cellIdentity", "lte-rrc.cellIdentity",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_28", HFILL }},
{ &hf_lte_rrc_locationAreaCode,
{ "locationAreaCode", "lte-rrc.locationAreaCode",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_16", HFILL }},
{ &hf_lte_rrc_cellIdentity_02,
{ "cellIdentity", "lte-rrc.cellIdentity",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_16", HFILL }},
{ &hf_lte_rrc_cellGlobalId1XRTT,
{ "cellGlobalId1XRTT", "lte-rrc.cellGlobalId1XRTT",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_47", HFILL }},
{ &hf_lte_rrc_cellGlobalIdHRPD,
{ "cellGlobalIdHRPD", "lte-rrc.cellGlobalIdHRPD",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_128", HFILL }},
{ &hf_lte_rrc_targetPhysCellId,
{ "targetPhysCellId", "lte-rrc.targetPhysCellId",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellId", HFILL }},
{ &hf_lte_rrc_carrierFreq_03,
{ "carrierFreq", "lte-rrc.carrierFreq_element",
FT_NONE, BASE_NONE, NULL, 0,
"CarrierFreqEUTRA", HFILL }},
{ &hf_lte_rrc_carrierBandwidth,
{ "carrierBandwidth", "lte-rrc.carrierBandwidth_element",
FT_NONE, BASE_NONE, NULL, 0,
"CarrierBandwidthEUTRA", HFILL }},
{ &hf_lte_rrc_t304_01,
{ "t304", "lte-rrc.t304",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_t304_01_vals), 0,
"T_t304_01", HFILL }},
{ &hf_lte_rrc_newUE_Identity,
{ "newUE-Identity", "lte-rrc.newUE_Identity",
FT_BYTES, BASE_NONE, NULL, 0,
"C_RNTI", HFILL }},
{ &hf_lte_rrc_radioResourceConfigCommon_01,
{ "radioResourceConfigCommon", "lte-rrc.radioResourceConfigCommon_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rach_ConfigDedicated,
{ "rach-ConfigDedicated", "lte-rrc.rach_ConfigDedicated_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_carrierFreq_v9e0_01,
{ "carrierFreq-v9e0", "lte-rrc.carrierFreq_v9e0_element",
FT_NONE, BASE_NONE, NULL, 0,
"CarrierFreqEUTRA_v9e0", HFILL }},
{ &hf_lte_rrc_drb_ContinueROHC_r11,
{ "drb-ContinueROHC-r11", "lte-rrc.drb_ContinueROHC_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_drb_ContinueROHC_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_dl_Bandwidth_01,
{ "dl-Bandwidth", "lte-rrc.dl_Bandwidth",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_dl_Bandwidth_01_vals), 0,
"T_dl_Bandwidth_01", HFILL }},
{ &hf_lte_rrc_ul_Bandwidth_01,
{ "ul-Bandwidth", "lte-rrc.ul_Bandwidth",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_ul_Bandwidth_01_vals), 0,
"T_ul_Bandwidth_01", HFILL }},
{ &hf_lte_rrc_dl_CarrierFreq_v9e0_01,
{ "dl-CarrierFreq-v9e0", "lte-rrc.dl_CarrierFreq_v9e0",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA_r9", HFILL }},
{ &hf_lte_rrc_ul_CarrierFreq_v9e0_01,
{ "ul-CarrierFreq-v9e0", "lte-rrc.ul_CarrierFreq_v9e0",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA_r9", HFILL }},
{ &hf_lte_rrc_t_Evaluation,
{ "t-Evaluation", "lte-rrc.t_Evaluation",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_t_Evaluation_vals), 0,
"T_t_Evaluation", HFILL }},
{ &hf_lte_rrc_t_HystNormal,
{ "t-HystNormal", "lte-rrc.t_HystNormal",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_t_HystNormal_vals), 0,
"T_t_HystNormal", HFILL }},
{ &hf_lte_rrc_n_CellChangeMedium,
{ "n-CellChangeMedium", "lte-rrc.n_CellChangeMedium",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_16", HFILL }},
{ &hf_lte_rrc_n_CellChangeHigh,
{ "n-CellChangeHigh", "lte-rrc.n_CellChangeHigh",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_16", HFILL }},
{ &hf_lte_rrc_MultiBandInfoList_item,
{ "FreqBandIndicator", "lte-rrc.FreqBandIndicator",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MultiBandInfoList_v9e0_item,
{ "MultiBandInfo-v9e0", "lte-rrc.MultiBandInfo_v9e0_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MultiBandInfoList_r11_item,
{ "FreqBandIndicator-r11", "lte-rrc.FreqBandIndicator_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_start_01,
{ "start", "lte-rrc.start",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellId", HFILL }},
{ &hf_lte_rrc_range,
{ "range", "lte-rrc.range",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_range_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_PhysCellIdRangeUTRA_FDDList_r9_item,
{ "PhysCellIdRangeUTRA-FDD-r9", "lte-rrc.PhysCellIdRangeUTRA_FDD_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_start_r9,
{ "start-r9", "lte-rrc.start_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellIdUTRA_FDD", HFILL }},
{ &hf_lte_rrc_range_r9,
{ "range-r9", "lte-rrc.range_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_2_512", HFILL }},
{ &hf_lte_rrc_networkColourCode,
{ "networkColourCode", "lte-rrc.networkColourCode",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_3", HFILL }},
{ &hf_lte_rrc_baseStationColourCode,
{ "baseStationColourCode", "lte-rrc.baseStationColourCode",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_3", HFILL }},
{ &hf_lte_rrc_mcc,
{ "mcc", "lte-rrc.mcc",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mnc,
{ "mnc", "lte-rrc.mnc",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MCC_item,
{ "MCC-MNC-Digit", "lte-rrc.MCC_MNC_Digit",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MNC_item,
{ "MCC-MNC-Digit", "lte-rrc.MCC_MNC_Digit",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_PLMN_IdentityList3_r11_item,
{ "PLMN-Identity", "lte-rrc.PLMN_Identity_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_preRegistrationAllowed,
{ "preRegistrationAllowed", "lte-rrc.preRegistrationAllowed",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_preRegistrationZoneId,
{ "preRegistrationZoneId", "lte-rrc.preRegistrationZoneId",
FT_UINT32, BASE_DEC, NULL, 0,
"PreRegistrationZoneIdHRPD", HFILL }},
{ &hf_lte_rrc_secondaryPreRegistrationZoneIdList,
{ "secondaryPreRegistrationZoneIdList", "lte-rrc.secondaryPreRegistrationZoneIdList",
FT_UINT32, BASE_DEC, NULL, 0,
"SecondaryPreRegistrationZoneIdListHRPD", HFILL }},
{ &hf_lte_rrc_SecondaryPreRegistrationZoneIdListHRPD_item,
{ "PreRegistrationZoneIdHRPD", "lte-rrc.PreRegistrationZoneIdHRPD",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf_Medium_01,
{ "sf-Medium", "lte-rrc.sf_Medium",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_sf_Medium_01_vals), 0,
"T_sf_Medium_01", HFILL }},
{ &hf_lte_rrc_sf_High_01,
{ "sf-High", "lte-rrc.sf_High",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_sf_High_01_vals), 0,
"T_sf_High_01", HFILL }},
{ &hf_lte_rrc_SystemInfoListGERAN_item,
{ "SystemInfoListGERAN item", "lte-rrc.SystemInfoListGERAN_item",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cdma_EUTRA_Synchronisation,
{ "cdma-EUTRA-Synchronisation", "lte-rrc.cdma_EUTRA_Synchronisation",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_cdma_SystemTime,
{ "cdma-SystemTime", "lte-rrc.cdma_SystemTime",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cdma_SystemTime_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_synchronousSystemTime,
{ "synchronousSystemTime", "lte-rrc.synchronousSystemTime",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_asynchronousSystemTime,
{ "asynchronousSystemTime", "lte-rrc.asynchronousSystemTime",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_locationCoordinates_r10,
{ "locationCoordinates-r10", "lte-rrc.locationCoordinates_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_locationCoordinates_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_ellipsoid_Point_r10,
{ "ellipsoid-Point-r10", "lte-rrc.ellipsoid_Point_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"T_ellipsoid_Point_r10", HFILL }},
{ &hf_lte_rrc_ellipsoidPointWithAltitude_r10,
{ "ellipsoidPointWithAltitude-r10", "lte-rrc.ellipsoidPointWithAltitude_r10",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ellipsoidPointWithUncertaintyCircle_r11,
{ "ellipsoidPointWithUncertaintyCircle-r11", "lte-rrc.ellipsoidPointWithUncertaintyCircle_r11",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ellipsoidPointWithUncertaintyEllipse_r11,
{ "ellipsoidPointWithUncertaintyEllipse-r11", "lte-rrc.ellipsoidPointWithUncertaintyEllipse_r11",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ellipsoidPointWithAltitudeAndUncertaintyEllipsoid_r11,
{ "ellipsoidPointWithAltitudeAndUncertaintyEllipsoid-r11", "lte-rrc.ellipsoidPointWithAltitudeAndUncertaintyEllipsoid_r11",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ellipsoidArc_r11,
{ "ellipsoidArc-r11", "lte-rrc.ellipsoidArc_r11",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_polygon_r11,
{ "polygon-r11", "lte-rrc.polygon_r11",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_horizontalVelocity_r10,
{ "horizontalVelocity-r10", "lte-rrc.horizontalVelocity_r10",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_gnss_TOD_msec_r10,
{ "gnss-TOD-msec-r10", "lte-rrc.gnss_TOD_msec_r10",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measObjectToRemoveList,
{ "measObjectToRemoveList", "lte-rrc.measObjectToRemoveList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measObjectToAddModList,
{ "measObjectToAddModList", "lte-rrc.measObjectToAddModList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_reportConfigToRemoveList,
{ "reportConfigToRemoveList", "lte-rrc.reportConfigToRemoveList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_reportConfigToAddModList,
{ "reportConfigToAddModList", "lte-rrc.reportConfigToAddModList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measIdToRemoveList,
{ "measIdToRemoveList", "lte-rrc.measIdToRemoveList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measIdToAddModList,
{ "measIdToAddModList", "lte-rrc.measIdToAddModList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_quantityConfig,
{ "quantityConfig", "lte-rrc.quantityConfig_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measGapConfig,
{ "measGapConfig", "lte-rrc.measGapConfig",
FT_UINT32, BASE_DEC, VALS(lte_rrc_MeasGapConfig_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_s_Measure,
{ "s-Measure", "lte-rrc.s_Measure",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_s_Measure_vals_ext, 0,
NULL, HFILL }},
{ &hf_lte_rrc_speedStatePars,
{ "speedStatePars", "lte-rrc.speedStatePars",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_speedStatePars_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_36,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_34", HFILL }},
{ &hf_lte_rrc_timeToTrigger_SF,
{ "timeToTrigger-SF", "lte-rrc.timeToTrigger_SF_element",
FT_NONE, BASE_NONE, NULL, 0,
"SpeedStateScaleFactors", HFILL }},
{ &hf_lte_rrc_measObjectToAddModList_v9e0,
{ "measObjectToAddModList-v9e0", "lte-rrc.measObjectToAddModList_v9e0",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MeasIdToRemoveList_item,
{ "MeasId", "lte-rrc.MeasId",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MeasObjectToRemoveList_item,
{ "MeasObjectId", "lte-rrc.MeasObjectId",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ReportConfigToRemoveList_item,
{ "ReportConfigId", "lte-rrc.ReportConfigId",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_37,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_35", HFILL }},
{ &hf_lte_rrc_gapOffset,
{ "gapOffset", "lte-rrc.gapOffset",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_gapOffset_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_gp0,
{ "gp0", "lte-rrc.gp0",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_39", HFILL }},
{ &hf_lte_rrc_gp1,
{ "gp1", "lte-rrc.gp1",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_79", HFILL }},
{ &hf_lte_rrc_MeasIdToAddModList_item,
{ "MeasIdToAddMod", "lte-rrc.MeasIdToAddMod_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measId,
{ "measId", "lte-rrc.measId",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measObjectId,
{ "measObjectId", "lte-rrc.measObjectId",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_reportConfigId,
{ "reportConfigId", "lte-rrc.reportConfigId",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_carrierFreq_04,
{ "carrierFreq", "lte-rrc.carrierFreq_element",
FT_NONE, BASE_NONE, NULL, 0,
"CarrierFreqCDMA2000", HFILL }},
{ &hf_lte_rrc_offsetFreq,
{ "offsetFreq", "lte-rrc.offsetFreq",
FT_INT32, BASE_DEC, NULL, 0,
"Q_OffsetRangeInterRAT", HFILL }},
{ &hf_lte_rrc_cellsToRemoveList,
{ "cellsToRemoveList", "lte-rrc.cellsToRemoveList",
FT_UINT32, BASE_DEC, NULL, 0,
"CellIndexList", HFILL }},
{ &hf_lte_rrc_cellsToAddModList,
{ "cellsToAddModList", "lte-rrc.cellsToAddModList",
FT_UINT32, BASE_DEC, NULL, 0,
"CellsToAddModListCDMA2000", HFILL }},
{ &hf_lte_rrc_cellForWhichToReportCGI,
{ "cellForWhichToReportCGI", "lte-rrc.cellForWhichToReportCGI",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellIdCDMA2000", HFILL }},
{ &hf_lte_rrc_CellsToAddModListCDMA2000_item,
{ "CellsToAddModCDMA2000", "lte-rrc.CellsToAddModCDMA2000_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellIndex,
{ "cellIndex", "lte-rrc.cellIndex",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_maxCellMeas", HFILL }},
{ &hf_lte_rrc_physCellId_02,
{ "physCellId", "lte-rrc.physCellId",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellIdCDMA2000", HFILL }},
{ &hf_lte_rrc_offsetFreq_01,
{ "offsetFreq", "lte-rrc.offsetFreq",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_Q_OffsetRange_vals_ext, 0,
"Q_OffsetRange", HFILL }},
{ &hf_lte_rrc_cellsToAddModList_01,
{ "cellsToAddModList", "lte-rrc.cellsToAddModList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_blackCellsToRemoveList,
{ "blackCellsToRemoveList", "lte-rrc.blackCellsToRemoveList",
FT_UINT32, BASE_DEC, NULL, 0,
"CellIndexList", HFILL }},
{ &hf_lte_rrc_blackCellsToAddModList,
{ "blackCellsToAddModList", "lte-rrc.blackCellsToAddModList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellForWhichToReportCGI_01,
{ "cellForWhichToReportCGI", "lte-rrc.cellForWhichToReportCGI",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellId", HFILL }},
{ &hf_lte_rrc_measCycleSCell_r10,
{ "measCycleSCell-r10", "lte-rrc.measCycleSCell_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_MeasCycleSCell_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_measSubframePatternConfigNeigh_r10,
{ "measSubframePatternConfigNeigh-r10", "lte-rrc.measSubframePatternConfigNeigh_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_MeasSubframePatternConfigNeigh_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_widebandRSRQ_Meas_r11,
{ "widebandRSRQ-Meas-r11", "lte-rrc.widebandRSRQ_Meas_r11",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_CellsToAddModList_item,
{ "CellsToAddMod", "lte-rrc.CellsToAddMod_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellIndividualOffset,
{ "cellIndividualOffset", "lte-rrc.cellIndividualOffset",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_Q_OffsetRange_vals_ext, 0,
"Q_OffsetRange", HFILL }},
{ &hf_lte_rrc_BlackCellsToAddModList_item,
{ "BlackCellsToAddMod", "lte-rrc.BlackCellsToAddMod_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_physCellIdRange,
{ "physCellIdRange", "lte-rrc.physCellIdRange_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_38,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_36", HFILL }},
{ &hf_lte_rrc_measSubframePatternNeigh_r10,
{ "measSubframePatternNeigh-r10", "lte-rrc.measSubframePatternNeigh_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_MeasSubframePattern_r10_vals), 0,
"MeasSubframePattern_r10", HFILL }},
{ &hf_lte_rrc_measSubframeCellList_r10,
{ "measSubframeCellList-r10", "lte-rrc.measSubframeCellList_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MeasSubframeCellList_r10_item,
{ "PhysCellIdRange", "lte-rrc.PhysCellIdRange_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellForWhichToReportCGI_02,
{ "cellForWhichToReportCGI", "lte-rrc.cellForWhichToReportCGI_element",
FT_NONE, BASE_NONE, NULL, 0,
"PhysCellIdGERAN", HFILL }},
{ &hf_lte_rrc_MeasObjectToAddModList_item,
{ "MeasObjectToAddMod", "lte-rrc.MeasObjectToAddMod_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MeasObjectToAddModList_v9e0_item,
{ "MeasObjectToAddMod-v9e0", "lte-rrc.MeasObjectToAddMod_v9e0_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measObject,
{ "measObject", "lte-rrc.measObject",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_measObject_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_measObjectEUTRA,
{ "measObjectEUTRA", "lte-rrc.measObjectEUTRA_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measObjectUTRA,
{ "measObjectUTRA", "lte-rrc.measObjectUTRA_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measObjectGERAN,
{ "measObjectGERAN", "lte-rrc.measObjectGERAN_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measObjectCDMA2000,
{ "measObjectCDMA2000", "lte-rrc.measObjectCDMA2000_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measObjectEUTRA_v9e0,
{ "measObjectEUTRA-v9e0", "lte-rrc.measObjectEUTRA_v9e0_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellsToAddModList_02,
{ "cellsToAddModList", "lte-rrc.cellsToAddModList",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cellsToAddModList_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellsToAddModListUTRA_FDD,
{ "cellsToAddModListUTRA-FDD", "lte-rrc.cellsToAddModListUTRA_FDD",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellsToAddModListUTRA_TDD,
{ "cellsToAddModListUTRA-TDD", "lte-rrc.cellsToAddModListUTRA_TDD",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellForWhichToReportCGI_03,
{ "cellForWhichToReportCGI", "lte-rrc.cellForWhichToReportCGI",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cellForWhichToReportCGI_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_utra_FDD_01,
{ "utra-FDD", "lte-rrc.utra_FDD",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellIdUTRA_FDD", HFILL }},
{ &hf_lte_rrc_utra_TDD_01,
{ "utra-TDD", "lte-rrc.utra_TDD",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellIdUTRA_TDD", HFILL }},
{ &hf_lte_rrc_csg_allowedReportingCells_v930,
{ "csg-allowedReportingCells-v930", "lte-rrc.csg_allowedReportingCells_v930_element",
FT_NONE, BASE_NONE, NULL, 0,
"CSG_AllowedReportingCells_r9", HFILL }},
{ &hf_lte_rrc_CellsToAddModListUTRA_FDD_item,
{ "CellsToAddModUTRA-FDD", "lte-rrc.CellsToAddModUTRA_FDD_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_physCellId_03,
{ "physCellId", "lte-rrc.physCellId",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellIdUTRA_FDD", HFILL }},
{ &hf_lte_rrc_CellsToAddModListUTRA_TDD_item,
{ "CellsToAddModUTRA-TDD", "lte-rrc.CellsToAddModUTRA_TDD_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_physCellId_04,
{ "physCellId", "lte-rrc.physCellId",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellIdUTRA_TDD", HFILL }},
{ &hf_lte_rrc_physCellIdRangeUTRA_FDDList_r9,
{ "physCellIdRangeUTRA-FDDList-r9", "lte-rrc.physCellIdRangeUTRA_FDDList_r9",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measResultPCell,
{ "measResultPCell", "lte-rrc.measResultPCell_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rsrpResult,
{ "rsrpResult", "lte-rrc.rsrpResult",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_RSRP_Range_vals_ext, 0,
"RSRP_Range", HFILL }},
{ &hf_lte_rrc_rsrqResult,
{ "rsrqResult", "lte-rrc.rsrqResult",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_RSRQ_Range_vals_ext, 0,
"RSRQ_Range", HFILL }},
{ &hf_lte_rrc_measResultNeighCells,
{ "measResultNeighCells", "lte-rrc.measResultNeighCells",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_measResultNeighCells_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_measResultListEUTRA,
{ "measResultListEUTRA", "lte-rrc.measResultListEUTRA",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measResultListUTRA,
{ "measResultListUTRA", "lte-rrc.measResultListUTRA",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measResultListGERAN,
{ "measResultListGERAN", "lte-rrc.measResultListGERAN",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measResultsCDMA2000,
{ "measResultsCDMA2000", "lte-rrc.measResultsCDMA2000_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measResultForECID_r9,
{ "measResultForECID-r9", "lte-rrc.measResultForECID_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measResultServFreqList_r10,
{ "measResultServFreqList-r10", "lte-rrc.measResultServFreqList_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MeasResultListEUTRA_item,
{ "MeasResultEUTRA", "lte-rrc.MeasResultEUTRA_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cgi_Info,
{ "cgi-Info", "lte-rrc.cgi_Info_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellGlobalId,
{ "cellGlobalId", "lte-rrc.cellGlobalId_element",
FT_NONE, BASE_NONE, NULL, 0,
"CellGlobalIdEUTRA", HFILL }},
{ &hf_lte_rrc_plmn_IdentityList_01,
{ "plmn-IdentityList", "lte-rrc.plmn_IdentityList",
FT_UINT32, BASE_DEC, NULL, 0,
"PLMN_IdentityList2", HFILL }},
{ &hf_lte_rrc_measResult,
{ "measResult", "lte-rrc.measResult_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_additionalSI_Info_r9,
{ "additionalSI-Info-r9", "lte-rrc.additionalSI_Info_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MeasResultServFreqList_r10_item,
{ "MeasResultServFreq-r10", "lte-rrc.MeasResultServFreq_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_servFreqId_r10,
{ "servFreqId-r10", "lte-rrc.servFreqId_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"ServCellIndex_r10", HFILL }},
{ &hf_lte_rrc_measResultSCell_r10,
{ "measResultSCell-r10", "lte-rrc.measResultSCell_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rsrpResultSCell_r10,
{ "rsrpResultSCell-r10", "lte-rrc.rsrpResultSCell_r10",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_RSRP_Range_vals_ext, 0,
"RSRP_Range", HFILL }},
{ &hf_lte_rrc_rsrqResultSCell_r10,
{ "rsrqResultSCell-r10", "lte-rrc.rsrqResultSCell_r10",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_RSRQ_Range_vals_ext, 0,
"RSRQ_Range", HFILL }},
{ &hf_lte_rrc_measResultBestNeighCell_r10,
{ "measResultBestNeighCell-r10", "lte-rrc.measResultBestNeighCell_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rsrpResultNCell_r10,
{ "rsrpResultNCell-r10", "lte-rrc.rsrpResultNCell_r10",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_RSRP_Range_vals_ext, 0,
"RSRP_Range", HFILL }},
{ &hf_lte_rrc_rsrqResultNCell_r10,
{ "rsrqResultNCell-r10", "lte-rrc.rsrqResultNCell_r10",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_RSRQ_Range_vals_ext, 0,
"RSRQ_Range", HFILL }},
{ &hf_lte_rrc_MeasResultListUTRA_item,
{ "MeasResultUTRA", "lte-rrc.MeasResultUTRA_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_physCellId_05,
{ "physCellId", "lte-rrc.physCellId",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_physCellId_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_fdd_01,
{ "fdd", "lte-rrc.fdd",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellIdUTRA_FDD", HFILL }},
{ &hf_lte_rrc_tdd_01,
{ "tdd", "lte-rrc.tdd",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellIdUTRA_TDD", HFILL }},
{ &hf_lte_rrc_cgi_Info_01,
{ "cgi-Info", "lte-rrc.cgi_Info_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_cgi_Info_01", HFILL }},
{ &hf_lte_rrc_cellGlobalId_01,
{ "cellGlobalId", "lte-rrc.cellGlobalId_element",
FT_NONE, BASE_NONE, NULL, 0,
"CellGlobalIdUTRA", HFILL }},
{ &hf_lte_rrc_routingAreaCode,
{ "routingAreaCode", "lte-rrc.routingAreaCode",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_8", HFILL }},
{ &hf_lte_rrc_measResult_01,
{ "measResult", "lte-rrc.measResult_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_measResult_01", HFILL }},
{ &hf_lte_rrc_utra_RSCP,
{ "utra-RSCP", "lte-rrc.utra_RSCP",
FT_INT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_utra_RSCP_vals_ext, 0,
"INTEGER_M5_91", HFILL }},
{ &hf_lte_rrc_utra_EcN0,
{ "utra-EcN0", "lte-rrc.utra_EcN0",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_utra_EcN0_vals_ext, 0,
"INTEGER_0_49", HFILL }},
{ &hf_lte_rrc_MeasResultListGERAN_item,
{ "MeasResultGERAN", "lte-rrc.MeasResultGERAN_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cgi_Info_02,
{ "cgi-Info", "lte-rrc.cgi_Info_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_cgi_Info_02", HFILL }},
{ &hf_lte_rrc_cellGlobalId_02,
{ "cellGlobalId", "lte-rrc.cellGlobalId_element",
FT_NONE, BASE_NONE, NULL, 0,
"CellGlobalIdGERAN", HFILL }},
{ &hf_lte_rrc_measResult_02,
{ "measResult", "lte-rrc.measResult_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_measResult_02", HFILL }},
{ &hf_lte_rrc_rssi,
{ "rssi", "lte-rrc.rssi",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &gsm_a_rr_rxlev_vals_ext, 0,
"INTEGER_0_63", HFILL }},
{ &hf_lte_rrc_preRegistrationStatusHRPD,
{ "preRegistrationStatusHRPD", "lte-rrc.preRegistrationStatusHRPD",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_measResultListCDMA2000,
{ "measResultListCDMA2000", "lte-rrc.measResultListCDMA2000",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MeasResultListCDMA2000_item,
{ "MeasResultCDMA2000", "lte-rrc.MeasResultCDMA2000_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cgi_Info_03,
{ "cgi-Info", "lte-rrc.cgi_Info",
FT_UINT32, BASE_DEC, VALS(lte_rrc_CellGlobalIdCDMA2000_vals), 0,
"CellGlobalIdCDMA2000", HFILL }},
{ &hf_lte_rrc_measResult_03,
{ "measResult", "lte-rrc.measResult_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_measResult_03", HFILL }},
{ &hf_lte_rrc_pilotPnPhase,
{ "pilotPnPhase", "lte-rrc.pilotPnPhase",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_32767", HFILL }},
{ &hf_lte_rrc_pilotStrength,
{ "pilotStrength", "lte-rrc.pilotStrength",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_63", HFILL }},
{ &hf_lte_rrc_ue_RxTxTimeDiffResult_r9,
{ "ue-RxTxTimeDiffResult-r9", "lte-rrc.ue_RxTxTimeDiffResult_r9",
FT_UINT32, BASE_CUSTOM, &lte_rrc_ue_RxTxTimeDiffResult_fmt, 0,
"INTEGER_0_4095", HFILL }},
{ &hf_lte_rrc_currentSFN_r9,
{ "currentSFN-r9", "lte-rrc.currentSFN_r9",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_10", HFILL }},
{ &hf_lte_rrc_PLMN_IdentityList2_item,
{ "PLMN-Identity", "lte-rrc.PLMN_Identity_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_csg_MemberStatus_r9,
{ "csg-MemberStatus-r9", "lte-rrc.csg_MemberStatus_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_csg_MemberStatus_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_csg_Identity_r9,
{ "csg-Identity-r9", "lte-rrc.csg_Identity_r9",
FT_BYTES, BASE_NONE, NULL, 0,
"CSG_Identity", HFILL }},
{ &hf_lte_rrc_quantityConfigEUTRA,
{ "quantityConfigEUTRA", "lte-rrc.quantityConfigEUTRA_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_quantityConfigUTRA,
{ "quantityConfigUTRA", "lte-rrc.quantityConfigUTRA_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_quantityConfigGERAN,
{ "quantityConfigGERAN", "lte-rrc.quantityConfigGERAN_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_quantityConfigCDMA2000,
{ "quantityConfigCDMA2000", "lte-rrc.quantityConfigCDMA2000_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_quantityConfigUTRA_v1020,
{ "quantityConfigUTRA-v1020", "lte-rrc.quantityConfigUTRA_v1020_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_filterCoefficientRSRP,
{ "filterCoefficientRSRP", "lte-rrc.filterCoefficientRSRP",
FT_UINT32, BASE_DEC, VALS(lte_rrc_FilterCoefficient_vals), 0,
"FilterCoefficient", HFILL }},
{ &hf_lte_rrc_filterCoefficientRSRQ,
{ "filterCoefficientRSRQ", "lte-rrc.filterCoefficientRSRQ",
FT_UINT32, BASE_DEC, VALS(lte_rrc_FilterCoefficient_vals), 0,
"FilterCoefficient", HFILL }},
{ &hf_lte_rrc_measQuantityUTRA_FDD,
{ "measQuantityUTRA-FDD", "lte-rrc.measQuantityUTRA_FDD",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_measQuantityUTRA_FDD_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_measQuantityUTRA_TDD,
{ "measQuantityUTRA-TDD", "lte-rrc.measQuantityUTRA_TDD",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_measQuantityUTRA_TDD_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_filterCoefficient2_FDD_r10,
{ "filterCoefficient2-FDD-r10", "lte-rrc.filterCoefficient2_FDD_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_FilterCoefficient_vals), 0,
"FilterCoefficient", HFILL }},
{ &hf_lte_rrc_measQuantityGERAN,
{ "measQuantityGERAN", "lte-rrc.measQuantityGERAN",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_measQuantityGERAN_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_measQuantityCDMA2000,
{ "measQuantityCDMA2000", "lte-rrc.measQuantityCDMA2000",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_measQuantityCDMA2000_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_triggerType,
{ "triggerType", "lte-rrc.triggerType",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_triggerType_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_event,
{ "event", "lte-rrc.event_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_eventId,
{ "eventId", "lte-rrc.eventId",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_eventId_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_eventA1,
{ "eventA1", "lte-rrc.eventA1_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_a1_Threshold,
{ "a1-Threshold", "lte-rrc.a1_Threshold",
FT_UINT32, BASE_DEC, VALS(lte_rrc_ThresholdEUTRA_vals), 0,
"ThresholdEUTRA", HFILL }},
{ &hf_lte_rrc_eventA2,
{ "eventA2", "lte-rrc.eventA2_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_a2_Threshold,
{ "a2-Threshold", "lte-rrc.a2_Threshold",
FT_UINT32, BASE_DEC, VALS(lte_rrc_ThresholdEUTRA_vals), 0,
"ThresholdEUTRA", HFILL }},
{ &hf_lte_rrc_eventA3,
{ "eventA3", "lte-rrc.eventA3_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_a3_Offset,
{ "a3-Offset", "lte-rrc.a3_Offset",
FT_INT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_a3_a6_Offset_vals_ext, 0,
"INTEGER_M30_30", HFILL }},
{ &hf_lte_rrc_reportOnLeave,
{ "reportOnLeave", "lte-rrc.reportOnLeave",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_eventA4,
{ "eventA4", "lte-rrc.eventA4_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_a4_Threshold,
{ "a4-Threshold", "lte-rrc.a4_Threshold",
FT_UINT32, BASE_DEC, VALS(lte_rrc_ThresholdEUTRA_vals), 0,
"ThresholdEUTRA", HFILL }},
{ &hf_lte_rrc_eventA5,
{ "eventA5", "lte-rrc.eventA5_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_a5_Threshold1,
{ "a5-Threshold1", "lte-rrc.a5_Threshold1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_ThresholdEUTRA_vals), 0,
"ThresholdEUTRA", HFILL }},
{ &hf_lte_rrc_a5_Threshold2,
{ "a5-Threshold2", "lte-rrc.a5_Threshold2",
FT_UINT32, BASE_DEC, VALS(lte_rrc_ThresholdEUTRA_vals), 0,
"ThresholdEUTRA", HFILL }},
{ &hf_lte_rrc_eventA6_r10,
{ "eventA6-r10", "lte-rrc.eventA6_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_a6_Offset_r10,
{ "a6-Offset-r10", "lte-rrc.a6_Offset_r10",
FT_INT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_a3_a6_Offset_vals_ext, 0,
"INTEGER_M30_30", HFILL }},
{ &hf_lte_rrc_a6_ReportOnLeave_r10,
{ "a6-ReportOnLeave-r10", "lte-rrc.a6_ReportOnLeave_r10",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_hysteresis,
{ "hysteresis", "lte-rrc.hysteresis",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_Hysteresis_vals_ext, 0,
NULL, HFILL }},
{ &hf_lte_rrc_timeToTrigger,
{ "timeToTrigger", "lte-rrc.timeToTrigger",
FT_UINT32, BASE_DEC, VALS(lte_rrc_TimeToTrigger_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_periodical,
{ "periodical", "lte-rrc.periodical_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_purpose_02,
{ "purpose", "lte-rrc.purpose",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_purpose_02_vals), 0,
"T_purpose_02", HFILL }},
{ &hf_lte_rrc_triggerQuantity,
{ "triggerQuantity", "lte-rrc.triggerQuantity",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_triggerQuantity_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_reportQuantity,
{ "reportQuantity", "lte-rrc.reportQuantity",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_reportQuantity_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_maxReportCells,
{ "maxReportCells", "lte-rrc.maxReportCells",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_maxCellReport", HFILL }},
{ &hf_lte_rrc_reportInterval,
{ "reportInterval", "lte-rrc.reportInterval",
FT_UINT32, BASE_DEC, VALS(lte_rrc_ReportInterval_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_reportAmount,
{ "reportAmount", "lte-rrc.reportAmount",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_reportAmount_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_si_RequestForHO_r9,
{ "si-RequestForHO-r9", "lte-rrc.si_RequestForHO_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_si_RequestForHO_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_ue_RxTxTimeDiffPeriodical_r9,
{ "ue-RxTxTimeDiffPeriodical-r9", "lte-rrc.ue_RxTxTimeDiffPeriodical_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_ue_RxTxTimeDiffPeriodical_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_includeLocationInfo_r10,
{ "includeLocationInfo-r10", "lte-rrc.includeLocationInfo_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_includeLocationInfo_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_reportAddNeighMeas_r10,
{ "reportAddNeighMeas-r10", "lte-rrc.reportAddNeighMeas_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_reportAddNeighMeas_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_threshold_RSRP,
{ "threshold-RSRP", "lte-rrc.threshold_RSRP",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_threshold_RSRP_vals_ext, 0,
"Threshold_RSRP_Range", HFILL }},
{ &hf_lte_rrc_threshold_RSRQ,
{ "threshold-RSRQ", "lte-rrc.threshold_RSRQ",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_threshold_RSRQ_vals_ext, 0,
"Threshold_RSRQ_Range", HFILL }},
{ &hf_lte_rrc_triggerType_01,
{ "triggerType", "lte-rrc.triggerType",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_triggerType_01_vals), 0,
"T_triggerType_01", HFILL }},
{ &hf_lte_rrc_event_01,
{ "event", "lte-rrc.event_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_event_01", HFILL }},
{ &hf_lte_rrc_eventId_01,
{ "eventId", "lte-rrc.eventId",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_eventId_01_vals), 0,
"T_eventId_01", HFILL }},
{ &hf_lte_rrc_eventB1,
{ "eventB1", "lte-rrc.eventB1_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_b1_Threshold,
{ "b1-Threshold", "lte-rrc.b1_Threshold",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_b1_Threshold_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_b1_ThresholdUTRA,
{ "b1-ThresholdUTRA", "lte-rrc.b1_ThresholdUTRA",
FT_UINT32, BASE_DEC, VALS(lte_rrc_ThresholdUTRA_vals), 0,
"ThresholdUTRA", HFILL }},
{ &hf_lte_rrc_b1_ThresholdGERAN,
{ "b1-ThresholdGERAN", "lte-rrc.b1_ThresholdGERAN",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_thresholdGERAN_vals_ext, 0,
"ThresholdGERAN", HFILL }},
{ &hf_lte_rrc_b1_ThresholdCDMA2000,
{ "b1-ThresholdCDMA2000", "lte-rrc.b1_ThresholdCDMA2000",
FT_UINT32, BASE_DEC, NULL, 0,
"ThresholdCDMA2000", HFILL }},
{ &hf_lte_rrc_eventB2,
{ "eventB2", "lte-rrc.eventB2_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_b2_Threshold1,
{ "b2-Threshold1", "lte-rrc.b2_Threshold1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_ThresholdEUTRA_vals), 0,
"ThresholdEUTRA", HFILL }},
{ &hf_lte_rrc_b2_Threshold2,
{ "b2-Threshold2", "lte-rrc.b2_Threshold2",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_b2_Threshold2_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_b2_Threshold2UTRA,
{ "b2-Threshold2UTRA", "lte-rrc.b2_Threshold2UTRA",
FT_UINT32, BASE_DEC, VALS(lte_rrc_ThresholdUTRA_vals), 0,
"ThresholdUTRA", HFILL }},
{ &hf_lte_rrc_b2_Threshold2GERAN,
{ "b2-Threshold2GERAN", "lte-rrc.b2_Threshold2GERAN",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_thresholdGERAN_vals_ext, 0,
"ThresholdGERAN", HFILL }},
{ &hf_lte_rrc_b2_Threshold2CDMA2000,
{ "b2-Threshold2CDMA2000", "lte-rrc.b2_Threshold2CDMA2000",
FT_UINT32, BASE_DEC, NULL, 0,
"ThresholdCDMA2000", HFILL }},
{ &hf_lte_rrc_periodical_01,
{ "periodical", "lte-rrc.periodical_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_periodical_01", HFILL }},
{ &hf_lte_rrc_purpose_03,
{ "purpose", "lte-rrc.purpose",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_purpose_03_vals), 0,
"T_purpose_03", HFILL }},
{ &hf_lte_rrc_reportAmount_01,
{ "reportAmount", "lte-rrc.reportAmount",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_reportAmount_01_vals), 0,
"T_reportAmount_01", HFILL }},
{ &hf_lte_rrc_si_RequestForHO_r9_01,
{ "si-RequestForHO-r9", "lte-rrc.si_RequestForHO_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_si_RequestForHO_r9_01_vals), 0,
"T_si_RequestForHO_r9_01", HFILL }},
{ &hf_lte_rrc_reportQuantityUTRA_FDD_r10,
{ "reportQuantityUTRA-FDD-r10", "lte-rrc.reportQuantityUTRA_FDD_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_reportQuantityUTRA_FDD_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_includeLocationInfo_r11,
{ "includeLocationInfo-r11", "lte-rrc.includeLocationInfo_r11",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_utra_RSCP_01,
{ "utra-RSCP", "lte-rrc.utra_RSCP",
FT_INT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_thresholdUTRA_RSCP_vals_ext, 0,
"Threshold_utra_RSRP", HFILL }},
{ &hf_lte_rrc_utra_EcN0_01,
{ "utra-EcN0", "lte-rrc.utra_EcN0",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_thresholdUTRA_EcN0_vals_ext, 0,
"Threshold_utra_EcN0", HFILL }},
{ &hf_lte_rrc_ReportConfigToAddModList_item,
{ "ReportConfigToAddMod", "lte-rrc.ReportConfigToAddMod_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_reportConfig,
{ "reportConfig", "lte-rrc.reportConfig",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_reportConfig_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_reportConfigEUTRA,
{ "reportConfigEUTRA", "lte-rrc.reportConfigEUTRA_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_reportConfigInterRAT,
{ "reportConfigInterRAT", "lte-rrc.reportConfigInterRAT_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_cellGlobalIdList_r10,
{ "cellGlobalIdList-r10", "lte-rrc.cellGlobalIdList_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_trackingAreaCodeList_r10,
{ "trackingAreaCodeList-r10", "lte-rrc.trackingAreaCodeList_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_trackingAreaCodeList_v1130,
{ "trackingAreaCodeList-v1130", "lte-rrc.trackingAreaCodeList_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CellGlobalIdList_r10_item,
{ "CellGlobalIdEUTRA", "lte-rrc.CellGlobalIdEUTRA_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_TrackingAreaCodeList_r10_item,
{ "TrackingAreaCode", "lte-rrc.TrackingAreaCode",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_plmn_Identity_perTAC_List_r11,
{ "plmn-Identity-perTAC-List-r11", "lte-rrc.plmn_Identity_perTAC_List_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_8_OF_PLMN_Identity", HFILL }},
{ &hf_lte_rrc_plmn_Identity_perTAC_List_r11_item,
{ "PLMN-Identity", "lte-rrc.PLMN_Identity_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_subframePatternFDD_r10,
{ "subframePatternFDD-r10", "lte-rrc.subframePatternFDD_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_40", HFILL }},
{ &hf_lte_rrc_subframePatternTDD_r10,
{ "subframePatternTDD-r10", "lte-rrc.subframePatternTDD_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_subframePatternTDD_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_subframeConfig1_5_r10,
{ "subframeConfig1-5-r10", "lte-rrc.subframeConfig1_5_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_20", HFILL }},
{ &hf_lte_rrc_subframeConfig0_r10,
{ "subframeConfig0-r10", "lte-rrc.subframeConfig0_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_70", HFILL }},
{ &hf_lte_rrc_subframeConfig6_r10,
{ "subframeConfig6-r10", "lte-rrc.subframeConfig6_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_60", HFILL }},
{ &hf_lte_rrc_reportProximityConfig_r9,
{ "reportProximityConfig-r9", "lte-rrc.reportProximityConfig_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_idc_Config_r11,
{ "idc-Config-r11", "lte-rrc.idc_Config_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_powerPrefIndicationConfig_r11,
{ "powerPrefIndicationConfig-r11", "lte-rrc.powerPrefIndicationConfig_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_PowerPrefIndicationConfig_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_obtainLocationConfig_r11,
{ "obtainLocationConfig-r11", "lte-rrc.obtainLocationConfig_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_idc_Indication_r11,
{ "idc-Indication-r11", "lte-rrc.idc_Indication_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_idc_Indication_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_autonomousDenialParameters_r11,
{ "autonomousDenialParameters-r11", "lte-rrc.autonomousDenialParameters_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_autonomousDenialSubframes_r11,
{ "autonomousDenialSubframes-r11", "lte-rrc.autonomousDenialSubframes_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_autonomousDenialSubframes_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_autonomousDenialValidity_r11,
{ "autonomousDenialValidity-r11", "lte-rrc.autonomousDenialValidity_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_autonomousDenialValidity_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_obtainLocation_r11,
{ "obtainLocation-r11", "lte-rrc.obtainLocation_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_obtainLocation_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_setup_39,
{ "setup", "lte-rrc.setup_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_setup_37", HFILL }},
{ &hf_lte_rrc_powerPrefIndicationTimer_r11,
{ "powerPrefIndicationTimer-r11", "lte-rrc.powerPrefIndicationTimer_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_powerPrefIndicationTimer_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_proximityIndicationEUTRA_r9,
{ "proximityIndicationEUTRA-r9", "lte-rrc.proximityIndicationEUTRA_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_proximityIndicationEUTRA_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_proximityIndicationUTRA_r9,
{ "proximityIndicationUTRA-r9", "lte-rrc.proximityIndicationUTRA_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_proximityIndicationUTRA_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_m_TMSI,
{ "m-TMSI", "lte-rrc.m_TMSI",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_32", HFILL }},
{ &hf_lte_rrc_plmn_Identity_r10,
{ "plmn-Identity-r10", "lte-rrc.plmn_Identity_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
"PLMN_Identity", HFILL }},
{ &hf_lte_rrc_traceId_r10,
{ "traceId-r10", "lte-rrc.traceId_r10",
FT_BYTES, BASE_NONE, NULL, 0,
"OCTET_STRING_SIZE_3", HFILL }},
{ &hf_lte_rrc_UE_CapabilityRAT_ContainerList_item,
{ "UE-CapabilityRAT-Container", "lte-rrc.UE_CapabilityRAT_Container_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rat_Type,
{ "rat-Type", "lte-rrc.rat_Type",
FT_UINT32, BASE_DEC, VALS(lte_rrc_RAT_Type_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_ueCapabilityRAT_Container,
{ "ueCapabilityRAT-Container", "lte-rrc.ueCapabilityRAT_Container",
FT_BYTES, BASE_NONE, NULL, 0,
"T_ueCapabilityRAT_Container", HFILL }},
{ &hf_lte_rrc_accessStratumRelease,
{ "accessStratumRelease", "lte-rrc.accessStratumRelease",
FT_UINT32, BASE_DEC, VALS(lte_rrc_AccessStratumRelease_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_ue_Category,
{ "ue-Category", "lte-rrc.ue_Category",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_5", HFILL }},
{ &hf_lte_rrc_pdcp_Parameters,
{ "pdcp-Parameters", "lte-rrc.pdcp_Parameters_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_phyLayerParameters,
{ "phyLayerParameters", "lte-rrc.phyLayerParameters_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rf_Parameters,
{ "rf-Parameters", "lte-rrc.rf_Parameters_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measParameters,
{ "measParameters", "lte-rrc.measParameters_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_featureGroupIndicators,
{ "featureGroupIndicators", "lte-rrc.featureGroupIndicators",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_interRAT_Parameters,
{ "interRAT-Parameters", "lte-rrc.interRAT_Parameters_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_interRAT_Parameters", HFILL }},
{ &hf_lte_rrc_utraFDD,
{ "utraFDD", "lte-rrc.utraFDD_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersUTRA_FDD", HFILL }},
{ &hf_lte_rrc_utraTDD128,
{ "utraTDD128", "lte-rrc.utraTDD128_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersUTRA_TDD128", HFILL }},
{ &hf_lte_rrc_utraTDD384,
{ "utraTDD384", "lte-rrc.utraTDD384_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersUTRA_TDD384", HFILL }},
{ &hf_lte_rrc_utraTDD768,
{ "utraTDD768", "lte-rrc.utraTDD768_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersUTRA_TDD768", HFILL }},
{ &hf_lte_rrc_geran_02,
{ "geran", "lte-rrc.geran_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersGERAN", HFILL }},
{ &hf_lte_rrc_cdma2000_HRPD_01,
{ "cdma2000-HRPD", "lte-rrc.cdma2000_HRPD_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersCDMA2000_HRPD", HFILL }},
{ &hf_lte_rrc_cdma2000_1xRTT_01,
{ "cdma2000-1xRTT", "lte-rrc.cdma2000_1xRTT_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersCDMA2000_1XRTT", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_111,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_Capability_v920_IEs", HFILL }},
{ &hf_lte_rrc_featureGroupIndRel9Add_r9,
{ "featureGroupIndRel9Add-r9", "lte-rrc.featureGroupIndRel9Add_r9",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_fdd_Add_UE_EUTRA_Capabilities_r9,
{ "fdd-Add-UE-EUTRA-Capabilities-r9", "lte-rrc.fdd_Add_UE_EUTRA_Capabilities_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_CapabilityAddXDD_Mode_r9", HFILL }},
{ &hf_lte_rrc_tdd_Add_UE_EUTRA_Capabilities_r9,
{ "tdd-Add-UE-EUTRA-Capabilities-r9", "lte-rrc.tdd_Add_UE_EUTRA_Capabilities_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_CapabilityAddXDD_Mode_r9", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_112,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_Capability_v9c0_IEs", HFILL }},
{ &hf_lte_rrc_interRAT_ParametersUTRA_v9c0,
{ "interRAT-ParametersUTRA-v9c0", "lte-rrc.interRAT_ParametersUTRA_v9c0_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersUTRA_v9c0", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_113,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_Capability_v9d0_IEs", HFILL }},
{ &hf_lte_rrc_phyLayerParameters_v9d0,
{ "phyLayerParameters-v9d0", "lte-rrc.phyLayerParameters_v9d0_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_114,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_Capability_v9e0_IEs", HFILL }},
{ &hf_lte_rrc_rf_Parameters_v9e0,
{ "rf-Parameters-v9e0", "lte-rrc.rf_Parameters_v9e0_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_115,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_Capability_v9h0_IEs", HFILL }},
{ &hf_lte_rrc_interRAT_ParametersUTRA_v9h0,
{ "interRAT-ParametersUTRA-v9h0", "lte-rrc.interRAT_ParametersUTRA_v9h0_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersUTRA_v9h0", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_116,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_Capability_v10c0_IEs", HFILL }},
{ &hf_lte_rrc_otdoa_PositioningCapabilities_r10,
{ "otdoa-PositioningCapabilities-r10", "lte-rrc.otdoa_PositioningCapabilities_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_117,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_47", HFILL }},
{ &hf_lte_rrc_phyLayerParameters_v920,
{ "phyLayerParameters-v920", "lte-rrc.phyLayerParameters_v920_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_interRAT_ParametersGERAN_v920,
{ "interRAT-ParametersGERAN-v920", "lte-rrc.interRAT_ParametersGERAN_v920_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersGERAN_v920", HFILL }},
{ &hf_lte_rrc_interRAT_ParametersUTRA_v920,
{ "interRAT-ParametersUTRA-v920", "lte-rrc.interRAT_ParametersUTRA_v920_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersUTRA_v920", HFILL }},
{ &hf_lte_rrc_interRAT_ParametersCDMA2000_v920,
{ "interRAT-ParametersCDMA2000-v920", "lte-rrc.interRAT_ParametersCDMA2000_v920_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersCDMA2000_1XRTT_v920", HFILL }},
{ &hf_lte_rrc_deviceType_r9,
{ "deviceType-r9", "lte-rrc.deviceType_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_deviceType_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_csg_ProximityIndicationParameters_r9,
{ "csg-ProximityIndicationParameters-r9", "lte-rrc.csg_ProximityIndicationParameters_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_neighCellSI_AcquisitionParameters_r9,
{ "neighCellSI-AcquisitionParameters-r9", "lte-rrc.neighCellSI_AcquisitionParameters_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_son_Parameters_r9,
{ "son-Parameters-r9", "lte-rrc.son_Parameters_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_118,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_Capability_v940_IEs", HFILL }},
{ &hf_lte_rrc_lateNonCriticalExtension_07,
{ "lateNonCriticalExtension", "lte-rrc.lateNonCriticalExtension",
FT_BYTES, BASE_NONE, NULL, 0,
"T_lateNonCriticalExtension_06", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_119,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_Capability_v1020_IEs", HFILL }},
{ &hf_lte_rrc_ue_Category_v1020,
{ "ue-Category-v1020", "lte-rrc.ue_Category_v1020",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_6_8", HFILL }},
{ &hf_lte_rrc_phyLayerParameters_v1020,
{ "phyLayerParameters-v1020", "lte-rrc.phyLayerParameters_v1020_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rf_Parameters_v1020,
{ "rf-Parameters-v1020", "lte-rrc.rf_Parameters_v1020_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measParameters_v1020,
{ "measParameters-v1020", "lte-rrc.measParameters_v1020_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_featureGroupIndRel10_r10,
{ "featureGroupIndRel10-r10", "lte-rrc.featureGroupIndRel10_r10",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_interRAT_ParametersCDMA2000_v1020,
{ "interRAT-ParametersCDMA2000-v1020", "lte-rrc.interRAT_ParametersCDMA2000_v1020_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersCDMA2000_1XRTT_v1020", HFILL }},
{ &hf_lte_rrc_ue_BasedNetwPerfMeasParameters_r10,
{ "ue-BasedNetwPerfMeasParameters-r10", "lte-rrc.ue_BasedNetwPerfMeasParameters_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_interRAT_ParametersUTRA_TDD_v1020,
{ "interRAT-ParametersUTRA-TDD-v1020", "lte-rrc.interRAT_ParametersUTRA_TDD_v1020_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersUTRA_TDD_v1020", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_120,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_Capability_v1060_IEs", HFILL }},
{ &hf_lte_rrc_fdd_Add_UE_EUTRA_Capabilities_v1060,
{ "fdd-Add-UE-EUTRA-Capabilities-v1060", "lte-rrc.fdd_Add_UE_EUTRA_Capabilities_v1060_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_CapabilityAddXDD_Mode_v1060", HFILL }},
{ &hf_lte_rrc_tdd_Add_UE_EUTRA_Capabilities_v1060,
{ "tdd-Add-UE-EUTRA-Capabilities-v1060", "lte-rrc.tdd_Add_UE_EUTRA_Capabilities_v1060_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_CapabilityAddXDD_Mode_v1060", HFILL }},
{ &hf_lte_rrc_rf_Parameters_v1060,
{ "rf-Parameters-v1060", "lte-rrc.rf_Parameters_v1060_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_121,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_Capability_v1090_IEs", HFILL }},
{ &hf_lte_rrc_rf_Parameters_v1090,
{ "rf-Parameters-v1090", "lte-rrc.rf_Parameters_v1090_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_122,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_Capability_v1130_IEs", HFILL }},
{ &hf_lte_rrc_pdcp_Parameters_v1130,
{ "pdcp-Parameters-v1130", "lte-rrc.pdcp_Parameters_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_phyLayerParameters_v1130,
{ "phyLayerParameters-v1130", "lte-rrc.phyLayerParameters_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rf_Parameters_v1130,
{ "rf-Parameters-v1130", "lte-rrc.rf_Parameters_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_measParameters_v1130,
{ "measParameters-v1130", "lte-rrc.measParameters_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_interRAT_ParametersCDMA2000_v1130,
{ "interRAT-ParametersCDMA2000-v1130", "lte-rrc.interRAT_ParametersCDMA2000_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersCDMA2000_v1130", HFILL }},
{ &hf_lte_rrc_otherParameters_r11,
{ "otherParameters-r11", "lte-rrc.otherParameters_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"Other_Parameters_r11", HFILL }},
{ &hf_lte_rrc_fdd_Add_UE_EUTRA_Capabilities_v1130,
{ "fdd-Add-UE-EUTRA-Capabilities-v1130", "lte-rrc.fdd_Add_UE_EUTRA_Capabilities_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_CapabilityAddXDD_Mode_v1130", HFILL }},
{ &hf_lte_rrc_tdd_Add_UE_EUTRA_Capabilities_v1130,
{ "tdd-Add-UE-EUTRA-Capabilities-v1130", "lte-rrc.tdd_Add_UE_EUTRA_Capabilities_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_CapabilityAddXDD_Mode_v1130", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_123,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_Capability_v1170_IEs", HFILL }},
{ &hf_lte_rrc_phyLayerParameters_v1170,
{ "phyLayerParameters-v1170", "lte-rrc.phyLayerParameters_v1170_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ue_Category_v1170,
{ "ue-Category-v1170", "lte-rrc.ue_Category_v1170",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_9_10", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_124,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_Capability_v1180_IEs", HFILL }},
{ &hf_lte_rrc_rf_Parameters_v1180,
{ "rf-Parameters-v1180", "lte-rrc.rf_Parameters_v1180_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mbms_Parameters_r11,
{ "mbms-Parameters-r11", "lte-rrc.mbms_Parameters_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_fdd_Add_UE_EUTRA_Capabilities_v1180,
{ "fdd-Add-UE-EUTRA-Capabilities-v1180", "lte-rrc.fdd_Add_UE_EUTRA_Capabilities_v1180_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_CapabilityAddXDD_Mode_v1180", HFILL }},
{ &hf_lte_rrc_tdd_Add_UE_EUTRA_Capabilities_v1180,
{ "tdd-Add-UE-EUTRA-Capabilities-v1180", "lte-rrc.tdd_Add_UE_EUTRA_Capabilities_v1180_element",
FT_NONE, BASE_NONE, NULL, 0,
"UE_EUTRA_CapabilityAddXDD_Mode_v1180", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_125,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_48", HFILL }},
{ &hf_lte_rrc_phyLayerParameters_r9,
{ "phyLayerParameters-r9", "lte-rrc.phyLayerParameters_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"PhyLayerParameters", HFILL }},
{ &hf_lte_rrc_featureGroupIndicators_r9,
{ "featureGroupIndicators-r9", "lte-rrc.featureGroupIndicators_r9",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_featureGroupIndRel9Add_r9_01,
{ "featureGroupIndRel9Add-r9", "lte-rrc.featureGroupIndRel9Add_r9",
FT_BYTES, BASE_NONE, NULL, 0,
"T_featureGroupIndRel9Add_r9_01", HFILL }},
{ &hf_lte_rrc_interRAT_ParametersGERAN_r9,
{ "interRAT-ParametersGERAN-r9", "lte-rrc.interRAT_ParametersGERAN_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersGERAN", HFILL }},
{ &hf_lte_rrc_interRAT_ParametersUTRA_r9,
{ "interRAT-ParametersUTRA-r9", "lte-rrc.interRAT_ParametersUTRA_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersUTRA_v920", HFILL }},
{ &hf_lte_rrc_interRAT_ParametersCDMA2000_r9,
{ "interRAT-ParametersCDMA2000-r9", "lte-rrc.interRAT_ParametersCDMA2000_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersCDMA2000_1XRTT_v920", HFILL }},
{ &hf_lte_rrc_phyLayerParameters_v1060,
{ "phyLayerParameters-v1060", "lte-rrc.phyLayerParameters_v1060_element",
FT_NONE, BASE_NONE, NULL, 0,
"PhyLayerParameters_v1020", HFILL }},
{ &hf_lte_rrc_featureGroupIndRel10_v1060,
{ "featureGroupIndRel10-v1060", "lte-rrc.featureGroupIndRel10_v1060",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_interRAT_ParametersCDMA2000_v1060,
{ "interRAT-ParametersCDMA2000-v1060", "lte-rrc.interRAT_ParametersCDMA2000_v1060_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersCDMA2000_1XRTT_v1020", HFILL }},
{ &hf_lte_rrc_interRAT_ParametersUTRA_TDD_v1060,
{ "interRAT-ParametersUTRA-TDD-v1060", "lte-rrc.interRAT_ParametersUTRA_TDD_v1060_element",
FT_NONE, BASE_NONE, NULL, 0,
"IRAT_ParametersUTRA_TDD_v1020", HFILL }},
{ &hf_lte_rrc_supportedROHC_Profiles,
{ "supportedROHC-Profiles", "lte-rrc.supportedROHC_Profiles_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_maxNumberROHC_ContextSessions,
{ "maxNumberROHC-ContextSessions", "lte-rrc.maxNumberROHC_ContextSessions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_maxNumberROHC_ContextSessions_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_pdcp_SN_Extension_r11,
{ "pdcp-SN-Extension-r11", "lte-rrc.pdcp_SN_Extension_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_pdcp_SN_Extension_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_supportRohcContextContinue_r11,
{ "supportRohcContextContinue-r11", "lte-rrc.supportRohcContextContinue_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_supportRohcContextContinue_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_ue_TxAntennaSelectionSupported,
{ "ue-TxAntennaSelectionSupported", "lte-rrc.ue_TxAntennaSelectionSupported",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_ue_SpecificRefSigsSupported,
{ "ue-SpecificRefSigsSupported", "lte-rrc.ue_SpecificRefSigsSupported",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_enhancedDualLayerFDD_r9,
{ "enhancedDualLayerFDD-r9", "lte-rrc.enhancedDualLayerFDD_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_enhancedDualLayerFDD_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_enhancedDualLayerTDD_r9,
{ "enhancedDualLayerTDD-r9", "lte-rrc.enhancedDualLayerTDD_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_enhancedDualLayerTDD_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_tm5_FDD_r9,
{ "tm5-FDD-r9", "lte-rrc.tm5_FDD_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_tm5_FDD_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_tm5_TDD_r9,
{ "tm5-TDD-r9", "lte-rrc.tm5_TDD_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_tm5_TDD_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_twoAntennaPortsForPUCCH_r10,
{ "twoAntennaPortsForPUCCH-r10", "lte-rrc.twoAntennaPortsForPUCCH_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_twoAntennaPortsForPUCCH_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_tm9_With_8Tx_FDD_r10,
{ "tm9-With-8Tx-FDD-r10", "lte-rrc.tm9_With_8Tx_FDD_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_tm9_With_8Tx_FDD_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_pmi_Disabling_r10,
{ "pmi-Disabling-r10", "lte-rrc.pmi_Disabling_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_pmi_Disabling_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_crossCarrierScheduling_r10,
{ "crossCarrierScheduling-r10", "lte-rrc.crossCarrierScheduling_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_crossCarrierScheduling_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_simultaneousPUCCH_PUSCH_r10_01,
{ "simultaneousPUCCH-PUSCH-r10", "lte-rrc.simultaneousPUCCH_PUSCH_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_simultaneousPUCCH_PUSCH_r10_01_vals), 0,
"T_simultaneousPUCCH_PUSCH_r10_01", HFILL }},
{ &hf_lte_rrc_multiClusterPUSCH_WithinCC_r10,
{ "multiClusterPUSCH-WithinCC-r10", "lte-rrc.multiClusterPUSCH_WithinCC_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_multiClusterPUSCH_WithinCC_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonContiguousUL_RA_WithinCC_List_r10,
{ "nonContiguousUL-RA-WithinCC-List-r10", "lte-rrc.nonContiguousUL_RA_WithinCC_List_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_crs_InterfHandl_r11,
{ "crs-InterfHandl-r11", "lte-rrc.crs_InterfHandl_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_crs_InterfHandl_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_ePDCCH_r11,
{ "ePDCCH-r11", "lte-rrc.ePDCCH_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_ePDCCH_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_multiACK_CSI_Reporting_r11,
{ "multiACK-CSI-Reporting-r11", "lte-rrc.multiACK_CSI_Reporting_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_multiACK_CSI_Reporting_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_ss_CCH_InterfHandl_r11,
{ "ss-CCH-InterfHandl-r11", "lte-rrc.ss_CCH_InterfHandl_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_ss_CCH_InterfHandl_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_tdd_SpecialSubframe_r11,
{ "tdd-SpecialSubframe-r11", "lte-rrc.tdd_SpecialSubframe_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_tdd_SpecialSubframe_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_txDiv_PUCCH1b_ChSelect_r11,
{ "txDiv-PUCCH1b-ChSelect-r11", "lte-rrc.txDiv_PUCCH1b_ChSelect_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_txDiv_PUCCH1b_ChSelect_r11_vals), 0,
"T_txDiv_PUCCH1b_ChSelect_r11", HFILL }},
{ &hf_lte_rrc_ul_CoMP_r11,
{ "ul-CoMP-r11", "lte-rrc.ul_CoMP_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_ul_CoMP_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_interBandTDD_CA_WithDifferentConfig_r11,
{ "interBandTDD-CA-WithDifferentConfig-r11", "lte-rrc.interBandTDD_CA_WithDifferentConfig_r11",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_NonContiguousUL_RA_WithinCC_List_r10_item,
{ "NonContiguousUL-RA-WithinCC-r10", "lte-rrc.NonContiguousUL_RA_WithinCC_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonContiguousUL_RA_WithinCC_Info_r10,
{ "nonContiguousUL-RA-WithinCC-Info-r10", "lte-rrc.nonContiguousUL_RA_WithinCC_Info_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_nonContiguousUL_RA_WithinCC_Info_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_supportedBandListEUTRA,
{ "supportedBandListEUTRA", "lte-rrc.supportedBandListEUTRA",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_supportedBandListEUTRA_v9e0,
{ "supportedBandListEUTRA-v9e0", "lte-rrc.supportedBandListEUTRA_v9e0",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_supportedBandCombination_r10,
{ "supportedBandCombination-r10", "lte-rrc.supportedBandCombination_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_supportedBandCombinationExt_r10,
{ "supportedBandCombinationExt-r10", "lte-rrc.supportedBandCombinationExt_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_supportedBandCombination_v1090,
{ "supportedBandCombination-v1090", "lte-rrc.supportedBandCombination_v1090",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_supportedBandCombination_v1130,
{ "supportedBandCombination-v1130", "lte-rrc.supportedBandCombination_v1130",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_freqBandRetrieval_r11,
{ "freqBandRetrieval-r11", "lte-rrc.freqBandRetrieval_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_freqBandRetrieval_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_reportedFrequencyBands_r11,
{ "reportedFrequencyBands-r11", "lte-rrc.reportedFrequencyBands_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_maxBands_OF_FreqBandIndicator_r11", HFILL }},
{ &hf_lte_rrc_reportedFrequencyBands_r11_item,
{ "FreqBandIndicator-r11", "lte-rrc.FreqBandIndicator_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_supportedBandCombinationAdd_r11,
{ "supportedBandCombinationAdd-r11", "lte-rrc.supportedBandCombinationAdd_r11",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SupportedBandCombination_r10_item,
{ "BandCombinationParameters-r10", "lte-rrc.BandCombinationParameters_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SupportedBandCombinationExt_r10_item,
{ "BandCombinationParametersExt-r10", "lte-rrc.BandCombinationParametersExt_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SupportedBandCombination_v1090_item,
{ "BandCombinationParameters-v1090", "lte-rrc.BandCombinationParameters_v1090",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SupportedBandCombination_v1130_item,
{ "BandCombinationParameters-v1130", "lte-rrc.BandCombinationParameters_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SupportedBandCombinationAdd_r11_item,
{ "BandCombinationParameters-r11", "lte-rrc.BandCombinationParameters_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_BandCombinationParameters_r10_item,
{ "BandParameters-r10", "lte-rrc.BandParameters_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_supportedBandwidthCombinationSet_r10,
{ "supportedBandwidthCombinationSet-r10", "lte-rrc.supportedBandwidthCombinationSet_r10",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_BandCombinationParameters_v1090_item,
{ "BandParameters-v1090", "lte-rrc.BandParameters_v1090_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_multipleTimingAdvance_r11,
{ "multipleTimingAdvance-r11", "lte-rrc.multipleTimingAdvance_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_multipleTimingAdvance_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_simultaneousRx_Tx_r11,
{ "simultaneousRx-Tx-r11", "lte-rrc.simultaneousRx_Tx_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_simultaneousRx_Tx_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_bandParameterList_r11,
{ "bandParameterList-r11", "lte-rrc.bandParameterList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_maxSimultaneousBands_r10_OF_BandParameters_v1130", HFILL }},
{ &hf_lte_rrc_bandParameterList_r11_item,
{ "BandParameters-v1130", "lte-rrc.BandParameters_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_bandParameterList_r11_01,
{ "bandParameterList-r11", "lte-rrc.bandParameterList_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"SEQUENCE_SIZE_1_maxSimultaneousBands_r10_OF_BandParameters_r11", HFILL }},
{ &hf_lte_rrc_bandParameterList_r11_item_01,
{ "BandParameters-r11", "lte-rrc.BandParameters_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_supportedBandwidthCombinationSet_r11,
{ "supportedBandwidthCombinationSet-r11", "lte-rrc.supportedBandwidthCombinationSet_r11",
FT_BYTES, BASE_NONE, NULL, 0,
"SupportedBandwidthCombinationSet_r10", HFILL }},
{ &hf_lte_rrc_multipleTimingAdvance_r11_01,
{ "multipleTimingAdvance-r11", "lte-rrc.multipleTimingAdvance_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_multipleTimingAdvance_r11_01_vals), 0,
"T_multipleTimingAdvance_r11_01", HFILL }},
{ &hf_lte_rrc_simultaneousRx_Tx_r11_01,
{ "simultaneousRx-Tx-r11", "lte-rrc.simultaneousRx_Tx_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_simultaneousRx_Tx_r11_01_vals), 0,
"T_simultaneousRx_Tx_r11_01", HFILL }},
{ &hf_lte_rrc_bandInfoEUTRA_r11,
{ "bandInfoEUTRA-r11", "lte-rrc.bandInfoEUTRA_r11_element",
FT_NONE, BASE_NONE, NULL, 0,
"BandInfoEUTRA", HFILL }},
{ &hf_lte_rrc_bandEUTRA_r10,
{ "bandEUTRA-r10", "lte-rrc.bandEUTRA_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"FreqBandIndicator", HFILL }},
{ &hf_lte_rrc_bandParametersUL_r10,
{ "bandParametersUL-r10", "lte-rrc.bandParametersUL_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_bandParametersDL_r10,
{ "bandParametersDL-r10", "lte-rrc.bandParametersDL_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_bandEUTRA_v1090,
{ "bandEUTRA-v1090", "lte-rrc.bandEUTRA_v1090",
FT_UINT32, BASE_DEC, NULL, 0,
"FreqBandIndicator_v9e0", HFILL }},
{ &hf_lte_rrc_supportedCSI_Proc_r11,
{ "supportedCSI-Proc-r11", "lte-rrc.supportedCSI_Proc_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_supportedCSI_Proc_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_bandEUTRA_r11,
{ "bandEUTRA-r11", "lte-rrc.bandEUTRA_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"FreqBandIndicator_r11", HFILL }},
{ &hf_lte_rrc_bandParametersUL_r11,
{ "bandParametersUL-r11", "lte-rrc.bandParametersUL_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"BandParametersUL_r10", HFILL }},
{ &hf_lte_rrc_bandParametersDL_r11,
{ "bandParametersDL-r11", "lte-rrc.bandParametersDL_r11",
FT_UINT32, BASE_DEC, NULL, 0,
"BandParametersDL_r10", HFILL }},
{ &hf_lte_rrc_supportedCSI_Proc_r11_01,
{ "supportedCSI-Proc-r11", "lte-rrc.supportedCSI_Proc_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_supportedCSI_Proc_r11_01_vals), 0,
"T_supportedCSI_Proc_r11_01", HFILL }},
{ &hf_lte_rrc_BandParametersUL_r10_item,
{ "CA-MIMO-ParametersUL-r10", "lte-rrc.CA_MIMO_ParametersUL_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ca_BandwidthClassUL_r10,
{ "ca-BandwidthClassUL-r10", "lte-rrc.ca_BandwidthClassUL_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_CA_BandwidthClass_r10_vals), 0,
"CA_BandwidthClass_r10", HFILL }},
{ &hf_lte_rrc_supportedMIMO_CapabilityUL_r10,
{ "supportedMIMO-CapabilityUL-r10", "lte-rrc.supportedMIMO_CapabilityUL_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_MIMO_CapabilityUL_r10_vals), 0,
"MIMO_CapabilityUL_r10", HFILL }},
{ &hf_lte_rrc_BandParametersDL_r10_item,
{ "CA-MIMO-ParametersDL-r10", "lte-rrc.CA_MIMO_ParametersDL_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ca_BandwidthClassDL_r10,
{ "ca-BandwidthClassDL-r10", "lte-rrc.ca_BandwidthClassDL_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_CA_BandwidthClass_r10_vals), 0,
"CA_BandwidthClass_r10", HFILL }},
{ &hf_lte_rrc_supportedMIMO_CapabilityDL_r10,
{ "supportedMIMO-CapabilityDL-r10", "lte-rrc.supportedMIMO_CapabilityDL_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_MIMO_CapabilityDL_r10_vals), 0,
"MIMO_CapabilityDL_r10", HFILL }},
{ &hf_lte_rrc_SupportedBandListEUTRA_item,
{ "SupportedBandEUTRA", "lte-rrc.SupportedBandEUTRA_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SupportedBandListEUTRA_v9e0_item,
{ "SupportedBandEUTRA-v9e0", "lte-rrc.SupportedBandEUTRA_v9e0_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_bandEUTRA,
{ "bandEUTRA", "lte-rrc.bandEUTRA",
FT_UINT32, BASE_DEC, NULL, 0,
"FreqBandIndicator", HFILL }},
{ &hf_lte_rrc_halfDuplex,
{ "halfDuplex", "lte-rrc.halfDuplex",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_bandEUTRA_v9e0,
{ "bandEUTRA-v9e0", "lte-rrc.bandEUTRA_v9e0",
FT_UINT32, BASE_DEC, NULL, 0,
"FreqBandIndicator_v9e0", HFILL }},
{ &hf_lte_rrc_bandListEUTRA,
{ "bandListEUTRA", "lte-rrc.bandListEUTRA",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_bandCombinationListEUTRA_r10,
{ "bandCombinationListEUTRA-r10", "lte-rrc.bandCombinationListEUTRA_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rsrqMeasWideband_r11,
{ "rsrqMeasWideband-r11", "lte-rrc.rsrqMeasWideband_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_rsrqMeasWideband_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_BandListEUTRA_item,
{ "BandInfoEUTRA", "lte-rrc.BandInfoEUTRA_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_BandCombinationListEUTRA_r10_item,
{ "BandInfoEUTRA", "lte-rrc.BandInfoEUTRA_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_interFreqBandList,
{ "interFreqBandList", "lte-rrc.interFreqBandList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_interRAT_BandList,
{ "interRAT-BandList", "lte-rrc.interRAT_BandList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_InterFreqBandList_item,
{ "InterFreqBandInfo", "lte-rrc.InterFreqBandInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_interFreqNeedForGaps,
{ "interFreqNeedForGaps", "lte-rrc.interFreqNeedForGaps",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_InterRAT_BandList_item,
{ "InterRAT-BandInfo", "lte-rrc.InterRAT_BandInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_interRAT_NeedForGaps,
{ "interRAT-NeedForGaps", "lte-rrc.interRAT_NeedForGaps",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_supportedBandListUTRA_FDD,
{ "supportedBandListUTRA-FDD", "lte-rrc.supportedBandListUTRA_FDD",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_e_RedirectionUTRA_r9,
{ "e-RedirectionUTRA-r9", "lte-rrc.e_RedirectionUTRA_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_e_RedirectionUTRA_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_voiceOverPS_HS_UTRA_FDD_r9,
{ "voiceOverPS-HS-UTRA-FDD-r9", "lte-rrc.voiceOverPS_HS_UTRA_FDD_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_voiceOverPS_HS_UTRA_FDD_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_voiceOverPS_HS_UTRA_TDD128_r9,
{ "voiceOverPS-HS-UTRA-TDD128-r9", "lte-rrc.voiceOverPS_HS_UTRA_TDD128_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_voiceOverPS_HS_UTRA_TDD128_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_srvcc_FromUTRA_FDD_ToUTRA_FDD_r9,
{ "srvcc-FromUTRA-FDD-ToUTRA-FDD-r9", "lte-rrc.srvcc_FromUTRA_FDD_ToUTRA_FDD_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_srvcc_FromUTRA_FDD_ToUTRA_FDD_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_srvcc_FromUTRA_FDD_ToGERAN_r9,
{ "srvcc-FromUTRA-FDD-ToGERAN-r9", "lte-rrc.srvcc_FromUTRA_FDD_ToGERAN_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_srvcc_FromUTRA_FDD_ToGERAN_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_srvcc_FromUTRA_TDD128_ToUTRA_TDD128_r9,
{ "srvcc-FromUTRA-TDD128-ToUTRA-TDD128-r9", "lte-rrc.srvcc_FromUTRA_TDD128_ToUTRA_TDD128_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_srvcc_FromUTRA_TDD128_ToUTRA_TDD128_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_srvcc_FromUTRA_TDD128_ToGERAN_r9,
{ "srvcc-FromUTRA-TDD128-ToGERAN-r9", "lte-rrc.srvcc_FromUTRA_TDD128_ToGERAN_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_srvcc_FromUTRA_TDD128_ToGERAN_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_mfbi_UTRA_r9,
{ "mfbi-UTRA-r9", "lte-rrc.mfbi_UTRA_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_mfbi_UTRA_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_SupportedBandListUTRA_FDD_item,
{ "SupportedBandUTRA-FDD", "lte-rrc.SupportedBandUTRA_FDD",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_SupportedBandUTRA_FDD_vals_ext, 0,
NULL, HFILL }},
{ &hf_lte_rrc_supportedBandListUTRA_TDD128,
{ "supportedBandListUTRA-TDD128", "lte-rrc.supportedBandListUTRA_TDD128",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SupportedBandListUTRA_TDD128_item,
{ "SupportedBandUTRA-TDD128", "lte-rrc.SupportedBandUTRA_TDD128",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SupportedBandUTRA_TDD128_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_supportedBandListUTRA_TDD384,
{ "supportedBandListUTRA-TDD384", "lte-rrc.supportedBandListUTRA_TDD384",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SupportedBandListUTRA_TDD384_item,
{ "SupportedBandUTRA-TDD384", "lte-rrc.SupportedBandUTRA_TDD384",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SupportedBandUTRA_TDD384_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_supportedBandListUTRA_TDD768,
{ "supportedBandListUTRA-TDD768", "lte-rrc.supportedBandListUTRA_TDD768",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_SupportedBandListUTRA_TDD768_item,
{ "SupportedBandUTRA-TDD768", "lte-rrc.SupportedBandUTRA_TDD768",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SupportedBandUTRA_TDD768_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_e_RedirectionUTRA_TDD_r10,
{ "e-RedirectionUTRA-TDD-r10", "lte-rrc.e_RedirectionUTRA_TDD_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_e_RedirectionUTRA_TDD_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_supportedBandListGERAN,
{ "supportedBandListGERAN", "lte-rrc.supportedBandListGERAN",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_interRAT_PS_HO_ToGERAN,
{ "interRAT-PS-HO-ToGERAN", "lte-rrc.interRAT_PS_HO_ToGERAN",
FT_BOOLEAN, BASE_NONE, NULL, 0,
"BOOLEAN", HFILL }},
{ &hf_lte_rrc_dtm_r9,
{ "dtm-r9", "lte-rrc.dtm_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_dtm_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_e_RedirectionGERAN_r9,
{ "e-RedirectionGERAN-r9", "lte-rrc.e_RedirectionGERAN_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_e_RedirectionGERAN_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_SupportedBandListGERAN_item,
{ "SupportedBandGERAN", "lte-rrc.SupportedBandGERAN",
FT_UINT32, BASE_DEC, VALS(lte_rrc_SupportedBandGERAN_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_supportedBandListHRPD,
{ "supportedBandListHRPD", "lte-rrc.supportedBandListHRPD",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_tx_ConfigHRPD,
{ "tx-ConfigHRPD", "lte-rrc.tx_ConfigHRPD",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_tx_ConfigHRPD_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_rx_ConfigHRPD,
{ "rx-ConfigHRPD", "lte-rrc.rx_ConfigHRPD",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_rx_ConfigHRPD_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_SupportedBandListHRPD_item,
{ "BandclassCDMA2000", "lte-rrc.BandclassCDMA2000",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_BandclassCDMA2000_vals_ext, 0,
NULL, HFILL }},
{ &hf_lte_rrc_supportedBandList1XRTT,
{ "supportedBandList1XRTT", "lte-rrc.supportedBandList1XRTT",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_tx_Config1XRTT,
{ "tx-Config1XRTT", "lte-rrc.tx_Config1XRTT",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_tx_Config1XRTT_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_rx_Config1XRTT,
{ "rx-Config1XRTT", "lte-rrc.rx_Config1XRTT",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_rx_Config1XRTT_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_e_CSFB_1XRTT_r9,
{ "e-CSFB-1XRTT-r9", "lte-rrc.e_CSFB_1XRTT_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_e_CSFB_1XRTT_r9_vals), 0,
"T_e_CSFB_1XRTT_r9", HFILL }},
{ &hf_lte_rrc_e_CSFB_ConcPS_Mob1XRTT_r9,
{ "e-CSFB-ConcPS-Mob1XRTT-r9", "lte-rrc.e_CSFB_ConcPS_Mob1XRTT_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_e_CSFB_ConcPS_Mob1XRTT_r9_vals), 0,
"T_e_CSFB_ConcPS_Mob1XRTT_r9", HFILL }},
{ &hf_lte_rrc_e_CSFB_dual_1XRTT_r10,
{ "e-CSFB-dual-1XRTT-r10", "lte-rrc.e_CSFB_dual_1XRTT_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_e_CSFB_dual_1XRTT_r10_vals), 0,
"T_e_CSFB_dual_1XRTT_r10", HFILL }},
{ &hf_lte_rrc_cdma2000_NW_Sharing_r11,
{ "cdma2000-NW-Sharing-r11", "lte-rrc.cdma2000_NW_Sharing_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_cdma2000_NW_Sharing_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_SupportedBandList1XRTT_item,
{ "BandclassCDMA2000", "lte-rrc.BandclassCDMA2000",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_BandclassCDMA2000_vals_ext, 0,
NULL, HFILL }},
{ &hf_lte_rrc_intraFreqProximityIndication_r9,
{ "intraFreqProximityIndication-r9", "lte-rrc.intraFreqProximityIndication_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_intraFreqProximityIndication_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_interFreqProximityIndication_r9,
{ "interFreqProximityIndication-r9", "lte-rrc.interFreqProximityIndication_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_interFreqProximityIndication_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_utran_ProximityIndication_r9,
{ "utran-ProximityIndication-r9", "lte-rrc.utran_ProximityIndication_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_utran_ProximityIndication_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_intraFreqSI_AcquisitionForHO_r9,
{ "intraFreqSI-AcquisitionForHO-r9", "lte-rrc.intraFreqSI_AcquisitionForHO_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_intraFreqSI_AcquisitionForHO_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_interFreqSI_AcquisitionForHO_r9,
{ "interFreqSI-AcquisitionForHO-r9", "lte-rrc.interFreqSI_AcquisitionForHO_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_interFreqSI_AcquisitionForHO_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_utran_SI_AcquisitionForHO_r9,
{ "utran-SI-AcquisitionForHO-r9", "lte-rrc.utran_SI_AcquisitionForHO_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_utran_SI_AcquisitionForHO_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_rach_Report_r9_01,
{ "rach-Report-r9", "lte-rrc.rach_Report_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_rach_Report_r9_01_vals), 0,
"T_rach_Report_r9_01", HFILL }},
{ &hf_lte_rrc_loggedMeasurementsIdle_r10,
{ "loggedMeasurementsIdle-r10", "lte-rrc.loggedMeasurementsIdle_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_loggedMeasurementsIdle_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_standaloneGNSS_Location_r10,
{ "standaloneGNSS-Location-r10", "lte-rrc.standaloneGNSS_Location_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_standaloneGNSS_Location_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_otdoa_UE_Assisted_r10,
{ "otdoa-UE-Assisted-r10", "lte-rrc.otdoa_UE_Assisted_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_otdoa_UE_Assisted_r10_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_interFreqRSTD_Measurement_r10,
{ "interFreqRSTD-Measurement-r10", "lte-rrc.interFreqRSTD_Measurement_r10",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_interFreqRSTD_Measurement_r10_vals), 0,
"T_interFreqRSTD_Measurement_r10", HFILL }},
{ &hf_lte_rrc_inDeviceCoexInd_r11,
{ "inDeviceCoexInd-r11", "lte-rrc.inDeviceCoexInd_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_inDeviceCoexInd_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_powerPrefInd_r11,
{ "powerPrefInd-r11", "lte-rrc.powerPrefInd_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_powerPrefInd_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_ue_Rx_TxTimeDiffMeasurements_r11,
{ "ue-Rx-TxTimeDiffMeasurements-r11", "lte-rrc.ue_Rx_TxTimeDiffMeasurements_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_ue_Rx_TxTimeDiffMeasurements_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_mbms_SCell_r11,
{ "mbms-SCell-r11", "lte-rrc.mbms_SCell_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_mbms_SCell_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_mbms_NonServingCell_r11,
{ "mbms-NonServingCell-r11", "lte-rrc.mbms_NonServingCell_r11",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_mbms_NonServingCell_r11_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_t300,
{ "t300", "lte-rrc.t300",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_t300_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_t301,
{ "t301", "lte-rrc.t301",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_t301_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_t310,
{ "t310", "lte-rrc.t310",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_t310_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_n310,
{ "n310", "lte-rrc.n310",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_n310_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_t311,
{ "t311", "lte-rrc.t311",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_t311_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_n311,
{ "n311", "lte-rrc.n311",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_n311_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_notificationRepetitionCoeff_r9,
{ "notificationRepetitionCoeff-r9", "lte-rrc.notificationRepetitionCoeff_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_notificationRepetitionCoeff_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_notificationOffset_r9,
{ "notificationOffset-r9", "lte-rrc.notificationOffset_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_10", HFILL }},
{ &hf_lte_rrc_notificationSF_Index_r9,
{ "notificationSF-Index-r9", "lte-rrc.notificationSF_Index_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_6", HFILL }},
{ &hf_lte_rrc_MBSFN_AreaInfoList_r9_item,
{ "MBSFN-AreaInfo-r9", "lte-rrc.MBSFN_AreaInfo_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mbsfn_AreaId_r9,
{ "mbsfn-AreaId-r9", "lte-rrc.mbsfn_AreaId_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_255", HFILL }},
{ &hf_lte_rrc_non_MBSFNregionLength,
{ "non-MBSFNregionLength", "lte-rrc.non_MBSFNregionLength",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_non_MBSFNregionLength_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_notificationIndicator_r9,
{ "notificationIndicator-r9", "lte-rrc.notificationIndicator_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_7", HFILL }},
{ &hf_lte_rrc_mcch_Config_r9,
{ "mcch-Config-r9", "lte-rrc.mcch_Config_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mcch_RepetitionPeriod_r9,
{ "mcch-RepetitionPeriod-r9", "lte-rrc.mcch_RepetitionPeriod_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_mcch_RepetitionPeriod_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_mcch_Offset_r9,
{ "mcch-Offset-r9", "lte-rrc.mcch_Offset_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_10", HFILL }},
{ &hf_lte_rrc_mcch_ModificationPeriod_r9,
{ "mcch-ModificationPeriod-r9", "lte-rrc.mcch_ModificationPeriod_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_mcch_ModificationPeriod_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_sf_AllocInfo_r9,
{ "sf-AllocInfo-r9", "lte-rrc.sf_AllocInfo_r9",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_6", HFILL }},
{ &hf_lte_rrc_signallingMCS_r9,
{ "signallingMCS-r9", "lte-rrc.signallingMCS_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_signallingMCS_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_radioframeAllocationPeriod,
{ "radioframeAllocationPeriod", "lte-rrc.radioframeAllocationPeriod",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_radioframeAllocationPeriod_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_radioframeAllocationOffset,
{ "radioframeAllocationOffset", "lte-rrc.radioframeAllocationOffset",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_7", HFILL }},
{ &hf_lte_rrc_subframeAllocation,
{ "subframeAllocation", "lte-rrc.subframeAllocation",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_subframeAllocation_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_oneFrame,
{ "oneFrame", "lte-rrc.oneFrame",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_6", HFILL }},
{ &hf_lte_rrc_fourFrames,
{ "fourFrames", "lte-rrc.fourFrames",
FT_BYTES, BASE_NONE, NULL, 0,
"BIT_STRING_SIZE_24", HFILL }},
{ &hf_lte_rrc_PMCH_InfoList_r9_item,
{ "PMCH-Info-r9", "lte-rrc.PMCH_Info_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_pmch_Config_r9,
{ "pmch-Config-r9", "lte-rrc.pmch_Config_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_mbms_SessionInfoList_r9,
{ "mbms-SessionInfoList-r9", "lte-rrc.mbms_SessionInfoList_r9",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_MBMS_SessionInfoList_r9_item,
{ "MBMS-SessionInfo-r9", "lte-rrc.MBMS_SessionInfo_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_tmgi_r9,
{ "tmgi-r9", "lte-rrc.tmgi_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sessionId_r9,
{ "sessionId-r9", "lte-rrc.sessionId_r9",
FT_BYTES, BASE_NONE, NULL, 0,
"OCTET_STRING_SIZE_1", HFILL }},
{ &hf_lte_rrc_logicalChannelIdentity_r9,
{ "logicalChannelIdentity-r9", "lte-rrc.logicalChannelIdentity_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_maxSessionPerPMCH_1", HFILL }},
{ &hf_lte_rrc_sf_AllocEnd_r9,
{ "sf-AllocEnd-r9", "lte-rrc.sf_AllocEnd_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_1535", HFILL }},
{ &hf_lte_rrc_dataMCS_r9,
{ "dataMCS-r9", "lte-rrc.dataMCS_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_0_28", HFILL }},
{ &hf_lte_rrc_mch_SchedulingPeriod_r9,
{ "mch-SchedulingPeriod-r9", "lte-rrc.mch_SchedulingPeriod_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_mch_SchedulingPeriod_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_plmn_Id_r9,
{ "plmn-Id-r9", "lte-rrc.plmn_Id_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_plmn_Id_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_plmn_Index_r9,
{ "plmn-Index-r9", "lte-rrc.plmn_Index_r9",
FT_UINT32, BASE_DEC, NULL, 0,
"INTEGER_1_maxPLMN_r11", HFILL }},
{ &hf_lte_rrc_explicitValue_r9,
{ "explicitValue-r9", "lte-rrc.explicitValue_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"PLMN_Identity", HFILL }},
{ &hf_lte_rrc_serviceId_r9,
{ "serviceId-r9", "lte-rrc.serviceId_r9",
FT_BYTES, BASE_NONE, NULL, 0,
"OCTET_STRING_SIZE_3", HFILL }},
{ &hf_lte_rrc_criticalExtensions_38,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_38_vals), 0,
"T_criticalExtensions_38", HFILL }},
{ &hf_lte_rrc_c1_34,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_34_vals), 0,
"T_c1_34", HFILL }},
{ &hf_lte_rrc_handoverCommand_r8,
{ "handoverCommand-r8", "lte-rrc.handoverCommand_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"HandoverCommand_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_38,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_38", HFILL }},
{ &hf_lte_rrc_handoverCommandMessage,
{ "handoverCommandMessage", "lte-rrc.handoverCommandMessage",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_126,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_49", HFILL }},
{ &hf_lte_rrc_criticalExtensions_39,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_39_vals), 0,
"T_criticalExtensions_39", HFILL }},
{ &hf_lte_rrc_c1_35,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_35_vals), 0,
"T_c1_35", HFILL }},
{ &hf_lte_rrc_handoverPreparationInformation_r8,
{ "handoverPreparationInformation-r8", "lte-rrc.handoverPreparationInformation_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"HandoverPreparationInformation_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_39,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_39", HFILL }},
{ &hf_lte_rrc_ue_RadioAccessCapabilityInfo,
{ "ue-RadioAccessCapabilityInfo", "lte-rrc.ue_RadioAccessCapabilityInfo",
FT_UINT32, BASE_DEC, NULL, 0,
"UE_CapabilityRAT_ContainerList", HFILL }},
{ &hf_lte_rrc_as_Config,
{ "as-Config", "lte-rrc.as_Config_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_rrm_Config,
{ "rrm-Config", "lte-rrc.rrm_Config_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_as_Context,
{ "as-Context", "lte-rrc.as_Context_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_127,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"HandoverPreparationInformation_v920_IEs", HFILL }},
{ &hf_lte_rrc_ue_ConfigRelease_r9,
{ "ue-ConfigRelease-r9", "lte-rrc.ue_ConfigRelease_r9",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_ue_ConfigRelease_r9_vals), 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_128,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"HandoverPreparationInformation_v9d0_IEs", HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_129,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"HandoverPreparationInformation_v9e0_IEs", HFILL }},
{ &hf_lte_rrc_as_Config_v9e0,
{ "as-Config-v9e0", "lte-rrc.as_Config_v9e0_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_130,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"HandoverPreparationInformation_v1130_IEs", HFILL }},
{ &hf_lte_rrc_as_Context_v1130,
{ "as-Context-v1130", "lte-rrc.as_Context_v1130_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_131,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_50", HFILL }},
{ &hf_lte_rrc_criticalExtensions_40,
{ "criticalExtensions", "lte-rrc.criticalExtensions",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_criticalExtensions_40_vals), 0,
"T_criticalExtensions_40", HFILL }},
{ &hf_lte_rrc_c1_36,
{ "c1", "lte-rrc.c1",
FT_UINT32, BASE_DEC, VALS(lte_rrc_T_c1_36_vals), 0,
"T_c1_36", HFILL }},
{ &hf_lte_rrc_ueRadioAccessCapabilityInformation_r8,
{ "ueRadioAccessCapabilityInformation-r8", "lte-rrc.ueRadioAccessCapabilityInformation_r8_element",
FT_NONE, BASE_NONE, NULL, 0,
"UERadioAccessCapabilityInformation_r8_IEs", HFILL }},
{ &hf_lte_rrc_criticalExtensionsFuture_40,
{ "criticalExtensionsFuture", "lte-rrc.criticalExtensionsFuture_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_criticalExtensionsFuture_40", HFILL }},
{ &hf_lte_rrc_ue_RadioAccessCapabilityInfo_01,
{ "ue-RadioAccessCapabilityInfo", "lte-rrc.ue_RadioAccessCapabilityInfo",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_nonCriticalExtension_132,
{ "nonCriticalExtension", "lte-rrc.nonCriticalExtension_element",
FT_NONE, BASE_NONE, NULL, 0,
"T_nonCriticalExtension_51", HFILL }},
{ &hf_lte_rrc_sourceMeasConfig,
{ "sourceMeasConfig", "lte-rrc.sourceMeasConfig_element",
FT_NONE, BASE_NONE, NULL, 0,
"MeasConfig", HFILL }},
{ &hf_lte_rrc_sourceRadioResourceConfig,
{ "sourceRadioResourceConfig", "lte-rrc.sourceRadioResourceConfig_element",
FT_NONE, BASE_NONE, NULL, 0,
"RadioResourceConfigDedicated", HFILL }},
{ &hf_lte_rrc_sourceSecurityAlgorithmConfig,
{ "sourceSecurityAlgorithmConfig", "lte-rrc.sourceSecurityAlgorithmConfig_element",
FT_NONE, BASE_NONE, NULL, 0,
"SecurityAlgorithmConfig", HFILL }},
{ &hf_lte_rrc_sourceUE_Identity,
{ "sourceUE-Identity", "lte-rrc.sourceUE_Identity",
FT_BYTES, BASE_NONE, NULL, 0,
"C_RNTI", HFILL }},
{ &hf_lte_rrc_sourceMasterInformationBlock,
{ "sourceMasterInformationBlock", "lte-rrc.sourceMasterInformationBlock_element",
FT_NONE, BASE_NONE, NULL, 0,
"MasterInformationBlock", HFILL }},
{ &hf_lte_rrc_sourceSystemInformationBlockType1,
{ "sourceSystemInformationBlockType1", "lte-rrc.sourceSystemInformationBlockType1_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType1", HFILL }},
{ &hf_lte_rrc_sourceSystemInformationBlockType2,
{ "sourceSystemInformationBlockType2", "lte-rrc.sourceSystemInformationBlockType2_element",
FT_NONE, BASE_NONE, NULL, 0,
"SystemInformationBlockType2", HFILL }},
{ &hf_lte_rrc_sourceDl_CarrierFreq,
{ "sourceDl-CarrierFreq", "lte-rrc.sourceDl_CarrierFreq",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA", HFILL }},
{ &hf_lte_rrc_sourceSystemInformationBlockType1Ext,
{ "sourceSystemInformationBlockType1Ext", "lte-rrc.sourceSystemInformationBlockType1Ext",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_sourceOtherConfig_r9,
{ "sourceOtherConfig-r9", "lte-rrc.sourceOtherConfig_r9_element",
FT_NONE, BASE_NONE, NULL, 0,
"OtherConfig_r9", HFILL }},
{ &hf_lte_rrc_sourceSCellConfigList_r10,
{ "sourceSCellConfigList-r10", "lte-rrc.sourceSCellConfigList_r10",
FT_UINT32, BASE_DEC, NULL, 0,
"SCellToAddModList_r10", HFILL }},
{ &hf_lte_rrc_sourceDl_CarrierFreq_v9e0,
{ "sourceDl-CarrierFreq-v9e0", "lte-rrc.sourceDl_CarrierFreq_v9e0",
FT_UINT32, BASE_DEC, NULL, 0,
"ARFCN_ValueEUTRA_v9e0", HFILL }},
{ &hf_lte_rrc_reestablishmentInfo,
{ "reestablishmentInfo", "lte-rrc.reestablishmentInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_idc_Indication_r11_01,
{ "idc-Indication-r11", "lte-rrc.idc_Indication_r11",
FT_BYTES, BASE_NONE, NULL, 0,
"T_idc_Indication_r11_01", HFILL }},
{ &hf_lte_rrc_mbmsInterestIndication_r11_01,
{ "mbmsInterestIndication-r11", "lte-rrc.mbmsInterestIndication_r11",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_powerPrefIndication_r11_01,
{ "powerPrefIndication-r11", "lte-rrc.powerPrefIndication_r11",
FT_BYTES, BASE_NONE, NULL, 0,
"T_powerPrefIndication_r11_01", HFILL }},
{ &hf_lte_rrc_sourcePhysCellId,
{ "sourcePhysCellId", "lte-rrc.sourcePhysCellId",
FT_UINT32, BASE_DEC, NULL, 0,
"PhysCellId", HFILL }},
{ &hf_lte_rrc_targetCellShortMAC_I,
{ "targetCellShortMAC-I", "lte-rrc.targetCellShortMAC_I",
FT_BYTES, BASE_NONE, NULL, 0,
"ShortMAC_I", HFILL }},
{ &hf_lte_rrc_additionalReestabInfoList,
{ "additionalReestabInfoList", "lte-rrc.additionalReestabInfoList",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_AdditionalReestabInfoList_item,
{ "AdditionalReestabInfo", "lte-rrc.AdditionalReestabInfo_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_key_eNodeB_Star,
{ "key-eNodeB-Star", "lte-rrc.key_eNodeB_Star",
FT_BYTES, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_ue_InactiveTime,
{ "ue-InactiveTime", "lte-rrc.ue_InactiveTime",
FT_UINT32, BASE_DEC|BASE_EXT_STRING, &lte_rrc_T_ue_InactiveTime_vals_ext, 0,
NULL, HFILL }},
{ &hf_lte_rrc_candidateCellInfoList_r10,
{ "candidateCellInfoList-r10", "lte-rrc.candidateCellInfoList_r10",
FT_UINT32, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_CandidateCellInfoList_r10_item,
{ "CandidateCellInfo-r10", "lte-rrc.CandidateCellInfo_r10_element",
FT_NONE, BASE_NONE, NULL, 0,
NULL, HFILL }},
#line 2453 "../../asn1/lte-rrc/packet-lte-rrc-template.c"
{ &hf_lte_rrc_eutra_cap_feat_group_ind_1,
{ "Indicator 1", "lte-rrc.eutra_cap_feat_group_ind_1",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_1_val), 0,
"EUTRA Feature Group Indicator 1", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_2,
{ "Indicator 2", "lte-rrc.eutra_cap_feat_group_ind_2",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_2_val), 0,
"EUTRA Feature Group Indicator 2", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_3,
{ "Indicator 3", "lte-rrc.eutra_cap_feat_group_ind_3",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_3_val), 0,
"EUTRA Feature Group Indicator 3", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_4,
{ "Indicator 4", "lte-rrc.eutra_cap_feat_group_ind_4",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_4_val), 0,
"EUTRA Feature Group Indicator 4", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_5,
{ "Indicator 5", "lte-rrc.eutra_cap_feat_group_ind_5",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_5_val), 0,
"EUTRA Feature Group Indicator 5", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_6,
{ "Indicator 6", "lte-rrc.eutra_cap_feat_group_ind_6",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_6_val), 0,
"EUTRA Feature Group Indicator 6", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_7,
{ "Indicator 7", "lte-rrc.eutra_cap_feat_group_ind_7",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_7_val), 0,
"EUTRA Feature Group Indicator 7", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_8,
{ "Indicator 8", "lte-rrc.eutra_cap_feat_group_ind_8",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_8_val), 0,
"EUTRA Feature Group Indicator 8", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_9,
{ "Indicator 9", "lte-rrc.eutra_cap_feat_group_ind_9",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_9_val), 0,
"EUTRA Feature Group Indicator 9", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_10,
{ "Indicator 10", "lte-rrc.eutra_cap_feat_group_ind_10",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_10_val), 0,
"EUTRA Feature Group Indicator 10", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_11,
{ "Indicator 11", "lte-rrc.eutra_cap_feat_group_ind_11",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_11_val), 0,
"EUTRA Feature Group Indicator 11", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_12,
{ "Indicator 12", "lte-rrc.eutra_cap_feat_group_ind_12",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_12_val), 0,
"EUTRA Feature Group Indicator 12", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_13,
{ "Indicator 13", "lte-rrc.eutra_cap_feat_group_ind_13",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_13_val), 0,
"EUTRA Feature Group Indicator", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_14,
{ "Indicator 14", "lte-rrc.eutra_cap_feat_group_ind_14",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_14_val), 0,
"EUTRA Feature Group Indicator 14", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_15,
{ "Indicator 15", "lte-rrc.eutra_cap_feat_group_ind_15",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_15_val), 0,
"EUTRA Feature Group Indicator 15", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_16,
{ "Indicator 16", "lte-rrc.eutra_cap_feat_group_ind_16",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_16_val), 0,
"EUTRA Feature Group Indicator 16", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_17,
{ "Indicator 17", "lte-rrc.eutra_cap_feat_group_ind_17",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_17_val), 0,
"EUTRA Feature Group Indicator 17", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_18,
{ "Indicator 18", "lte-rrc.eutra_cap_feat_group_ind_18",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_18_val), 0,
"EUTRA Feature Group Indicator 18", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_19,
{ "Indicator 19", "lte-rrc.eutra_cap_feat_group_ind_19",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_19_val), 0,
"EUTRA Feature Group Indicator 19", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_20,
{ "Indicator 20", "lte-rrc.eutra_cap_feat_group_ind_20",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_20_val), 0,
"EUTRA Feature Group Indicator 20", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_21,
{ "Indicator 21", "lte-rrc.eutra_cap_feat_group_ind_21",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_21_val), 0,
"EUTRA Feature Group Indicator 21", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_22,
{ "Indicator 22", "lte-rrc.eutra_cap_feat_group_ind_22",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_22_val), 0,
"EUTRA Feature Group Indicator 22", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_23,
{ "Indicator 23", "lte-rrc.eutra_cap_feat_group_ind_23",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_23_val), 0,
"EUTRA Feature Group Indicator 23", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_24,
{ "Indicator 24", "lte-rrc.eutra_cap_feat_group_ind_24",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_24_val), 0,
"EUTRA Feature Group Indicator 24", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_25,
{ "Indicator 25", "lte-rrc.eutra_cap_feat_group_ind_25",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_25_val), 0,
"EUTRA Feature Group Indicator 25", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_26,
{ "Indicator 26", "lte-rrc.eutra_cap_feat_group_ind_26",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_26_val), 0,
"EUTRA Feature Group Indicator 26", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_27,
{ "Indicator 27", "lte-rrc.eutra_cap_feat_group_ind_27",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_27_val), 0,
"EUTRA Feature Group Indicator 27", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_28,
{ "Indicator 28", "lte-rrc.eutra_cap_feat_group_ind_28",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_28_val), 0,
"EUTRA Feature Group Indicator 28", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_29,
{ "Indicator 29", "lte-rrc.eutra_cap_feat_group_ind_29",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_29_val), 0,
"EUTRA Feature Group Indicator 29", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_30,
{ "Indicator 30", "lte-rrc.eutra_cap_feat_group_ind_30",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_30_val), 0,
"EUTRA Feature Group Indicator 30", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_31,
{ "Indicator 31", "lte-rrc.eutra_cap_feat_group_ind_31",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_31_val), 0,
"EUTRA Feature Group Indicator 31", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_32,
{ "Indicator 32", "lte-rrc.eutra_cap_feat_group_ind_32",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_32_val), 0,
"EUTRA Feature Group Indicator 32", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_33,
{ "Indicator 33", "lte-rrc.eutra_cap_feat_group_ind_33",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_33_val), 0,
"EUTRA Feature Group Indicator 33", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_34,
{ "Indicator 34", "lte-rrc.eutra_cap_feat_group_ind_34",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_34_val), 0,
"EUTRA Feature Group Indicator 34", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_35,
{ "Indicator 35", "lte-rrc.eutra_cap_feat_group_ind_35",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_35_val), 0,
"EUTRA Feature Group Indicator 35", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_36,
{ "Indicator 36", "lte-rrc.eutra_cap_feat_group_ind_36",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_36_val), 0,
"EUTRA Feature Group Indicator 36", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_37,
{ "Indicator 37", "lte-rrc.eutra_cap_feat_group_ind_37",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_37_val), 0,
"EUTRA Feature Group Indicator 37", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_38,
{ "Indicator 38", "lte-rrc.eutra_cap_feat_group_ind_38",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_38_val), 0,
"EUTRA Feature Group Indicator 38", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_39,
{ "Indicator 39", "lte-rrc.eutra_cap_feat_group_ind_39",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_39_val), 0,
"EUTRA Feature Group Indicator 39", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_40,
{ "Indicator 40", "lte-rrc.eutra_cap_feat_group_ind_40",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_40_val), 0,
"EUTRA Feature Group Indicator 40", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_41,
{ "Indicator 41", "lte-rrc.eutra_cap_feat_group_ind_41",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_41_val), 0,
"EUTRA Feature Group Indicator 41", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_42,
{ "Indicator 42", "lte-rrc.eutra_cap_feat_group_ind_42",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_42_val), 0,
"EUTRA Feature Group Indicator 42", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_43,
{ "Indicator 43", "lte-rrc.eutra_cap_feat_group_ind_43",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_43_val), 0,
"EUTRA Feature Group Indicator 43", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_44,
{ "Indicator 44", "lte-rrc.eutra_cap_feat_group_ind_44",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_44_val), 0,
"EUTRA Feature Group Indicator 44", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_45,
{ "Indicator 45", "lte-rrc.eutra_cap_feat_group_ind_45",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_45_val), 0,
"EUTRA Feature Group Indicator 45", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_46,
{ "Indicator 46", "lte-rrc.eutra_cap_feat_group_ind_46",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_46_val), 0,
"EUTRA Feature Group Indicator 46", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_47,
{ "Indicator 47", "lte-rrc.eutra_cap_feat_group_ind_47",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_47_val), 0,
"EUTRA Feature Group Indicator 47", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_48,
{ "Indicator 48", "lte-rrc.eutra_cap_feat_group_ind_48",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_48_val), 0,
"EUTRA Feature Group Indicator 48", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_49,
{ "Indicator 49", "lte-rrc.eutra_cap_feat_group_ind_49",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_49_val), 0,
"EUTRA Feature Group Indicator 49", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_50,
{ "Indicator 50", "lte-rrc.eutra_cap_feat_group_ind_50",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_50_val), 0,
"EUTRA Feature Group Indicator 50", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_51,
{ "Indicator 51", "lte-rrc.eutra_cap_feat_group_ind_51",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_51_val), 0,
"EUTRA Feature Group Indicator 51", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_52,
{ "Indicator 52", "lte-rrc.eutra_cap_feat_group_ind_52",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_52_val), 0,
"EUTRA Feature Group Indicator 52", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_53,
{ "Indicator 53", "lte-rrc.eutra_cap_feat_group_ind_53",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_53_val), 0,
"EUTRA Feature Group Indicator 53", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_54,
{ "Indicator 54", "lte-rrc.eutra_cap_feat_group_ind_54",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_54_val), 0,
"EUTRA Feature Group Indicator 54", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_55,
{ "Indicator 55", "lte-rrc.eutra_cap_feat_group_ind_55",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_55_val), 0,
"EUTRA Feature Group Indicator 55", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_56,
{ "Indicator 56", "lte-rrc.eutra_cap_feat_group_ind_56",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_56_val), 0,
"EUTRA Feature Group Indicator 56", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_57,
{ "Indicator 57", "lte-rrc.eutra_cap_feat_group_ind_57",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_57_val), 0,
"EUTRA Feature Group Indicator 57", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_58,
{ "Indicator 58", "lte-rrc.eutra_cap_feat_group_ind_58",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_58_val), 0,
"EUTRA Feature Group Indicator 58", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_59,
{ "Indicator 59", "lte-rrc.eutra_cap_feat_group_ind_59",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_59_val), 0,
"EUTRA Feature Group Indicator 59", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_60,
{ "Indicator 60", "lte-rrc.eutra_cap_feat_group_ind_60",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_60_val), 0,
"EUTRA Feature Group Indicator 60", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_61,
{ "Indicator 61", "lte-rrc.eutra_cap_feat_group_ind_61",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_61_val), 0,
"EUTRA Feature Group Indicator 61", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_62,
{ "Indicator 62", "lte-rrc.eutra_cap_feat_group_ind_62",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_62_val), 0,
"EUTRA Feature Group Indicator 62", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_63,
{ "Indicator 63", "lte-rrc.eutra_cap_feat_group_ind_63",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_63_val), 0,
"EUTRA Feature Group Indicator 63", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_64,
{ "Indicator 64", "lte-rrc.eutra_cap_feat_group_ind_64",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_64_val), 0,
"EUTRA Feature Group Indicator 64", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_101,
{ "Indicator 101", "lte-rrc.eutra_cap_feat_group_ind_101",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_101_val), 0,
"EUTRA Feature Group Indicator 101", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_102,
{ "Indicator 102", "lte-rrc.eutra_cap_feat_group_ind_102",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_102_val), 0,
"EUTRA Feature Group Indicator 102", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_103,
{ "Indicator 103", "lte-rrc.eutra_cap_feat_group_ind_103",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_103_val), 0,
"EUTRA Feature Group Indicator 103", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_104,
{ "Indicator 104", "lte-rrc.eutra_cap_feat_group_ind_104",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_104_val), 0,
"EUTRA Feature Group Indicator 104", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_105,
{ "Indicator 105", "lte-rrc.eutra_cap_feat_group_ind_105",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_105_val), 0,
"EUTRA Feature Group Indicator 105", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_106,
{ "Indicator 106", "lte-rrc.eutra_cap_feat_group_ind_106",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_106_val), 0,
"EUTRA Feature Group Indicator 106", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_107,
{ "Indicator 107", "lte-rrc.eutra_cap_feat_group_ind_107",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_107_val), 0,
"EUTRA Feature Group Indicator 107", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_108,
{ "Indicator 108", "lte-rrc.eutra_cap_feat_group_ind_108",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_108_val), 0,
"EUTRA Feature Group Indicator 108", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_109,
{ "Indicator 109", "lte-rrc.eutra_cap_feat_group_ind_109",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_109_val), 0,
"EUTRA Feature Group Indicator 109", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_110,
{ "Indicator 110", "lte-rrc.eutra_cap_feat_group_ind_110",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_110_val), 0,
"EUTRA Feature Group Indicator 110", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_111,
{ "Indicator 111", "lte-rrc.eutra_cap_feat_group_ind_111",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_111_val), 0,
"EUTRA Feature Group Indicator 111", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_112,
{ "Indicator 112", "lte-rrc.eutra_cap_feat_group_ind_112",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_112_val), 0,
"EUTRA Feature Group Indicator 112", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_113,
{ "Indicator 113", "lte-rrc.eutra_cap_feat_group_ind_113",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_113_val), 0,
"EUTRA Feature Group Indicator 113", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_114,
{ "Indicator 114", "lte-rrc.eutra_cap_feat_group_ind_114",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_114_val), 0,
"EUTRA Feature Group Indicator 114", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_115,
{ "Indicator 115", "lte-rrc.eutra_cap_feat_group_ind_115",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_115_val), 0,
"EUTRA Feature Group Indicator 115", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_116,
{ "Indicator 116", "lte-rrc.eutra_cap_feat_group_ind_116",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_116_val), 0,
"EUTRA Feature Group Indicator 116", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_117,
{ "Indicator 117", "lte-rrc.eutra_cap_feat_group_ind_117",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_117_val), 0,
"EUTRA Feature Group Indicator 117", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_118,
{ "Indicator 118", "lte-rrc.eutra_cap_feat_group_ind_118",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_118_val), 0,
"EUTRA Feature Group Indicator 118", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_119,
{ "Indicator 119", "lte-rrc.eutra_cap_feat_group_ind_119",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_119_val), 0,
"EUTRA Feature Group Indicator 119", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_120,
{ "Indicator 120", "lte-rrc.eutra_cap_feat_group_ind_120",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_120_val), 0,
"EUTRA Feature Group Indicator 120", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_121,
{ "Indicator 121", "lte-rrc.eutra_cap_feat_group_ind_121",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_121_val), 0,
"EUTRA Feature Group Indicator 121", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_122,
{ "Indicator 122", "lte-rrc.eutra_cap_feat_group_ind_122",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_122_val), 0,
"EUTRA Feature Group Indicator 122", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_123,
{ "Indicator 123", "lte-rrc.eutra_cap_feat_group_ind_123",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_123_val), 0,
"EUTRA Feature Group Indicator 123", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_124,
{ "Indicator 124", "lte-rrc.eutra_cap_feat_group_ind_124",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_124_val), 0,
"EUTRA Feature Group Indicator 124", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_125,
{ "Indicator 125", "lte-rrc.eutra_cap_feat_group_ind_125",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_125_val), 0,
"EUTRA Feature Group Indicator 125", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_126,
{ "Indicator 126", "lte-rrc.eutra_cap_feat_group_ind_126",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_126_val), 0,
"EUTRA Feature Group Indicator 126", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_127,
{ "Indicator 127", "lte-rrc.eutra_cap_feat_group_ind_127",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_127_val), 0,
"EUTRA Feature Group Indicator 127", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_128,
{ "Indicator 128", "lte-rrc.eutra_cap_feat_group_ind_128",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_128_val), 0,
"EUTRA Feature Group Indicator 128", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_129,
{ "Indicator 129", "lte-rrc.eutra_cap_feat_group_ind_129",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_129_val), 0,
"EUTRA Feature Group Indicator 129", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_130,
{ "Indicator 130", "lte-rrc.eutra_cap_feat_group_ind_130",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_130_val), 0,
"EUTRA Feature Group Indicator 130", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_131,
{ "Indicator 131", "lte-rrc.eutra_cap_feat_group_ind_131",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_131_val), 0,
"EUTRA Feature Group Indicator 131", HFILL }},
{ &hf_lte_rrc_eutra_cap_feat_group_ind_132,
{ "Indicator 132", "lte-rrc.eutra_cap_feat_group_ind_132",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_eutra_cap_feat_group_ind_132_val), 0,
"EUTRA Feature Group Indicator 132", HFILL }},
{ &hf_lte_rrc_serialNumber_gs,
{ "Geographical Scope", "lte-rrc.serialNumber.gs",
FT_UINT16, BASE_DEC, VALS(lte_rrc_serialNumber_gs_vals), 0xc000,
NULL, HFILL }},
{ &hf_lte_rrc_serialNumber_msg_code,
{ "Message Code", "lte-rrc.serialNumber.msg_code",
FT_UINT16, BASE_DEC, NULL, 0x3ff0,
NULL, HFILL }},
{ &hf_lte_rrc_serialNumber_upd_nb,
{ "Update Number", "lte-rrc.serialNumber.upd_nb",
FT_UINT16, BASE_DEC, NULL, 0x000f,
NULL, HFILL }},
{ &hf_lte_rrc_warningType_value,
{ "Warning Type Value", "lte-rrc.warningType.value",
FT_UINT16, BASE_DEC, VALS(lte_rrc_warningType_vals), 0xfe00,
NULL, HFILL }},
{ &hf_lte_rrc_warningType_emergency_user_alert,
{ "Emergency User Alert", "lte-rrc.warningType.emergency_user_alert",
FT_BOOLEAN, 16, TFS(&tfs_yes_no), 0x0100,
NULL, HFILL }},
{ &hf_lte_rrc_warningType_popup,
{ "Popup", "lte-rrc.warningType.popup",
FT_BOOLEAN, 16, TFS(&tfs_yes_no), 0x0080,
NULL, HFILL }},
{ &hf_lte_rrc_warningMessageSegment_nb_pages,
{ "Number of Pages", "lte-rrc.warningMessageSegment.nb_pages",
FT_UINT8, BASE_DEC, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_warningMessageSegment_decoded_page,
{ "Decoded Page", "lte-rrc.warningMessageSegment.decoded_page",
FT_STRING, BASE_NONE, NULL, 0,
NULL, HFILL }},
{ &hf_lte_rrc_interBandTDD_CA_WithDifferentConfig_bit1,
{ "Bit 1", "lte-rrc.interBandTDD_CA_WithDifferentConfig.bit1",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_interBandTDD_CA_WithDifferentConfig_bit1_val), 0,
NULL, HFILL }},
{ &hf_lte_rrc_interBandTDD_CA_WithDifferentConfig_bit2,
{ "Bit 2", "lte-rrc.interBandTDD_CA_WithDifferentConfig.bit2",
FT_BOOLEAN, BASE_NONE, TFS(&lte_rrc_interBandTDD_CA_WithDifferentConfig_bit2_val), 0,
NULL, HFILL }},
{ &hf_lte_rrc_sr_config_periodicity,
{ "Periodicity", "lte-rrc.sr_Periodicity",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }},
{ &hf_lte_rrc_sr_config_subframe_offset,
{ "Subframe Offset", "lte-rrc.sr_SubframeOffset",
FT_UINT16, BASE_DEC, NULL, 0x0,
NULL, HFILL }},
{ &hf_lte_rrc_cdma_time,
{ "CDMA time", "lte-rrc.cdma_time",
FT_ABSOLUTE_TIME, ABSOLUTE_TIME_UTC, NULL, 0x0,
NULL, HFILL }},
{ &hf_lte_rrc_utc_time,
{ "UTC time", "lte-rrc.utc_time",
FT_ABSOLUTE_TIME, ABSOLUTE_TIME_UTC, NULL, 0x0,
NULL, HFILL }},
{ &hf_lte_rrc_local_time,
{ "Local time", "lte-rrc.local_time",
FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x0,
NULL, HFILL }},
{ &hf_lte_rrc_absolute_time,
{ "Absolute time", "lte-rrc.absolute_time",
FT_STRING, BASE_NONE, NULL, 0x0,
NULL, HFILL }},
};
static gint *ett[] = {
&ett_lte_rrc,
#line 1 "../../asn1/lte-rrc/packet-lte-rrc-ettarr.c"
&ett_lte_rrc_BCCH_BCH_Message,
&ett_lte_rrc_BCCH_DL_SCH_Message,
&ett_lte_rrc_BCCH_DL_SCH_MessageType,
&ett_lte_rrc_T_c1,
&ett_lte_rrc_T_messageClassExtension,
&ett_lte_rrc_MCCH_Message,
&ett_lte_rrc_MCCH_MessageType,
&ett_lte_rrc_T_c1_01,
&ett_lte_rrc_T_later,
&ett_lte_rrc_T_c2,
&ett_lte_rrc_T_messageClassExtension_01,
&ett_lte_rrc_PCCH_Message,
&ett_lte_rrc_PCCH_MessageType,
&ett_lte_rrc_T_c1_02,
&ett_lte_rrc_T_messageClassExtension_02,
&ett_lte_rrc_DL_CCCH_Message,
&ett_lte_rrc_DL_CCCH_MessageType,
&ett_lte_rrc_T_c1_03,
&ett_lte_rrc_T_messageClassExtension_03,
&ett_lte_rrc_DL_DCCH_Message,
&ett_lte_rrc_DL_DCCH_MessageType,
&ett_lte_rrc_T_c1_04,
&ett_lte_rrc_T_messageClassExtension_04,
&ett_lte_rrc_UL_CCCH_Message,
&ett_lte_rrc_UL_CCCH_MessageType,
&ett_lte_rrc_T_c1_05,
&ett_lte_rrc_T_messageClassExtension_05,
&ett_lte_rrc_UL_DCCH_Message,
&ett_lte_rrc_UL_DCCH_MessageType,
&ett_lte_rrc_T_c1_06,
&ett_lte_rrc_T_messageClassExtension_06,
&ett_lte_rrc_T_c2_01,
&ett_lte_rrc_T_messageClassExtensionFuture_r11,
&ett_lte_rrc_CounterCheck,
&ett_lte_rrc_T_criticalExtensions,
&ett_lte_rrc_T_c1_07,
&ett_lte_rrc_T_criticalExtensionsFuture,
&ett_lte_rrc_CounterCheck_r8_IEs,
&ett_lte_rrc_CounterCheck_v8a0_IEs,
&ett_lte_rrc_T_nonCriticalExtension,
&ett_lte_rrc_DRB_CountMSB_InfoList,
&ett_lte_rrc_DRB_CountMSB_Info,
&ett_lte_rrc_CounterCheckResponse,
&ett_lte_rrc_T_criticalExtensions_01,
&ett_lte_rrc_T_criticalExtensionsFuture_01,
&ett_lte_rrc_CounterCheckResponse_r8_IEs,
&ett_lte_rrc_CounterCheckResponse_v8a0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_01,
&ett_lte_rrc_DRB_CountInfoList,
&ett_lte_rrc_DRB_CountInfo,
&ett_lte_rrc_CSFBParametersRequestCDMA2000,
&ett_lte_rrc_T_criticalExtensions_02,
&ett_lte_rrc_T_criticalExtensionsFuture_02,
&ett_lte_rrc_CSFBParametersRequestCDMA2000_r8_IEs,
&ett_lte_rrc_CSFBParametersRequestCDMA2000_v8a0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_02,
&ett_lte_rrc_CSFBParametersResponseCDMA2000,
&ett_lte_rrc_T_criticalExtensions_03,
&ett_lte_rrc_T_criticalExtensionsFuture_03,
&ett_lte_rrc_CSFBParametersResponseCDMA2000_r8_IEs,
&ett_lte_rrc_CSFBParametersResponseCDMA2000_v8a0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_03,
&ett_lte_rrc_DLInformationTransfer,
&ett_lte_rrc_T_criticalExtensions_04,
&ett_lte_rrc_T_c1_08,
&ett_lte_rrc_T_criticalExtensionsFuture_04,
&ett_lte_rrc_DLInformationTransfer_r8_IEs,
&ett_lte_rrc_T_dedicatedInfoType,
&ett_lte_rrc_DLInformationTransfer_v8a0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_04,
&ett_lte_rrc_HandoverFromEUTRAPreparationRequest,
&ett_lte_rrc_T_criticalExtensions_05,
&ett_lte_rrc_T_c1_09,
&ett_lte_rrc_T_criticalExtensionsFuture_05,
&ett_lte_rrc_HandoverFromEUTRAPreparationRequest_r8_IEs,
&ett_lte_rrc_HandoverFromEUTRAPreparationRequest_v890_IEs,
&ett_lte_rrc_HandoverFromEUTRAPreparationRequest_v920_IEs,
&ett_lte_rrc_HandoverFromEUTRAPreparationRequest_v1020_IEs,
&ett_lte_rrc_T_nonCriticalExtension_05,
&ett_lte_rrc_InDeviceCoexIndication_r11,
&ett_lte_rrc_T_criticalExtensions_06,
&ett_lte_rrc_T_c1_10,
&ett_lte_rrc_T_criticalExtensionsFuture_06,
&ett_lte_rrc_InDeviceCoexIndication_r11_IEs,
&ett_lte_rrc_T_nonCriticalExtension_06,
&ett_lte_rrc_AffectedCarrierFreqList_r11,
&ett_lte_rrc_AffectedCarrierFreq_r11,
&ett_lte_rrc_TDM_AssistanceInfo_r11,
&ett_lte_rrc_T_drx_AssistanceInfo_r11,
&ett_lte_rrc_IDC_SubframePatternList_r11,
&ett_lte_rrc_IDC_SubframePattern_r11,
&ett_lte_rrc_T_subframePatternTDD_r11,
&ett_lte_rrc_InterFreqRSTDMeasurementIndication_r10,
&ett_lte_rrc_T_criticalExtensions_07,
&ett_lte_rrc_T_c1_11,
&ett_lte_rrc_T_criticalExtensionsFuture_07,
&ett_lte_rrc_InterFreqRSTDMeasurementIndication_r10_IEs,
&ett_lte_rrc_T_rstd_InterFreqIndication_r10,
&ett_lte_rrc_T_start,
&ett_lte_rrc_T_nonCriticalExtension_07,
&ett_lte_rrc_RSTD_InterFreqInfoList_r10,
&ett_lte_rrc_RSTD_InterFreqInfo_r10,
&ett_lte_rrc_LoggedMeasurementConfiguration_r10,
&ett_lte_rrc_T_criticalExtensions_08,
&ett_lte_rrc_T_c1_12,
&ett_lte_rrc_T_criticalExtensionsFuture_08,
&ett_lte_rrc_LoggedMeasurementConfiguration_r10_IEs,
&ett_lte_rrc_LoggedMeasurementConfiguration_v1080_IEs,
&ett_lte_rrc_LoggedMeasurementConfiguration_v1130_IEs,
&ett_lte_rrc_T_nonCriticalExtension_08,
&ett_lte_rrc_MasterInformationBlock,
&ett_lte_rrc_MBMSCountingRequest_r10,
&ett_lte_rrc_T_nonCriticalExtension_09,
&ett_lte_rrc_CountingRequestList_r10,
&ett_lte_rrc_CountingRequestInfo_r10,
&ett_lte_rrc_MBMSCountingResponse_r10,
&ett_lte_rrc_T_criticalExtensions_09,
&ett_lte_rrc_T_c1_13,
&ett_lte_rrc_T_criticalExtensionsFuture_09,
&ett_lte_rrc_MBMSCountingResponse_r10_IEs,
&ett_lte_rrc_T_nonCriticalExtension_10,
&ett_lte_rrc_CountingResponseList_r10,
&ett_lte_rrc_CountingResponseInfo_r10,
&ett_lte_rrc_MBMSInterestIndication_r11,
&ett_lte_rrc_T_criticalExtensions_10,
&ett_lte_rrc_T_c1_14,
&ett_lte_rrc_T_criticalExtensionsFuture_10,
&ett_lte_rrc_MBMSInterestIndication_r11_IEs,
&ett_lte_rrc_T_nonCriticalExtension_11,
&ett_lte_rrc_MBSFNAreaConfiguration_r9,
&ett_lte_rrc_MBSFNAreaConfiguration_v930_IEs,
&ett_lte_rrc_T_nonCriticalExtension_12,
&ett_lte_rrc_CommonSF_AllocPatternList_r9,
&ett_lte_rrc_MeasurementReport,
&ett_lte_rrc_T_criticalExtensions_11,
&ett_lte_rrc_T_c1_15,
&ett_lte_rrc_T_criticalExtensionsFuture_11,
&ett_lte_rrc_MeasurementReport_r8_IEs,
&ett_lte_rrc_MeasurementReport_v8a0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_13,
&ett_lte_rrc_MobilityFromEUTRACommand,
&ett_lte_rrc_T_criticalExtensions_12,
&ett_lte_rrc_T_c1_16,
&ett_lte_rrc_T_criticalExtensionsFuture_12,
&ett_lte_rrc_MobilityFromEUTRACommand_r8_IEs,
&ett_lte_rrc_T_purpose,
&ett_lte_rrc_MobilityFromEUTRACommand_v8a0_IEs,
&ett_lte_rrc_MobilityFromEUTRACommand_v8d0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_14,
&ett_lte_rrc_MobilityFromEUTRACommand_r9_IEs,
&ett_lte_rrc_T_purpose_01,
&ett_lte_rrc_MobilityFromEUTRACommand_v930_IEs,
&ett_lte_rrc_MobilityFromEUTRACommand_v960_IEs,
&ett_lte_rrc_T_nonCriticalExtension_15,
&ett_lte_rrc_Handover,
&ett_lte_rrc_CellChangeOrder,
&ett_lte_rrc_T_targetRAT_Type_01,
&ett_lte_rrc_T_geran,
&ett_lte_rrc_SI_OrPSI_GERAN,
&ett_lte_rrc_E_CSFB_r9,
&ett_lte_rrc_Paging,
&ett_lte_rrc_Paging_v890_IEs,
&ett_lte_rrc_Paging_v920_IEs,
&ett_lte_rrc_Paging_v1130_IEs,
&ett_lte_rrc_T_nonCriticalExtension_16,
&ett_lte_rrc_PagingRecordList,
&ett_lte_rrc_PagingRecord,
&ett_lte_rrc_PagingUE_Identity,
&ett_lte_rrc_IMSI,
&ett_lte_rrc_ProximityIndication_r9,
&ett_lte_rrc_T_criticalExtensions_13,
&ett_lte_rrc_T_c1_17,
&ett_lte_rrc_T_criticalExtensionsFuture_13,
&ett_lte_rrc_ProximityIndication_r9_IEs,
&ett_lte_rrc_T_carrierFreq_r9,
&ett_lte_rrc_ProximityIndication_v930_IEs,
&ett_lte_rrc_T_nonCriticalExtension_17,
&ett_lte_rrc_RNReconfiguration_r10,
&ett_lte_rrc_T_criticalExtensions_14,
&ett_lte_rrc_T_c1_18,
&ett_lte_rrc_T_criticalExtensionsFuture_14,
&ett_lte_rrc_RNReconfiguration_r10_IEs,
&ett_lte_rrc_T_nonCriticalExtension_18,
&ett_lte_rrc_RN_SystemInfo_r10,
&ett_lte_rrc_RNReconfigurationComplete_r10,
&ett_lte_rrc_T_criticalExtensions_15,
&ett_lte_rrc_T_c1_19,
&ett_lte_rrc_T_criticalExtensionsFuture_15,
&ett_lte_rrc_RNReconfigurationComplete_r10_IEs,
&ett_lte_rrc_T_nonCriticalExtension_19,
&ett_lte_rrc_RRCConnectionReconfiguration,
&ett_lte_rrc_T_criticalExtensions_16,
&ett_lte_rrc_T_c1_20,
&ett_lte_rrc_T_criticalExtensionsFuture_16,
&ett_lte_rrc_RRCConnectionReconfiguration_r8_IEs,
&ett_lte_rrc_SEQUENCE_SIZE_1_maxDRB_OF_DedicatedInfoNAS,
&ett_lte_rrc_RRCConnectionReconfiguration_v890_IEs,
&ett_lte_rrc_RRCConnectionReconfiguration_v920_IEs,
&ett_lte_rrc_RRCConnectionReconfiguration_v1020_IEs,
&ett_lte_rrc_RRCConnectionReconfiguration_v1130_IEs,
&ett_lte_rrc_T_nonCriticalExtension_20,
&ett_lte_rrc_SCellToAddModList_r10,
&ett_lte_rrc_SCellToAddMod_r10,
&ett_lte_rrc_T_cellIdentification_r10,
&ett_lte_rrc_SCellToReleaseList_r10,
&ett_lte_rrc_SecurityConfigHO,
&ett_lte_rrc_T_handoverType,
&ett_lte_rrc_T_intraLTE,
&ett_lte_rrc_T_interRAT,
&ett_lte_rrc_RRCConnectionReconfigurationComplete,
&ett_lte_rrc_T_criticalExtensions_17,
&ett_lte_rrc_T_criticalExtensionsFuture_17,
&ett_lte_rrc_RRCConnectionReconfigurationComplete_r8_IEs,
&ett_lte_rrc_RRCConnectionReconfigurationComplete_v8a0_IEs,
&ett_lte_rrc_RRCConnectionReconfigurationComplete_v1020_IEs,
&ett_lte_rrc_RRCConnectionReconfigurationComplete_v1130_IEs,
&ett_lte_rrc_T_nonCriticalExtension_21,
&ett_lte_rrc_RRCConnectionReestablishment,
&ett_lte_rrc_T_criticalExtensions_18,
&ett_lte_rrc_T_c1_21,
&ett_lte_rrc_T_criticalExtensionsFuture_18,
&ett_lte_rrc_RRCConnectionReestablishment_r8_IEs,
&ett_lte_rrc_RRCConnectionReestablishment_v8a0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_22,
&ett_lte_rrc_RRCConnectionReestablishmentComplete,
&ett_lte_rrc_T_criticalExtensions_19,
&ett_lte_rrc_T_criticalExtensionsFuture_19,
&ett_lte_rrc_RRCConnectionReestablishmentComplete_r8_IEs,
&ett_lte_rrc_RRCConnectionReestablishmentComplete_v920_IEs,
&ett_lte_rrc_RRCConnectionReestablishmentComplete_v8a0_IEs,
&ett_lte_rrc_RRCConnectionReestablishmentComplete_v1020_IEs,
&ett_lte_rrc_RRCConnectionReestablishmentComplete_v1130_IEs,
&ett_lte_rrc_T_nonCriticalExtension_23,
&ett_lte_rrc_RRCConnectionReestablishmentReject,
&ett_lte_rrc_T_criticalExtensions_20,
&ett_lte_rrc_T_criticalExtensionsFuture_20,
&ett_lte_rrc_RRCConnectionReestablishmentReject_r8_IEs,
&ett_lte_rrc_RRCConnectionReestablishmentReject_v8a0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_24,
&ett_lte_rrc_RRCConnectionReestablishmentRequest,
&ett_lte_rrc_T_criticalExtensions_21,
&ett_lte_rrc_T_criticalExtensionsFuture_21,
&ett_lte_rrc_RRCConnectionReestablishmentRequest_r8_IEs,
&ett_lte_rrc_ReestabUE_Identity,
&ett_lte_rrc_RRCConnectionReject,
&ett_lte_rrc_T_criticalExtensions_22,
&ett_lte_rrc_T_c1_22,
&ett_lte_rrc_T_criticalExtensionsFuture_22,
&ett_lte_rrc_RRCConnectionReject_r8_IEs,
&ett_lte_rrc_RRCConnectionReject_v8a0_IEs,
&ett_lte_rrc_RRCConnectionReject_v1020_IEs,
&ett_lte_rrc_RRCConnectionReject_v1130_IEs,
&ett_lte_rrc_T_deprioritisationReq_r11,
&ett_lte_rrc_T_nonCriticalExtension_25,
&ett_lte_rrc_RRCConnectionRelease,
&ett_lte_rrc_T_criticalExtensions_23,
&ett_lte_rrc_T_c1_23,
&ett_lte_rrc_T_criticalExtensionsFuture_23,
&ett_lte_rrc_RRCConnectionRelease_r8_IEs,
&ett_lte_rrc_RRCConnectionRelease_v890_IEs,
&ett_lte_rrc_RRCConnectionRelease_v9e0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_26,
&ett_lte_rrc_RRCConnectionRelease_v920_IEs,
&ett_lte_rrc_T_cellInfoList_r9,
&ett_lte_rrc_RRCConnectionRelease_v1020_IEs,
&ett_lte_rrc_T_nonCriticalExtension_27,
&ett_lte_rrc_RedirectedCarrierInfo,
&ett_lte_rrc_RedirectedCarrierInfo_v9e0,
&ett_lte_rrc_CarrierFreqListUTRA_TDD_r10,
&ett_lte_rrc_IdleModeMobilityControlInfo,
&ett_lte_rrc_IdleModeMobilityControlInfo_v9e0,
&ett_lte_rrc_SEQUENCE_SIZE_1_maxFreq_OF_FreqPriorityEUTRA_v9e0,
&ett_lte_rrc_FreqPriorityListEUTRA,
&ett_lte_rrc_FreqPriorityEUTRA,
&ett_lte_rrc_FreqPriorityEUTRA_v9e0,
&ett_lte_rrc_FreqsPriorityListGERAN,
&ett_lte_rrc_FreqsPriorityGERAN,
&ett_lte_rrc_FreqPriorityListUTRA_FDD,
&ett_lte_rrc_FreqPriorityUTRA_FDD,
&ett_lte_rrc_FreqPriorityListUTRA_TDD,
&ett_lte_rrc_FreqPriorityUTRA_TDD,
&ett_lte_rrc_BandClassPriorityListHRPD,
&ett_lte_rrc_BandClassPriorityHRPD,
&ett_lte_rrc_BandClassPriorityList1XRTT,
&ett_lte_rrc_BandClassPriority1XRTT,
&ett_lte_rrc_CellInfoListGERAN_r9,
&ett_lte_rrc_CellInfoGERAN_r9,
&ett_lte_rrc_CellInfoListUTRA_FDD_r9,
&ett_lte_rrc_CellInfoUTRA_FDD_r9,
&ett_lte_rrc_CellInfoListUTRA_TDD_r9,
&ett_lte_rrc_CellInfoUTRA_TDD_r9,
&ett_lte_rrc_CellInfoListUTRA_TDD_r10,
&ett_lte_rrc_CellInfoUTRA_TDD_r10,
&ett_lte_rrc_RRCConnectionRequest,
&ett_lte_rrc_T_criticalExtensions_24,
&ett_lte_rrc_T_criticalExtensionsFuture_24,
&ett_lte_rrc_RRCConnectionRequest_r8_IEs,
&ett_lte_rrc_InitialUE_Identity,
&ett_lte_rrc_RRCConnectionSetup,
&ett_lte_rrc_T_criticalExtensions_25,
&ett_lte_rrc_T_c1_24,
&ett_lte_rrc_T_criticalExtensionsFuture_25,
&ett_lte_rrc_RRCConnectionSetup_r8_IEs,
&ett_lte_rrc_RRCConnectionSetup_v8a0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_28,
&ett_lte_rrc_RRCConnectionSetupComplete,
&ett_lte_rrc_T_criticalExtensions_26,
&ett_lte_rrc_T_c1_25,
&ett_lte_rrc_T_criticalExtensionsFuture_26,
&ett_lte_rrc_RRCConnectionSetupComplete_r8_IEs,
&ett_lte_rrc_RRCConnectionSetupComplete_v8a0_IEs,
&ett_lte_rrc_RRCConnectionSetupComplete_v1020_IEs,
&ett_lte_rrc_RRCConnectionSetupComplete_v1130_IEs,
&ett_lte_rrc_T_nonCriticalExtension_29,
&ett_lte_rrc_RegisteredMME,
&ett_lte_rrc_SecurityModeCommand,
&ett_lte_rrc_T_criticalExtensions_27,
&ett_lte_rrc_T_c1_26,
&ett_lte_rrc_T_criticalExtensionsFuture_27,
&ett_lte_rrc_SecurityModeCommand_r8_IEs,
&ett_lte_rrc_SecurityModeCommand_v8a0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_30,
&ett_lte_rrc_SecurityConfigSMC,
&ett_lte_rrc_SecurityModeComplete,
&ett_lte_rrc_T_criticalExtensions_28,
&ett_lte_rrc_T_criticalExtensionsFuture_28,
&ett_lte_rrc_SecurityModeComplete_r8_IEs,
&ett_lte_rrc_SecurityModeComplete_v8a0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_31,
&ett_lte_rrc_SecurityModeFailure,
&ett_lte_rrc_T_criticalExtensions_29,
&ett_lte_rrc_T_criticalExtensionsFuture_29,
&ett_lte_rrc_SecurityModeFailure_r8_IEs,
&ett_lte_rrc_SecurityModeFailure_v8a0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_32,
&ett_lte_rrc_SystemInformation,
&ett_lte_rrc_T_criticalExtensions_30,
&ett_lte_rrc_T_criticalExtensionsFuture_30,
&ett_lte_rrc_SystemInformation_r8_IEs,
&ett_lte_rrc_T_sib_TypeAndInfo,
&ett_lte_rrc_T_sib_TypeAndInfo_item,
&ett_lte_rrc_SystemInformation_v8a0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_33,
&ett_lte_rrc_SystemInformationBlockType1,
&ett_lte_rrc_T_cellAccessRelatedInfo,
&ett_lte_rrc_T_cellSelectionInfo,
&ett_lte_rrc_SystemInformationBlockType1_v890_IEs,
&ett_lte_rrc_SystemInformationBlockType1_v8h0_IEs,
&ett_lte_rrc_SystemInformationBlockType1_v9e0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_34,
&ett_lte_rrc_SystemInformationBlockType1_v920_IEs,
&ett_lte_rrc_SystemInformationBlockType1_v1130_IEs,
&ett_lte_rrc_T_nonCriticalExtension_35,
&ett_lte_rrc_PLMN_IdentityList,
&ett_lte_rrc_PLMN_IdentityInfo,
&ett_lte_rrc_SchedulingInfoList,
&ett_lte_rrc_SchedulingInfo,
&ett_lte_rrc_SIB_MappingInfo,
&ett_lte_rrc_CellSelectionInfo_v920,
&ett_lte_rrc_CellSelectionInfo_v1130,
&ett_lte_rrc_UEAssistanceInformation_r11,
&ett_lte_rrc_T_criticalExtensions_31,
&ett_lte_rrc_T_c1_27,
&ett_lte_rrc_T_criticalExtensionsFuture_31,
&ett_lte_rrc_UEAssistanceInformation_r11_IEs,
&ett_lte_rrc_T_nonCriticalExtension_36,
&ett_lte_rrc_UECapabilityEnquiry,
&ett_lte_rrc_T_criticalExtensions_32,
&ett_lte_rrc_T_c1_28,
&ett_lte_rrc_T_criticalExtensionsFuture_32,
&ett_lte_rrc_UECapabilityEnquiry_r8_IEs,
&ett_lte_rrc_UECapabilityEnquiry_v8a0_IEs,
&ett_lte_rrc_UECapabilityEnquiry_v1180_IEs,
&ett_lte_rrc_SEQUENCE_SIZE_1_16_OF_FreqBandIndicator_r11,
&ett_lte_rrc_T_nonCriticalExtension_37,
&ett_lte_rrc_UE_CapabilityRequest,
&ett_lte_rrc_UECapabilityInformation,
&ett_lte_rrc_T_criticalExtensions_33,
&ett_lte_rrc_T_c1_29,
&ett_lte_rrc_T_criticalExtensionsFuture_33,
&ett_lte_rrc_UECapabilityInformation_r8_IEs,
&ett_lte_rrc_UECapabilityInformation_v8a0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_38,
&ett_lte_rrc_UEInformationRequest_r9,
&ett_lte_rrc_T_criticalExtensions_34,
&ett_lte_rrc_T_c1_30,
&ett_lte_rrc_T_criticalExtensionsFuture_34,
&ett_lte_rrc_UEInformationRequest_r9_IEs,
&ett_lte_rrc_UEInformationRequest_v930_IEs,
&ett_lte_rrc_UEInformationRequest_v1020_IEs,
&ett_lte_rrc_UEInformationRequest_v1130_IEs,
&ett_lte_rrc_T_nonCriticalExtension_39,
&ett_lte_rrc_UEInformationResponse_r9,
&ett_lte_rrc_T_criticalExtensions_35,
&ett_lte_rrc_T_c1_31,
&ett_lte_rrc_T_criticalExtensionsFuture_35,
&ett_lte_rrc_UEInformationResponse_r9_IEs,
&ett_lte_rrc_T_rach_Report_r9,
&ett_lte_rrc_UEInformationResponse_v9e0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_40,
&ett_lte_rrc_UEInformationResponse_v930_IEs,
&ett_lte_rrc_UEInformationResponse_v1020_IEs,
&ett_lte_rrc_UEInformationResponse_v1130_IEs,
&ett_lte_rrc_T_nonCriticalExtension_41,
&ett_lte_rrc_RLF_Report_r9,
&ett_lte_rrc_T_measResultLastServCell_r9,
&ett_lte_rrc_T_measResultNeighCells_r9,
&ett_lte_rrc_T_failedPCellId_r10,
&ett_lte_rrc_T_pci_arfcn_r10,
&ett_lte_rrc_T_failedPCellId_v1090,
&ett_lte_rrc_T_basicFields_r11,
&ett_lte_rrc_T_previousUTRA_CellId_r11,
&ett_lte_rrc_T_physCellId_r11,
&ett_lte_rrc_T_selectedUTRA_CellId_r11,
&ett_lte_rrc_T_physCellId_r11_01,
&ett_lte_rrc_RLF_Report_v9e0,
&ett_lte_rrc_MeasResultList2EUTRA_r9,
&ett_lte_rrc_MeasResultList2EUTRA_v9e0,
&ett_lte_rrc_MeasResult2EUTRA_r9,
&ett_lte_rrc_MeasResult2EUTRA_v9e0,
&ett_lte_rrc_MeasResultList2UTRA_r9,
&ett_lte_rrc_MeasResult2UTRA_r9,
&ett_lte_rrc_MeasResultList2CDMA2000_r9,
&ett_lte_rrc_MeasResult2CDMA2000_r9,
&ett_lte_rrc_LogMeasReport_r10,
&ett_lte_rrc_LogMeasInfoList_r10,
&ett_lte_rrc_LogMeasInfo_r10,
&ett_lte_rrc_T_measResultServCell_r10,
&ett_lte_rrc_T_measResultNeighCells_r10,
&ett_lte_rrc_MeasResultList2GERAN_r10,
&ett_lte_rrc_ConnEstFailReport_r11,
&ett_lte_rrc_T_measResultFailedCell_r11,
&ett_lte_rrc_T_measResultNeighCells_r11,
&ett_lte_rrc_ULHandoverPreparationTransfer,
&ett_lte_rrc_T_criticalExtensions_36,
&ett_lte_rrc_T_c1_32,
&ett_lte_rrc_T_criticalExtensionsFuture_36,
&ett_lte_rrc_ULHandoverPreparationTransfer_r8_IEs,
&ett_lte_rrc_ULHandoverPreparationTransfer_v8a0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_42,
&ett_lte_rrc_ULInformationTransfer,
&ett_lte_rrc_T_criticalExtensions_37,
&ett_lte_rrc_T_c1_33,
&ett_lte_rrc_T_criticalExtensionsFuture_37,
&ett_lte_rrc_ULInformationTransfer_r8_IEs,
&ett_lte_rrc_T_dedicatedInfoType_01,
&ett_lte_rrc_ULInformationTransfer_v8a0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_43,
&ett_lte_rrc_SystemInformationBlockType2,
&ett_lte_rrc_T_ac_BarringInfo,
&ett_lte_rrc_T_freqInfo,
&ett_lte_rrc_SystemInformationBlockType2_v8h0_IEs,
&ett_lte_rrc_SEQUENCE_SIZE_1_maxMultiBands_OF_AdditionalSpectrumEmission,
&ett_lte_rrc_SystemInformationBlockType2_v9e0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_44,
&ett_lte_rrc_AC_BarringConfig,
&ett_lte_rrc_MBSFN_SubframeConfigList,
&ett_lte_rrc_SystemInformationBlockType3,
&ett_lte_rrc_T_cellReselectionInfoCommon,
&ett_lte_rrc_T_speedStateReselectionPars,
&ett_lte_rrc_T_q_HystSF,
&ett_lte_rrc_T_cellReselectionServingFreqInfo,
&ett_lte_rrc_T_intraFreqCellReselectionInfo,
&ett_lte_rrc_T_s_IntraSearch_v920,
&ett_lte_rrc_T_s_NonIntraSearch_v920,
&ett_lte_rrc_SystemInformationBlockType4,
&ett_lte_rrc_IntraFreqNeighCellList,
&ett_lte_rrc_IntraFreqNeighCellInfo,
&ett_lte_rrc_IntraFreqBlackCellList,
&ett_lte_rrc_SystemInformationBlockType5,
&ett_lte_rrc_SystemInformationBlockType5_v8h0_IEs,
&ett_lte_rrc_SEQUENCE_SIZE_1_maxFreq_OF_InterFreqCarrierFreqInfo_v8h0,
&ett_lte_rrc_SystemInformationBlockType5_v9e0_IEs,
&ett_lte_rrc_SEQUENCE_SIZE_1_maxFreq_OF_InterFreqCarrierFreqInfo_v9e0,
&ett_lte_rrc_T_nonCriticalExtension_45,
&ett_lte_rrc_InterFreqCarrierFreqList,
&ett_lte_rrc_InterFreqCarrierFreqInfo,
&ett_lte_rrc_T_threshX_Q_r9,
&ett_lte_rrc_InterFreqCarrierFreqInfo_v8h0,
&ett_lte_rrc_InterFreqCarrierFreqInfo_v9e0,
&ett_lte_rrc_InterFreqNeighCellList,
&ett_lte_rrc_InterFreqNeighCellInfo,
&ett_lte_rrc_InterFreqBlackCellList,
&ett_lte_rrc_SystemInformationBlockType6,
&ett_lte_rrc_SystemInformationBlockType6_v8h0_IEs,
&ett_lte_rrc_SEQUENCE_SIZE_1_maxUTRA_FDD_Carrier_OF_CarrierFreqInfoUTRA_FDD_v8h0,
&ett_lte_rrc_T_nonCriticalExtension_46,
&ett_lte_rrc_CarrierFreqListUTRA_FDD,
&ett_lte_rrc_CarrierFreqUTRA_FDD,
&ett_lte_rrc_T_threshX_Q_r9_01,
&ett_lte_rrc_CarrierFreqInfoUTRA_FDD_v8h0,
&ett_lte_rrc_SEQUENCE_SIZE_1_maxMultiBands_OF_FreqBandIndicator_UTRA_FDD,
&ett_lte_rrc_CarrierFreqListUTRA_TDD,
&ett_lte_rrc_CarrierFreqUTRA_TDD,
&ett_lte_rrc_SystemInformationBlockType7,
&ett_lte_rrc_CarrierFreqsInfoListGERAN,
&ett_lte_rrc_CarrierFreqsInfoGERAN,
&ett_lte_rrc_T_commonInfo,
&ett_lte_rrc_SystemInformationBlockType8,
&ett_lte_rrc_T_parametersHRPD,
&ett_lte_rrc_T_parameters1XRTT,
&ett_lte_rrc_CellReselectionParametersCDMA2000,
&ett_lte_rrc_CellReselectionParametersCDMA2000_r11,
&ett_lte_rrc_SEQUENCE_SIZE_1_16_OF_NeighCellCDMA2000_r11,
&ett_lte_rrc_CellReselectionParametersCDMA2000_v920,
&ett_lte_rrc_NeighCellListCDMA2000,
&ett_lte_rrc_NeighCellCDMA2000,
&ett_lte_rrc_NeighCellCDMA2000_r11,
&ett_lte_rrc_SEQUENCE_SIZE_1_16_OF_NeighCellsPerBandclassCDMA2000_r11,
&ett_lte_rrc_NeighCellsPerBandclassListCDMA2000,
&ett_lte_rrc_NeighCellsPerBandclassCDMA2000,
&ett_lte_rrc_NeighCellsPerBandclassCDMA2000_r11,
&ett_lte_rrc_SEQUENCE_SIZE_1_40_OF_PhysCellIdCDMA2000,
&ett_lte_rrc_NeighCellListCDMA2000_v920,
&ett_lte_rrc_NeighCellCDMA2000_v920,
&ett_lte_rrc_NeighCellsPerBandclassListCDMA2000_v920,
&ett_lte_rrc_NeighCellsPerBandclassCDMA2000_v920,
&ett_lte_rrc_PhysCellIdListCDMA2000,
&ett_lte_rrc_PhysCellIdListCDMA2000_v920,
&ett_lte_rrc_BandClassListCDMA2000,
&ett_lte_rrc_BandClassInfoCDMA2000,
&ett_lte_rrc_AC_BarringConfig1XRTT_r9,
&ett_lte_rrc_SIB8_PerPLMN_List_r11,
&ett_lte_rrc_SIB8_PerPLMN_r11,
&ett_lte_rrc_T_parametersCDMA2000_r11,
&ett_lte_rrc_ParametersCDMA2000_r11,
&ett_lte_rrc_T_systemTimeInfo_r11,
&ett_lte_rrc_T_parametersHRPD_r11,
&ett_lte_rrc_T_parameters1XRTT_r11,
&ett_lte_rrc_SystemInformationBlockType9,
&ett_lte_rrc_SystemInformationBlockType10,
&ett_lte_rrc_SystemInformationBlockType11,
&ett_lte_rrc_SystemInformationBlockType12_r9,
&ett_lte_rrc_SystemInformationBlockType13_r9,
&ett_lte_rrc_SystemInformationBlockType14_r11,
&ett_lte_rrc_T_eab_Param_r11,
&ett_lte_rrc_SEQUENCE_SIZE_1_maxPLMN_r11_OF_EAB_ConfigPLMN_r11,
&ett_lte_rrc_EAB_ConfigPLMN_r11,
&ett_lte_rrc_EAB_Config_r11,
&ett_lte_rrc_SystemInformationBlockType15_r11,
&ett_lte_rrc_MBMS_SAI_List_r11,
&ett_lte_rrc_MBMS_SAI_InterFreqList_r11,
&ett_lte_rrc_MBMS_SAI_InterFreqList_v1140,
&ett_lte_rrc_MBMS_SAI_InterFreq_r11,
&ett_lte_rrc_MBMS_SAI_InterFreq_v1140,
&ett_lte_rrc_SystemInformationBlockType16_r11,
&ett_lte_rrc_T_timeInfo_r11,
&ett_lte_rrc_AntennaInfoCommon,
&ett_lte_rrc_AntennaInfoDedicated,
&ett_lte_rrc_T_codebookSubsetRestriction,
&ett_lte_rrc_T_ue_TransmitAntennaSelection,
&ett_lte_rrc_AntennaInfoDedicated_v920,
&ett_lte_rrc_T_codebookSubsetRestriction_v920,
&ett_lte_rrc_AntennaInfoDedicated_r10,
&ett_lte_rrc_T_ue_TransmitAntennaSelection_01,
&ett_lte_rrc_AntennaInfoUL_r10,
&ett_lte_rrc_CQI_ReportConfig,
&ett_lte_rrc_CQI_ReportConfig_v920,
&ett_lte_rrc_CQI_ReportConfig_r10,
&ett_lte_rrc_T_csi_SubframePatternConfig_r10,
&ett_lte_rrc_T_setup_02,
&ett_lte_rrc_CQI_ReportConfig_v1130,
&ett_lte_rrc_CQI_ReportConfigSCell_r10,
&ett_lte_rrc_CQI_ReportPeriodic,
&ett_lte_rrc_T_setup_03,
&ett_lte_rrc_T_cqi_FormatIndicatorPeriodic,
&ett_lte_rrc_T_subbandCQI,
&ett_lte_rrc_CQI_ReportPeriodic_r10,
&ett_lte_rrc_T_setup_04,
&ett_lte_rrc_T_cqi_FormatIndicatorPeriodic_r10,
&ett_lte_rrc_T_widebandCQI_r10,
&ett_lte_rrc_T_subbandCQI_r10,
&ett_lte_rrc_T_csi_ConfigIndex_r10,
&ett_lte_rrc_T_setup_05,
&ett_lte_rrc_CQI_ReportPeriodic_v1130,
&ett_lte_rrc_CQI_ReportPeriodicProcExtToAddModList_r11,
&ett_lte_rrc_CQI_ReportPeriodicProcExtToReleaseList_r11,
&ett_lte_rrc_CQI_ReportPeriodicProcExt_r11,
&ett_lte_rrc_T_cqi_FormatIndicatorPeriodic_r11,
&ett_lte_rrc_T_widebandCQI_r11,
&ett_lte_rrc_T_subbandCQI_r11,
&ett_lte_rrc_T_csi_ConfigIndex_r11,
&ett_lte_rrc_T_setup_06,
&ett_lte_rrc_CQI_ReportAperiodic_r10,
&ett_lte_rrc_T_setup_07,
&ett_lte_rrc_T_aperiodicCSI_Trigger_r10,
&ett_lte_rrc_CQI_ReportAperiodicProc_r11,
&ett_lte_rrc_CQI_ReportBoth_r11,
&ett_lte_rrc_CSI_IM_ConfigToAddModList_r11,
&ett_lte_rrc_CSI_IM_ConfigToReleaseList_r11,
&ett_lte_rrc_CSI_ProcessToAddModList_r11,
&ett_lte_rrc_CSI_ProcessToReleaseList_r11,
&ett_lte_rrc_CQI_ReportBothProc_r11,
&ett_lte_rrc_CrossCarrierSchedulingConfig_r10,
&ett_lte_rrc_T_schedulingCellInfo_r10,
&ett_lte_rrc_T_own_r10,
&ett_lte_rrc_T_other_r10,
&ett_lte_rrc_CSI_IM_Config_r11,
&ett_lte_rrc_CSI_Process_r11,
&ett_lte_rrc_SEQUENCE_SIZE_1_2_OF_P_C_AndCBSR_r11,
&ett_lte_rrc_P_C_AndCBSR_r11,
&ett_lte_rrc_CSI_RS_Config_r10,
&ett_lte_rrc_T_csi_RS_r10,
&ett_lte_rrc_T_setup_08,
&ett_lte_rrc_T_zeroTxPowerCSI_RS_r10,
&ett_lte_rrc_T_setup_09,
&ett_lte_rrc_CSI_RS_ConfigNZP_r11,
&ett_lte_rrc_T_qcl_CRS_Info_r11,
&ett_lte_rrc_T_mbsfn_SubframeConfigList_r11,
&ett_lte_rrc_T_setup_10,
&ett_lte_rrc_CSI_RS_ConfigZP_r11,
&ett_lte_rrc_DMRS_Config_r11,
&ett_lte_rrc_T_setup_11,
&ett_lte_rrc_EPDCCH_Config_r11,
&ett_lte_rrc_T_config_r11,
&ett_lte_rrc_T_setup_12,
&ett_lte_rrc_T_subframePatternConfig_r11,
&ett_lte_rrc_T_setup_13,
&ett_lte_rrc_EPDCCH_SetConfigToAddModList_r11,
&ett_lte_rrc_EPDCCH_SetConfigToReleaseList_r11,
&ett_lte_rrc_EPDCCH_SetConfig_r11,
&ett_lte_rrc_T_resourceBlockAssignment_r11,
&ett_lte_rrc_LogicalChannelConfig,
&ett_lte_rrc_T_ul_SpecificParameters,
&ett_lte_rrc_MAC_MainConfig,
&ett_lte_rrc_T_ul_SCH_Config,
&ett_lte_rrc_T_phr_Config,
&ett_lte_rrc_T_setup_14,
&ett_lte_rrc_T_mac_MainConfig_v1020,
&ett_lte_rrc_MAC_MainConfigSCell_r11,
&ett_lte_rrc_DRX_Config,
&ett_lte_rrc_T_setup_15,
&ett_lte_rrc_T_longDRX_CycleStartOffset,
&ett_lte_rrc_T_shortDRX,
&ett_lte_rrc_DRX_Config_v1130,
&ett_lte_rrc_T_longDRX_CycleStartOffset_v1130,
&ett_lte_rrc_STAG_ToReleaseList_r11,
&ett_lte_rrc_STAG_ToAddModList_r11,
&ett_lte_rrc_STAG_ToAddMod_r11,
&ett_lte_rrc_PDCP_Config,
&ett_lte_rrc_T_rlc_AM,
&ett_lte_rrc_T_rlc_UM,
&ett_lte_rrc_T_headerCompression,
&ett_lte_rrc_T_rohc,
&ett_lte_rrc_T_profiles,
&ett_lte_rrc_PDSCH_ConfigCommon,
&ett_lte_rrc_PDSCH_ConfigDedicated,
&ett_lte_rrc_PDSCH_ConfigDedicated_v1130,
&ett_lte_rrc_RE_MappingQCLConfigToAddModList_r11,
&ett_lte_rrc_RE_MappingQCLConfigToReleaseList_r11,
&ett_lte_rrc_PDSCH_RE_MappingQCL_Config_r11,
&ett_lte_rrc_T_optionalSetOfFields_r11,
&ett_lte_rrc_T_mbsfn_SubframeConfigList_r11_01,
&ett_lte_rrc_T_setup_16,
&ett_lte_rrc_PHICH_Config,
&ett_lte_rrc_PhysicalConfigDedicated,
&ett_lte_rrc_T_antennaInfo,
&ett_lte_rrc_T_antennaInfo_r10,
&ett_lte_rrc_T_additionalSpectrumEmissionCA_r10,
&ett_lte_rrc_T_setup_17,
&ett_lte_rrc_PhysicalConfigDedicatedSCell_r10,
&ett_lte_rrc_T_nonUL_Configuration_r10,
&ett_lte_rrc_T_ul_Configuration_r10,
&ett_lte_rrc_CSI_RS_ConfigNZPToAddModList_r11,
&ett_lte_rrc_CSI_RS_ConfigNZPToReleaseList_r11,
&ett_lte_rrc_CSI_RS_ConfigZPToAddModList_r11,
&ett_lte_rrc_CSI_RS_ConfigZPToReleaseList_r11,
&ett_lte_rrc_PRACH_ConfigSIB,
&ett_lte_rrc_PRACH_Config,
&ett_lte_rrc_PRACH_ConfigSCell_r10,
&ett_lte_rrc_PRACH_ConfigInfo,
&ett_lte_rrc_PUCCH_ConfigCommon,
&ett_lte_rrc_PUCCH_ConfigDedicated,
&ett_lte_rrc_T_ackNackRepetition,
&ett_lte_rrc_T_setup_18,
&ett_lte_rrc_PUCCH_ConfigDedicated_v1020,
&ett_lte_rrc_T_pucch_Format_r10,
&ett_lte_rrc_T_format3_r10,
&ett_lte_rrc_T_n3PUCCH_AN_List_r10,
&ett_lte_rrc_T_twoAntennaPortActivatedPUCCH_Format3_r10,
&ett_lte_rrc_T_setup_19,
&ett_lte_rrc_T_n3PUCCH_AN_ListP1_r10,
&ett_lte_rrc_T_channelSelection_r10,
&ett_lte_rrc_T_n1PUCCH_AN_CS_r10,
&ett_lte_rrc_T_setup_20,
&ett_lte_rrc_SEQUENCE_SIZE_1_2_OF_N1PUCCH_AN_CS_r10,
&ett_lte_rrc_PUCCH_ConfigDedicated_v1130,
&ett_lte_rrc_T_n1PUCCH_AN_CS_v1130,
&ett_lte_rrc_T_setup_21,
&ett_lte_rrc_T_n1PUCCH_AN_CS_ListP1_r11,
&ett_lte_rrc_T_nPUCCH_Param_r11,
&ett_lte_rrc_T_setup_22,
&ett_lte_rrc_N1PUCCH_AN_CS_r10,
&ett_lte_rrc_PUSCH_ConfigCommon,
&ett_lte_rrc_T_pusch_ConfigBasic,
&ett_lte_rrc_PUSCH_ConfigDedicated,
&ett_lte_rrc_PUSCH_ConfigDedicated_v1020,
&ett_lte_rrc_T_betaOffsetMC_r10,
&ett_lte_rrc_PUSCH_ConfigDedicated_v1130,
&ett_lte_rrc_T_pusch_DMRS_r11,
&ett_lte_rrc_T_setup_23,
&ett_lte_rrc_PUSCH_ConfigDedicatedSCell_r10,
&ett_lte_rrc_UL_ReferenceSignalsPUSCH,
&ett_lte_rrc_RACH_ConfigCommon,
&ett_lte_rrc_T_preambleInfo,
&ett_lte_rrc_T_preamblesGroupAConfig,
&ett_lte_rrc_T_ra_SupervisionInfo,
&ett_lte_rrc_RACH_ConfigCommonSCell_r11,
&ett_lte_rrc_T_ra_SupervisionInfo_r11,
&ett_lte_rrc_PowerRampingParameters,
&ett_lte_rrc_RACH_ConfigDedicated,
&ett_lte_rrc_RadioResourceConfigCommonSIB,
&ett_lte_rrc_RadioResourceConfigCommon,
&ett_lte_rrc_RadioResourceConfigCommonSCell_r10,
&ett_lte_rrc_T_nonUL_Configuration_r10_01,
&ett_lte_rrc_T_ul_Configuration_r10_01,
&ett_lte_rrc_T_ul_FreqInfo_r10,
&ett_lte_rrc_BCCH_Config,
&ett_lte_rrc_PCCH_Config,
&ett_lte_rrc_RadioResourceConfigDedicated,
&ett_lte_rrc_T_mac_MainConfig,
&ett_lte_rrc_RadioResourceConfigDedicatedSCell_r10,
&ett_lte_rrc_SRB_ToAddModList,
&ett_lte_rrc_SRB_ToAddMod,
&ett_lte_rrc_T_rlc_Config,
&ett_lte_rrc_T_logicalChannelConfig,
&ett_lte_rrc_DRB_ToAddModList,
&ett_lte_rrc_DRB_ToAddMod,
&ett_lte_rrc_DRB_ToReleaseList,
&ett_lte_rrc_MeasSubframePatternPCell_r10,
&ett_lte_rrc_NeighCellsCRS_Info_r11,
&ett_lte_rrc_CRS_AssistanceInfoList_r11,
&ett_lte_rrc_CRS_AssistanceInfo_r11,
&ett_lte_rrc_RLC_Config,
&ett_lte_rrc_T_am,
&ett_lte_rrc_T_um_Bi_Directional,
&ett_lte_rrc_T_um_Uni_Directional_UL,
&ett_lte_rrc_T_um_Uni_Directional_DL,
&ett_lte_rrc_UL_AM_RLC,
&ett_lte_rrc_DL_AM_RLC,
&ett_lte_rrc_UL_UM_RLC,
&ett_lte_rrc_DL_UM_RLC,
&ett_lte_rrc_RLF_TimersAndConstants_r9,
&ett_lte_rrc_T_setup_24,
&ett_lte_rrc_RN_SubframeConfig_r10,
&ett_lte_rrc_T_subframeConfigPattern_r10,
&ett_lte_rrc_T_rpdcch_Config_r10,
&ett_lte_rrc_T_resourceBlockAssignment_r10,
&ett_lte_rrc_T_type01_r10,
&ett_lte_rrc_T_type2_r10,
&ett_lte_rrc_T_demodulationRS_r10,
&ett_lte_rrc_T_pucch_Config_r10,
&ett_lte_rrc_T_tdd,
&ett_lte_rrc_T_channelSelectionMultiplexingBundling,
&ett_lte_rrc_T_n1PUCCH_AN_List_r10,
&ett_lte_rrc_T_fallbackForFormat3,
&ett_lte_rrc_T_fdd,
&ett_lte_rrc_SchedulingRequestConfig,
&ett_lte_rrc_T_setup_25,
&ett_lte_rrc_SchedulingRequestConfig_v1020,
&ett_lte_rrc_SoundingRS_UL_ConfigCommon,
&ett_lte_rrc_T_setup_26,
&ett_lte_rrc_SoundingRS_UL_ConfigDedicated,
&ett_lte_rrc_T_setup_27,
&ett_lte_rrc_SoundingRS_UL_ConfigDedicated_v1020,
&ett_lte_rrc_SoundingRS_UL_ConfigDedicatedAperiodic_r10,
&ett_lte_rrc_T_setup_28,
&ett_lte_rrc_SEQUENCE_SIZE_1_3_OF_SRS_ConfigAp_r10,
&ett_lte_rrc_T_srs_ActivateAp_r10,
&ett_lte_rrc_T_setup_29,
&ett_lte_rrc_SRS_ConfigAp_r10,
&ett_lte_rrc_SPS_Config,
&ett_lte_rrc_SPS_ConfigDL,
&ett_lte_rrc_T_setup_30,
&ett_lte_rrc_T_twoAntennaPortActivated_r10,
&ett_lte_rrc_T_setup_31,
&ett_lte_rrc_SPS_ConfigUL,
&ett_lte_rrc_T_setup_32,
&ett_lte_rrc_T_p0_Persistent,
&ett_lte_rrc_N1PUCCH_AN_PersistentList,
&ett_lte_rrc_TDD_Config,
&ett_lte_rrc_TDD_Config_v1130,
&ett_lte_rrc_TPC_PDCCH_Config,
&ett_lte_rrc_T_setup_33,
&ett_lte_rrc_TPC_Index,
&ett_lte_rrc_UplinkPowerControlCommon,
&ett_lte_rrc_UplinkPowerControlCommon_v1020,
&ett_lte_rrc_UplinkPowerControlCommonSCell_r10,
&ett_lte_rrc_UplinkPowerControlCommonSCell_v1130,
&ett_lte_rrc_UplinkPowerControlDedicated,
&ett_lte_rrc_UplinkPowerControlDedicated_v1020,
&ett_lte_rrc_UplinkPowerControlDedicated_v1130,
&ett_lte_rrc_UplinkPowerControlDedicatedSCell_r10,
&ett_lte_rrc_DeltaFList_PUCCH,
&ett_lte_rrc_DeltaTxD_OffsetListPUCCH_r10,
&ett_lte_rrc_DeltaTxD_OffsetListPUCCH_v1130,
&ett_lte_rrc_SecurityAlgorithmConfig,
&ett_lte_rrc_CarrierFreqCDMA2000,
&ett_lte_rrc_CarrierFreqGERAN,
&ett_lte_rrc_CarrierFreqsGERAN,
&ett_lte_rrc_T_followingARFCNs,
&ett_lte_rrc_T_equallySpacedARFCNs,
&ett_lte_rrc_ExplicitListOfARFCNs,
&ett_lte_rrc_CarrierFreqListMBMS_r11,
&ett_lte_rrc_CellIndexList,
&ett_lte_rrc_CSFB_RegistrationParam1XRTT,
&ett_lte_rrc_CSFB_RegistrationParam1XRTT_v920,
&ett_lte_rrc_CellGlobalIdEUTRA,
&ett_lte_rrc_CellGlobalIdUTRA,
&ett_lte_rrc_CellGlobalIdGERAN,
&ett_lte_rrc_CellGlobalIdCDMA2000,
&ett_lte_rrc_MobilityControlInfo,
&ett_lte_rrc_CarrierBandwidthEUTRA,
&ett_lte_rrc_CarrierFreqEUTRA,
&ett_lte_rrc_CarrierFreqEUTRA_v9e0,
&ett_lte_rrc_MobilityStateParameters,
&ett_lte_rrc_MultiBandInfoList,
&ett_lte_rrc_MultiBandInfoList_v9e0,
&ett_lte_rrc_MultiBandInfoList_r11,
&ett_lte_rrc_MultiBandInfo_v9e0,
&ett_lte_rrc_PhysCellIdRange,
&ett_lte_rrc_PhysCellIdRangeUTRA_FDDList_r9,
&ett_lte_rrc_PhysCellIdRangeUTRA_FDD_r9,
&ett_lte_rrc_PhysCellIdGERAN,
&ett_lte_rrc_PLMN_Identity,
&ett_lte_rrc_MCC,
&ett_lte_rrc_MNC,
&ett_lte_rrc_PLMN_IdentityList3_r11,
&ett_lte_rrc_PreRegistrationInfoHRPD,
&ett_lte_rrc_SecondaryPreRegistrationZoneIdListHRPD,
&ett_lte_rrc_SpeedStateScaleFactors,
&ett_lte_rrc_SystemInfoListGERAN,
&ett_lte_rrc_SystemTimeInfoCDMA2000,
&ett_lte_rrc_T_cdma_SystemTime,
&ett_lte_rrc_LocationInfo_r10,
&ett_lte_rrc_T_locationCoordinates_r10,
&ett_lte_rrc_MeasConfig,
&ett_lte_rrc_T_speedStatePars,
&ett_lte_rrc_T_setup_34,
&ett_lte_rrc_MeasIdToRemoveList,
&ett_lte_rrc_MeasObjectToRemoveList,
&ett_lte_rrc_ReportConfigToRemoveList,
&ett_lte_rrc_MeasGapConfig,
&ett_lte_rrc_T_setup_35,
&ett_lte_rrc_T_gapOffset,
&ett_lte_rrc_MeasIdToAddModList,
&ett_lte_rrc_MeasIdToAddMod,
&ett_lte_rrc_MeasObjectCDMA2000,
&ett_lte_rrc_CellsToAddModListCDMA2000,
&ett_lte_rrc_CellsToAddModCDMA2000,
&ett_lte_rrc_MeasObjectEUTRA,
&ett_lte_rrc_MeasObjectEUTRA_v9e0,
&ett_lte_rrc_CellsToAddModList,
&ett_lte_rrc_CellsToAddMod,
&ett_lte_rrc_BlackCellsToAddModList,
&ett_lte_rrc_BlackCellsToAddMod,
&ett_lte_rrc_MeasSubframePatternConfigNeigh_r10,
&ett_lte_rrc_T_setup_36,
&ett_lte_rrc_MeasSubframeCellList_r10,
&ett_lte_rrc_MeasObjectGERAN,
&ett_lte_rrc_MeasObjectToAddModList,
&ett_lte_rrc_MeasObjectToAddModList_v9e0,
&ett_lte_rrc_MeasObjectToAddMod,
&ett_lte_rrc_T_measObject,
&ett_lte_rrc_MeasObjectToAddMod_v9e0,
&ett_lte_rrc_MeasObjectUTRA,
&ett_lte_rrc_T_cellsToAddModList,
&ett_lte_rrc_T_cellForWhichToReportCGI,
&ett_lte_rrc_CellsToAddModListUTRA_FDD,
&ett_lte_rrc_CellsToAddModUTRA_FDD,
&ett_lte_rrc_CellsToAddModListUTRA_TDD,
&ett_lte_rrc_CellsToAddModUTRA_TDD,
&ett_lte_rrc_CSG_AllowedReportingCells_r9,
&ett_lte_rrc_MeasResults,
&ett_lte_rrc_T_measResultPCell,
&ett_lte_rrc_T_measResultNeighCells,
&ett_lte_rrc_MeasResultListEUTRA,
&ett_lte_rrc_MeasResultEUTRA,
&ett_lte_rrc_T_cgi_Info,
&ett_lte_rrc_T_measResult,
&ett_lte_rrc_MeasResultServFreqList_r10,
&ett_lte_rrc_MeasResultServFreq_r10,
&ett_lte_rrc_T_measResultSCell_r10,
&ett_lte_rrc_T_measResultBestNeighCell_r10,
&ett_lte_rrc_MeasResultListUTRA,
&ett_lte_rrc_MeasResultUTRA,
&ett_lte_rrc_T_physCellId,
&ett_lte_rrc_T_cgi_Info_01,
&ett_lte_rrc_T_measResult_01,
&ett_lte_rrc_MeasResultListGERAN,
&ett_lte_rrc_MeasResultGERAN,
&ett_lte_rrc_T_cgi_Info_02,
&ett_lte_rrc_T_measResult_02,
&ett_lte_rrc_MeasResultsCDMA2000,
&ett_lte_rrc_MeasResultListCDMA2000,
&ett_lte_rrc_MeasResultCDMA2000,
&ett_lte_rrc_T_measResult_03,
&ett_lte_rrc_MeasResultForECID_r9,
&ett_lte_rrc_PLMN_IdentityList2,
&ett_lte_rrc_AdditionalSI_Info_r9,
&ett_lte_rrc_QuantityConfig,
&ett_lte_rrc_QuantityConfigEUTRA,
&ett_lte_rrc_QuantityConfigUTRA,
&ett_lte_rrc_QuantityConfigUTRA_v1020,
&ett_lte_rrc_QuantityConfigGERAN,
&ett_lte_rrc_QuantityConfigCDMA2000,
&ett_lte_rrc_ReportConfigEUTRA,
&ett_lte_rrc_T_triggerType,
&ett_lte_rrc_T_event,
&ett_lte_rrc_T_eventId,
&ett_lte_rrc_T_eventA1,
&ett_lte_rrc_T_eventA2,
&ett_lte_rrc_T_eventA3,
&ett_lte_rrc_T_eventA4,
&ett_lte_rrc_T_eventA5,
&ett_lte_rrc_T_eventA6_r10,
&ett_lte_rrc_T_periodical,
&ett_lte_rrc_ThresholdEUTRA,
&ett_lte_rrc_ReportConfigInterRAT,
&ett_lte_rrc_T_triggerType_01,
&ett_lte_rrc_T_event_01,
&ett_lte_rrc_T_eventId_01,
&ett_lte_rrc_T_eventB1,
&ett_lte_rrc_T_b1_Threshold,
&ett_lte_rrc_T_eventB2,
&ett_lte_rrc_T_b2_Threshold2,
&ett_lte_rrc_T_periodical_01,
&ett_lte_rrc_ThresholdUTRA,
&ett_lte_rrc_ReportConfigToAddModList,
&ett_lte_rrc_ReportConfigToAddMod,
&ett_lte_rrc_T_reportConfig,
&ett_lte_rrc_AreaConfiguration_r10,
&ett_lte_rrc_AreaConfiguration_v1130,
&ett_lte_rrc_CellGlobalIdList_r10,
&ett_lte_rrc_TrackingAreaCodeList_r10,
&ett_lte_rrc_TrackingAreaCodeList_v1130,
&ett_lte_rrc_SEQUENCE_SIZE_1_8_OF_PLMN_Identity,
&ett_lte_rrc_MeasSubframePattern_r10,
&ett_lte_rrc_T_subframePatternTDD_r10,
&ett_lte_rrc_OtherConfig_r9,
&ett_lte_rrc_IDC_Config_r11,
&ett_lte_rrc_T_autonomousDenialParameters_r11,
&ett_lte_rrc_ObtainLocationConfig_r11,
&ett_lte_rrc_PowerPrefIndicationConfig_r11,
&ett_lte_rrc_T_setup_37,
&ett_lte_rrc_ReportProximityConfig_r9,
&ett_lte_rrc_S_TMSI,
&ett_lte_rrc_TraceReference_r10,
&ett_lte_rrc_UE_CapabilityRAT_ContainerList,
&ett_lte_rrc_UE_CapabilityRAT_Container,
&ett_lte_rrc_UE_EUTRA_Capability,
&ett_lte_rrc_T_interRAT_Parameters,
&ett_lte_rrc_UE_EUTRA_Capability_v9a0_IEs,
&ett_lte_rrc_UE_EUTRA_Capability_v9c0_IEs,
&ett_lte_rrc_UE_EUTRA_Capability_v9d0_IEs,
&ett_lte_rrc_UE_EUTRA_Capability_v9e0_IEs,
&ett_lte_rrc_UE_EUTRA_Capability_v9h0_IEs,
&ett_lte_rrc_UE_EUTRA_Capability_v10c0_IEs,
&ett_lte_rrc_T_nonCriticalExtension_47,
&ett_lte_rrc_UE_EUTRA_Capability_v920_IEs,
&ett_lte_rrc_UE_EUTRA_Capability_v940_IEs,
&ett_lte_rrc_UE_EUTRA_Capability_v1020_IEs,
&ett_lte_rrc_UE_EUTRA_Capability_v1060_IEs,
&ett_lte_rrc_UE_EUTRA_Capability_v1090_IEs,
&ett_lte_rrc_UE_EUTRA_Capability_v1130_IEs,
&ett_lte_rrc_UE_EUTRA_Capability_v1170_IEs,
&ett_lte_rrc_UE_EUTRA_Capability_v1180_IEs,
&ett_lte_rrc_T_nonCriticalExtension_48,
&ett_lte_rrc_UE_EUTRA_CapabilityAddXDD_Mode_r9,
&ett_lte_rrc_UE_EUTRA_CapabilityAddXDD_Mode_v1060,
&ett_lte_rrc_UE_EUTRA_CapabilityAddXDD_Mode_v1130,
&ett_lte_rrc_UE_EUTRA_CapabilityAddXDD_Mode_v1180,
&ett_lte_rrc_PDCP_Parameters,
&ett_lte_rrc_T_supportedROHC_Profiles,
&ett_lte_rrc_PDCP_Parameters_v1130,
&ett_lte_rrc_PhyLayerParameters,
&ett_lte_rrc_PhyLayerParameters_v920,
&ett_lte_rrc_PhyLayerParameters_v9d0,
&ett_lte_rrc_PhyLayerParameters_v1020,
&ett_lte_rrc_PhyLayerParameters_v1130,
&ett_lte_rrc_PhyLayerParameters_v1170,
&ett_lte_rrc_NonContiguousUL_RA_WithinCC_List_r10,
&ett_lte_rrc_NonContiguousUL_RA_WithinCC_r10,
&ett_lte_rrc_RF_Parameters,
&ett_lte_rrc_RF_Parameters_v9e0,
&ett_lte_rrc_RF_Parameters_v1020,
&ett_lte_rrc_RF_Parameters_v1060,
&ett_lte_rrc_RF_Parameters_v1090,
&ett_lte_rrc_RF_Parameters_v1130,
&ett_lte_rrc_RF_Parameters_v1180,
&ett_lte_rrc_SEQUENCE_SIZE_1_maxBands_OF_FreqBandIndicator_r11,
&ett_lte_rrc_SupportedBandCombination_r10,
&ett_lte_rrc_SupportedBandCombinationExt_r10,
&ett_lte_rrc_SupportedBandCombination_v1090,
&ett_lte_rrc_SupportedBandCombination_v1130,
&ett_lte_rrc_SupportedBandCombinationAdd_r11,
&ett_lte_rrc_BandCombinationParameters_r10,
&ett_lte_rrc_BandCombinationParametersExt_r10,
&ett_lte_rrc_BandCombinationParameters_v1090,
&ett_lte_rrc_BandCombinationParameters_v1130,
&ett_lte_rrc_SEQUENCE_SIZE_1_maxSimultaneousBands_r10_OF_BandParameters_v1130,
&ett_lte_rrc_BandCombinationParameters_r11,
&ett_lte_rrc_SEQUENCE_SIZE_1_maxSimultaneousBands_r10_OF_BandParameters_r11,
&ett_lte_rrc_BandParameters_r10,
&ett_lte_rrc_BandParameters_v1090,
&ett_lte_rrc_BandParameters_v1130,
&ett_lte_rrc_BandParameters_r11,
&ett_lte_rrc_BandParametersUL_r10,
&ett_lte_rrc_CA_MIMO_ParametersUL_r10,
&ett_lte_rrc_BandParametersDL_r10,
&ett_lte_rrc_CA_MIMO_ParametersDL_r10,
&ett_lte_rrc_SupportedBandListEUTRA,
&ett_lte_rrc_SupportedBandListEUTRA_v9e0,
&ett_lte_rrc_SupportedBandEUTRA,
&ett_lte_rrc_SupportedBandEUTRA_v9e0,
&ett_lte_rrc_MeasParameters,
&ett_lte_rrc_MeasParameters_v1020,
&ett_lte_rrc_MeasParameters_v1130,
&ett_lte_rrc_BandListEUTRA,
&ett_lte_rrc_BandCombinationListEUTRA_r10,
&ett_lte_rrc_BandInfoEUTRA,
&ett_lte_rrc_InterFreqBandList,
&ett_lte_rrc_InterFreqBandInfo,
&ett_lte_rrc_InterRAT_BandList,
&ett_lte_rrc_InterRAT_BandInfo,
&ett_lte_rrc_IRAT_ParametersUTRA_FDD,
&ett_lte_rrc_IRAT_ParametersUTRA_v920,
&ett_lte_rrc_IRAT_ParametersUTRA_v9c0,
&ett_lte_rrc_IRAT_ParametersUTRA_v9h0,
&ett_lte_rrc_SupportedBandListUTRA_FDD,
&ett_lte_rrc_IRAT_ParametersUTRA_TDD128,
&ett_lte_rrc_SupportedBandListUTRA_TDD128,
&ett_lte_rrc_IRAT_ParametersUTRA_TDD384,
&ett_lte_rrc_SupportedBandListUTRA_TDD384,
&ett_lte_rrc_IRAT_ParametersUTRA_TDD768,
&ett_lte_rrc_SupportedBandListUTRA_TDD768,
&ett_lte_rrc_IRAT_ParametersUTRA_TDD_v1020,
&ett_lte_rrc_IRAT_ParametersGERAN,
&ett_lte_rrc_IRAT_ParametersGERAN_v920,
&ett_lte_rrc_SupportedBandListGERAN,
&ett_lte_rrc_IRAT_ParametersCDMA2000_HRPD,
&ett_lte_rrc_SupportedBandListHRPD,
&ett_lte_rrc_IRAT_ParametersCDMA2000_1XRTT,
&ett_lte_rrc_IRAT_ParametersCDMA2000_1XRTT_v920,
&ett_lte_rrc_IRAT_ParametersCDMA2000_1XRTT_v1020,
&ett_lte_rrc_IRAT_ParametersCDMA2000_v1130,
&ett_lte_rrc_SupportedBandList1XRTT,
&ett_lte_rrc_CSG_ProximityIndicationParameters_r9,
&ett_lte_rrc_NeighCellSI_AcquisitionParameters_r9,
&ett_lte_rrc_SON_Parameters_r9,
&ett_lte_rrc_UE_BasedNetwPerfMeasParameters_r10,
&ett_lte_rrc_OTDOA_PositioningCapabilities_r10,
&ett_lte_rrc_Other_Parameters_r11,
&ett_lte_rrc_MBMS_Parameters_r11,
&ett_lte_rrc_UE_TimersAndConstants,
&ett_lte_rrc_MBMS_NotificationConfig_r9,
&ett_lte_rrc_MBSFN_AreaInfoList_r9,
&ett_lte_rrc_MBSFN_AreaInfo_r9,
&ett_lte_rrc_T_mcch_Config_r9,
&ett_lte_rrc_MBSFN_SubframeConfig,
&ett_lte_rrc_T_subframeAllocation,
&ett_lte_rrc_PMCH_InfoList_r9,
&ett_lte_rrc_PMCH_Info_r9,
&ett_lte_rrc_MBMS_SessionInfoList_r9,
&ett_lte_rrc_MBMS_SessionInfo_r9,
&ett_lte_rrc_PMCH_Config_r9,
&ett_lte_rrc_TMGI_r9,
&ett_lte_rrc_T_plmn_Id_r9,
&ett_lte_rrc_HandoverCommand,
&ett_lte_rrc_T_criticalExtensions_38,
&ett_lte_rrc_T_c1_34,
&ett_lte_rrc_T_criticalExtensionsFuture_38,
&ett_lte_rrc_HandoverCommand_r8_IEs,
&ett_lte_rrc_T_nonCriticalExtension_49,
&ett_lte_rrc_HandoverPreparationInformation,
&ett_lte_rrc_T_criticalExtensions_39,
&ett_lte_rrc_T_c1_35,
&ett_lte_rrc_T_criticalExtensionsFuture_39,
&ett_lte_rrc_HandoverPreparationInformation_r8_IEs,
&ett_lte_rrc_HandoverPreparationInformation_v920_IEs,
&ett_lte_rrc_HandoverPreparationInformation_v9d0_IEs,
&ett_lte_rrc_HandoverPreparationInformation_v9e0_IEs,
&ett_lte_rrc_HandoverPreparationInformation_v1130_IEs,
&ett_lte_rrc_T_nonCriticalExtension_50,
&ett_lte_rrc_UERadioAccessCapabilityInformation,
&ett_lte_rrc_T_criticalExtensions_40,
&ett_lte_rrc_T_c1_36,
&ett_lte_rrc_T_criticalExtensionsFuture_40,
&ett_lte_rrc_UERadioAccessCapabilityInformation_r8_IEs,
&ett_lte_rrc_T_nonCriticalExtension_51,
&ett_lte_rrc_AS_Config,
&ett_lte_rrc_AS_Config_v9e0,
&ett_lte_rrc_AS_Context,
&ett_lte_rrc_AS_Context_v1130,
&ett_lte_rrc_ReestablishmentInfo,
&ett_lte_rrc_AdditionalReestabInfoList,
&ett_lte_rrc_AdditionalReestabInfo,
&ett_lte_rrc_RRM_Config,
&ett_lte_rrc_CandidateCellInfoList_r10,
&ett_lte_rrc_CandidateCellInfo_r10,
#line 2908 "../../asn1/lte-rrc/packet-lte-rrc-template.c"
&ett_lte_rrc_featureGroupIndicators,
&ett_lte_rrc_featureGroupIndRel9Add,
&ett_lte_rrc_featureGroupIndRel10,
&ett_lte_rrc_absTimeInfo,
&ett_lte_rrc_nas_SecurityParam,
&ett_lte_rrc_targetRAT_MessageContainer,
&ett_lte_rrc_siPsiSibContainer,
&ett_lte_rrc_dedicatedInfoNAS,
&ett_lte_rrc_timeInfo,
&ett_lte_rrc_serialNumber,
&ett_lte_rrc_warningType,
&ett_lte_rrc_dataCodingScheme,
&ett_lte_rrc_warningMessageSegment,
&ett_lte_rrc_interBandTDD_CA_WithDifferentConfig,
&ett_lte_rrc_sr_ConfigIndex
};
static ei_register_info ei[] = {
{ &ei_lte_rrc_number_pages_le15, { "lte_rrc.number_pages_le15", PI_MALFORMED, PI_ERROR, "Number of pages should be <=15", EXPFILL }},
{ &ei_lte_rrc_si_info_value_changed, { "lte_rrc.si_info_value_changed", PI_SEQUENCE, PI_WARN, "SI Info Value changed", EXPFILL }},
{ &ei_lte_rrc_sibs_changing, { "lte_rrc.sibs_changing", PI_SEQUENCE, PI_WARN, "SIBs changing in next BCCH modification period - signalled in Paging message", EXPFILL }},
{ &ei_lte_rrc_earthquake_warning_sys, { "lte_rrc.earthquake_warning_sys", PI_SEQUENCE, PI_WARN, "Earthquake and Tsunami Warning System Indication!", EXPFILL }},
{ &ei_lte_rrc_commercial_mobile_alert_sys, { "lte_rrc.commercial_mobile_alert_sys", PI_SEQUENCE, PI_WARN, "Commercial Mobile Alert System Indication!", EXPFILL }},
{ &ei_lte_rrc_unexpected_type_value, { "lte_rrc.unexpected_type_value", PI_MALFORMED, PI_ERROR, "Unexpected type value", EXPFILL }},
{ &ei_lte_rrc_unexpected_length_value, { "lte_rrc.unexpected_length_value", PI_MALFORMED, PI_ERROR, "Unexpected type length", EXPFILL }},
{ &ei_lte_rrc_too_many_group_a_rapids, { "lte_rrc.too_many_groupa_rapids", PI_MALFORMED, PI_ERROR, "Too many group A RAPIDs", EXPFILL }},
{ &ei_lte_rrc_invalid_drx_config, { "lte_rrc.invalid_drx_config", PI_MALFORMED, PI_ERROR, "Invalid dedicated DRX config detected", EXPFILL }},
};
expert_module_t* expert_lte_rrc;
proto_lte_rrc = proto_register_protocol(PNAME, PSNAME, PFNAME);
register_dissector("lte_rrc.dl_ccch", dissect_lte_rrc_DL_CCCH, proto_lte_rrc);
register_dissector("lte_rrc.dl_dcch", dissect_lte_rrc_DL_DCCH, proto_lte_rrc);
register_dissector("lte_rrc.ul_ccch", dissect_lte_rrc_UL_CCCH, proto_lte_rrc);
register_dissector("lte_rrc.ul_dcch", dissect_lte_rrc_UL_DCCH, proto_lte_rrc);
register_dissector("lte_rrc.bcch_bch", dissect_lte_rrc_BCCH_BCH, proto_lte_rrc);
register_dissector("lte_rrc.bcch_dl_sch", dissect_lte_rrc_BCCH_DL_SCH, proto_lte_rrc);
register_dissector("lte_rrc.pcch", dissect_lte_rrc_PCCH, proto_lte_rrc);
register_dissector("lte_rrc.mcch", dissect_lte_rrc_MCCH, proto_lte_rrc);
register_dissector("lte_rrc.handover_prep_info", dissect_lte_rrc_Handover_Preparation_Info, proto_lte_rrc);
proto_register_field_array(proto_lte_rrc, hf, array_length(hf));
proto_register_subtree_array(ett, array_length(ett));
expert_lte_rrc = expert_register_protocol(proto_lte_rrc);
expert_register_field_array(expert_lte_rrc, ei, array_length(ei));
#line 1 "../../asn1/lte-rrc/packet-lte-rrc-dis-reg.c"
new_register_dissector("lte-rrc.bcch.bch", dissect_BCCH_BCH_Message_PDU, proto_lte_rrc);
new_register_dissector("lte-rrc.bcch.dl.sch", dissect_BCCH_DL_SCH_Message_PDU, proto_lte_rrc);
new_register_dissector("lte-rrc.mcch", dissect_MCCH_Message_PDU, proto_lte_rrc);
new_register_dissector("lte-rrc.pcch", dissect_PCCH_Message_PDU, proto_lte_rrc);
new_register_dissector("lte-rrc.dl.ccch", dissect_DL_CCCH_Message_PDU, proto_lte_rrc);
new_register_dissector("lte-rrc.dl.dcch", dissect_DL_DCCH_Message_PDU, proto_lte_rrc);
new_register_dissector("lte-rrc.ul.ccch", dissect_UL_CCCH_Message_PDU, proto_lte_rrc);
new_register_dissector("lte-rrc.ul.dcch", dissect_UL_DCCH_Message_PDU, proto_lte_rrc);
new_register_dissector("lte-rrc.ue_cap_info", dissect_UECapabilityInformation_PDU, proto_lte_rrc);
new_register_dissector("lte-rrc.ue_eutra_cap", dissect_lte_rrc_UE_EUTRA_Capability_PDU, proto_lte_rrc);
#line 2962 "../../asn1/lte-rrc/packet-lte-rrc-template.c"
register_init_routine(&lte_rrc_init_protocol);
}
void
proto_reg_handoff_lte_rrc(void)
{
static dissector_handle_t lte_rrc_dl_ccch_handle;
lte_rrc_dl_ccch_handle = find_dissector("lte_rrc.dl_ccch");
dissector_add_for_decode_as("udp.port", lte_rrc_dl_ccch_handle);
nas_eps_handle = find_dissector("nas-eps");
rrc_irat_ho_to_utran_cmd_handle = find_dissector("rrc.irat.ho_to_utran_cmd");
rrc_sys_info_cont_handle = find_dissector("rrc.sysinfo.cont");
gsm_a_dtap_handle = find_dissector("gsm_a_dtap");
gsm_rlcmac_dl_handle = find_dissector("gsm_rlcmac_dl");
}
