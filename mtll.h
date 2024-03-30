#ifndef MTLL_H
#define MTLL_H

typedef struct mtll{
    char data_type;
    int list_index;

    int i_value;
    float f_value;
    char c_value;
    char s_value[128];

    struct mtll * next;

    struct mtll * reference;
} node;

typedef struct list{
    struct mtll * head;
} list;

extern node * mtll_create(int size, node * nodes, list * lists, int * num_nodes, int * num_list);

extern void mtll_view(node * head);

extern void mtll_view_list(node * head);

extern void mtll_view_nested(node * head);

extern void mtll_type(node * head);

extern node * mtll_insert(node * nodes, list * lists, node * head, int position, int * num_nodes , char * element);

extern node * mtll_delete(node * nodes, node * head, int position);

#endif
