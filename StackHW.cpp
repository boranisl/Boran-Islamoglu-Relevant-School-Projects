#include <iostream>
#include <string>
#include "Stack.h"

using namespace std;

int main()
{
	string input;
	cout << "Please enter your input: " ;
	cin >> input ;
	Stack<char> mainStack, subStack, reverseHelperStack, reverseHelperStack2,printerStack,FandDHelperStack; //Usually main stack contains the left side of the cursor and subStack contains the right side of the cursor.
	//F and D helper Stack is helping to the find and delete function.
	bool invalidInput = false; //Checks the input is valid or not.

	for (int i = 0; i < input.length(); i++)
	{
		if (input.at(i) == '1')
		{
			break; //it just ends the for loop.
		}
		else if (input.at(i) == '2')
		{
			mainStack.pop(); //it just deletes the top element of the stack. Because this element is the element that is on the left side of the cursor.
		}
		else if (input.at(i) == '3')
		{
			subStack.push(mainStack.top->value); //Saves the right side of the cursor to the subStack.
			mainStack.pop();
		}
		else if (input.at(i) == '4')
		{
			if (subStack.top != nullptr)
			{
				mainStack.push(subStack.top->value); //Takes the right side of the cursor from the subStack.
			subStack.pop();
			}
		}
		else if (input.at(i) == '5')
		{
			while (mainStack.isEmpty()==false)
			{
				reverseHelperStack.push(mainStack.top->value); //Saves the left side of the cursor to the reverseHelperStack.
				mainStack.pop();
			}
			while (reverseHelperStack.isEmpty()==false)
			{
				reverseHelperStack2.push(reverseHelperStack.top->value); //Saves the right side of the cursor in the reverseHelperStack2 to reverse the stack.
				reverseHelperStack.pop();
			}
			while (reverseHelperStack2.isEmpty()==false)
			{
				mainStack.push(reverseHelperStack2.top->value); //Carry the values to the mainStack from the Reverse Helper 2 in correct order.
				reverseHelperStack2.pop();
			}
		}
		else if (input.at(i) == '6')
		{
			char element; //That's the element we wanna delete. (it's the x of 6x)
			if (!('0' <= input.at(i+1) && input.at(i+1)<= '9')) //that x can't be a number.
			{
				element = input.at(i+1);
				i++; // that just passes that value (x of 6x) from our input.
			}
			else
			{
				i++;
				continue;
			}
			while (mainStack.isEmpty()==false)
			{
				if (element != mainStack.top->value)
				{
					FandDHelperStack.push(mainStack.top->value);
				}
				mainStack.pop();
			}
			while (FandDHelperStack.isEmpty()==false)
			{
				mainStack.push(FandDHelperStack.top->value);
				FandDHelperStack.pop();
			}
		}
		else if (input.at(i)=='7' || input.at(i)=='8' || input.at(i)=='9' || input.at(i)=='0')
		{
			invalidInput = true;
			break;
		}
		else
		{
			//Push the characters into the stack
			mainStack.push(input.at(i));
		}
	}
	while (subStack.isEmpty() == false)
	{
		mainStack.push(subStack.top->value); //Carry the values from subStack(right side of the cursor) to our mainStack.
		subStack.pop();
	}
	while (mainStack.isEmpty() == false)
	{
		printerStack.push(mainStack.top->value); //Makes the order of the stack suitable for printing.
		mainStack.pop();
	}
	while (printerStack.isEmpty() == false && invalidInput == false)
	{
		cout << printerStack.top->value; 
		printerStack.pop();
	}
	if (invalidInput == true)
	{
		cout << "Invalid Input!" << endl;
	}
	else
	{
		cout << endl;
	}
}