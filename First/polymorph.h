#ifndef POLYMORPH
#define POLYMORPH

typedef struct 
{
    size_t size;
    void (*input)(void *out);
    void (*print)(const void *in);
} TypeInfo;

int int_input(void *out);
void int_print(const void *in);
int double_input(void *out);
void double_print(const void *in);
TypeInfo* get_int_type_info();
TypeInfo* get_double_type_info(); 

#endif