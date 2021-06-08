/*
  File:    mytest.cpp
  Author:  Abel Asfaw
  Date:    1/31/20
  Section: 04
  E-mail:  aasfaw1@umbc.edu
  Description:
  This program tests the copy constructor, overloaded assignmnet operator,
  and destructor from the stack.h file
*/

#include "stack.h"

int main(){
    //original stack
    Stack<int> newStack;
    newStack.push(1);
    newStack.push(2);
    newStack.push(3);
    newStack.push(4);
    cout << "Oringinal stack:\n";
    newStack.dump();
    cout << endl;

    //assignment operator test:
    Stack<int> assignedStack;
    assignedStack = newStack;
    cout << "assignedStack:\n";
    assignedStack.dump();
    cout << endl;

    //copy constructor test:
    Stack<int> copiedStack(newStack);
    cout << "copiedStack:\n";
    copiedStack.dump();
    cout << endl;

    //pop test:
    newStack.pop();
    newStack.pop();
    cout << "Original stack after pop():\n";
    newStack.dump();
    cout << endl;
    cout << "assignedStack:\n";
    assignedStack.dump();
    cout << endl;
    cout << "copiedStack:\n";
    copiedStack.dump();
    cout << endl;

    //popping from assigned stack:
    assignedStack.pop();
    cout << "assignedStack stack after pop():\n";
    assignedStack.dump();
    cout << endl;

    //popping from copied stack:
    copiedStack.pop();
    cout << "copiedStack stack after pop():\n";
    copiedStack.dump();
    cout << endl;

    //empty stack test (edge cases):
    Stack<int> newStack2;
    cout << "newStack2:\n";
    newStack2.dump();
    cout << endl;
    Stack<int> assignedStack2;
    assignedStack2 = newStack2;
    cout << "assignedStack2:\n";
    assignedStack2.dump();
    cout << endl;
    Stack<int> copiedStack2(newStack2);
    cout << "copiedStack2:\n";
    copiedStack2.dump();
    cout << endl;

    //self-assignment test:
    assignedStack = assignedStack;
    cout << "assignedStack:\n";
    assignedStack.dump();
    cout << endl;
    return 0;
}