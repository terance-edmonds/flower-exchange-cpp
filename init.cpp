#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <list>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

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

/* read csv */
void read_csv(string, string);
/* write csv */
void write_csv(ofstream &, Order);
/* process an order */
list<Order> process_order(Order);

int main()
{
    string fin_path, fout_path = "out/execution_rep.csv";

    cout << "Enter input file path: ";
    cin >> fin_path;

    high_resolution_clock::time_point begin_time = high_resolution_clock::now();
    read_csv(fin_path, fout_path);
    high_resolution_clock::time_point end_time = high_resolution_clock::now();

    duration<double> time_elapsed = duration_cast<duration<double>>(end_time - begin_time);
    cout << endl;
    cout << "Finished in: " << time_elapsed.count() << "s" << endl;
    cout << "Output file: " << fout_path;

    return 0;
}

/* read csv file line by line */
void read_csv(string fin_path, string fout_path)
{
    fstream fin;
    ofstream fout;
    static string line;
    unsigned int i = 1;

    /* open data csv file */
    fin.open(fin_path);

    /* open output csv file */
    fout.open(fout_path);
    /* set header for output file */
    fout << "Client Order ID,Order ID,Instrument,Side,Price,Quantity,Exec Status,Reason,Transaction Time\n";

    if (!fin.is_open())
    {
        cout << "Data file not found";
        exit(0);
    }

    cout << "Processing..." << endl;

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
        {
            write_csv(fout, order);
            continue;
        }

        /* write the trades to output file */
        list<Order> trades = process_order(order);
        for (Order &_order : trades)
        {
            write_csv(fout, _order);
        }
    }

    /* close data file */
    fin.close();
    /* close output file */
    fout.close();
}

list<Order> process_order(Order order)
{
    /* get the relevant order book */
    OrderBook &order_book = order_books[order.instrument];

    /* add order to relevant order book */
    /* returns the trades */
    return order_book.add(order);
}

void write_csv(ofstream &file, Order order)
{
    file << order.client_id << "," << order.order_id << "," << order.instrument << "," << order.side << "," << order.price << "," << order.quantity << "," << Status[order.status] << "," << order.reason << ","
         << order.transaction_time
         << "\n";
}
