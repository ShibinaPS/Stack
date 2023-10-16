#include "Stack.h"
#include "Debug.h"

//=============================================================================================================

int main()
{
    struct Stack stk = {};
    elem_t num = 10;

    if (stk_ctor(&stk) == 0)
    {
        stk_push(&stk, 80);
        stk_push(&stk, 20);
        stk_push(&stk, 13);

        stk_pop(&stk, &num);
        stk_pop(&stk, &num);
        stk_pop(&stk, &num);

        stk_dtor(&stk);
    }
}
