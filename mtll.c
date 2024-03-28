#include "mtll.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

void mtll_free(int * is, float * fs, char * cs, char * ss, node * ns, struct list * lists){
    free(lists);
    free(ns);
    free(is);
    free(fs);
    free(cs);
    free(ss);

    is = NULL;
    fs = NULL;
    cs = NULL;
    ss = NULL;
    ns = NULL;
    lists = NULL;
}

node * mtll_create(int size, int index, node * ns, int * is, float * fs, char * cs, char * ss, struct list * lists, int * num_ns, int * num_is, int * num_fs, int * num_cs, int * num_ss) {
    node * nodes = &ns[(*num_ns + 1) * sizeof(node)];

    // check size value
    if (size == 0 || size == 45){
        nodes[0].index = index;
        nodes[0].data_type = 'e'; 
        nodes[0].data = NULL;
        nodes[0].next = NULL;
        if (size == 45){
            printf("INVALID COMMAND: NEW\n");
            nodes[0].data_type = 'n';
        }
        *num_ns = *num_ns + 1;
        return &nodes[0];
    }

    char buffer[100];

    int nested = 0;

    for (int i = 0; i < size; i++){
        if (fgets(buffer, sizeof(buffer), stdin) != NULL){
            char input[100];
            strcpy(input, buffer);

            // remove new line character
            input[strlen(input) - 1] = '\0';

            nodes[i].index = index;
            nodes[i].data_type = 'c';
            nodes[i].data = NULL;
            nodes[i].reference = NULL;

            // nest list
            if (input[0] == '{' && input[strlen(input) - 1] == '}'){
                nested = 1;

                int list_num;
                sscanf(input, "{%d}", &list_num);

                if (list_num > *num_ns || lists[list_num].head == NULL){
                    i--;
                    printf("INVALID COMMAND: NEW\n");
                    continue;
                }

                nodes[i].reference = &lists[list_num].head;
                nodes[i].data_type = 'r';

                // linking nodes
                if (i < size-1){
                    nodes[i].next = &nodes[i+1];
                }
                else{
                    nodes[i].next = NULL;
                }

                *num_ns = *num_ns + 1;
                continue;
            }

            // special case: empty character
            if (strlen(input) == 0){
                nodes[i].data_type = 'c';
                char s = '\0';
                ss[*num_ss*100] = s;
                *num_ss = *num_ss + 1;
            }

            // checking input type
            int is_integer = 1;
            for (size_t j = 0; j < strlen(input); j++){
                // not integer
                if (input[j] < 48 || input[j] > 57){
                    is_integer = 0;
                    // float
                    if (input[j] == '.') {
                        nodes[i].data_type = 'f';

                        float f;
                        sscanf(input, "%f", &f);
                        fs[*num_fs] = f;
                        nodes[i].data = &fs[*num_fs];
                        *num_fs = *num_fs + 1;
                    }
                    // string
                    else if (strlen(input) > 1) {
                        nodes[i].data_type = 's';
                        
                        strcpy(&ss[(*num_ss) * 100], input);

                        nodes[i].data = &ss[(*num_ss) * 100];
                        *num_ss = *num_ss + 1;
                    }
                    // char
                    else {
                        nodes[i].data_type = 'c';

                        char c;
                        sscanf(input, "%c", &c);
                        cs[*num_cs] = c;
                        nodes[i].data = &cs[*num_cs];
                        *num_cs = *num_cs + 1;
                    }
                    break;
                }
            }
            // interger
            if (is_integer == 1){
                nodes[i].data_type = 'i';

                int integer;
                sscanf(input, "%i", &integer);
                is[*num_is] = integer;
                nodes[i].data = &is[*num_is];
                *num_is = *num_is + 1;
            }

            // linking nodes
            if (i < size-1){
                nodes[i].next = &nodes[i+1];
            }
            else{
                nodes[i].next = NULL;
            }

            *num_ns = *num_ns + 1;
            continue;
        }
        break;
    }

    // display list content
    if (nested == 1){
        printf("nested ");
    }
    printf("list %d: ", index);
    mtll_view(&nodes[0], 0);
    
    return &nodes[0];
}

void mtll_view(node * head, int reference){
    node cursor = *head;

    while (1){
        if (cursor.next != NULL){
            // reference node
            if (cursor.data_type == 'r'){
                printf("{list %d}", cursor.index);
            }
            else if (cursor.data_type == 'i'){
                int * i = cursor.data;
                printf("%d", *i);
            }
            else if (cursor.data_type == 'f'){
                float * f = cursor.data;
                printf("%.02f", *f);
            }
            else if (cursor.data_type == 'c'){
                char * c = cursor.data;
                printf("%c", *c);
            }
            else if (cursor.data_type == 's'){
                char * s = cursor.data;
                printf("%s", s);
            }

            printf(" -> ");
            cursor = *cursor.next;
        }
        else{
            // reference node
            if (cursor.data_type == 'r'){
                printf("{list %d}", cursor.index);
            }
            else if (cursor.data_type == 'i'){
                int * i = cursor.data;
                printf("%d", *i);
            }
            else if (cursor.data_type == 'f'){
                float * f = cursor.data;
                printf("%.02f", *f);
            }
            else if (cursor.data_type == 'c'){
                char * c = cursor.data;
                printf("%c", *c);
            }
            else if (cursor.data_type == 's'){
                char * s = cursor.data;
                printf("%s", s);
            }

            if (reference == 0){
                printf("\n");
            }
            break;
        }
    }
}

void mtll_view_nested(node * head){
    node cursor = *head;

    while (1){
        if (cursor.next != NULL){
            // reference node
            if (cursor.data_type == 'r'){
                printf("{");
                mtll_view(*cursor.reference, 1);
                printf("}");
            }
            else if (cursor.data_type == 'i'){
                int * i = cursor.data;
                printf("%d", *i);
            }
            else if (cursor.data_type == 'f'){
                float * f = cursor.data;
                printf("%.02f", *f);
            }
            else if (cursor.data_type == 'c'){
                char * c = cursor.data;
                printf("%c", *c);
            }
            else if (cursor.data_type == 's'){
                char * s = cursor.data;
                printf("%s", s);
            }

            printf(" -> ");
            cursor = *cursor.next;
        }
        else{
            // reference node
            if (cursor.data_type == 'r'){
                printf("{");
                mtll_view(*cursor.reference, 1);
                printf("}");
            }
            else if (cursor.data_type == 'i'){
                int * i = cursor.data;
                printf("%d", *i);
            }
            else if (cursor.data_type == 'f'){
                float * f = cursor.data;
                printf("%.02f", *f);
            }
            else if (cursor.data_type == 'c'){
                char * c = cursor.data;
                printf("%c", *c);
            }
            else if (cursor.data_type == 's'){
                char * s = cursor.data;
                printf("%s", s);
            }

            printf("\n");
            break;
        }
    }
}

void mtll_type(node * head){
    node cursor = *head;

    while (1){
        if (cursor.next != NULL){
            // reference node
            if (cursor.data_type == 'r'){
                printf("{reference}");
            }
            else if (cursor.data_type == 'i'){
                printf("integer");
            }
            else if (cursor.data_type == 'f'){
                printf("float");
            }
            else if (cursor.data_type == 'c'){
                printf("char");
            }
            else if (cursor.data_type == 's'){
               printf("string");
            }

            printf(" -> ");
            cursor = *cursor.next;
        }
        else{
            // reference node
            if (cursor.data_type == 'r'){
                printf("{reference}");
            }
            else if (cursor.data_type == 'i'){
                printf("integer");
            }
            else if (cursor.data_type == 'f'){
                printf("float");
            }
            else if (cursor.data_type == 'c'){
                printf("char");
            }
            else if (cursor.data_type == 's'){
               printf("string");
            }

            printf("\n");
            break;
        }
    }
}

node * mtll_insert(node * head, int position, char element[], node * ns, int * is, float * fs, char * cs, char * ss, struct list * lists, int * num_ns, int * num_is, int * num_fs, int * num_cs, int * num_ss){
    node * bottom = head;

    // measuring list size
    int size = 0;
    while (bottom->next != NULL){
        size++;
        bottom = bottom->next;
    }

    // determine insert position
    int rev = 0;
    if (abs(position) > size){
        printf("INVALID COMMAND: INSERT\n");
        return NULL;
    }
    else if (position < 0){
        position = size + position + 1;
        rev = 1;
    }

    // find bottom node
    bottom = head;
    for (int i = 0; i < position; i++){
        bottom = bottom->next;
    }

    // nest list
    if (element[0] == '{' && element[strlen(element) - 1] == '}'){
        int list_num;
        char spare[100];
        strcpy(spare, element);

        sscanf(spare, "{%d}", &list_num);

        if (list_num > *num_ns || lists[list_num].head == NULL){
            printf("INVALID COMMAND: NEW\n");
        }
                
        node * i_head = NULL;
        node * i_tail = NULL;

        // find head and tail
        if (lists[list_num].head != NULL){
            i_head = lists[list_num].head;

            node * cursor = i_head;
            while(1){
                if (cursor->next == NULL){
                    i_tail = cursor;
                    break;
                }
                else {
                    cursor = cursor->next;
                }
            }
        }

        if (position == 0){
            printf("INVALID COMMAND: INSERT\n");
            return head;
        }
        else if (position != size || (position == size && rev == 0)){
            node * top = head;
            for (int i = 1; i < position; i++){
                top = top->next;
            }
            top->next = i_head;
            i_tail->next = bottom;
            bottom = i_head;
        }
        else if (position == size && rev == 1){
            bottom->next = i_head;
        }

        // display list content
        printf("list %d: ", head->index);
        mtll_view(head, 0);

        return head;
    }

    // node insertion
    node * i_node = &ns[(*num_ns + 1) * sizeof(node)];

    char input;
    char * i_ptr = &input;
    strcpy(i_ptr, element);

    i_node->data_type = 'c';
    i_node->data = NULL;

    // special case: empty character
    if (strlen(i_ptr) == 0){
        i_node->data_type = 'c';
        char s = '\0';
        ss[*num_ss*100] = s;
        *num_ss = *num_ss + 1;
    }

    // checking input type
    int is_integer = 1;

    // not integer
    for (size_t j = 0; j < strlen(i_ptr); j++){
        if (i_ptr[j] < 48 || i_ptr[j] > 57){
            is_integer = 0;

            // float
            if (i_ptr[j] == '.') {
                i_node->data_type = 'f';

                float f = atof(element);
                fs[*num_fs] = f;
                i_node->data = &fs[*num_fs];
                *num_fs = *num_fs + 1;
            }

            // string
            else if (strlen(element) > 1) {
                i_node->data_type = 's';

                strcpy(&ss[(*num_ss) * 100], element);

                i_node->data = &ss[(*num_ss) * 100];
                *num_ss = *num_ss + 1;
            }

            // char
            else {
                i_node->data_type = 'c';

                char c;
                sscanf(i_ptr, "%c", &c);
                cs[*num_cs] = c;
                i_node->data = &cs[*num_cs];
                *num_cs = *num_cs + 1;
            }
            break;
        }
    }
        
    // interger
    if (is_integer == 1){
        i_node->data_type = 'i';
        int integer;
        sscanf(i_ptr, "%i", &integer);
        is[*num_is] = integer;
        i_node->data = &is[*num_is];
        *num_is = *num_is + 1;
    }

    // insert new element to list
    *num_ns = *num_ns + 1;

    // insertion for empty lists
    if (bottom->data_type == 'e'){
        printf("list %d: ", head->index);
        mtll_view(i_node, 0);
        return i_node;
    }

    if (position == 0){
        i_node->next = bottom;
        head = i_node;
    }
    else if (position != size || (position == size && rev == 0)){
        node * top = head;
        for (int i = 1; i < position; i++){
            top = top->next;
        }
        top->next = i_node;
        i_node->next = bottom;
        bottom = i_node;
    }
    else if (position == size && rev == 1){
        bottom->next = i_node;
    }

    // display list content
    printf("list %d: ", head->index);
    mtll_view(head, 0);

    return head;
}

node * mtll_delete(node * head, int position, int index, node * ns, int * num_ns){
    node * target = head;

    // checking empty lists
    if (head->data_type == 'n'){
        printf("INVALID COMMAND: DELETE\n");
        return NULL;
    }

    // measuring list size
    int size = 0;
    while (target->next != NULL){
        size++;
        target = target->next;
    }

    // determine insert position
    if (abs(position) > size){
        printf("INVALID COMMAND: DELETE\n");
        return NULL;
    }
    else if (position < 0){
        position = size + position + 1;
    }

    // find target node
    target = head;
    for (int i = 0; i < position; i++){
        target = target->next;
    }

    // skipping target node
    if (position == 0){
        if (target->next == NULL){
            node * empty = &ns[(*num_ns + 1) * sizeof(node)];
            empty->index = index;
            empty->data_type = 'e'; 
            empty->data = NULL;
            empty->next = NULL;
            if (size == 45){
            printf("INVALID COMMAND: NEW\n");
            empty->data_type = 'n';
            }
            *num_ns = *num_ns + 1;
            head = empty;
        }
        else{
            head = target->next;
        }
    }
    else{
        node * top = head;
        for (int i = 1; i < position; i++){
            top = top->next;
        }

        if (position == size){
            top->next = NULL;
        }
        else if (position != size){
            top->next = target->next;
        }
    }

    // display list content
    printf("list %d: ", head->index + 1);
    mtll_view(head, 0);

    return head;
}
