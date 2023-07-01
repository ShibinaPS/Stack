#include "Stack.h"
#include "Debug.h"

//=============================================================================================================

void stk_dump(Stack* stk, const char* file_name, size_t line, const char* function_name)
{
    fprintf(stk->log_file, "in %s at %s line - %lu:\n", function_name, file_name, line);

    fprintf(stk->log_file, "Stack[%p]\n", stk->data);

    stk_error_decoder(stk);
}

//=============================================================================================================

int stk_verify(Stack* stk)
{
#ifdef HASH_PROTECTION

#   ifdef CANARY_PROTECTION

        stk->error_code =   (stk->data == nullptr)                * ERROR_STACK_NULLPTR     +
                            (stk->size > stk->capacity)           * ERROR_STACK_CAPACITY    +
                            (stk->data_lft_cnr != DATA_CANARY)    * ERROR_DATA_LFT_CNR_DEAD +
                            (stk->data_rgt_cnr != DATA_CANARY)    * ERROR_DATA_RGT_CNR_DEAD +
                            (stk->stk_lft_cnr != STK_CANARY)      * ERROR_STK_LFT_CNR_DEAD  +
                            (stk->stk_rgt_cnr != STK_CANARY)      * ERROR_STK_RGT_CNR_DEAD  +
                            (stk->data_hash != data_hash(stk))    * ERROR_DATA_HASH;

#   else

        stk->error_code = (stk->data == nullptr)                * ERROR_STACK_NULLPTR  +
                            (stk->size > stk->capacity)         * ERROR_STACK_CAPACITY +
                            (stk->data_hash != data_hash(stk))  * ERROR_DATA_HASH;

#   endif //CANARY_PROTECTION

#else

#   ifdef CANARY_PROTECTION

        stk->error_code = (stk->data == nullptr)             * ERROR_STACK_NULLPTR      +
                            (stk->size > stk->capacity)      * ERROR_STACK_CAPACITY     +
                            (stk->data_lft_cnr != DATA_CANARY) * ERROR_DATA_LFT_CNR_DEAD  +
                            (stk->data_rgt_cnr != DATA_CANARY) * ERROR_DATA_RGT_CNR_DEAD  +
                            (stk->stk_lft_cnr != STK_CANARY)   * ERROR_STK_LFT_CNR_DEAD   +
                            (stk->stk_rgt_cnr != STK_CANARY)   * ERROR_STK_RGT_CNR_DEAD;

    #else

        stk->error_code = (stk->data == nullptr)        * ERROR_STACK_NULLPTR +
                          (stk->size > stk->capacity)   * ERROR_STACK_CAPACITY;


    #endif //CANARY_PROTECTION

#endif //HASH_PROTECTION

    return stk->error_code;
}

//=============================================================================================================

void stk_error_decoder(Stack* stk)
{
    if (stk->error_code == 0)
    {
        fprintf(stk->log_file, "-> OK\n");
    }

    else
    {
        if(stk->error_code & ERROR_DATA_NULLPTR)
        {
            fprintf(stk->log_file, "ERROR_DATA_NULLPTR\n");
        }

        if(stk->error_code & ERROR_STACK_CAPACITY)
        {
            fprintf(stk->log_file, "ERROR_STACK_CAPACITY\n");
        }

    #ifdef CANARY_PROTECTION
        if(stk->error_code & ERROR_DATA_LFT_CNR_DEAD)
        {
            fprintf(stk->log_file, "ERROR_DATA_LFT_CNR_DEAD\n");
        }

        if(stk->error_code & ERROR_DATA_RGT_CNR_DEAD)
        {
            fprintf(stk->log_file, "ERROR_DATA_RGT_CNR_DEAD\n");
        }

        if(stk->error_code & ERROR_STK_LFT_CNR_DEAD)
        {
            fprintf(stk->log_file, "ERROR_STK_LFT_CNR_DEAD\n");
        }

        if(stk->error_code & ERROR_STK_RGT_CNR_DEAD)
        {
            fprintf(stk->log_file, "ERROR_STK_RGT_CNR_DEAD\n");
        }
    #endif //CANARY_PROTECTION
    
    #ifdef HASH_PROTECTION

        if(stk->error_code & ERROR_DATA_HASH)
        {
            fprintf(stk->log_file, "ERROR_DATA_HASH\n");
        }

    #endif //HASH_PROTECTION
    }
}

//=============================================================================================================

void assert_dtor(Stack* stk)
{
    fill_with_poison(stk, 0, stk->capacity);

#ifdef CANARY_PROTECTION
    stk->data--;
#endif //CANARY_PROTECTION

    stk->capacity     = 0;
    stk->size         = 0;
    stk->data_hash    = 0;
    stk->error_code   = 0;
    stk->data_lft_cnr = 0;
    stk->data_rgt_cnr = 0;

    free(stk->data);
    stk->data = nullptr;

    fclose(stk->log_file);
    stk->log_file = nullptr;

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

long long data_hash(Stack* stk)
{
    return calculate_hash(stk->data, stk->capacity);
}

//=============================================================================================================
