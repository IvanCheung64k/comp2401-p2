#include "defs.h"
#include <stdlib.h>
#include <stdio.h>

/* Event functions */

/**
 * Initializes an `Event` structure.
 *
 * Sets up an `Event` with the provided system, resource, status, priority, and amount.
 *
 * @param[out] event     Pointer to the `Event` to initialize.
 * @param[in]  system    Pointer to the `System` that generated the event.
 * @param[in]  resource  Pointer to the `Resource` associated with the event.
 * @param[in]  status    Status code representing the event type.
 * @param[in]  priority  Priority level of the event.
 * @param[in]  amount    Amount related to the event (e.g., resource amount).
 */
void event_init(Event *event, System *system, Resource *resource, int status, int priority, int amount) {
    event->system = system;
    event->resource = resource;
    event->status = status;
    event->priority = priority;
    event->amount = amount;
}

/* EventQueue functions */

/**
 * Initializes the `EventQueue`.
 *
 * Sets up the queue for use, initializing any necessary data (e.g., semaphores when threading).
 *
 * @param[out] queue  Pointer to the `EventQueue` to initialize.
 */
void event_queue_init(EventQueue *queue) {
    queue->head = NULL;
    queue->size = 0;
    sem_init(&queue->mutex, 0, 1);
}

/**
 * Cleans up the `EventQueue`.
 *
 * Frees any memory and resources associated with the `EventQueue`.
 * 
 * @param[in,out] queue  Pointer to the `EventQueue` to clean.
 */
void event_queue_clean(EventQueue *queue) {
    EventNode* temp;
    for (int i = 0; i<queue->size;i++){
        temp = queue->head->next;
        free(queue->head);
        queue->head = temp;
    }
    sem_destroy(&queue->mutex);
}

/**
 * Pushes an `Event` onto the `EventQueue`.
 *
 * Adds the event to the queue in a thread-safe manner, maintaining priority order (highest first).
 *
 * @param[in,out] queue  Pointer to the `EventQueue`.
 * @param[in]     event  Pointer to the `Event` to push onto the queue.
 */
void event_queue_push(EventQueue *queue, const Event *event) {
    EventNode* temp = queue->head;
    EventNode* insert, *insertPrev;
    insert = (EventNode*)malloc(sizeof(EventNode));
    insertPrev = NULL;
    insert->event = (*event);
    sem_wait(&queue->mutex);
    while (temp != NULL){
        if (temp->event.priority < insert->event.priority){
            break;
        }
        insertPrev = temp;
        temp = temp->next;
    }
    if (insertPrev == NULL){
        queue->head = insert;
    }
    else {
        insertPrev->next = insert;
    }
    insert->next = temp;
    queue->size++;
    sem_post(&queue->mutex);
}

/**
 * Pops an `Event` from the `EventQueue`.
 *
 * Removes the highest priority event from the queue in a thread-safe manner.
 *
 * @param[in,out] queue  Pointer to the `EventQueue`.
 * @param[out]    event  Pointer to the `Event` structure to store the popped event.
 * @return               Non-zero if an event was successfully popped; zero otherwise.
 */
int event_queue_pop(EventQueue *queue, Event *event) {
    // Temporarily, this only returns 0 so that it is ignored 
    // during early testing. Replace this with the correct logic.
    sem_wait(&queue->mutex);
    if (queue->head != NULL){
        
        EventNode *temp = queue->head;
       (*event) = temp->event;
        queue->head = temp->next;
        free(temp);
        queue->size--;
        sem_post(&queue->mutex);
        return 1;
    }
    sem_post(&queue->mutex);
    return 0;
}
