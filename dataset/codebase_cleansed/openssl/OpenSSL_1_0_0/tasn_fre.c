void ASN1_item_free(ASN1_VALUE *val, const ASN1_ITEM *it)
{
asn1_item_combine_free(&val, it, 0);
}
void ASN1_item_ex_free(ASN1_VALUE **pval, const ASN1_ITEM *it)
{
asn1_item_combine_free(pval, it, 0);
}
static void asn1_item_combine_free(ASN1_VALUE **pval, const ASN1_ITEM *it, int combine)
{
const ASN1_TEMPLATE *tt = NULL, *seqtt;
const ASN1_EXTERN_FUNCS *ef;
const ASN1_COMPAT_FUNCS *cf;
const ASN1_AUX *aux = it->funcs;
ASN1_aux_cb *asn1_cb;
int i;
if (!pval)
return;
if ((it->itype != ASN1_ITYPE_PRIMITIVE) && !*pval)
return;
if (aux && aux->asn1_cb)
asn1_cb = aux->asn1_cb;
else
asn1_cb = 0;
switch(it->itype)
{
case ASN1_ITYPE_PRIMITIVE:
if (it->templates)
ASN1_template_free(pval, it->templates);
else
ASN1_primitive_free(pval, it);
break;
case ASN1_ITYPE_MSTRING:
ASN1_primitive_free(pval, it);
break;
case ASN1_ITYPE_CHOICE:
if (asn1_cb)
{
i = asn1_cb(ASN1_OP_FREE_PRE, pval, it, NULL);
if (i == 2)
return;
}
i = asn1_get_choice_selector(pval, it);
if ((i >= 0) && (i < it->tcount))
{
ASN1_VALUE **pchval;
tt = it->templates + i;
pchval = asn1_get_field_ptr(pval, tt);
ASN1_template_free(pchval, tt);
}
if (asn1_cb)
asn1_cb(ASN1_OP_FREE_POST, pval, it, NULL);
if (!combine)
{
OPENSSL_free(*pval);
*pval = NULL;
}
break;
case ASN1_ITYPE_COMPAT:
cf = it->funcs;
if (cf && cf->asn1_free)
cf->asn1_free(*pval);
break;
case ASN1_ITYPE_EXTERN:
ef = it->funcs;
if (ef && ef->asn1_ex_free)
ef->asn1_ex_free(pval, it);
break;
case ASN1_ITYPE_NDEF_SEQUENCE:
case ASN1_ITYPE_SEQUENCE:
if (asn1_do_lock(pval, -1, it) > 0)
return;
if (asn1_cb)
{
i = asn1_cb(ASN1_OP_FREE_PRE, pval, it, NULL);
if (i == 2)
return;
}
asn1_enc_free(pval, it);
tt = it->templates + it->tcount - 1;
for (i = 0; i < it->tcount; tt--, i++)
{
ASN1_VALUE **pseqval;
seqtt = asn1_do_adb(pval, tt, 0);
if (!seqtt)
continue;
pseqval = asn1_get_field_ptr(pval, seqtt);
ASN1_template_free(pseqval, seqtt);
}
if (asn1_cb)
asn1_cb(ASN1_OP_FREE_POST, pval, it, NULL);
if (!combine)
{
OPENSSL_free(*pval);
*pval = NULL;
}
break;
}
}
void ASN1_template_free(ASN1_VALUE **pval, const ASN1_TEMPLATE *tt)
{
int i;
if (tt->flags & ASN1_TFLG_SK_MASK)
{
STACK_OF(ASN1_VALUE) *sk = (STACK_OF(ASN1_VALUE) *)*pval;
for (i = 0; i < sk_ASN1_VALUE_num(sk); i++)
{
ASN1_VALUE *vtmp;
vtmp = sk_ASN1_VALUE_value(sk, i);
asn1_item_combine_free(&vtmp, ASN1_ITEM_ptr(tt->item),
0);
}
sk_ASN1_VALUE_free(sk);
*pval = NULL;
}
else
asn1_item_combine_free(pval, ASN1_ITEM_ptr(tt->item),
tt->flags & ASN1_TFLG_COMBINE);
}
void ASN1_primitive_free(ASN1_VALUE **pval, const ASN1_ITEM *it)
{
int utype;
if (it)
{
const ASN1_PRIMITIVE_FUNCS *pf;
pf = it->funcs;
if (pf && pf->prim_free)
{
pf->prim_free(pval, it);
return;
}
}
if (!it)
{
ASN1_TYPE *typ = (ASN1_TYPE *)*pval;
utype = typ->type;
pval = &typ->value.asn1_value;
if (!*pval)
return;
}
else if (it->itype == ASN1_ITYPE_MSTRING)
{
utype = -1;
if (!*pval)
return;
}
else
{
utype = it->utype;
if ((utype != V_ASN1_BOOLEAN) && !*pval)
return;
}
switch(utype)
{
case V_ASN1_OBJECT:
ASN1_OBJECT_free((ASN1_OBJECT *)*pval);
break;
case V_ASN1_BOOLEAN:
if (it)
*(ASN1_BOOLEAN *)pval = it->size;
else
*(ASN1_BOOLEAN *)pval = -1;
return;
case V_ASN1_NULL:
break;
case V_ASN1_ANY:
ASN1_primitive_free(pval, NULL);
OPENSSL_free(*pval);
break;
default:
ASN1_STRING_free((ASN1_STRING *)*pval);
*pval = NULL;
break;
}
*pval = NULL;
}
