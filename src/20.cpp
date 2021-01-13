#include<iostream>
#include<string>

using namespace std;

class Person {
public:
	Person(){}
	Person(string name,string address):m_name(name),m_address(address) {

	}
	virtual ~Person() {

	}

private:
	string m_name;
	string m_address;
};

class Student :public Person {
public:
	Student(){}
	Student(string schoolName,string schoolAddress):m_schoolName(schoolName),m_schoolAddress(schoolAddress) {

	}
	~Student() {

	}

private:
	string m_schoolName;
	string m_schoolAddress;
};

bool validateStudent(const Student& s);

class Window {
public:
	string name() const;               //返回窗口名称
	virtual void display() const;      //显示窗口和其内容
};

class WindowWithScrollBars :public Window {
public:
	virtual void display() const;
};

int main()
{
	Student plato;
	bool platoIsOk = validateStudent(plato);
	return 0;	
}