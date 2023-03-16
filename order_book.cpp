template <class C>
void _add(C &&c, Order order)
{
    list<Order>::iterator it;
    for (it = c.begin(); it != c.end(); ++it)
    {
        if (it->price >= order.price)
        {
            c.insert(it, order);
            return;
        }
    }

    c.push_back(order);
}

class OrderBook
{
public:
    list<Order> buy_side;
    list<Order> sell_side;

    /* add a new order to the book */
    list<Order> add(Order order);

    /* display the current orders */
    void display();
};

list<Order> OrderBook::add(Order order)
{
    list<Order> trades;

    /* buy */
    if (order.side == 1)
    {
        list<Order>::iterator it;
        for (it = sell_side.begin(); it != sell_side.end(); ++it)
        {
            Order sell_order = *it;
            Order buy_order = order;
            int quantity;

            if (it->price > order.price)
                break;

            if (order.quantity >= it->quantity)
            {
                order.quantity -= it->quantity;
                quantity = it->quantity;
                it->quantity = 0;
            }
            else if (it->quantity >= order.quantity)
            {
                it->quantity = order.quantity;
                quantity = order.quantity;
                order.quantity = 0;
            }
            else
            {
                break;
            }

            /* self update order */
            it->update();

            /* self update order */
            order.update();

            /* set the current buy trade info */
            buy_order.status = order.status;
            buy_order.price = order.price;
            buy_order.quantity = quantity;
            buy_order.set_transaction_time();

            /* set the current sell trade info */
            sell_order.status = it->status;
            sell_order.price = order.price;
            sell_order.quantity = quantity;
            sell_order.set_transaction_time();

            /* add the current order to trades list */
            trades.push_back(buy_order);
            trades.push_back(sell_order);

            /* if the seller order is complete remove from order book */
            if (it->status == 3)
                it = --sell_side.erase(it++);

            /* stop processing if no stocks */
            if (order.quantity == 2)
                break;
        }

        /* if the order is not rejected and not filled add to the book */
        if (order.status != 1 && order.status != 3)
            _add(buy_side, order);

        /* if the order is new */
        if (order.status == 0)
        {
            order.set_transaction_time();
            trades.push_back(order);
        }

        return trades;
    }

    /* sell */
    if (order.side == 2)
    {
        list<Order>::iterator it;
        for (it = buy_side.begin(); it != buy_side.end(); ++it)
        {
            Order buy_order = *it;
            Order sell_order = order;
            int quantity;

            if (it->price < order.price)
                break;

            if (order.quantity >= it->quantity)
            {
                order.quantity -= it->quantity;
                quantity = it->quantity;
                it->quantity = 0;
            }
            else if (it->quantity >= order.quantity)
            {
                it->quantity -= order.quantity;
                quantity = order.quantity;
                order.quantity = 0;
            }
            else
            {
                break;
            }

            /* self update order */
            it->update();

            /* self update order */
            order.update();

            /* set the current buy trade info */
            buy_order.status = it->status;
            buy_order.price = it->price;
            buy_order.quantity = quantity;
            buy_order.set_transaction_time();

            /* set the current sell trade info */
            sell_order.status = order.status;
            sell_order.price = it->price;
            sell_order.quantity = quantity;
            sell_order.set_transaction_time();

            /* add the current order to trades list */
            trades.push_back(sell_order);
            trades.push_back(buy_order);

            /* if the buyer order is complete remove from order book */
            if (it->status == 2)
                it = --buy_side.erase(it);

            /* stop processing if no stocks */
            if (order.quantity == 0)
                break;
        }

        /* if the order is not rejected and not filled add to the book */
        if (order.status != 1 && order.status != 3)
            _add(sell_side, order);

        /* if the order is new */
        if (order.status == 0)
        {
            order.set_transaction_time();
            trades.push_back(order);
        }

        return trades;
    }
}

void OrderBook::display()
{

    cout << "=== Buy Side ==="
         << "\n"
         << endl;
    list<Order>::iterator b_it;
    for (b_it = buy_side.begin(); b_it != buy_side.end(); ++b_it)
    {
        b_it->display();
    }
    cout << endl;

    cout << "=== Sell Side ==="
         << "\n"
         << endl;
    list<Order>::iterator s_it;
    for (s_it = sell_side.begin(); s_it != sell_side.end(); ++s_it)
    {
        s_it->display();
    }
    cout << endl;
}