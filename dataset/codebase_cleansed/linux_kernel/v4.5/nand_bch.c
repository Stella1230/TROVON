int nand_bch_calculate_ecc(struct mtd_info *mtd, const unsigned char *buf,
unsigned char *code)
{
const struct nand_chip *chip = mtd_to_nand(mtd);
struct nand_bch_control *nbc = chip->ecc.priv;
unsigned int i;
memset(code, 0, chip->ecc.bytes);
encode_bch(nbc->bch, buf, chip->ecc.size, code);
for (i = 0; i < chip->ecc.bytes; i++)
code[i] ^= nbc->eccmask[i];
return 0;
}
int nand_bch_correct_data(struct mtd_info *mtd, unsigned char *buf,
unsigned char *read_ecc, unsigned char *calc_ecc)
{
const struct nand_chip *chip = mtd_to_nand(mtd);
struct nand_bch_control *nbc = chip->ecc.priv;
unsigned int *errloc = nbc->errloc;
int i, count;
count = decode_bch(nbc->bch, NULL, chip->ecc.size, read_ecc, calc_ecc,
NULL, errloc);
if (count > 0) {
for (i = 0; i < count; i++) {
if (errloc[i] < (chip->ecc.size*8))
buf[errloc[i] >> 3] ^= (1 << (errloc[i] & 7));
pr_debug("%s: corrected bitflip %u\n", __func__,
errloc[i]);
}
} else if (count < 0) {
printk(KERN_ERR "ecc unrecoverable error\n");
count = -EBADMSG;
}
return count;
}
struct nand_bch_control *
nand_bch_init(struct mtd_info *mtd, unsigned int eccsize, unsigned int eccbytes,
struct nand_ecclayout **ecclayout)
{
unsigned int m, t, eccsteps, i;
struct nand_ecclayout *layout;
struct nand_bch_control *nbc = NULL;
unsigned char *erased_page;
if (!eccsize || !eccbytes) {
printk(KERN_WARNING "ecc parameters not supplied\n");
goto fail;
}
m = fls(1+8*eccsize);
t = (eccbytes*8)/m;
nbc = kzalloc(sizeof(*nbc), GFP_KERNEL);
if (!nbc)
goto fail;
nbc->bch = init_bch(m, t, 0);
if (!nbc->bch)
goto fail;
if (nbc->bch->ecc_bytes != eccbytes) {
printk(KERN_WARNING "invalid eccbytes %u, should be %u\n",
eccbytes, nbc->bch->ecc_bytes);
goto fail;
}
eccsteps = mtd->writesize/eccsize;
if (!*ecclayout) {
if (mtd->oobsize < 64) {
printk(KERN_WARNING "must provide an oob scheme for "
"oobsize %d\n", mtd->oobsize);
goto fail;
}
layout = &nbc->ecclayout;
layout->eccbytes = eccsteps*eccbytes;
if (layout->eccbytes+2 > mtd->oobsize) {
printk(KERN_WARNING "no suitable oob scheme available "
"for oobsize %d eccbytes %u\n", mtd->oobsize,
eccbytes);
goto fail;
}
for (i = 0; i < layout->eccbytes; i++)
layout->eccpos[i] = mtd->oobsize-layout->eccbytes+i;
layout->oobfree[0].offset = 2;
layout->oobfree[0].length = mtd->oobsize-2-layout->eccbytes;
*ecclayout = layout;
}
if (8*(eccsize+eccbytes) >= (1 << m)) {
printk(KERN_WARNING "eccsize %u is too large\n", eccsize);
goto fail;
}
if ((*ecclayout)->eccbytes != (eccsteps*eccbytes)) {
printk(KERN_WARNING "invalid ecc layout\n");
goto fail;
}
nbc->eccmask = kmalloc(eccbytes, GFP_KERNEL);
nbc->errloc = kmalloc(t*sizeof(*nbc->errloc), GFP_KERNEL);
if (!nbc->eccmask || !nbc->errloc)
goto fail;
erased_page = kmalloc(eccsize, GFP_KERNEL);
if (!erased_page)
goto fail;
memset(erased_page, 0xff, eccsize);
memset(nbc->eccmask, 0, eccbytes);
encode_bch(nbc->bch, erased_page, eccsize, nbc->eccmask);
kfree(erased_page);
for (i = 0; i < eccbytes; i++)
nbc->eccmask[i] ^= 0xff;
return nbc;
fail:
nand_bch_free(nbc);
return NULL;
}
void nand_bch_free(struct nand_bch_control *nbc)
{
if (nbc) {
free_bch(nbc->bch);
kfree(nbc->errloc);
kfree(nbc->eccmask);
kfree(nbc);
}
}
