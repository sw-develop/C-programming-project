#include <stdio.h> //입출력
#include <stdlib.h> //문자열 변환
#include <unistd.h> //표준 심볼 상수 및 자료형
#include <string.h> //문자열 상수
#include <arpa/inet.h> //주소변환
#include <sys/socket.h> //소켓 연결
#include <netinet/in.h> //IPv4 전용 기능
#include <pthread.h> //쓰레드 사용


void open_file(void *arg, char *openFileName, int clnt_num){//인자로 client의 주소? 와  open할 파일 이름, 클라이언트 번호를 받는다
        int clnt_sock=*((int*)arg);
        FILE *fileName;
        char line[255] = {0};

        fileName = fopen(openFileName, "r");

        while(1)
        {//파일 내용 한 줄 씩 입력 받고 클라이언트에게 보내기
                if(fgets(line, 255, fileName) == NULL)
                {
                        memset(line, 0, 255);
                        break;
                }
                write(clnt_sock, line, 255);

                memset(line, 0, 255);//line초기화하기
        }

        printf("send file %s to player %d\n\n", openFileName, clnt_num);//파일 보냈다는 것 서버에 출력하기
        rewind(fileName);
}
void open_file2(void *arg, char *openFileName, int clnt_num){//인자로 client의 주소? 와  open할 파일 이름, 클라이언트 번호를 받는다
        int clnt_sock=*((int*)arg);
        FILE *fileName;
        char line[255] = {0};

        fileName = fopen(openFileName, "r");

        while(1)
        {//파일 내용 한 줄 씩 입력 받고 클라이언트에게 보내기
                if(fgets(line, 255, fileName) == NULL)
                {
                        memset(line, 0, 255);
                        break;
                }
                write(clnt_sock, line, 255);
                sleep(1);
                memset(line, 0, 255);//line초기화하기
        }

        printf("send file %s to player %d\n\n", openFileName, clnt_num);//파일 보냈다는 것 서버에 출력하기
        rewind(fileName);
}


