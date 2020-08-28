#include <stdio.h> //입출력
#include <stdlib.h> //문자열 변환
#include <unistd.h> //표준 심볼 상수 및 자료형
#include <string.h> //문자열 상수
#include <arpa/inet.h> //주소변환
#include <sys/socket.h> //소켓 연결
#include <netinet/in.h> //IPv4 전용 기능
#include <pthread.h> //쓰레드 사용
#include "global.h"
#include "send_all.h"

extern int clnt_cnt;

//oneq_oneuser, show_mvp, count_score
int problem_state[5];
pthread_mutex_t mutx;
//이 함수 언제 호출할 건지 정해야 함
void oneq_oneuser(int problem, int change){//problem은 문제번호, change는 바뀔 상태

    problem_state[problem-1] = change; //problem 번호에 해당하는 배열에 change변수 대입하기
    //problem이 어떤 거 전달되는 건지 보고 problem_number - 1해야 되는지 결정해야 함
   // System.out.printf("문제 번호[%d] : 문제 상태 [%d]",problem-1, problem_state[problem-1]);

}

//메뉴얼 3번 선택 후 키워드 최종 맞혔을 때 호출 될 함수
void show_mvp(){
    int k = 0, m = 0;//동점자 계산 변수

    pthread_mutex_lock(&mutx);

    for(int i=0; i<clnt_cnt; i++){
        for(int j = 0; j<clnt_cnt; j++){
            if(player[i].score < player[j].score){//다른 원소 값보다 작을 때
                player[i].rank += 1; //랭킹이 1씩 추가
            }
        }
    }
   // send_all("오늘의 MVP!!!\n", 255);
        for(int i =0; i<clnt_cnt; i++){
                write(clnt_socks[i], "오늘의 MVP!!\n",255);
        }


    for(int i = 0; i<clnt_cnt; i++){
        char buffer[255];
        if(player[i].rank == 0)
        {
            sprintf(buffer, "%s\n", player[i].player_name);
           // send_all(buffer, 255);
            for(int i =0; i<clnt_cnt; i++){
                write(clnt_socks[i], buffer, 255);
            }
            //memset(buffer, '0', 255);
        }
        memset(buffer, '0' , 255);
    }

  //  send_all("사용자의 순위 !!", 255);
        for(int i = 0 ;i<clnt_cnt; i++){
                write(clnt_socks[i], "\n사용자의 순위 !!\n", 255);

        }
    for(int i = 0; i<clnt_cnt; i++){
        for(int j = 0 ; j<clnt_cnt; j++){
            if(i==player[j].rank){
                k++; //동점자 계산
                m = j;
            }
        }

        if(k==1){//동점자가 없을 때
            char buffer[255];
            sprintf(buffer,"%d 등 : %s, %d 점\n", i+1, player[m].player_name, player[m].score);
           // send_all(buffer, 255);
            for(int i = 0; i<clnt_cnt; i++){
                write(clnt_socks[i], buffer, 255);
}
            memset(buffer,'0',255);
        }
        else if(k>1){
            char buffer[255];
            sprintf(buffer, "아래 %d 명은 동점자입니다.\n", k);
           // send_all(buffer, 255);
            for(int i = 0; i< clnt_cnt; i++){
                write(clnt_socks[i], buffer, 255);
}
            memset(buffer,'0',255);
            for(int l = 0; l < clnt_cnt; l++){
                if(i == player[l].rank){//i 값과 같은 rank값을 가지는 원소 찾기

                    sprintf(buffer,"%d등 : %s, %d점\n",i+1,player[l].player_name,player[l].score);
                   // send_all(buffer, 255);
                    for(int i = 0; i < clnt_cnt; i++){
                write(clnt_socks[i], buffer, 255);
}
                    memset(buffer, '0', 255);
                }
            }
        }
        k=m=0;
    }


    pthread_mutex_unlock(&mutx);
}

void count_score(int clnt_sock){ //퀴즈 맞히면 횟수 증가 메서드
    for(int i = 0; i < MAX_CLNT; i++){
        if(player[i].clnt_socknum == clnt_sock) //전달 받은 clnt_sock과 일치하는 것 찾기
            player[i].score++; //맞힌 횟수 증가
    }
}

