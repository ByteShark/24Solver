// 24Solver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <assert.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class Solve24
{
	public:

		Solve24()
		{
			findOperations(std::vector<Operation>());
		}

		bool solve(int a, int b, int c, int d)
		{
			bool result = false;

			solutions = 0;

			// For speed, this is all 24 permutations of 4 items.

			result = (tryPermutation(a, b, c, d) || result);
			result = (tryPermutation(a, b, d, c) || result);
			result = (tryPermutation(a, c, b, d) || result);
			result = (tryPermutation(a, c, d, b) || result);
			result = (tryPermutation(a, d, b, c) || result);
			result = (tryPermutation(a, d, c, b) || result);
			result = (tryPermutation(b, a, c, d) || result);
			result = (tryPermutation(b, a, d, c) || result);
			result = (tryPermutation(b, c, a, d) || result);
			result = (tryPermutation(b, c, d, a) || result);
			result = (tryPermutation(b, d, a, c) || result);
			result = (tryPermutation(b, d, c, a) || result);
			result = (tryPermutation(c, a, b, d) || result);
			result = (tryPermutation(c, a, d, b) || result);
			result = (tryPermutation(c, b, a, d) || result);
			result = (tryPermutation(c, b, d, a) || result);
			result = (tryPermutation(c, d, a, b) || result);
			result = (tryPermutation(c, d, b, a) || result);
			result = (tryPermutation(d, a, b, c) || result);
			result = (tryPermutation(d, a, c, b) || result);
			result = (tryPermutation(d, b, a, c) || result);
			result = (tryPermutation(d, b, c, a) || result);
			result = (tryPermutation(d, c, a, b) || result);
			result = (tryPermutation(d, c, b, a) || result);

			if (solutions == 0)
			{
				std::cout << std::endl << "No solutions found for numbers " << a << ", " << b << ", " << c << ", and " << d << std::endl;
			}
			else
			{
				std::cout << std::endl << solutions << " solutions found for numbers " << a << ", " << b << ", " << c << ", and " << d << std::endl;
			}
			
			return result;
		}

	protected:

		// Arithmetic operations

		enum Operation
		{
			Addition,
			Subtraction,
			Multiplication,
			Division
		};

		// Reverse Polish Notation for operation ordering
		//
		// N N N N O O O
		// N N N O N O O
		// N N N O O N O
		// N N O N N O O
		// N N O N O N O

		// N  is number/operand, O is operation

		enum RPN
		{
			N_N_N_N_O_O_O,
			N_N_N_O_N_O_O,
			N_N_N_O_O_N_O,
			N_N_O_N_N_O_O,
			N_N_O_N_O_N_O
		};

		static const int target = 24;

		int solutions = 0;

		std::vector<std::vector<Operation>> operations;
		std::vector<RPN> rpn = {N_N_N_N_O_O_O, N_N_N_O_N_O_O, N_N_N_O_O_N_O, N_N_O_N_N_O_O, N_N_O_N_O_N_O};
		
		void findOperations(std::vector<Operation> currentOperation)
		{
			if (currentOperation.size() == 3)
			{
				operations.push_back(currentOperation);
			}
			else
			{
				std::vector<Operation> operations = { Addition, Subtraction, Multiplication, Division };

				for (size_t i = 0; i < operations.size(); i++)
				{
					currentOperation.push_back(operations[i]);

					findOperations(currentOperation);

					currentOperation.pop_back();
				}
			}
		}

		bool tryPermutation(int a, int b, int c, int d)
		{
			bool result = false;

			for (size_t i = 0; i < operations.size(); i++)
			{
				result = (calculateOperations(a, b, c, d, operations[i]) || result);
			}

			return result;
		}

		bool calculateOperations(int a, int b, int c, int d, std::vector<Operation> operations)
		{
			bool result = false;
			
			for (size_t i = 0; i < rpn.size(); i++)
			{
				result = (calculateOperations(a, b, c, d, operations, rpn[i]) || result);
			}

			return result;
		}

		bool calculateOperations(int a, int b, int c, int d, std::vector<Operation> operations, RPN rpn)
		{
			bool result = true;

			// For each operation collection, there are 5 possible grouping of 4 operands and 3 operators
			//
			// In Reverse Polish Notation they look like this 
			//
			// N N N N O O O
			// N N N O N O O
			// N N N O O N O
			// N N O N N O O
			// N N O N O N O

			// N  is number/operand, O is operation

			double value = 0;
			double value2 = 0;
	
			try
			{
				switch (rpn)
				{
					case N_N_N_N_O_O_O:

						//  infix a + (b + (c + d))

						value = calculate(c, operations[2], d);
						value = calculate(b, operations[1], value);
						value = calculate(a, operations[0], value);

						break;

					case N_N_N_O_N_O_O:

						// infix a + ((b + c) + d)

						value = calculate(b, operations[1], c);
						value = calculate(value, operations[2], d);
						value = calculate(a, operations[0], value);

						break;

					case N_N_N_O_O_N_O:

						// infix (a + (b + c)) + d

						value = calculate(b, operations[1], c);
						value = calculate(a, operations[0], value);
						value = calculate(value, operations[2], d);

						break;

					case N_N_O_N_N_O_O:

						// infix (a + b) + (c + d)

						value = calculate(a, operations[0], b);
						value2 = calculate(c, operations[2], d);
						value = calculate(value, operations[1], value2);

						break;

					case N_N_O_N_O_N_O:

						// infix ((a + b) + c) + d

						value = calculate(a, operations[0], b);
						value = calculate(value, operations[1], c);
						value = calculate(value, operations[2], d);

						break;
				}
			}
			catch (...)
			{
				// division by zero

				result = false;
			}

			if (result)
			{
				if (value == target)
				{
					printOperations(a, b, c, d, operations, rpn);
				}
				else
				{
					result = false;
				}
			}
			
			return result;
		}

		void printOperations(int a, int b, int c, int d, std::vector<Operation> operations, RPN rpn)
		{
			solutions++;

			std::string operators;

			assert(operations.size() == 3);

			for (size_t i = 0; i < operations.size(); i++)
			{
				switch (operations[i])
				{
					case Addition:
						operators += "+";
						break;
					case Subtraction:
						operators += "-";
						break;
					case Multiplication:
						operators += "*";
						break;
					case Division:
						operators += "/";
						break;
				}
			}

			switch (rpn)
			{
				case N_N_N_N_O_O_O:

					//  infix a + (b + (c + d))
					
					std::cout << a << " " << operators[0] << " " <<  "(" << b << " " << operators[1] <<  " (" << c << " " << operators[2] << " " << d << "))" << std::endl;
					break;

				case N_N_N_O_N_O_O:

					// infix a + ((b + c) + d)
				
					std::cout << a << " " << operators[0] << " " << "((" << b << " " << operators[1] << " " << c << ") " << operators[2] << " " << d << "))" << std::endl;
					break;

				case N_N_N_O_O_N_O:

					// infix (a + (b + c)) + d

					std::cout << "(" << a << " " << operators[0] << " " << "(" << b << " " << operators[1] << " " << c << ")) " << operators[2] << " " << d << std::endl;
					break;

				case N_N_O_N_N_O_O:

					// (a + b) + (c + d)

					std::cout << "(" << a << " " << operators[0] << " " << b << ") " << operators[1] << " (" << c << " " << operators[2] << " " << d << ")" << std::endl;
					break;

				case N_N_O_N_O_N_O:

					// infix ((a + b) + c) + d

					std::cout << "((" << a << " " << operators[0] << " " << b << ") " << operators[1] << " " << c << ") " << operators[2] << " " << d  << std::endl;
					break;
			}
		}

		double calculate(double a, Operation operation, double b)
		{
			double value = 0;

			switch (operation)
			{
				case Addition:
					value = a + b;
					break;
				case Subtraction:
					value = a - b;
					break;
				case Multiplication:
					value = a * b;
					break;
				case Division:

					// Check for divide by zero

					if (b != 0)
					{
						value = a / b;
					}
					else
					{
						throw("divide by zero");
					}
					break;
			}
			
			return value;
		}
};

int main()
{
	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;

	char answer;

	Solve24 solver;

	while (true)
	{
		std::cout << " Enter first number: ";
		std::cin >> a;

		std::cout << "Enter second number: ";
		std::cin >> b;

		std::cout << " Enter third number: ";
		std::cin >> c;

		std::cout << "Enter fourth number: ";
		std::cin >> d;

		std::cout << std::endl;

		solver.solve(a, b, c, d);

		std::cout << std::endl << "Continue (Y/N)? ";
		std::cin >> answer;

		if (answer == 'Y' || answer == 'y')
		{
			std::cout << std::endl;
		}
		else
		{
			break;
		}
	}

    return 0;
}

