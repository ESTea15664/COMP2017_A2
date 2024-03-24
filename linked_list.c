#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node{
    char data_type;
    void * data;
    struct node * next;
};

struct linked_list{
    struct node * head;
    int index;
};

// declarations
struct node * new_list(int size, int index, struct node * ns, int * is, float * fs, char * cs, char * ss, int * num_ns, int * num_is, int * num_fs, int * num_cs, int * num_ss);
void view_list(struct node * head);
void type_list(struct node * head);
void remove_list(struct node * head);

int main(void){
    // recieving command from stdin
    char buffer[100];

    // a variable to keep track of number of lists
    int index_of_lists = 0;
    int num_of_lists = 0;

    // stroing all lists in here
    struct linked_list * lists = (struct linked_list *)malloc(100 * sizeof(struct node));

    // allocate memory to store nodes
    struct node * ns = (struct node *)malloc(100 * sizeof(struct node));
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

    char * ss = malloc(100 * 100);
    int num_s = 0;
    int * num_ss = &num_s;
    
    while(fgets(buffer, sizeof(buffer), stdin) != NULL){
        char prem_cmd[100];
        char deux_cmd[100];
        char * cmds[] = {prem_cmd, deux_cmd};

        if (strlen(buffer) != 1){
            // format command through sscanf
            sscanf(buffer, "%s %s", cmds[0], cmds[1]);

            // convert second command into integer
            int numeric = 1;
            // using ascii table as reference
            for (size_t i = 0; i < strlen(deux_cmd); i++){
                if (deux_cmd[i] < 48 || deux_cmd[i] > 57){
                    numeric = 0;
                    break;
                }
            }
            if (numeric == 1) *cmds[1] = atoi(cmds[1]);

            // checking different first command inputs
                // NEW
            if (strcmp(cmds[0], "NEW") == 0){
               lists[index_of_lists].head = new_list(*deux_cmd, index_of_lists, ns, is, fs, cs, ss, num_ns, num_is, num_fs, num_cs, num_ss);
                if (lists[index_of_lists].head->data_type != 'n'){
                    lists[index_of_lists].index = index_of_lists;

                    num_of_lists++;
                    index_of_lists++;
                }
            }

                // VIEW
            else if (strcmp(cmds[0], "VIEW") == 0){
                if (numeric){
                    int access = *deux_cmd;
                    if (lists[access - 1].head != NULL){
                        view_list(lists[access - 1].head);
                    }
                    else{
                        printf("VIEWING UNDEFINED LIST\n");
                    }
                }
                else if (strcmp(cmds[1], "ALL") == 0){
                    printf("Number of all list: %d\n", num_of_lists);
                    for(int i = 0; i < index_of_lists; i++){
                        if (lists[i].head != NULL){
                            printf("List %d\n", lists[i].index + 1);
                        }
                    }
                }
                else{
                    printf("INVALID COMMAND: INPUT\n");
                    continue;
                }
            }

                // TYPE
            else if (strcmp(cmds[0], "TYPE") == 0){
                int access = *deux_cmd;
                type_list(lists[(access - 1)  * sizeof(struct node)].head);
            }

                // REMOVE (set head as "NULL")
            else if (strcmp(cmds[0], "REMOVE") == 0){
                int access = *deux_cmd;
                lists[access - 1].head = NULL;
                num_of_lists--;
            }

            else{
                printf("INVALID COMMAND: INPUT\n");
                continue;
            }
            continue;
        }
        break;
    }

    free(is);
    free(fs);
    free(cs);
    free(ss);
    free(ns);
    free(lists);

    is = NULL;
    fs = NULL;
    cs = NULL;
    ss = NULL;
    ns = NULL;
    lists = NULL;

    return 0;
}


// "NEW" function
struct node * new_list(int size, int index, struct node * ns, int * is, float * fs, char * cs, char * ss, int * num_ns, int * num_is, int * num_fs, int * num_cs, int * num_ss){
    struct node * head;
    struct node * nodes = &ns[(*num_ns + 1) * sizeof(struct node)];

    // check size value
    if (size == 0 || size == 45){
        nodes[0].data_type = 'e'; 
        nodes[0].data = NULL;
        nodes[0].next = NULL;

        if (size == 45){
            printf("INVALID COMMAND: NEGATIVE SIZE NOT ALLOWED\n");
            nodes[0].data_type = 'n';
        }
        head = &nodes[0];
        *num_ns = *num_ns + 1;
        return head;
    }

    char buffer[100];
    char input;
    char * i_ptr = &input;

    for (int i = 0; i < size; i++){
        if (fgets(buffer, sizeof(buffer), stdin) != NULL){

            sscanf(buffer, "%s", i_ptr);

            nodes[i].data_type = 'c';
            nodes[i].data = NULL;

            // special case: empty character
            if (strlen(i_ptr) == 0){
                nodes[i].data_type = 'c';
                char s = '\0';
                ss[*num_ss*100] = s;
                *num_ss = *num_ss + 1;
            }

            // checking input type
            int is_integer = 1;
            for (size_t j = 0; j < strlen(i_ptr); j++){
                // not integer
                if (i_ptr[j] < 48 || i_ptr[j] > 57){
                    is_integer = 0;
                    // float
                    if (i_ptr[j] == '.') {
                        nodes[i].data_type = 'f';

                        float f;
                        sscanf(i_ptr, "%f", &f);
                        fs[*num_fs] = f;
                        nodes[i].data = &fs[*num_fs];
                        *num_fs = *num_fs + 1;
                    }
                    // string
                    else if (strlen(i_ptr) > 1) {
                        nodes[i].data_type = 's';

                        for (int k = 0; k <= strlen(i_ptr); k++){
                            if (k == strlen(i_ptr)){
                                ss[*num_ss*100 + k] = '\0';
                                break;
                            }
                            ss[*num_ss*100 + k] = i_ptr[k];
                        }
                        nodes[i].data = &ss[*num_ss*100];
                        *num_ss = *num_ss + 1;
                    }
                    // char
                    else {
                        nodes[i].data_type = 'c';

                        char c;
                        sscanf(i_ptr, "%c", &c);
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
                sscanf(i_ptr, "%i", &integer);
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
    printf("list %d: ", index+1);
    view_list(&nodes[0]);
    
    return &nodes[0];
}

// "VIEW" function
void view_list(struct node * head){
    struct node cursor = *head;

    while (1){
        if (cursor.next != NULL){
            if (cursor.data_type == 105){
                int * i = cursor.data;
                printf("%d", *i);
            }
            else if (cursor.data_type == 102){
                float * f = cursor.data;
                printf("%.02f", *f);
            }
            else if (cursor.data_type == 99){
                char * c = cursor.data;
                printf("%c", *c);
            }
            else if (cursor.data_type == 115){
                char * s = cursor.data;
                printf("%s", s);
            }
            printf(" -> ");
            cursor = *cursor.next;
        }
        else{
            if (cursor.data_type == 105){
                int * i = cursor.data;
                printf("%d", *i);
            }
            else if (cursor.data_type == 102){
                float * f = cursor.data;
                printf("%.02f", *f);
            }
            else if (cursor.data_type == 99){
                char * c = cursor.data;
                printf("%c", *c);
            }
            else if (cursor.data_type == 115){
                char * s = cursor.data;
                printf("%s", s);
            }
            printf("\n");
            break;
        }
    }
}

// "TYPE" function
void type_list(struct node * head){
    struct node cursor = *head;

    while (1){
        if (cursor.next != NULL){
            if (cursor.data_type == 105){
                printf("integer-> ");
            }
            else if (cursor.data_type == 102){
                printf("float -> ");
            }
            else if (cursor.data_type == 99){
                printf("char -> ");
            }
            else if (cursor.data_type == 115){
                printf("string -> ");
            }
            cursor = *cursor.next;
        }
        else{
            if (cursor.data_type == 105){
                printf("string\n");
            }
            else if (cursor.data_type == 102){
                printf("float\n");
            }
            else if (cursor.data_type == 99){
                printf("char\n");
            }
            else if (cursor.data_type == 115){
                printf("string\n");
            }
            break;
        }
    }
}
