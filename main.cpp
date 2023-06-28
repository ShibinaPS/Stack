#include "Stack.h"
#include "Debug.h"

//=============================================================================================================

int main()
{
    Stack stack = {};
    elem_t num = 15;

    if (stack_ctor(&stack) == 0)
    {
        stack_push(&stack, 15);
        stack_push(&stack, 25);
        stack_push(&stack, 25);
        stack_push(&stack, 25);
        // printf("%d\n", stack.data[0]);

        stack_pop(&stack, &num);

        stack_dtor(&stack);
    }
}
