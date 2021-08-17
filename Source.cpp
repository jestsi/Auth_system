#include <iostream>
#include <fstream>
#include <string>
#include "sha3.h"

void login_or_register_system(void (*start_on_enter)() = nullptr) {
	std::fstream file; // ������ � ������ � ����
	std::string entered_user_name;		// �������� �����
	std::string entered_user_password;	// �������� ������
	uint8_t selected;					// ����� ����� ��� �����������������

	enum class Action { REGISTER = '1', LOGIN = '2' }; 

	std::cout << "1 - Register." << std::endl <<
				 "2 - Login."    << std::endl <<
				 "->";

	std::cin >> selected;				// ���� ������ ���. ��� ����
	
	if (selected == char(Action::LOGIN)) {

		std::cout << "Enter your login: "; std::cin >> entered_user_name;		// ��������� ������
		std::cout << "Enter your password: "; std::cin >> entered_user_password;// ��������� ������

		std::string from_file_user_name;		// ������ �� ����� (�����)
		std::string from_file_user_password;	// ������ �� ����� (������)

		std::string buf;	// ������

		int pos_p;		// ������� ����������� ������ � ������ 
		pos_p = 0;
		bool login_success = false;		// ������ �����
		try
		{
			file.open("Users.cache", std::ios::app | std::ios::in);
			while (!file.eof()) {		// ���� ������ �� � ����� �����
				from_file_user_name = "";		// ������� �� ������� ������
				from_file_user_password = "";	// ������� �� ������� ������

				buf = "";						// ������� �� ������� ������

				pos_p = 0;						// ������� �� ������� ������

				std::getline(file, buf, '\n');		// ��������� ������

				for (size_t i = 0; i < buf.size(); i++) {	// ��������� �����
					if (buf[i] != ']')
						from_file_user_name += buf[i];
					else {
						pos_p = ++i;
						break;
					}
				}

				for (size_t i = pos_p; i < buf.size(); i++)		// ��������� ������
				{
					if (buf[i] != '\0')
						from_file_user_password += buf[i];
					else break;
				}

				SHA3 sha3_512(SHA3::Bits512);		// ������������ ������ SHA3

				if (from_file_user_name == sha3_512(entered_user_name) && // ����������� ������
					from_file_user_password == sha3_512(entered_user_password)) {
					std::cout << "You enter!" << std::endl;
					login_success = true;		// ��������� ������� �����
					break;						// ����� �� ����� while
				}
			}
		}
		catch (const std::exception& exp)
		{
			std::cerr << "Error - " << exp.what() << std::endl;
		}
		

		if (!login_success) {		// ���� ���� �� ��������
			std::cout << "The password or login is incorrect :( try again." << std::endl;
			file.seekg(0);
		}
		else
		{
			if (start_on_enter != nullptr) // ���������� ������� ����� �����
				start_on_enter();
		}

	}
	else if (selected == char(Action::REGISTER)) {
		std::cout << "Enter your name: "; std::cin >> entered_user_name; // �������� �����

		std::cout << "Enter password: ";  std::cin >> entered_user_password; // �������� ������

		std::cout << std::endl <<
			"Ok, your account create.." <<
			std::endl;

		if (entered_user_name.size() >= 2 && entered_user_password.size() > 6) { // ������� �������� ��������
			try
			{
				SHA3 sha3_512(SHA3::Bits512); // ������������ ������ SHA3
				
				file.open("Users.cache", std::ios::app | std::ios::in);
				file << sha3_512(entered_user_name) << ']' << sha3_512(entered_user_password) << '\0' << '\n'; // ������ ������ � ����
				std::cout << "Success you account created!" << std::endl;

				if (start_on_enter != nullptr) // ������� ����� ����� 
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