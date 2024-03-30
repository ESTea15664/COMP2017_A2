#include "mtll.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int my_pow(int base, int expo){
    int result = 1;

    if (expo >= 0){
        for (int i = 0; i < expo; i++){
            result *= base;
        }
    }
    else{
        expo = -expo;
        for (int i = 0; i < expo; i++){
            result /= base;
        }
    }

    return result;
}

node * mtll_create(int size, node * nodes, list * lists, int * num_nodes, int * num_lists){
    char buffer[128];

    node * head = NULL;

    int nest = 0;

    // check for special cases (size = 0 and size < 0)
    if (size == 0){
        node * new_node = &nodes[*num_nodes * sizeof(node)];
        new_node->list_index = *num_lists;
        new_node->data_type = 'e';
        new_node->next = NULL;

        *num_nodes = *num_nodes + 1;

        return new_node;
    }
    else if (size < 0){
        return NULL;
    }

    // loop until empty line or reached size
    for (int i = 0; (i < size) && fgets(buffer, sizeof(buffer), stdin) != NULL; i++){
        // copy input from buffer
        char input[128];
        strcpy(input, buffer);
        // remove new line character
        input[strlen(input) - 1] ='\0';

        // node creation
        node * new_node = &nodes[*num_nodes * sizeof(node)];
        new_node->list_index = *num_lists;

        if (head == NULL){
            head = new_node;
        }
            // nested list
        if (input[0] == '{' && input[strlen(input) - 1] == '}'){
            // reminder for print
            nest = 1;

            // get list index from input
            int target_index;

            char copy[128];
            strcpy(copy, input);

            sscanf(copy, "{%d}", &target_index);

            // reference list in node
            new_node->data_type = 'r';
            new_node->reference = lists[target_index * sizeof(node)].head;
        }
            // simple list
        else{
            // check data type
            int data_type = 0; // 0:int 1:power 2:float 3:char 4:string
            int * dt_ptr = &data_type;

            // speical case: empty input
            if(strlen(input) == 0){
                *dt_ptr = 3;
            }
            // non-empty input
            else{
                for(int j = 0; j < strlen(input); j++){
                    // if input[j] is not a number
                    if (input[j] < '0' || input[j] > '9'){
                        // if input only has one char
                        if (strlen(input) == 1){
                            *dt_ptr = 3;
                        }
                        // if input[j] is a minus sign
                        else if (input[j] == '-' && (data_type == 0 || data_type == 1)){
                            continue;
                        }
                        // if input[j] is a power sign
                        else if (input[j] == 'e' && data_type == 0){
                            *dt_ptr = 1;
                        }
                        // if input[j] is a dot
                        else if (input[j] == '.' && data_type == 0){
                            *dt_ptr = 2;
                        }
                        // else ipnut is a string
                        else {
                            *dt_ptr = 4;
                        }
                    }
                }
            }

            // insert data to node
                // int
            if (data_type == 0){
                new_node->i_value = atoi(input);
                new_node->data_type = 'i';
            }
                // power
            else if (data_type == 1){
                int base;
                int expo;
                sscanf(input, "%de%d", &base, &expo);

                new_node->i_value = my_pow(base, expo);
                new_node->data_type = 'i';
            }
                // float
            else if (data_type == 2){
                new_node->f_value = atof(input);
                new_node->data_type = 'f';
            }
                // char
            else if (data_type == 3){
                new_node->c_value = input[0];
                new_node->data_type = 'c';   
            }
                // string
            else if (data_type == 4){
                strcpy(new_node->s_value, input);
                new_node->data_type = 's'; 
            }
        }

        // link nodes
        *num_nodes = *num_nodes + 1;
        if (i < size - 1){
            new_node->next = &nodes[*num_nodes * sizeof(node)];
        }
        else{
            new_node->next = NULL;
        }
    }

    // print list
    if (nest == 0){
        printf("List %d: ", head->list_index);
    }
    else{
        printf("Nested %d: ", head->list_index);
    }
    
    mtll_view(head);
    printf("\n");

    return head;
}

void mtll_view(node * head){
    node * cursor = head;

    while(cursor != NULL){
        if (cursor->data_type == 'i'){
            printf("%d", cursor->i_value);
        }
        else if (cursor->data_type == 'f'){
            printf("%.02f", cursor->f_value);
        }
        else if (cursor->data_type == 'c'){
            printf("%c", cursor->c_value);
        }
        else if (cursor->data_type == 's'){
            printf("%s", cursor->s_value);
        }
        else if (cursor->data_type == 'r'){
            printf("{List %d}", cursor->reference->list_index);
        }

        // print arrow if next != NULL
        if (cursor->next != NULL){
            printf(" -> ");
        }
        cursor = cursor->next;
    }
}

void mtll_view_nested(node * head){
    node * cursor = head;

    while(cursor != NULL){
        if (cursor->data_type == 'i'){
            printf("%d", cursor->i_value);
        }
        else if (cursor->data_type == 'f'){
            printf("%.02f", cursor->f_value);
        }
        else if (cursor->data_type == 'c'){
            printf("%c", cursor->c_value);
        }
        else if (cursor->data_type == 's'){
            printf("%s", cursor->s_value);
        }
        else if (cursor->data_type == 'r'){
            printf("{");
            mtll_view(cursor->reference);
            printf("}");
        }

        // print arrow if next != NULL
        if (cursor->next != NULL){
            printf(" -> ");
        }
        cursor = cursor->next;
    }
}

void mtll_view_list(node * head){
    if (head == NULL){
        return;
    }

    int nested = 0;
    node * cursor = head;

    while(cursor != NULL){
        if (cursor->data_type == 'r'){
            nested = 1;
            break;
        }
        cursor = cursor->next;
    }

    if (nested == 0){
        printf("List %d\n", head->list_index);
    }
    else if (nested == 1){
        printf("Nested %d\n", head->list_index);
    }
}

void mtll_type(node * head){
    node * cursor = head;

    while(cursor != NULL){
        if (cursor->data_type == 'i'){
            printf("int");
        }
        else if (cursor->data_type == 'f'){
            printf("float");
        }
        else if (cursor->data_type == 'c'){
            printf("char");
        }
        else if (cursor->data_type == 's'){
            printf("string");
        }
        else if (cursor->data_type == 'r'){
            printf("reference");
        }
        
        // print arrow if next != NULL
        if (cursor->next != NULL){
            printf(" -> ");
        }
        cursor = cursor->next;
    }
}

node * mtll_delete(node * nodes, node * head, int position){
    node * cursor = head;

    node * top = NULL;
    node * target = head;

    // get list size
    int list_size = 0;
    while(cursor != NULL){
        list_size++;
        cursor = cursor->next;
    }
    cursor = head;

    // check size value
    if (position < 0){
        position = list_size + position;
    }

    // position out of bounds
    if ((position < 0) || (position > list_size)){
        return NULL;
    }

    // locate delete position
    else{
        for (int i = 0; i <= position; i++){
            if (i == position - 1){
                top = cursor;
            }
            if (i == position){
                target = cursor;
            }
            cursor = cursor->next;
        }
    }

    // deletion
    node * return_node = head;
    if (position == 0){
        return_node = target->next;
    }
    else {
        top->next = target->next;
    }
    
    // print list
    printf("List %d: ", head->list_index);
    
    mtll_view(return_node);
    printf("\n");

    return return_node;
}

node * mtll_insert(node * nodes, list * lists, node * head, int position, int * num_nodes, char * element){
    node * cursor = head;

    node * top = NULL;
    node * bottom = head;

    // get list size
    int list_size = 0;
    while(cursor != NULL){
        list_size++;
        cursor = cursor->next;
    }
    cursor = head;

    // check size value
    int rev = 0;
    if (position < 0){
        rev = 1;
        position = list_size + position;
        if (position == 0){
            rev = 0;
            position = 1;
        }
    }

    // position out of bounds
    if ((position < 0) || (position > list_size)){
        return NULL;
    }

    // locate insert position
    else{
        for (int i = 0; i <= position; i++){
            if (rev == 0 && i == position - 1){
                top = cursor;
            }
            if (i == position){
                bottom = cursor;
            }
            cursor = cursor->next;
        }
    }

    // copy input from element
    char input[128];
    strcpy(input, element);
    // remove new line character
    input[strlen(input)] ='\0';

    // node creation
    node * new_node = &nodes[*num_nodes * sizeof(node)];
    new_node->list_index = head->list_index;

    // nested list
    if (input[0] == '{' && input[strlen(input) - 1] == '}'){
        // get list index from input
        int target_index;

        char copy[128];
        strcpy(copy, input);

        sscanf(copy, "{%d}", &target_index);

        // reference list in node
        new_node->data_type = 'r';
        new_node->reference = lists[target_index * sizeof(node)].head;
    }
    // simple list
    else{
        // check data type
        int data_type = 0; // 0:int 1:power 2:float 3:char 4:string
        int * dt_ptr = &data_type;

        // speical case: empty input
        if(strlen(input) == 0){
            *dt_ptr = 3;
        }
        // non-empty input
        else{
            for(int j = 0; j < strlen(input); j++){
                // if input[j] is not a number
                if (input[j] < '0' || input[j] > '9'){
                    // if input only has one char
                    if (strlen(input) == 1){
                        *dt_ptr = 3;
                    }
                    // if input[j] is a minus sign
                    else if (input[j] == '-' && (data_type == 0 || data_type == 1)){
                        continue;
                    }
                    // if input[j] is a power sign
                    else if (input[j] == 'e' && data_type == 0){
                        *dt_ptr = 1;
                    }
                    // if input[j] is a dot
                    else if (input[j] == '.' && data_type == 0){
                        *dt_ptr = 2;
                    }
                    // else ipnut is a string
                    else {
                        *dt_ptr = 4;
                    }
                }
            }
        }

        // insert data to node
            // int
        if (data_type == 0){
            new_node->i_value = atoi(input);
            new_node->data_type = 'i';
        }
            // power
        else if (data_type == 1){
            int base;
            int expo;
            sscanf(input, "%de%d", &base, &expo);

            new_node->i_value = my_pow(base, expo);
            new_node->data_type = 'i';
        }
            // float
        else if (data_type == 2){
            new_node->f_value = atof(input);
            new_node->data_type = 'f';
        }
            // char
        else if (data_type == 3){
            new_node->c_value = input[0];
            new_node->data_type = 'c';   
        }
            // string
        else if (data_type == 4){
            strcpy(new_node->s_value, input);
            new_node->data_type = 's'; 
        }
    }

    // insertion
    *num_nodes = *num_nodes + 1;
    node * return_node = head;
    if (position == 0){
        new_node->next = bottom;
        return_node = new_node;
    }
    else if (rev == 0){
        top->next = new_node;
        new_node->next = bottom;
    }
    else if (rev == 1){
        new_node->next = bottom->next;
        bottom->next = new_node;
    }
    
    // print list
    printf("List %d: ", head->list_index);
    
    mtll_view(return_node);
    printf("\n");

    return return_node;
}
