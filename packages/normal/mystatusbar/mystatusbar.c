#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string.h>

#define LEN(x) (sizeof (x) / sizeof *(x))
char buf[1024];
char buffer[1024];

int
pscanf(const char* path, const char* fmt, ...) {
    FILE* fp;
    va_list ap;
    int n;
    if(!(fp = fopen(path, "r"))) {
        return -1;
    }
    va_start(ap, fmt);
    n = vfscanf(fp, fmt, ap);
    va_end(ap);
    fclose(fp);
    return (n == EOF) ? -1 : n;
}

const char*
datetime(const char* fmt) {
    time_t t;
    t = time(NULL);
    if(!strftime(buf, sizeof(buf), fmt, localtime(&t))) {
        return NULL;
    }
    return buf;
}


const char*
cpu_freq_formatted(const char* fmt) {
    uintmax_t freq;
    /* in kHz */
    if(pscanf("/sys/devices/system/cpu/cpu0/cpufreq/"
            "scaling_cur_freq", "%ju", &freq) != 1) {
        return NULL;
    }
    sprintf(buffer, fmt, freq / (1000.0 * 1000));
    return buffer;
}

const char* cpu_perc(const char* fmt) {
    long double a[7];
    long work, total;
    static long prevWork;
    static long prevTotal;
    /* cpu user nice system idle iowait irq softirq */
    if(pscanf("/proc/stat", "%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf",
            &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6])
        != 7) {
        return NULL;
    }
    work = (a[0] + a[1] + a[2] + a[5] + a[6]);
    total = (work + a[3] + a[4]);
    int percent = !prevTotal || total == prevTotal ? 0 : (int)(100 * (work - prevWork) / (double)(total - prevTotal));
    sprintf(buffer, fmt, percent);
    prevWork = work;
    prevTotal = total;
    return buffer;
}

const char*
battery_status(const char* fmt, const char* bat) {
    int perc;
    char path[255];
    static struct {
        char* state;
        char* symbol;
    } map[] = {
        { "Full", "Full" },
        { "Charging",    "+" },
        { "Discharging", "-" },
        { NULL, "" },
    };
    size_t i;
    char state[12];
    if(sprintf(path, "/sys/class/power_supply/%s/status", bat) < 0) {
        return NULL;
    }
    if(pscanf(path, "%12s", state) != 1) {
        return NULL;
    }
    for(i = 0; i < LEN(map) - 1; i++) {
        if(!strcmp(map[i].state, state)) {
            break;
        }
    }
    if(i == 0) {
        return map[i].symbol;
    }
    if(sprintf(path, "/sys/class/power_supply/%s/capacity", bat) < 0) {
        return NULL;
    }
    if(pscanf(path, "%d", &perc) != 1) {
        return NULL;
    }
    char* color = perc > 90 ? "green" : perc < 3 ? "red" : "cyan";
    sprintf(buffer, fmt, color, map[i].symbol, perc);
    return buffer;
}

const char*
ram_status(const char* fmt) {
    uintmax_t total, free, buffers, cached;
    if(pscanf("/proc/meminfo",
            "MemTotal: %ju kB\n"
            "MemFree: %ju kB\n"
            "MemAvailable: %ju kB\n"
            "Buffers: %ju kB\n"
            "Cached: %ju kB\n",
            &total, &free, &buffers, &buffers, &cached) != 5) {
        return NULL;
    }
    if(total == 0) {
        return NULL;
    }
    int used = (total - free) - buffers - cached;
    sprintf(buffer, fmt, used / 1024 / 1024.0, (int)(100 * (used) / total));
    return buffer;
}
const char*
run_command(const char* fmt __attribute__((unused)), const char* cmd) {
    char* p;
    FILE* fp;
    if(!(fp = popen(cmd, "r"))) {
        return NULL;
    }
    p = fgets(buf, sizeof(buf) - 1, fp);
    if(pclose(fp) < 0) {
        return NULL;
    }
    if(!p) {
        return NULL;
    }
    if((p = strrchr(buf, '\n'))) {
        p[0] = '\0';
    }
    return buf[0] ? buf : NULL;
}
#include <unistd.h>
const char*
run_command_silent(const char* fmt __attribute__((unused)), const char* cmd) {
    if(!fork()) {
        close(1);
        execl("/bin/sh", "/bin/sh", "-c", cmd, (char*)0);
    }
    return NULL;
}

struct arg {
    const char* (*func)();
    const char* fmt;
    const char* args;
    uint16_t cache;
};

static const struct arg args[] = {
    /* function format          argument */
    { datetime, "^fg(#FCD862)%a %b %d %H:%M:%S^fg()|", NULL, .cache = 1},
    { cpu_perc, "^fg(cyan)%02d%%",                   "",  },
    { cpu_freq_formatted, "(%02.1f)^fg()|",          "", },
    { ram_status, "^fg(green)%.2fG;%02d%%^fg()|",           "", },
    { battery_status, "^fg(%s)%s%d%%^fg()|",           "BAT0", },
    {run_command, "%s",           "weather.sh",        600},
};
char cache[LEN(args)][64];
#define DEFAULT_CACHE 10
/* interval between updates (in ms) */
const unsigned int interval = 1000;

static void
difftimespec(struct timespec* res, struct timespec* a, struct timespec* b) {
    res->tv_sec = a->tv_sec - b->tv_sec - (a->tv_nsec < b->tv_nsec);
    res->tv_nsec = a->tv_nsec - b->tv_nsec +
        (a->tv_nsec < b->tv_nsec) * 1E9;
}
void sleepRemainingInterval(struct timespec* start) {
    struct timespec current, diff, wait, intspec;
    clock_gettime(CLOCK_MONOTONIC, &current);
    difftimespec(&diff, &current, start);
    intspec.tv_sec = interval / 1000;
    intspec.tv_nsec = (interval % 1000) * 1E6;
    difftimespec(&wait, &intspec, &diff);
    if(wait.tv_sec >= 0)
        nanosleep(&wait, NULL);
}

int
main() {
    size_t i;
    uint32_t counter = 0;
    char status[255];
    const char* res;
    struct timespec start;
    while(1) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        char* statusEnd = status;
        status[0] = '\0';
        for(i = 0; i < LEN(args); i++) {
            if(!cache[i][0] || counter % (args[i].cache == 0 ? DEFAULT_CACHE : args[i].cache) == 0) {
                if(!(res = args[i].func(args[i].fmt, args[i].args))) {
                    res = "";
                    cache[i][0] = 0;
                }
                else
                    strcpy(cache[i], res);
            }
            else
                res = cache[i];
            strcpy(statusEnd, res);
            statusEnd += strlen(res);
        }
        puts(status);
        fflush(stdout);
        sleepRemainingInterval(&start);
        counter++;
    }
    return 0;
}
