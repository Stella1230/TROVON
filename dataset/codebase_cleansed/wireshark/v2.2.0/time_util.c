time_t
mktime_utc(struct tm *tm)
{
#ifndef HAVE_TIMEGM
time_t retval;
static const int days_before[] =
{
0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
};
int yr;
if (tm->tm_mon < 0 || tm->tm_mon > 11)
return (time_t) -1;
retval = (tm->tm_year - 70) * 365;
yr = tm->tm_year + 1900;
if (tm->tm_mon + 1 < 3 && (yr % 4) == 0 && ((yr % 100) != 0 || (yr % 400) == 0))
yr--;
retval += (((yr / 4) - (yr / 100) + (yr / 400)) - 477);
retval += days_before[tm->tm_mon] + tm->tm_mday - 1;
retval = ((((retval * 24) + tm->tm_hour) * 60) + tm->tm_min) * 60 + tm->tm_sec;
return retval;
#else
return timegm(tm);
#endif
}
void get_resource_usage(double *user_time, double *sys_time) {
#ifndef _WIN32
struct rusage ru;
getrusage(RUSAGE_SELF, &ru);
*user_time = ru.ru_utime.tv_sec + (ru.ru_utime.tv_usec / 1000000.0);
*sys_time = ru.ru_stime.tv_sec + (ru.ru_stime.tv_usec / 1000000.0);
#else
HANDLE h_proc = GetCurrentProcess();
FILETIME cft, eft, kft, uft;
ULARGE_INTEGER uli_time;
GetProcessTimes(h_proc, &cft, &eft, &kft, &uft);
uli_time.LowPart = uft.dwLowDateTime;
uli_time.HighPart = uft.dwHighDateTime;
*user_time = uli_time.QuadPart / 10000000.0;
uli_time.LowPart = kft.dwLowDateTime;
uli_time.HighPart = kft.dwHighDateTime;
*sys_time = uli_time.QuadPart / 1000000000.0;
#endif
}
void log_resource_usage(gboolean reset_delta, const char *format, ...) {
va_list ap;
GString *log_str = g_string_new("");
double user_time;
double sys_time;
get_resource_usage(&user_time, &sys_time);
if (reset_delta || last_user_time == 0.0) {
last_user_time = user_time;
last_sys_time = sys_time;
}
g_string_append_printf(log_str, "user %.3f +%.3f sys %.3f +%.3f ",
user_time, user_time - last_user_time,
sys_time, sys_time - last_sys_time);
va_start(ap, format);
g_string_append_vprintf(log_str, format, ap);
va_end(ap);
g_warning("%s", log_str->str);
g_string_free(log_str, TRUE);
}
