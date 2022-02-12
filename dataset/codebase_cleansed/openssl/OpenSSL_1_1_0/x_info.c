X509_INFO *X509_INFO_new(void)
{
X509_INFO *ret;
ret = OPENSSL_zalloc(sizeof(*ret));
if (ret == NULL) {
ASN1err(ASN1_F_X509_INFO_NEW, ERR_R_MALLOC_FAILURE);
return NULL;
}
return ret;
}
void X509_INFO_free(X509_INFO *x)
{
if (x == NULL)
return;
X509_free(x->x509);
X509_CRL_free(x->crl);
X509_PKEY_free(x->x_pkey);
OPENSSL_free(x->enc_data);
OPENSSL_free(x);
}
