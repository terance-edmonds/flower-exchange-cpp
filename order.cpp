/* valid instruments */
string Instruments[5] = {"Rose", "Lavender", "Lotus", "Tulip", "Orchid"};
/* valid status */
map<int, string> Status = {{0, "New"}, {1, "Rejected"}, {2, "Fill"}, {3, "Pfill"}};

/* check if the given value is in array */
template <class C, typename T>
bool contains(C &&c, T t)
{
    return find(begin(c), end(c), t) != end(c);
};

/* get milliseconds till now since epoch time  */
time_t get_time()
{
    return system_clock::to_time_t(system_clock::now());
}

class Order
{
public:
    string client_id;
    string order_id;
    string instrument;
    int side;
    double price;
    int quantity;
    int status;
    string reason;
    time_t transaction_time;

    Order()
    {
        status = 0;
        transaction_time = get_time();
    };

    /* validate the order */
    void validate();
    /* update order */
    void update();
    /* display the order */
    void display();
    /* set transaction time */
    void set_transaction_time();
    /* get the formatted transaction time */
    char *get_transaction_time();
};

void Order::validate()
{
    if (client_id.empty())
    {
        reason = "Client ID not defined";
    }

    if (!contains(Instruments, instrument))
    {
        reason = "Invalid instrument type";
    }

    if (side != 1 && side != 2)
    {
        reason = "Invalid side value";
    }

    if (price <= 0)
    {
        reason = "Invalid price value";
    }

    if (quantity % 10 != 0 || quantity > 1000 || quantity <= 0)
    {
        reason = "Invalid quantity value";
    }

    if (!reason.empty())
    {
        status = 1;
    }
}

void Order::update()
{
    if (quantity > 0)
    {
        status = 3;
    }

    if (quantity == 0)
    {
        status = 2;
    };
}

void Order::set_transaction_time()
{
    transaction_time = get_time();
}

char *Order::get_transaction_time()
{
    char *buffer;
    struct tm *tmp = gmtime(&transaction_time);
    strftime(buffer, 25, "%Y.%m.%d-%H:%M:%S", tmp);

    return buffer;
}

void Order::display()
{
    cout << "Client ID: " << client_id << endl;
    cout << "Order ID: " << order_id << endl;
    cout << "Instrument: " << instrument << endl;
    cout << "Side: " << side << endl;
    cout << "Price: " << price << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "Status: " << Status[status] << endl;
    cout << "Reason: " << reason << endl;
    cout << "Transaction Time: " << get_transaction_time() << endl;
    cout << endl;
}