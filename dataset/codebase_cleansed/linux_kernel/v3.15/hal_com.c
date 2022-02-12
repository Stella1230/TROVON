void dump_chip_info23a(struct hal_version ChipVersion)
{
int cnt = 0;
u8 buf[128];
cnt += sprintf((buf + cnt), "Chip Version Info: CHIP_8723A_");
cnt += sprintf((buf + cnt), "%s_", IS_NORMAL_CHIP(ChipVersion) ?
"Normal_Chip" : "Test_Chip");
cnt += sprintf((buf + cnt), "%s_",
IS_CHIP_VENDOR_TSMC(ChipVersion) ? "TSMC" : "UMC");
if (IS_A_CUT(ChipVersion))
cnt += sprintf((buf + cnt), "A_CUT_");
else if (IS_B_CUT(ChipVersion))
cnt += sprintf((buf + cnt), "B_CUT_");
else if (IS_C_CUT(ChipVersion))
cnt += sprintf((buf + cnt), "C_CUT_");
else if (IS_D_CUT(ChipVersion))
cnt += sprintf((buf + cnt), "D_CUT_");
else if (IS_E_CUT(ChipVersion))
cnt += sprintf((buf + cnt), "E_CUT_");
else
cnt += sprintf((buf + cnt), "UNKNOWN_CUT(%d)_",
ChipVersion.CUTVersion);
if (IS_1T1R(ChipVersion))
cnt += sprintf((buf + cnt), "1T1R_");
else if (IS_1T2R(ChipVersion))
cnt += sprintf((buf + cnt), "1T2R_");
else if (IS_2T2R(ChipVersion))
cnt += sprintf((buf + cnt), "2T2R_");
else
cnt += sprintf((buf + cnt), "UNKNOWN_RFTYPE(%d)_",
ChipVersion.RFType);
cnt += sprintf((buf + cnt), "RomVer(%d)\n", ChipVersion.ROMVer);
DBG_8723A("%s", buf);
}
u8 hal_com_get_channel_plan23a(struct rtw_adapter *padapter, u8 hw_channel_plan,
u8 sw_channel_plan, u8 def_channel_plan,
bool AutoLoadFail)
{
u8 swConfig;
u8 chnlPlan;
swConfig = true;
if (!AutoLoadFail) {
if (!rtw_is_channel_plan_valid(sw_channel_plan))
swConfig = false;
if (hw_channel_plan & EEPROM_CHANNEL_PLAN_BY_HW_MASK)
swConfig = false;
}
if (swConfig == true)
chnlPlan = sw_channel_plan;
else
chnlPlan = hw_channel_plan & (~EEPROM_CHANNEL_PLAN_BY_HW_MASK);
if (!rtw_is_channel_plan_valid(chnlPlan))
chnlPlan = def_channel_plan;
return chnlPlan;
}
u8 MRateToHwRate23a(u8 rate)
{
u8 ret = DESC_RATE1M;
switch (rate) {
case IEEE80211_CCK_RATE_1MB:
ret = DESC_RATE1M;
break;
case IEEE80211_CCK_RATE_2MB:
ret = DESC_RATE2M;
break;
case IEEE80211_CCK_RATE_5MB:
ret = DESC_RATE5_5M;
break;
case IEEE80211_CCK_RATE_11MB:
ret = DESC_RATE11M;
break;
case IEEE80211_OFDM_RATE_6MB:
ret = DESC_RATE6M;
break;
case IEEE80211_OFDM_RATE_9MB:
ret = DESC_RATE9M;
break;
case IEEE80211_OFDM_RATE_12MB:
ret = DESC_RATE12M;
break;
case IEEE80211_OFDM_RATE_18MB:
ret = DESC_RATE18M;
break;
case IEEE80211_OFDM_RATE_24MB:
ret = DESC_RATE24M;
break;
case IEEE80211_OFDM_RATE_36MB:
ret = DESC_RATE36M;
break;
case IEEE80211_OFDM_RATE_48MB:
ret = DESC_RATE48M;
break;
case IEEE80211_OFDM_RATE_54MB:
ret = DESC_RATE54M;
break;
default:
break;
}
return ret;
}
void HalSetBrateCfg23a(struct rtw_adapter *padapter, u8 *mBratesOS)
{
struct hal_data_8723a *pHalData = GET_HAL_DATA(padapter);
u8 i, is_brate, brate;
u16 brate_cfg = 0;
u8 rate_index;
for (i = 0; i < NDIS_802_11_LENGTH_RATES_EX; i++) {
is_brate = mBratesOS[i] & IEEE80211_BASIC_RATE_MASK;
brate = mBratesOS[i] & 0x7f;
if (is_brate) {
switch (brate) {
case IEEE80211_CCK_RATE_1MB:
brate_cfg |= RATE_1M;
break;
case IEEE80211_CCK_RATE_2MB:
brate_cfg |= RATE_2M;
break;
case IEEE80211_CCK_RATE_5MB:
brate_cfg |= RATE_5_5M;
break;
case IEEE80211_CCK_RATE_11MB:
brate_cfg |= RATE_11M;
break;
case IEEE80211_OFDM_RATE_6MB:
brate_cfg |= RATE_6M;
break;
case IEEE80211_OFDM_RATE_9MB:
brate_cfg |= RATE_9M;
break;
case IEEE80211_OFDM_RATE_12MB:
brate_cfg |= RATE_12M;
break;
case IEEE80211_OFDM_RATE_18MB:
brate_cfg |= RATE_18M;
break;
case IEEE80211_OFDM_RATE_24MB:
brate_cfg |= RATE_24M;
break;
case IEEE80211_OFDM_RATE_36MB:
brate_cfg |= RATE_36M;
break;
case IEEE80211_OFDM_RATE_48MB:
brate_cfg |= RATE_48M;
break;
case IEEE80211_OFDM_RATE_54MB:
brate_cfg |= RATE_54M;
break;
}
}
}
brate_cfg = (brate_cfg | 0xd) & 0x15d;
pHalData->BasicRateSet = brate_cfg;
brate_cfg |= 0x01;
DBG_8723A("HW_VAR_BASIC_RATE: BrateCfg(%#x)\n", brate_cfg);
rtw_write8(padapter, REG_RRSR, brate_cfg & 0xff);
rtw_write8(padapter, REG_RRSR + 1, (brate_cfg >> 8) & 0xff);
rtw_write8(padapter, REG_RRSR + 2,
rtw_read8(padapter, REG_RRSR + 2) & 0xf0);
rate_index = 0;
while (brate_cfg > 0x1) {
brate_cfg = (brate_cfg >> 1);
rate_index++;
}
rtw_write8(padapter, REG_INIRTS_RATE_SEL, rate_index);
return;
}
static void _OneOutPipeMapping(struct rtw_adapter *pAdapter)
{
struct dvobj_priv *pdvobjpriv = adapter_to_dvobj(pAdapter);
pdvobjpriv->Queue2Pipe[0] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[1] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[2] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[3] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[4] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[5] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[6] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[7] = pdvobjpriv->RtOutPipe[0];
}
static void _TwoOutPipeMapping(struct rtw_adapter *pAdapter, bool bWIFICfg)
{
struct dvobj_priv *pdvobjpriv = adapter_to_dvobj(pAdapter);
if (bWIFICfg) {
pdvobjpriv->Queue2Pipe[0] = pdvobjpriv->RtOutPipe[1];
pdvobjpriv->Queue2Pipe[1] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[2] = pdvobjpriv->RtOutPipe[1];
pdvobjpriv->Queue2Pipe[3] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[4] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[5] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[6] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[7] = pdvobjpriv->RtOutPipe[0];
} else {
pdvobjpriv->Queue2Pipe[0] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[1] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[2] = pdvobjpriv->RtOutPipe[1];
pdvobjpriv->Queue2Pipe[3] = pdvobjpriv->RtOutPipe[1];
pdvobjpriv->Queue2Pipe[4] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[5] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[6] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[7] = pdvobjpriv->RtOutPipe[0];
}
}
static void _ThreeOutPipeMapping(struct rtw_adapter *pAdapter, bool bWIFICfg)
{
struct dvobj_priv *pdvobjpriv = adapter_to_dvobj(pAdapter);
if (bWIFICfg) {
pdvobjpriv->Queue2Pipe[0] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[1] = pdvobjpriv->RtOutPipe[1];
pdvobjpriv->Queue2Pipe[2] = pdvobjpriv->RtOutPipe[2];
pdvobjpriv->Queue2Pipe[3] = pdvobjpriv->RtOutPipe[1];
pdvobjpriv->Queue2Pipe[4] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[5] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[6] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[7] = pdvobjpriv->RtOutPipe[0];
} else {
pdvobjpriv->Queue2Pipe[0] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[1] = pdvobjpriv->RtOutPipe[1];
pdvobjpriv->Queue2Pipe[2] = pdvobjpriv->RtOutPipe[2];
pdvobjpriv->Queue2Pipe[3] = pdvobjpriv->RtOutPipe[2];
pdvobjpriv->Queue2Pipe[4] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[5] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[6] = pdvobjpriv->RtOutPipe[0];
pdvobjpriv->Queue2Pipe[7] = pdvobjpriv->RtOutPipe[0];
}
}
bool Hal_MappingOutPipe23a(struct rtw_adapter *pAdapter, u8 NumOutPipe)
{
struct registry_priv *pregistrypriv = &pAdapter->registrypriv;
bool bWIFICfg = (pregistrypriv->wifi_spec) ? true : false;
bool result = true;
switch (NumOutPipe) {
case 2:
_TwoOutPipeMapping(pAdapter, bWIFICfg);
break;
case 3:
_ThreeOutPipeMapping(pAdapter, bWIFICfg);
break;
case 1:
_OneOutPipeMapping(pAdapter);
break;
default:
result = false;
break;
}
return result;
}
void hal_init_macaddr23a(struct rtw_adapter *adapter)
{
rtw_hal_set_hwreg23a(adapter, HW_VAR_MAC_ADDR,
adapter->eeprompriv.mac_addr);
}
void c2h_evt_clear23a(struct rtw_adapter *adapter)
{
rtw_write8(adapter, REG_C2HEVT_CLEAR, C2H_EVT_HOST_CLOSE);
}
s32 c2h_evt_read23a(struct rtw_adapter *adapter, u8 *buf)
{
s32 ret = _FAIL;
struct c2h_evt_hdr *c2h_evt;
int i;
u8 trigger;
if (buf == NULL)
goto exit;
trigger = rtw_read8(adapter, REG_C2HEVT_CLEAR);
if (trigger == C2H_EVT_HOST_CLOSE)
goto exit;
else if (trigger != C2H_EVT_FW_CLOSE)
goto clear_evt;
c2h_evt = (struct c2h_evt_hdr *)buf;
memset(c2h_evt, 0, 16);
*buf = rtw_read8(adapter, REG_C2HEVT_MSG_NORMAL);
*(buf + 1) = rtw_read8(adapter, REG_C2HEVT_MSG_NORMAL + 1);
RT_PRINT_DATA(_module_hal_init_c_, _drv_info_, "c2h_evt_read23a(): ",
&c2h_evt, sizeof(c2h_evt));
if (0) {
DBG_8723A("%s id:%u, len:%u, seq:%u, trigger:0x%02x\n",
__func__, c2h_evt->id, c2h_evt->plen, c2h_evt->seq,
trigger);
}
for (i = 0; i < c2h_evt->plen; i++)
c2h_evt->payload[i] = rtw_read8(adapter,
REG_C2HEVT_MSG_NORMAL +
sizeof(*c2h_evt) + i);
RT_PRINT_DATA(_module_hal_init_c_, _drv_info_,
"c2h_evt_read23a(): Command Content:\n", c2h_evt->payload,
c2h_evt->plen);
ret = _SUCCESS;
clear_evt:
c2h_evt_clear23a(adapter);
exit:
return ret;
}
void
rtl8723a_set_ampdu_min_space(struct rtw_adapter *padapter, u8 MinSpacingToSet)
{
u8 SecMinSpace;
if (MinSpacingToSet <= 7) {
switch (padapter->securitypriv.dot11PrivacyAlgrthm) {
case _NO_PRIVACY_:
case _AES_:
SecMinSpace = 0;
break;
case _WEP40_:
case _WEP104_:
case _TKIP_:
case _TKIP_WTMIC_:
SecMinSpace = 6;
break;
default:
SecMinSpace = 7;
break;
}
if (MinSpacingToSet < SecMinSpace)
MinSpacingToSet = SecMinSpace;
MinSpacingToSet |=
rtw_read8(padapter, REG_AMPDU_MIN_SPACE) & 0xf8;
rtw_write8(padapter, REG_AMPDU_MIN_SPACE,
MinSpacingToSet);
}
}
void rtl8723a_set_ampdu_factor(struct rtw_adapter *padapter, u8 FactorToSet)
{
u8 RegToSet_Normal[4] = { 0x41, 0xa8, 0x72, 0xb9 };
u8 MaxAggNum;
u8 *pRegToSet;
u8 index = 0;
pRegToSet = RegToSet_Normal;
#ifdef CONFIG_8723AU_BT_COEXIST
if ((BT_IsBtDisabled(padapter) == false) &&
(BT_1Ant(padapter) == true)) {
MaxAggNum = 0x8;
} else
#endif
{
MaxAggNum = 0xF;
}
if (FactorToSet <= 3) {
FactorToSet = (1 << (FactorToSet + 2));
if (FactorToSet > MaxAggNum)
FactorToSet = MaxAggNum;
for (index = 0; index < 4; index++) {
if ((pRegToSet[index] & 0xf0) > (FactorToSet << 4))
pRegToSet[index] = (pRegToSet[index] & 0x0f) |
(FactorToSet << 4);
if ((pRegToSet[index] & 0x0f) > FactorToSet)
pRegToSet[index] = (pRegToSet[index] & 0xf0) |
FactorToSet;
rtw_write8(padapter, REG_AGGLEN_LMT + index,
pRegToSet[index]);
}
}
}
void rtl8723a_set_acm_ctrl(struct rtw_adapter *padapter, u8 ctrl)
{
u8 hwctrl = 0;
if (ctrl != 0) {
hwctrl |= AcmHw_HwEn;
if (ctrl & BIT(1))
hwctrl |= AcmHw_BeqEn;
if (ctrl & BIT(2))
hwctrl |= AcmHw_ViqEn;
if (ctrl & BIT(3))
hwctrl |= AcmHw_VoqEn;
}
DBG_8723A("[HW_VAR_ACM_CTRL] Write 0x%02X\n", hwctrl);
rtw_write8(padapter, REG_ACMHWCTRL, hwctrl);
}
void rtl8723a_set_media_status(struct rtw_adapter *padapter, u8 status)
{
u8 val8;
val8 = rtw_read8(padapter, MSR) & 0x0c;
val8 |= status;
rtw_write8(padapter, MSR, val8);
}
void rtl8723a_set_media_status1(struct rtw_adapter *padapter, u8 status)
{
u8 val8;
val8 = rtw_read8(padapter, MSR) & 0x03;
val8 |= status << 2;
rtw_write8(padapter, MSR, val8);
}
void rtl8723a_set_bcn_func(struct rtw_adapter *padapter, u8 val)
{
if (val)
SetBcnCtrlReg23a(padapter, EN_BCN_FUNCTION | EN_TXBCN_RPT, 0);
else
SetBcnCtrlReg23a(padapter, 0, EN_BCN_FUNCTION | EN_TXBCN_RPT);
}
void rtl8723a_check_bssid(struct rtw_adapter *padapter, u8 val)
{
u32 val32;
val32 = rtw_read32(padapter, REG_RCR);
if (val)
val32 |= RCR_CBSSID_DATA | RCR_CBSSID_BCN;
else
val32 &= ~(RCR_CBSSID_DATA | RCR_CBSSID_BCN);
rtw_write32(padapter, REG_RCR, val32);
}
void rtl8723a_mlme_sitesurvey(struct rtw_adapter *padapter, u8 flag)
{
if (flag) {
u32 v32;
v32 = rtw_read32(padapter, REG_RCR);
v32 &= ~(RCR_CBSSID_BCN);
rtw_write32(padapter, REG_RCR, v32);
rtw_write16(padapter, REG_RXFLTMAP2, 0);
SetBcnCtrlReg23a(padapter, DIS_TSF_UDT, 0);
} else {
struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
struct mlme_ext_info *pmlmeinfo;
u32 v32;
pmlmeinfo = &pmlmeext->mlmext_info;
if ((is_client_associated_to_ap23a(padapter) == true) ||
((pmlmeinfo->state & 0x03) == WIFI_FW_ADHOC_STATE) ||
((pmlmeinfo->state & 0x03) == WIFI_FW_AP_STATE)) {
rtw_write16(padapter, REG_RXFLTMAP2, 0xFFFF);
SetBcnCtrlReg23a(padapter, 0, DIS_TSF_UDT);
}
v32 = rtw_read32(padapter, REG_RCR);
v32 |= RCR_CBSSID_BCN;
rtw_write32(padapter, REG_RCR, v32);
}
#ifdef CONFIG_8723AU_BT_COEXIST
BT_WifiScanNotify(padapter, flag ? true : false);
#endif
}
void rtl8723a_on_rcr_am(struct rtw_adapter *padapter)
{
rtw_write32(padapter, REG_RCR, rtw_read32(padapter, REG_RCR) | RCR_AM);
DBG_8723A("%s, %d, RCR = %x \n", __FUNCTION__, __LINE__,
rtw_read32(padapter, REG_RCR));
}
void rtl8723a_off_rcr_am(struct rtw_adapter *padapter)
{
rtw_write32(padapter, REG_RCR,
rtw_read32(padapter, REG_RCR) & (~RCR_AM));
DBG_8723A("%s, %d, RCR = %x \n", __FUNCTION__, __LINE__,
rtw_read32(padapter, REG_RCR));
}
void rtl8723a_set_slot_time(struct rtw_adapter *padapter, u8 slottime)
{
u8 u1bAIFS, aSifsTime;
struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
struct mlme_ext_info *pmlmeinfo = &pmlmeext->mlmext_info;
rtw_write8(padapter, REG_SLOT, slottime);
if (pmlmeinfo->WMM_enable == 0) {
if (pmlmeext->cur_wireless_mode == WIRELESS_11B)
aSifsTime = 10;
else
aSifsTime = 16;
u1bAIFS = aSifsTime + (2 * pmlmeinfo->slotTime);
rtw_write8(padapter, REG_EDCA_VO_PARAM, u1bAIFS);
rtw_write8(padapter, REG_EDCA_VI_PARAM, u1bAIFS);
rtw_write8(padapter, REG_EDCA_BE_PARAM, u1bAIFS);
rtw_write8(padapter, REG_EDCA_BK_PARAM, u1bAIFS);
}
}
void rtl8723a_ack_preamble(struct rtw_adapter *padapter, u8 bShortPreamble)
{
struct hal_data_8723a *pHalData = GET_HAL_DATA(padapter);
u8 regTmp;
regTmp = (pHalData->nCur40MhzPrimeSC) << 5;
if (bShortPreamble)
regTmp |= 0x80;
rtw_write8(padapter, REG_RRSR + 2, regTmp);
}
void rtl8723a_set_sec_cfg(struct rtw_adapter *padapter, u8 sec)
{
rtw_write8(padapter, REG_SECCFG, sec);
}
void rtl8723a_cam_empty_entry(struct rtw_adapter *padapter, u8 ucIndex)
{
u8 i;
u32 ulCommand = 0;
u32 ulContent = 0;
u32 ulEncAlgo = CAM_AES;
for (i = 0; i < CAM_CONTENT_COUNT; i++) {
if (i == 0) {
ulContent |= (ucIndex & 0x03) |
((u16) (ulEncAlgo) << 2);
} else {
ulContent = 0;
}
ulCommand = CAM_CONTENT_COUNT * ucIndex + i;
ulCommand = ulCommand | CAM_POLLINIG | CAM_WRITE;
rtw_write32(padapter, WCAMI, ulContent);
rtw_write32(padapter, RWCAM, ulCommand);
}
}
void rtl8723a_cam_invalid_all(struct rtw_adapter *padapter)
{
rtw_write32(padapter, RWCAM, BIT(31) | BIT(30));
}
void rtl8723a_cam_write(struct rtw_adapter *padapter, u32 val1, u32 val2)
{
u32 cmd;
rtw_write32(padapter, WCAMI, val1);
cmd = CAM_POLLINIG | CAM_WRITE | val2;
rtw_write32(padapter, RWCAM, cmd);
}
void rtl8723a_fifo_cleanup(struct rtw_adapter *padapter)
{
#define RW_RELEASE_EN BIT(18)
#define RXDMA_IDLE BIT(17)
struct pwrctrl_priv *pwrpriv = &padapter->pwrctrlpriv;
u8 trycnt = 100;
rtw_write8(padapter, REG_TXPAUSE, 0xff);
padapter->xmitpriv.nqos_ssn = rtw_read16(padapter, REG_NQOS_SEQ);
if (pwrpriv->bkeepfwalive != true) {
u32 v32;
v32 = rtw_read32(padapter, REG_RXPKT_NUM);
v32 |= RW_RELEASE_EN;
rtw_write32(padapter, REG_RXPKT_NUM, v32);
do {
v32 = rtw_read32(padapter, REG_RXPKT_NUM) & RXDMA_IDLE;
if (!v32)
break;
} while (trycnt--);
if (trycnt == 0) {
DBG_8723A("Stop RX DMA failed......\n");
}
rtw_write16(padapter, REG_RQPN_NPQ, 0);
rtw_write32(padapter, REG_RQPN, 0x80000000);
mdelay(10);
}
}
void rtl8723a_set_apfm_on_mac(struct rtw_adapter *padapter, u8 val)
{
struct hal_data_8723a *pHalData = GET_HAL_DATA(padapter);
pHalData->bMacPwrCtrlOn = val;
DBG_8723A("%s: bMacPwrCtrlOn =%d\n", __func__, pHalData->bMacPwrCtrlOn);
}
void rtl8723a_bcn_valid(struct rtw_adapter *padapter)
{
rtw_write8(padapter, REG_TDECTRL + 2,
rtw_read8(padapter, REG_TDECTRL + 2) | BIT0);
}
void rtl8723a_set_tx_pause(struct rtw_adapter *padapter, u8 pause)
{
rtw_write8(padapter, REG_TXPAUSE, pause);
}
void rtl8723a_set_beacon_interval(struct rtw_adapter *padapter, u16 interval)
{
rtw_write16(padapter, REG_BCN_INTERVAL, interval);
}
void rtl8723a_set_resp_sifs(struct rtw_adapter *padapter,
u8 r2t1, u8 r2t2, u8 t2t1, u8 t2t2)
{
rtw_write8(padapter, REG_R2T_SIFS, r2t1);
rtw_write8(padapter, REG_R2T_SIFS + 1, r2t2);
rtw_write8(padapter, REG_T2T_SIFS, t2t1);
rtw_write8(padapter, REG_T2T_SIFS + 1, t2t2);
}
void rtl8723a_set_ac_param_vo(struct rtw_adapter *padapter, u32 vo)
{
rtw_write32(padapter, REG_EDCA_VO_PARAM, vo);
}
void rtl8723a_set_ac_param_vi(struct rtw_adapter *padapter, u32 vi)
{
rtw_write32(padapter, REG_EDCA_VI_PARAM, vi);
}
void rtl8723a_set_ac_param_be(struct rtw_adapter *padapter, u32 be)
{
struct hal_data_8723a *pHalData = GET_HAL_DATA(padapter);
pHalData->AcParam_BE = be;
rtw_write32(padapter, REG_EDCA_BE_PARAM, be);
}
void rtl8723a_set_ac_param_bk(struct rtw_adapter *padapter, u32 bk)
{
rtw_write32(padapter, REG_EDCA_BK_PARAM, bk);
}
void rtl8723a_set_rxdma_agg_pg_th(struct rtw_adapter *padapter, u8 val)
{
rtw_write8(padapter, REG_RXDMA_AGG_PG_TH, val);
}
void rtl8723a_set_nav_upper(struct rtw_adapter *padapter, u32 usNavUpper)
{
if (usNavUpper > HAL_8723A_NAV_UPPER_UNIT * 0xFF) {
RT_TRACE(_module_hal_init_c_, _drv_notice_,
("The setting value (0x%08X us) of NAV_UPPER "
"is larger than (%d * 0xFF)!!!\n",
usNavUpper, HAL_8723A_NAV_UPPER_UNIT));
return;
}
usNavUpper = (usNavUpper + HAL_8723A_NAV_UPPER_UNIT - 1) /
HAL_8723A_NAV_UPPER_UNIT;
rtw_write8(padapter, REG_NAV_UPPER, (u8) usNavUpper);
}
void rtl8723a_set_initial_gain(struct rtw_adapter *padapter, u32 rx_gain)
{
struct hal_data_8723a *pHalData = GET_HAL_DATA(padapter);
struct dig_t *pDigTable = &pHalData->odmpriv.DM_DigTable;
if (rx_gain == 0xff)
ODM_Write_DIG23a(&pHalData->odmpriv, pDigTable->BackupIGValue);
else {
pDigTable->BackupIGValue = pDigTable->CurIGValue;
ODM_Write_DIG23a(&pHalData->odmpriv, rx_gain);
}
}
void rtl8723a_odm_support_ability_write(struct rtw_adapter *padapter, u32 val)
{
struct hal_data_8723a *pHalData = GET_HAL_DATA(padapter);
pHalData->odmpriv.SupportAbility = val;
}
void rtl8723a_odm_support_ability_backup(struct rtw_adapter *padapter, u8 val)
{
struct hal_data_8723a *pHalData = GET_HAL_DATA(padapter);
if (val)
pHalData->odmpriv.BK_SupportAbility =
pHalData->odmpriv.SupportAbility;
else
pHalData->odmpriv.SupportAbility =
pHalData->odmpriv.BK_SupportAbility;
}
void rtl8723a_odm_support_ability_set(struct rtw_adapter *padapter, u32 val)
{
struct hal_data_8723a *pHalData = GET_HAL_DATA(padapter);
if (val == DYNAMIC_ALL_FUNC_ENABLE) {
pHalData->dmpriv.DMFlag = pHalData->dmpriv.InitDMFlag;
pHalData->odmpriv.SupportAbility = pHalData->dmpriv.InitODMFlag;
} else {
pHalData->odmpriv.SupportAbility |= val;
}
}
void rtl8723a_odm_support_ability_clr(struct rtw_adapter *padapter, u32 val)
{
struct hal_data_8723a *pHalData = GET_HAL_DATA(padapter);
pHalData->odmpriv.SupportAbility &= val;
}
void rtl8723a_set_rpwm(struct rtw_adapter *padapter, u8 val)
{
rtw_write8(padapter, REG_USB_HRPWM, val);
}
