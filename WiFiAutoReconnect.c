/*---------------------------------------------------------+
| WiFiAutoReconnect v1.0                                   |
+----------------------------------------------------------+
| Author: Wuifan <Admin@Wuifan.com> | License: MIT License |
+----------------------------------------------------------+
| Copyright (C) 2000-2016 Wuifan.com                       |
+---------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <windows.h>

/* Settings */
char *WiFiSSID = "WuifanWiFi";
char *CheckServer = "Wuifan.com";
int CheckInterval = 5000;

int PingCheck(int times) {
  char cmd[128];
  char Return[128];
  int state = 1;
  FILE *fp; 
  sprintf(cmd,"ping -n %d %s",times,CheckServer);
  if((fp = popen(cmd, "r")) == NULL) {
    printf("[X] Popen error!(open)\n");
    return -1;
  } else {
    memset(Return, 0, 128);
    while(fgets(Return, 128, fp) != NULL) {
      //printf("[Debug] %s",Return);
      if(strstr(Return,"请求找不到主机") != NULL ||
         strstr(Return,"could not find host") != NULL) {
   	    state = 0;
      }
    }
    if(pclose(fp) == -1) {
      printf("[X] Popen error!(close)\n");
      return -2;
    }
   	return state;
  }
}

void CheckAndRepair() {
  int ret = PingCheck(1);
  if(ret) {
    printf("[O] Net is connecting!\n");
  } else if(ret == 0) {
    printf("[?] Net may be disconnected!Retrying...\n");
    ret = PingCheck(5);
    if(ret) {
      printf("[O] Net is connecting!\n");
    } else if(ret == 0) {
      printf("[X] Net is disconnected! Reconnecting...\n");
      char cmd[128] = "netsh wlan disconnect&netsh wlan connect name=";
      strcat(cmd,WiFiSSID);
      system(cmd);
      printf("[?] Completed execution! Waiting to check...\n");
      Sleep(10000);
      CheckAndRepair(WiFiSSID);
    } else {
      CheckAndRepair(WiFiSSID);
    }
  } else {
    CheckAndRepair(WiFiSSID);
  }
}

int main() {
  char *pTitle = "WiFiAutoRoconnect - Wuifan.com";
  SetConsoleTitle(pTitle);
  while(1) {
    CheckAndRepair();
  	Sleep(CheckInterval);
  }
  return 0;
}
