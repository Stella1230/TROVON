void dp_save_restore(struct task_struct *prev, struct task_struct *next)
{
struct eznps_dp *prev_task_dp = &prev->thread.dp;
struct eznps_dp *next_task_dp = &next->thread.dp;
prev_task_dp->eflags = read_aux_reg(CTOP_AUX_EFLAGS);
write_aux_reg(CTOP_AUX_EFLAGS, next_task_dp->eflags);
prev_task_dp->gpa1 = read_aux_reg(CTOP_AUX_GPA1);
write_aux_reg(CTOP_AUX_GPA1, next_task_dp->gpa1);
}
