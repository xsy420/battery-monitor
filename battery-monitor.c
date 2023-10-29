#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "unistd.h"

void read_from_file(char *filename, char *res) {
    FILE *fp = fopen(filename, "rb");
    fgets(res, 5, fp);
    fclose(fp);
}

int main() {
    bool warn_20 = false;
    bool warn_10 = false;
    char status[5] = {0};
    char capacity[5] = {0};
    while (true) {
        // 避免 CPU 占用过高
        sleep(1);
        read_from_file("/sys/class/power_supply/BAT0/status", status);
        // 如果还在充电或者充满了，continue;
        if (*status != 'D') {
            warn_10 = false;
            warn_20 = false;
            continue;
        }
        if (warn_10 == false || warn_20 == false) {
            read_from_file("/sys/class/power_supply/BAT0/capacity", capacity);
            // strlen = 36
            char command[40] = "zenity --warning --text 电量还剩";
            // cap 个位数
            if (*(capacity + 1) == '\n' && warn_10 == false) {
                command[36] = capacity[0];
                command[37] = '%';
                command[38] = '\0';
                system(command);
                warn_10 = true;
                continue;
            }
            // cap 两位数 且 20 <= cap <= 99
            if (*capacity > '1' && *capacity <= '9') continue;
            // 排除 cap == 100
            if (*(capacity + 2) != '\n') continue;
            if (warn_20 == false) {
                command[36] = capacity[0];
                command[37] = capacity[1];
                command[38] = '%';
                command[39] = '\0';
                system(command);
                warn_20 = true;
            }
        }
    }
}
