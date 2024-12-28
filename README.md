# Dining-Philosophers-
A simulation of the common multi-threading pardox

The Dining Philosophers a common problem when considering multithreading and its problems. The basic idea is that there are 5 philosophers and 5 forks (eating utensils). At any given moment a philosopher is either thinking, eating or hungry. If they are hungry, they need to pick both forks from either side. A problem arises when adjacent philosophers are hungry and/or eating (mutually exclusive) since there would be a shortage in the required 2 forks. Thus, the philosopher needs to wait until the resource is released. Moreover, a group of three philosophers share any of the 5 forks so there might be contention over who gets the resource. This is known as a race condition. A mutex may be used to lock and unlock the resource and avoid the race condition when acquiring the resource. Another problem arises if one of the philosophers does not get to eat causing starvation. To avoid this issue a queue on each fork may be used, ensuring that the forks are acquired on a first-come first serve bases. Lastly, the process might stall completely if each philosopher has one fork (deadlock). To avoid this issue even numbered philosophers will check the fork with the same ID then the one after it. On the other hand, odd numbered philosophers will check the fork of the following ID before the similar one. By doing this common multithreading issues are mitigated. Thus, the final code should address the following:
1. Memory Leaks
2. Starvation
3. Race Conditions
4. Deadlocks
