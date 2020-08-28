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

int solve_quiz(void* arg, int clnt_num);  //arg:(void*)&clnt_sock
void choose_quiz(int clnt_sock, int clnt_num);
void send_quiz(int clnt_sock, int quizNum, int clnt_num, int state); //state가 0이면 quiz만 1이면 quiz+answer 파일 보내기
int check_answer(int clnt_sock, int quizNum, int clnt_num);

int problem_state[5];

int solve_quiz(void* arg, int clnt_num)  //arg:(void*)&clnt_sock
{
        int quizNum;

        int clnt_sock = *((int*)arg); //소켓 연결
        choose_quiz(clnt_sock, clnt_num);
        return 0;
}

void choose_quiz(int clnt_sock, int clnt_num)
{
        int quizNum;
        char buf_get[255] = { 0 }; //퀴즈 번호 입력 받을 버퍼
        int tryOrNot=2; //1.재도전 2.다른퀴즈풀기 3.매뉴얼``
                while(1)
                {
                if (tryOrNot == 3) return;
                write(clnt_sock, "퀴즈 번호를 선택하세요 ( 1 2 3 4 5 ) 중\n", 255);
                printf("퀴즈 번호 매뉴얼 보냄\n");
                read(clnt_sock, buf_get, 255);
                printf("%s%d\n",buf_get,strlen(buf_get));
                buf_get[strlen(buf_get) - 1] = '\0'; //이걸 해야되는지 모르겠음
                printf("이젠 길이 %d\n\n",strlen(buf_get));
        //      printf("buf_get %c",buf_get[0]);
                quizNum = buf_get[0] - 48; //int형으로 바꿈
                printf("quizNum: %d\n",quizNum);
                //여기까지 됨ㅇㅇㅇㅇㅇㅇ
                if (strlen(buf_get) == 1 && quizNum >= 1 && quizNum <= 5) //입력한 숫자가 1자리이어야하고, 1에서 5사이 이어야함
                {
                        if (problem_state[quizNum - 1] == 0)
                        {
                                printf("Player%d choose %d\n", clnt_num, quizNum);
                                oneq_oneuser(quizNum, 1);
                                while (1)
                                {
                                        send_quiz(clnt_sock, quizNum, clnt_num, 0);
                                        tryOrNot = check_answer(clnt_sock, quizNum, clnt_num);
                                        if (tryOrNot == 1) continue;
                                        else if (tryOrNot == 2||tryOrNot==3) break;

                                }
                        }
                        else if (problem_state[quizNum - 1] == 1)
                        {
                                printf("Player%d가 진행 중인 퀴즈 번호를 택함\n", clnt_num);
                                write(clnt_sock, "누가 풀고 있습니다! 다른 번호를 선택하세요\n", 255);
                                memset(buf_get, 0, 255);
                        }
                        else if (problem_state[quizNum - 1] == 2)
                        {
                                printf("Player%d가 이미 맞힌 퀴즈 번호를 택함\n", clnt_num);
                                write(clnt_sock,"이 퀴즈는 이미 풀려있습니다. 정답과 함께 문제를 보여드리겠습니다!\n",255);
                                send_quiz(clnt_sock, quizNum, clnt_num, 1);
                                return;
                        }
                        else //아니면 다시 while문 반복
                        {
                                printf("Player%d choose WRONG NUMBER!\n", clnt_num);
                                write(clnt_sock, "다시 선택하세요\n", 255);
                                memset(buf_get, 0, 255);
                        }
                }
        }
        return ;
}//
void send_quiz(int clnt_sock, int quizNum, int clnt_num, int state) //state가 0이면 quiz만 1이면 quiz+answer 파일 보내기
{
        char quizBuffer[20]={0};//파일 이름 저장할 문자열 배열

        if(state == 0)//퀴즈 파일 보내줘야 할 경우
        {
                sprintf(quizBuffer, "quiz%d.txt", quizNum);
                open_file((void*)&clnt_sock, quizBuffer, clnt_num); //open_file에서 write해서 클라이언트한테 보내줌
        }
        else if(state==1)//답 파일 보내줘야 할 경우
        {
                sprintf(quizBuffer, "quiz%d_ans.txt", quizNum);
                printf("%s\n",quizBuffer);
                open_file((void*)&clnt_sock, quizBuffer, clnt_num); //open_file에서 write해서 클라이언트한테 보내줌
        }

        memset(quizBuffer, 0, 20);
        return;
}//open_file 파라미터: arg, fileName, clnt_num

int check_answer(int clnt_sock, int quizNum, int clnt_num)
{
        FILE* ansFile;
        char quizAns; //퀴즈 답
        char ansBuffer[30]={0}; //퀴즈파일이름 저장하는 버퍼
        char buf_playerAns[5]; // player가 적은 답

        printf("check_answer로 넘어옴\n");
        read(clnt_sock, buf_playerAns, 5);

	printf("%s\n",buf_playerAns);
	//여기까지 ㅇㅇㅇㅇㅇㅇㅇㅇㅇ
        sprintf(ansBuffer, "ans%d.txt", quizNum);
        printf("%s\n",ansBuffer);
        //여기까지 ㅇㅇㅇㅇㅇㅇㅇㅇㅇㅇ
        ansFile = fopen(ansBuffer, "r");
        printf("fopen 성공\n");
//      memset(ansBuffer, '0', 30);

        quizAns = fgetc(ansFile);
        printf("%c\n",quizAns);

        if (quizAns == buf_playerAns[0])
        {
                char one_keyword; //해당 번호 키워드 글자
                printf("Player%d wrote answer\n", clnt_num);
                write(clnt_sock, "ANSWER!\n", 255);

                //memset(buf_sendAll, 0, 120);

                //yecrb

                //until_keyword.txt 바꿔주기

                if (quizNum == 1)  one_keyword = 'y';
                else if (quizNum == 2) one_keyword = 'e';
                else if (quizNum == 3) one_keyword = 'c';
                else if (quizNum == 4) one_keyword = 'r';
                else if (quizNum == 5) one_keyword = 'b';

                char buf_sendAll[255]={0}; //정답인 경우 모두에게 보낼 메세지 저장하는 버퍼
                sprintf(buf_sendAll, "------<공지>Player %s가 quiz%d 을(/를) 맞혔습니다!------\n-----------해당 퀴즈의 키워드는 %c입니다.-----------\n", player[clnt_num-1].player_name, quizNum, one_keyword);

                send_all(buf_sendAll, 255);
                memset(buf_sendAll,0,255);

                FILE* fp = fopen("until_keyword.txt", "r"); //읽기,쓰기 가능
                char keyword[6]; //지금까지 맞춘 키워드 문자열
                fgets(keyword, 6, fp);

                printf("%s에서 ", keyword);
                keyword[quizNum - 1] = one_keyword; //키워드 추가해주기
                printf("%s로 추가해줌 \n", keyword);

                fclose(fp);

                fp = fopen("until_keyword.txt", "w"); //읽기,쓰기 가능
                fprintf(fp, keyword); //fp에 keyword 쓰기

                fclose(fp);

                memset(keyword,0,6);

                oneq_oneuser(quizNum,2);
                player[clnt_num - 1].score += 1;
                //count_score +1해줘야함 clnt_num-1에 +1 해줌
                return 3; //매뉴얼로 돌아가기
        }
        else
        {
                char user_select[10]={0}; //user가 y고르는지 n고르는지
                printf("Player%d Wrong\n", clnt_num);
                oneq_oneuser(quizNum, 0);
                write(clnt_sock, "Wrong!\n1. 재도전 2. 다른 퀴즈 풀기 3. 매뉴얼로 돌아가기\n", 255);
                while (1)
                {
                        read(clnt_sock, user_select, 10);
                        printf("%d\n\n",strlen(user_select));
                        if (strlen(user_select)-1 == 1)
                        {
                                if (user_select[0] == '1')  return 1; //재도전
                                else if (user_select[0] == '2') return 2; //다른 퀴즈 풀기
                                else if (user_select[0] == '3') return 3; //매뉴얼로 돌아가기
                        }
                        else write(clnt_sock, "다시 입력하세요!\n", 255);
                        memset(user_select,0,10);
                }
        }
        //rewind(ansFile);
}



