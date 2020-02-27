#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"
#include "strnatcmp.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL) {
        printf("ERROR::q_new()::Allocate memory to queue failed.\n");
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL)
        return;
    list_ele_t *tmp = q->head;
    while (tmp) {
        q->head = tmp->next;
        free(tmp->value);
        free(tmp);
        tmp = q->head;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (q == NULL) {
        printf("ERROR::q_insert_head()::Pointer to queue is NULL.\n");
        return false;
    }
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        printf("ERROR::q_insert_head()::Allocate memory to newh failed.\n");
        return false;
    }
    int length = strlen(s);
    char *value = malloc(sizeof(char) * (length + 1));
    if (value == NULL) {
        printf("ERROR::q_insert_head()::Allocate memory to value failed.\n");
        free(newh);
        return false;
    }
    strncpy(value, s, length);
    value[length] = '\0';

    newh->next = q->head;
    newh->value = value;
    q->head = newh;
    if (q->tail == NULL)
        q->tail = newh;
    q->size += 1;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (q == NULL) {
        printf("ERROR::q_insert_tail()::Pointer to queue is NULL.\n");
        return false;
    }
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        printf("ERROR::q_insert_tail()::Allocate memory to newh failed.\n");
        return false;
    }
    int length = strlen(s);
    char *value = malloc(sizeof(char) * (length + 1));
    if (value == NULL) {
        printf("ERROR::q_insert_tail()::Allocate memory to value failed.\n");
        free(newh);
        return false;
    }
    strncpy(value, s, length);
    value[length] = '\0';

    newh->next = NULL;
    newh->value = value;
    if (q->tail) {
        q->tail->next = newh;
    } else {
        q->head = newh;
    }
    q->tail = newh;
    q->size += 1;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (q == NULL) {
        printf("ERROR::q_remove_head()::Pointer to queue is NULL.\n");
        return false;
    }
    if (q->head == NULL) {
        printf("ERROR::q_remove_head()::Queue is empty.\n");
        return false;
    }
    if (sp == NULL) {
        printf("ERROR::q_remove_head()::Pointer to string is NULL.\n");
        return false;
    }
    list_ele_t *tmp = q->head;
    int length = strlen(tmp->value);
    if (length < bufsize) {
        strncpy(sp, tmp->value, length);
        sp[length] = '\0';
    } else {
        strncpy(sp, tmp->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    if (q->tail == q->head)
        q->tail = NULL;
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);
    q->size -= 1;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q == NULL || q->head == NULL)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL || q->head == NULL || q->head->next == NULL)
        return;
    q->tail = q->head;
    list_ele_t *prevEle = NULL;
    list_ele_t *nextEle = q->head->next;
    while (nextEle) {
        q->head->next = prevEle;
        prevEle = q->head;
        q->head = nextEle;
        nextEle = nextEle->next;
    }
    q->head->next = prevEle;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

int min(int x, int y)
{
    return x < y ? x : y;
}

void q_sort(queue_t *q)
{
    if (q == NULL)
        return;

    int length = q->size;
    for (int width = 1; width < length; width *= 2) {
        list_ele_t tmp_head;
        list_ele_t *tmp = &tmp_head;
        list_ele_t *l_node = q->head;
        list_ele_t *r_node = q->head;
        for (int start = 0; start < length; start += width * 2) {
            int mid = min(start + width, length),
                end = min(start + width * 2, length);
            int ls = start;
            int le = mid;
            int rs = mid;
            int re = end;
            for (int i = 0; i < width; i++) {
                if (r_node->next)
                    r_node = r_node->next;
                else
                    break;
            }
            while (ls < le && rs < re) {
                if (strnatcmp(l_node->value, r_node->value) < 0) {
                    tmp->next = l_node;
                    tmp = tmp->next;
                    l_node = l_node->next;
                    tmp->next = NULL;
                    ls += 1;
                } else {
                    tmp->next = r_node;
                    tmp = tmp->next;
                    r_node = r_node->next;
                    tmp->next = NULL;
                    rs += 1;
                }
            }
            while (ls < le) {
                tmp->next = l_node;
                tmp = tmp->next;
                l_node = l_node->next;
                tmp->next = NULL;
                ls += 1;
            }
            while (rs < re) {
                tmp->next = r_node;
                tmp = tmp->next;
                r_node = r_node->next;
                tmp->next = NULL;
                rs += 1;
            }
            l_node = r_node;
        }
        q->head = tmp_head.next;
    }
}