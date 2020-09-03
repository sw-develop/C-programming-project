#ifndef SRC_SOLVE_QUIZ_H
#define SRC_SOLVE_QUIZ_H

int solve_quiz(void* arg, int clnt_num);  //arg:(void*)&clnt_sock
void choose_quiz(int clnt_sock, int clnt_num);
void send_quiz(int clnt_sock, int quizNum, int clnt_num, int state); //state가 0이면 quiz만 1이면 quiz+answer 파일 보내기
int check_answer(int clnt_sock, int quizNum, int clnt_num);

#endif //SRC_SOLVE_QUIZ_H
