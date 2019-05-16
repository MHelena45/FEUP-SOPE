#include <stdlib.h>
#include "types.h"

#define QUEUE_TYPE tlv_request_t

typedef struct node {
  QUEUE_TYPE request;
  struct node *next;
} node_t;

typedef struct list_queue {
  node_t *head;
  node_t *back;
  int size;
  void (*push)(struct list_queue *req_queue, QUEUE_TYPE *request);
  void (*pop)(struct list_queue *req_queue);
  QUEUE_TYPE (*front)(struct list_queue *req_queue);
} list_queue_t;

void list_queue_push(list_queue_t *req_queue, QUEUE_TYPE *request) {
  node_t *new_data = (node_t *)malloc(sizeof(node_t));
  new_data->request = *request;
  new_data->next = NULL;
  if (req_queue->head == NULL) {
    req_queue->head = new_data;
    req_queue->back = new_data;
  } else {
    req_queue->back->next = new_data;
    req_queue->back = new_data;
  }
  ++req_queue->size;
}

void list_queue_pop(list_queue_t *req_queue) {
  if (req_queue->head == NULL) return;

  node_t *poped_data = req_queue->head;
  req_queue->head = poped_data->next;
  --req_queue->size;
  free(poped_data);
}

QUEUE_TYPE list_queue_front(list_queue_t *req_queue) {
  return req_queue->head->request;
}

void list_queue_init(list_queue_t *req_queue) {
  req_queue->head = NULL;
  req_queue->back = NULL;
  req_queue->size = 0;
  req_queue->push = list_queue_push;
  req_queue->pop = list_queue_pop;
  req_queue->front = list_queue_front;
}