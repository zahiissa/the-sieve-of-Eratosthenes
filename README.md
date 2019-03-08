# the-Concurrent-sieve-of-Eratosthenes
sieve-of-Eratosthenes is an ancient algorithm for finding all prime numbers up to any given limit 

# about input/output
The application executable called “main”. The program accept two parameters:
N and T, where N is the upper bound for prime searching and T is the number of threads. The
program creates T + 1 log files named:
primes.log contains a sorted list of the detected prime numbers, with each number appearing on a separate line.

thread-1.log containa the output of thread 1. The output lists the prime candidate,
and the numbers that were removed by the thread as a result of using that prime candidate.

thread-2.log Similar output for thread 2
... etc.

