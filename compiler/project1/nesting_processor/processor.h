#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "nstack.h"

typedef struct PROCESSOR {
    int comment_depth;
    nstack* cstack;
    bool mode;          // comment mode (0: no, 1: yes)
} PROCESSOR;

// 프로세서 생성
PROCESSOR* processor_create(int capacity);

// 프로세서 해제
void processor_delete(PROCESSOR* processor);

// 주석 시작 처리
void process_comment_start(PROCESSOR* processor);

// 주석 종료 처리
bool process_comment_end(PROCESSOR* processor);

// 주석 상태 확인
bool is_in_comment(PROCESSOR* processor);

#endif
