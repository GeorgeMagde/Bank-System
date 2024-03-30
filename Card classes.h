#include "bits/stdc++.h"
using namespace std;
int read_int(int low, int high) {
	cout << "\nEnter number in range " << low << " - " << high << ": ";
	int value;
	cin >> value;
	if (low <= value && value <= high)
		return value;

	cout << "ERROR: invalid number...Try again\n";
	return read_int(low, high);
}
int show_menu(const vector<string>& choices) {
	cout << "\nMenu:\n";
	for (int ch = 0; ch < (int)choices.size(); ++ch) {
		cout << "\t" << ch + 1 << ": " << choices[ch] << "\n";
	}
	return read_int(1, choices.size());
}
class Card {
private:
	string card_number;// unique for every card
	string card_cvv; // 3 digit 
	string holder_name;
	string phone_number;
	string pin;// pass , 6 digit
	double my_money;
public:
	Card() {}
	Card(const string& card_number, const string& card_cvv, const string& holder_name, 
		const string& phone_number, const string& pin, const double& my_money = 0) :
		card_number(card_number), card_cvv(card_cvv), holder_name(holder_name), phone_number(phone_number), pin(pin), my_money(my_money) {
	}

	const string& get_card_numbner() const {
		return this->card_number;
	}
	const string& get_card_cvv()const {
		return this->card_cvv;
	}
	const string& get_holder_name()const {
		return this->holder_name;
	}
	const string& get_phone_number()const {
		return this->phone_number;
	}
	const string& get_pin()const {
		return this->pin;
	}
	const double& get_my_money()const {
		return this->my_money;
	}


	const void set_holder_name(const string& new_holder_name) {
		this->holder_name = new_holder_name;
	}
	const void set_phone_number(const string& new_phone_number) {
		this->phone_number = new_phone_number;
	}
	const void set_pin(const string& new_pin) {
		this->pin = pin;
	}

	void operator += (const double& money_added) {
		this->my_money += money_added;
	}
	void operator -= (const double& money_spent) {
		this->my_money -= money_spent;
	}

	virtual bool send(Card*& second_card, const double& cost) = 0;
	virtual bool can_send(const double &cost) = 0;
	virtual void show_informations() = 0;

};
class Visa_card :public Card {
public:
	Visa_card() {}
	Visa_card(const string& card_number, const string& card_cvv, const string& holder_name,
		const string& phone_number, const string& pin, const double& my_money = 0) :
		Card(card_number, card_cvv, holder_name, phone_number, pin, my_money) {
	}

	bool send(Card*& second_card, const double& cost) override {
		if (!can_send(cost)) {
			cout << "\nNot Enough Money to send this amount\n";
			return 0;
		}
		*this -= cost;
		*second_card += cost;
		return 1;
	}
    bool can_send(const double& cost) override {
		return cost <= this->get_my_money();
	}
	void show_informations() override {
		cout << "\nCard Number: " << get_card_numbner() << "\n";
		cout << "Cardholder Name: " << get_holder_name() << "\n";
		cout << "Available Money: $" << get_my_money() << "\n";
	}

};
class Master_card :public Card {
private:
	double extra_fee;
public:
	Master_card() {}
	Master_card(const string& card_number, const string& card_cvv, const string& holder_name, 
		const string& phone_number, const string& pin, const double& money = 0, const double& extra_fee = 0.0) :
		Card(card_number, card_cvv, holder_name, phone_number, pin, money), extra_fee(extra_fee) {};

     bool send(Card*& second_card, const double& cost) override {
		double total_cost = cost + cost * extra_fee;
		if (!can_send(total_cost)) {
			cout << "\nNot Enough Money to send this amount\n";
			return 0;
		}
		*this -= total_cost;
		*second_card += cost;
		return 1;
	}
	bool can_send(const double& total_cost) override {
		return total_cost <= this->get_my_money();
	}
	void show_informations() override {
		cout << "\nCard Number: " << get_card_numbner() << "\n";
		cout << "Cardholder Name: " << get_holder_name() << "\n";
		cout << "Available Money: " << get_my_money() << "\n";
		cout << "Extra fee is: $" << extra_fee << " Percent of Amount to be Transmitted" << "\n";
	}
};
struct transaction{
	string from;
	string to;
	double movable_money;
};
class Card_manager {
private:
	map<string, Card*>card_database;
	map<string, bool>visa_or_master;// 0 for visa , 1 for master
	map<string, vector<transaction>>transactions;
	void free_database() {
		for (auto& pair : card_database) {
			delete pair.second;
		}
		card_database.clear();
	}
	void load_database() {
		ifstream file("visa cards data.txt");
		if (!file.is_open()) {
			cout << "Cannot open file: " << "visa cards data";
		}
		else {
			while (1) {
				string temp;
				string card_number;
				string card_cvv;
				string holder_name;
				string phone_number;
				string pin;
				string tmp_money;
				getline(file, temp);
				if (file.fail())break;
				getline(file, card_number);
				getline(file, card_cvv);
				getline(file, holder_name);
				getline(file, phone_number);
				getline(file, pin);
				getline(file, tmp_money);
				double my_money = stod(tmp_money);
				card_database[card_number] = new Visa_card(card_number, card_cvv, holder_name, phone_number, pin, my_money);
				visa_or_master[card_number] = 0;
			}
		}
		file.close();
		file.open("master cards data.txt");
		if (!file.is_open()) {
			cout << "Cannot open file: " << "visa cards data";
		}
		else {
			while (1) {
				string temp;
				string card_number;
				string card_cvv;
				string holder_name;
				string phone_number;
				string pin;
				string tmp_money;
				string tmp_fee;
				getline(file, temp);
				if (file.fail())break;
				getline(file, card_number);
				getline(file, card_cvv);
				getline(file, holder_name);
				getline(file, phone_number);
				getline(file, pin);
				getline(file, tmp_money);
				getline(file, tmp_fee);
				double my_money = stod(tmp_money);
				double extra_fee = stod(tmp_fee);
				card_database[card_number] = new Master_card(card_number, card_cvv, holder_name, phone_number, pin, my_money,extra_fee);
				visa_or_master[card_number] = 1;
			}

		  /*  for (auto& it : card_database) {
				it.second->show_informations();
			}*/
		}

	
	}
public:
	Card_manager() { load_database(); }
	~Card_manager() { free_database(); }

	 Card*& get_card(const string& card_number) {
		return card_database[card_number];
	}
	string read_card_number() {
		string card_number;
		cout << "Enter Card Number (16 digit no spaces) : "; cin >> card_number;
		if (card_number.size() != 16) {
			cout << "\n Invalid number, try again\n";
			return read_card_number();
		}
		if (card_database.count(card_number)) {
			cout << "\nAlready added\n";
			return "-1";
		}
		return card_number;
	}
	string read_card_cvv() {
		string card_cvv;
		cout << "Enter Card CVV (3 digit no spaces) : "; cin >> card_cvv;
		if (card_cvv.size() != 3) {
			cout << "\n Invalid number, try again\n";
			return read_card_cvv();
		}
		return card_cvv;
	}
	void read_attached_data(string& holder_name, string& phone_number, string& pin, double& my_money) {
		cout << "Enter Cardholder Name :"; cin >> holder_name;
		cout << "Enter Phone Number :"; cin >> phone_number;
		cout << "Enter PIN (6 digits) :"; cin >> pin;
		cout << "Add Initial Amount of Money or just Enter Zero :"; cin >> my_money;
	}
	void add_card_to_database() {
		cout << "\nChoose type of card\n";
		cout << "1: Visa card \n";
		cout << "2: Master card \n";
		int type = read_int(1, 2);
		string card_number = read_card_number();
		if (card_number == "-1")return;
		string card_cvv = read_card_cvv();
		string holder_name;
		string phone_number;
		string pin;
		double my_money;
		read_attached_data(holder_name, phone_number, pin, my_money);
		if (type == 1) {
			card_database[card_number] = new Visa_card(card_number, card_cvv, holder_name, phone_number, pin, my_money);
			visa_or_master[card_number] = 0;
		}
		else {
			double extra_fee = 0.0;
			cout << "Enter Extra Fee Value: "; cin >> extra_fee;
			card_database[card_number] = new Master_card(card_number, card_cvv, holder_name, phone_number, pin, my_money, extra_fee);
			visa_or_master[card_number] = 0;
		}
	}
	void edit_cardholder(const string& card_number) {
		cout << "\nEnter New Cardholder name :";
		string holder_name; cin >> holder_name;
		card_database[card_number]->set_holder_name(holder_name);
	}
	void edit_phone_number(const string& card_number) {
		cout << "\nEnter New Phone Number :";
		string phone_number; cin >> phone_number;
		card_database[card_number]->set_phone_number(phone_number);
	}
	void edit_pin(const string& card_number) {
		cout << "\nEnter New PIN :";
		string pin; cin >> pin;
		card_database[card_number]->set_pin(pin);
	}
	void edit_card() {
		cout << "\nEnert Card Number to be edited : ";
		string card_number; cin >> card_number;
		if (!card_database.count(card_number)) {
			cout << "\nNo such Card\n";
			return;
		}
		cout << "\n";
		vector<string>menu;
		menu.push_back("Edit Cardholder Name");
		menu.push_back("Edit Attached Phone number");
		menu.push_back("Edit PIN");
		int i = show_menu(menu);
		if (i == 1)
			edit_cardholder(card_number);
		else if (i == 2)
			edit_phone_number(card_number);
		else
			edit_pin(card_number);
	}
	const bool verify_card(const string& card_number, const string& card_cvv, const string& pin) {
		if (!card_database.count(card_number)) {
			cout << "\nNo such card\n";
			return 0;
		}
		if (card_database[card_number]->get_card_cvv() != card_cvv or card_database[card_number]->get_pin() != pin) {
			cout << "\nWrong card informations\n";
			return 0;
		}
		return 1;
	}
	const bool is_master(const string &card_number) {
		return visa_or_master[card_number];
	}
	void show_information(const string card_number) {
		card_database[card_number]->show_informations();
	}
	const vector<transaction>& get_transactions(const string& card_number) {
		return transactions[card_number];
	}
	void add_transaction1(const transaction& op) {
		transactions[op.from].push_back(op);
	}
	void add_transaction2(const transaction& op) {
		transactions[op.to].push_back(op);
	}


};