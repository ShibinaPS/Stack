#ifndef MY_STACK_H
#define MY_STACK_H

//=============================================================================================================
#include <stdio.h>
#include <stdlib.h>

//=============================================================================================================

// #define HASH_PROTECTION
#define CANARY_PROTECTION

//=============================================================================================================

typedef int elem_t;

//=============================================================================================================

/**
 * @brief Stack structure
*/
typedef struct Stack
{
    size_t stk_lft_cnr  = 0;

    FILE* log_file  = nullptr;
    size_t capacity = 0;
    size_t size     = 0;

    size_t data_lft_cnr = 0;
    elem_t* data        = nullptr;
    size_t data_rgt_cnr = 0;

    int error_code      = 0;
    long long data_hash = 0;

    size_t stk_rgt_cnr  = 0;

} stack;

//=============================================================================================================

/**
 * @brief Notes the codes of errors.
 * 
*/

enum ErrorCodes
{
    ERROR_DATA_REALLOC  = 1,
    ERROR_STACK_NULLPTR = 2,
    ERROR_LOG_FILE_OPEN = 3,
    ERROR_DATA_CALLOC   = 4,
};

//=============================================================================================================

const size_t RESIZE_FACTOR     = 2;
const size_t RESIZE_POP_FACTOR = 4;
const size_t POISON            = 0xDEAD;
const size_t DATA_CANARY       = 0xDEDDEAD;
const size_t STK_CANARY        = 0xDEADDED;

//=============================================================================================================

/**
 * @brief  Creates and initializes stack.
 * 
 * @param  stack the stack pointer
 * 
 * @return int error
*/

int stack_ctor(Stack* stack); 

//=============================================================================================================

/**
 * @brief  Pushes a new element to the stack.
 * 
 * @param  stack the stack pointer
 * @param  elem a new value pushing to the stack
 * 
*/

void stack_push(Stack* stack, elem_t elem);

//=============================================================================================================

/**
 * @brief Deletes the value from the stack.
 * 
 * @param stack the stack pointer
 * @param elem the value deleting from the stack
 * 
*/

void stack_pop(Stack* stack, elem_t* elem);

//=============================================================================================================

/**
 * @brief Fills the fields of the stack with NAN value after its creating.
 * 
 * @param stack the stack pointer
 * @param start the start position of filling stack with NAN value
 * @param finish the finish position of filling stack with NAN value
 * 
*/

void fill_with_poison(Stack* stack, size_t start, size_t finish);

//=============================================================================================================

/**
 * @brief Changes the size of stack taking into consideration the degree of fulness.
 * 
 * @param stack the stack pointer
 * @param new_capacity new stack capacity
 * 
*/
void stack_resize(Stack* stack, size_t new_capacity);

//=============================================================================================================

/**
 * @brief  Opens file "log_file.txt".
 * 
 * @param  stack the stack pointer
 * 
 * @return int error
*/

int open_log_file(Stack* stack);

//=============================================================================================================

/**
 * @brief  Prints new changes in file "log_file.txt".
 * 
 * @param  stack the stack pointer
 * 
 * @return int 
*/

int print_log_file(Stack* stack);

//=============================================================================================================

/**
 * @brief  Deletes the stack, frees memory, fills stack filelds with POISON value.
 * 
 * @param  stack the stack pointer
*/

void stack_dtor(Stack* stack);

//=============================================================================================================

#endif //MY_STACK_H