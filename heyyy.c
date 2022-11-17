#include <stdio.h>
#include <stdlib.h>
#include <math.h>


//STRUCTURES
struct stackNode{  //Stack
    int data;
    struct stackNode *nextPtr;
};
typedef struct stackNode StackNode;
typedef StackNode *StackNodePtr;


struct pointerStack{ //This is a stack of pointers. Each pointer points a stack.
    StackNodePtr ptr;
    struct pointerStack *nextPtr;
};
typedef struct pointerStack PointerStack;
typedef PointerStack *PointerStackPtr;


//FUNCTION PROTOTYPES
int isEmpty(StackNodePtr topPtr);
int pop(StackNodePtr *topPtr);
int push(StackNodePtr *topPtr, int value);
int isEmpty2(PointerStackPtr stack);
int pop2(PointerStackPtr *topPtr);
int push2(PointerStackPtr *topPtr, StackNodePtr stackNodePtr);
int stackToNum(StackNodePtr stack);
void reverse(StackNodePtr *stack);
int findMaxDigit(PointerStackPtr stack);
int length(StackNodePtr stack);
void sum(PointerStackPtr numbers, StackNodePtr *result, int base);
void multiply(StackNodePtr num1, StackNodePtr num2, int base, StackNodePtr *result);
void convert_base10(StackNodePtr *base_10, StackNodePtr stack, int base);
void printStack(StackNodePtr stack);



int main(int argc, char *argv[]) {

    StackNodePtr num1 = NULL; //Numbers are stored in a stack
    StackNodePtr num2 = NULL;
    StackNodePtr baseStack = NULL;


    //Reading the numbers
    FILE *fptr = fopen(argv[1], "r");
    if(fptr == NULL){  //Print error if the file cannot be opened
        printf(" Input file couldn't be opened! Check your file.\n");
    }

    int lineNumber = 1; //In the input file, we have num1 in line 1, num2 in line 2 and base in line 3;
    int currentChar;

    while((currentChar = fgetc(fptr)) != EOF){

        if(currentChar == ' ') //if the current char is a blank, skip it
            continue;

        if(currentChar == '\n'){ //If we get the \n char, we update our lineNumber and continue from the new line
            lineNumber++;
            continue;
        }

        switch (lineNumber) {
            case 1: push(&num1, currentChar - '0'); break;
            case 2: push(&num2, currentChar - '0'); break;
            case 3: push(&baseStack, currentChar - '0'); break;
        }
    }
    fclose(fptr);
    //Data has been read.



    int base = stackToNum(baseStack);

    StackNodePtr result = NULL;
    multiply(num1, num2, base, &result);




    //In the second part, we don't have to multiply it again, basically we will convert num1, num2 and the result to base 10.
    StackNodePtr num1_base10 = NULL;
    StackNodePtr num2_base10 = NULL;
    StackNodePtr result_base10 = NULL;

    convert_base10(&num1_base10, num1, base);
    convert_base10(&num2_base10, num2, base);
    convert_base10(&result_base10, result, base);



    //printing the result
    printStack(num1);
    printStack(num2);
    printStack(result);
    printStack(num1_base10);
    printStack(num2_base10);
    printStack(result_base10);



    return 0;
}


int push(StackNodePtr *topPtr, int value){
    StackNodePtr newNode = malloc(sizeof(StackNode));
    if(newNode == NULL){
        printf("Memory is not available!\n");
        return 0; //Fail
    }
    newNode->data = value;
    newNode->nextPtr = *topPtr;
    (*topPtr) = newNode;
    return 1; //Success
}


int pop(StackNodePtr *topPtr){
    if(isEmpty(*topPtr)){
        printf("Stack is empty!\n");
        return -1; //fail
    }
    int poppedValue = (*topPtr)->data;
    StackNodePtr temp = *topPtr;
    *topPtr = (*topPtr)->nextPtr;
    free(temp);
    return poppedValue;
}


int isEmpty(StackNodePtr topPtr){
    return topPtr == NULL;
}

int push2(PointerStackPtr *topPtr, StackNodePtr stackNodePtr){
    PointerStackPtr newStack = malloc(sizeof(PointerStack));
    if(newStack == NULL){
        printf("Memory is not available!\n");
        return 0; //Fail
    }
    newStack->ptr = stackNodePtr;
    newStack->nextPtr = *topPtr;
    *topPtr = newStack;
    return 1; //Success
}

int pop2(PointerStackPtr *topPtr){
    if(isEmpty2(*topPtr)){
        printf("Stack is empty!\n");
        return 0; //Fail
    }
    PointerStackPtr temp = *topPtr;
    *topPtr = (*topPtr)->nextPtr;
    free(temp);
    return 1; //Success
}

int isEmpty2(PointerStackPtr stack){
    return stack == NULL;
}

int stackToNum(StackNodePtr stack){
    int num = 0;
    int count = 0;
    while(stack != NULL){
        num = num + (stack->data) * pow(10, count);
        stack = stack->nextPtr;
        count++;
    }
    return num;
}

void reverse(StackNodePtr *stack){
    StackNodePtr newStack = NULL;

    while((*stack) != NULL){     //Transfer all the elements to a new stack so that elements will be up down.
        int value = pop(stack);
        push(&newStack, value);
    }
    *stack = newStack;
    return;
}


int length(StackNodePtr stack){
    int counter = 0;
    while(stack != NULL){
        counter++;
        stack = stack->nextPtr;
    }
    return counter;
}

int findMaxDigit(PointerStackPtr stack){
    int maxDigit = 0;
    while(stack != NULL){      //Finding the max number of digits among the partial multiplies
        int currentMax = length(stack->ptr);
        if(currentMax > maxDigit)
            maxDigit = currentMax;
        stack = stack->nextPtr;
    }
    return maxDigit;
}


void convert_base10(StackNodePtr *base_10, StackNodePtr stack, int base){

    int num = 0;
    PointerStackPtr partialMultiplies = NULL;

    StackNodePtr exponent = NULL;
    push(&exponent, 1);

    StackNodePtr baseValue = NULL;
    push(&baseValue, base);



    while(stack != NULL){
        StackNodePtr currentMultiply = NULL;
        StackNodePtr digit = NULL;
        push(&digit, stack->data);

        multiply(digit, exponent, 10, &currentMultiply);
        push2(&partialMultiplies, currentMultiply);

        StackNodePtr temp = NULL;
        multiply(exponent, baseValue, 10, &temp); //Update the exponent
        exponent = temp;
        stack = stack->nextPtr;
    }
    sum(partialMultiplies, base_10, 10);



}



void sum(PointerStackPtr numbers, StackNodePtr *result, int base){
    int maxDigit = findMaxDigit(numbers); //Finding the max digit among the numbers
    int carryOut = 0;
    for(int i = 0; i < maxDigit; i++){
        int sum = 0;
        PointerStackPtr currentNumbers = numbers;

        while(currentNumbers != NULL){
            if(!isEmpty(currentNumbers->ptr))
                sum = sum + pop(&(currentNumbers->ptr));
            currentNumbers = currentNumbers->nextPtr;
        }

        int remainder = (sum + carryOut) % base;
        carryOut = (sum + carryOut) / base;
        push(result, remainder);
    }
    if(carryOut != 0)
        push(result, carryOut);
    reverse(result);
}


void multiply(StackNodePtr num1, StackNodePtr num2, int base, StackNodePtr *result){


    if(length(num2) > length(num1)) { //To multiply correctly, we have to check the length and place it correctly!
        StackNodePtr temp = num1;
        num1 = num2;
        num2 = temp;
    }


    StackNodePtr currentNum1 = num1;
    StackNodePtr currentNum2 = num2;


    PointerStackPtr partialMultiplies = NULL;
    int numberOfPartialMultiplies = 0;

    while(currentNum2 != NULL){
        int digit2 = currentNum2->data;
        StackNodePtr partialMultiply = NULL;
        int carryOut = 0;

        for(int i = 0; i < numberOfPartialMultiplies; i++){ //Add zero to the beginning of the partial multiply by analyzing which partial multiply it is
            push(&partialMultiply, 0);
        }

        currentNum1 = num1;
        while(currentNum1 != NULL){
            int digit1 = currentNum1->data;
            int sum = digit1 * digit2 + carryOut;
            int remainder = sum % base;
            push(&partialMultiply, remainder);
            carryOut = sum / base;
            currentNum1 = currentNum1->nextPtr;
        }
        if(carryOut != 0){
            push(&partialMultiply, carryOut);
        }

        reverse(&partialMultiply); //We have to reverse the partialmultiply for the next operation which is summing all the partial multiplies
        push2(&partialMultiplies, partialMultiply);
        numberOfPartialMultiplies++;
        currentNum2 = currentNum2->nextPtr;
    }

    sum(partialMultiplies, result, base);
}

void printStack(StackNodePtr stack){
    reverse(&stack);
    while(stack != NULL){
        printf("%d", stack->data);
        stack = stack->nextPtr;
    }
    printf("\n");
}