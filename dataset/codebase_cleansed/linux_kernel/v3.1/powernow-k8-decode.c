static int get_fidvid(uint32_t cpu, uint32_t *fid, uint32_t *vid)
{
int err = 1;
uint64_t msr = 0;
int fd;
char file[20];
if (cpu > MCPU)
goto out;
sprintf(file, "/dev/cpu/%d/msr", cpu);
fd = open(file, O_RDONLY);
if (fd < 0)
goto out;
lseek(fd, MSR_FIDVID_STATUS, SEEK_CUR);
if (read(fd, &msr, 8) != 8)
goto err1;
*fid = ((uint32_t )(msr & 0xffffffffull)) & MSR_S_LO_CURRENT_FID;
*vid = ((uint32_t )(msr>>32 & 0xffffffffull)) & MSR_S_HI_CURRENT_VID;
err = 0;
err1:
close(fd);
out:
return err;
}
static uint32_t find_freq_from_fid(uint32_t fid)
{
return 800 + (fid * 100);
}
static uint32_t find_millivolts_from_vid(uint32_t vid)
{
return 1550-vid*25;
}
int main (int argc, char *argv[])
{
int err;
int cpu;
uint32_t fid, vid;
if (argc < 2)
cpu = 0;
else
cpu = strtoul(argv[1], NULL, 0);
err = get_fidvid(cpu, &fid, &vid);
if (err) {
printf("can't get fid, vid from MSR\n");
printf("Possible trouble: you don't run a powernow-k8 capable cpu\n");
printf("or you are not root, or the msr driver is not present\n");
exit(1);
}
printf("cpu %d currently at %d MHz and %d mV\n",
cpu,
find_freq_from_fid(fid),
find_millivolts_from_vid(vid));
return 0;
}
