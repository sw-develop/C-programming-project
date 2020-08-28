#include <stdio.h> //입출력
#include <stdlib.h> //문자열 변환, 의사 난수 생성
#include <unistd.h> //표준 심볼 상수 및 자료형
#include <string.h> //문자열 상수
#include <arpa/inet.h> //주소변환
#include <sys/socket.h> //소켓 연결
#include <netinet/in.h> //IPv4 전용 기능
#include <pthread.h> //쓰레드 사용

#include "solve_quiz.h"
#include "global.h"
#include "check_keyword.h"
#include "show_keyword.h"
#include "read_manual.h"
#include "file.h"

extern int clnt_socks[4];
extern int clnt_cnt;
extern int problem_state[5];
void* start_game(void* arg)
{
    char buf_get[255] = { 0 };
    char state = '0';
    char buffer[255] = { 0 };
    int clnt_sock = *((int*)arg); //연결하게 함
    int clnt_num = clnt_sock - 3;//플레이어 번호

    FILE* fp;
    fp=fopen("until_keyword.txt","w");
        fprintf(fp,"00000");
        fclose(fp);

    while (1)
    {
        open_file2((void*)&clnt_sock, "howtoplay.txt", clnt_num);
        read(clnt_sock, buf_get, 255);
        char ch = buf_get[0];
        memset(buf_get, '0', 255);
        if (ch != '0')
            break;
    }
    while (1)
    {
        read_manual((void*)&clnt_sock);//메인 매뉴얼 함수 실행
        read(clnt_sock, buf_get, 255);
        char ch = buf_get[0];
        memset(buf_get, '0', 255);

        if (ch == '1')
            solve_quiz((void*)&clnt_sock, clnt_num);
        else if (ch == '2')
            show_keyword((void*)&clnt_sock, clnt_num);
        else if (ch == '3')
        {
            for (int i = 0; i < 5; i++)
            {
                if (problem_state[i] != 2) state = '1';
            }
            if (state == '1')
            {
                printf("아직 풀지 않은 퀴즈가 남아있어서 Player%d가 접근 못하게 함\n",clnt_num);
                write(clnt_sock, "아직 풀지 않은 퀴즈가 남아있습니다.\n퀴즈 풀기를 선택해 남아 있는 퀴즈를 풀어주세요!\n", 255);
                //memset(buf_get,'0',255);
                state = '0';
            }

            else if (state == '0') check_keyword((void*)&clnt_sock, clnt_num);
        }
        else if (ch == '4')
        {
            printf("player%d 종료시킴\n", clnt_num);
            write(clnt_sock, "종료\n", 255);
	    printf("종료 후 클라이언트 수: %d\n",clnt_cnt);
	    clnt_socks[clnt_num-1]=0;
	   // clnt_cnt--;
            sprintf(buffer, "\n----<공지>player%d가 게임을 떠났습니다.-----\n", clnt_num);
            send_all(buffer, 255);
            break;
        }
        else {
            write(clnt_sock, "\n잘못된 입력입니다 다시 입력해주세요\n", 255);
            continue;
        }
    }


    return 0;
}

