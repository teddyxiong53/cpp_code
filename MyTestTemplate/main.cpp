#include <iostream>
#include <memory>
#include "Executor.h"
#include <chrono>
#include <unistd.h>

std::chrono::milliseconds SHORT_DELAY = std::chrono::milliseconds(50);
std::chrono::seconds LONG_DELAY = std::chrono::seconds(5);

class Test {
public:
	Test() {
		std::cout << "Test no param Construct\n";
	}
	Test(int a) : m_a(a){
		//m_a = a;
		std::cout << "Test param Construct\n";
	}
	virtual ~Test() = default;
	Test(const Test& t) {
		m_a = t.m_a;
		std::cout << "Test Copy Construct\n";
	}
	Test& operator=(Test&) = default;
	int m_a;
	Executor m_executor;
};

class TestChild : public Test {
public:
	TestChild(int a) {
		std::cout << "TestChild param Construct\n";
		
	}
};


int main()
{
	auto t1 = std::shared_ptr<Test>(new Test(1));
	std::function<void()> func = []() {
		std::cout << "future func \n";
	};
	
	auto future = t1->m_executor.submit(func);
	std::cout << "before wait\n";
	auto future_status = future.wait_for(SHORT_DELAY);
	std::cout << "after wait\n";
	if(future_status == std::future_status::ready) {
		std::cout << "wait ready ok\n"; 
	}

	while(1) {
		usleep(1000*1000);
	}
}



