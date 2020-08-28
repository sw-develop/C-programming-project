#include <stdio.h> //입출력
#include <stdlib.h> //문자열 변환, 의사 난수 생성
#include <unistd.h> //표준 심볼 상수 및 자료형
#include <string.h> //문자열 상수
#include <arpa/inet.h> //주소변환
#include <sys/socket.h> //소켓 연결
#include <pthread.h> //쓰레드 사용

#include "data.h" //oneq-oneuser, count_score
#include "file.h" //open_file
#include "send_all.h" //send_all
#include "global.h"

extern int clnt_cnt;

void check_keyword(void* arg, int clnt_num) {
    int clnt_sock = *((int *) arg);
    char buf_get[255] = { 0 };
    char keywordAns[10]={0};
    FILE *keywordFile;
    char line[100] = {0};
    int compare;

    keywordFile=fopen("keyword.txt","r");
    fgets(keywordAns, 10, keywordFile);
    //fclose(keywordFile);
    keywordAns[strlen(keywordAns)-1]='\0';
 //   printf("이제 길이%d\n",strlen(keywordAns));

    while (1)
        {
        compare=0;
        write(clnt_sock, "키워드를 입력해주세요.\n", 255);
        memset(buf_get,0,255);
        read(clnt_sock, buf_get, 255); // 함수 안에서 입력받는게 맞나..
        buf_get[strlen(buf_get)-1]='\0';
//      printf("입력받은키워드길이: %d\n",strlen(buf_get));
//      for ( int i = 0; i < 5 ; i++)
//      {
        if (strcmp (keywordAns,  buf_get)!=0) compare = 1;
//      }
       //keywordFile = fopen("keywordFile.txt", "r");
        // 아니면 keword가 담긴 배열을 선언 (keyword[5] = {c,y,b,e,r};

        //keywordAns[0]=fgetc(keywordFile); 문자열을 받아야함
        if (strlen(buf_get)==5 && compare == 0 ) //buf_get이 read_manual에 선언되어있는 배열이면 써도되나..?
        {
            FILE *fp;
            char buffer[200] = {0};
            printf("Player%d wrote keyword\n", clnt_num);
            write(clnt_sock, "ANSWER!\n", 255);

            fp=fopen("until_keyword.txt","w");
            fprintf(fp,"00000");
            fclose(fp);

            sprintf(buffer, "------<공지>Player %s 가 keyword을(/를) 맞혔습니다!------\n", player[clnt_num-1].player_name);
            send_all(buffer, 255);
	    printf("모든 플레이어에게 keyword 맞힌 거 공지 띄움\n");
            show_mvp();
            memset(buffer, '0', 255);
            //send_all("종료\n", strlen("종료\n"));
            for(int i=0;i<clnt_cnt;i++)
            {
                write(clnt_socks[i],"종료\n",255);
            }
            memset(buffer,'0',255);
           // memset(buf_get,'0',255);
        }
        else
        {
            printf("Player%d Wrong\n", clnt_num);
            write(clnt_sock, "WRONG..\n", 255);

            while (1) {
                memset(buf_get, 0, 255);
                write(clnt_sock, "\n1. 다시 풀기 2. 힌트 보기 3. 홈으로 돌아가기\n", 255);
                int err=read(clnt_sock,buf_get,255);
                if (strlen(buf_get) == 2) {
                    if (err < 0) {
                        memset(buf_get, '0', 255);
                        printf("Player%d 종료시킴\n", clnt_num);
                        write(clnt_sock, "종료\n", 255);
                        close(clnt_sock);
                        pthread_mutex_unlock(&mutx); // 쓰레드 놓아줌
                        break;
                    }
                    else if (buf_get[0] == '1')
                    {
                        printf("클라이언트가 키워드 다시 풀기를 선택함\n");
                        break;
                    }
                    else if (buf_get[0] == '2') {
                        FILE *hintFile;
                        hintFile = fopen("hintfile.txt", "r"); // 힌트파일 이름?
                        while (1) //파일 내용 한줄씩 입력받기
                        {
                            if (fgets(line, 100, hintFile) == NULL) {
                                //memset(line,'0',100);
                                break;
                            }
                            write(clnt_sock, line, 255);  //쓰고 저장
                            memset(line, '0', 255); //line초기화해서 (다음 줄 입력받기위해서)
                        }
                        //    write(clnt_sock, "end", 255); //clinet_sockfd에 "end"쓰면 클라이언트에서 end 읽으면 끝내게
                        printf("send hint to player%d\n\n", clnt_num); //hint보냇다는거 서버에 출력되게
                    } // 힌트보여주고 어디로 가..? 다시 1,2,3 매뉴얼로 가는 것 같은데 맞나???
                    else if (buf_get[0] == '3')
                    {
                        // while이 2개인데 어떻게 빠져나가...
                        return;
                    }
                }


                else
                {
                printf("Player%d가 잘못 입력함\n",clnt_num);
                write(clnt_sock,"다시 입력하세요\n",255);
                }

        }
    }
}
}

