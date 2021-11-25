//객체지향설계 텀프로젝트
//학번 : 202002505   이름 : 서지혜

#include<iostream>
#include<stdlib.h>
#include<string>
#include<stack>
#include<vector>
using namespace std;

//-------------------Singleton(연산자 우선순위 관련 부분 수행)--------------------------

//OperatorPriority : 연산자의 우선순위를 저장하는 클래스
class OperatorPriority {

	int mulAndDiv = 2;		//곱하기, 나누기 우선순위
	int plusAndMinus = 1;	//더하기, 빼기 우선순위
	int openParen = 0;      //여는 괄호의 우선순위
	int closeParen = -1;    //닫는 괄호의 우선순위

public:
	int getPriority(char op) {	//주어진 연산자의 우선순위를 반환한다.
		switch (op) {
			
		case '*':
		case '/':
			return mulAndDiv;
		case '+':
		case '-':
			return plusAndMinus;
		case '(':
			return openParen;
		case ')':
			return closeParen;
		default:
			return -2;

		}
	}
};

//Singleton : Singleton패턴으로 작성된 클래스
class Singleton {
	
	static Singleton* instance;
	
	Singleton(OperatorPriority op) {
		this->op = op;
	}

public:
	OperatorPriority op;

	static Singleton& getInstance(OperatorPriority op);
};

//Singleton의 static 멤버 내용
Singleton* Singleton::instance = NULL;
Singleton& Singleton::getInstance(OperatorPriority op) {
	if (instance == NULL) {
		instance = new Singleton(op);
	}
	return *instance;
}

//-------------------------------------------------------------------------------

//--------------------------Strategy Pattern (연산기능)--------------------------

class OperationStrategy {
public:
	virtual int doOperation(int num1, int num2) = 0;
};

class OperationMultiply : public OperationStrategy {	//곱셈 연산
public:
	int doOperation(int num1, int num2) {
		return num1 * num2;
	}
};

class OperationDivision : public OperationStrategy {	//나눗셈 연산
public:
	int doOperation(int num1, int num2) {
		return num1 / num2;
	}
};

class OperationAdd : public OperationStrategy {			//덧셈 연산
public:
	int doOperation(int num1, int num2) {
		return num1 + num2;
	}
};

class OperationSubtract : public OperationStrategy {	//뺄셈 연산
public:
	int doOperation(int num1, int num2) {
		return num1 - num2;
	}
};

class OperationContext {
	OperationStrategy* strategy;

public:
	OperationContext(OperationStrategy* strategy){
		this->strategy = strategy;
	}

	int executeStrategy(int num1, int num2) {
		return strategy->doOperation(num1, num2);
	}

};

//-------------------------------------------------------------------------------

//-----------------Strategy Pattern(정수 변환)-----------------------------------

class ConvertToDecimalStrategy {
public:
	virtual int convertToDecimal(char* num1) = 0;
};

class ConvertHexaToDecimal : public ConvertToDecimalStrategy {		//16진수 문자열에서 10진수 정수로 변환
public:
	int convertToDecimal(char* num1) {
		int result = strtol(num1, NULL, 16);
		return result;
	}
};

class ConvertBinaryToDecimal : public ConvertToDecimalStrategy {	//2진수 문자열에서 10진수 정수로 변환
public:
	int convertToDecimal(char* num1) {
		int result = strtol(num1, NULL, 2);
		return result;
	}
};

class ConvertDecimalToDecimal : public ConvertToDecimalStrategy {	//10진수 문자열에서 10진수 정수로 변환
public:
	int convertToDecimal(char* num1) {
		int result = strtol(num1, NULL, 10);
		return result;
	}
};

class ConvertToDecimalContext {
	ConvertToDecimalStrategy* strategy;

public:
	ConvertToDecimalContext(ConvertToDecimalStrategy* strategy) {
		this->strategy = strategy;
	}

	int executeStrategy(char* num1) {
		return strategy->convertToDecimal(num1);
	}

};

//-------------------------------------------------------------------------------


//--------------------------계산기 기능을 하는 클래스----------------------------

class MainController {

	OperatorPriority op;
	Singleton singleton = Singleton::getInstance(op);

public:
	
	//피연산자를 정수로 바꾸는 함수
	int convertToDecimalNumber(char* number) {

		int value = 0;

		if (number[0] == '0') {
			if (number[1] == 'x') {
				//hexa를 decimal로 변경
				ConvertToDecimalContext* context = new ConvertToDecimalContext(new ConvertHexaToDecimal());
				value = context->executeStrategy(number);
			}
			else if (number[1] == '0') {
				//binary를 decimal로 변경
				ConvertToDecimalContext* context = new ConvertToDecimalContext(new ConvertBinaryToDecimal());
				value = context->executeStrategy(number);
			}
		}
		else {
			//decimal을 decimal로 변경
			ConvertToDecimalContext* context = new ConvertToDecimalContext(new ConvertDecimalToDecimal());
			value = context->executeStrategy(number);


		}
		return value;
	}

	//중위수식에서 후위수식으로 바꾸는 함수
	vector<char> convertInfixToPostfix(string infix, vector<char> v) {
		
		stack<char> stack;

		int infixLength = infix.size();
		
		char oper;	//연산자를 저장한다.	
		int k = 0;	//하나의 피연산자의 길이를 저장한다.

		int beforeOP = 0;		  //이전 값이 연산자(+. -, /, *)인지 저장한다.	(연산자라면 1, 그렇지 않다면 0)
		int beforeOpenParen = 0;  //이전 값이 여는 괄호였는지 저장한다.			(여는괄호라면 1, 그렇지 않다면 0)

		for (int i = 0; i < infixLength; i++) {

			if (infix[i]>=48 && infix[i]<=57) {	//피연산자일 경우이다.

				char number[32] = { NULL };		//문자열 피연산자를 저장한다.
				int value = 0;					//정수 피연산자 값을 저장한다.
				int p = i;						//피연산자가 시작되는 지점이다.

				//infix수식에서 어디까지가 하나의 피연산자인지 찾는다.	
				while ((infix[p] >= 48 && infix[p] <= 57) || (infix[p] >= 65 && infix[p] <= 70) || (infix[p] >= 97 && infix[p] <= 102) || infix[p] == 'x' || infix[p] == 'b') {		
					number[k] = infix[p];
					p++;
					k++;
				}
					
				if (number[1] == 'b') {		//2진수 일 경우 number[1]에 0을 저장한다.
					number[1] = '0';
				}

				value = convertToDecimalNumber(number);		//피연산자를 정수로 바꾼다.

				if (value > 127) {			//오류 : 128이상의 수는 char에 저장될 수 없다.
					v.clear();
					return v;
				}

				i = p - 1;					//피연산자가 끝나는 지점으로 infix탐색 위치를 이동한다.

				v.push_back(value);			//피연산자를 후위수식에 저장한다.
				
				k = 0;
				beforeOP = 0;				//(+, -, /, *)가 아니므로 0 저장
				beforeOpenParen = 0;		//여는괄호가 아니므로 0 저장

			}
			else if(infix[i] == '*' || infix[i] == '/' || infix[i] == '+' || infix[i] == '-' || infix[i] == '(' || infix[i] == ')') {		//연산자일 경우이다.

				oper = infix[i];	   //연산자 저장

				if (stack.empty()) {   //스택이 비어있다면 연산자를 스택에 저장한다.					
					stack.push(oper);

					if (oper == '(' || oper == ')') {
						beforeOP = 0;				//+, -, /, * 가 아니므로 0 저장

						if (oper == '(') {
							beforeOpenParen = 1;	//여는 괄호이므로 1 저장
						}
						else {
							beforeOpenParen = 0;	//여는 괄호가 아니므로 0 저장
						}
					}
					else {
						beforeOP = 1;				//+, -, /, * 중 하나이므로 1 저장
						beforeOpenParen = 0;		//여는 괄호가 아니므로 0 저장
					}
				}
				else {					//스택이 비어있지 않다면
					
					int periorityOper = singleton.getInstance(op).op.getPriority(oper);		//연산자의 우선순위를 가져온다.
					char poppedOper;
					int temp = 0;

					switch (periorityOper) {

					case 2:			//연산이 곱셈, 나눗셈일 경우
					case 1:			//연산이 덧셈, 뺄셈일 경우

						if (beforeOP) {							//오류 : 만약 이전 값이 연산자였다면(괄호 제외) 오류이다.(예 : 10+*3)
							v.clear();
							return v;
						}

						if (beforeOpenParen) {					//오류 : 만약 이전 값이 여는괄호라면 오류이다.(예 : 10(*3))
							v.clear();
							return v;
						}

						if (i == (infixLength - 1)) {			//오류 : 수식의 마지막 값이 연산자라면 오류이다. (예 : 10-3-)
							v.clear();
							return v;
						}

						temp = stack.size();
						while (temp > 0) {
							char topOper = stack.top();			//stack의 top에 있는 연산자를 저장한다.
							int periorityOfTopOper = singleton.getInstance(op).op.getPriority(topOper);		//top에 있는 연산자의 우선순위를 가져온다.

							temp--;

							if (topOper == '(') {				//스택에 있는 연산자가 여는 괄호이면 연산자를 push 한다.
								break;
							}

							if (periorityOfTopOper >= periorityOper) {	//스택 연산자 우선순위 >= 현재 연산자 우선순위
								stack.pop();					//스택에서 연산자를 꺼낸다.
								poppedOper = topOper;
								v.push_back(poppedOper);		//스택에서 꺼낸 연산자를 후위수식에 추가한다.
								
							}
							else {			//스택 연산자의 우선순위 < 현재 연산자의 우선순위
								break;
							}
						}
						stack.push(oper);		//현재 연산자를 스택에 추가한다.
						beforeOP = 1;			//(+, -, *, /)이기 때문에 1을 저장한다.
						beforeOpenParen = 0;	//여는괄호가 아니기 때문에 0을 저장한다.
						break;					//case 2, case 1 종료

					case 0:		//연산자가 여는 괄호일 경우
						stack.push(oper);		//스택에 연산자를 추가한다.
						beforeOpenParen = 1;	//여는괄호이기 때문에 1을 저장한다.
						beforeOP = 0;			//(+, -, *, /)가 아니므로 0을 저장한다.
						break;					//case 0 종료

					case -1:	//연산자가 닫는 괄호일 경우
					
						if (beforeOP) {			//오류 : 만약 이전 값이 연산자라면(괄호 제외) 오류이다.(예 : (1+10*)3)
							v.clear();
							return v;
						}

						temp = stack.size();
						while (temp > 0) {
							char topOper = stack.top();		//스택의 top 연산자를 저장한다.

							if (topOper == '(') {			//여는 괄호 만나면 while을 종료한다.		
								stack.pop();				//스택에서 여는 괄호를 꺼낸다.
								break;
							}
							else {							//여는 괄호 만날때까지 스택에서 연산자를 꺼낸다.
								stack.pop();				//스택에서 연산자를 꺼낸다.
								poppedOper = topOper;
								v.push_back(poppedOper);	//연산자를 후위수식에 추가한다.
								
							}
							temp--;
						}
						beforeOP = 0;				//(+, -, /, *)가 아니므로 0 저장
						beforeOpenParen = 0;		//여는 괄호가 아니므로 0 저장

						if (temp == 0) {			//오류 : 스택에 여는 괄호가 없다면 오류이다. (예 : 10*3)+3)
							v.clear();
							return v;
						}	
					default:
						break;	
					}
				}
			}
			else {			//오류 : 수식의 값이 연산자, 피연산자가 아닐 경우 오류이다. v를 비운 뒤 반환한다.
				v.clear();
				return v;
			}
		}

		while (!stack.empty()) {		//스택에 남은 연산자를 모두 꺼낸다.
			char topOper = stack.top();

			if (topOper == '(') {		//오류 : 여는괄호가 스택에 남아있으면 오류이다.	(예 : 10(3+3 )
				v.clear();
				return v;
			}

			stack.pop();				//스택에서 연산자를 꺼낸다.
			char poppedOper = topOper;
			v.push_back(poppedOper);	//꺼낸 연산자를 후위수식에 추가한다.
			
		}
		return v;
	}

	//후위수식 연산을 수행하는 함수
	int calculate(vector<char> postfix) {
		
		stack<int> stack;
		int i = 0;
		int postfixlength = postfix.size();

		char component;
		int operationResult;

		while (i < postfixlength) {			//후위 수식의 끝까지 반복한다.
			
			component = postfix.at(i);		//수식에 있는 값을 가져온다.
			i++;
			if (component == '*' || component == '/' || component == '+' || component == '-') {		//수식의 값이 연산자일 경우
				
				if (stack.size() == 1) {	//오류 : 피연산자가 1개인데 연산자가 있는 경우는 오류이다.
					return NULL;
				}

				int stackNum1 = stack.top();	//스택에서 연산자를 꺼낸다
				stack.pop();
				int stackNum2 = stack.top();	//스택에서 연산자를 꺼낸다.
				stack.pop();

				OperationContext* context;

				//연산자에 맞는 연산을 수행한다.
				if (component == '*') {
					context = new OperationContext(new OperationMultiply());
				}
				else if (component == '/') {
					context = new OperationContext(new OperationDivision());
					if (stackNum1 == 0) {		//오류 : 0으로 나누는 것은 오류이다.
						return NULL;
					}
				}
				else if (component == '+') {
					context = new OperationContext(new OperationAdd());
				}
				else {
					context = new OperationContext(new OperationSubtract());
				}
				int result = context->executeStrategy(stackNum2, stackNum1);

				stack.push(result);		//연산결과를 스택에 저장한다.
			}
			else {		//수식의 값이 피연산자일 경우
				int b = component;		
				stack.push(b);	//스택에 피연산자를 추가한다.
			}
		}
		operationResult = stack.top();		//스택에 있는 연산결과를 저장한다.
		stack.pop();

		if (!stack.empty()) {				//오류 : 스택에 피연산자가 남아있으면 오류이다.
			return NULL;
		}

		return operationResult;
	}
};
//-------------------------------------------------------------------------------

void main() {
	
	//----전체적인 절차----
	//수식 입력받기
	//후위수식으로 변경하기
	//후위수식 계산하기
	//결과 출력하기
	//---------------------

	try {

		string input;	//입력된 수식을 저장한다.

		cout << "-----------------------[ 계산기 프로그램 입니다. ]-----------------------" << endl;
		cout << "--------[계산이 가능한 피연산자는 정수 0이상 127이하의 값입니다.]--------" << endl;
		cout << "[  계산을 진행하려면 [실행], 프로그램을 종료하려면 [종료]를 입력하세요.  ]" << endl;
		cout << "> 입력 : ";
		getline(cin, input);

		while (input != "종료") {		//종료가 입력되기 전까지 프로그램을 실행한다.
			if (input != "실행") {
				cout << "[        <오류> 잘못된 입력입니다. [종료] 혹은 [실행]을 입력하세요.      ]" << endl;			//오류 : 종료, 실행을 입력하지 않을 경우
			}
			else {
				cout << "[                           수식을 입력하세요.                           ]" << endl;
				cout << "> 수식입력 : ";
				getline(cin, input);
				if (input.empty()) {
					cout << "[         <오류> 수식이 입력되지 않았습니다. 수식을 입력하세요.          ]" << endl;		//오류 : 수식 입력이 없을 경우
				}
				else {
					//입력된 중위 수식을 후위 표기식으로 변환한다.
					MainController mc;
					vector<char> postfixVector;
					postfixVector = mc.convertInfixToPostfix(input, postfixVector);
					if (postfixVector.empty()) {
						cout << "[          <오류> 잘못된 수식입니다. 올바른 수식을 입력하세요.           ]" << endl;	//후위표기식 변환 과정에서의 오류출력
					}
					else {
						//변환된 후위 표기식을 출력한다.
						cout << "[                 입력한 식을 후위표기법으로 변환합니다.                 ]" << endl;
						cout << "> 후위수식 : ";
						int k = 0;
						while (k < postfixVector.size()) {

							if (postfixVector.at(k) == '*' || postfixVector.at(k) == '/' || postfixVector.at(k) == '+' || postfixVector.at(k) == '-') {
								char a = postfixVector.at(k);
								cout << a;
							}
							else {
								int b = postfixVector.at(k);
								cout << b;
							}
							k++;
						}
						cout << "" << endl;

						//수식을 계산한다.
						int result = mc.calculate(postfixVector);

						if (result == NULL) {
							cout << "[    <오류> 계산에 적합하지 않은 수식입니다. 올바른 수식을 입력하세요.   ]" << endl;	//수식 계산 과정에서 발생한 오류를 출력한다.
						}
						else {
							//계산결과를 출력한다.
							cout << "[                     입력한 식의 결과를 출력합니다.                     ]" << endl;
							cout << "> 계산결과 : " << result << endl;
							cout << "" << endl;
							cout << "" << endl;
						}
					}
				}
			}
			cout << "[  계산을 진행하려면 [실행], 프로그램을 종료하려면 [종료]를 입력하세요.  ]" << endl;
			cout << "> 입력 : ";
			getline(cin, input);
		}
		cout << "------------------------[ 프로그램을 종료합니다. ]-----------------------" << endl;
	}
	catch (...) {
		cout << "[              예외가 발생하였습니다. 프로그램을 종료합니다.             ]" << endl;
	}

	return;
}









