void OPENSSL_cleanse(void *ptr, size_t len)
{
unsigned char *p = ptr;
size_t loop = len;
while(loop--)
{
*(p++) = cleanse_ctr;
cleanse_ctr += (17 + (unsigned char)((unsigned long)p & 0xF));
}
if(memchr(ptr, cleanse_ctr, len))
cleanse_ctr += 63;
}
