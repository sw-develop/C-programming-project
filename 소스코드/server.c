#include <stdio.h> //입출력
#include <stdlib.h> //문자열 변환, 의사 난수 생성
#include <unistd.h> //표준 심볼 상수 및 자료형
#include <string.h> //문자열 상수
#include <arpa/inet.h> //주소변환
#include <sys/socket.h> //소켓 연결
#include <netinet/in.h> //IPv4 전용 기능
#include <pthread.h> //쓰레드 사i용

#include "global.h"
#include "start_game.h"

#define BUF_SIZE 255
#define MAX_CLNT 4


void error_handling(char* msg); //에러 핸들링 코드

int clnt_cnt=0;
int problem_state[5]={0};
//pthread_mutex_t mutx;

int main(int argc, char* argv[])
{
    int serv_sock, clnt_sock; //서버소켓, 클라이언트 소켓 변수 선언
    struct sockaddr_in serv_adr, clnt_adr; //서버,클라이언트 주소 변수 선언
    int clnt_adr_sz; //클라이언트 주소 크기
    char playerID[20];
    int again=0; //아이디 중복이면 1로 고치기

    pthread_t t_id; //쓰레드 아이디
    if (argc != 2) { //port번호 받기
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    pthread_mutex_init(&mutx, NULL); //mutex 초기화
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);  //서버 소켓 연결

    memset(&serv_adr, 0, sizeof(serv_adr)); //초기화
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) //bind
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1) //listen
        error_handling("listen() error");

    while (1) //값 받아오기
    {
        clnt_adr_sz = sizeof(clnt_adr); //크기 할당
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz); //소켓 할당
        pthread_mutex_lock(&mutx);
        read(clnt_sock, playerID, 20);
//      playerID[strlen(playerID)-1]='\0';
        for(int j=0; j<clnt_cnt+1;j++)
        {
                if(!strcmp(playerID,player[j].player_name))
                {
                        again=1;
                        break;
                }
        }
        //if(again ==1)
        //{
//              printf("Player ID가 중복됨\n");
//              again=0;
//              write(clnt_sock,"PlayerID 중복, 다시 시도하세요!\n",255);
//              write(clnt_sock,"종료\n",255);
//              clnt_sock--;
//      }
        memcpy(player[clnt_cnt].player_name, playerID, 20); //구조체 변수의 player_name에 값 저장
        if (clnt_sock > 7)
        {
            printf("Player 수가 초과되어, client를 게임 접속 불가하게 함\n");
            write(clnt_sock, "Player 수 초과, 게임에 접속 불가!\n", 255);
            write(clnt_sock, "종료\n", 255);
            //      close(clnt_sock);
        }
        else
        {
            while(again==1)
            {
		char buffer[20];
                char buf_ID[20];
                write(clnt_sock,"아이디 중복, 다시 입력하세요\n",255);
                read(clnt_sock,buffer,20);
		buffer[strlen(buffer)-1]='\0';
		sprintf(buf_ID,"[%s]",buffer);
                again=0;
                for(int i=0;i<clnt_cnt;i++)
                {
                        if(!(strcmp(buf_ID,player[i].player_name)))
                        {
                         again=1;
                        }
                }
                if(again==0) memcpy(player[clnt_cnt].player_name,buf_ID,20);
                memset(buf_ID,'0',20);
            }
           // pthread_mutex_lock(&mutx); //쓰레드 잡기
            player[clnt_cnt].clnt_socknum = clnt_sock;//구조체에 클라이언트 소켓 저장
            clnt_socks[clnt_cnt] = clnt_sock; //클라이언트 소켓을 배열에 저장
           // pthread_mutex_unlock(&mutx); // 쓰레드 놓아줌
            //pthread_create(&t_id, NULL, start_game, (void*)&clnt_sock); //쓰레드 생성, read_manual함수 실행
            //   pthread_detach(t_id); //쓰레드 소멸
            printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
           // memcpy(player[clnt_cnt].player_name, playerID, sizeof(playerID)); //구조체 변수의 player_name에 값 저장
            clnt_cnt++;

            printf("client 수: %d\n", clnt_cnt);
            printf("player%d ID:%s\n", clnt_cnt, player[clnt_cnt-1].player_name);
            pthread_mutex_unlock(&mutx);//쓰레드 놓아줌

        if(clnt_cnt != 4){//유저가 4명이 안 채워졌을 때
                write(clnt_sock,"--------------WAIT------------\n",255);
        }
        else{//유저가 4명 채워졌을 때
                for(int i = 0; i<clnt_cnt; i++){
                        write(clnt_socks[i], "-------------START!!----------\n", 255);
                }
                for(int j = 0; j<clnt_cnt; j++){
			if(clnt_socks[j]==0) continue;
			else{
                        pthread_mutex_lock(&mutx); //쓰레드 잡기
                        pthread_create(&t_id, NULL, start_game, (void*)&clnt_socks[j]); //쓰레드 생성, read_manual함수 실행
                        pthread_mutex_unlock(&mutx);//쓰레드 놓아줌
			}
                }


        }
        }
        }
  //  if(clnt_cnt==0)
//	{
	close(serv_sock);
   	return 0;
//	}
}


void error_handling(char* msg) //에러 핸들링 코드
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}
                                                                                             

