BIO_METHOD *BIO_f_linebuffer(void)
{
return(&methods_linebuffer);
}
static int linebuffer_new(BIO *bi)
{
BIO_LINEBUFFER_CTX *ctx;
ctx=(BIO_LINEBUFFER_CTX *)OPENSSL_malloc(sizeof(BIO_LINEBUFFER_CTX));
if (ctx == NULL) return(0);
ctx->obuf=(char *)OPENSSL_malloc(DEFAULT_LINEBUFFER_SIZE);
if (ctx->obuf == NULL) { OPENSSL_free(ctx); return(0); }
ctx->obuf_size=DEFAULT_LINEBUFFER_SIZE;
ctx->obuf_len=0;
bi->init=1;
bi->ptr=(char *)ctx;
bi->flags=0;
return(1);
}
static int linebuffer_free(BIO *a)
{
BIO_LINEBUFFER_CTX *b;
if (a == NULL) return(0);
b=(BIO_LINEBUFFER_CTX *)a->ptr;
if (b->obuf != NULL) OPENSSL_free(b->obuf);
OPENSSL_free(a->ptr);
a->ptr=NULL;
a->init=0;
a->flags=0;
return(1);
}
static int linebuffer_read(BIO *b, char *out, int outl)
{
int ret=0;
if (out == NULL) return(0);
if (b->next_bio == NULL) return(0);
ret=BIO_read(b->next_bio,out,outl);
BIO_clear_retry_flags(b);
BIO_copy_next_retry(b);
return(ret);
}
static int linebuffer_write(BIO *b, const char *in, int inl)
{
int i,num=0,foundnl;
BIO_LINEBUFFER_CTX *ctx;
if ((in == NULL) || (inl <= 0)) return(0);
ctx=(BIO_LINEBUFFER_CTX *)b->ptr;
if ((ctx == NULL) || (b->next_bio == NULL)) return(0);
BIO_clear_retry_flags(b);
do
{
const char *p;
for(p = in; p < in + inl && *p != '\n'; p++)
;
if (*p == '\n')
{
p++;
foundnl = 1;
}
else
foundnl = 0;
while ((foundnl || p - in > ctx->obuf_size - ctx->obuf_len)
&& ctx->obuf_len > 0)
{
int orig_olen = ctx->obuf_len;
i = ctx->obuf_size - ctx->obuf_len;
if (p - in > 0)
{
if (i >= p - in)
{
memcpy(&(ctx->obuf[ctx->obuf_len]),
in,p - in);
ctx->obuf_len += p - in;
inl -= p - in;
num += p - in;
in = p;
}
else
{
memcpy(&(ctx->obuf[ctx->obuf_len]),
in,i);
ctx->obuf_len += i;
inl -= i;
in += i;
num += i;
}
}
#if 0
BIO_write(b->next_bio, "<*<", 3);
#endif
i=BIO_write(b->next_bio,
ctx->obuf, ctx->obuf_len);
if (i <= 0)
{
ctx->obuf_len = orig_olen;
BIO_copy_next_retry(b);
#if 0
BIO_write(b->next_bio, ">*>", 3);
#endif
if (i < 0) return((num > 0)?num:i);
if (i == 0) return(num);
}
#if 0
BIO_write(b->next_bio, ">*>", 3);
#endif
if (i < ctx->obuf_len)
memmove(ctx->obuf, ctx->obuf + i,
ctx->obuf_len - i);
ctx->obuf_len-=i;
}
if ((foundnl || p - in > ctx->obuf_size) && p - in > 0)
{
#if 0
BIO_write(b->next_bio, "<*<", 3);
#endif
i=BIO_write(b->next_bio,in,p - in);
if (i <= 0)
{
BIO_copy_next_retry(b);
#if 0
BIO_write(b->next_bio, ">*>", 3);
#endif
if (i < 0) return((num > 0)?num:i);
if (i == 0) return(num);
}
#if 0
BIO_write(b->next_bio, ">*>", 3);
#endif
num+=i;
in+=i;
inl-=i;
}
}
while(foundnl && inl > 0);
if (inl > 0)
{
memcpy(&(ctx->obuf[ctx->obuf_len]), in, inl);
ctx->obuf_len += inl;
num += inl;
}
return num;
}
static long linebuffer_ctrl(BIO *b, int cmd, long num, void *ptr)
{
BIO *dbio;
BIO_LINEBUFFER_CTX *ctx;
long ret=1;
char *p;
int r;
int obs;
ctx=(BIO_LINEBUFFER_CTX *)b->ptr;
switch (cmd)
{
case BIO_CTRL_RESET:
ctx->obuf_len=0;
if (b->next_bio == NULL) return(0);
ret=BIO_ctrl(b->next_bio,cmd,num,ptr);
break;
case BIO_CTRL_INFO:
ret=(long)ctx->obuf_len;
break;
case BIO_CTRL_WPENDING:
ret=(long)ctx->obuf_len;
if (ret == 0)
{
if (b->next_bio == NULL) return(0);
ret=BIO_ctrl(b->next_bio,cmd,num,ptr);
}
break;
case BIO_C_SET_BUFF_SIZE:
obs=(int)num;
p=ctx->obuf;
if ((obs > DEFAULT_LINEBUFFER_SIZE) && (obs != ctx->obuf_size))
{
p=(char *)OPENSSL_malloc((int)num);
if (p == NULL)
goto malloc_error;
}
if (ctx->obuf != p)
{
if (ctx->obuf_len > obs)
{
ctx->obuf_len = obs;
}
memcpy(p, ctx->obuf, ctx->obuf_len);
OPENSSL_free(ctx->obuf);
ctx->obuf=p;
ctx->obuf_size=obs;
}
break;
case BIO_C_DO_STATE_MACHINE:
if (b->next_bio == NULL) return(0);
BIO_clear_retry_flags(b);
ret=BIO_ctrl(b->next_bio,cmd,num,ptr);
BIO_copy_next_retry(b);
break;
case BIO_CTRL_FLUSH:
if (b->next_bio == NULL) return(0);
if (ctx->obuf_len <= 0)
{
ret=BIO_ctrl(b->next_bio,cmd,num,ptr);
break;
}
for (;;)
{
BIO_clear_retry_flags(b);
if (ctx->obuf_len > 0)
{
r=BIO_write(b->next_bio,
ctx->obuf, ctx->obuf_len);
#if 0
fprintf(stderr,"FLUSH %3d -> %3d\n",ctx->obuf_len,r);
#endif
BIO_copy_next_retry(b);
if (r <= 0) return((long)r);
if (r < ctx->obuf_len)
memmove(ctx->obuf, ctx->obuf + r,
ctx->obuf_len - r);
ctx->obuf_len-=r;
}
else
{
ctx->obuf_len=0;
ret=1;
break;
}
}
ret=BIO_ctrl(b->next_bio,cmd,num,ptr);
break;
case BIO_CTRL_DUP:
dbio=(BIO *)ptr;
if ( !BIO_set_write_buffer_size(dbio,ctx->obuf_size))
ret=0;
break;
default:
if (b->next_bio == NULL) return(0);
ret=BIO_ctrl(b->next_bio,cmd,num,ptr);
break;
}
return(ret);
malloc_error:
BIOerr(BIO_F_LINEBUFFER_CTRL,ERR_R_MALLOC_FAILURE);
return(0);
}
static long linebuffer_callback_ctrl(BIO *b, int cmd, bio_info_cb *fp)
{
long ret=1;
if (b->next_bio == NULL) return(0);
switch (cmd)
{
default:
ret=BIO_callback_ctrl(b->next_bio,cmd,fp);
break;
}
return(ret);
}
static int linebuffer_gets(BIO *b, char *buf, int size)
{
if (b->next_bio == NULL) return(0);
return(BIO_gets(b->next_bio,buf,size));
}
static int linebuffer_puts(BIO *b, const char *str)
{
return(linebuffer_write(b,str,strlen(str)));
}
