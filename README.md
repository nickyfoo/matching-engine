# Matching Engine
An implementation of an order book matching engine, along witih a multithreaded TCP server and client.

This was done as an exploration of the Windows Socket API, TCP networking and multithreading in C++, and to attempt to implement an order book in an efficient manner.

# Implementation Details

## Order book
### A brief background
An order book is a list of buy and sell orders, organized by price level, and we attempt to match buyers and sellers.
This order book supports a few operations:
- Limit Order: an order to buy or sell a quantity at a specific price or better
- Market Order: an order to buy or sell a specific quantity immediately, regardless of price
- Cancel Order: an order to cancel a previous order in the order book

### Design decisions
Keeping in mind that the order book is organized by price level, I chose a sorted associative container, `std::map`, for being able to access any price level with `O(logn)` time complexity.

The keys are thus the price level, while the values are a `std::list` of the current valid orders at that price level. This allows fast access to the first order at the price level, when it needs to be matched to an incoming order. Furthermore, given the iterators to any element in `std::list`, we can erase it in `O(1)` time, which would support the Cancel order very well. We thus use an additional `std::unordered_map` mapping the `orderID` to the iterator to that `Order` in the order book.

Note also the use of `std::move` semantics to avoid making unnecessary copies when adding `Order`s to the order book.

### OrderRequest and OrderDispatcher
When accepting a request from the TCP client, I opted to instantiate a dispatcher that converts a request into a proper `Order` object to be inserted into the order book, based on the type of order request.

### Testing
The OrderBook was covered with some tests, found in `server/test`. The test suite can be run just by instantiating an `OrderBookTest` object, and it will print the number of passed cases.

Testing was important for making changes quickly and confidently, since I would know when any changes broke the regression tests. This ensured the correctness of the code.




## TCP Networking

### RAII and wrapping WSA for clean code
I used the Windows Socket API (WSA), but wrapped most of the function calls to illustrate RAII (Resource acquisition is initialization) in order to reduce the chances of leaking resources. This can be seen in the `common/WSALoader`, `server/TCPServer` and `server/TCPConnection` classes. Wrapping WSA calls within functions that threw `std::runtime_error` helped with cleaner code and one place to handle the cleanup of resources.

### Sending requests from client to server
I opted to do bytewise copy of the `OrderRequest` class when sending packets from client to server for convenience. Note that this is only possible because the `OrderRequest` class has only Plain Old Data (POD) member types. It wouldn't work if say the OrderID were a `std::string` since that needs to be allocated on the heap.

## Multithreading
I used `std::jthread` in both the client and server to maximise CPU usage.
The choice of `std::jthread` over the normal `std::thread` is because of the automatic joining when it goes out of scope, following RAII principles.

### Client
I used multithreading here to generate multiple threads to make a series of random orders to the order book, to simulate load from up to a thousand users.

### Server
I used multithreading here to accept waiting TCP connections from the client. I spawned a new thread every time there was an incoming connection, and assigned it to call the dispatch method, so that the current thread is able to continue accepting more connections.

Note the use of `std::lock_guard` in the `OrderDispatcher::dispatch()` method to avoid data races.

## Benchmarking

To assess the performance benefit of the multithreading, I adjusted parameters such as number of client threads spawned, and number of orders made per thread. I timed it using the `Timer` class, which uses RAII to calculate the time taken between its instantiation and destruction.

As was to be expected, the multithreading outperformed the sequential execution, and prevented connection failures when there were many client threads, likely due to the sequential execution taking too long that the connection request timed out.

It is worth noting that for sequential execution, there were a significant amount of connection failures when there were more than 100 orders per thread, and the runtime might have been larger had these connections been successful. However, the fact that it is rejecting so many connections is already indication enough that the concurrent execution is superior.

### Concurrent Execution:

Benchmarking for 100 client threads, timings in ms
| Num Orders / Thread | Run 1 | Run 2 | Run 3 | Average |
|---------------------|-------|-------|-------|---------|
| 1000                | 252   | 258   | 284   | 265     |
| 10000               | 1689  | 1867  | 1738  | 1765    |
| 100000              | 15998 | 15964 | 16275 | 16079   |

Benchmarking for 1000 client threads, timings in ms
| Num Orders / Thread | Run 1 | Run 2 | Run 3 | Average |
|---------------------|-------|-------|-------|---------|
| 100                 | 1285  | 1208  | 1352  | 1282    |
| 1000                | 3180  | 2976  | 2902  | 3019    |
| 10000               | 20074 | 20171 | 19794 | 20013   |

### Sequential Execution:

Benchmarking for 100 client threads, timings in ms
| Num Orders / Thread | Run 1 | Run 2 | Run 3 | Average |
|---------------------|-------|-------|-------|---------|
| 1000                | 1431  | 1465  | 1479  | 1458    |
| 10000               | 12600 | 12417 | 12358 | 12458   |
| 100000              | 122681| 122010| 122460| 122384  |

Benchmarking for 1000 client threads, timings in ms
| Num Orders / Thread | Run 1 | Run 2 | Run 3 | Average |
|---------------------|-------|-------|-------|---------|
| 100                 | 2463  | 2484  | 2543  | 2497    |
| 1000                | 4872  | 4847  | 4978  | 4899    |
| 10000               | 26406 | 26676 | 26032 | 26371   |

Connection Failures for 1000 client threads
| Num Orders / Thread | Run 1 | Run 2 | Run 3 | Average |
|---------------------|-------|-------|-------|---------|
| 100                 | 0     | 0     | 0     | 0       |
| 1000                | 695   | 698   | 689   | 694     |
| 10000               | 799   | 797   | 796   | 797     |
