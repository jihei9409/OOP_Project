//��ü���⼳�� ��������Ʈ
//�й� : 202002505   �̸� : ������

#include<iostream>
#include<stdlib.h>
#include<string>
#include<stack>
#include<vector>
using namespace std;

//-------------------Singleton(������ �켱���� ���� �κ� ����)--------------------------

//OperatorPriority : �������� �켱������ �����ϴ� Ŭ����
class OperatorPriority {

	int mulAndDiv = 2;		//���ϱ�, ������ �켱����
	int plusAndMinus = 1;	//���ϱ�, ���� �켱����
	int openParen = 0;      //���� ��ȣ�� �켱����
	int closeParen = -1;    //�ݴ� ��ȣ�� �켱����

public:
	int getPriority(char op) {	//�־��� �������� �켱������ ��ȯ�Ѵ�.
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

//Singleton : Singleton�������� �ۼ��� Ŭ����
class Singleton {
	
	static Singleton* instance;
	
	Singleton(OperatorPriority op) {
		this->op = op;
	}

public:
	OperatorPriority op;

	static Singleton& getInstance(OperatorPriority op);
};

//Singleton�� static ��� ����
Singleton* Singleton::instance = NULL;
Singleton& Singleton::getInstance(OperatorPriority op) {
	if (instance == NULL) {
		instance = new Singleton(op);
	}
	return *instance;
}

//-------------------------------------------------------------------------------

//--------------------------Strategy Pattern (������)--------------------------

class OperationStrategy {
public:
	virtual int doOperation(int num1, int num2) = 0;
};

class OperationMultiply : public OperationStrategy {	//���� ����
public:
	int doOperation(int num1, int num2) {
		return num1 * num2;
	}
};

class OperationDivision : public OperationStrategy {	//������ ����
public:
	int doOperation(int num1, int num2) {
		return num1 / num2;
	}
};

class OperationAdd : public OperationStrategy {			//���� ����
public:
	int doOperation(int num1, int num2) {
		return num1 + num2;
	}
};

class OperationSubtract : public OperationStrategy {	//���� ����
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

//-----------------Strategy Pattern(���� ��ȯ)-----------------------------------

class ConvertToDecimalStrategy {
public:
	virtual int convertToDecimal(char* num1) = 0;
};

class ConvertHexaToDecimal : public ConvertToDecimalStrategy {		//16���� ���ڿ����� 10���� ������ ��ȯ
public:
	int convertToDecimal(char* num1) {
		int result = strtol(num1, NULL, 16);
		return result;
	}
};

class ConvertBinaryToDecimal : public ConvertToDecimalStrategy {	//2���� ���ڿ����� 10���� ������ ��ȯ
public:
	int convertToDecimal(char* num1) {
		int result = strtol(num1, NULL, 2);
		return result;
	}
};

class ConvertDecimalToDecimal : public ConvertToDecimalStrategy {	//10���� ���ڿ����� 10���� ������ ��ȯ
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


//--------------------------���� ����� �ϴ� Ŭ����----------------------------

class MainController {

	OperatorPriority op;
	Singleton singleton = Singleton::getInstance(op);

public:
	
	//�ǿ����ڸ� ������ �ٲٴ� �Լ�
	int convertToDecimalNumber(char* number) {

		int value = 0;

		if (number[0] == '0') {
			if (number[1] == 'x') {
				//hexa�� decimal�� ����
				ConvertToDecimalContext* context = new ConvertToDecimalContext(new ConvertHexaToDecimal());
				value = context->executeStrategy(number);
			}
			else if (number[1] == '0') {
				//binary�� decimal�� ����
				ConvertToDecimalContext* context = new ConvertToDecimalContext(new ConvertBinaryToDecimal());
				value = context->executeStrategy(number);
			}
		}
		else {
			//decimal�� decimal�� ����
			ConvertToDecimalContext* context = new ConvertToDecimalContext(new ConvertDecimalToDecimal());
			value = context->executeStrategy(number);


		}
		return value;
	}

	//�������Ŀ��� ������������ �ٲٴ� �Լ�
	vector<char> convertInfixToPostfix(string infix, vector<char> v) {
		
		stack<char> stack;

		int infixLength = infix.size();
		
		char oper;	//�����ڸ� �����Ѵ�.	
		int k = 0;	//�ϳ��� �ǿ������� ���̸� �����Ѵ�.

		int beforeOP = 0;		  //���� ���� ������(+. -, /, *)���� �����Ѵ�.	(�����ڶ�� 1, �׷��� �ʴٸ� 0)
		int beforeOpenParen = 0;  //���� ���� ���� ��ȣ������ �����Ѵ�.			(���°�ȣ��� 1, �׷��� �ʴٸ� 0)

		for (int i = 0; i < infixLength; i++) {

			if (infix[i]>=48 && infix[i]<=57) {	//�ǿ������� ����̴�.

				char number[32] = { NULL };		//���ڿ� �ǿ����ڸ� �����Ѵ�.
				int value = 0;					//���� �ǿ����� ���� �����Ѵ�.
				int p = i;						//�ǿ����ڰ� ���۵Ǵ� �����̴�.

				//infix���Ŀ��� �������� �ϳ��� �ǿ��������� ã�´�.	
				while ((infix[p] >= 48 && infix[p] <= 57) || (infix[p] >= 65 && infix[p] <= 70) || (infix[p] >= 97 && infix[p] <= 102) || infix[p] == 'x' || infix[p] == 'b') {		
					number[k] = infix[p];
					p++;
					k++;
				}
					
				if (number[1] == 'b') {		//2���� �� ��� number[1]�� 0�� �����Ѵ�.
					number[1] = '0';
				}

				value = convertToDecimalNumber(number);		//�ǿ����ڸ� ������ �ٲ۴�.

				if (value > 127) {			//���� : 128�̻��� ���� char�� ����� �� ����.
					v.clear();
					return v;
				}

				i = p - 1;					//�ǿ����ڰ� ������ �������� infixŽ�� ��ġ�� �̵��Ѵ�.

				v.push_back(value);			//�ǿ����ڸ� �������Ŀ� �����Ѵ�.
				
				k = 0;
				beforeOP = 0;				//(+, -, /, *)�� �ƴϹǷ� 0 ����
				beforeOpenParen = 0;		//���°�ȣ�� �ƴϹǷ� 0 ����

			}
			else if(infix[i] == '*' || infix[i] == '/' || infix[i] == '+' || infix[i] == '-' || infix[i] == '(' || infix[i] == ')') {		//�������� ����̴�.

				oper = infix[i];	   //������ ����

				if (stack.empty()) {   //������ ����ִٸ� �����ڸ� ���ÿ� �����Ѵ�.					
					stack.push(oper);

					if (oper == '(' || oper == ')') {
						beforeOP = 0;				//+, -, /, * �� �ƴϹǷ� 0 ����

						if (oper == '(') {
							beforeOpenParen = 1;	//���� ��ȣ�̹Ƿ� 1 ����
						}
						else {
							beforeOpenParen = 0;	//���� ��ȣ�� �ƴϹǷ� 0 ����
						}
					}
					else {
						beforeOP = 1;				//+, -, /, * �� �ϳ��̹Ƿ� 1 ����
						beforeOpenParen = 0;		//���� ��ȣ�� �ƴϹǷ� 0 ����
					}
				}
				else {					//������ ������� �ʴٸ�
					
					int periorityOper = singleton.getInstance(op).op.getPriority(oper);		//�������� �켱������ �����´�.
					char poppedOper;
					int temp = 0;

					switch (periorityOper) {

					case 2:			//������ ����, �������� ���
					case 1:			//������ ����, ������ ���

						if (beforeOP) {							//���� : ���� ���� ���� �����ڿ��ٸ�(��ȣ ����) �����̴�.(�� : 10+*3)
							v.clear();
							return v;
						}

						if (beforeOpenParen) {					//���� : ���� ���� ���� ���°�ȣ��� �����̴�.(�� : 10(*3))
							v.clear();
							return v;
						}

						if (i == (infixLength - 1)) {			//���� : ������ ������ ���� �����ڶ�� �����̴�. (�� : 10-3-)
							v.clear();
							return v;
						}

						temp = stack.size();
						while (temp > 0) {
							char topOper = stack.top();			//stack�� top�� �ִ� �����ڸ� �����Ѵ�.
							int periorityOfTopOper = singleton.getInstance(op).op.getPriority(topOper);		//top�� �ִ� �������� �켱������ �����´�.

							temp--;

							if (topOper == '(') {				//���ÿ� �ִ� �����ڰ� ���� ��ȣ�̸� �����ڸ� push �Ѵ�.
								break;
							}

							if (periorityOfTopOper >= periorityOper) {	//���� ������ �켱���� >= ���� ������ �켱����
								stack.pop();					//���ÿ��� �����ڸ� ������.
								poppedOper = topOper;
								v.push_back(poppedOper);		//���ÿ��� ���� �����ڸ� �������Ŀ� �߰��Ѵ�.
								
							}
							else {			//���� �������� �켱���� < ���� �������� �켱����
								break;
							}
						}
						stack.push(oper);		//���� �����ڸ� ���ÿ� �߰��Ѵ�.
						beforeOP = 1;			//(+, -, *, /)�̱� ������ 1�� �����Ѵ�.
						beforeOpenParen = 0;	//���°�ȣ�� �ƴϱ� ������ 0�� �����Ѵ�.
						break;					//case 2, case 1 ����

					case 0:		//�����ڰ� ���� ��ȣ�� ���
						stack.push(oper);		//���ÿ� �����ڸ� �߰��Ѵ�.
						beforeOpenParen = 1;	//���°�ȣ�̱� ������ 1�� �����Ѵ�.
						beforeOP = 0;			//(+, -, *, /)�� �ƴϹǷ� 0�� �����Ѵ�.
						break;					//case 0 ����

					case -1:	//�����ڰ� �ݴ� ��ȣ�� ���
					
						if (beforeOP) {			//���� : ���� ���� ���� �����ڶ��(��ȣ ����) �����̴�.(�� : (1+10*)3)
							v.clear();
							return v;
						}

						temp = stack.size();
						while (temp > 0) {
							char topOper = stack.top();		//������ top �����ڸ� �����Ѵ�.

							if (topOper == '(') {			//���� ��ȣ ������ while�� �����Ѵ�.		
								stack.pop();				//���ÿ��� ���� ��ȣ�� ������.
								break;
							}
							else {							//���� ��ȣ ���������� ���ÿ��� �����ڸ� ������.
								stack.pop();				//���ÿ��� �����ڸ� ������.
								poppedOper = topOper;
								v.push_back(poppedOper);	//�����ڸ� �������Ŀ� �߰��Ѵ�.
								
							}
							temp--;
						}
						beforeOP = 0;				//(+, -, /, *)�� �ƴϹǷ� 0 ����
						beforeOpenParen = 0;		//���� ��ȣ�� �ƴϹǷ� 0 ����

						if (temp == 0) {			//���� : ���ÿ� ���� ��ȣ�� ���ٸ� �����̴�. (�� : 10*3)+3)
							v.clear();
							return v;
						}	
					default:
						break;	
					}
				}
			}
			else {			//���� : ������ ���� ������, �ǿ����ڰ� �ƴ� ��� �����̴�. v�� ��� �� ��ȯ�Ѵ�.
				v.clear();
				return v;
			}
		}

		while (!stack.empty()) {		//���ÿ� ���� �����ڸ� ��� ������.
			char topOper = stack.top();

			if (topOper == '(') {		//���� : ���°�ȣ�� ���ÿ� ���������� �����̴�.	(�� : 10(3+3 )
				v.clear();
				return v;
			}

			stack.pop();				//���ÿ��� �����ڸ� ������.
			char poppedOper = topOper;
			v.push_back(poppedOper);	//���� �����ڸ� �������Ŀ� �߰��Ѵ�.
			
		}
		return v;
	}

	//�������� ������ �����ϴ� �Լ�
	int calculate(vector<char> postfix) {
		
		stack<int> stack;
		int i = 0;
		int postfixlength = postfix.size();

		char component;
		int operationResult;

		while (i < postfixlength) {			//���� ������ ������ �ݺ��Ѵ�.
			
			component = postfix.at(i);		//���Ŀ� �ִ� ���� �����´�.
			i++;
			if (component == '*' || component == '/' || component == '+' || component == '-') {		//������ ���� �������� ���
				
				if (stack.size() == 1) {	//���� : �ǿ����ڰ� 1���ε� �����ڰ� �ִ� ���� �����̴�.
					return NULL;
				}

				int stackNum1 = stack.top();	//���ÿ��� �����ڸ� ������
				stack.pop();
				int stackNum2 = stack.top();	//���ÿ��� �����ڸ� ������.
				stack.pop();

				OperationContext* context;

				//�����ڿ� �´� ������ �����Ѵ�.
				if (component == '*') {
					context = new OperationContext(new OperationMultiply());
				}
				else if (component == '/') {
					context = new OperationContext(new OperationDivision());
					if (stackNum1 == 0) {		//���� : 0���� ������ ���� �����̴�.
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

				stack.push(result);		//�������� ���ÿ� �����Ѵ�.
			}
			else {		//������ ���� �ǿ������� ���
				int b = component;		
				stack.push(b);	//���ÿ� �ǿ����ڸ� �߰��Ѵ�.
			}
		}
		operationResult = stack.top();		//���ÿ� �ִ� �������� �����Ѵ�.
		stack.pop();

		if (!stack.empty()) {				//���� : ���ÿ� �ǿ����ڰ� ���������� �����̴�.
			return NULL;
		}

		return operationResult;
	}
};
//-------------------------------------------------------------------------------

void main() {
	
	//----��ü���� ����----
	//���� �Է¹ޱ�
	//������������ �����ϱ�
	//�������� ����ϱ�
	//��� ����ϱ�
	//---------------------

	try {

		string input;	//�Էµ� ������ �����Ѵ�.

		cout << "-----------------------[ ���� ���α׷� �Դϴ�. ]-----------------------" << endl;
		cout << "--------[����� ������ �ǿ����ڴ� ���� 0�̻� 127������ ���Դϴ�.]--------" << endl;
		cout << "[  ����� �����Ϸ��� [����], ���α׷��� �����Ϸ��� [����]�� �Է��ϼ���.  ]" << endl;
		cout << "> �Է� : ";
		getline(cin, input);

		while (input != "����") {		//���ᰡ �ԷµǱ� ������ ���α׷��� �����Ѵ�.
			if (input != "����") {
				cout << "[        <����> �߸��� �Է��Դϴ�. [����] Ȥ�� [����]�� �Է��ϼ���.      ]" << endl;			//���� : ����, ������ �Է����� ���� ���
			}
			else {
				cout << "[                           ������ �Է��ϼ���.                           ]" << endl;
				cout << "> �����Է� : ";
				getline(cin, input);
				if (input.empty()) {
					cout << "[         <����> ������ �Էµ��� �ʾҽ��ϴ�. ������ �Է��ϼ���.          ]" << endl;		//���� : ���� �Է��� ���� ���
				}
				else {
					//�Էµ� ���� ������ ���� ǥ������� ��ȯ�Ѵ�.
					MainController mc;
					vector<char> postfixVector;
					postfixVector = mc.convertInfixToPostfix(input, postfixVector);
					if (postfixVector.empty()) {
						cout << "[          <����> �߸��� �����Դϴ�. �ùٸ� ������ �Է��ϼ���.           ]" << endl;	//����ǥ��� ��ȯ ���������� �������
					}
					else {
						//��ȯ�� ���� ǥ����� ����Ѵ�.
						cout << "[                 �Է��� ���� ����ǥ������� ��ȯ�մϴ�.                 ]" << endl;
						cout << "> �������� : ";
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

						//������ ����Ѵ�.
						int result = mc.calculate(postfixVector);

						if (result == NULL) {
							cout << "[    <����> ��꿡 �������� ���� �����Դϴ�. �ùٸ� ������ �Է��ϼ���.   ]" << endl;	//���� ��� �������� �߻��� ������ ����Ѵ�.
						}
						else {
							//������� ����Ѵ�.
							cout << "[                     �Է��� ���� ����� ����մϴ�.                     ]" << endl;
							cout << "> ����� : " << result << endl;
							cout << "" << endl;
							cout << "" << endl;
						}
					}
				}
			}
			cout << "[  ����� �����Ϸ��� [����], ���α׷��� �����Ϸ��� [����]�� �Է��ϼ���.  ]" << endl;
			cout << "> �Է� : ";
			getline(cin, input);
		}
		cout << "------------------------[ ���α׷��� �����մϴ�. ]-----------------------" << endl;
	}
	catch (...) {
		cout << "[              ���ܰ� �߻��Ͽ����ϴ�. ���α׷��� �����մϴ�.             ]" << endl;
	}

	return;
}









