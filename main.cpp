#include <iostream>
#include <thread>
#include <csignal>
#include "Counter.h"
#include "Trigger.h"



int main(int argc, const char *argv[])
{
    Counter counter("counter", "127.0.0.1", "6379");
    Trigger trigger("trigger", "127.0.0.1", "6379");
    
    return 0;
}
