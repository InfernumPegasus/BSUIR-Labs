#ifndef PARENT_FUNCTIONS_H_
#define PARENT_FUNCTIONS_H_

void print_envp(char* envp[]);
char** make_child_env(char* file_envp);

char* get_child_path(char** str_arr);

#endif //PARENT_FUNCTIONS_H_
