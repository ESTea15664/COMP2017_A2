#include "mtll.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(void) {
    // allocate to store nodes
    node * nodes = (node*)calloc(128 * 128, sizeof(node));
    int * num_nodes = (int*)malloc(sizeof(int));
    *num_nodes = 0;

    // allocate to store lists
    list * lists = (list*)calloc(128, sizeof(node));

    int * num_list = (int*)malloc(sizeof(int));
    *num_list = 0;

    int * num_active_lists = (int*)malloc(sizeof(int));
    *num_active_lists = 0;

    if (nodes == NULL || num_nodes == NULL || lists == NULL || num_list == NULL || num_active_lists == NULL){
        printf("MEMORY ALLOCATION FAILED\n");
        return -1;
    }

    char buffer[128];

    // terminates if empty input
    while (fgets(buffer, sizeof(buffer), stdin) != NULL && strlen(buffer) != 1){
        // copy buffer to copy
        char copy[128];
        strcpy(copy, buffer);

        char cmd0[128];
        char cmd1[128];
        char * cmds[] = {cmd0, cmd1};

        sscanf(copy, "%s %s", cmds[0], cmds[1]);

        // check if second command is a number
        int is_num = 1;
        int * in_ptr = &is_num;
        for(int j = 0; j < strlen(cmd1); j++){
            // if input[j] is not a number
            if (cmd1[j] < '0' || cmd1[j] > '9'){
                *in_ptr = 0;
                break;
            }
        }

        // NEW
        if (strcmp(cmd0, "NEW") == 0){
            if (is_num == 1){
                // function returns a head
                node * new_head = mtll_create(atoi(cmd1), nodes, lists, num_nodes, num_list);

                // store head
                if (new_head != NULL){
                    lists[*num_list * sizeof(node)].head = new_head;
                    *num_list = *num_list + 1;
                    *num_active_lists = *num_active_lists + 1;
                }
                else{
                    printf("INVALID COMMAND: NEW\n");
                }
            }
            else {
                printf("INVALID COMMAND: NEW \n");
            }
        }
        // VIEW & VIEW ALL
        else if (strcmp(cmd0, "VIEW") == 0){
            if (is_num == 1){
                mtll_view(lists[atoi(cmd1) * sizeof(node)].head);
                printf("\n");
            }
            else if (strcmp(cmd1, "ALL") == 0){
                printf("Number of lists: %d\n", *num_active_lists);
                for(int i = 0; i < *num_list; i++){
                    mtll_view_list(lists[i * sizeof(node)].head);
                }
            }
            else {
                printf("INVALID COMMAND: VIEW \n");
            }
        }
        // VIEW-NESTED
        else if (strcmp(cmd0, "VIEW-NESTED") == 0){
            if (is_num == 1){
                mtll_view_nested(lists[atoi(cmd1) * sizeof(node)].head);
                printf("\n");
            }
            else {
                printf("INVALID COMMAND: VIEW-NESTED \n");
            }
        }
        // TYPE
        else if (strcmp(cmd0, "TYPE") == 0){
            if (is_num == 1){
                mtll_type(lists[atoi(cmd1) * sizeof(node)].head);
                printf("\n");
            }
            else {
                printf("INVALID COMMAND: VIEW-NESTED \n");
            }
        }
        // REMOVE
        else if (strcmp(cmd0, "REMOVE") == 0){
            if (lists[atoi(cmd1) * sizeof(node)].head != NULL){
                lists[atoi(cmd1) * sizeof(node)].head = NULL;
                *num_active_lists = *num_active_lists - 1;
            }

            printf("List %d has been removed.\n\n", atoi(cmd1));

            printf("Number of lists: %d\n", *num_active_lists);
            for(int i = 0; i < *num_list; i++){
                mtll_view_list(lists[i * sizeof(node)].head);
            }
        }
        // INSERT
        else if (strcmp(cmd0, "INSERT") == 0){
            strcpy(copy, buffer);

            int position;
            char insert_element[128];
            char * ie_ptr = &insert_element[0];

            sscanf(copy, "%s %s %d %s", cmds[0], cmds[1], &position, ie_ptr);

            node * new_head = mtll_insert(nodes, lists, lists[atoi(cmd1) * sizeof(node)].head, position, num_nodes, insert_element);
            if (new_head != NULL){
                lists[atoi(cmd1) * sizeof(node)].head = new_head;
            }
            else{
                printf("INVALID COMMAND: INSERT\n");
            }
        }
        // DELETE
        else if (strcmp(cmd0, "DELETE") == 0){
            strcpy(copy, buffer);

            int position;

            sscanf(copy, "%s %s %d", cmds[0], cmds[1], &position);

            node * new_head = mtll_delete(nodes, lists[atoi(cmd1) * sizeof(node)].head, position);
            if (new_head != NULL){
                lists[atoi(cmd1) * sizeof(node)].head = new_head;
            }
            else{
                printf("INVALID COMMAND: INSERT\n");
            }
        }
    }

    free(nodes);
    free(num_list);
    free(num_active_lists);
    free(num_nodes);
    free(lists);

    return 0;
}
