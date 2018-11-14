#include "stdafx.h"

#include "JN_Queue.h"


// Default constructor
JN_Queue::JN_Queue()
{
	container = {};
}


// Add an item to the structure
void JN_Queue::Add(std::string s)
{
	container.push_back(s);
}


// Grab next item in the queue
std::string JN_Queue::Next()
{
	std::string r;

	if (container.size() > 0)
	{
		r = container.front();
		container.erase(container.begin());
	}
	else
		r = "0";

	return r;
}


// Get the length of the container
int JN_Queue::Size()
{
	return (int)container.size();
}