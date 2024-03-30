#include "bits/stdc++.h"
#include "User classes.h"
using namespace std;
class System {
private:
	Card_manager card_manager;
	map<string, Account>account_database;// usernames
	map<string, string>ipa_to_username;
	map<string, string>phone_to_username;
	string current_account;
	void load_database() {
		fstream file("accounts data.txt");
		if (!file.is_open()) {
			cout << "Cannot open file: " << "visa cards data";
		}
		else {
			while (1) {
				string temp;
				string username;
				string pass;
				string phone;
				string def_card_number;
				string ipa;
				string sz1;
				vector<string>v1;
				string sz2;
				vector<string>v2;
				getline(file, temp);
				if (file.fail())break;
				getline(file, username);
				getline(file, pass);
				getline(file, phone);
				getline(file, def_card_number);
				getline(file, ipa);
				getline(file, sz1);
				v1 = vector<string>(stoi(sz1));
				for (auto& it : v1)
					getline(file, it);

				getline(file, sz2);
				v2 = vector<string>(stoi(sz2));
				for (auto& it : v2)
					getline(file, it);

				account_database[username] = Account(User(username, pass, phone), def_card_number, ipa, v1, v2);
				ipa_to_username[ipa] = username;
				phone_to_username[phone] = username;
			}

		}
	}
public:
	System() {
		load_database();
	}
	void modify_bank_cards() {
		vector<string>menu;
		menu.push_back("Add new BankCard to System");
		menu.push_back("Edit Existing card\n");
		int i = show_menu(menu);
		if (i == 1) {
			card_manager.add_card_to_database();
		}
		else {
			card_manager.edit_card();
		}
	}
	const string enter_initial_card() {
		string card_number, card_cvv, pin;
		cout << "Enter Card Number: "; cin >> card_number;
		cout << "Enter Card CVV: "; cin >> card_cvv;
		cout << "Enter Card PIN: "; cin >> pin;
		if (card_manager.verify_card(card_number, card_cvv, pin)) {
			cout << "\n Card Added Successfully\n";
			return card_number;
		}
		cout << "\nTry again.\n";
		return enter_initial_card();
	}
	void login() {
		string username, password;
		cout << "\nEnter Username: "; cin >> username;
		cout << "Enter Password: "; cin >> password;
		if (!account_database.count(username)) {
			cout << "\nThe Username You Entered is Incorrect\n";
			login();
			return;
		}
		if (account_database[username].get_user().get_password() != password) {
			cout << "\nThe Password You Entered is Incorrect\n";
			login();
			return;
		}
		current_account = username;
	}
	void sign_up() {
		User user;
		string s;
		cout << "\nEnter UserName: "; cin >> s;
		if (account_database.count(s)) {
			cout << "Already used ,try again\n";
			sign_up();
			return;
		}
		user.set_username(s);
		cout << "Enter Password: "; cin >> s;
		user.set_password(s);
		while (1) {
			cout << "Enter Phone Number: "; cin >> s;
			if (s.size() != 11) {
				cout << "Invalid Phone Number (should be 11 digit) , try again\n";
			}
			else if (phone_to_username.count(s)) {
				cout << "Used phone number ,try again\n";
			}
			else
				break;
		}
		user.set_phone_number(s);
		cout << "Enter Inital Card For Your Account\n";
		string card_number = enter_initial_card();
		cout << "\nEnter IPA for Account so others can use it while transactions :";
		string ipa;
		while (1) {
			cin >> ipa;
			if (ipa_to_username.count(ipa)) {
				cout << "Used IPA ,try again\n";
			}
			else
				break;
		}
		if (card_manager.is_master(card_number)) {
			account_database[user.get_username()] = Account(user, card_number, ipa, {}, { card_number });
		}
		else {
			account_database[user.get_username()] = Account(user, card_number, ipa, { card_number }, {});
		}
		ipa_to_username[ipa] = user.get_username();
		phone_to_username[user.get_phone_number()] = user.get_username();
		current_account = user.get_username();
	}
	void access_system() {
		while (1) {
			int choice = show_menu({ "Login", "Sign Up" });
			if (choice == 1)
				login();
			else
				sign_up();
			account_database[current_account].Et3amel(card_manager, account_database, ipa_to_username, phone_to_username);
		}
	}
	void start() {
		cout << "Do You to modify Bank Cards ? Eneter Y/N : ";
		char c; cin >> c;
		if (c == 'Y') {
			modify_bank_cards();
		}
		access_system();
	}
	


};



int main() {
	System acc;
	acc.start();
	
}
