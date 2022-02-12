char *nfs_path(char **p, struct dentry *dentry, char *buffer, ssize_t buflen,
unsigned flags)
{
char *end;
int namelen;
unsigned seq;
const char *base;
rename_retry:
end = buffer+buflen;
*--end = '\0';
buflen--;
seq = read_seqbegin(&rename_lock);
rcu_read_lock();
while (1) {
spin_lock(&dentry->d_lock);
if (IS_ROOT(dentry))
break;
namelen = dentry->d_name.len;
buflen -= namelen + 1;
if (buflen < 0)
goto Elong_unlock;
end -= namelen;
memcpy(end, dentry->d_name.name, namelen);
*--end = '/';
spin_unlock(&dentry->d_lock);
dentry = dentry->d_parent;
}
if (read_seqretry(&rename_lock, seq)) {
spin_unlock(&dentry->d_lock);
rcu_read_unlock();
goto rename_retry;
}
if ((flags & NFS_PATH_CANONICAL) && *end != '/') {
if (--buflen < 0) {
spin_unlock(&dentry->d_lock);
rcu_read_unlock();
goto Elong;
}
*--end = '/';
}
*p = end;
base = dentry->d_fsdata;
if (!base) {
spin_unlock(&dentry->d_lock);
rcu_read_unlock();
WARN_ON(1);
return end;
}
namelen = strlen(base);
if (*end == '/') {
while (namelen > 0 && base[namelen - 1] == '/')
namelen--;
}
buflen -= namelen;
if (buflen < 0) {
spin_unlock(&dentry->d_lock);
rcu_read_unlock();
goto Elong;
}
end -= namelen;
memcpy(end, base, namelen);
spin_unlock(&dentry->d_lock);
rcu_read_unlock();
return end;
Elong_unlock:
spin_unlock(&dentry->d_lock);
rcu_read_unlock();
if (read_seqretry(&rename_lock, seq))
goto rename_retry;
Elong:
return ERR_PTR(-ENAMETOOLONG);
}
struct vfsmount *nfs_d_automount(struct path *path)
{
struct vfsmount *mnt;
struct nfs_server *server = NFS_SERVER(d_inode(path->dentry));
struct nfs_fh *fh = NULL;
struct nfs_fattr *fattr = NULL;
if (IS_ROOT(path->dentry))
return ERR_PTR(-ESTALE);
mnt = ERR_PTR(-ENOMEM);
fh = nfs_alloc_fhandle();
fattr = nfs_alloc_fattr();
if (fh == NULL || fattr == NULL)
goto out;
mnt = server->nfs_client->rpc_ops->submount(server, path->dentry, fh, fattr);
if (IS_ERR(mnt))
goto out;
mntget(mnt);
mnt_set_expiry(mnt, &nfs_automount_list);
schedule_delayed_work(&nfs_automount_task, nfs_mountpoint_expiry_timeout);
out:
nfs_free_fattr(fattr);
nfs_free_fhandle(fh);
return mnt;
}
static int
nfs_namespace_getattr(const struct path *path, struct kstat *stat,
u32 request_mask, unsigned int query_flags)
{
if (NFS_FH(d_inode(path->dentry))->size != 0)
return nfs_getattr(path, stat, request_mask, query_flags);
generic_fillattr(d_inode(path->dentry), stat);
return 0;
}
static int
nfs_namespace_setattr(struct dentry *dentry, struct iattr *attr)
{
if (NFS_FH(d_inode(dentry))->size != 0)
return nfs_setattr(dentry, attr);
return -EACCES;
}
static void nfs_expire_automounts(struct work_struct *work)
{
struct list_head *list = &nfs_automount_list;
mark_mounts_for_expiry(list);
if (!list_empty(list))
schedule_delayed_work(&nfs_automount_task, nfs_mountpoint_expiry_timeout);
}
void nfs_release_automount_timer(void)
{
if (list_empty(&nfs_automount_list))
cancel_delayed_work(&nfs_automount_task);
}
static struct vfsmount *nfs_do_clone_mount(struct nfs_server *server,
const char *devname,
struct nfs_clone_mount *mountdata)
{
return vfs_submount(mountdata->dentry, &nfs_xdev_fs_type, devname, mountdata);
}
struct vfsmount *nfs_do_submount(struct dentry *dentry, struct nfs_fh *fh,
struct nfs_fattr *fattr, rpc_authflavor_t authflavor)
{
struct nfs_clone_mount mountdata = {
.sb = dentry->d_sb,
.dentry = dentry,
.fh = fh,
.fattr = fattr,
.authflavor = authflavor,
};
struct vfsmount *mnt;
char *page = (char *) __get_free_page(GFP_USER);
char *devname;
if (page == NULL)
return ERR_PTR(-ENOMEM);
devname = nfs_devname(dentry, page, PAGE_SIZE);
if (IS_ERR(devname))
mnt = ERR_CAST(devname);
else
mnt = nfs_do_clone_mount(NFS_SB(dentry->d_sb), devname, &mountdata);
free_page((unsigned long)page);
return mnt;
}
struct vfsmount *nfs_submount(struct nfs_server *server, struct dentry *dentry,
struct nfs_fh *fh, struct nfs_fattr *fattr)
{
int err;
struct dentry *parent = dget_parent(dentry);
err = server->nfs_client->rpc_ops->lookup(d_inode(parent), &dentry->d_name, fh, fattr, NULL);
dput(parent);
if (err != 0)
return ERR_PTR(err);
return nfs_do_submount(dentry, fh, fattr, server->client->cl_auth->au_flavor);
}
