#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <list>
#include <time.h>

using namespace std;

#include "order.cpp"
#include "order_book.cpp"

/* initiate order books */
OrderBook rose_orders;
OrderBook lavender_orders;
OrderBook lotus_orders;
OrderBook tulip_orders;
OrderBook orchid_orders;

/* set order books according to the instrument type */
map<string, OrderBook> order_books = {
    {"Rose", rose_orders},
    {"Lavender", lavender_orders},
    {"Lotus", lotus_orders},
    {"Tulip", tulip_orders},
    {"Orchid", orchid_orders},
};

/* report */
list<Order> report;

/* read csv */
void read_csv(string);
/* write csv */
void write_csv(string);
/* process an order */
void process_order(Order);

int main()
{
    read_csv("data/data_1.csv");

    write_csv("execution_rep.csv");

    cout << "Done";

    return 0;
}

/* read csv file line by line */
void read_csv(string path)
{
    fstream fin;
    static string line;
    unsigned int i = 1;

    /* open csv file */
    fin.open(path);

    if (!fin.is_open())
    {
        cout << "Data file not found";
        return;
    }

    /* to ignore the first line */
    getline(fin, line);

    while (getline(fin, line))
    {
        Order order;
        stringstream line_string(line);
        static string temp;

        order.order_id = "ord" + to_string(i++);

        getline(line_string, order.client_id, ',');
        getline(line_string, order.instrument, ',');

        temp = "";
        getline(line_string, temp, ',');
        order.side = atoi(temp.c_str());

        temp = "";
        getline(line_string, temp, ',');
        order.quantity = atoi(temp.c_str());

        temp = "";
        getline(line_string, temp, ',');
        order.price = atoi(temp.c_str());

        /* validate order */
        order.validate();

        /* if the order is rejected continue to the next line */
        if (order.status == 1)
            continue;

        process_order(order);
    }
}

void process_order(Order order)
{
    /* get the relevant order book */
    OrderBook &order_book = order_books[order.instrument];

    /* add to relevant order book */
    list<Order> trades = order_book.add(order);

    report.splice(report.end(), trades);
}

void write_csv(string path)
{
    ofstream file;

    file.open(path);

    /* header */
    file << "Client Order ID,Order ID,Instrument,Side,Price,Quantity,Exec Status,Reason,Transaction Time\n";

    list<Order>::iterator order;
    for (order = report.begin(); order != report.end(); ++order)
    {
        file << order->client_id << "," << order->order_id << "," << order->instrument << "," << order->side << "," << order->price << "," << order->quantity << "," << Status[order->status] << "," << order->reason << ","
             << "transaction time"
             << "\n";
    }

    file.close();
}