#include <iostream>
#include <memory>

class Test {
public:
	Test(int a) {
		m_a = a;
		std::cout << "Test Construct\n";
	}
	virtual ~Test() = default;
	Test(const Test& t) {
		m_a = t.m_a;
		std::cout << "Test Copy Construct\n";
	}
	Test& operator=(Test&) = default;
	int m_a;
};

class TestChild : public Test {
	
};

int main()
{
	std::shared_ptr<Test> t1(new Test(1));//这种是对的。
	//std::shared_ptr<Test> t1 = new Test(1);//这种会编译错误。
	std::cout << "m_a: " << t1->m_a << std::endl;
	
}



