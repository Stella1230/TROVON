EVP_PKEY *d2i_PublicKey(int type, EVP_PKEY **a, unsigned char **pp,
long length)
{
EVP_PKEY *ret;
if ((a == NULL) || (*a == NULL))
{
if ((ret=EVP_PKEY_new()) == NULL)
{
ASN1err(ASN1_F_D2I_PUBLICKEY,ERR_R_EVP_LIB);
return(NULL);
}
}
else ret= *a;
ret->save_type=type;
ret->type=EVP_PKEY_type(type);
switch (ret->type)
{
#ifndef NO_RSA
case EVP_PKEY_RSA:
if ((ret->pkey.rsa=d2i_RSAPublicKey(NULL,pp,length)) == NULL)
{
ASN1err(ASN1_F_D2I_PUBLICKEY,ERR_R_ASN1_LIB);
goto err;
}
break;
#endif
#ifndef NO_DSA
case EVP_PKEY_DSA:
if ((ret->pkey.dsa=d2i_DSAPublicKey(NULL,pp,length)) == NULL)
{
ASN1err(ASN1_F_D2I_PUBLICKEY,ERR_R_ASN1_LIB);
goto err;
}
break;
#endif
default:
ASN1err(ASN1_F_D2I_PUBLICKEY,ASN1_R_UNKNOWN_PUBLIC_KEY_TYPE);
goto err;
}
if (a != NULL) (*a)=ret;
return(ret);
err:
if ((ret != NULL) && ((a == NULL) || (*a != ret))) EVP_PKEY_free(ret);
return(NULL);
}
