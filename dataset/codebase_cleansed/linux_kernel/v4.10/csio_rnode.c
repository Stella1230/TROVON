int
csio_is_rnode_ready(struct csio_rnode *rn)
{
return csio_match_state(rn, csio_rns_ready);
}
static int
csio_is_rnode_uninit(struct csio_rnode *rn)
{
return csio_match_state(rn, csio_rns_uninit);
}
static int
csio_is_rnode_wka(uint8_t rport_type)
{
if ((rport_type == FLOGI_VFPORT) ||
(rport_type == FDISC_VFPORT) ||
(rport_type == NS_VNPORT) ||
(rport_type == FDMI_VNPORT))
return 1;
return 0;
}
static struct csio_rnode *
csio_rn_lookup(struct csio_lnode *ln, uint32_t flowid)
{
struct csio_rnode *rnhead = (struct csio_rnode *) &ln->rnhead;
struct list_head *tmp;
struct csio_rnode *rn;
list_for_each(tmp, &rnhead->sm.sm_list) {
rn = (struct csio_rnode *) tmp;
if (rn->flowid == flowid)
return rn;
}
return NULL;
}
static struct csio_rnode *
csio_rn_lookup_wwpn(struct csio_lnode *ln, uint8_t *wwpn)
{
struct csio_rnode *rnhead = (struct csio_rnode *) &ln->rnhead;
struct list_head *tmp;
struct csio_rnode *rn;
list_for_each(tmp, &rnhead->sm.sm_list) {
rn = (struct csio_rnode *) tmp;
if (!memcmp(csio_rn_wwpn(rn), wwpn, 8))
return rn;
}
return NULL;
}
struct csio_rnode *
csio_rnode_lookup_portid(struct csio_lnode *ln, uint32_t portid)
{
struct csio_rnode *rnhead = (struct csio_rnode *) &ln->rnhead;
struct list_head *tmp;
struct csio_rnode *rn;
list_for_each(tmp, &rnhead->sm.sm_list) {
rn = (struct csio_rnode *) tmp;
if (rn->nport_id == portid)
return rn;
}
return NULL;
}
static int
csio_rn_dup_flowid(struct csio_lnode *ln, uint32_t rdev_flowid,
uint32_t *vnp_flowid)
{
struct csio_rnode *rnhead;
struct list_head *tmp, *tmp1;
struct csio_rnode *rn;
struct csio_lnode *ln_tmp;
struct csio_hw *hw = csio_lnode_to_hw(ln);
list_for_each(tmp1, &hw->sln_head) {
ln_tmp = (struct csio_lnode *) tmp1;
if (ln_tmp == ln)
continue;
rnhead = (struct csio_rnode *)&ln_tmp->rnhead;
list_for_each(tmp, &rnhead->sm.sm_list) {
rn = (struct csio_rnode *) tmp;
if (csio_is_rnode_ready(rn)) {
if (rn->flowid == rdev_flowid) {
*vnp_flowid = csio_ln_flowid(ln_tmp);
return 1;
}
}
}
}
return 0;
}
static struct csio_rnode *
csio_alloc_rnode(struct csio_lnode *ln)
{
struct csio_hw *hw = csio_lnode_to_hw(ln);
struct csio_rnode *rn = mempool_alloc(hw->rnode_mempool, GFP_ATOMIC);
if (!rn)
goto err;
memset(rn, 0, sizeof(struct csio_rnode));
if (csio_rnode_init(rn, ln))
goto err_free;
CSIO_INC_STATS(ln, n_rnode_alloc);
return rn;
err_free:
mempool_free(rn, hw->rnode_mempool);
err:
CSIO_INC_STATS(ln, n_rnode_nomem);
return NULL;
}
static void
csio_free_rnode(struct csio_rnode *rn)
{
struct csio_hw *hw = csio_lnode_to_hw(csio_rnode_to_lnode(rn));
csio_rnode_exit(rn);
CSIO_INC_STATS(rn->lnp, n_rnode_free);
mempool_free(rn, hw->rnode_mempool);
}
static struct csio_rnode *
csio_get_rnode(struct csio_lnode *ln, uint32_t flowid)
{
struct csio_rnode *rn;
rn = csio_rn_lookup(ln, flowid);
if (!rn) {
rn = csio_alloc_rnode(ln);
if (!rn)
return NULL;
rn->flowid = flowid;
}
return rn;
}
void
csio_put_rnode(struct csio_lnode *ln, struct csio_rnode *rn)
{
CSIO_DB_ASSERT(csio_is_rnode_uninit(rn) != 0);
csio_free_rnode(rn);
}
struct csio_rnode *
csio_confirm_rnode(struct csio_lnode *ln, uint32_t rdev_flowid,
struct fcoe_rdev_entry *rdevp)
{
uint8_t rport_type;
struct csio_rnode *rn, *match_rn;
uint32_t vnp_flowid = 0;
__be32 *port_id;
port_id = (__be32 *)&rdevp->r_id[0];
rport_type =
FW_RDEV_WR_RPORT_TYPE_GET(rdevp->rd_xfer_rdy_to_rport_type);
if (rport_type == FAB_CTLR_VNPORT) {
csio_ln_dbg(ln,
"Unhandled rport_type:%d recv in rdev evt "
"ssni:x%x\n", rport_type, rdev_flowid);
return NULL;
}
rn = csio_rn_lookup(ln, rdev_flowid);
if (!rn) {
if (csio_rn_dup_flowid(ln, rdev_flowid, &vnp_flowid)) {
csio_ln_warn(ln,
"ssni:%x already active on vnpi:%x",
rdev_flowid, vnp_flowid);
return NULL;
}
rn = csio_rn_lookup_wwpn(ln, rdevp->wwpn);
if (!rn)
goto alloc_rnode;
} else {
if (csio_is_rnode_wka(rport_type)) {
match_rn = csio_rnode_lookup_portid(ln,
((ntohl(*port_id) >> 8) & CSIO_DID_MASK));
if (match_rn == NULL) {
csio_rn_flowid(rn) = CSIO_INVALID_IDX;
goto alloc_rnode;
}
if (!memcmp(csio_rn_wwpn(match_rn), rdevp->wwpn, 8)) {
if (rn == match_rn)
goto found_rnode;
csio_ln_dbg(ln,
"nport_id:x%x and wwpn:%llx"
" match for ssni:x%x\n",
rn->nport_id,
wwn_to_u64(rdevp->wwpn),
rdev_flowid);
if (csio_is_rnode_ready(rn)) {
csio_ln_warn(ln,
"rnode is already"
"active ssni:x%x\n",
rdev_flowid);
CSIO_ASSERT(0);
}
csio_rn_flowid(rn) = CSIO_INVALID_IDX;
rn = match_rn;
goto found_rnode;
}
csio_rn_flowid(rn) = CSIO_INVALID_IDX;
goto alloc_rnode;
}
if (!memcmp(csio_rn_wwpn(rn), rdevp->wwpn, 8))
goto found_rnode;
match_rn = csio_rn_lookup_wwpn(ln, rdevp->wwpn);
if (match_rn != NULL) {
csio_ln_dbg(ln,
"ssni:x%x changed for rport name(wwpn):%llx "
"did:x%x\n", rdev_flowid,
wwn_to_u64(rdevp->wwpn),
match_rn->nport_id);
csio_rn_flowid(rn) = CSIO_INVALID_IDX;
rn = match_rn;
} else {
csio_ln_dbg(ln,
"rnode wwpn mismatch found ssni:x%x "
"name(wwpn):%llx\n",
rdev_flowid,
wwn_to_u64(csio_rn_wwpn(rn)));
if (csio_is_rnode_ready(rn)) {
csio_ln_warn(ln,
"rnode is already active "
"wwpn:%llx ssni:x%x\n",
wwn_to_u64(csio_rn_wwpn(rn)),
rdev_flowid);
CSIO_ASSERT(0);
}
csio_rn_flowid(rn) = CSIO_INVALID_IDX;
goto alloc_rnode;
}
}
found_rnode:
csio_ln_dbg(ln, "found rnode:%p ssni:x%x name(wwpn):%llx\n",
rn, rdev_flowid, wwn_to_u64(rdevp->wwpn));
csio_rn_flowid(rn) = rdev_flowid;
rn->rdev_entry = rdevp;
CSIO_INC_STATS(ln, n_rnode_match);
return rn;
alloc_rnode:
rn = csio_get_rnode(ln, rdev_flowid);
if (!rn)
return NULL;
csio_ln_dbg(ln, "alloc rnode:%p ssni:x%x name(wwpn):%llx\n",
rn, rdev_flowid, wwn_to_u64(rdevp->wwpn));
rn->rdev_entry = rdevp;
return rn;
}
static int
csio_rn_verify_rparams(struct csio_lnode *ln, struct csio_rnode *rn,
struct fcoe_rdev_entry *rdevp)
{
uint8_t null[8];
uint8_t rport_type;
uint8_t fc_class;
__be32 *did;
did = (__be32 *) &rdevp->r_id[0];
rport_type =
FW_RDEV_WR_RPORT_TYPE_GET(rdevp->rd_xfer_rdy_to_rport_type);
switch (rport_type) {
case FLOGI_VFPORT:
rn->role = CSIO_RNFR_FABRIC;
if (((ntohl(*did) >> 8) & CSIO_DID_MASK) != FC_FID_FLOGI) {
csio_ln_err(ln, "ssni:x%x invalid fabric portid\n",
csio_rn_flowid(rn));
return -EINVAL;
}
if (FW_RDEV_WR_NPIV_GET(rdevp->vft_to_qos))
ln->flags |= CSIO_LNF_NPIVSUPP;
break;
case NS_VNPORT:
rn->role = CSIO_RNFR_NS;
if (((ntohl(*did) >> 8) & CSIO_DID_MASK) != FC_FID_DIR_SERV) {
csio_ln_err(ln, "ssni:x%x invalid fabric portid\n",
csio_rn_flowid(rn));
return -EINVAL;
}
break;
case REG_FC4_VNPORT:
case REG_VNPORT:
rn->role = CSIO_RNFR_NPORT;
if (rdevp->event_cause == PRLI_ACC_RCVD ||
rdevp->event_cause == PRLI_RCVD) {
if (FW_RDEV_WR_TASK_RETRY_ID_GET(
rdevp->enh_disc_to_tgt))
rn->fcp_flags |= FCP_SPPF_OVLY_ALLOW;
if (FW_RDEV_WR_RETRY_GET(rdevp->enh_disc_to_tgt))
rn->fcp_flags |= FCP_SPPF_RETRY;
if (FW_RDEV_WR_CONF_CMPL_GET(rdevp->enh_disc_to_tgt))
rn->fcp_flags |= FCP_SPPF_CONF_COMPL;
if (FW_RDEV_WR_TGT_GET(rdevp->enh_disc_to_tgt))
rn->role |= CSIO_RNFR_TARGET;
if (FW_RDEV_WR_INI_GET(rdevp->enh_disc_to_tgt))
rn->role |= CSIO_RNFR_INITIATOR;
}
break;
case FDMI_VNPORT:
case FAB_CTLR_VNPORT:
rn->role = 0;
break;
default:
csio_ln_err(ln, "ssni:x%x invalid rport type recv x%x\n",
csio_rn_flowid(rn), rport_type);
return -EINVAL;
}
if (rport_type == REG_VNPORT || rport_type == NS_VNPORT) {
memset(null, 0, 8);
if (!memcmp(rdevp->wwnn, null, 8)) {
csio_ln_err(ln,
"ssni:x%x invalid wwnn received from"
" rport did:x%x\n",
csio_rn_flowid(rn),
(ntohl(*did) & CSIO_DID_MASK));
return -EINVAL;
}
if (!memcmp(rdevp->wwpn, null, 8)) {
csio_ln_err(ln,
"ssni:x%x invalid wwpn received from"
" rport did:x%x\n",
csio_rn_flowid(rn),
(ntohl(*did) & CSIO_DID_MASK));
return -EINVAL;
}
}
rn->nport_id = (ntohl(*did) >> 8) & CSIO_DID_MASK;
memcpy(csio_rn_wwnn(rn), rdevp->wwnn, 8);
memcpy(csio_rn_wwpn(rn), rdevp->wwpn, 8);
rn->rn_sparm.csp.sp_bb_data = rdevp->rcv_fr_sz;
fc_class = FW_RDEV_WR_CLASS_GET(rdevp->vft_to_qos);
rn->rn_sparm.clsp[fc_class - 1].cp_class = htons(FC_CPC_VALID);
return 0;
}
static void
__csio_reg_rnode(struct csio_rnode *rn)
{
struct csio_lnode *ln = csio_rnode_to_lnode(rn);
struct csio_hw *hw = csio_lnode_to_hw(ln);
spin_unlock_irq(&hw->lock);
csio_reg_rnode(rn);
spin_lock_irq(&hw->lock);
if (rn->role & CSIO_RNFR_TARGET)
ln->n_scsi_tgts++;
if (rn->nport_id == FC_FID_MGMT_SERV)
csio_ln_fdmi_start(ln, (void *) rn);
}
static void
__csio_unreg_rnode(struct csio_rnode *rn)
{
struct csio_lnode *ln = csio_rnode_to_lnode(rn);
struct csio_hw *hw = csio_lnode_to_hw(ln);
LIST_HEAD(tmp_q);
int cmpl = 0;
if (!list_empty(&rn->host_cmpl_q)) {
csio_dbg(hw, "Returning completion queue I/Os\n");
list_splice_tail_init(&rn->host_cmpl_q, &tmp_q);
cmpl = 1;
}
if (rn->role & CSIO_RNFR_TARGET) {
ln->n_scsi_tgts--;
ln->last_scan_ntgts--;
}
spin_unlock_irq(&hw->lock);
csio_unreg_rnode(rn);
spin_lock_irq(&hw->lock);
if (cmpl)
csio_scsi_cleanup_io_q(csio_hw_to_scsim(hw), &tmp_q);
}
static void
csio_rns_uninit(struct csio_rnode *rn, enum csio_rn_ev evt)
{
struct csio_lnode *ln = csio_rnode_to_lnode(rn);
int ret = 0;
CSIO_INC_STATS(rn, n_evt_sm[evt]);
switch (evt) {
case CSIO_RNFE_LOGGED_IN:
case CSIO_RNFE_PLOGI_RECV:
ret = csio_rn_verify_rparams(ln, rn, rn->rdev_entry);
if (!ret) {
csio_set_state(&rn->sm, csio_rns_ready);
__csio_reg_rnode(rn);
} else {
CSIO_INC_STATS(rn, n_err_inval);
}
break;
case CSIO_RNFE_LOGO_RECV:
csio_ln_dbg(ln,
"ssni:x%x Ignoring event %d recv "
"in rn state[uninit]\n", csio_rn_flowid(rn), evt);
CSIO_INC_STATS(rn, n_evt_drop);
break;
default:
csio_ln_dbg(ln,
"ssni:x%x unexp event %d recv "
"in rn state[uninit]\n", csio_rn_flowid(rn), evt);
CSIO_INC_STATS(rn, n_evt_unexp);
break;
}
}
static void
csio_rns_ready(struct csio_rnode *rn, enum csio_rn_ev evt)
{
struct csio_lnode *ln = csio_rnode_to_lnode(rn);
int ret = 0;
CSIO_INC_STATS(rn, n_evt_sm[evt]);
switch (evt) {
case CSIO_RNFE_LOGGED_IN:
case CSIO_RNFE_PLOGI_RECV:
csio_ln_dbg(ln,
"ssni:x%x Ignoring event %d recv from did:x%x "
"in rn state[ready]\n", csio_rn_flowid(rn), evt,
rn->nport_id);
CSIO_INC_STATS(rn, n_evt_drop);
break;
case CSIO_RNFE_PRLI_DONE:
case CSIO_RNFE_PRLI_RECV:
ret = csio_rn_verify_rparams(ln, rn, rn->rdev_entry);
if (!ret)
__csio_reg_rnode(rn);
else
CSIO_INC_STATS(rn, n_err_inval);
break;
case CSIO_RNFE_DOWN:
csio_set_state(&rn->sm, csio_rns_offline);
__csio_unreg_rnode(rn);
break;
case CSIO_RNFE_LOGO_RECV:
csio_set_state(&rn->sm, csio_rns_offline);
__csio_unreg_rnode(rn);
break;
case CSIO_RNFE_CLOSE:
csio_set_state(&rn->sm, csio_rns_uninit);
__csio_unreg_rnode(rn);
break;
case CSIO_RNFE_NAME_MISSING:
csio_set_state(&rn->sm, csio_rns_disappeared);
__csio_unreg_rnode(rn);
break;
default:
csio_ln_dbg(ln,
"ssni:x%x unexp event %d recv from did:x%x "
"in rn state[uninit]\n", csio_rn_flowid(rn), evt,
rn->nport_id);
CSIO_INC_STATS(rn, n_evt_unexp);
break;
}
}
static void
csio_rns_offline(struct csio_rnode *rn, enum csio_rn_ev evt)
{
struct csio_lnode *ln = csio_rnode_to_lnode(rn);
int ret = 0;
CSIO_INC_STATS(rn, n_evt_sm[evt]);
switch (evt) {
case CSIO_RNFE_LOGGED_IN:
case CSIO_RNFE_PLOGI_RECV:
ret = csio_rn_verify_rparams(ln, rn, rn->rdev_entry);
if (!ret) {
csio_set_state(&rn->sm, csio_rns_ready);
__csio_reg_rnode(rn);
} else {
CSIO_INC_STATS(rn, n_err_inval);
csio_post_event(&rn->sm, CSIO_RNFE_CLOSE);
}
break;
case CSIO_RNFE_DOWN:
csio_ln_dbg(ln,
"ssni:x%x Ignoring event %d recv from did:x%x "
"in rn state[offline]\n", csio_rn_flowid(rn), evt,
rn->nport_id);
CSIO_INC_STATS(rn, n_evt_drop);
break;
case CSIO_RNFE_CLOSE:
csio_set_state(&rn->sm, csio_rns_uninit);
break;
case CSIO_RNFE_NAME_MISSING:
csio_set_state(&rn->sm, csio_rns_disappeared);
break;
default:
csio_ln_dbg(ln,
"ssni:x%x unexp event %d recv from did:x%x "
"in rn state[offline]\n", csio_rn_flowid(rn), evt,
rn->nport_id);
CSIO_INC_STATS(rn, n_evt_unexp);
break;
}
}
static void
csio_rns_disappeared(struct csio_rnode *rn, enum csio_rn_ev evt)
{
struct csio_lnode *ln = csio_rnode_to_lnode(rn);
int ret = 0;
CSIO_INC_STATS(rn, n_evt_sm[evt]);
switch (evt) {
case CSIO_RNFE_LOGGED_IN:
case CSIO_RNFE_PLOGI_RECV:
ret = csio_rn_verify_rparams(ln, rn, rn->rdev_entry);
if (!ret) {
csio_set_state(&rn->sm, csio_rns_ready);
__csio_reg_rnode(rn);
} else {
CSIO_INC_STATS(rn, n_err_inval);
csio_post_event(&rn->sm, CSIO_RNFE_CLOSE);
}
break;
case CSIO_RNFE_CLOSE:
csio_set_state(&rn->sm, csio_rns_uninit);
break;
case CSIO_RNFE_DOWN:
case CSIO_RNFE_NAME_MISSING:
csio_ln_dbg(ln,
"ssni:x%x Ignoring event %d recv from did x%x"
"in rn state[disappeared]\n", csio_rn_flowid(rn),
evt, rn->nport_id);
break;
default:
csio_ln_dbg(ln,
"ssni:x%x unexp event %d recv from did x%x"
"in rn state[disappeared]\n", csio_rn_flowid(rn),
evt, rn->nport_id);
CSIO_INC_STATS(rn, n_evt_unexp);
break;
}
}
void
csio_rnode_devloss_handler(struct csio_rnode *rn)
{
struct csio_lnode *ln = csio_rnode_to_lnode(rn);
if (csio_is_rnode_ready(rn))
return;
csio_post_event(&rn->sm, CSIO_RNFE_CLOSE);
if (csio_is_rnode_uninit(rn))
csio_put_rnode(ln, rn);
}
void
csio_rnode_fwevt_handler(struct csio_rnode *rn, uint8_t fwevt)
{
struct csio_lnode *ln = csio_rnode_to_lnode(rn);
enum csio_rn_ev evt;
evt = CSIO_FWE_TO_RNFE(fwevt);
if (!evt) {
csio_ln_err(ln, "ssni:x%x Unhandled FW Rdev event: %d\n",
csio_rn_flowid(rn), fwevt);
CSIO_INC_STATS(rn, n_evt_unexp);
return;
}
CSIO_INC_STATS(rn, n_evt_fw[fwevt]);
rn->prev_evt = rn->cur_evt;
rn->cur_evt = fwevt;
csio_post_event(&rn->sm, evt);
if (csio_is_rnode_uninit(rn))
csio_put_rnode(ln, rn);
}
static int
csio_rnode_init(struct csio_rnode *rn, struct csio_lnode *ln)
{
csio_rnode_to_lnode(rn) = ln;
csio_init_state(&rn->sm, csio_rns_uninit);
INIT_LIST_HEAD(&rn->host_cmpl_q);
csio_rn_flowid(rn) = CSIO_INVALID_IDX;
list_add_tail(&rn->sm.sm_list, &ln->rnhead);
return 0;
}
static void
csio_rnode_exit(struct csio_rnode *rn)
{
list_del_init(&rn->sm.sm_list);
CSIO_DB_ASSERT(list_empty(&rn->host_cmpl_q));
}
