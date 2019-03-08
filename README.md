# the-Concurrent-sieve-of-Eratosthenes
sieve-of-Eratosthenes is an ancient algorithm for finding all prime numbers up to any given limit 

# Interface
The application executable called “main”. The program accept two parameters:
N and T, where N is the upper bound for prime searching and T is the number of threads. The
program creates T + 1 log files named:
primes.log contains a sorted list of the detected prime numbers, with each number appearing on a separate line.

thread-1.log containa the output of thread 1. The output lists the prime candidate,
and the numbers that were removed by the thread as a result of using that prime candidate.

thread-2.log Similar output for thread 1
... etc.


# Concurrent
multiple threads traverse the list and trying to remove non-primes from it, with each thread running the exact same algorithm.
By using synchronization, each element removed once. 
each thread eliminates numbers that divide by a different prime. 
the algorithm use a simple heuristic to decide whether the prime candidate that a thread is
looking at is already being handled by a different thread, based on the following observations:
Observation 1 When using hand-over-hand locking , let
T1 be a thread that uses a prime candidate p1 and let T2 be a thread that uses a prime candidate
p2. Then, if p1 < p2 then any element currently locked by T2 is smaller than any element locked
by T1.
Observation 2 Let p be a prime candidate. Then p2 can only be removed by a thread that used
p as its prime candidate.
Observation 3 Let p be a prime candidate, then according to the concurrent version of the
algorithm p is a prime number (it is clearly a prime number in the sequential version of the
algorithm).
That is, if a thread selected p as its prime candidate according to the algorithm above, and it
passes the point where it should have seen p2 but did not detect it, it means that a different thread
must have removed p2 and therefore is already taking care of removing numbers that divide by p,
and thus the thread moves on to the next prime candidate.

# Synchronization considerations
Since multiple threads are accessing the list, searching through it and removing elements,the algorithm uses fine-grained synchronization.




