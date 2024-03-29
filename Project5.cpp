/*
File Name: Project5.cpp
Author: Lucas Combs
Course: CSC 402
Date: 04/07/2018
*/

#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <iomanip>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class ItemInfo {
public:
	//Construct directly from input file
	ItemInfo(vector<string> tokens) {
		description = tokens.at(0);
		price = stod(tokens.at(1));
		weight = stod(tokens.at(2));
	}

	//Manual constructor
	ItemInfo(string description, double price, double weight) : description(description), price(price), weight(weight) {};

	//<Getters>
	string getDescription() {
		return this->description;
	}

	double getPrice() {
		return this->price;
	}

	double getWeight() {
		return this->weight;
	}
	//</Getters>

	//Print support
	friend ostream& operator<<(ostream& out, const ItemInfo);

private:
	string description;
	double price, weight;

};

//Print operator for ItemInfo class
ostream& operator<<(ostream& out, const ItemInfo i) {
	out << setw(60) << left << i.description
		<< '$' << setw(15) << i.price
		<< setw(15) << i.weight;
	return out;
}

//Needed typedefs for our "Tables"
typedef map<string, ItemInfo> Catalog;
typedef map<string, int> Order;

// reads the input file and creates the catalog; throws a 
// runtime_error if the file cannot be opened
void readCatalog(Catalog& catalog, const string& fileName) {
	//Open file
	ifstream data_file(fileName);

	//Make sure the file exists
	if (!data_file) {
		throw invalid_argument{ "File not found\n" };
	}

	//Variable to store each line
	string line;

	//Loop line-by-line
	while (getline(data_file, line, '\n')) {
		stringstream iss(line); //Break string down by : tokens
		string elem; // line token swap area
		vector<string> tokens; //All tokens (Except the item#, which is first)
		string SKU; //Store item SKU

		//Grab SKU before loop
		getline(iss, SKU, ':');

		//Loop through remaining tokens, assembly list to pass to ItemInfo(vector<string>)
		while (getline(iss, elem, ':')) {
			tokens.push_back(elem);
		}

		//Create item to place in catalog
		ItemInfo i = ItemInfo(tokens);

		//Catalog item
		catalog.insert({ SKU, i });
	}

	//Close file, we are finished
	data_file.close();
}

// prints the SKU, description, price, and weight of every item in 
// the catalog 
void printCatalog(const Catalog& catalog) {
	//Print headings
	cout << "Catalog:\n";
	cout
		<< setw(10) << left << "SKU"
		<< setw(60) << left << "Description"
		<< setw(15) << left << "Unit Price"
		<< setw(15) << left << "Shipping Wt. (lbs.)"
		<< "\n";
	cout << string(110, '=') << "\n";

	//Loop through items & print them out
	for (Catalog::const_iterator it = catalog.begin(); it != catalog.end(); it++) {
		cout << setw(10) << left << it->first << it->second << "\n";
	}
}

// finds a single item by SKU and returns the details as a struct;
// returns a dummy struct with the description "Item not found", 
// price 0.00, and weight 0.00 if the SKU is not in the catalog
ItemInfo getItemData(const Catalog& catalog, const string& sku) {
	Catalog::const_iterator it = catalog.find(sku);
	if (it == catalog.end()) { //Handle invalid item
		return ItemInfo("Item Not Found", 0, 0);
	}
	else {
		return it->second;
	}
}

// Lists the SKU, description, and quantity of each type of order item
void displayOrderItems(const Order& order, const Catalog& catalog) {
	//Print headings
	cout << "Order Items:\n";
	cout
		<< setw(10) << left << "SKU"
		<< setw(60) << left << "Description"
		<< setw(15) << left << "Quantity"
		<< "\n";
	cout << string(110, '=') << "\n";

	//Loop through items
	for (Order::const_iterator it = order.begin(); it != order.end(); it++) {
		ItemInfo d = getItemData(catalog, it->first);
		cout
			<< setw(10) << left << it->first
			<< setw(60) << left << d.getDescription()
			<< setw(15) << left << it->second
			<< "\n";
	}
}

// adds item(s) to the order; throws a logic_error if the item cannot
// be found in the catalog 
void addItem(Order & order, const Catalog& catalog, const string& sku, int quantity) {

	//Make sure item is valid
	ItemInfo i = getItemData(catalog, sku);
	if (i.getDescription() == "Item Not Found") {
		throw logic_error("Item not found is catalog: " + sku);
	}
	else {
		//Find existing qty (if any)
		Order::const_iterator it = order.find(sku);
		int exisiting_qty = it == order.end() ? 0 : it->second;

		//Update order with new QTY
		order.insert_or_assign(sku, exisiting_qty + quantity);
	}
}

// removes items(s) from the order; throws a logic_error if the item 
// cannot be found in the order 
void removeItem(Order & order, const string& sku, int quantity) {
	//Pull item
	Order::const_iterator it = order.find(sku);

	//Make sure item is on order
	if (it == order.end()) {
		throw logic_error("Item not found in order: " + sku);
	}

	//Remove item if <= 0 are left on order
	if (it->second - quantity <= 0) {
		order.erase(sku);
	}
	else {
		//remove QTY from order
		order.insert_or_assign(sku, it->second - quantity);
	}
}

// displays the number of unique item types, the total number of 
// items, the total cost, and the total shipping weight 
void displayOrderSummary(const Order& order, const Catalog& catalog) {
	//Print heading
	cout << "Order Summary:\n";
	cout << string(110, '=') << "\n";

	int num_items = 0; //# items orders TOTAL(QTY)
	double subtotal = 0; //Total cose
	double weight = 0; //Total weight

	//Loop through items
	for (Order::const_iterator it = order.begin(); it != order.end(); it++) {
		//Grab item info
		ItemInfo info = getItemData(catalog, it->first);

		//Update totals
		num_items += it->second;
		subtotal += info.getPrice() * it->second;
		weight += info.getWeight() * it->second;
	}

	//Display summary
	cout << "Distinct items: " << order.size() << "\n";
	cout << "Total number of items: " << num_items << "\n";
	cout << "Total cost: $" << subtotal << "\n";
	cout << "Total weight: " << weight << " lbs\n";
}

//Load and use catalog!
int main()
{
	// construct default Order and Catalog
	Catalog catalog = Catalog();
	Order order = Order();

	cout << "\nReading invalid catalog:\n";
	// test readCatalog exception handling by opening a non-existent file
	try {
		readCatalog(catalog, "C:\\Users\\Lucas\not_real_file.txt");
	}
	catch (invalid_argument e) {
		cout << "Caught invalid file exception.\n";
	}

	cout << "\nReading correct catalog:\n";
	// open CatalogData.txt by calling readCatalog which populates the Catalog map 
	readCatalog(catalog, "C:\\Users\\Lucas\\Dropbox\\CSC 402\\Project5\\CatalogData.txt");

	// print out the entire catalog
	printCatalog(catalog);

	// search for a few specific items by SKU, some found, at least one that cannot be found
	// print out the details of few items using getItemData
	cout
		<< "\nSearching for 3 items, last one will not be found:\n"
		<< getItemData(catalog, "4123BLU") << "\n"
		<< getItemData(catalog, "4123BLA") << "\n"
		<< getItemData(catalog, "Keyboard") << "\n";

	cout << "\nAdding items to order.....\n";
	// Add several items to order
	addItem(order, catalog, "4123BLU", 5);
	addItem(order, catalog, "4123BLU", 5); //Add a second time
	addItem(order, catalog, "93456C", 12);
	addItem(order, catalog, "93456C", 1); //Add a second time
	addItem(order, catalog, "93456B", 1);
	addItem(order, catalog, "576361B", 1);

	// display items in the order
	displayOrderItems(order, catalog);


	// remove item(s) such that no item is completely removed
	removeItem(order, "93456C", 2);
	removeItem(order, "4123BLU", 2);

	cout << "\nRemoving 2 of 93456C and 4123BLU\n";
	// display items in the order to verify quantities
	displayOrderItems(order, catalog);

	// try to remove an item that is not in the order to test exception handling
	cout << "\nRemoving item not on order: 93456D\n";
	try {
		removeItem(order, "93456D", 12);
	}
	catch (logic_error) {
		cout << "Caught item not being on order.\n";
	}

	// remove all of at least one item to make sure the order no longer shows it
	cout << "\nRemoving the entire 93456B item.\n";
	removeItem(order, "93456B", 1);

	// display items in the order
	displayOrderItems(order, catalog);

	// display order summary
	cout << "\n";
	displayOrderSummary(order, catalog);

	// portable pause
	cin.get();
	return 0;
}