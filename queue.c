#include "queue.h"

void qi_init(Queue_int *q){
  q->now=0;
  q->head=0;
  q->size=0;
}

int qi_capacity(Queue_int q){
  return QUEUE_INT_MAX-q.size;
}

int qi_size(Queue_int q){
  return q.size;
}

void qi_enqueue(Queue_int *q,int i){
  if(qi_capacity(*q)>0){
    q->buf[q->now]=i;
    q->now=(q->now+1)%QUEUE_INT_MAX;
    q->size+=1;
  }else{
    q->buf[q->now]=i;
    q->now=(q->now+1)%QUEUE_INT_MAX;
    q->head=q->now;
  }
}

int qi_dequeue(Queue_int *q){
  int ret;
  if(q->size>0){
    ret=q->buf[q->head];
    q->head=(q->head+1)%QUEUE_INT_MAX;
    q->size-=1;
  }
  return ret;
}

int qi_gettail(Queue_int*q){
  int ret;
  if(q->size>0){
    ret=q->buf[(q->head+q->size-1)%QUEUE_INT_MAX];
  }else{
    ret=-1;
  }
  return ret;
}

void qh_init(Queue_hist *q){
  q->now=0;
  q->head=0;
  q->size=0;
}

int qh_capacity(Queue_hist q){
  return QUEUE_HIST_MAX-q.size;
}

int qh_size(Queue_hist q){
  return q.size;
}

void qh_enqueue(Queue_hist *q,Mem_hist i){
  if(qh_capacity(*q)>0){
    q->buf[q->now]=i;
    q->now=(q->now+1)%QUEUE_HIST_MAX;
    q->size+=1;
  }else{
    q->buf[q->now]=i;
    q->now=(q->now+1)%QUEUE_HIST_MAX;
    q->head=q->now;
  }
}

Mem_hist qh_dequeue(Queue_hist *q){
  Mem_hist ret;
  if(q->size>0){
    ret=q->buf[q->head];
    q->head=(q->head+1)%QUEUE_INT_MAX;
    q->size-=1;
  }
  return ret;
}

Mem_hist qh_gettail(Queue_hist *q){
  Mem_hist ret;
  if(q->size>0){
    ret=q->buf[(q->head+q->size-1)%QUEUE_HIST_MAX];
  }else{
    ret.pc=-1;
  }
  return ret;
}


