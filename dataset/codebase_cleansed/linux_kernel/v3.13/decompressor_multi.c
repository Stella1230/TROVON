int squashfs_max_decompressors(void)
{
return MAX_DECOMPRESSOR;
}
static void put_decomp_stream(struct decomp_stream *decomp_strm,
struct squashfs_stream *stream)
{
mutex_lock(&stream->mutex);
list_add(&decomp_strm->list, &stream->strm_list);
mutex_unlock(&stream->mutex);
wake_up(&stream->wait);
}
void *squashfs_decompressor_create(struct squashfs_sb_info *msblk,
void *comp_opts)
{
struct squashfs_stream *stream;
struct decomp_stream *decomp_strm = NULL;
int err = -ENOMEM;
stream = kzalloc(sizeof(*stream), GFP_KERNEL);
if (!stream)
goto out;
stream->comp_opts = comp_opts;
mutex_init(&stream->mutex);
INIT_LIST_HEAD(&stream->strm_list);
init_waitqueue_head(&stream->wait);
decomp_strm = kmalloc(sizeof(*decomp_strm), GFP_KERNEL);
if (!decomp_strm)
goto out;
decomp_strm->stream = msblk->decompressor->init(msblk,
stream->comp_opts);
if (IS_ERR(decomp_strm->stream)) {
err = PTR_ERR(decomp_strm->stream);
goto out;
}
list_add(&decomp_strm->list, &stream->strm_list);
stream->avail_decomp = 1;
return stream;
out:
kfree(decomp_strm);
kfree(stream);
return ERR_PTR(err);
}
void squashfs_decompressor_destroy(struct squashfs_sb_info *msblk)
{
struct squashfs_stream *stream = msblk->stream;
if (stream) {
struct decomp_stream *decomp_strm;
while (!list_empty(&stream->strm_list)) {
decomp_strm = list_entry(stream->strm_list.prev,
struct decomp_stream, list);
list_del(&decomp_strm->list);
msblk->decompressor->free(decomp_strm->stream);
kfree(decomp_strm);
stream->avail_decomp--;
}
WARN_ON(stream->avail_decomp);
kfree(stream->comp_opts);
kfree(stream);
}
}
static struct decomp_stream *get_decomp_stream(struct squashfs_sb_info *msblk,
struct squashfs_stream *stream)
{
struct decomp_stream *decomp_strm;
while (1) {
mutex_lock(&stream->mutex);
if (!list_empty(&stream->strm_list)) {
decomp_strm = list_entry(stream->strm_list.prev,
struct decomp_stream, list);
list_del(&decomp_strm->list);
mutex_unlock(&stream->mutex);
break;
}
if (stream->avail_decomp >= MAX_DECOMPRESSOR)
goto wait;
decomp_strm = kmalloc(sizeof(*decomp_strm), GFP_KERNEL);
if (!decomp_strm)
goto wait;
decomp_strm->stream = msblk->decompressor->init(msblk,
stream->comp_opts);
if (IS_ERR(decomp_strm->stream)) {
kfree(decomp_strm);
goto wait;
}
stream->avail_decomp++;
WARN_ON(stream->avail_decomp > MAX_DECOMPRESSOR);
mutex_unlock(&stream->mutex);
break;
wait:
mutex_unlock(&stream->mutex);
wait_event(stream->wait,
!list_empty(&stream->strm_list));
}
return decomp_strm;
}
int squashfs_decompress(struct squashfs_sb_info *msblk, struct buffer_head **bh,
int b, int offset, int length, struct squashfs_page_actor *output)
{
int res;
struct squashfs_stream *stream = msblk->stream;
struct decomp_stream *decomp_stream = get_decomp_stream(msblk, stream);
res = msblk->decompressor->decompress(msblk, decomp_stream->stream,
bh, b, offset, length, output);
put_decomp_stream(decomp_stream, stream);
if (res < 0)
ERROR("%s decompression failed, data probably corrupt\n",
msblk->decompressor->name);
return res;
}
