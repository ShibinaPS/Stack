#include "Stack.h"
#include "Debug.h"

//=============================================================================================================

void stack_dump(Stack* stack, const char* file_name, size_t line, const char* function_name)
{
    fprintf(stack->log_file, "in %s at %s line - %lu:\n", function_name, file_name, line);

    fprintf(stack->log_file, "Stack[%p]\n", stack->data);

    stack_error_decoder(stack);
}

//=============================================================================================================

int stack_verify(Stack* stack)
{
#ifdef HASH_PROTECTION

#   ifdef CANARY_PROTECTION

        stack->error_code = (stack->data == nullptr)                * ERROR_STACK_NULLPTR     +
                            (stack->size > stack->capacity)         * ERROR_STACK_CAPACITY    +
                            (stack->data_lft_cnr != DATA_CANARY)    * ERROR_DATA_LFT_CNR_DEAD +
                            (stack->data_rgt_cnr != DATA_CANARY)    * ERROR_DATA_RGT_CNR_DEAD +
                            (stack->stk_lft_cnr != STK_CANARY)      * ERROR_STK_LFT_CNR_DEAD  +
                            (stack->stk_rgt_cnr != STK_CANARY)      * ERROR_STK_RGT_CNR_DEAD  +
                            (stack->data_hash != data_hash(stack))  * ERROR_DATA_HASH;

#   else

        stack->error_code = (stack->data == nullptr)                * ERROR_STACK_NULLPTR  +
                            (stack->size > stack->capacity)         * ERROR_STACK_CAPACITY +
                            (stack->data_hash != data_hash(stack))  * ERROR_DATA_HASH;

#   endif //CANARY_PROTECTION

#else

#   ifdef CANARY_PROTECTION

        stack->error_code = (stack->data == nullptr)             * ERROR_STACK_NULLPTR      +
                            (stack->size > stack->capacity)      * ERROR_STACK_CAPACITY     +
                            (stack->data_lft_cnr != DATA_CANARY) * ERROR_DATA_LFT_CNR_DEAD  +
                            (stack->data_rgt_cnr != DATA_CANARY) * ERROR_DATA_RGT_CNR_DEAD  +
                            (stack->stk_lft_cnr != STK_CANARY)   * ERROR_STK_LFT_CNR_DEAD   +
                            (stack->stk_rgt_cnr != STK_CANARY)   * ERROR_STK_RGT_CNR_DEAD;

    #else

        stack->error_code = (stack->data == nullptr)        * ERROR_STACK_NULLPTR +
                            (stack->size > stack->capacity) * ERROR_STACK_CAPACITY;


    #endif //CANARY_PROTECTION

#endif //HASH_PROTECTION

    return stack->error_code;
}

//=============================================================================================================

void stack_error_decoder(Stack* stack)
{
    if (stack->error_code == 0)
    {
        fprintf(stack->log_file, "-> OK\n");
    }

    else
    {
        if(stack->error_code & ERROR_DATA_NULLPTR)
        {
            fprintf(stack->log_file, "ERROR_DATA_NULLPTR\n");
        }

        if(stack->error_code & ERROR_STACK_CAPACITY)
        {
            fprintf(stack->log_file, "ERROR_STACK_CAPACITY\n");
        }

    #ifdef CANARY_PROTECTION
        if(stack->error_code & ERROR_DATA_LFT_CNR_DEAD)
        {
            fprintf(stack->log_file, "ERROR_DATA_LFT_CNR_DEAD\n");
        }

        if(stack->error_code & ERROR_DATA_RGT_CNR_DEAD)
        {
            fprintf(stack->log_file, "ERROR_DATA_RGT_CNR_DEAD\n");
        }

        if(stack->error_code & ERROR_STK_LFT_CNR_DEAD)
        {
            fprintf(stack->log_file, "ERROR_STK_LFT_CNR_DEAD\n");
        }

        if(stack->error_code & ERROR_STK_RGT_CNR_DEAD)
        {
            fprintf(stack->log_file, "ERROR_STK_RGT_CNR_DEAD\n");
        }
    #endif //CANARY_PROTECTION
    
    #ifdef HASH_PROTECTION

        if(stack->error_code & ERROR_DATA_HASH)
        {
            fprintf(stack->log_file, "ERROR_DATA_HASH\n");
        }

    #endif //HASH_PROTECTION
    }
}

//=============================================================================================================

void assert_dtor(Stack* stack)
{
    fill_with_poison(stack, 0, stack->capacity);

#ifdef CANARY_PROTECTION
    stack->data--;
#endif //CANARY_PROTECTION

    stack->capacity     = 0;
    stack->size         = 0;
    stack->data_hash    = 0;
    stack->error_code   = 0;
    stack->data_lft_cnr = 0;
    stack->data_rgt_cnr = 0;

    free(stack->data);
    stack->data = nullptr;

    fclose(stack->log_file);
    stack->log_file = nullptr;

    exit(0);
}

//=============================================================================================================

long long calculate_hash(elem_t* pointer, size_t size)
{
    long long hash = 0;

    for(size_t i = 0; i < size; i++)
    {
        hash = (long long)(((hash << 5) + hash) + (long long)pointer[i]);
    }
    return hash;
}

//=============================================================================================================

long long data_hash(Stack* stack)
{
    return calculate_hash(stack->data, stack->capacity);
}

//=============================================================================================================
