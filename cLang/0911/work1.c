//
// Created by csimo on 9/11/2024.
//

#include <stdio.h>

struct studentData{
    int student_no;
    char grade;
    double korean_score;
    double english_score;
} data;

int main()
{
    data.student_no = 20180018;
    data.grade = 'A';
    data.korean_score = 95.6;
    data.english_score = 89.2;

    printf("�й� : %d\n", data.student_no);
    printf("���� : %c\n", data.grade);
    printf("���� ���� : %.2f\n", data.korean_score);
    printf("���� ���� : %.2f\n", data.english_score);
}