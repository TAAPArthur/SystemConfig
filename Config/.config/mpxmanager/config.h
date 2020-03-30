#ifndef MPXMANAGER_CONFIG
#define MPXMANAGER_CONFIG
#define SHUTDOWN_CMD "notify-send 'System will shutdown in 1 min'; shutdown 1"
#define SUSPEND_CMD "systemctl suspend"
#define REBOOT_CMD "reboot"

void clean();
#endif
