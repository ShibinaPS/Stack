#ifndef MY_STACK_H
#define MY_STACK_H

//=============================================================================================================
#include <stdio.h>
#include <stdlib.h>

//=============================================================================================================

#define HASH_PROTECTION
#define CANARY_PROTECTION

//=============================================================================================================

#define TYPE "%d\n"

//=============================================================================================================

typedef int elem_t;

//=============================================================================================================

/**
 * 
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

}stk;

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
 * @brief  Creates and initializes stk.
 * 
 * @param  stk the stk pointer
 * 
 * @return int error
*/

int stk_ctor(Stack* stk); 

//=============================================================================================================

/**
 * @brief  Pushes a new element to the stk.
 * 
 * @param  stk the stack pointer
 * @param  elem a new value pushing to the stack
 * 
*/

void stk_push(Stack* stk, elem_t elem);

//=============================================================================================================

/**
 * @brief Deletes the value from the stack.
 * 
 * @param stk the stack pointer
 * @param elem the value deleting from the stack
 * 
*/

void stk_pop(Stack* stk, elem_t* elem);

//=============================================================================================================

/**
 * @brief Fills the fields of the stack with NAN value after its creating.
 * 
 * @param stk the stack pointer
 * @param start the start position of filling stack with NAN value
 * @param finish the finish position of filling stack with NAN value
 * 
*/

void fill_with_poison(Stack* stk, size_t start, size_t finish);

//=============================================================================================================

/**
 * @brief Changes the size of stack taking into consideration the degree of fulness.
 * 
 * @param stk the stack pointer
 * @param new_capacity new stack capacity
 * 
*/
void stk_resize(Stack* stk, size_t new_capacity);

//=============================================================================================================

/**
 * @brief  Opens file "log_file.txt".
 * 
 * @param  stk the stack pointer
 * 
 * @return int error
*/

int open_log_file(Stack* stk);

//=============================================================================================================

/**
 * @brief  Prints new changes in file "log_file.txt".
 * 
 * @param  stk the stack pointer
 * 
 * @return int 
*/

int print_log_file(Stack* stk);

//=============================================================================================================

/**
 * @brief  Deletes the stack, frees memory, fills stack filelds with POISON value.
 * 
 * @param  stk the stack pointer
*/

void stk_dtor(Stack* stk);

//=============================================================================================================

#endif //MY_STACK_H