#include "Stack.h"
#include "Debug.h"

//=============================================================================================================

const size_t INITIAL_STK_CAP = 10;

//=============================================================================================================

int stk_ctor(Stack* stk)
{
    if (stk == nullptr)
    {
        printf("Error code: %d\n", ERROR_STACK_NULLPTR);
        return ERROR_STACK_NULLPTR;
    }

    stk->capacity = INITIAL_STK_CAP;

    if (open_log_file(stk) != 0)
    {
        printf("Error code: %d\n", ERROR_LOG_FILE_OPEN);
        return ERROR_LOG_FILE_OPEN;
    }

#ifdef CANARY_PROTECTION
    stk->stk_lft_cnr = STK_CANARY;
    stk->stk_rgt_cnr = STK_CANARY;

    stk->data = (elem_t*)calloc(stk->capacity + 2, sizeof(elem_t));

    if(stk->data == nullptr)
    {
        fclose(stk->log_file);
        printf("ERROR CODE: %d\n", ERROR_DATA_CALLOC);
        return ERROR_DATA_CALLOC;
    }

    *stk->data = (elem_t)DATA_CANARY;

    stk->data_lft_cnr = (size_t)*stk->data;
    stk->data_rgt_cnr = (size_t)*(stk->data + stk->capacity + 1); 

    stk->data_lft_cnr = DATA_CANARY;
    stk->data_rgt_cnr = DATA_CANARY;

    stk->data++;
#else
    stk->data = (elem_t*)calloc(stk->capacity, sizeof(elem_t));

    if(stk->data == nullptr)
    {
        fclose(stk->log_file);
        printf("ERROR CODE: %d\n", ERROR_DATA_CALLOC);
        return ERROR_DATA_CALLOC;
    }
#endif //CANARY_PROTECTION

    fill_with_poison(stk, 0, stk->capacity);

#ifdef HASH_PROTECTION

    stk->data_hash = data_hash(stk);   

#endif //HASH_PROTECTION

    // printf("%lld, %lld\n", stk->data_hash, data_hash(stk));

    fprintf(stk->log_file, "---------------------- Stack ----------------------\n");
    print_log_file(stk);

    return 0;
}

//=============================================================================================================

void stk_push(Stack* stk, elem_t elem)
{
    ASSERT_OK(stk);

    if(stk->size == stk->capacity - 1)
    {
        stk_resize(stk, stk->capacity * RESIZE_FACTOR);
    }

    stk->data[stk->size] = elem;
    stk->size++;

#ifdef HASH_PROTECTION
    stk->data_hash = data_hash(stk);
#endif //HASH_PROTECTION

    print_log_file(stk);
}

//=============================================================================================================

void stk_pop(Stack* stk, elem_t* elem)
{
    ASSERT_OK(stk);

    stk->size--;

    *elem = stk->data[stk->size];
    stk->data[stk->size] = POISON;

#ifdef HASH_PROTECTION
    stk->data_hash = data_hash(stk);
#endif //HASH_PROTECTION

    if(stk->size * RESIZE_POP_FACTOR <= stk->capacity)  //при размере стека <= 1/4 от capacity,
    {                                                      
        stk_resize(stk, stk->capacity/RESIZE_FACTOR); //последний уменьшается в 2 раза
    }

    print_log_file(stk);

    if (stk->size == 0)
    {
        fprintf(stk->log_file, "Stack is empty...\n");
    }

}

//=============================================================================================================

void stk_resize(Stack* stk, size_t new_capacity)
{
    ASSERT_OK(stk);

    size_t old_capacity = stk->capacity;
    stk->capacity     = new_capacity;

#ifdef CANARY_PROTECTION

    stk->data--;

    stk->data = (elem_t*)realloc(stk->data, (stk->capacity + 2) * sizeof(elem_t));

    if(stk->data == nullptr)
    {
        printf("Error code: %d.\n", ERROR_DATA_REALLOC);
        exit(ERROR_DATA_REALLOC);
    }

    *stk->data = (elem_t)DATA_CANARY;

    stk->data_lft_cnr  = (size_t)*stk->data;
    stk->data_rgt_cnr  = (size_t)*(stk->data + stk->capacity + 1);
    
    stk->data_lft_cnr = DATA_CANARY;
    stk->data_rgt_cnr = DATA_CANARY;

    stk->data++;

#else

    stk->data = (elem_t*)realloc(stk->data, stk->capacity * sizeof(elem_t));

    if(stk->data == nullptr)
    {
        printf("Error code: %d.\n", ERROR_DATA_REALLOC);
        exit(ERROR_DATA_REALLOC);
    }

#endif //CANARY_PROTECTION

#ifdef HASH_PROTECTION

    stk->data_hash = data_hash(stk);

#endif //HASH_PROTECTION

    fill_with_poison(stk, old_capacity, stk->capacity);

#ifdef HASH_PROTECTION

    stk->data_hash = data_hash(stk);

#endif //HASH_PROTECTION
}

//=============================================================================================================

void fill_with_poison(Stack* stk, size_t start, size_t finish)
{
    ASSERT_OK(stk);
    
    for (size_t i = start; i < finish; i++)
    {
        stk->data[i] = POISON;
    }
}

//=============================================================================================================

int open_log_file(Stack* stk)
{
    stk->log_file = fopen("log_file.txt", "w");

    if (stk->log_file == nullptr)
    {
        return ERROR_LOG_FILE_OPEN;
    }

    return 0;
}

//=============================================================================================================

int print_log_file(Stack* stk)
{
    fprintf(stk->log_file, "{\n");

    #ifdef CANARY_PROTECTION
        fprintf(stk->log_file, "[STK_LFT_CANARY] = %lu\n", stk->stk_lft_cnr);
    #endif //CANARY_PROTECTION

        fprintf(stk->log_file, "capacity  = %lu;\n", stk->capacity);

        fprintf(stk->log_file, "size      = %lu;\n", stk->size);

    #ifdef HASH_PROTECTION
        fprintf(stk->log_file, "data_hash = [%lld];\n", stk->data_hash);
    #endif //HASH_PROTECTION

        fprintf(stk->log_file, "data      = %p;\n", stk->data);

        fprintf(stk->log_file, "\t{\n");

    #ifdef CANARY_PROTECTION
        fprintf(stk->log_file, "\t[DATA_LFT_CANARY] = %lu\n", stk->data_lft_cnr);
    #endif //CANARY_PROTECTION

        for(size_t i = 0; i < stk->capacity; i++)
        {
            if(stk->data[i] == POISON)
            {
                fprintf(stk->log_file, "\tdata[%04lu] = %d             <- POISON\n", i, stk->data[i]);
            }

            else
            {
                fprintf(stk->log_file, "\tdata[%04lu] = " TYPE, i, stk->data[i]);
            }
        }

    #ifdef CANARY_PROTECTION
        fprintf(stk->log_file, "\t[DATA_RGT_CANARY] = %lu\n", stk->data_rgt_cnr);
    #endif //CANARY_PROTECTION

    fprintf(stk->log_file, "\t}\n");

    #ifdef CANARY_PROTECTION
        fprintf(stk->log_file, "[STK_RGT_CANARY] = %lu\n", stk->stk_rgt_cnr);
    #endif //CANARY_PROTECTION

    fprintf(stk->log_file, "}\n\n");

    fprintf(stk->log_file, "----------------------------------------------------\n");

    fprintf(stk->log_file, "\n");

}

//=============================================================================================================

void stk_dtor(Stack* stk)
{
    ASSERT_OK(stk);

    fill_with_poison(stk, 0, stk->capacity);

#ifdef CANARY_PROTECTION
    stk->data--;
#endif //CANARY_PROTECTION

    stk->capacity     = 0;
    stk->size         = 0;
    stk->error_code   = 0;
    stk->data_hash    = 0;
    stk->data_lft_cnr = 0;
    stk->data_rgt_cnr = 0;

    free(stk->data);
    stk->data = nullptr;

    fclose(stk->log_file);
    stk->log_file = nullptr;
}
