/*
File Name: Project5.cpp
Author: Lucas Combs
Course: CSC 402
Date: 4/11/2018
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
	ItemInfo(vector<string> values) {
		description = values.at(0);
		price = stod(values.at(1));
		weight = stod(values.at(2));
	}

	ItemInfo(string description, double price, double weight) : description(description), price(price), weight(weight) {};

	string getDescription() {
		return this->description;
	}

	double getPrice() {
		return this->price;
	}

	double getWeight() {
		return this->weight;
	}

	friend ostream& operator<<(ostream& out, const ItemInfo);
private:
		string description;
		double price, weight;

};

ostream& operator<<(ostream& out, const ItemInfo i){
	out << setw(60) << left << i.description
		<< '$' << setw(15) << i.price
		<< setw(15) << i.weight;
	return out;
}

typedef map<string, ItemInfo> Catalog;
typedef map<string, int> Order;

void readCatalog(Catalog& catalog, const string& fileName) {
	ifstream data_file(fileName);
	if (!data_file) {
		throw invalid_argument{"File not found\n"};
		return;
	}
	string line;

	while (getline(data_file, line, '\n')) {

		stringstream iss(line);
		string elem;
		vector<string> vals;
		string start;
		getline(iss, start, ':');
		while (getline(iss, elem, ':')){
			vals.push_back(elem);
		}
		ItemInfo i = ItemInfo(vals);
		catalog.insert({start, i});
	}
	data_file.close();
}

// reads the input file and creates the catalog; throws a 
// runtime_error if the file cannot be opened
void printCatalog(const Catalog& catalog) {
	cout << "Catalog:\n";
	cout 
		<< setw(10) << left <<  "SKU" 
		<< setw(60) << left << "Description"
		<< setw(15) << left << "Unit Price"
		<< setw(15) << left << "Shipping Wt. (lbs.)"
		<< "\n";
	cout << string(110, '=') << "\n";
	for (pair<string, ItemInfo> i : catalog) {
		cout << setw(10) << left << i.first << i.second << "\n";
	}
}
// prints the SKU, description, price, and weight of every item in 
// the catalog 


ItemInfo getItemData(const Catalog& catalog, const string& sku) {
	try {
		return catalog.at(sku);
	}catch (out_of_range) {
		return ItemInfo("Item Not Found",0,0);
	}
}
// finds a single item by SKU and returns the details as a struct;
// returns a dummy struct with the description "Item not found", 
// price 0.00, and weight 0.00 if the SKU is not in the catalog


void displayOrderItems(const Order& order, const Catalog& catalog) {
	cout << "Order Items:\n";
	cout
		<< setw(10) << left << "SKU"
		<< setw(60) << left << "Description"
		<< setw(15) << left << "Unit Price"
		<< "\n";
	cout << string(110, '=') << "\n";

	for (pair<string, int> item: order) {
		ItemInfo d = getItemData(catalog, item.first);
		cout
			<< setw(10) << left << item.first
			<< setw(60) << left << d.getDescription()
			<< setw(15) << left << d.getPrice()
			<< "\n";
	}
}
// Lists the SKU, description, and quantity of each type of order item

void addItem(Order & order, const Catalog& catalog, const string& sku, int quantity) {
	ItemInfo i = getItemData(catalog, sku);
	if (i.getDescription() == "Item Not Found") {
		throw logic_error("Item not found is catalog: " + sku);
	}else {
		try {
			int exisiting_qty = order.at(sku);
			order.insert_or_assign(sku, exisiting_qty + quantity);
		}catch (out_of_range) {
			order.insert_or_assign(sku, quantity);
		}
	}
}
// adds item(s) to the order; throws a logic_error if the item cannot
// be found in the catalog 

void removeItem(Order & order, const string& sku, int quantity) {
	try {
		int existing_qty = order.at(sku);
		if (existing_qty - quantity <= 0) {
			order.erase(sku);
		}else {
			order.insert_or_assign(sku, existing_qty - quantity);
		}
	}catch (out_of_range) {
		throw logic_error("Item not found in order: " + sku);	
	}
}
// removes items(s) from the order; throws a logic_error if the item 
// cannot be found in the order 

void displayOrderSummary(const Order& order, const Catalog& catalog) {
	cout << "Order Summary:\n";
	cout << string(110, '=') << "\n";

	

	cout << "Distinct item types";
}
// displays the number of unique item types, the total number of 
// items, the total cost, and the total shipping weight 


int main()
{
	// construct default Order and Catalog
	Catalog catalog = Catalog();
	Order order = Order();

	// test readCatalog exception handling by opening a non-existent file
	try {
		readCatalog(catalog, "C:\\Users\Lucas\not_real_file.txt");
	}
	catch (invalid_argument e) {
		cout << "Caught exception.";
	}

	cout << "\nReal file now\n";
	// open CatalogData.txt by calling readCatalog which populates the Catalog map 
	readCatalog(catalog, "C:\\Users\\Lucas\\Dropbox\\CSC 402\\Project5\\CatalogData.txt");

	// print out the entire catalog
	printCatalog(catalog);

	// search for a few specific items by SKU, some found, at least one that cannot be found


	// print out the details of few items using getItemData


	// Add several items to order


	// display items in the order



	// remove item(s) such that no item is completely removed



	// display items in the order to verify quantities


	// try to remove an item that is not in the order to test exception handling


	// remove all of at least one item to make sure the order no longer shows it


	// display items in the order




	// display order summary


	// portable pause
	cin.get();
	return 0;
}
