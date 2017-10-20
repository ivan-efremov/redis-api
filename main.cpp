#include <iostream>
#include <thread>
#include <csignal>
#include "Counter.h"
#include "Trigger.h"




void testAddCounter(Counter& a_counter)
{
    std::cout << "testAddCounter..." << std::endl;
    size_t n = 0UL, i = 0UL;
    time_t tstart = time(NULL);
    while(i < 10) {
	a_counter.addEvent(
	    time(NULL),
	    "310",
	    "0",
	    "f910cbf0-d864-47eb-b668-91e7cdd16b8a",
	    std::make_shared<ListGroupByGroupByValue>(
		1,
		GroupByGroupByValue("user.name", "ivan")
	    )
	);
	++n;
	if((time(NULL) - tstart) >= 60) {
	    std::cout << n << std::endl;
	    ++i;
	    n = 0UL;
	    tstart = time(NULL);
	}
    }
}

void testGetCounter(Counter& a_counter)
{
    std::cout << "testGetCounter..." << std::endl;
    size_t n = 0UL, i = 0UL;
    time_t tstart = time(NULL);
    while(i < 10) {
	size_t k = a_counter.getCounter(
	    "310",
	    "0",
	    1000,
	    std::make_shared<ListGroupByGroupByValue>(
		1,
		GroupByGroupByValue("user.name", "ivan")
	    )
	);
	++n;
	if((time(NULL) - tstart) >= 60) {
	    std::cout << n << std::endl;
	    ++i;
	    n = 0UL;
	    tstart = time(NULL);
	}
    }
}

void testSetTrigger(Trigger& a_trigger)
{
    std::cout << "testSetTrigger..." << std::endl;
    size_t n = 0UL, i = 0UL;
    time_t tstart = time(NULL);
    while(i < 10) {
	a_trigger.setState("310|0|user.name|ivan");
	++n;
	if((time(NULL) - tstart) >= 60) {
	    std::cout << n << std::endl;
	    ++i;
	    n = 0UL;
	    tstart = time(NULL);
	}
    }
}

void testGetTrigger(Trigger& a_trigger)
{
    std::cout << "testGetTrigger..." << std::endl;
    size_t n = 0UL, i = 0UL;
    time_t tstart = time(NULL);
    while(i < 10) {
	bool s = a_trigger.getState("310|0|user.name|ivan");
	++n;
	if((time(NULL) - tstart) >= 60) {
	    std::cout << n << std::endl;
	    ++i;
	    n = 0UL;
	    tstart = time(NULL);
	}
    }
}

void testGetTimeTrigger(Trigger& a_trigger)
{
    std::cout << "testGetTimeTrigger..." << std::endl;
    size_t n = 0UL, i = 0UL;
    time_t tstart = time(NULL);
    while(i < 10) {
	time_t t = a_trigger.getTime("310|0|user.name|ivan");
	++n;
	if((time(NULL) - tstart) >= 60) {
	    std::cout << n << std::endl;
	    ++i;
	    n = 0UL;
	    tstart = time(NULL);
	}
    }
}



int main(int argc, const char *argv[])
{
    Counter counter("counter", "127.0.0.1", "6379");
    Trigger trigger("trigger");
    testAddCounter(counter);
    testGetCounter(counter);
    testSetTrigger(trigger);
    testGetTrigger(trigger);
    testGetTimeTrigger(trigger);
    return 0;
}
