#ifndef SRC_GLOBAL_H
#define SRC_GLOBAL_H


#define BUF_SIZE 255
#define MAX_CLNT 4
#include <pthread.h> //쓰레드 사용
struct player_info {
    int clnt_socknum; //소켓번호 저장
    char player_name[20]; //입력 받은 playerID 저장
    int score; //문제 맞힌 횟수 저장
    int rank; //사용자의 랭킹을 저장할 변수
};

struct player_info player[4]; //크기가 4인 구조체 배열 선언

//int clnt_cnt = 0; //클라이언트 수 카운팅
int clnt_socks[MAX_CLNT]; //최대 클라이언트 소켓
pthread_mutex_t mutx; //mutex : 쓰레드 동시접근 해결
//int problem_state[5] = { 0 };

#endif //SRC_GLOBAL_H

