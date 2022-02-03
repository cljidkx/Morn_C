/*
Copyright (C) 2019-2020 JingWeiZhangHuai <jingweizhanghuai@163.com>
Licensed under the Apache License, Version 2.0; you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
*/

//build_mingw: gcc -O2 -fopenmp test_UDP.c -o test_UDP.exe -lmorn -lWs2_32
//build_gnu: gcc -O2 -fopenmp test_UDP.c -o test_UDP.exe -I ../include/ -L ../lib/x64_gnu/ -lmorn -lm
//build_msvc: cl.exe -O2 -openmp -I ..\include\ ..\lib\x64_msvc\libmorn.lib .\test_UDP.c

#include "morn_util.h"

void test_send()
{
    char addr[32];

    char data[64];
    for(int i=0;i<100;i++)
    {
        mRandString(data,32,64);
        // printf("data=%s\n",data);
        // sprintf(addr,"localhost:%d",rand()%4+1000);
        sprintf(addr,"192.168.5.255:%d",rand()%4+1000);
        char *ip=mUDPWrite(addr,data,DFLT);
        //char *ip=mUDPWrite("localhost:1234",data,DFLT);
        printf("send: to %s, size=%zd, data is %s\n",ip,strlen(data),data);
        mSleep(100);
    }
}



void test_recive()
{
    int wait_time=150;
    mPropertyWrite("UDP","wait_time",&wait_time,sizeof(int));
    
    char data[64];
    while(1)
    {
        int size=64;
        char *ip;
        ip=mUDPRead(":1002",data,&size);
        if(ip==NULL) printf("recive2 error\n");
        else if(strcmp(data,"exit")==0) break;
        else printf("recive: from %s, size=%d, data is %s\n",ip,size,data);
    }
}

int main(int argc,char *argv[])
{
         if(strcmp(argv[1],"send"  )==0) test_send();
    else if(strcmp(argv[1],"recive")==0) test_recive();
    else mUDPWrite("localhost:1234",argv[1],DFLT);
    return 0;
}

