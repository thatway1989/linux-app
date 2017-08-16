/* appsw_at is Copyright (c) 2015-2025 906470132@qq.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 dated June, 1991, or
   (at your option) version 3 dated 29 June, 2007.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
     
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/select.h>
#include <signal.h>
#include <pthread.h>

#include "atfwd_daemon.h"
#include "libqrt_mc.h"

#define LOGI(...) fprintf(stderr, "I:" __VA_ARGS__)
pthread_mutex_t art_at_ctrMutex = PTHREAD_MUTEX_INITIALIZER; 


const char *str_at_res_code[ ] =
{
    "OK\r\n",
    "ERROR",
    "NO CARRIER\r\n",
    "NO DIALTONE\r\n",
    "BUSY\r\n",
    "NO ANSWER\r\n", 
    "DELAYED\r\n",
    "CONNECT\r\n",
    "RING\r\n",
    NULL
};


int resp_is_finished(char * buf)
{
    char *str = NULL;
    char **str_cmp = str_at_res_code;
    
    while(*str_cmp){
        if(NULL != strstr(buf,*str_cmp)){
            return 1;
        }else{
            str_cmp++;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int  ret = 0;
    int i=0;
    int fd=-1;
    char atcmd[CMD_LEN];
    int size;
    char receive[NV_LENGTH];
    char at_resp[RESP_MAX_COUNT];
    int at_resp_count=0;

    if(argc <= 0){
        LOGI("error: argc <= 0");
        exit(1);
    }

    pthread_mutex_lock(&art_at_ctrMutex);
    memset(at_resp, 0, sizeof(at_resp));
    memset(atcmd, 0, sizeof(atcmd));
    memset(receive,0,sizeof(receive));

    strcpy(atcmd,argv[1]);
    strcat(atcmd,"\r\n");
    at_resp_count = 0;

    fd = open(AT_DIVER_FILE, O_RDWR);
    //LOGI("atcmd:[%s]",atcmd);
    write(fd, atcmd, strlen(atcmd)+1);

    while(1){
        size = read(fd, receive, sizeof(receive)-1);        
        //LOGI("receive:[%d][%s]\n",size,receive);
        
        if(at_resp_count + size >= sizeof(at_resp)){
            printf("%s",at_resp);
            memset(at_resp, 0, sizeof(at_resp));
            memcpy(at_resp,receive,size);
            at_resp_count = size;
            at_resp[at_resp_count]=0;
        }else{
            memcpy(&at_resp[at_resp_count],receive,size);
            at_resp_count += size;
            at_resp[at_resp_count]=0;
        }

        if(resp_is_finished(at_resp))
        {
            printf("%s",at_resp);
            break;
        }
        memset(receive,0,sizeof(receive));
        usleep(10);
    }

    close(fd);
    pthread_mutex_unlock(&art_at_ctrMutex);

    return 0;
}

