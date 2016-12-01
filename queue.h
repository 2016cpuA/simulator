#ifndef QUEUE_H
#define QUEUE_H
#define QUEUE_INT_MAX 16

#include "simulator.h"

typedef struct{
  int now;
  int head;
  int size;
  int buf[QUEUE_INT_MAX];
}Queue_int;

void qi_init(Queue_int *q);

int qi_capacity(Queue_int q);

void qi_enqueue(Queue_int *q,int i);

int qi_dequeue(Queue_int *q);

int qi_size(Queue_int q);

int qi_gettail(Queue_int *q);

#define QUEUE_HIST_MAX 256

typedef struct{
  int now;
  int head;
  int size;
  Mem_hist buf[QUEUE_HIST_MAX];
}Queue_hist;

void qh_init(Queue_hist *q);

int qh_capacity(Queue_hist q);

void qh_enqueue(Queue_hist *q,Mem_hist i);

Mem_hist qh_dequeue(Queue_hist *q);

int qh_size(Queue_hist q);

Mem_hist qh_gettail(Queue_hist *q);


#endif
