# Tool Rental

The task is to implement a set of classes that will optimize the profit of rental companies.

We assume that we are running a tool rental service. The rental service has various tools available for rent in different quantities, e.g., 1x excavator, 3x vibrating rammer, etc. The rental service has a website where customers submit their rental requests. The rental service uses innovative business models, thus renting out its tools through an auction. Customers fill out their rental requests, each containing information about the time period (interval from-to) and the offered payment for the rental. The rental service then selects from the submitted intervals to achieve maximum profit. The following constraints must be observed during the rental process:

- We can serve only as many customers at once as the number of pieces of a given type of tool we have.
- The intervals of consecutive rentals of individual pieces of equipment must not overlap or touch.
- Our rental service is not very wealthy, so we have a maximum of 3 pieces of each type of equipment.

The above-described problem is not as trivial as it might seem at first glance. The solution can be found using dynamic programming; the algorithm was explained in the last BI-AG1 lecture. For 1, 2, and 3 pieces of rental equipment, the problem can be solved in times O(n log(n)), O(n^2), and O(n^3). Since it is a problem with higher time complexity, it is worth using multiple threads to speed up the computation.

For example, for the input intervals:

| from | to | offered payment |
|------|----|------------------|
| 0    | 6  | 8                |
| 7    | 9  | 2                |
| 10   | 13 | 2                |
| 0    | 1  | 4                |
| 2    | 11 | 4                |
| 12   | 13 | 4                |
| 0    | 3  | 2                |
| 4    | 5  | 2                |
| 8    | 13 | 8                |


```text
Rental plan if we rent 1 piece of equipment:
1: 0-6 8-13  
profit 16

Rental plan if we rent 2 pieces of equipment:
1: 0-6 7-9 12-13  
2: 0-1 4-5 8-13  
profit 28

Rental plan if we rent 3 pieces of equipment:
1: 0-6 7-9 10-13  
2: 0-1 2-11 12-13  
3: 0-3 4-5 8-13  
profit 36
```
The expected solution must properly integrate into the infrastructure described below and must correctly create, plan, synchronize, and terminate threads. The actual algorithmic solution to the problem is not strictly necessary; the test environment provides an interface that can solve the given problem sequentially.

***

The problem to be solved is represented by an instance of `CProblem`. The class contains member variables with customer requests and information on the number of pieces of rented equipment.

Problems are bundled into groups (packs), represented by the class `CProblemPack`. For example, if the rental service rents multiple types of equipment, there will be one instance of the problem to be solved for each type of equipment in `CProblemPack`.

Rental services submit problem packs continuously, and communication between the solver and the rental service is handled by the class `CCompany`. This class contains an interface that allows for loading additional problems to be solved (`waitForPack` method) and an interface for returning solved problems (`solvedPack` method). Your implementation will create 2 helper threads for each instance of `CCompany`. One thread will continuously load new problem packs to be solved and forward them for processing. The second thread will return the solved problems (continuously calling `solvedPack`). These helper threads handle communication with the solver but are not intended for performing the actual computations (the problems to be solved are computationally intensive, and the communication threads might not be able to forward the data in time). The task of the communication threads is to ensure the correct order of returning the solved problem packs, which must be returned in the same order they were received. Multiple instances of problem packs can be loaded and processed simultaneously (this is desirable and necessary). However, it is crucial to maintain the order when returning them.

The entire computation is encapsulated in an instance of `COptimizer`. This class contains references to the individual rental services, controls the starting and pausing of the computation, and manages the computational threads. The computational threads are used to solve the assigned problems from all serviced rental companies. The computational threads will receive a problem from the communication thread, perform the actual computations, and return the solved problem to the communication thread of the appropriate rental service to return it at the correct moment (maintaining the order). There will be exactly and only the specified number of computational threads, which allows for the optimal use of the computer's hardware (number of CPUs).

The `COptimizer` class will be used according to the following scenario:

- An instance of `COptimizer` is created.
- Rental services are registered (by calling the `addCompany` method).
- The actual computation is started (by calling the `start` method). The `start` method receives the number of computational threads (workThreads) as a parameter. It creates these threads and lets them wait for incoming requests. At the same time, it creates two communication threads for each rental service, one for receiving requests and the other for returning results. After starting the threads, the `start` method returns to the caller.
- The communication threads of the rental services continuously take over requests (calling the corresponding `waitForPack` methods in a loop). The thread receiving requests will terminate when it reads an empty request (smart pointer contains nullptr).
- The computational threads take over problems from the communication threads and solve them. After solving, they pass the problem back to the return thread of the corresponding rental service.
- The return threads wait for the solved problem and return the problems in the correct order (using the `solvedPack` method). Solved problems must be returned as soon as possible (according to the order) and cannot be stored and returned only at the end of the computation. The return thread will terminate if no more problems come from the given rental service (waitForPack previously returned nullptr) and all problems from this rental service have been solved and returned.
- The test environment will call the `stop` method at some point. The call will wait for the completion of all rental service handling, terminate the working threads, and return to the caller.
- The `COptimizer` instance is released.

Used classes and their interfaces:

`CInterval` is a class representing a single customer request. It is a very simple class that encapsulates the rental interval and the offered price. The class is implemented in the test environment, and its interface must not be changed. The interface includes:
- `m_From` indicates the start of the interval,
- `m_To` indicates the end of the interval,
- `m_Payment` indicates the offered price.
You can rely on `m_From < m_To`.

`CProblem` is a class representing a single problem to be solved. It is an abstract class, and its implementation (specifically, the implementation of the descendant) is done in the test environment. The interface must not be changed. The interface includes:
- `m_Count` indicates the number of pieces of rented equipment (1, 2, or 3),
- `m_Intervals` a list of customer rental requests,
- `m_MaxProfit` the result of the calculation - the maximum profit achievable from the rentals. The value needs to be calculated and set.
The class also includes a constructor and an `add` method to simplify work (see attached implementation).

`CProblemPack` is a class representing a pack of problems to be solved. It is an abstract class, and its implementation (specifically, the implementation of the descendant) is done in the test environment. The interface must not be changed. The interface includes:
- `m_Problems` an array of instances of problems to be solved.
The class also includes an `add` method to simplify work (see attached implementation).

`CCompany` is a class representing a single rental service. The class only defines the interface, and the actual implementation in the test environment is a descendant of `CCompany`. Thus, you cannot change the class interface. The available methods are:
- `waitForPack` for loading the next problem pack from the rental service. The method returns an instance to be processed or an invalid pointer (smart pointer contains nullptr) if there are no more instances of problems to be processed for this rental service. The method call may take a long time, so you must allocate a separate thread to handle it, which will call the method in a loop. The thread must not perform any computationally intensive tasks; it must forward the obtained instance of `CProblemPack` for processing to the working threads. It is checked that in one instance of the rental service, this method is always called by one (the same) thread.
- `solvedPack` for returning the solved instance of `CProblemPack`. The parameter is the solved instance of the problem pack previously obtained from the `waitForPack` call. Since returning can take a long time, you must create a dedicated thread for returning. The thread will receive solved instances of problems from the working threads and call the `solvedPack` method on them. Solved instances of problems must be returned in the same order they were received from `waitForPack`. The return thread must not perform any computationally intensive tasks. It is checked that in one instance of the rental service, this method is always called by one (the same) thread.

`COptimizer` is a class encapsulating the entire computation. You will create this class, but you must adhere to the following public interface:
- A parameterless constructor initializes a new instance of the class. It does not create any threads yet.
- The method `addCompany(x)` registers the rental service x.
- The method `start(workThr)` creates communication threads for all registered rental services and starts `workThr` working threads. After starting the threads, the `start` method returns to the caller.
- The method `stop` waits for the completion of all rental service handling and the termination of the created threads. After this, the `stop` call returns to the caller.
- The method `usingProgtestSolver()` returns true if you use the provided `CProgtestSolver` solver for the actual computation or false if you implement the entire computation with your own code. If this method returns true, the test environment does not use the `COptimizer::checkAlgorithm(problem)` method below (you can leave it empty). If the method returns false, the test environment adjusts the behavior of the provided solver `CProgtestSolver` so that it deliberately fills in incorrect results.
- The class method `checkAlgorithm(problem)` is used to test the correctness of your computation algorithm. The parameter is one instance of `CProblem`. The method code will ensure the necessary calculations and fill in the `m_MaxProfit` field in the `problem` instance. Besides checking the correctness of the implemented algorithms, the method is used to calibrate the speed of your solution. The speed will adapt to the size of the given problems to make the testing take a reasonable amount of time. Implement the method only if you do not use the provided solver `CProgtestSolver` (that is, if your method `COptimizer::usingProgtestSolver()` returns false).

`CProgtestSolver` is a class providing an interface for sequentially solving the given problems. The author decided to make its use a bit more interesting, so the behavior of this class is somewhat whimsical. `CProgtestSolver` is an abstract class, and its implementation is realized by a subclass. Instances of `CProgtestSolver` are created using the factory function `createProgtestSolver()`. Additionally, `CProgtestSolver` instances solve the given problems in batches. Each `CProgtestSolver` instance has a specified capacity indicating how many instances of problems it can accommodate at most. `CProgtestSolver` instances are used only once. If more problem instances need to be solved, another instance of `CProgtestSolver` must be created. The class has the following interface:
- `hasFreeCapacity()` method returns true if another problem instance can be added to the instance, false if the instance is completely full.
- `addProblem(x)` method adds the problem instance x to be solved. The return value is true if the problem was added, false if not (because the solver's

`CProgtestSolver` is a class providing an interface for sequentially solving given problems. Its author decided to make its use a bit more interesting, so the behavior of this class is somewhat whimsical. `CProgtestSolver` is an abstract class, and its implementation is realized by a subclass. Instances of `CProgtestSolver` are created using the factory function `createProgtestSolver()`. Additionally, `CProgtestSolver` instances solve the given problems in batches. Each `CProgtestSolver` instance has a specified capacity indicating how many instances of problems it can accommodate at most. `CProgtestSolver` instances are used only once. If more problem instances need to be solved, another instance of `CProgtestSolver` must be created. The class has the following interface:
- `hasFreeCapacity()` method returns true if another problem instance can be added to the instance, false if the instance is completely full.
- `addProblem(x)` method adds the problem instance x to be solved. The return value is true if the problem was added, false if not (because the solver's capacity has been reached). After adding the problem, it is reasonable to check whether the solver's capacity is full (`hasFreeCapacity`). If the capacity is utilized, start the computation (`solve`).
- `solve()` method starts the computation. The result is determined for the inserted instances and placed in the corresponding fields `m_MaxProfit`. The method does nothing else, especially not informing the rental services about the solution of the given problems (does not call `CCompany::solvedPack`). Further processing of solved problems is entirely your responsibility. The `solve` method can be called only once for the given `CProgtestSolver` instance; additional attempts will result in an error. The method returns the number of solved problem instances; a return value of 0 typically indicates an error (repeated attempts to call the method).

Instances of `CProgtestSolver` do not allow more problems to be inserted than their capacity. On the other hand, the `solve` method can be called at any time (but only once for the given instance). However, do not attempt to misuse the solver and solve problems only one at a time:

- The test environment creates instances of `CProgtestSolver` so that the sum of their capacities M covers the total number of given problems N in the test (typically M is slightly larger than N).
- If you use each instance of `CProgtestSolver` to solve only one problem instance, you will soon exhaust the capacity M and will not have a way to solve further given problems.
- If you exceed the capacity M, calls to `createProgtestSolver()` will return useless solver instances (depending on the mood, an empty smart pointer will be returned, the solver will have a capacity of 0, or will deliberately fill in incorrect results).
- Therefore, it is important to use the solver's capacity to its fullest.
- The capacities of individual solver instances are chosen randomly. As mentioned, this class tries to make the programmer's work more interesting.

Furthermore, the solver provided by the test environment is available only in mandatory and optional tests (not in bonus tests). If you try to use it in a bonus test, the factory function `createProgtestSolver()` will return empty instances, instances with zero capacity, or instances calculating incorrect results.
