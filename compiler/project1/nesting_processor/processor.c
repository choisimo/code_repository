#include "processor.h"
#include <stdlib.h>
#include <stdio.h>


/**
typedef struct PROCESSOR {
    int comment_depth;
    nstack* cstack;
    bool mode;          // comment mode (true: in comment mode)
} PROCESSOR;
*/

// 프로세서 생성
PROCESSOR* processor_create(int capacity) {
    PROCESSOR* proc = 
    (PROCESSOR *)malloc(sizeof(PROCESSOR));

    if (proc == NULL) {
        return NULL;
    }

    proc->comment_depth = 0;
    proc->mode = false;
    proc->cstack = nstack_creat(capacity);

    if (proc->cstack == NULL) {
        free(proc);
        return NULL;
    }

    return proc;
}

// 프로세서 해제
void processor_delete(PROCESSOR* proc) {
    if (proc != NULL) {
        nstack_delete(proc->cstack);
        free(proc);
    }
}

// 주석 시작 처리
void process_comment_start(PROCESSOR* proc) {
    proc->comment_depth++;
    push(proc->cstack, '/*');
    proc->mode = true;
}

// 주석 종료 처리
bool process_comment_end(PROCESSOR* proc) {
    if (!empty(proc->cstack)) {
        pop(proc->cstack);
        proc->comment_depth--;

        if (proc->comment_depth == 0) {
            proc->mode = false;
            return true;
        }
    }
    return false;
}

// 주석 상태 확인
bool is_in_comment(PROCESSOR* proc) {
    return proc->mode;
}

