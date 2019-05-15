#include <stdlib.h>
#include "types.h"

typedef struct node {
  tlv_request_t request;
  struct node *next;
} node_t;

typedef struct request_queue {
  node_t *head;
  node_t *back;
  int size;
  void (*push)(struct request_queue *req_queue, tlv_request_t *request);
  void (*pop)(struct request_queue *req_queue);
  tlv_request_t (*front)(struct request_queue *req_queue);
} request_queue_t;

void request_queue_push(request_queue_t *req_queue, tlv_request_t *request) {
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

void request_queue_pop(request_queue_t *req_queue) {
  if (req_queue->head == NULL) return;

  node_t *poped_data = req_queue->head;
  req_queue->head = poped_data->next;
  --req_queue->size;
  free(poped_data);
}

tlv_request_t request_queue_front(request_queue_t *req_queue) {
  return req_queue->head->request;
}

void request_queue_init(request_queue_t *req_queue) {
  req_queue->head = NULL;
  req_queue->back = NULL;
  req_queue->size = 0;
  req_queue->push = request_queue_push;
  req_queue->pop = request_queue_pop;
  req_queue->front = request_queue_front;
}