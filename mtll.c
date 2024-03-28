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

    char buffer[256];

    int nested = 0;

    for (int i = 0; i < size; i++){
        if (fgets(buffer, sizeof(buffer), stdin) != NULL){
            char input[128];
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
            int data_type = 0; // 0:int 1:float 2:string 3:char 4:powerd int
            int * dt_ptr = &data_type;

            for (size_t j = 0; j < strlen(input); j++){
                if (input[j] <= '/' || input[j] >= ':'){
                    // "-" sign
                    if (input[j] == '-' && data_type == 0){
                        *dt_ptr = 0;
                    }
                    // float
                    else if (input[j] == '.' && !(data_type == 2 && data_type == 3)){
                        *dt_ptr = 1;
                    }
                    // char
                    else if (strlen(input) == 1){
                        *dt_ptr = 3;
                    }
                    // "e" sign
                    else if (input[j] == 'e' && data_type == 0){
                        *dt_ptr = 4;
                    }
                    // string
                    else if (strlen(input) > 1){
                        *dt_ptr = 2;
                    }
                }
            }

            // checking for NULL inputs
            if (strcmp(input, "") == 0) {
                *dt_ptr = 3;
            }

            // converting powerd int to int
            int power = 0;
            if (data_type == 4){
                char spare[128];
                strcpy(spare, input);

                int base;
                int factor;

                sscanf(spare, "%de%d", &base, &factor);
                
                double result = pow(base, factor);

                power = (int)result;
            }

            // insert data to node
                // int
            if (data_type == 0){
                nodes[i].data_type = 'i';

                int integer =atoi(input);

                is[*num_is * sizeof(int)] = integer;
                nodes[i].data = &is[*num_is * sizeof(int)];
                *num_is = *num_is + 1;
            }
                // powered int
            else if (data_type == 4){
                nodes[i].data_type = 'i';

                is[*num_is * sizeof(int)] = power;
                nodes[i].data = &is[*num_is * sizeof(int)];
                *num_is = *num_is + 1;
            }
                // float
            else if (data_type == 1) {
                nodes[i].data_type = 'f';

                float f = atof(input);

                fs[*num_fs * sizeof(float)] = f;
                nodes[i].data = &fs[*num_fs * sizeof(float)];
                *num_fs = *num_fs + 1;
            }
                // string
            else if (data_type == 2) {
                nodes[i].data_type = 's';

                for (int i = 0; i < strlen(input); i++){
                    ss[(*num_ss) * 128 + i] = input[i];
                }

                nodes[i].data = &ss[(*num_ss) * 128];
                *num_ss = *num_ss + 1;
            }
                // char
            else if (data_type == 3) {
                nodes[i].data_type = 'c';

                cs[*num_cs * sizeof(char)] = *input;
                nodes[i].data = &cs[*num_cs * sizeof(char)];
                *num_cs = *num_cs + 1;
            }
            else {
                printf("Interesting, this shouldn't be happening\n");
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
    printf("List %d: ", index);
    mtll_view(&nodes[0], 0);

    return &nodes[0];
}

void mtll_view(node * head, int reference){
    node cursor = *head;

    while (1){
        if (cursor.next != NULL){
            // reference node
            if (cursor.data_type == 'r'){
                printf("{List %d}", cursor.index);
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
                if (strcmp(cursor.data, "") != 0){
                    char * c = cursor.data;
                    printf("%c", *c);
                }
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
                printf("{List %d}", cursor.index);
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
                if (strcmp(cursor.data, "") != 0){
                    char * c = cursor.data;
                    printf("%c", *c);
                }
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
                printf("int");
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
                printf("int");
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
        printf("List %d: ", head->index);
        mtll_view(head, 0);

        return head;
    }

    // node insertion
    node * i_node = &ns[(*num_ns + 1) * sizeof(node)];

    char input[128];
    strcpy(input, element);

    i_node->data_type = 'c';
    i_node->data = NULL;

    // special case: empty character
    if (strlen(input) == 0){
        i_node->data_type = 'c';
        char s = '\0';
        ss[*num_ss*100] = s;
        *num_ss = *num_ss + 1;
    }

    // checking input type
    int data_type = 0; // 0:int 1:float 2:string 3:char 4:powerd int
    int * dt_ptr = &data_type;

    for (size_t j = 0; j < strlen(input); j++){
        if (input[j] <= '/' || input[j] >= ':'){
            // "-" sign
            if (input[j] == '-' && data_type == 0){
                *dt_ptr = 0;
            }
            // float
            else if (input[j] == '.' && !(data_type == 2 && data_type == 3)){
                *dt_ptr = 1;
            }
            // char
            else if (strlen(input) == 1){
                *dt_ptr = 3;
            }
            // "e" sign
            else if (input[j] == 'e' && data_type == 0){
                *dt_ptr = 4;
            }
            // string
            else if (strlen(input) > 1){
                *dt_ptr = 2;
            }
        }
    }

    // checking for NULL inputs
    if (strcmp(input, "") == 0) {
        *dt_ptr = 3;
    }

    // converting powerd int to int
    int power = 0;
    if (data_type == 4){
        char spare[128];
        strcpy(spare, input);

        int base;
        int factor;

        sscanf(spare, "%de%d", &base, &factor);
        
        double result = pow(base, factor);

        power = (int)result;
    }
        
    // insert data to node
        // int
    if (data_type == 0){
        i_node->data_type = 'i';

        int integer =atoi(input);

        is[*num_is * sizeof(int)] = integer;
        i_node->data = &is[*num_is * sizeof(int)];
        *num_is = *num_is + 1;
    }
        // powered int
    else if (data_type == 4){
        i_node->data_type = 'i';

        is[*num_is * sizeof(int)] = power;
        i_node->data = &is[*num_is * sizeof(int)];
        *num_is = *num_is + 1;
    }
        // float
    else if (data_type == 1) {
        i_node->data_type = 'f';

        float f = atof(input);

        fs[*num_fs * sizeof(float)] = f;
        i_node->data = &fs[*num_fs * sizeof(float)];
        *num_fs = *num_fs + 1;
    }
        // string
    else if (data_type == 2) {
        i_node->data_type = 's';

        for (int i = 0; i < strlen(input); i++){
            ss[(*num_ss) * 128 + i] = input[i];
        }

        i_node->data = &ss[(*num_ss) * 128];
        *num_ss = *num_ss + 1;
    }
        // char
    else if (data_type == 3) {
        i_node->data_type = 'c';

        cs[*num_cs * sizeof(char)] = *input;
        i_node->data = &cs[*num_cs * sizeof(char)];
        *num_cs = *num_cs + 1;
    }
    else {
        printf("Interesting, this shouldn't be happening\n");
    }

    // insert new element to list
    *num_ns = *num_ns + 1;

    // insertion for empty lists
    if (bottom->data_type == 'e'){
        printf("List %d: ", head->index);
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
    printf("List %d: ", head->index);
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
    printf("List %d: ", head->index + 1);
    mtll_view(head, 0);

    return head;
}
