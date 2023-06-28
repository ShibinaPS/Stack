#include "Stack.h"
#include "Debug.h"

//=============================================================================================================

const size_t INITIAL_STK_CAP = 15;

//=============================================================================================================

int stack_ctor(Stack* stack)
{
    if (stack == nullptr)
    {
        printf("Error code: %d\n", ERROR_STACK_NULLPTR);
        return ERROR_STACK_NULLPTR;
    }

    stack->capacity = INITIAL_STK_CAP;

    if (open_log_file(stack) != 0)
    {
        printf("Error code: %d\n", ERROR_LOG_FILE_OPEN);
        return ERROR_LOG_FILE_OPEN;
    }

#ifdef CANARY_PROTECTION
    stack->stk_lft_cnr = STK_CANARY;
    stack->stk_rgt_cnr = STK_CANARY;

    stack->data = (elem_t*)calloc(stack->capacity + 2, sizeof(elem_t));

    if(stack->data == nullptr)
    {
        fclose(stack->log_file);
        printf("ERROR CODE: %d\n", ERROR_DATA_CALLOC);
        return ERROR_DATA_CALLOC;
    }

    *stack->data = (elem_t)DATA_CANARY;

    stack->data_lft_cnr = (size_t)*stack->data;
    stack->data_rgt_cnr = (size_t)*(stack->data + stack->capacity + 1); 

    stack->data_lft_cnr = DATA_CANARY;
    stack->data_rgt_cnr = DATA_CANARY;

    stack->data++;
#else
    stack->data = (elem_t*)calloc(stack->capacity, sizeof(elem_t));

    if(stack->data == nullptr)
    {
        fclose(stack->log_file);
        printf("ERROR CODE: %d\n", ERROR_DATA_CALLOC);
        return ERROR_DATA_CALLOC;
    }
#endif //CANARY_PROTECTION

#ifdef HASH_PROTECTION

    stack->data_hash = data_hash(stack);

#endif //HASH_PROTECTION

    fill_with_poison(stack, 0, stack->capacity);
    fprintf(stack->log_file, "---------------------- Stack ----------------------\n");
    print_log_file(stack);

    return 0;
}

//=============================================================================================================

void stack_push(Stack* stack, elem_t elem)
{
    // ASSERT_OK(stack);

    if(stack->size == stack->capacity - 1)
    {
        stack_resize(stack, stack->capacity * RESIZE_FACTOR);
    }

    stack->data[stack->size] = elem;
    stack->size++;

#ifdef HASH_PROTECTION
    stack->data_hash = data_hash(stack);
#endif //HASH_PROTECTION

    print_log_file(stack);
}

//=============================================================================================================

void stack_pop(Stack* stack, elem_t* elem)
{
    ASSERT_OK(stack);

    stack->size--;

    *elem = stack->data[stack->size];
    stack->data[stack->size] = POISON;

#ifdef HASH_PROTECTION
    stack->data_hash = data_hash(stack);
#endif //HASH_PROTECTION

    if(stack->size * RESIZE_POP_FACTOR <= stack->capacity)  //при размере стека <= 1/4 от capacity,
    {                                                      
        stack_resize(stack, stack->capacity/RESIZE_FACTOR); //последний уменьшается в 2 раза
    }

    print_log_file(stack);

    if (stack->size == 0)
    {
        fprintf(stack->log_file, "Stack is empty...\n");
    }

}

//=============================================================================================================

void stack_resize(Stack* stack, size_t new_capacity)
{
    ASSERT_OK(stack);

    size_t old_capacity = stack->capacity;
    stack->capacity     = new_capacity;

#ifdef CANARY_PROTECTION

    stack->data--;

    stack->data = (elem_t*)realloc(stack->data, (stack->capacity + 2) * sizeof(elem_t));

    if(stack->data == nullptr)
    {
        printf("Error code: %d.\n", ERROR_DATA_REALLOC);
        exit(ERROR_DATA_REALLOC);
    }

    *stack->data = (elem_t)DATA_CANARY;

    stack->data_lft_cnr  = (size_t)*stack->data;
    stack->data_rgt_cnr  = (size_t)*(stack->data + stack->capacity + 1);
    
    stack->data_lft_cnr = DATA_CANARY;
    stack->data_rgt_cnr = DATA_CANARY;

    stack->data++;

#else

    stack->data = (elem_t*)realloc(stack->data, stack->capacity * sizeof(elem_t));

    if(stack->data == nullptr)
    {
        printf("Error code: %d.\n", ERROR_DATA_REALLOC);
        exit(ERROR_DATA_REALLOC);
    }

#endif //CANARY_PROTECTION

#ifdef HASH_PROTECTION

    stack->data_hash = data_hash(stack);

#endif //HASH_PROTECTION

    fill_with_poison(stack, old_capacity, stack->capacity);

#ifdef HASH_PROTECTION

    stack->data_hash = data_hash(stack);

#endif //HASH_PROTECTION
}

//=============================================================================================================

void fill_with_poison(Stack* stack, size_t start, size_t finish)
{
    ASSERT_OK(stack);
    
    for (size_t i = start; i < finish; i++)
    {
        stack->data[i] = POISON;
    }
}

//=============================================================================================================

int open_log_file(Stack* stack)
{
    stack->log_file = fopen("log_file.txt", "w");

    if (stack->log_file == nullptr)
    {
        return ERROR_LOG_FILE_OPEN;
    }

    return 0;
}

//=============================================================================================================

int print_log_file(Stack* stack)
{
    fprintf(stack->log_file, "{\n");

    #ifdef CANARY_PROTECTION
        fprintf(stack->log_file, "[STK_LFT_CANARY] = %lu\n", stack->stk_lft_cnr);
    #endif //CANARY_PROTECTION

        fprintf(stack->log_file, "Capacity  = %lu;\n", stack->capacity);

        fprintf(stack->log_file, "Size      = %lu;\n", stack->size);

    #ifdef HASH_PROTECTION
        fprintf(stack->log_file, "data_hash = [%lld];\n", stack->data_hash);
    #endif //HASH_PROTECTION

        fprintf(stack->log_file, "Data      = %p;\n", stack->data);

        fprintf(stack->log_file, "\t{\n");

    #ifdef CANARY_PROTECTION
        fprintf(stack->log_file, "\t[DATA_LFT_CANARY] = %lu\n", stack->data_lft_cnr);
    #endif //CANARY_PROTECTION

        for(size_t i = 0; i < stack->capacity; i++)
        {
            if(stack->data[i] == POISON)
            {
                fprintf(stack->log_file, "\tdata[%04lu] = %d             <- POISON\n", i, stack->data[i]);
            }

            else
            {
                fprintf(stack->log_file, "\tdata[%04lu] = %d\n", i, stack->data[i]);
            }
        }

    #ifdef CANARY_PROTECTION
        fprintf(stack->log_file, "\t[DATA_RGT_CANARY] = %lu\n", stack->data_rgt_cnr);
    #endif //CANARY_PROTECTION

    fprintf(stack->log_file, "\t}\n");

    #ifdef CANARY_PROTECTION
        fprintf(stack->log_file, "[STK_RGT_CANARY] = %lu\n", stack->stk_rgt_cnr);
    #endif //CANARY_PROTECTION

    fprintf(stack->log_file, "}\n\n");

    fprintf(stack->log_file, "----------------------------------------------------\n");

    fprintf(stack->log_file, "\n");

}

//=============================================================================================================

void stack_dtor(Stack* stack)
{
    ASSERT_OK(stack);

    fill_with_poison(stack, 0, stack->capacity);

#ifdef CANARY_PROTECTION
    stack->data--;
#endif //CANARY_PROTECTION

    stack->capacity     = 0;
    stack->size         = 0;
    stack->error_code   = 0;
    stack->data_hash    = 0;
    stack->data_lft_cnr = 0;
    stack->data_rgt_cnr = 0;

    free(stack->data);
    stack->data = nullptr;

    fclose(stack->log_file);
    stack->log_file = nullptr;
}
