#ifndef SRC_DATA_H
#define SRC_DATA_H

void oneq_oneuser(char problem, int change);//problem은 문제번호, change는 바뀔 상태
void show_mvp();
void count_score(int clnt_sock);//clnt_sock은 클라이언트가 할당받은 소켓 번호
void show_result();

#endif //SRC_DATA_H
