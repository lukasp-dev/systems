/*
Implement a simple limit order book.

Each order has:
- order_id: unique identifier
- side: BUY or SELL
- type: IOC or GFD
- price: limit price
- quantity: number of units

Order Types:
- IOC (Immediate Or Cancel):
  Match immediately against existing orders.
  Any remaining quantity is cancelled and must NOT be added to the order book.

- GFD (Good For Day):
  Match immediately against existing orders.
  Any remaining quantity stays in the order book.

Matching Rules:
1. A BUY order can match a SELL order if:
    buy_price >= sell_price

2. A SELL order can match a BUY order if:
    sell_price <= buy_price

3. BUY orders match against the LOWEST available SELL price first.

4. SELL orders match against the HIGHEST available BUY price first.

5. For multiple orders at the same price,
   match the OLDEST order first (FIFO / price-time priority).

Supported Operations:

BUY
    Submit a BUY order.

SELL
    Submit a SELL order.

CANCEL <order_id>
    Remove the specified resting order from the order book.
    If the order does not exist, do nothing.

MODIFY <order_id> <new_side> <new_price> <new_quantity>
    Modify an existing resting order.
    Treat the modification as:
        1. cancel the old order
        2. submit a new GFD order with the same order_id

    The modified order loses its previous time priority.

PRINT
    Print the current resting order book.

Implementation Requirements:
- Fully filled orders must be removed from the book.
- Fully filled orders must also be removed from the order-id index.
- Empty price levels must be removed.
- GFD orders with remaining quantity stay in the book.
- IOC orders with remaining quantity are discarded.

Example:

SELL GFD 101 5 A
SELL GFD 102 10 B
BUY GFD 102 12 X

Processing:
- X buys 5 from A at price 101
- X buys 7 from B at price 102

Remaining book:
SELL 102 -> B has quantity 3

Possible data structure:

BUY side:
    price -> FIFO list of orders
    highest price first

SELL side:
    price -> FIFO list of orders
    lowest price first

order_id -> { side, price, iterator }

Target complexities:
- Best BUY / SELL price lookup: O(1) from map begin()
- Add price level: O(log P)
- Cancel known order: O(log P) + O(1) list erase
*/

#include <iostream>
#include <unordered_map>
#include <map>
#include <list>
#include <string>
#include <algorithm>
#include <iterator>
#include <functional>

using namespace std;

enum OrderType {
    IOC,
    GFD
};

enum Side {
    BUY,
    SELL
};

struct Order {
    string order_id;
    Side side;
    OrderType type;
    int quantity;
};

struct OrderHandle {
    Side side;
    int price;
    list<Order>::iterator it;
};

class OrderBook {
private:
    // Highest BUY price first
    map<int, list<Order>, greater<int>> buys;

    // Lowest SELL price first
    map<int, list<Order>> sells;

    // order_id -> location in the order book
    unordered_map<string, OrderHandle> orderIndex;

    void processBuy(Order& order, int price) {
        // TODO:
        // Match against sells.begin()
        auto& [buy_order_id, buy_side, buy_type, buy_quantity] = order;
        int buy_price = price;

        while(
            buy_quantity > 0 &&
            !sells.empty() &&
            sells.begin()->first <= buy_price
        ) {
            auto cheapestSellIt = sells.begin();
            auto& orders = cheapestSellIt->second;
            Order& resting = orders.front();

            int traded = min(buy_quantity,  resting.quantity);
            buy_quantity -= traded;
            resting.quantity -= traded;

            if(resting.quantity == 0) {
                orderIndex.erase(resting.order_id);
                orders.pop_front();

                if(orders.empty()){
                    sells.erase(cheapestSellIt);
                }
            }
        }

        if(buy_quantity > 0 && buy_type == GFD) {
            addBuy(order, price);
        }
    }

    void processSell(Order& order, int price) {
        // TODO:
        // Match against buys.begin()
        auto& [sell_order_id, sell_side, sell_type, sell_quantity] = order;
        int sell_price = price;

        while(
            sell_quantity > 0 &&
            !buys.empty() &&
            buys.begin()->first >= sell_price
        ) {
            auto largestBuyPriceIt = buys.begin();
            auto& orders = largestBuyPriceIt->second;
            Order& resting = orders.front();

            int traded = min(resting.quantity, sell_quantity);
            sell_quantity -= traded;
            resting.quantity -= traded;

            if(resting.quantity == 0) {
                orderIndex.erase(resting.order_id);
                orders.pop_front();

                if(orders.empty()) {
                    buys.erase(largestBuyPriceIt);
                }
            }
        }

        if(sell_quantity > 0 && sell_type == GFD) {
            addSell(order, price);
        }
    }

    void addBuy(Order& order, int price) {
        // TODO:
        // Add remaining GFD BUY order to buys
        // Update orderIndex
        buys[price].push_back(order);
        auto it = prev(buys[price].end());

        OrderHandle order_handle = {
            BUY,
            price,
            it
        };

        orderIndex[order.order_id] = order_handle;
    }

    void addSell(Order& order, int price) {
        // TODO:
        // Add remaining GFD SELL order to sells
        // Update orderIndex
        sells[price].push_back(order);
        auto it = prev(sells[price].end());

        OrderHandle order_handle = {
            SELL,
            price,
            it
        };
        
        orderIndex[order.order_id] = order_handle;
    }

public:
    void buy(
        OrderType type,
        int price,
        int quantity,
        const string& order_id
    ) {
        Order order{
            order_id,
            BUY,
            type,
            quantity
        };

        processBuy(order, price);
    }

    void sell(
        OrderType type,
        int price,
        int quantity,
        const string& order_id
    ) {
        Order order{
            order_id,
            SELL,
            type,
            quantity
        };

        processSell(order, price);
    }

    void cancel(const string& order_id) {
        // TODO
        auto cancel_it = orderIndex.find(order_id);
        if(cancel_it == orderIndex.end()) {
            return; // no corresponding order in the order book.
        }

        OrderHandle handle = cancel_it->second;
        
        if(handle.side == BUY) {
            auto& orders = buys[handle.price];
            orders.erase(handle.it);

            if(orders.empty()) {
                buys.erase(handle.price);
            }
        } else {
            auto& orders = sells[handle.price];
            orders.erase(handle.it);

            if(orders.empty()) {
                sells.erase(handle.price);
            }
        }

        orderIndex.erase(cancel_it);
    }

    void modify(
        const string& order_id,
        Side new_side,
        int new_price,
        int new_quantity
    ) {
        // TODO
        auto modify_it = orderIndex.find(order_id);
        if(modify_it == orderIndex.end()) {
            return; // no corresponding order in the order book.
        }

        cancel(order_id);

        if(new_side == BUY) {
            buy(GFD, new_price, new_quantity, order_id);
        } else {
            sell(GFD, new_price, new_quantity, order_id);
        }
    }

    void print() const {
        // TODO
        cout << "SELL: " << '\n';
        
        for (auto it = sells.rbegin(); it != sells.rend(); ++it) {
            int total_quantity = 0;
            for(const Order& order : it->second) {
                total_quantity += order.quantity;
            }
            cout << it->first << " " << total_quantity << "\n";
        }

        cout << "BUY: " << '\n';

        for (const auto& [price, orders] : buys) {
            int total_quantity = 0;

            for (const auto& order : orders) {
                total_quantity += order.quantity;
            }
            cout << price << " " << total_quantity << '\n';
        }
    }
};

int main() {
    OrderBook book;

    // Example:
    //
    // book.sell(GFD, 101, 5, "A");
    // book.sell(GFD, 102, 10, "B");
    // book.buy(GFD, 102, 12, "X");
    //
    // book.cancel("B");
    // book.modify("A", BUY, 100, 20);
    // book.print();

    return 0;
}