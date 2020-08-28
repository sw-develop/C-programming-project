#include <stdio.h> //입출력
#include <stdlib.h> //문자열 변환, 의사 난수 생성
#include <unistd.h> //표준 심볼 상수 및 자료형
#include <string.h> //문자열 상수
#include <sys/socket.h> //소켓 연결
#include "global.h"

void read_manual(void* arg) {
    int clnt_sock = *((int*)arg); //연결하게 함
    int clnt_num = clnt_sock - 3;
    write(clnt_sock, "\n1. 퀴즈 풀기 2. 지금까지 keyword 보기 3. 키워드 맞추기 4. 종료하기\n", 255); //36
    printf("player%d 에게 메인 매뉴얼 보냄\n",clnt_num);
}
