# /proc/uptime
# #1 uptime of the system (seconds)
# /proc/[PID]/stat
# #14 utime - CPU time spent in user code, measured in clock ticks
# #15 stime - CPU time spent in kernel code, measured in clock ticks
# #16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
# #17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
# #22 starttime - Time when the process started, measured in clock ticks
# Hertz (number of clock ticks per second) of your system.
# In most cases, getconf CLK_TCK can be used to return the number of clock ticks.
# The sysconf(_SC_CLK_TCK) C function call may also be used to return the hertz value.

shopt -s extglob
uptime=$(cat /proc/uptime | awk '{print $1}')
printf "%0.s_" {1..90}
printf "\n"
printf "%-20s | %-20s | %-20s | %-20s\n" "Name" "PID" "PPID" "FD Open"
printf "%0.s_" {1..90}
printf "\n"
for file in /proc/+([0-9]); do
    _pid=$(cat $file/status | grep -w Pid | awk '{print $2}')
    _ppid=$(cat $file/status | grep -w PPid | awk '{print $2}')
    _name=$(cat $file/status | grep -w Name | awk '{print $2}')
    _fdcnt=$(sudo ls -1q $file/fd | wc -l)
    printf "%-20s | %-20s | %-20s | %-20s\n" $_name $_pid $_ppid $_fdcnt
done