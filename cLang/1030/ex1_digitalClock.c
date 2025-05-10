//
// Created by nodove on 24. 10. 30.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
  int h, m, s;
  printf("%02d : %02d : %02d");
  s++;
  if (s == 60)
  {
    m++;
    s = 0;
  }
  if (m == 60)
  {
    h++;
    m = 0;
  }
  if (h == 24)
  {
    h = m = s =0;
  }
  usleep(1000);


  return 0;

}