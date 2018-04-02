/*
File Name: Project5.cpp
Author: Lucas Combs
Course: CSC 402
Date: 4/11/2018
*/

#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable: 4018 )
#pragma once

#include <iomanip>
#include <map>
#include <string>
#include <iostream>

using namespace std;

class ItemInfo {

};

typedef map<string, ItemInfo> Catalog;
typedef map<string, int> Order;




void readCatalog(Catalog& catalog, const string& fileName) {
	while (getline(ss, tmp, ':')) {   // : as separator
									  // turn the string into a series of tokens 
									  //   stored in a vector of strings called tokens
		tokens.push_back(tmp);
	}
}

// reads the input file and creates the catalog; throws a 
// runtime_error if the file cannot be opened
void printCatalog(const Catalog& catalog) {

}
// prints the SKU, description, price, and weight of every item in 
// the catalog 


ItemInfo getItemData(const Catalog& catalog, const string& sku) {

}
// finds a single item by SKU and returns the details as a struct;
// returns a dummy struct with the description "Item not found", 
// price 0.00, and weight 0.00 if the SKU is not in the catalog


void displayOrderItems(const Order& order, const Catalog& catalog) {

}
// Lists the SKU, description, and quantity of each type of order item

void addItem(Order & order, const Catalog& catalog, const string& sku, int quantity) {

}
// adds item(s) to the order; throws a logic_error if the item cannot
// be found in the catalog 

void removeItem(Order & order, const string& sku, int quantity) {

}
// removes items(s) from the order; throws a logic_error if the item 
// cannot be found in the order 

void displayOrderSummary(const Order& order, const Catalog& catalog) {

}
// displays the number of unique item types, the total number of 
// items, the total cost, and the total shipping weight 


int main()
{
	// construct default Order and Catalog
	Catalog catalog = Catalog();
	Order order = Order();

	// test readCatalog exception handling by opening a non-existent file




	// open CatalogData.txt by calling readCatalog which populates the Catalog map 




	// print out the entire catalog

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
