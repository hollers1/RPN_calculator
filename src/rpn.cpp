#include "rpn.h"
#include <stdexcept>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <cmath>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>

// Вспомогательная функция для разбиения строки на токены
std::vector<std::string> tokenize(const std::string& expression) {
    std::vector<std::string> tokens;
    std::istringstream iss(expression);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

// Вспомогательная функция для проверки, является ли строка числом
bool is_number(const std::string& s) {
    if (s.empty()) return false;
    
    size_t start = 0;
    // Проверяем знак числа
    if (s[0] == '-' || s[0] == '+') {
        if (s.size() == 1) return false; // Только знак без цифр
        start = 1;
    }
    
    bool has_dot = false;
    for (size_t i = start; i < s.size(); ++i) {
        if (s[i] == '.') {
            if (has_dot) return false; // Две точки в числе
            has_dot = true;
        } else if (!std::isdigit(s[i])) {
            return false;
        }
    }
    return true;
}

//Вспомогательная функция, определяющая необходимое количество операндов
int cnt_of_operands(const std::string& oper) {
	if (oper == "dup" ||
		oper == "swap" ||
		oper == "drop" ||
		oper == "clear" ||
		oper == "stack" ||
		oper == "pi" ||
		oper == "e" ||
		oper == "rand" ||
		oper == "vars")
		return 0;
    else if (oper == "~" ||
        oper == "++" ||
        oper == "--" ||
        oper == "!" ||
		oper == "sqrt" ||
		oper == "abs" ||
		oper == "sin" ||
		oper == "cos" ||
		oper == "tan" ||
		oper == "asin" ||
		oper == "acos" ||
		oper == "atan" ||
		oper == "ln" ||
		oper == "lg" ||
		oper == "floor" ||
		oper == "ceil" ||
		oper == "round")
        return 1;
    else if (oper == "+" ||
        oper == "-" ||
        oper == "*" ||
        oper == "/" ||
		oper == "^" ||
		oper == "%" ||
		oper == "log" ||
		oper == "min" ||
		oper == "max" ||
		oper == "==" ||
		oper == "!=" ||
		oper == "<" ||
		oper == ">" ||
		oper == "<=" ||
		oper == ">=")
        return 2;
    else {
		return -10;
	}
}

double evaluate_rpn(const std::string& expression) {
    std::stack<double> stack;
    std::vector<std::string> tokens = tokenize(expression);
    
    for (const auto& token : tokens) {
        if (is_number(token)) {
            // Преобразуем строку в число и помещаем в стек
            stack.push(std::stod(token));
        } else {
            int cntOfOperands = cnt_of_operands(token);
            
            // Обрабатываем оператор
            if (stack.size() < abs(cntOfOperands)) {
                throw std::invalid_argument("Not enough operands for operator '" + token + "'");
            }

            double result, a, b;
			bool flag = true;
			std::string var;
			switch (cntOfOperands)
			{
			case 2:
            	b = stack.top(); stack.pop();
			case 1:
				a = stack.top(); stack.pop();
				break;
			case -2:
				a = stack.top(); stack.pop();
			case -1:
				var = stack.top(); stack.pop();
			}

			//Команды
			if (token == "dup") { //Дублирование вершины
				result = stack.top();
			} else if (token == "swap") { //Обмен элементов
				double t = stack.top(); stack.pop();
				result = stack.top(); stack.pop();
				stack.push(t);
			} else if (token == "drop") { //Удаление элемента
				stack.pop();
				flag = false;
			} else if (token == "clear") { //Очистка стека
				while (!stack.empty()) {
					stack.pop();
				}
				flag = false;
			} else if (token == "stack") { //Вывод стека
				std::stack<double> sec;
				while (!stack.empty()) {
					sec.push(stack.top()); stack.pop();
				}
				while (!sec.empty()) {
					stack.push(sec.top()); sec.pop();
					std::cout << stack.top() << " ";
				}
				std::cout << std::endl;
			} else if (token == "pi") { //Число пи
				result = M_PI;
			} else if (token == "e") { //Число Эйлера
				result = M_E;
			} else if (token == "rand") { //Случайное число от 0 до 1
				srand(time(NULL));
				result = rand() / double(RAND_MAX);
			} else
			//Один операнд
        	if (token == "~") { //Унарный минус
                result = -a;
            } else if (token == "++") { //Инкремент
                result = ++a;
            } else if (token == "--") { //Декремент
                result = --a;
            } else if (token == "!") { //Факториал
                if (a < 0.0) {
                    throw std::invalid_argument("Factorial of zero");
                }
                for (result = 1; a > 0; --a) {
                    result *= a;
                }
            } else if (token == "sqrt") { //Квадратный корень
				result = sqrt(a);
			} else if (token == "abs") { //Модуль
				result = abs(a);
			} else if (token == "sin") { //Синус
				result = sin(a);
			} else if (token == "cos") { //Косинус
				result = cos(a);
			} else if (token == "tan") { //Тангенс
				result = tan(a);
			} else if (token == "asin") { //Арксинус
				if (abs(a) > 1){
					throw std::invalid_argument("There is no arcsinus");
				}
				result = asin(a);
			} else if (token == "acos") { //Арккосинус
				if (abs(a) > 1){
					throw std::invalid_argument("There is no arccosine");
				}
				result = acos(a);
			} else if (token == "atan") { //Арктангенс
				result = atan(a);
			} else if (token == "ln") { //Натуральный логарифм
				if (a <= 0){
					throw std::invalid_argument("There is no logarithm");
				}
				result = log(a);
			} else if (token == "lg") { //Десятичный логарифм
				if (a <= 0){
					throw std::invalid_argument("There is no logarithm");
				}
				result = log10(a);
			} else if (token == "floor") { //Округление вниз
				result = floor(a);
			} else if (token == "ceil") { //Округление вверх
				result = ceil(a);
			} else if (token == "round") { //Округление по математическим правилам
				result = round(a);
			} else
			//Два операнда
			if (token == "+") { //Сложение
                result = a + b;
            } else if (token == "-") { //Вычитание
                result = a - b;
            } else if (token == "*") { //Умножение
                result = a * b;
            } else if (token == "/") { //Деление
                if (b == 0.0) {
                    throw std::invalid_argument("Division by zero");
                }
                result = a / b;
            } else if (token == "^") { //Возведение в степень
                for (result = 1; b > 0; --b) {
                    result *= a;
                }
            } else if (token == "%") { //Остаток от деления
                if (double(int(a)) != a || double(int(b)) != b) {
                    throw std::invalid_argument("The remainder of a non-integer division");
                }
				int ai = int(a), bi = int(b);
                result = (ai % bi + bi) % bi;
            } else if (token == "log") { //Логарифм
				if (a <= 0 || a == 1 || b <= 0){
					throw std::invalid_argument("There is no logarithm");
				}
                result = log(b) / log(a);
            } else if (token == "min") { //Минимум
                result = std::min(a, b);
            } else if (token == "max") { //Максимум
                result = std::max(a, b);
            } else
			//Сравнение
			if (token == "==") {
                result = double(a == b);
            } else if (token == "!=") {
                result = double(a != b);
            } else if (token == "<") {
                result = double(a < b);
            } else if (token == ">") {
                result = double(a > b);
            } else if (token == "<=") {
                result = double(a <= b);
            } else if (token == ">=") {
                result = double(a >= b);
            } else { //Неизвестный оператор
				throw std::invalid_argument("Invalid operator: '" + token + "'");
			}
            
			if (flag) {
            	stack.push(result);
			}
        }
    }
    
    if (stack.size() != 1) {
        throw std::invalid_argument("Invalid expression: too many operands");
    }
    
    return stack.top();
}
