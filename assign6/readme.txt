Name: Kyle Romero
UID: 204747283
Section: Lab 7
TA: Tan

Homework 6 - Multithreaded Performance

Initially, I had quite an issue just figuring out what I needed to
do in order to separate this program into threads. First, I looked at
the code for a bit. That didn't work too well and so I decided to start
with something simple. I altered the makefile to make sure I included
all of the necessary libraries for threading.

Then, I read some documentation about pthread_join and pthread_create.
I found out that I really needed some kind of function to feed to
these functions and so I decided to see what part of the main function
I might be able to separate into multiple threads.

I then quickly identified the piece of code that contains both of the for
loops as a potential area of optimization. I then separated this section into
its own functiona and was quickly able to start making sense of how to
convert this program into a multithreaded program. 

Most of the changes amounted to adding two for loops to create and join the
threads inside of the main function, altering how we store the values each
thread is creating, and removing computations from the main function into
another function that is threadable.

Performance Improvements

The results of the timings are shown below:
time ./srt 1-test.ppm >1-test.ppm.tmp
real	0m49.409s
user	0m49.400s
sys	0m0.002s

time ./srt 2-test.ppm >2-test.ppm.tmp
real	0m24.846s
user	0m49.405s
sys	0m0.003s

time ./srt 4-test.ppm >4-test.ppm.tmp
real	0m12.681s
user	0m50.111s
sys	0m0.002s

time ./srt 8-test.ppm >8-test.ppm.tmp
real	0m6.703s
user	0m52.023s
sys	0m0.003s

As the number of threads increases, the execution time decreases because
we are able to execute all of these tasks concurrently. This is good because
then our program takes less time to execute overall. In other words, we have
successfully parallelized the tasks.

Furthermore, we can see that the time decrease is roughly linear with the
number of threads that we are running concurrently on. This makes sense since,
for example, we should be able to run 8x as many computations on 8 threads than
on 1 and this results in 8x faster execution time.
