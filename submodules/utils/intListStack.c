#include "./headers/intListStack.h"

// This creates a stack of int arrays with determined maximum size
intListStack* createIntListStackWithFirstElem(intListeElem* elem, int maxSize)
{
    intListStack* stack = malloc(sizeof(intListStack));
    stack->maxSize = maxSize;
    stack->currentSize = 1;

    // Now we create the stack element
    intListStackElem* stackElem = malloc(sizeof(intListStackElem));
    stackElem->value = elem;
    stackElem->previous = NULL;
    stackElem->next = NULL;

    stack->first = stackElem;
    stack->last = stackElem;

    return stack;
}

void stackIntListStack(intListStack* stack, intListeElem* elem)
{
    intListStackElem* stackElem = malloc(sizeof(intListStackElem));
    stackElem->value = elem;
    stackElem->previous = NULL;
    stackElem->next = stack->first;

    stack->first->previous = stackElem;  // We add the previous value for the second value

    stack->first = stackElem;
    stack->currentSize ++;

    if(stack->currentSize > stack->maxSize)
    {
        removeLastElem(stack);
    }
}

intListeElem* unstackIntListStack(intListStack* stack)
{
    if(stack->currentSize > 1)  // If there is at least 2 elements
    {
        intListStackElem* valueToRemove = stack->first;

        stack->first = valueToRemove->next;
        stack->first->previous = NULL;

        intListeElem* valueToReturn = valueToRemove->value;
        free(valueToRemove);

        stack->currentSize --;

        return valueToReturn;
    }
    else  // Else we simply return the first value and do nothing
    {
        return stack->first->value;
    }
}

void removeLastElem(intListStack* stack)
{
    intListStackElem* valueToRemove = stack->last;
    destroyIntList(stack->last->value);

    stack->last = valueToRemove->previous;
    stack->last->next = NULL;

    free(valueToRemove);
    
    stack->currentSize --;
}

void destroyIntListStack(intListStack* stack)
{
    while(stack->currentSize > 1)
    {
        removeLastElem(stack);  // easier and probably faster to do it from last to first element;
    }
    // Now we need to handle the last element
    destroyIntList(stack->first->value);
    free(stack->first);
    free(stack);
}

void printIntListStack(intListStack* stack)
{
    printf("Stack : size : %i/%i, first : %p, last : %p\n", stack->currentSize, stack->maxSize, stack->first, stack->last);

    int cmp = 0;
    intListStackElem* currentElem = stack->first;
    while(currentElem != NULL)
    {
        printf("Elem %i :\nadress : %p, value : %p\nprevious : %p,next : %p\n",
            cmp, currentElem, currentElem->value, currentElem->previous, currentElem->next);
        currentElem = currentElem->next;
        cmp++;
    }
}