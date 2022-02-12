int jffs2_initxattrs(struct inode *inode, const struct xattr *xattr_array,
void *fs_info)
{
const struct xattr *xattr;
int err = 0;
for (xattr = xattr_array; xattr->name != NULL; xattr++) {
err = do_jffs2_setxattr(inode, JFFS2_XPREFIX_SECURITY,
xattr->name, xattr->value,
xattr->value_len, 0);
if (err < 0)
break;
}
return err;
}
int jffs2_init_security(struct inode *inode, struct inode *dir,
const struct qstr *qstr)
{
return security_inode_init_security(inode, dir, qstr,
&jffs2_initxattrs, NULL);
}
static int jffs2_security_getxattr(struct dentry *dentry, const char *name,
void *buffer, size_t size, int type)
{
if (!strcmp(name, ""))
return -EINVAL;
return do_jffs2_getxattr(dentry->d_inode, JFFS2_XPREFIX_SECURITY,
name, buffer, size);
}
static int jffs2_security_setxattr(struct dentry *dentry, const char *name,
const void *buffer, size_t size, int flags, int type)
{
if (!strcmp(name, ""))
return -EINVAL;
return do_jffs2_setxattr(dentry->d_inode, JFFS2_XPREFIX_SECURITY,
name, buffer, size, flags);
}
static size_t jffs2_security_listxattr(struct dentry *dentry, char *list,
size_t list_size, const char *name, size_t name_len, int type)
{
size_t retlen = XATTR_SECURITY_PREFIX_LEN + name_len + 1;
if (list && retlen <= list_size) {
strcpy(list, XATTR_SECURITY_PREFIX);
strcpy(list + XATTR_SECURITY_PREFIX_LEN, name);
}
return retlen;
}
