#include <stdio.h>
#include <stdlib.h> //문자열 변환 
#include <unistd.h> //표준 심볼 상수 및 자료형
#include <string.h> //문자열 상수
#include <arpa/inet.h> //주소변환
#include <sys/socket.h> //소켓 연결
#include <netinet/in.h> //IPv4 전용 기능
#include <pthread.h> //쓰레드 사용

extern int problem_state[5];
void show_keyword(void* arg, int clnt_num)
{
    int clnt_sock = *((int*)arg);
    FILE* fp;
    char until_keyword[6] = { 0 };
    char s;
    int i = 0;
    fp = fopen("until_keyword.txt", "r");
    while ((s = fgetc(fp)) != EOF) //file이 끝날 때까지
    {
        if (s == '0') continue; //'0'이면 while문으로 돌아감
        until_keyword[i] = s; //0아닌 것만 배열에 저장
        i++;
    }
    fclose(fp);
    write(clnt_sock,"*현재까지 획득한 키워드★\n",255);
    write(clnt_sock, until_keyword, 255); //5
   // write(clnt_sock, nn,255);
    printf("지금까지 맞힌 키워드 Player에게 보냄\n");
   
    write(clnt_sock, "\n\n▶ 아직 풀지 않은 문제 번호 : ",255);
    for(int i=0;i<5;i++)
    {
	if(problem_state[i]==0 || problem_state[i]==1)
	{
		char c[255]={0};
		sprintf(c,"<%d> ",i+1);
		write(clnt_sock,c,255);
	}
    }
    write(clnt_sock,"◀\n",255);
    return 0;
}
