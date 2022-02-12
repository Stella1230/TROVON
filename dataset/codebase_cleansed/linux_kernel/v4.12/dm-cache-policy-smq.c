static unsigned safe_div(unsigned n, unsigned d)
{
return d ? n / d : 0u;
}
static unsigned safe_mod(unsigned n, unsigned d)
{
return d ? n % d : 0u;
}
static int space_init(struct entry_space *es, unsigned nr_entries)
{
if (!nr_entries) {
es->begin = es->end = NULL;
return 0;
}
es->begin = vzalloc(sizeof(struct entry) * nr_entries);
if (!es->begin)
return -ENOMEM;
es->end = es->begin + nr_entries;
return 0;
}
static void space_exit(struct entry_space *es)
{
vfree(es->begin);
}
static struct entry *__get_entry(struct entry_space *es, unsigned block)
{
struct entry *e;
e = es->begin + block;
BUG_ON(e >= es->end);
return e;
}
static unsigned to_index(struct entry_space *es, struct entry *e)
{
BUG_ON(e < es->begin || e >= es->end);
return e - es->begin;
}
static struct entry *to_entry(struct entry_space *es, unsigned block)
{
if (block == INDEXER_NULL)
return NULL;
return __get_entry(es, block);
}
static void l_init(struct ilist *l)
{
l->nr_elts = 0;
l->head = l->tail = INDEXER_NULL;
}
static struct entry *l_head(struct entry_space *es, struct ilist *l)
{
return to_entry(es, l->head);
}
static struct entry *l_tail(struct entry_space *es, struct ilist *l)
{
return to_entry(es, l->tail);
}
static struct entry *l_next(struct entry_space *es, struct entry *e)
{
return to_entry(es, e->next);
}
static struct entry *l_prev(struct entry_space *es, struct entry *e)
{
return to_entry(es, e->prev);
}
static bool l_empty(struct ilist *l)
{
return l->head == INDEXER_NULL;
}
static void l_add_head(struct entry_space *es, struct ilist *l, struct entry *e)
{
struct entry *head = l_head(es, l);
e->next = l->head;
e->prev = INDEXER_NULL;
if (head)
head->prev = l->head = to_index(es, e);
else
l->head = l->tail = to_index(es, e);
if (!e->sentinel)
l->nr_elts++;
}
static void l_add_tail(struct entry_space *es, struct ilist *l, struct entry *e)
{
struct entry *tail = l_tail(es, l);
e->next = INDEXER_NULL;
e->prev = l->tail;
if (tail)
tail->next = l->tail = to_index(es, e);
else
l->head = l->tail = to_index(es, e);
if (!e->sentinel)
l->nr_elts++;
}
static void l_add_before(struct entry_space *es, struct ilist *l,
struct entry *old, struct entry *e)
{
struct entry *prev = l_prev(es, old);
if (!prev)
l_add_head(es, l, e);
else {
e->prev = old->prev;
e->next = to_index(es, old);
prev->next = old->prev = to_index(es, e);
if (!e->sentinel)
l->nr_elts++;
}
}
static void l_del(struct entry_space *es, struct ilist *l, struct entry *e)
{
struct entry *prev = l_prev(es, e);
struct entry *next = l_next(es, e);
if (prev)
prev->next = e->next;
else
l->head = e->next;
if (next)
next->prev = e->prev;
else
l->tail = e->prev;
if (!e->sentinel)
l->nr_elts--;
}
static struct entry *l_pop_tail(struct entry_space *es, struct ilist *l)
{
struct entry *e;
for (e = l_tail(es, l); e; e = l_prev(es, e))
if (!e->sentinel) {
l_del(es, l, e);
return e;
}
return NULL;
}
static void q_init(struct queue *q, struct entry_space *es, unsigned nr_levels)
{
unsigned i;
q->es = es;
q->nr_elts = 0;
q->nr_levels = nr_levels;
for (i = 0; i < q->nr_levels; i++) {
l_init(q->qs + i);
q->target_count[i] = 0u;
}
q->last_target_nr_elts = 0u;
q->nr_top_levels = 0u;
q->nr_in_top_levels = 0u;
}
static unsigned q_size(struct queue *q)
{
return q->nr_elts;
}
static void q_push(struct queue *q, struct entry *e)
{
BUG_ON(e->pending_work);
if (!e->sentinel)
q->nr_elts++;
l_add_tail(q->es, q->qs + e->level, e);
}
static void q_push_front(struct queue *q, struct entry *e)
{
BUG_ON(e->pending_work);
if (!e->sentinel)
q->nr_elts++;
l_add_head(q->es, q->qs + e->level, e);
}
static void q_push_before(struct queue *q, struct entry *old, struct entry *e)
{
BUG_ON(e->pending_work);
if (!e->sentinel)
q->nr_elts++;
l_add_before(q->es, q->qs + e->level, old, e);
}
static void q_del(struct queue *q, struct entry *e)
{
l_del(q->es, q->qs + e->level, e);
if (!e->sentinel)
q->nr_elts--;
}
static struct entry *q_peek(struct queue *q, unsigned max_level, bool can_cross_sentinel)
{
unsigned level;
struct entry *e;
max_level = min(max_level, q->nr_levels);
for (level = 0; level < max_level; level++)
for (e = l_head(q->es, q->qs + level); e; e = l_next(q->es, e)) {
if (e->sentinel) {
if (can_cross_sentinel)
continue;
else
break;
}
return e;
}
return NULL;
}
static struct entry *q_pop(struct queue *q)
{
struct entry *e = q_peek(q, q->nr_levels, true);
if (e)
q_del(q, e);
return e;
}
static struct entry *__redist_pop_from(struct queue *q, unsigned level)
{
struct entry *e;
for (; level < q->nr_levels; level++)
for (e = l_head(q->es, q->qs + level); e; e = l_next(q->es, e))
if (!e->sentinel) {
l_del(q->es, q->qs + e->level, e);
return e;
}
return NULL;
}
static void q_set_targets_subrange_(struct queue *q, unsigned nr_elts, unsigned lbegin, unsigned lend)
{
unsigned level, nr_levels, entries_per_level, remainder;
BUG_ON(lbegin > lend);
BUG_ON(lend > q->nr_levels);
nr_levels = lend - lbegin;
entries_per_level = safe_div(nr_elts, nr_levels);
remainder = safe_mod(nr_elts, nr_levels);
for (level = lbegin; level < lend; level++)
q->target_count[level] =
(level < (lbegin + remainder)) ? entries_per_level + 1u : entries_per_level;
}
static void q_set_targets(struct queue *q)
{
if (q->last_target_nr_elts == q->nr_elts)
return;
q->last_target_nr_elts = q->nr_elts;
if (q->nr_top_levels > q->nr_levels)
q_set_targets_subrange_(q, q->nr_elts, 0, q->nr_levels);
else {
q_set_targets_subrange_(q, q->nr_in_top_levels,
q->nr_levels - q->nr_top_levels, q->nr_levels);
if (q->nr_in_top_levels < q->nr_elts)
q_set_targets_subrange_(q, q->nr_elts - q->nr_in_top_levels,
0, q->nr_levels - q->nr_top_levels);
else
q_set_targets_subrange_(q, 0, 0, q->nr_levels - q->nr_top_levels);
}
}
static void q_redistribute(struct queue *q)
{
unsigned target, level;
struct ilist *l, *l_above;
struct entry *e;
q_set_targets(q);
for (level = 0u; level < q->nr_levels - 1u; level++) {
l = q->qs + level;
target = q->target_count[level];
while (l->nr_elts < target) {
e = __redist_pop_from(q, level + 1u);
if (!e) {
break;
}
e->level = level;
l_add_tail(q->es, l, e);
}
l_above = q->qs + level + 1u;
while (l->nr_elts > target) {
e = l_pop_tail(q->es, l);
if (!e)
break;
e->level = level + 1u;
l_add_tail(q->es, l_above, e);
}
}
}
static void q_requeue(struct queue *q, struct entry *e, unsigned extra_levels,
struct entry *s1, struct entry *s2)
{
struct entry *de;
unsigned sentinels_passed = 0;
unsigned new_level = min(q->nr_levels - 1u, e->level + extra_levels);
if (extra_levels && (e->level < q->nr_levels - 1u)) {
for (de = l_head(q->es, q->qs + new_level); de && de->sentinel; de = l_next(q->es, de))
sentinels_passed++;
if (de) {
q_del(q, de);
de->level = e->level;
if (s1) {
switch (sentinels_passed) {
case 0:
q_push_before(q, s1, de);
break;
case 1:
q_push_before(q, s2, de);
break;
default:
q_push(q, de);
}
} else
q_push(q, de);
}
}
q_del(q, e);
e->level = new_level;
q_push(q, e);
}
static void stats_init(struct stats *s, unsigned nr_levels)
{
s->hit_threshold = (nr_levels * 3u) / 4u;
s->hits = 0u;
s->misses = 0u;
}
static void stats_reset(struct stats *s)
{
s->hits = s->misses = 0u;
}
static void stats_level_accessed(struct stats *s, unsigned level)
{
if (level >= s->hit_threshold)
s->hits++;
else
s->misses++;
}
static void stats_miss(struct stats *s)
{
s->misses++;
}
static enum performance stats_assess(struct stats *s)
{
unsigned confidence = safe_div(s->hits << FP_SHIFT, s->hits + s->misses);
if (confidence < SIXTEENTH)
return Q_POOR;
else if (confidence < EIGHTH)
return Q_FAIR;
else
return Q_WELL;
}
static int h_init(struct smq_hash_table *ht, struct entry_space *es, unsigned nr_entries)
{
unsigned i, nr_buckets;
ht->es = es;
nr_buckets = roundup_pow_of_two(max(nr_entries / 4u, 16u));
ht->hash_bits = __ffs(nr_buckets);
ht->buckets = vmalloc(sizeof(*ht->buckets) * nr_buckets);
if (!ht->buckets)
return -ENOMEM;
for (i = 0; i < nr_buckets; i++)
ht->buckets[i] = INDEXER_NULL;
return 0;
}
static void h_exit(struct smq_hash_table *ht)
{
vfree(ht->buckets);
}
static struct entry *h_head(struct smq_hash_table *ht, unsigned bucket)
{
return to_entry(ht->es, ht->buckets[bucket]);
}
static struct entry *h_next(struct smq_hash_table *ht, struct entry *e)
{
return to_entry(ht->es, e->hash_next);
}
static void __h_insert(struct smq_hash_table *ht, unsigned bucket, struct entry *e)
{
e->hash_next = ht->buckets[bucket];
ht->buckets[bucket] = to_index(ht->es, e);
}
static void h_insert(struct smq_hash_table *ht, struct entry *e)
{
unsigned h = hash_64(from_oblock(e->oblock), ht->hash_bits);
__h_insert(ht, h, e);
}
static struct entry *__h_lookup(struct smq_hash_table *ht, unsigned h, dm_oblock_t oblock,
struct entry **prev)
{
struct entry *e;
*prev = NULL;
for (e = h_head(ht, h); e; e = h_next(ht, e)) {
if (e->oblock == oblock)
return e;
*prev = e;
}
return NULL;
}
static void __h_unlink(struct smq_hash_table *ht, unsigned h,
struct entry *e, struct entry *prev)
{
if (prev)
prev->hash_next = e->hash_next;
else
ht->buckets[h] = e->hash_next;
}
static struct entry *h_lookup(struct smq_hash_table *ht, dm_oblock_t oblock)
{
struct entry *e, *prev;
unsigned h = hash_64(from_oblock(oblock), ht->hash_bits);
e = __h_lookup(ht, h, oblock, &prev);
if (e && prev) {
__h_unlink(ht, h, e, prev);
__h_insert(ht, h, e);
}
return e;
}
static void h_remove(struct smq_hash_table *ht, struct entry *e)
{
unsigned h = hash_64(from_oblock(e->oblock), ht->hash_bits);
struct entry *prev;
e = __h_lookup(ht, h, e->oblock, &prev);
if (e)
__h_unlink(ht, h, e, prev);
}
static void init_allocator(struct entry_alloc *ea, struct entry_space *es,
unsigned begin, unsigned end)
{
unsigned i;
ea->es = es;
ea->nr_allocated = 0u;
ea->begin = begin;
l_init(&ea->free);
for (i = begin; i != end; i++)
l_add_tail(ea->es, &ea->free, __get_entry(ea->es, i));
}
static void init_entry(struct entry *e)
{
e->hash_next = INDEXER_NULL;
e->next = INDEXER_NULL;
e->prev = INDEXER_NULL;
e->level = 0u;
e->dirty = true;
e->allocated = true;
e->sentinel = false;
e->pending_work = false;
}
static struct entry *alloc_entry(struct entry_alloc *ea)
{
struct entry *e;
if (l_empty(&ea->free))
return NULL;
e = l_pop_tail(ea->es, &ea->free);
init_entry(e);
ea->nr_allocated++;
return e;
}
static struct entry *alloc_particular_entry(struct entry_alloc *ea, unsigned i)
{
struct entry *e = __get_entry(ea->es, ea->begin + i);
BUG_ON(e->allocated);
l_del(ea->es, &ea->free, e);
init_entry(e);
ea->nr_allocated++;
return e;
}
static void free_entry(struct entry_alloc *ea, struct entry *e)
{
BUG_ON(!ea->nr_allocated);
BUG_ON(!e->allocated);
ea->nr_allocated--;
e->allocated = false;
l_add_tail(ea->es, &ea->free, e);
}
static bool allocator_empty(struct entry_alloc *ea)
{
return l_empty(&ea->free);
}
static unsigned get_index(struct entry_alloc *ea, struct entry *e)
{
return to_index(ea->es, e) - ea->begin;
}
static struct entry *get_entry(struct entry_alloc *ea, unsigned index)
{
return __get_entry(ea->es, ea->begin + index);
}
static struct entry *get_sentinel(struct entry_alloc *ea, unsigned level, bool which)
{
return get_entry(ea, which ? level : NR_CACHE_LEVELS + level);
}
static struct entry *writeback_sentinel(struct smq_policy *mq, unsigned level)
{
return get_sentinel(&mq->writeback_sentinel_alloc, level, mq->current_writeback_sentinels);
}
static struct entry *demote_sentinel(struct smq_policy *mq, unsigned level)
{
return get_sentinel(&mq->demote_sentinel_alloc, level, mq->current_demote_sentinels);
}
static void __update_writeback_sentinels(struct smq_policy *mq)
{
unsigned level;
struct queue *q = &mq->dirty;
struct entry *sentinel;
for (level = 0; level < q->nr_levels; level++) {
sentinel = writeback_sentinel(mq, level);
q_del(q, sentinel);
q_push(q, sentinel);
}
}
static void __update_demote_sentinels(struct smq_policy *mq)
{
unsigned level;
struct queue *q = &mq->clean;
struct entry *sentinel;
for (level = 0; level < q->nr_levels; level++) {
sentinel = demote_sentinel(mq, level);
q_del(q, sentinel);
q_push(q, sentinel);
}
}
static void update_sentinels(struct smq_policy *mq)
{
if (time_after(jiffies, mq->next_writeback_period)) {
mq->next_writeback_period = jiffies + WRITEBACK_PERIOD;
mq->current_writeback_sentinels = !mq->current_writeback_sentinels;
__update_writeback_sentinels(mq);
}
if (time_after(jiffies, mq->next_demote_period)) {
mq->next_demote_period = jiffies + DEMOTE_PERIOD;
mq->current_demote_sentinels = !mq->current_demote_sentinels;
__update_demote_sentinels(mq);
}
}
static void __sentinels_init(struct smq_policy *mq)
{
unsigned level;
struct entry *sentinel;
for (level = 0; level < NR_CACHE_LEVELS; level++) {
sentinel = writeback_sentinel(mq, level);
sentinel->level = level;
q_push(&mq->dirty, sentinel);
sentinel = demote_sentinel(mq, level);
sentinel->level = level;
q_push(&mq->clean, sentinel);
}
}
static void sentinels_init(struct smq_policy *mq)
{
mq->next_writeback_period = jiffies + WRITEBACK_PERIOD;
mq->next_demote_period = jiffies + DEMOTE_PERIOD;
mq->current_writeback_sentinels = false;
mq->current_demote_sentinels = false;
__sentinels_init(mq);
mq->current_writeback_sentinels = !mq->current_writeback_sentinels;
mq->current_demote_sentinels = !mq->current_demote_sentinels;
__sentinels_init(mq);
}
static void del_queue(struct smq_policy *mq, struct entry *e)
{
q_del(e->dirty ? &mq->dirty : &mq->clean, e);
}
static void push_queue(struct smq_policy *mq, struct entry *e)
{
if (e->dirty)
q_push(&mq->dirty, e);
else
q_push(&mq->clean, e);
}
static void push(struct smq_policy *mq, struct entry *e)
{
h_insert(&mq->table, e);
if (!e->pending_work)
push_queue(mq, e);
}
static void push_queue_front(struct smq_policy *mq, struct entry *e)
{
if (e->dirty)
q_push_front(&mq->dirty, e);
else
q_push_front(&mq->clean, e);
}
static void push_front(struct smq_policy *mq, struct entry *e)
{
h_insert(&mq->table, e);
if (!e->pending_work)
push_queue_front(mq, e);
}
static dm_cblock_t infer_cblock(struct smq_policy *mq, struct entry *e)
{
return to_cblock(get_index(&mq->cache_alloc, e));
}
static void requeue(struct smq_policy *mq, struct entry *e)
{
if (e->pending_work)
return;
if (!test_and_set_bit(from_cblock(infer_cblock(mq, e)), mq->cache_hit_bits)) {
if (!e->dirty) {
q_requeue(&mq->clean, e, 1u, NULL, NULL);
return;
}
q_requeue(&mq->dirty, e, 1u,
get_sentinel(&mq->writeback_sentinel_alloc, e->level, !mq->current_writeback_sentinels),
get_sentinel(&mq->writeback_sentinel_alloc, e->level, mq->current_writeback_sentinels));
}
}
static unsigned default_promote_level(struct smq_policy *mq)
{
static unsigned table[] = {1, 1, 1, 2, 4, 6, 7, 8, 7, 6, 4, 4, 3, 3, 2, 2, 1};
unsigned hits = mq->cache_stats.hits;
unsigned misses = mq->cache_stats.misses;
unsigned index = safe_div(hits << 4u, hits + misses);
return table[index];
}
static void update_promote_levels(struct smq_policy *mq)
{
unsigned threshold_level = allocator_empty(&mq->cache_alloc) ?
default_promote_level(mq) : (NR_HOTSPOT_LEVELS / 2u);
threshold_level = max(threshold_level, NR_HOTSPOT_LEVELS);
switch (stats_assess(&mq->hotspot_stats)) {
case Q_POOR:
threshold_level /= 4u;
break;
case Q_FAIR:
threshold_level /= 2u;
break;
case Q_WELL:
break;
}
mq->read_promote_level = NR_HOTSPOT_LEVELS - threshold_level;
mq->write_promote_level = (NR_HOTSPOT_LEVELS - threshold_level);
}
static void update_level_jump(struct smq_policy *mq)
{
switch (stats_assess(&mq->hotspot_stats)) {
case Q_POOR:
mq->hotspot_level_jump = 4u;
break;
case Q_FAIR:
mq->hotspot_level_jump = 2u;
break;
case Q_WELL:
mq->hotspot_level_jump = 1u;
break;
}
}
static void end_hotspot_period(struct smq_policy *mq)
{
clear_bitset(mq->hotspot_hit_bits, mq->nr_hotspot_blocks);
update_promote_levels(mq);
if (time_after(jiffies, mq->next_hotspot_period)) {
update_level_jump(mq);
q_redistribute(&mq->hotspot);
stats_reset(&mq->hotspot_stats);
mq->next_hotspot_period = jiffies + HOTSPOT_UPDATE_PERIOD;
}
}
static void end_cache_period(struct smq_policy *mq)
{
if (time_after(jiffies, mq->next_cache_period)) {
clear_bitset(mq->cache_hit_bits, from_cblock(mq->cache_size));
q_redistribute(&mq->dirty);
q_redistribute(&mq->clean);
stats_reset(&mq->cache_stats);
mq->next_cache_period = jiffies + CACHE_UPDATE_PERIOD;
}
}
static unsigned percent_to_target(struct smq_policy *mq, unsigned p)
{
return from_cblock(mq->cache_size) * p / 100u;
}
static bool clean_target_met(struct smq_policy *mq, bool idle)
{
if (idle) {
return q_size(&mq->dirty) == 0u;
}
return true;
}
static bool free_target_met(struct smq_policy *mq)
{
unsigned nr_free;
nr_free = from_cblock(mq->cache_size) - mq->cache_alloc.nr_allocated;
return (nr_free + btracker_nr_demotions_queued(mq->bg_work)) >=
percent_to_target(mq, FREE_TARGET);
}
static void mark_pending(struct smq_policy *mq, struct entry *e)
{
BUG_ON(e->sentinel);
BUG_ON(!e->allocated);
BUG_ON(e->pending_work);
e->pending_work = true;
}
static void clear_pending(struct smq_policy *mq, struct entry *e)
{
BUG_ON(!e->pending_work);
e->pending_work = false;
}
static void queue_writeback(struct smq_policy *mq)
{
int r;
struct policy_work work;
struct entry *e;
e = q_peek(&mq->dirty, mq->dirty.nr_levels, !mq->migrations_allowed);
if (e) {
mark_pending(mq, e);
q_del(&mq->dirty, e);
work.op = POLICY_WRITEBACK;
work.oblock = e->oblock;
work.cblock = infer_cblock(mq, e);
r = btracker_queue(mq->bg_work, &work, NULL);
WARN_ON_ONCE(r);
}
}
static void queue_demotion(struct smq_policy *mq)
{
struct policy_work work;
struct entry *e;
if (unlikely(WARN_ON_ONCE(!mq->migrations_allowed)))
return;
e = q_peek(&mq->clean, mq->clean.nr_levels / 2, true);
if (!e) {
if (!clean_target_met(mq, true))
queue_writeback(mq);
return;
}
mark_pending(mq, e);
q_del(&mq->clean, e);
work.op = POLICY_DEMOTE;
work.oblock = e->oblock;
work.cblock = infer_cblock(mq, e);
btracker_queue(mq->bg_work, &work, NULL);
}
static void queue_promotion(struct smq_policy *mq, dm_oblock_t oblock,
struct policy_work **workp)
{
struct entry *e;
struct policy_work work;
if (!mq->migrations_allowed)
return;
if (allocator_empty(&mq->cache_alloc)) {
if (!free_target_met(mq))
queue_demotion(mq);
return;
}
if (btracker_promotion_already_present(mq->bg_work, oblock))
return;
e = alloc_entry(&mq->cache_alloc);
BUG_ON(!e);
e->pending_work = true;
work.op = POLICY_PROMOTE;
work.oblock = oblock;
work.cblock = infer_cblock(mq, e);
btracker_queue(mq->bg_work, &work, workp);
}
static enum promote_result maybe_promote(bool promote)
{
return promote ? PROMOTE_PERMANENT : PROMOTE_NOT;
}
static enum promote_result should_promote(struct smq_policy *mq, struct entry *hs_e,
int data_dir, bool fast_promote)
{
if (data_dir == WRITE) {
if (!allocator_empty(&mq->cache_alloc) && fast_promote)
return PROMOTE_TEMPORARY;
return maybe_promote(hs_e->level >= mq->write_promote_level);
} else
return maybe_promote(hs_e->level >= mq->read_promote_level);
}
static dm_oblock_t to_hblock(struct smq_policy *mq, dm_oblock_t b)
{
sector_t r = from_oblock(b);
(void) sector_div(r, mq->cache_blocks_per_hotspot_block);
return to_oblock(r);
}
static struct entry *update_hotspot_queue(struct smq_policy *mq, dm_oblock_t b)
{
unsigned hi;
dm_oblock_t hb = to_hblock(mq, b);
struct entry *e = h_lookup(&mq->hotspot_table, hb);
if (e) {
stats_level_accessed(&mq->hotspot_stats, e->level);
hi = get_index(&mq->hotspot_alloc, e);
q_requeue(&mq->hotspot, e,
test_and_set_bit(hi, mq->hotspot_hit_bits) ?
0u : mq->hotspot_level_jump,
NULL, NULL);
} else {
stats_miss(&mq->hotspot_stats);
e = alloc_entry(&mq->hotspot_alloc);
if (!e) {
e = q_pop(&mq->hotspot);
if (e) {
h_remove(&mq->hotspot_table, e);
hi = get_index(&mq->hotspot_alloc, e);
clear_bit(hi, mq->hotspot_hit_bits);
}
}
if (e) {
e->oblock = hb;
q_push(&mq->hotspot, e);
h_insert(&mq->hotspot_table, e);
}
}
return e;
}
static struct smq_policy *to_smq_policy(struct dm_cache_policy *p)
{
return container_of(p, struct smq_policy, policy);
}
static void smq_destroy(struct dm_cache_policy *p)
{
struct smq_policy *mq = to_smq_policy(p);
btracker_destroy(mq->bg_work);
h_exit(&mq->hotspot_table);
h_exit(&mq->table);
free_bitset(mq->hotspot_hit_bits);
free_bitset(mq->cache_hit_bits);
space_exit(&mq->es);
kfree(mq);
}
static int __lookup(struct smq_policy *mq, dm_oblock_t oblock, dm_cblock_t *cblock,
int data_dir, bool fast_copy,
struct policy_work **work, bool *background_work)
{
struct entry *e, *hs_e;
enum promote_result pr;
*background_work = false;
e = h_lookup(&mq->table, oblock);
if (e) {
stats_level_accessed(&mq->cache_stats, e->level);
requeue(mq, e);
*cblock = infer_cblock(mq, e);
return 0;
} else {
stats_miss(&mq->cache_stats);
hs_e = update_hotspot_queue(mq, oblock);
pr = should_promote(mq, hs_e, data_dir, fast_copy);
if (pr != PROMOTE_NOT) {
queue_promotion(mq, oblock, work);
*background_work = true;
}
return -ENOENT;
}
}
static int smq_lookup(struct dm_cache_policy *p, dm_oblock_t oblock, dm_cblock_t *cblock,
int data_dir, bool fast_copy,
bool *background_work)
{
int r;
unsigned long flags;
struct smq_policy *mq = to_smq_policy(p);
spin_lock_irqsave(&mq->lock, flags);
r = __lookup(mq, oblock, cblock,
data_dir, fast_copy,
NULL, background_work);
spin_unlock_irqrestore(&mq->lock, flags);
return r;
}
static int smq_lookup_with_work(struct dm_cache_policy *p,
dm_oblock_t oblock, dm_cblock_t *cblock,
int data_dir, bool fast_copy,
struct policy_work **work)
{
int r;
bool background_queued;
unsigned long flags;
struct smq_policy *mq = to_smq_policy(p);
spin_lock_irqsave(&mq->lock, flags);
r = __lookup(mq, oblock, cblock, data_dir, fast_copy, work, &background_queued);
spin_unlock_irqrestore(&mq->lock, flags);
return r;
}
static int smq_get_background_work(struct dm_cache_policy *p, bool idle,
struct policy_work **result)
{
int r;
unsigned long flags;
struct smq_policy *mq = to_smq_policy(p);
spin_lock_irqsave(&mq->lock, flags);
r = btracker_issue(mq->bg_work, result);
if (r == -ENODATA) {
if (!clean_target_met(mq, idle)) {
queue_writeback(mq);
r = btracker_issue(mq->bg_work, result);
}
}
spin_unlock_irqrestore(&mq->lock, flags);
return r;
}
static void __complete_background_work(struct smq_policy *mq,
struct policy_work *work,
bool success)
{
struct entry *e = get_entry(&mq->cache_alloc,
from_cblock(work->cblock));
switch (work->op) {
case POLICY_PROMOTE:
clear_pending(mq, e);
if (success) {
e->oblock = work->oblock;
e->level = NR_CACHE_LEVELS - 1;
push(mq, e);
} else {
free_entry(&mq->cache_alloc, e);
}
break;
case POLICY_DEMOTE:
if (success) {
h_remove(&mq->table, e);
free_entry(&mq->cache_alloc, e);
} else {
clear_pending(mq, e);
push_queue(mq, e);
}
break;
case POLICY_WRITEBACK:
clear_pending(mq, e);
push_queue(mq, e);
break;
}
btracker_complete(mq->bg_work, work);
}
static void smq_complete_background_work(struct dm_cache_policy *p,
struct policy_work *work,
bool success)
{
unsigned long flags;
struct smq_policy *mq = to_smq_policy(p);
spin_lock_irqsave(&mq->lock, flags);
__complete_background_work(mq, work, success);
spin_unlock_irqrestore(&mq->lock, flags);
}
static void __smq_set_clear_dirty(struct smq_policy *mq, dm_cblock_t cblock, bool set)
{
struct entry *e = get_entry(&mq->cache_alloc, from_cblock(cblock));
if (e->pending_work)
e->dirty = set;
else {
del_queue(mq, e);
e->dirty = set;
push_queue(mq, e);
}
}
static void smq_set_dirty(struct dm_cache_policy *p, dm_cblock_t cblock)
{
unsigned long flags;
struct smq_policy *mq = to_smq_policy(p);
spin_lock_irqsave(&mq->lock, flags);
__smq_set_clear_dirty(mq, cblock, true);
spin_unlock_irqrestore(&mq->lock, flags);
}
static void smq_clear_dirty(struct dm_cache_policy *p, dm_cblock_t cblock)
{
struct smq_policy *mq = to_smq_policy(p);
unsigned long flags;
spin_lock_irqsave(&mq->lock, flags);
__smq_set_clear_dirty(mq, cblock, false);
spin_unlock_irqrestore(&mq->lock, flags);
}
static unsigned random_level(dm_cblock_t cblock)
{
return hash_32(from_cblock(cblock), 9) & (NR_CACHE_LEVELS - 1);
}
static int smq_load_mapping(struct dm_cache_policy *p,
dm_oblock_t oblock, dm_cblock_t cblock,
bool dirty, uint32_t hint, bool hint_valid)
{
struct smq_policy *mq = to_smq_policy(p);
struct entry *e;
e = alloc_particular_entry(&mq->cache_alloc, from_cblock(cblock));
e->oblock = oblock;
e->dirty = dirty;
e->level = hint_valid ? min(hint, NR_CACHE_LEVELS - 1) : random_level(cblock);
e->pending_work = false;
push_front(mq, e);
return 0;
}
static int smq_invalidate_mapping(struct dm_cache_policy *p, dm_cblock_t cblock)
{
struct smq_policy *mq = to_smq_policy(p);
struct entry *e = get_entry(&mq->cache_alloc, from_cblock(cblock));
if (!e->allocated)
return -ENODATA;
del_queue(mq, e);
h_remove(&mq->table, e);
free_entry(&mq->cache_alloc, e);
return 0;
}
static uint32_t smq_get_hint(struct dm_cache_policy *p, dm_cblock_t cblock)
{
struct smq_policy *mq = to_smq_policy(p);
struct entry *e = get_entry(&mq->cache_alloc, from_cblock(cblock));
if (!e->allocated)
return 0;
return e->level;
}
static dm_cblock_t smq_residency(struct dm_cache_policy *p)
{
dm_cblock_t r;
unsigned long flags;
struct smq_policy *mq = to_smq_policy(p);
spin_lock_irqsave(&mq->lock, flags);
r = to_cblock(mq->cache_alloc.nr_allocated);
spin_unlock_irqrestore(&mq->lock, flags);
return r;
}
static void smq_tick(struct dm_cache_policy *p, bool can_block)
{
struct smq_policy *mq = to_smq_policy(p);
unsigned long flags;
spin_lock_irqsave(&mq->lock, flags);
mq->tick++;
update_sentinels(mq);
end_hotspot_period(mq);
end_cache_period(mq);
spin_unlock_irqrestore(&mq->lock, flags);
}
static void smq_allow_migrations(struct dm_cache_policy *p, bool allow)
{
struct smq_policy *mq = to_smq_policy(p);
mq->migrations_allowed = allow;
}
static int mq_set_config_value(struct dm_cache_policy *p,
const char *key, const char *value)
{
unsigned long tmp;
if (kstrtoul(value, 10, &tmp))
return -EINVAL;
if (!strcasecmp(key, "random_threshold") ||
!strcasecmp(key, "sequential_threshold") ||
!strcasecmp(key, "discard_promote_adjustment") ||
!strcasecmp(key, "read_promote_adjustment") ||
!strcasecmp(key, "write_promote_adjustment")) {
DMWARN("tunable '%s' no longer has any effect, mq policy is now an alias for smq", key);
return 0;
}
return -EINVAL;
}
static int mq_emit_config_values(struct dm_cache_policy *p, char *result,
unsigned maxlen, ssize_t *sz_ptr)
{
ssize_t sz = *sz_ptr;
DMEMIT("10 random_threshold 0 "
"sequential_threshold 0 "
"discard_promote_adjustment 0 "
"read_promote_adjustment 0 "
"write_promote_adjustment 0 ");
*sz_ptr = sz;
return 0;
}
static void init_policy_functions(struct smq_policy *mq, bool mimic_mq)
{
mq->policy.destroy = smq_destroy;
mq->policy.lookup = smq_lookup;
mq->policy.lookup_with_work = smq_lookup_with_work;
mq->policy.get_background_work = smq_get_background_work;
mq->policy.complete_background_work = smq_complete_background_work;
mq->policy.set_dirty = smq_set_dirty;
mq->policy.clear_dirty = smq_clear_dirty;
mq->policy.load_mapping = smq_load_mapping;
mq->policy.invalidate_mapping = smq_invalidate_mapping;
mq->policy.get_hint = smq_get_hint;
mq->policy.residency = smq_residency;
mq->policy.tick = smq_tick;
mq->policy.allow_migrations = smq_allow_migrations;
if (mimic_mq) {
mq->policy.set_config_value = mq_set_config_value;
mq->policy.emit_config_values = mq_emit_config_values;
}
}
static bool too_many_hotspot_blocks(sector_t origin_size,
sector_t hotspot_block_size,
unsigned nr_hotspot_blocks)
{
return (hotspot_block_size * nr_hotspot_blocks) > origin_size;
}
static void calc_hotspot_params(sector_t origin_size,
sector_t cache_block_size,
unsigned nr_cache_blocks,
sector_t *hotspot_block_size,
unsigned *nr_hotspot_blocks)
{
*hotspot_block_size = cache_block_size * 16u;
*nr_hotspot_blocks = max(nr_cache_blocks / 4u, 1024u);
while ((*hotspot_block_size > cache_block_size) &&
too_many_hotspot_blocks(origin_size, *hotspot_block_size, *nr_hotspot_blocks))
*hotspot_block_size /= 2u;
}
static struct dm_cache_policy *__smq_create(dm_cblock_t cache_size,
sector_t origin_size,
sector_t cache_block_size,
bool mimic_mq,
bool migrations_allowed)
{
unsigned i;
unsigned nr_sentinels_per_queue = 2u * NR_CACHE_LEVELS;
unsigned total_sentinels = 2u * nr_sentinels_per_queue;
struct smq_policy *mq = kzalloc(sizeof(*mq), GFP_KERNEL);
if (!mq)
return NULL;
init_policy_functions(mq, mimic_mq);
mq->cache_size = cache_size;
mq->cache_block_size = cache_block_size;
calc_hotspot_params(origin_size, cache_block_size, from_cblock(cache_size),
&mq->hotspot_block_size, &mq->nr_hotspot_blocks);
mq->cache_blocks_per_hotspot_block = div64_u64(mq->hotspot_block_size, mq->cache_block_size);
mq->hotspot_level_jump = 1u;
if (space_init(&mq->es, total_sentinels + mq->nr_hotspot_blocks + from_cblock(cache_size))) {
DMERR("couldn't initialize entry space");
goto bad_pool_init;
}
init_allocator(&mq->writeback_sentinel_alloc, &mq->es, 0, nr_sentinels_per_queue);
for (i = 0; i < nr_sentinels_per_queue; i++)
get_entry(&mq->writeback_sentinel_alloc, i)->sentinel = true;
init_allocator(&mq->demote_sentinel_alloc, &mq->es, nr_sentinels_per_queue, total_sentinels);
for (i = 0; i < nr_sentinels_per_queue; i++)
get_entry(&mq->demote_sentinel_alloc, i)->sentinel = true;
init_allocator(&mq->hotspot_alloc, &mq->es, total_sentinels,
total_sentinels + mq->nr_hotspot_blocks);
init_allocator(&mq->cache_alloc, &mq->es,
total_sentinels + mq->nr_hotspot_blocks,
total_sentinels + mq->nr_hotspot_blocks + from_cblock(cache_size));
mq->hotspot_hit_bits = alloc_bitset(mq->nr_hotspot_blocks);
if (!mq->hotspot_hit_bits) {
DMERR("couldn't allocate hotspot hit bitset");
goto bad_hotspot_hit_bits;
}
clear_bitset(mq->hotspot_hit_bits, mq->nr_hotspot_blocks);
if (from_cblock(cache_size)) {
mq->cache_hit_bits = alloc_bitset(from_cblock(cache_size));
if (!mq->cache_hit_bits) {
DMERR("couldn't allocate cache hit bitset");
goto bad_cache_hit_bits;
}
clear_bitset(mq->cache_hit_bits, from_cblock(mq->cache_size));
} else
mq->cache_hit_bits = NULL;
mq->tick = 0;
spin_lock_init(&mq->lock);
q_init(&mq->hotspot, &mq->es, NR_HOTSPOT_LEVELS);
mq->hotspot.nr_top_levels = 8;
mq->hotspot.nr_in_top_levels = min(mq->nr_hotspot_blocks / NR_HOTSPOT_LEVELS,
from_cblock(mq->cache_size) / mq->cache_blocks_per_hotspot_block);
q_init(&mq->clean, &mq->es, NR_CACHE_LEVELS);
q_init(&mq->dirty, &mq->es, NR_CACHE_LEVELS);
stats_init(&mq->hotspot_stats, NR_HOTSPOT_LEVELS);
stats_init(&mq->cache_stats, NR_CACHE_LEVELS);
if (h_init(&mq->table, &mq->es, from_cblock(cache_size)))
goto bad_alloc_table;
if (h_init(&mq->hotspot_table, &mq->es, mq->nr_hotspot_blocks))
goto bad_alloc_hotspot_table;
sentinels_init(mq);
mq->write_promote_level = mq->read_promote_level = NR_HOTSPOT_LEVELS;
mq->next_hotspot_period = jiffies;
mq->next_cache_period = jiffies;
mq->bg_work = btracker_create(10240);
if (!mq->bg_work)
goto bad_btracker;
mq->migrations_allowed = migrations_allowed;
return &mq->policy;
bad_btracker:
h_exit(&mq->hotspot_table);
bad_alloc_hotspot_table:
h_exit(&mq->table);
bad_alloc_table:
free_bitset(mq->cache_hit_bits);
bad_cache_hit_bits:
free_bitset(mq->hotspot_hit_bits);
bad_hotspot_hit_bits:
space_exit(&mq->es);
bad_pool_init:
kfree(mq);
return NULL;
}
static struct dm_cache_policy *smq_create(dm_cblock_t cache_size,
sector_t origin_size,
sector_t cache_block_size)
{
return __smq_create(cache_size, origin_size, cache_block_size, false, true);
}
static struct dm_cache_policy *mq_create(dm_cblock_t cache_size,
sector_t origin_size,
sector_t cache_block_size)
{
return __smq_create(cache_size, origin_size, cache_block_size, true, true);
}
static struct dm_cache_policy *cleaner_create(dm_cblock_t cache_size,
sector_t origin_size,
sector_t cache_block_size)
{
return __smq_create(cache_size, origin_size, cache_block_size, false, false);
}
static int __init smq_init(void)
{
int r;
r = dm_cache_policy_register(&smq_policy_type);
if (r) {
DMERR("register failed %d", r);
return -ENOMEM;
}
r = dm_cache_policy_register(&mq_policy_type);
if (r) {
DMERR("register failed (as mq) %d", r);
goto out_mq;
}
r = dm_cache_policy_register(&cleaner_policy_type);
if (r) {
DMERR("register failed (as cleaner) %d", r);
goto out_cleaner;
}
r = dm_cache_policy_register(&default_policy_type);
if (r) {
DMERR("register failed (as default) %d", r);
goto out_default;
}
return 0;
out_default:
dm_cache_policy_unregister(&cleaner_policy_type);
out_cleaner:
dm_cache_policy_unregister(&mq_policy_type);
out_mq:
dm_cache_policy_unregister(&smq_policy_type);
return -ENOMEM;
}
static void __exit smq_exit(void)
{
dm_cache_policy_unregister(&cleaner_policy_type);
dm_cache_policy_unregister(&smq_policy_type);
dm_cache_policy_unregister(&mq_policy_type);
dm_cache_policy_unregister(&default_policy_type);
}
