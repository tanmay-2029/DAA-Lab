#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Structures
typedef struct {
    int *arr;
    int n;
} Array;

typedef struct Node {
    int key;
    struct Node *next;
} Node;

typedef struct DNode {
    int key;
    struct DNode *next, *prev;
} DNode;

typedef struct {
    DNode *head;
    DNode *tail;
} DList;

// --- ARRAY OPERATIONS ---
void ua_insert(Array *a, int x) { a->arr[a->n++] = x; }

int ua_search(Array *a, int k) {
    for (int i = 0; i < a->n; i++) if (a->arr[i] == k) return i;
    return -1;
}

void ua_delete(Array *a, int idx) {
    if (idx < 0 || idx >= a->n) return;
    a->arr[idx] = a->arr[a->n - 1];
    a->n--;
}

int ua_max(Array *a) {
    if (a->n == 0) return -1;
    int m = a->arr[0];
    for (int i = 1; i < a->n; i++) if (a->arr[i] > m) m = a->arr[i];
    return m;
}

void sa_insert(Array *a, int x) {
    int i = a->n - 1;
    while (i >= 0 && a->arr[i] > x) { a->arr[i + 1] = a->arr[i]; i--; }
    a->arr[i + 1] = x;
    a->n++;
}

int sa_search(Array *a, int k) {
    int low = 0, high = a->n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (a->arr[mid] == k) return mid;
        if (a->arr[mid] < k) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

void sa_delete(Array *a, int idx) {
    if (idx < 0 || idx >= a->n) return;
    for (int i = idx; i < a->n - 1; i++) a->arr[i] = a->arr[i + 1];
    a->n--;
}

// --- SINGLY LINKED LIST ---
Node* sl_insert_unsorted(Node *head, int x) {
    Node *n = (Node*)malloc(sizeof(Node));
    n->key = x; n->next = head;
    return n;
}

Node* sl_insert_sorted(Node *head, int x) {
    Node *n = (Node*)malloc(sizeof(Node));
    n->key = x;
    if (!head || head->key >= x) { n->next = head; return n; }
    Node *cur = head;
    while (cur->next && cur->next->key < x) cur = cur->next;
    n->next = cur->next; cur->next = n;
    return head;
}

Node* sl_search(Node *head, int k) {
    while (head && head->key != k) head = head->next;
    return head;
}

Node* sl_delete(Node *head, int k) {
    Node *cur = head, *prev = NULL;
    while (cur && cur->key != k) { prev = cur; cur = cur->next; }
    if (!cur) return head;
    if (!prev) head = cur->next;
    else prev->next = cur->next;
    free(cur);
    return head;
}

int sl_max_unsorted(Node *head) {
    if (!head) return -1;
    int m = head->key;
    for (Node *c = head; c; c = c->next) if (c->key > m) m = c->key;
    return m;
}

int sl_max_sorted(Node *head) {
    if (!head) return -1;
    while (head->next) head = head->next;
    return head->key;
}

void sl_free(Node *head) {
    while (head) { Node *tmp = head; head = head->next; free(tmp); }
}

// --- DOUBLY LINKED LIST ---
void dl_insert_unsorted(DList *l, int x) {
    DNode *n = (DNode*)malloc(sizeof(DNode));
    n->key = x; n->prev = NULL; n->next = l->head;
    if (l->head) l->head->prev = n; else l->tail = n;
    l->head = n;
}

void dl_insert_sorted(DList *l, int x) {
    DNode *n = (DNode*)malloc(sizeof(DNode)); n->key = x;
    if (!l->head || l->head->key >= x) {
        n->prev = NULL; n->next = l->head;
        if (l->head) l->head->prev = n; else l->tail = n;
        l->head = n;
    } else {
        DNode *cur = l->head;
        while (cur->next && cur->next->key < x) cur = cur->next;
        n->next = cur->next; n->prev = cur;
        if (cur->next) cur->next->prev = n; else l->tail = n;
        cur->next = n;
    }
}

DNode* dl_search(DList *l, int k) {
    DNode *cur = l->head;
    while (cur && cur->key != k) cur = cur->next;
    return cur;
}

void dl_delete_node(DList *l, DNode *n) {
    if (!n) return;
    if (n->prev) n->prev->next = n->next; else l->head = n->next;
    if (n->next) n->next->prev = n->prev; else l->tail = n->prev;
    free(n);
}

void dl_free(DList *l) {
    if (!l) return;
    DNode *cur = l->head;
    while (cur) { DNode *tmp = cur->next; free(cur); cur = tmp; }
    l->head = l->tail = NULL; // Reset pointers without freeing l
}

// --- MAIN BENCHMARK ---
int main() {
    int sizes[] = {1000, 5000, 10000, 20000, 40000};
    int num_sizes = 5;

    FILE *fp = fopen("results_q1.csv", "w");
    if (!fp) {
        perror("Failed to open file");
        return 1;
    }
    fprintf(fp, "structure,n,insert_ms,search_ms,delete_ms,max_ms\n");

    srand(42);

    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        int *data = (int*)malloc(sizeof(int) * n);
        for (int j = 0; j < n; j++) data[j] = rand() % 100000;
        int target = data[n / 2];

        clock_t t1, t2;
        double ins, sea, del, mx;

        // 1. Unsorted Array
        Array ua = {(int*)malloc(sizeof(int) * n), 0};
        t1 = clock(); for (int j = 0; j < n; j++) ua_insert(&ua, data[j]); t2 = clock();
        ins = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); int idx = ua_search(&ua, target); t2 = clock();
        sea = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); ua_delete(&ua, idx); t2 = clock();
        del = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); ua_max(&ua); t2 = clock();
        mx = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        fprintf(fp, "unsorted_array,%d,%f,%f,%f,%f\n", n, ins, sea, del, mx);
        free(ua.arr);

        // 2. Sorted Array
        Array sa = {(int*)malloc(sizeof(int) * n), 0};
        t1 = clock(); for (int j = 0; j < n; j++) sa_insert(&sa, data[j]); t2 = clock();
        ins = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); idx = sa_search(&sa, target); t2 = clock();
        sea = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); sa_delete(&sa, idx); t2 = clock();
        del = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); int sa_m = (sa.n > 0) ? sa.arr[sa.n - 1] : -1; t2 = clock();
        (void)sa_m;
        mx = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        fprintf(fp, "sorted_array,%d,%f,%f,%f,%f\n", n, ins, sea, del, mx);
        free(sa.arr);

        // 3. Singly Linked Unsorted
        Node *s_head = NULL;
        t1 = clock(); for (int j = 0; j < n; j++) s_head = sl_insert_unsorted(s_head, data[j]); t2 = clock();
        ins = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); sl_search(s_head, target); t2 = clock();
        sea = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); s_head = sl_delete(s_head, target); t2 = clock();
        del = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); sl_max_unsorted(s_head); t2 = clock();
        mx = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        fprintf(fp, "singly_linked_unsorted,%d,%f,%f,%f,%f\n", n, ins, sea, del, mx);
        sl_free(s_head);

        // 4. Singly Linked Sorted
        s_head = NULL;
        t1 = clock(); for (int j = 0; j < n; j++) s_head = sl_insert_sorted(s_head, data[j]); t2 = clock();
        ins = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); sl_search(s_head, target); t2 = clock();
        sea = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); s_head = sl_delete(s_head, target); t2 = clock();
        del = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); sl_max_sorted(s_head); t2 = clock();
        mx = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        fprintf(fp, "singly_linked_sorted,%d,%f,%f,%f,%f\n", n, ins, sea, del, mx);
        sl_free(s_head);

        // 5. Doubly Linked Unsorted
        DList dl = {NULL, NULL};
        t1 = clock(); for (int j = 0; j < n; j++) dl_insert_unsorted(&dl, data[j]); t2 = clock();
        ins = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); DNode *found = dl_search(&dl, target); t2 = clock();
        sea = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); dl_delete_node(&dl, found); t2 = clock();
        del = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); 
        if (dl.head) {
            int d_max = dl.head->key; 
            for(DNode *c = dl.head; c; c = c->next) if(c->key > d_max) d_max = c->key; 
            (void)d_max;
        }
        t2 = clock();
        mx = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        fprintf(fp, "doubly_linked_unsorted,%d,%f,%f,%f,%f\n", n, ins, sea, del, mx);
        dl_free(&dl);

        // 6. Doubly Linked Sorted
        dl.head = dl.tail = NULL;
        t1 = clock(); for (int j = 0; j < n; j++) dl_insert_sorted(&dl, data[j]); t2 = clock();
        ins = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); found = dl_search(&dl, target); t2 = clock();
        sea = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); dl_delete_node(&dl, found); t2 = clock();
        del = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        t1 = clock(); int d_sorted_max = dl.tail ? dl.tail->key : -1; (void)d_sorted_max; t2 = clock();
        mx = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        fprintf(fp, "doubly_linked_sorted,%d,%f,%f,%f,%f\n", n, ins, sea, del, mx);
        dl_free(&dl);

        free(data);
        printf("n = %d done\n", n);
    }

    fclose(fp);
    printf("Results written to results_q1.csv\n");
    return 0;
}