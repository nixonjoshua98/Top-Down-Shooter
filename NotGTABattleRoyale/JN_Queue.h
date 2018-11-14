#ifndef JN_QUEUE_H
#define JN_QUEUE_H

#include <string>
#include <vector>

class JN_Queue
{
public:
	///<summary>Default constructor</summary>
	JN_Queue();


	///<summary>Grab next item in the queue</summary>
	std::string Next();


	///<summary>Add an item to the structure</summary>
	void Add(std::string s);


	///<summary>Get the length of the container</summary>
	int Size();

private:
	std::vector<std::string> container;	// Internal container
};

#endif // !JN_QUEUE_H