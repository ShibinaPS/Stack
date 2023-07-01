#ifndef MY_DEBUG_H
#define MY_DEBUG_H

//=============================================================================================================

#define STACK_DUMP(stk) stk_dump(stk, __FILE__, __LINE__, __PRETTY_FUNCTION__)

#define ASSERT_OK(stk)              \
    do                              \
    {                               \
        if(stk_verify(stk) != 0)    \
        {                           \
            STACK_DUMP(stk);        \
            assert_dtor(stk);       \
        }                           \
    } while(0)                      \

//=============================================================================================================

/**
 * @brief Notes the type of error in stack.
 * 
*/
enum StackErrors
{
      ERROR_DATA_NULLPTR      = 1 << 0,
      ERROR_STACK_CAPACITY    = 1 << 1,
      ERROR_STK_LFT_CNR_DEAD  = 1 << 2,
      ERROR_STK_RGT_CNR_DEAD  = 1 << 3,
      ERROR_DATA_LFT_CNR_DEAD = 1 << 4,
      ERROR_DATA_RGT_CNR_DEAD = 1 << 5,
      ERROR_DATA_HASH         = 1 << 6,
};

//=============================================================================================================

/**
 * @brief Prints the error data to the file stk_log.txt.
 * 
 * @param stk the stack pointer
 * @param file_name the name of the file that contains the function
 * @param line line number where the error was found
 * @param function_name the name of the function in which the error was found
 */

void stk_dump(Stack* stk, const char* file_name, size_t line, const char* function_name);

//=============================================================================================================

/**
 * @brief Considers the error code as the sum of errors in binary code. 
 * 
 * @param stk the stack pointer
*/

int stk_verify(Stack* stk);

//=============================================================================================================

/**
 * @brief Decodes the errors and prints the name of the error to the file.
 * 
 * @param  stk the stack pointer
 * @return int stk->error_code
*/

void stk_error_decoder(Stack* stk);

//=============================================================================================================

/**
 * @brief If an error was found in the middle of running the program, this function
 *        removes the stack, fills stack fields with POISON value, frees memory.
 * 
 * @param stk the stack pointer
 */

void assert_dtor(Stack* stk);

//=============================================================================================================

/**
 * @brief Calculates hash given pointer and size.
 * 
 * @param pointer pointer to part of the stack
 * @param size the number of elements over which the hash will be calculated
 * @return long long 
 */

long long calculate_hash(elem_t* pointer, size_t size);

//=============================================================================================================

/**
 * @brief Applies function calculate_hash(...) for calculate hash of stack data.
 * 
 * @param stk the stack pointer
 * @return long long 
 */

long long data_hash(Stack* stk);

//=============================================================================================================


#endif //MY_DEBUG_H
