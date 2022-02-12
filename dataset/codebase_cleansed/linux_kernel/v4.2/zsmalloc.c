static int create_handle_cache(struct zs_pool *pool)
{
pool->handle_cachep = kmem_cache_create("zs_handle", ZS_HANDLE_SIZE,
0, 0, NULL);
return pool->handle_cachep ? 0 : 1;
}
static void destroy_handle_cache(struct zs_pool *pool)
{
if (pool->handle_cachep)
kmem_cache_destroy(pool->handle_cachep);
}
static unsigned long alloc_handle(struct zs_pool *pool)
{
return (unsigned long)kmem_cache_alloc(pool->handle_cachep,
pool->flags & ~__GFP_HIGHMEM);
}
static void free_handle(struct zs_pool *pool, unsigned long handle)
{
kmem_cache_free(pool->handle_cachep, (void *)handle);
}
static void record_obj(unsigned long handle, unsigned long obj)
{
*(unsigned long *)handle = obj;
}
static void *zs_zpool_create(char *name, gfp_t gfp, struct zpool_ops *zpool_ops,
struct zpool *zpool)
{
return zs_create_pool(name, gfp);
}
static void zs_zpool_destroy(void *pool)
{
zs_destroy_pool(pool);
}
static int zs_zpool_malloc(void *pool, size_t size, gfp_t gfp,
unsigned long *handle)
{
*handle = zs_malloc(pool, size);
return *handle ? 0 : -1;
}
static void zs_zpool_free(void *pool, unsigned long handle)
{
zs_free(pool, handle);
}
static int zs_zpool_shrink(void *pool, unsigned int pages,
unsigned int *reclaimed)
{
return -EINVAL;
}
static void *zs_zpool_map(void *pool, unsigned long handle,
enum zpool_mapmode mm)
{
enum zs_mapmode zs_mm;
switch (mm) {
case ZPOOL_MM_RO:
zs_mm = ZS_MM_RO;
break;
case ZPOOL_MM_WO:
zs_mm = ZS_MM_WO;
break;
case ZPOOL_MM_RW:
default:
zs_mm = ZS_MM_RW;
break;
}
return zs_map_object(pool, handle, zs_mm);
}
static void zs_zpool_unmap(void *pool, unsigned long handle)
{
zs_unmap_object(pool, handle);
}
static u64 zs_zpool_total_size(void *pool)
{
return zs_get_total_pages(pool) << PAGE_SHIFT;
}
static unsigned int get_maxobj_per_zspage(int size, int pages_per_zspage)
{
return pages_per_zspage * PAGE_SIZE / size;
}
static int is_first_page(struct page *page)
{
return PagePrivate(page);
}
static int is_last_page(struct page *page)
{
return PagePrivate2(page);
}
static void get_zspage_mapping(struct page *page, unsigned int *class_idx,
enum fullness_group *fullness)
{
unsigned long m;
BUG_ON(!is_first_page(page));
m = (unsigned long)page->mapping;
*fullness = m & FULLNESS_MASK;
*class_idx = (m >> FULLNESS_BITS) & CLASS_IDX_MASK;
}
static void set_zspage_mapping(struct page *page, unsigned int class_idx,
enum fullness_group fullness)
{
unsigned long m;
BUG_ON(!is_first_page(page));
m = ((class_idx & CLASS_IDX_MASK) << FULLNESS_BITS) |
(fullness & FULLNESS_MASK);
page->mapping = (struct address_space *)m;
}
static int get_size_class_index(int size)
{
int idx = 0;
if (likely(size > ZS_MIN_ALLOC_SIZE))
idx = DIV_ROUND_UP(size - ZS_MIN_ALLOC_SIZE,
ZS_SIZE_CLASS_DELTA);
return min(zs_size_classes - 1, idx);
}
static inline void zs_stat_inc(struct size_class *class,
enum zs_stat_type type, unsigned long cnt)
{
class->stats.objs[type] += cnt;
}
static inline void zs_stat_dec(struct size_class *class,
enum zs_stat_type type, unsigned long cnt)
{
class->stats.objs[type] -= cnt;
}
static inline unsigned long zs_stat_get(struct size_class *class,
enum zs_stat_type type)
{
return class->stats.objs[type];
}
static int __init zs_stat_init(void)
{
if (!debugfs_initialized())
return -ENODEV;
zs_stat_root = debugfs_create_dir("zsmalloc", NULL);
if (!zs_stat_root)
return -ENOMEM;
return 0;
}
static void __exit zs_stat_exit(void)
{
debugfs_remove_recursive(zs_stat_root);
}
static int zs_stats_size_show(struct seq_file *s, void *v)
{
int i;
struct zs_pool *pool = s->private;
struct size_class *class;
int objs_per_zspage;
unsigned long class_almost_full, class_almost_empty;
unsigned long obj_allocated, obj_used, pages_used;
unsigned long total_class_almost_full = 0, total_class_almost_empty = 0;
unsigned long total_objs = 0, total_used_objs = 0, total_pages = 0;
seq_printf(s, " %5s %5s %11s %12s %13s %10s %10s %16s\n",
"class", "size", "almost_full", "almost_empty",
"obj_allocated", "obj_used", "pages_used",
"pages_per_zspage");
for (i = 0; i < zs_size_classes; i++) {
class = pool->size_class[i];
if (class->index != i)
continue;
spin_lock(&class->lock);
class_almost_full = zs_stat_get(class, CLASS_ALMOST_FULL);
class_almost_empty = zs_stat_get(class, CLASS_ALMOST_EMPTY);
obj_allocated = zs_stat_get(class, OBJ_ALLOCATED);
obj_used = zs_stat_get(class, OBJ_USED);
spin_unlock(&class->lock);
objs_per_zspage = get_maxobj_per_zspage(class->size,
class->pages_per_zspage);
pages_used = obj_allocated / objs_per_zspage *
class->pages_per_zspage;
seq_printf(s, " %5u %5u %11lu %12lu %13lu %10lu %10lu %16d\n",
i, class->size, class_almost_full, class_almost_empty,
obj_allocated, obj_used, pages_used,
class->pages_per_zspage);
total_class_almost_full += class_almost_full;
total_class_almost_empty += class_almost_empty;
total_objs += obj_allocated;
total_used_objs += obj_used;
total_pages += pages_used;
}
seq_puts(s, "\n");
seq_printf(s, " %5s %5s %11lu %12lu %13lu %10lu %10lu\n",
"Total", "", total_class_almost_full,
total_class_almost_empty, total_objs,
total_used_objs, total_pages);
return 0;
}
static int zs_stats_size_open(struct inode *inode, struct file *file)
{
return single_open(file, zs_stats_size_show, inode->i_private);
}
static int zs_pool_stat_create(char *name, struct zs_pool *pool)
{
struct dentry *entry;
if (!zs_stat_root)
return -ENODEV;
entry = debugfs_create_dir(name, zs_stat_root);
if (!entry) {
pr_warn("debugfs dir <%s> creation failed\n", name);
return -ENOMEM;
}
pool->stat_dentry = entry;
entry = debugfs_create_file("classes", S_IFREG | S_IRUGO,
pool->stat_dentry, pool, &zs_stat_size_ops);
if (!entry) {
pr_warn("%s: debugfs file entry <%s> creation failed\n",
name, "classes");
return -ENOMEM;
}
return 0;
}
static void zs_pool_stat_destroy(struct zs_pool *pool)
{
debugfs_remove_recursive(pool->stat_dentry);
}
static inline void zs_stat_inc(struct size_class *class,
enum zs_stat_type type, unsigned long cnt)
{
}
static inline void zs_stat_dec(struct size_class *class,
enum zs_stat_type type, unsigned long cnt)
{
}
static inline unsigned long zs_stat_get(struct size_class *class,
enum zs_stat_type type)
{
return 0;
}
static int __init zs_stat_init(void)
{
return 0;
}
static void __exit zs_stat_exit(void)
{
}
static inline int zs_pool_stat_create(char *name, struct zs_pool *pool)
{
return 0;
}
static inline void zs_pool_stat_destroy(struct zs_pool *pool)
{
}
static enum fullness_group get_fullness_group(struct page *page)
{
int inuse, max_objects;
enum fullness_group fg;
BUG_ON(!is_first_page(page));
inuse = page->inuse;
max_objects = page->objects;
if (inuse == 0)
fg = ZS_EMPTY;
else if (inuse == max_objects)
fg = ZS_FULL;
else if (inuse <= 3 * max_objects / fullness_threshold_frac)
fg = ZS_ALMOST_EMPTY;
else
fg = ZS_ALMOST_FULL;
return fg;
}
static void insert_zspage(struct page *page, struct size_class *class,
enum fullness_group fullness)
{
struct page **head;
BUG_ON(!is_first_page(page));
if (fullness >= _ZS_NR_FULLNESS_GROUPS)
return;
head = &class->fullness_list[fullness];
if (*head)
list_add_tail(&page->lru, &(*head)->lru);
*head = page;
zs_stat_inc(class, fullness == ZS_ALMOST_EMPTY ?
CLASS_ALMOST_EMPTY : CLASS_ALMOST_FULL, 1);
}
static void remove_zspage(struct page *page, struct size_class *class,
enum fullness_group fullness)
{
struct page **head;
BUG_ON(!is_first_page(page));
if (fullness >= _ZS_NR_FULLNESS_GROUPS)
return;
head = &class->fullness_list[fullness];
BUG_ON(!*head);
if (list_empty(&(*head)->lru))
*head = NULL;
else if (*head == page)
*head = (struct page *)list_entry((*head)->lru.next,
struct page, lru);
list_del_init(&page->lru);
zs_stat_dec(class, fullness == ZS_ALMOST_EMPTY ?
CLASS_ALMOST_EMPTY : CLASS_ALMOST_FULL, 1);
}
static enum fullness_group fix_fullness_group(struct size_class *class,
struct page *page)
{
int class_idx;
enum fullness_group currfg, newfg;
BUG_ON(!is_first_page(page));
get_zspage_mapping(page, &class_idx, &currfg);
newfg = get_fullness_group(page);
if (newfg == currfg)
goto out;
remove_zspage(page, class, currfg);
insert_zspage(page, class, newfg);
set_zspage_mapping(page, class_idx, newfg);
out:
return newfg;
}
static int get_pages_per_zspage(int class_size)
{
int i, max_usedpc = 0;
int max_usedpc_order = 1;
for (i = 1; i <= ZS_MAX_PAGES_PER_ZSPAGE; i++) {
int zspage_size;
int waste, usedpc;
zspage_size = i * PAGE_SIZE;
waste = zspage_size % class_size;
usedpc = (zspage_size - waste) * 100 / zspage_size;
if (usedpc > max_usedpc) {
max_usedpc = usedpc;
max_usedpc_order = i;
}
}
return max_usedpc_order;
}
static struct page *get_first_page(struct page *page)
{
if (is_first_page(page))
return page;
else
return page->first_page;
}
static struct page *get_next_page(struct page *page)
{
struct page *next;
if (is_last_page(page))
next = NULL;
else if (is_first_page(page))
next = (struct page *)page_private(page);
else
next = list_entry(page->lru.next, struct page, lru);
return next;
}
static void *location_to_obj(struct page *page, unsigned long obj_idx)
{
unsigned long obj;
if (!page) {
BUG_ON(obj_idx);
return NULL;
}
obj = page_to_pfn(page) << OBJ_INDEX_BITS;
obj |= ((obj_idx) & OBJ_INDEX_MASK);
obj <<= OBJ_TAG_BITS;
return (void *)obj;
}
static void obj_to_location(unsigned long obj, struct page **page,
unsigned long *obj_idx)
{
obj >>= OBJ_TAG_BITS;
*page = pfn_to_page(obj >> OBJ_INDEX_BITS);
*obj_idx = (obj & OBJ_INDEX_MASK);
}
static unsigned long handle_to_obj(unsigned long handle)
{
return *(unsigned long *)handle;
}
static unsigned long obj_to_head(struct size_class *class, struct page *page,
void *obj)
{
if (class->huge) {
VM_BUG_ON(!is_first_page(page));
return *(unsigned long *)page_private(page);
} else
return *(unsigned long *)obj;
}
static unsigned long obj_idx_to_offset(struct page *page,
unsigned long obj_idx, int class_size)
{
unsigned long off = 0;
if (!is_first_page(page))
off = page->index;
return off + obj_idx * class_size;
}
static inline int trypin_tag(unsigned long handle)
{
unsigned long *ptr = (unsigned long *)handle;
return !test_and_set_bit_lock(HANDLE_PIN_BIT, ptr);
}
static void pin_tag(unsigned long handle)
{
while (!trypin_tag(handle));
}
static void unpin_tag(unsigned long handle)
{
unsigned long *ptr = (unsigned long *)handle;
clear_bit_unlock(HANDLE_PIN_BIT, ptr);
}
static void reset_page(struct page *page)
{
clear_bit(PG_private, &page->flags);
clear_bit(PG_private_2, &page->flags);
set_page_private(page, 0);
page->mapping = NULL;
page->freelist = NULL;
page_mapcount_reset(page);
}
static void free_zspage(struct page *first_page)
{
struct page *nextp, *tmp, *head_extra;
BUG_ON(!is_first_page(first_page));
BUG_ON(first_page->inuse);
head_extra = (struct page *)page_private(first_page);
reset_page(first_page);
__free_page(first_page);
if (!head_extra)
return;
list_for_each_entry_safe(nextp, tmp, &head_extra->lru, lru) {
list_del(&nextp->lru);
reset_page(nextp);
__free_page(nextp);
}
reset_page(head_extra);
__free_page(head_extra);
}
static void init_zspage(struct page *first_page, struct size_class *class)
{
unsigned long off = 0;
struct page *page = first_page;
BUG_ON(!is_first_page(first_page));
while (page) {
struct page *next_page;
struct link_free *link;
unsigned int i = 1;
void *vaddr;
if (page != first_page)
page->index = off;
vaddr = kmap_atomic(page);
link = (struct link_free *)vaddr + off / sizeof(*link);
while ((off += class->size) < PAGE_SIZE) {
link->next = location_to_obj(page, i++);
link += class->size / sizeof(*link);
}
next_page = get_next_page(page);
link->next = location_to_obj(next_page, 0);
kunmap_atomic(vaddr);
page = next_page;
off %= PAGE_SIZE;
}
}
static struct page *alloc_zspage(struct size_class *class, gfp_t flags)
{
int i, error;
struct page *first_page = NULL, *uninitialized_var(prev_page);
error = -ENOMEM;
for (i = 0; i < class->pages_per_zspage; i++) {
struct page *page;
page = alloc_page(flags);
if (!page)
goto cleanup;
INIT_LIST_HEAD(&page->lru);
if (i == 0) {
SetPagePrivate(page);
set_page_private(page, 0);
first_page = page;
first_page->inuse = 0;
}
if (i == 1)
set_page_private(first_page, (unsigned long)page);
if (i >= 1)
page->first_page = first_page;
if (i >= 2)
list_add(&page->lru, &prev_page->lru);
if (i == class->pages_per_zspage - 1)
SetPagePrivate2(page);
prev_page = page;
}
init_zspage(first_page, class);
first_page->freelist = location_to_obj(first_page, 0);
first_page->objects = class->pages_per_zspage * PAGE_SIZE / class->size;
error = 0;
cleanup:
if (unlikely(error) && first_page) {
free_zspage(first_page);
first_page = NULL;
}
return first_page;
}
static struct page *find_get_zspage(struct size_class *class)
{
int i;
struct page *page;
for (i = 0; i < _ZS_NR_FULLNESS_GROUPS; i++) {
page = class->fullness_list[i];
if (page)
break;
}
return page;
}
static inline int __zs_cpu_up(struct mapping_area *area)
{
if (area->vm)
return 0;
area->vm = alloc_vm_area(PAGE_SIZE * 2, NULL);
if (!area->vm)
return -ENOMEM;
return 0;
}
static inline void __zs_cpu_down(struct mapping_area *area)
{
if (area->vm)
free_vm_area(area->vm);
area->vm = NULL;
}
static inline void *__zs_map_object(struct mapping_area *area,
struct page *pages[2], int off, int size)
{
BUG_ON(map_vm_area(area->vm, PAGE_KERNEL, pages));
area->vm_addr = area->vm->addr;
return area->vm_addr + off;
}
static inline void __zs_unmap_object(struct mapping_area *area,
struct page *pages[2], int off, int size)
{
unsigned long addr = (unsigned long)area->vm_addr;
unmap_kernel_range(addr, PAGE_SIZE * 2);
}
static inline int __zs_cpu_up(struct mapping_area *area)
{
if (area->vm_buf)
return 0;
area->vm_buf = kmalloc(ZS_MAX_ALLOC_SIZE, GFP_KERNEL);
if (!area->vm_buf)
return -ENOMEM;
return 0;
}
static inline void __zs_cpu_down(struct mapping_area *area)
{
kfree(area->vm_buf);
area->vm_buf = NULL;
}
static void *__zs_map_object(struct mapping_area *area,
struct page *pages[2], int off, int size)
{
int sizes[2];
void *addr;
char *buf = area->vm_buf;
pagefault_disable();
if (area->vm_mm == ZS_MM_WO)
goto out;
sizes[0] = PAGE_SIZE - off;
sizes[1] = size - sizes[0];
addr = kmap_atomic(pages[0]);
memcpy(buf, addr + off, sizes[0]);
kunmap_atomic(addr);
addr = kmap_atomic(pages[1]);
memcpy(buf + sizes[0], addr, sizes[1]);
kunmap_atomic(addr);
out:
return area->vm_buf;
}
static void __zs_unmap_object(struct mapping_area *area,
struct page *pages[2], int off, int size)
{
int sizes[2];
void *addr;
char *buf;
if (area->vm_mm == ZS_MM_RO)
goto out;
buf = area->vm_buf;
if (!area->huge) {
buf = buf + ZS_HANDLE_SIZE;
size -= ZS_HANDLE_SIZE;
off += ZS_HANDLE_SIZE;
}
sizes[0] = PAGE_SIZE - off;
sizes[1] = size - sizes[0];
addr = kmap_atomic(pages[0]);
memcpy(addr + off, buf, sizes[0]);
kunmap_atomic(addr);
addr = kmap_atomic(pages[1]);
memcpy(addr, buf + sizes[0], sizes[1]);
kunmap_atomic(addr);
out:
pagefault_enable();
}
static int zs_cpu_notifier(struct notifier_block *nb, unsigned long action,
void *pcpu)
{
int ret, cpu = (long)pcpu;
struct mapping_area *area;
switch (action) {
case CPU_UP_PREPARE:
area = &per_cpu(zs_map_area, cpu);
ret = __zs_cpu_up(area);
if (ret)
return notifier_from_errno(ret);
break;
case CPU_DEAD:
case CPU_UP_CANCELED:
area = &per_cpu(zs_map_area, cpu);
__zs_cpu_down(area);
break;
}
return NOTIFY_OK;
}
static int zs_register_cpu_notifier(void)
{
int cpu, uninitialized_var(ret);
cpu_notifier_register_begin();
__register_cpu_notifier(&zs_cpu_nb);
for_each_online_cpu(cpu) {
ret = zs_cpu_notifier(NULL, CPU_UP_PREPARE, (void *)(long)cpu);
if (notifier_to_errno(ret))
break;
}
cpu_notifier_register_done();
return notifier_to_errno(ret);
}
static void zs_unregister_cpu_notifier(void)
{
int cpu;
cpu_notifier_register_begin();
for_each_online_cpu(cpu)
zs_cpu_notifier(NULL, CPU_DEAD, (void *)(long)cpu);
__unregister_cpu_notifier(&zs_cpu_nb);
cpu_notifier_register_done();
}
static void init_zs_size_classes(void)
{
int nr;
nr = (ZS_MAX_ALLOC_SIZE - ZS_MIN_ALLOC_SIZE) / ZS_SIZE_CLASS_DELTA + 1;
if ((ZS_MAX_ALLOC_SIZE - ZS_MIN_ALLOC_SIZE) % ZS_SIZE_CLASS_DELTA)
nr += 1;
zs_size_classes = nr;
}
static bool can_merge(struct size_class *prev, int size, int pages_per_zspage)
{
if (prev->pages_per_zspage != pages_per_zspage)
return false;
if (get_maxobj_per_zspage(prev->size, prev->pages_per_zspage)
!= get_maxobj_per_zspage(size, pages_per_zspage))
return false;
return true;
}
static bool zspage_full(struct page *page)
{
BUG_ON(!is_first_page(page));
return page->inuse == page->objects;
}
unsigned long zs_get_total_pages(struct zs_pool *pool)
{
return atomic_long_read(&pool->pages_allocated);
}
void *zs_map_object(struct zs_pool *pool, unsigned long handle,
enum zs_mapmode mm)
{
struct page *page;
unsigned long obj, obj_idx, off;
unsigned int class_idx;
enum fullness_group fg;
struct size_class *class;
struct mapping_area *area;
struct page *pages[2];
void *ret;
BUG_ON(!handle);
BUG_ON(in_interrupt());
pin_tag(handle);
obj = handle_to_obj(handle);
obj_to_location(obj, &page, &obj_idx);
get_zspage_mapping(get_first_page(page), &class_idx, &fg);
class = pool->size_class[class_idx];
off = obj_idx_to_offset(page, obj_idx, class->size);
area = &get_cpu_var(zs_map_area);
area->vm_mm = mm;
if (off + class->size <= PAGE_SIZE) {
area->vm_addr = kmap_atomic(page);
ret = area->vm_addr + off;
goto out;
}
pages[0] = page;
pages[1] = get_next_page(page);
BUG_ON(!pages[1]);
ret = __zs_map_object(area, pages, off, class->size);
out:
if (!class->huge)
ret += ZS_HANDLE_SIZE;
return ret;
}
void zs_unmap_object(struct zs_pool *pool, unsigned long handle)
{
struct page *page;
unsigned long obj, obj_idx, off;
unsigned int class_idx;
enum fullness_group fg;
struct size_class *class;
struct mapping_area *area;
BUG_ON(!handle);
obj = handle_to_obj(handle);
obj_to_location(obj, &page, &obj_idx);
get_zspage_mapping(get_first_page(page), &class_idx, &fg);
class = pool->size_class[class_idx];
off = obj_idx_to_offset(page, obj_idx, class->size);
area = this_cpu_ptr(&zs_map_area);
if (off + class->size <= PAGE_SIZE)
kunmap_atomic(area->vm_addr);
else {
struct page *pages[2];
pages[0] = page;
pages[1] = get_next_page(page);
BUG_ON(!pages[1]);
__zs_unmap_object(area, pages, off, class->size);
}
put_cpu_var(zs_map_area);
unpin_tag(handle);
}
static unsigned long obj_malloc(struct page *first_page,
struct size_class *class, unsigned long handle)
{
unsigned long obj;
struct link_free *link;
struct page *m_page;
unsigned long m_objidx, m_offset;
void *vaddr;
handle |= OBJ_ALLOCATED_TAG;
obj = (unsigned long)first_page->freelist;
obj_to_location(obj, &m_page, &m_objidx);
m_offset = obj_idx_to_offset(m_page, m_objidx, class->size);
vaddr = kmap_atomic(m_page);
link = (struct link_free *)vaddr + m_offset / sizeof(*link);
first_page->freelist = link->next;
if (!class->huge)
link->handle = handle;
else
set_page_private(first_page, handle);
kunmap_atomic(vaddr);
first_page->inuse++;
zs_stat_inc(class, OBJ_USED, 1);
return obj;
}
unsigned long zs_malloc(struct zs_pool *pool, size_t size)
{
unsigned long handle, obj;
struct size_class *class;
struct page *first_page;
if (unlikely(!size || size > ZS_MAX_ALLOC_SIZE))
return 0;
handle = alloc_handle(pool);
if (!handle)
return 0;
size += ZS_HANDLE_SIZE;
class = pool->size_class[get_size_class_index(size)];
spin_lock(&class->lock);
first_page = find_get_zspage(class);
if (!first_page) {
spin_unlock(&class->lock);
first_page = alloc_zspage(class, pool->flags);
if (unlikely(!first_page)) {
free_handle(pool, handle);
return 0;
}
set_zspage_mapping(first_page, class->index, ZS_EMPTY);
atomic_long_add(class->pages_per_zspage,
&pool->pages_allocated);
spin_lock(&class->lock);
zs_stat_inc(class, OBJ_ALLOCATED, get_maxobj_per_zspage(
class->size, class->pages_per_zspage));
}
obj = obj_malloc(first_page, class, handle);
fix_fullness_group(class, first_page);
record_obj(handle, obj);
spin_unlock(&class->lock);
return handle;
}
static void obj_free(struct zs_pool *pool, struct size_class *class,
unsigned long obj)
{
struct link_free *link;
struct page *first_page, *f_page;
unsigned long f_objidx, f_offset;
void *vaddr;
int class_idx;
enum fullness_group fullness;
BUG_ON(!obj);
obj &= ~OBJ_ALLOCATED_TAG;
obj_to_location(obj, &f_page, &f_objidx);
first_page = get_first_page(f_page);
get_zspage_mapping(first_page, &class_idx, &fullness);
f_offset = obj_idx_to_offset(f_page, f_objidx, class->size);
vaddr = kmap_atomic(f_page);
link = (struct link_free *)(vaddr + f_offset);
link->next = first_page->freelist;
if (class->huge)
set_page_private(first_page, 0);
kunmap_atomic(vaddr);
first_page->freelist = (void *)obj;
first_page->inuse--;
zs_stat_dec(class, OBJ_USED, 1);
}
void zs_free(struct zs_pool *pool, unsigned long handle)
{
struct page *first_page, *f_page;
unsigned long obj, f_objidx;
int class_idx;
struct size_class *class;
enum fullness_group fullness;
if (unlikely(!handle))
return;
pin_tag(handle);
obj = handle_to_obj(handle);
obj_to_location(obj, &f_page, &f_objidx);
first_page = get_first_page(f_page);
get_zspage_mapping(first_page, &class_idx, &fullness);
class = pool->size_class[class_idx];
spin_lock(&class->lock);
obj_free(pool, class, obj);
fullness = fix_fullness_group(class, first_page);
if (fullness == ZS_EMPTY) {
zs_stat_dec(class, OBJ_ALLOCATED, get_maxobj_per_zspage(
class->size, class->pages_per_zspage));
atomic_long_sub(class->pages_per_zspage,
&pool->pages_allocated);
free_zspage(first_page);
}
spin_unlock(&class->lock);
unpin_tag(handle);
free_handle(pool, handle);
}
static void zs_object_copy(unsigned long src, unsigned long dst,
struct size_class *class)
{
struct page *s_page, *d_page;
unsigned long s_objidx, d_objidx;
unsigned long s_off, d_off;
void *s_addr, *d_addr;
int s_size, d_size, size;
int written = 0;
s_size = d_size = class->size;
obj_to_location(src, &s_page, &s_objidx);
obj_to_location(dst, &d_page, &d_objidx);
s_off = obj_idx_to_offset(s_page, s_objidx, class->size);
d_off = obj_idx_to_offset(d_page, d_objidx, class->size);
if (s_off + class->size > PAGE_SIZE)
s_size = PAGE_SIZE - s_off;
if (d_off + class->size > PAGE_SIZE)
d_size = PAGE_SIZE - d_off;
s_addr = kmap_atomic(s_page);
d_addr = kmap_atomic(d_page);
while (1) {
size = min(s_size, d_size);
memcpy(d_addr + d_off, s_addr + s_off, size);
written += size;
if (written == class->size)
break;
s_off += size;
s_size -= size;
d_off += size;
d_size -= size;
if (s_off >= PAGE_SIZE) {
kunmap_atomic(d_addr);
kunmap_atomic(s_addr);
s_page = get_next_page(s_page);
BUG_ON(!s_page);
s_addr = kmap_atomic(s_page);
d_addr = kmap_atomic(d_page);
s_size = class->size - written;
s_off = 0;
}
if (d_off >= PAGE_SIZE) {
kunmap_atomic(d_addr);
d_page = get_next_page(d_page);
BUG_ON(!d_page);
d_addr = kmap_atomic(d_page);
d_size = class->size - written;
d_off = 0;
}
}
kunmap_atomic(d_addr);
kunmap_atomic(s_addr);
}
static unsigned long find_alloced_obj(struct page *page, int index,
struct size_class *class)
{
unsigned long head;
int offset = 0;
unsigned long handle = 0;
void *addr = kmap_atomic(page);
if (!is_first_page(page))
offset = page->index;
offset += class->size * index;
while (offset < PAGE_SIZE) {
head = obj_to_head(class, page, addr + offset);
if (head & OBJ_ALLOCATED_TAG) {
handle = head & ~OBJ_ALLOCATED_TAG;
if (trypin_tag(handle))
break;
handle = 0;
}
offset += class->size;
index++;
}
kunmap_atomic(addr);
return handle;
}
static int migrate_zspage(struct zs_pool *pool, struct size_class *class,
struct zs_compact_control *cc)
{
unsigned long used_obj, free_obj;
unsigned long handle;
struct page *s_page = cc->s_page;
struct page *d_page = cc->d_page;
unsigned long index = cc->index;
int nr_migrated = 0;
int ret = 0;
while (1) {
handle = find_alloced_obj(s_page, index, class);
if (!handle) {
s_page = get_next_page(s_page);
if (!s_page)
break;
index = 0;
continue;
}
if (zspage_full(d_page)) {
unpin_tag(handle);
ret = -ENOMEM;
break;
}
used_obj = handle_to_obj(handle);
free_obj = obj_malloc(d_page, class, handle);
zs_object_copy(used_obj, free_obj, class);
index++;
record_obj(handle, free_obj);
unpin_tag(handle);
obj_free(pool, class, used_obj);
nr_migrated++;
}
cc->s_page = s_page;
cc->index = index;
cc->nr_migrated = nr_migrated;
return ret;
}
static struct page *alloc_target_page(struct size_class *class)
{
int i;
struct page *page;
for (i = 0; i < _ZS_NR_FULLNESS_GROUPS; i++) {
page = class->fullness_list[i];
if (page) {
remove_zspage(page, class, i);
break;
}
}
return page;
}
static void putback_zspage(struct zs_pool *pool, struct size_class *class,
struct page *first_page)
{
enum fullness_group fullness;
BUG_ON(!is_first_page(first_page));
fullness = get_fullness_group(first_page);
insert_zspage(first_page, class, fullness);
set_zspage_mapping(first_page, class->index, fullness);
if (fullness == ZS_EMPTY) {
zs_stat_dec(class, OBJ_ALLOCATED, get_maxobj_per_zspage(
class->size, class->pages_per_zspage));
atomic_long_sub(class->pages_per_zspage,
&pool->pages_allocated);
free_zspage(first_page);
}
}
static struct page *isolate_source_page(struct size_class *class)
{
struct page *page;
page = class->fullness_list[ZS_ALMOST_EMPTY];
if (page)
remove_zspage(page, class, ZS_ALMOST_EMPTY);
return page;
}
static unsigned long __zs_compact(struct zs_pool *pool,
struct size_class *class)
{
int nr_to_migrate;
struct zs_compact_control cc;
struct page *src_page;
struct page *dst_page = NULL;
unsigned long nr_total_migrated = 0;
spin_lock(&class->lock);
while ((src_page = isolate_source_page(class))) {
BUG_ON(!is_first_page(src_page));
nr_to_migrate = src_page->inuse;
cc.index = 0;
cc.s_page = src_page;
while ((dst_page = alloc_target_page(class))) {
cc.d_page = dst_page;
if (!migrate_zspage(pool, class, &cc))
break;
putback_zspage(pool, class, dst_page);
nr_total_migrated += cc.nr_migrated;
nr_to_migrate -= cc.nr_migrated;
}
if (dst_page == NULL)
break;
putback_zspage(pool, class, dst_page);
putback_zspage(pool, class, src_page);
spin_unlock(&class->lock);
nr_total_migrated += cc.nr_migrated;
cond_resched();
spin_lock(&class->lock);
}
if (src_page)
putback_zspage(pool, class, src_page);
spin_unlock(&class->lock);
return nr_total_migrated;
}
unsigned long zs_compact(struct zs_pool *pool)
{
int i;
unsigned long nr_migrated = 0;
struct size_class *class;
for (i = zs_size_classes - 1; i >= 0; i--) {
class = pool->size_class[i];
if (!class)
continue;
if (class->index != i)
continue;
nr_migrated += __zs_compact(pool, class);
}
return nr_migrated;
}
struct zs_pool *zs_create_pool(char *name, gfp_t flags)
{
int i;
struct zs_pool *pool;
struct size_class *prev_class = NULL;
pool = kzalloc(sizeof(*pool), GFP_KERNEL);
if (!pool)
return NULL;
pool->size_class = kcalloc(zs_size_classes, sizeof(struct size_class *),
GFP_KERNEL);
if (!pool->size_class) {
kfree(pool);
return NULL;
}
pool->name = kstrdup(name, GFP_KERNEL);
if (!pool->name)
goto err;
if (create_handle_cache(pool))
goto err;
for (i = zs_size_classes - 1; i >= 0; i--) {
int size;
int pages_per_zspage;
struct size_class *class;
size = ZS_MIN_ALLOC_SIZE + i * ZS_SIZE_CLASS_DELTA;
if (size > ZS_MAX_ALLOC_SIZE)
size = ZS_MAX_ALLOC_SIZE;
pages_per_zspage = get_pages_per_zspage(size);
if (prev_class) {
if (can_merge(prev_class, size, pages_per_zspage)) {
pool->size_class[i] = prev_class;
continue;
}
}
class = kzalloc(sizeof(struct size_class), GFP_KERNEL);
if (!class)
goto err;
class->size = size;
class->index = i;
class->pages_per_zspage = pages_per_zspage;
if (pages_per_zspage == 1 &&
get_maxobj_per_zspage(size, pages_per_zspage) == 1)
class->huge = true;
spin_lock_init(&class->lock);
pool->size_class[i] = class;
prev_class = class;
}
pool->flags = flags;
if (zs_pool_stat_create(name, pool))
goto err;
return pool;
err:
zs_destroy_pool(pool);
return NULL;
}
void zs_destroy_pool(struct zs_pool *pool)
{
int i;
zs_pool_stat_destroy(pool);
for (i = 0; i < zs_size_classes; i++) {
int fg;
struct size_class *class = pool->size_class[i];
if (!class)
continue;
if (class->index != i)
continue;
for (fg = 0; fg < _ZS_NR_FULLNESS_GROUPS; fg++) {
if (class->fullness_list[fg]) {
pr_info("Freeing non-empty class with size %db, fullness group %d\n",
class->size, fg);
}
}
kfree(class);
}
destroy_handle_cache(pool);
kfree(pool->size_class);
kfree(pool->name);
kfree(pool);
}
static int __init zs_init(void)
{
int ret = zs_register_cpu_notifier();
if (ret)
goto notifier_fail;
init_zs_size_classes();
#ifdef CONFIG_ZPOOL
zpool_register_driver(&zs_zpool_driver);
#endif
ret = zs_stat_init();
if (ret) {
pr_err("zs stat initialization failed\n");
goto stat_fail;
}
return 0;
stat_fail:
#ifdef CONFIG_ZPOOL
zpool_unregister_driver(&zs_zpool_driver);
#endif
notifier_fail:
zs_unregister_cpu_notifier();
return ret;
}
static void __exit zs_exit(void)
{
#ifdef CONFIG_ZPOOL
zpool_unregister_driver(&zs_zpool_driver);
#endif
zs_unregister_cpu_notifier();
zs_stat_exit();
}
