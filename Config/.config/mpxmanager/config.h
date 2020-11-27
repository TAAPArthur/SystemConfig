#ifndef CONFIG_H
#define CONFIG_H
#define SHUTDOWN_CMD "notify-send 'System will shutdown in 1 min'; shutdown 1"
#define SUSPEND_CMD "systemctl suspend"
#define REBOOT_CMD "reboot"

void clean();
#endif
