#include "Stack.h"
#include "Debug.h"

//=============================================================================================================

int main()
{
    Stack stk = {};
    elem_t num = 10;

    if (stk_ctor(&stk) == 0)
    {
        stk_push(&stk, 15);
        stk_push(&stk, 20);
        stk_push(&stk, 35);

        stk_pop(&stk, &num);
        stk_pop(&stk, &num);

        stk_dtor(&stk);
    }
}
