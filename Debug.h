#ifndef MY_DEBUG_H
#define MY_DEBUG_H

//=============================================================================================================

#define STACK_DUMP(stack) stack_dump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__)

#define ASSERT_OK(stack)                    \
    do                                      \
    {                                       \
        if(stack_verify(stack) != 0)        \
        {                                   \
            STACK_DUMP(stack);              \
            assert_dtor(stack);             \
        }                                   \
    } while(0)                              \

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
 * @brief Prints the error data to the file stack_log.txt.
 * 
 * @param stack the stack pointer
 * @param file_name the name of the file that contains the function
 * @param line line number where the error was found
 * @param function_name the name of the function in which the error was found
 */

void stack_dump(Stack* stack, const char* file_name, size_t line, const char* function_name);

//=============================================================================================================

/**
 * @brief Considers the error code as the sum of errors in binary code. 
 * 
 * @param stack the stack pointer
*/

int stack_verify(Stack* stack);

//=============================================================================================================

/**
 * @brief Decodes the errors and prints the name of the error to the file.
 * 
 * @param  stack the stack pointer
 * @return int stack->error_code
*/

void stack_error_decoder(Stack* stack);

//=============================================================================================================

/**
 * @brief If an error was found in the middle of running the program, this function
 *        removes the stack, fills stack fields with POISON value, frees memory.
 * 
 * @param stack the stack pointer
 */

void assert_dtor(Stack* stack);

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
 * @param stack the stack pointer
 * @return long long 
 */

long long data_hash(Stack* stack);

//=============================================================================================================


#endif //MY_DEBUG_H
