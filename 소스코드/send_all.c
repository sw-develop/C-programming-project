#include <stdio.h> //입출력
#include <stdlib.h> //문자열 변환 
#include <unistd.h> //표준 심볼 상수 및 자료형
#include <string.h> //문자열 상수
#include <arpa/inet.h> //주소변환
#include <sys/socket.h> //소켓 연결
#include <netinet/in.h> //IPv4 전용 기능
#include <pthread.h> //쓰레드 사용
#include "global.h"

extern int clnt_cnt;
extern pthread_mutex_t mutx;

void send_all(char * msg, int len){
	int i;
	pthread_mutex_lock(&mutx);
	for(i = 0; i<clnt_cnt; i++)
		write(clnt_socks[i], msg, 255);
	//memset(msg, '0', len);
	pthread_mutex_unlock(&mutx);
} 
