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
    time_t arrival_time;
    time_t completion_time;

    Order()
    {
        status = 0;
        arrival_time = time(0);
        completion_time = arrival_time;
    };

    /* validate the order */
    void validate();
    /* update order */
    void update();
    /* return the formatted transaction time */
    char *transaction_time();
    /* display the order */
    void display();
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
        completion_time = time(0);
    };
}

char *Order::transaction_time()
{
    char *buffer;
    time_t time_diff = completion_time - arrival_time;
    struct tm *tmp;

    tmp = localtime(&time_diff);
    strftime(buffer, 256, "%Y.%m.%d-%H:%M:%S", tmp);

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
    cout << "Transaction Time: " << transaction_time() << endl;
    cout << endl;
}