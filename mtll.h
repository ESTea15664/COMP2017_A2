#ifndef MTLL_H
#define MTLL_H

typedef struct mtll{
    int index;
    char data_type;
    void * data;
    struct mtll * next;
} node;

struct list {
    node * head;
};

extern void mtll_free(int * is, float * fs, char * cs, char * ss, node * ns, struct list * lists);

extern node * mtll_create(int size, int index, node * ns, int * is, float * fs, char * cs, char * ss, struct list * lists, int * num_ns, int * num_is, int * num_fs, int * num_cs, int * num_ss);

extern void mtll_view(node * head);

extern void mtll_view_nested(node * head);

extern void mtll_type(node * head);

extern node * mtll_insert(node * head, int position, char element[], node * ns, int * is, float * fs, char * cs, char * ss, struct list * lists, int * num_ns, int * num_is, int * num_fs, int * num_cs, int * num_ss);

extern node * mtll_delete(node * head, int position, int index, node * ns, int * num_ns);

#endif
