#include <iostream>
#include <fstream>
#include <string>
#include "sha3.h"

void login_or_register_system(void (*start_on_enter)() = nullptr) {
	std::fstream file; // чтение и запись в файл
	std::string entered_user_name;		// введённый логин
	std::string entered_user_password;	// введённый пароль
	uint8_t selected;					// выбор войти или зарегестрироватся

	enum class Action { REGISTER = '1', LOGIN = '2' }; 

	std::cout << "1 - Register." << std::endl <<
				 "2 - Login."    << std::endl <<
				 "->";

	std::cin >> selected;				// ввод выбора рег. или вход
	
	if (selected == char(Action::LOGIN)) {

		std::cout << "Enter your login: "; std::cin >> entered_user_name;		// получение логина
		std::cout << "Enter your password: "; std::cin >> entered_user_password;// получение пароля

		std::string from_file_user_name;		// данные из файла (логин)
		std::string from_file_user_password;	// данные из файла (пароль)

		std::string buf;	// буффер

		int pos_p;		// позиция разделителя логина и пароля 
		pos_p = 0;
		bool login_success = false;		// статус входа
		try
		{
			file.open("Users.cache", std::ios::app | std::ios::in);
			while (!file.eof()) {		// пока курсор не в конце файла
				from_file_user_name = "";		// очистка от прошлых данных
				from_file_user_password = "";	// очистка от прошлых данных

				buf = "";						// очистка от прошлых данных

				pos_p = 0;						// очистка от прошлых данных

				std::getline(file, buf, '\n');		// получение строки

				for (size_t i = 0; i < buf.size(); i++) {	// получение имени
					if (buf[i] != ']')
						from_file_user_name += buf[i];
					else {
						pos_p = ++i;
						break;
					}
				}

				for (size_t i = pos_p; i < buf.size(); i++)		// получение пароля
				{
					if (buf[i] != '\0')
						from_file_user_password += buf[i];
					else break;
				}

				SHA3 sha3_512(SHA3::Bits512);		// иницилизация класса SHA3

				if (from_file_user_name == sha3_512(entered_user_name) && // сравнивание данных
					from_file_user_password == sha3_512(entered_user_password)) {
					std::cout << "You enter!" << std::endl;
					login_success = true;		// изменение статуса входа
					break;						// выход из цикла while
				}
			}
		}
		catch (const std::exception& exp)
		{
			std::cerr << "Error - " << exp.what() << std::endl;
		}
		

		if (!login_success) {		// если вход не выполнен
			std::cout << "The password or login is incorrect :( try again." << std::endl;
			file.seekg(0);
		}
		else
		{
			if (start_on_enter != nullptr) // выполнение функции после входа
				start_on_enter();
		}

	}
	else if (selected == char(Action::REGISTER)) {
		std::cout << "Enter your name: "; std::cin >> entered_user_name; // введённый логин

		std::cout << "Enter password: ";  std::cin >> entered_user_password; // введённый пароль

		std::cout << std::endl <<
			"Ok, your account create.." <<
			std::endl;

		if (entered_user_name.size() >= 2 && entered_user_password.size() > 6) { // условия создания аккаунта
			try
			{
				SHA3 sha3_512(SHA3::Bits512); // иницилизация класса SHA3
				
				file.open("Users.cache", std::ios::app | std::ios::in);
				file << sha3_512(entered_user_name) << ']' << sha3_512(entered_user_password) << '\0' << '\n'; // запись данных в файл
				std::cout << "Success you account created!" << std::endl;

				if (start_on_enter != nullptr) // функция после входа 
					start_on_enter();
			}
			catch (const std::exception& exp)
			{
				std::cerr << "The account was not created, an error :(" << std::endl <<
					"Error - " << exp.what() << std::endl;
			}
		}
	}

	file.close();
}

auto main(int argc, char** argv) -> int {
	argc = NULL;
	argv = nullptr;

	login_or_register_system();

	return 0;
}