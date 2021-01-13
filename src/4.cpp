#include<iostream>
#include<string>
#include<list>
using namespace std;

class PhoneNumber{};
class ABEntry {
public:
	ABEntry(const std::string& name, const std::string& address, const std::list<PhoneNumber>& phones)
	{
		//这些都是赋值（assignments）而非初始化（initializations）。
		//不是最佳做法。
		theName = name;
		theAddress = address;
		thePhones = phones;
		numTimesConsulted = 0;
	}

private:
	std::string theName;
	std::string theAddress;
	std::list<PhoneNumber> thePhones;
	int numTimesConsulted;
};

int main()
{
	return 0;	
}