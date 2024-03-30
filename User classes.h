#include "bits/stdc++.h"
#include "Card classes.h"
using namespace std;
class User {
private:
	string username;
	string password;
	string phone_number;
public:
	User() {}
	User(const string& username, const string& password, const string& phone_number) :username(username),password(password),phone_number(phone_number) {}
	const string& get_username()const {
		return this->username;
	}
	const string& get_password()const {
		return this->password;
	}
	const string& get_phone_number()const {
		return this->phone_number;
	}

	const void set_username(const string &new_username) {
		this->username = new_username;
	}
	const void set_password(const string& new_password) {
		this->password = new_password;
	}
	const void set_phone_number(const string& new_phone_number) {
		this->phone_number = new_phone_number;
	}

};
class Account {
private:
	User user;
	string def_card_number;
	string ipa;
	vector<string>visa_cards_numbers;
	vector<string>master_cards_numbers;
	set<string>cards;
public:
	Account() {};
	Account(const User& user, const string& def_card_number, const string& ipa
		, const vector<string>& visa_cards_numbers, const vector<string>& master_cards_numbers) :
		user(user), def_card_number(def_card_number), ipa(ipa),
		visa_cards_numbers(visa_cards_numbers), master_cards_numbers(master_cards_numbers) {
		for (auto& it : visa_cards_numbers)cards.insert(it);
		for (auto& it : master_cards_numbers)cards.insert(it);
	}

	const User& get_user() {
		return this->user;
	}
	 string& get_def_card_number() {
		return this->def_card_number;
	}
	const string& get_ipa() {
		return this->ipa;
	}
	const vector<string>& get_visa_cards_numbers() {
		return this->visa_cards_numbers;
	}
	const vector<string>& get_master_cards_numbers() {
		return this->master_cards_numbers;
	}
	void show_my_cards()const {
		if (visa_cards_numbers.empty()) {
			cout << "\nYou have no Visa Cards\n";
		}
		else {
			cout << "\nYou have " << visa_cards_numbers.size() << " Visa Cards\n";
			for (int g = 0; g < visa_cards_numbers.size(); g++) {
				cout << g + 1 << " " << visa_cards_numbers[g] << "\n";
			}
		}
		cout << "\n";
		if (master_cards_numbers.empty()) {
			cout << "\nYou have no Master Cards\n";
		}
		else {
			cout << "\nYou have " << master_cards_numbers.size() << " Master Cards\n";
			for (int g = 0; g < master_cards_numbers.size(); g++) {
				cout << g + 1 << " " << master_cards_numbers[g] << "\n";
			}
		}
		cout << "\n";

		cout << "And Your default Card is : " << def_card_number << "\n";
	}
	void show_specific_card(Card_manager& card_manager) {
		string card_number; 
		cout << "\nEnter Card Number: "; cin >> card_number;
		if (!cards.count(card_number)) {
			cout << "\nYou dont have this Card\n";
			return;
		}
		card_manager.show_information(card_number);
	}
	void show_transactions(Card_manager &card_manager) {// for one card
		string card_number;
		cout << "\nEnter Card Number: "; cin >> card_number;
		if (!cards.count(card_number)) {
			cout << "\nYou dont have this Card\n";
			return;
		}
		bool flag = 0;
		for (auto& it : card_manager.get_transactions(card_number)) {
			flag = 1;
			cout << "\nFrom: " << it.from << "\t" << "To: " << it.to << "\t" << "Amount of Money Transferred: " << it.movable_money;
		}
		if (!flag) {
			cout << "\n You Have no Transactions\n";
		}
	}
	void add_card(Card_manager& card_manager) {
		string card_number, card_cvv, pin;
		cout << "Enter Card Number: "; cin >> card_number;
		cout << "Enter Card CVV: "; cin >> card_cvv;
		cout << "Enter Card PIN: "; cin >> pin;
		if (card_manager.verify_card(card_number, card_cvv, pin)) {
			cards.insert(card_number);
			if (card_manager.is_master(card_number)) {
				master_cards_numbers.push_back(card_number);
			}
			else {
				visa_cards_numbers.push_back(card_number);
			}
			cout << "\n Card Added Successfully\n";
		}
	}
	void remove_card() {
		if (cards.size() == 1) {
			cout << "\nYou only have 1 Card , you cant delete it\n";
			return;
		}
		string card_number;
		cout << "\nEnter Card Number: "; cin >> card_number;
		if (!cards.count(card_number)) {
			cout << "\nYou dont have this Card\n";
			return;
		}
		if (card_number == get_def_card_number()) {
			cout << "\n You cant delete your Default Card\n";
			return;
		}
		int i = -1;
		for (int g = 0; g < visa_cards_numbers.size(); g++) {
			if (visa_cards_numbers[g] == card_number) {
				i = g;
				break;
			}
		}
	
		if (i != -1) {
			visa_cards_numbers.erase(visa_cards_numbers.begin() + i);
		}
		else {
			for (int g = 0; g < master_cards_numbers.size(); g++) {
				if (master_cards_numbers[g] == card_number) {
					i = g;
					break;
				}
			}
			master_cards_numbers.erase(master_cards_numbers.begin() + i);
		}
		cards.erase(card_number);
		cout << "\nCard has been deleted\n";
	}
	void change_def() {
		string card_number;
		cout << "\nEnter Card Number: "; cin >> card_number;
		if (!cards.count(card_number)) {
			cout << "\nYou dont have this Card\n";
			return;
		}
		def_card_number = card_number;
		cout << "\nYour current default Card is: " << get_def_card_number() << "\n";
	}
	void send_money(Card_manager& card_manager, map<string, Account>& account_database, map<string, string>& ipa_to_username, map<string, string>& phone_to_username) {
		cout << "\nYou want to send to IPA or Phone Number ? Enter 1 or 2";
		int i = read_int(1, 2);
		if (i == 1) {
			string sec_ipa;
			cout << "\nEnter IPA: "; cin >> sec_ipa;
			if (!ipa_to_username.count(sec_ipa)) {
				cout << "\nNo such IPA, try again\n";
				send_money(card_manager, account_database, ipa_to_username, phone_to_username);
				return;
			}
            if(sec_ipa==this->get_ipa()){
                cout << "\nYou can't send to Yourself\n";
                return ;
            }
			double cost;
			cout << "Enter amount of money you want to transfer: "; cin >> cost;
			Card*& fir_card = card_manager.get_card(def_card_number);
			string temp_user_name = ipa_to_username[sec_ipa];
			string sec_card_number = account_database[temp_user_name].get_def_card_number();
			Card*& sec_card = card_manager.get_card(sec_card_number);

			if (fir_card->send(sec_card, cost)) {
				cout << "Money sent successfully\n";
				transaction op1 = { def_card_number,sec_ipa,cost };
				transaction op2 = { this->get_ipa(),sec_card_number,cost };
				card_manager.add_transaction1(op1);
				card_manager.add_transaction2(op2);
			}
		}
		else {
			string sec_phone;
			cout << "\n Enter Phone Number: "; cin >> sec_phone;
			if (!phone_to_username.count(sec_phone)) {
				cout << "\nNo such Phone Number, try again\n";
				send_money(card_manager, account_database, ipa_to_username, phone_to_username);
				return;
			}
             if(sec_phone==this->get_user().get_phone_number()){
                cout << "\nYou can't send to Yourself\n";
                return ;
            }
			double cost;
			cout << "Enter amount of money you want to transfer: "; cin >> cost;
			Card*& fir_card = card_manager.get_card(def_card_number);
			string temp_user_name = phone_to_username[sec_phone];
			string sec_card_number = account_database[temp_user_name].get_def_card_number();
			Card*& sec_card = card_manager.get_card(sec_card_number);

			if (fir_card->send(sec_card, cost)) {
				cout << "Money sent successfully\n";
				transaction op1 = { def_card_number,sec_phone,cost };
				transaction op2 = { this->get_ipa(),sec_card_number,cost };
				card_manager.add_transaction1(op1);
				card_manager.add_transaction2(op2);
			}

		}

	}

	void Et3amel(Card_manager& card_manager, map<string, Account>& account_database, map<string, string>& ipa_to_username, map<string, string>& phone_to_username) {
		vector<string>menu;
		menu.push_back("Show My Cards");
		menu.push_back("Show Sepcific Card details");
		menu.push_back("Show Transactions for a Card");
		menu.push_back("Add Card to Account");
		menu.push_back("Remove Card from Account");
		menu.push_back("Change My Default Card");
		menu.push_back("Send Money");
		menu.push_back("Logout");
		while (1) {
			int i = show_menu(menu);
			if (i == 1) {
				show_my_cards();
			}
			else if (i == 2) {
				show_specific_card(card_manager);
			}
			else if (i == 3) {
				show_transactions(card_manager);
			}
			else if (i == 4) {
				add_card(card_manager);
			}
			else if (i == 5) {
				remove_card();
			}
			else if (i == 6) {
				change_def();
			}
			else if (i == 7) {
				send_money(card_manager, account_database, ipa_to_username, phone_to_username);
			}
			else
				break;
		}
		cout << "\n\tI Hate Goodbyes.\n\n";
	}
};