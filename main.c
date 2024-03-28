#include "mtll.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(void) {
    // recieving command from stdin
    char buffer[100];

    // a variable to keep track of number of lists
    int index_of_lists = 0;
    int num_of_lists = 0;

    // stroing all heads in here
    struct list * lists = (struct list *)malloc(100 * sizeof(node));

    // allocate memory to store nodes
    node * ns = (node *)malloc(100 * sizeof(node));
    int num_n = 0;
    int * num_ns = &num_n;

    // allocate memory to store data
    int * is = (int *)malloc(100 * sizeof(int));
    int num_i = 0;
    int * num_is = &num_i;

    float * fs = (float *)malloc(100 * sizeof(float));
    int num_f = 0;
    int * num_fs = &num_f;

    char * cs = (char *)malloc(100 * sizeof(char));
    int num_c = 0;
    int * num_cs = &num_c;

    char * ss = malloc(100 * 128);
    int num_s = 0;
    int * num_ss = &num_s;
    
    while(fgets(buffer, sizeof(buffer), stdin) != NULL){
        char prem_cmd[100];
        char deux_cmd[100];
        char * cmds[] = {prem_cmd, deux_cmd};

        if (strlen(buffer) != 1){
            // format command through sscanf
            sscanf(buffer, "%s %s", cmds[0], cmds[1]);

            // convert second command into integer if aplicable
            int numeric = 1;
            // checking non_numeric char in deux_cmd
            for (size_t i = 0; i < strlen(deux_cmd); i++){
                if (deux_cmd[i] < 48 || deux_cmd[i] > 57){
                    numeric = 0;
                    break;
                }
            }
            if (numeric == 1) {
                *cmds[1] = atoi(cmds[1]);
            }

            // checking different first command inputs
                // NEW
            if (strcmp(cmds[0], "NEW") == 0){
                if (numeric == 1){
                    lists[index_of_lists].head = mtll_create(*deux_cmd, index_of_lists, ns, is, fs, cs, ss, lists, num_ns, num_is, num_fs, num_cs, num_ss);
                    if (lists[index_of_lists].head->data_type != 'n'){
                        num_of_lists++;
                        index_of_lists++;
                    }
                }
                else {
                    printf("INVALID COMMAND: NEW\n");
                    continue;
                }
            }

                // VIEW
            else if (strcmp(cmds[0], "VIEW") == 0){
                if (numeric){
                    if (lists[*deux_cmd - 0].head != NULL){
                        mtll_view(lists[*deux_cmd - 0].head, 0);
                    }
                    else{
                        printf("INVALID COMMAND: VIEW\n");
                        continue;
                    }
                }
                else if (strcmp(cmds[1], "ALL") == 0){
                    printf("Number of all list: %d\n", num_of_lists);
                    for(int i = 0; i < index_of_lists; i++){
                        if (lists[i].head != NULL){
                            printf("List %d\n", lists[i].head->index);
                        }
                    }
                }
                else{
                    printf("INVALID COMMAND: INPUT\n");
                    continue;
                }
            }

                // VIEW_NESTED
            else if (strcmp(cmds[0], "VIEW_NESTED") == 0){
                if (numeric){
                    if (lists[*deux_cmd - 0].head != NULL){
                        mtll_view_nested(lists[*deux_cmd - 0].head);
                    }
                    else{
                        printf("INVALID COMMAND: VIEW\n");
                        continue;
                    }
                }
                else{
                    printf("INVALID COMMAND: INPUT\n");
                    continue;
                }
            }

                // TYPE
            else if (strcmp(cmds[0], "TYPE") == 0){
                mtll_type(lists[*deux_cmd - 0].head);
            }

                // REMOVE (set head as "NULL")
            else if (strcmp(cmds[0], "REMOVE") == 0){
                lists[*deux_cmd - 0].head = NULL;
                num_of_lists--;
            }

            // part 2 commands
                // INSERT
            else if (strcmp(cmds[0], "INSERT") == 0){
                int target = *(int *)cmds[1];
                int position;
                char element[100];

                int * t_ptr = &target;
                int * p_ptr = &position;
                char * e_ptr = &element[0];

                sscanf(buffer, "%s %d %d %s", cmds[0], t_ptr, p_ptr, e_ptr);

                node * insertion = mtll_insert(lists[target].head, position, element, ns, is, fs, cs, ss, lists, num_ns, num_is, num_fs, num_cs, num_ss);

                if (insertion != NULL){
                    lists[*deux_cmd - 0].head = insertion;
                }
            }

                // DELETE
            else if (strcmp(cmds[0], "DELETE") == 0){
                int target = *(int *)cmds[1];
                int position;

                int * t_ptr = &target;
                int * p_ptr = &position;

                sscanf(buffer, "%s %d %d", cmds[0], t_ptr, p_ptr);

                node * deletion = mtll_delete(lists[target - 1].head, position, lists[target - 1].head->index, ns, num_ns);

                if (deletion != NULL){
                    (lists[*deux_cmd - 0]).head = deletion;
                }
            }

            // part 3 commands
                // NEST

            else{
                printf("INVALID COMMAND: INPUT\n");
                continue;
            }


            continue;
        }
        break;
    }

    mtll_free(is, fs, cs, ss, ns, lists);

    return 0;
}
