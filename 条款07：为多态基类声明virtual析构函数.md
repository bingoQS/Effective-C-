[TOC]

## 条款02：尽量以const, enum, inline, 替换 #define

​                               **Prefer const, enum, and inline to #defines.**



## 条款03：尽可能多使用const

​                              **Use const whenever possible**

## 条款04：确定对象被使用前已先被初始化

​                                **Make sure that objects are initialized before thay’are used.**

 

## 条款05：了解C++默默编写并调用那些函数

​                               **Know what functions C++ silently writes and calls.**



## 条款06：若不想使用编译器自动生成的函数，就应该明确拒绝

​                             **Explicitly disallow the use of compiler-generated functions you do not want.**

如果你不希望有 copying 构造函数和 assignment 操作符，那么可以将它们设为 private，**并且只进行声明不进行定义**（防止 friend 函数和 member 访问它们）。

```c++
class Uncopyable {
protected:
	Uncopyable() {}
	~Uncopyable() {}

private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);
};
```



将 Uncopyable 作为一个 base 类，private 继承

```c++
class HomeForSale :private Uncopyable {
public:
	HomeForSale() {}
	HomeForSale(const HomeForSale&);
};
```



### 总结：

- n 为驳回编译器自动（暗自）提供的机能，可将相应的成员函数*声明为**private**并且不予实现*。使用像*Uncopyable**这样的base class**也是一种做法*。



## 条款07：为多态基类声明virtual析构函数

​                             **Declare destructors virtual in polymorphic base class.**

### 总结：

- ***polymorphic（带多态性质的）base class***应该声明一个virtual析构函数。***如果class带有任何virtual函数***，它就应该有一个virtual析构函数。
- ***CLasses的设计目的如果不是为了base classes 使用，或不是为了具备多态性（polymorphically），就不应该声明为virtual函数。***



## 条款08：别让异常逃离析构函数

​                                **Prevent exceptions from leaving destructors**

**C++允许析构函数抛出异常，但是它不鼓励你这么做。**

举个例子：

```c++
class Widget {
public:
    ...
	~Widget(){...}           //可能抛出异常
};

void doSomething()
{
	std::vector<Widget> v;
    ...
}                            //v在这里被撤销毁
```

一个 Widget 类，它的析构函数可能会抛出异常。假设v有10个元素，当执行完 doSomething() 函数时，将调用 Widget 的析构函数进行释放，如果调用第一个元素的析构函数发生异常，再调用第二个元素的析构函数发生异常，那么对于C++程序而言两个异常同时存在时就会崩溃（**结束执行或导致不明确行为**），***导致其余的 Widget 元素没有释放造成资源泄露***。

### 总结：

- 析构函数绝对不要抛出异常。如果一个被析构函数调用的函数可能抛出异常，析构函数应该捕捉任何异常，然后吞下它们或结束程序。
- 如果客户需要对某个操作函数抛出的异常做出反应，那么 class 应该提供一个普通函数（而非在析构函数中）执行该操作。

## 条款09： 绝不在构造和析构析构过程中调用virtual函数

​                                      **Never call virtual functions during constructors or destructors.**

情景：base 类 A ，其有一个 vrtual 函数，并且 A 的构造函数调用了这个 virtual 函数，A 的  derived 类 AA。

```c++
class A{
public:
      A(){
          test();
      }   
      
      virtual test() const {}
};

class AA{
public:
    B(){}
    
    virtual test() const{}
};

AA aa;
```

创建一个 AA 对象 aa 时，***先调用的是 base 类 A 的构造函数***，A 的构造函数执行时调用到了 virtual 函数 test() ，而这时候调用的 test() 版本时 base 类 A 的，不是 derived 类 AA 的，***导致多态实现失败***。

在 derived class 对象的 base class 构造期间，对象的类型是 base class 而不是 derived class 。

***同样的道理也适用于析构函数。***

## 总结：

- 在构造和析构期间不要调用 virtual 函数，因为这类调用从不降至 dervied class 。

## 条款10：令operator= 返回一个reference to *this

**Have assignment operators return a reference to *this.**

关于赋值，连锁形式，并且是有结合律。如：

```c++
int x, y, z;
x = y = z = 15;
x = (y = (z = 15) );
```



为了实现 “连锁赋值” ，赋值操作符必须返回一个 reference 指向操作符的左侧实参，这是你为 classes 实现赋值操作符时应该遵循的协议：



```c++
class Widget {
public:

	Widget(){}
	Widget& operator=(const Widget& rhs)
	{
		std::cout << "operator =执行了"<<std::endl;
		return* this;
	}
};
```

 

注意，这只是一个协议，并无强制性。

**请记住**

- 令赋值操作符（assignment）返回一个reference to *this。



## 条款11：在 operator= 中处理 “自我赋值”

​                                   **Handle assignment to self in operator**

“自我赋值”。

```c++
class Widget{ ... };
Widget w;
w = w;

//隐蔽比一点的“自我赋值”
a[i] = a[j];       //如果 i 和 j 相等
*px = *py;         //如果 px 和 py 指向同一个地址
```



容易出现别名的情况有：指针、引用、继承时创建的对象

```c++
class Base{ ... };
class Derived: public Base{ ... };
void doSomething(const Base& rb,Derived* pd);   //rb 和 *pd 有可能是同一个对象
```



## 条款12：复制对象时勿忘其每一个成分

​                             **Copy all parts of an object.

copy构造函数和 copy assignment 操作符负责复制对象。**当你声明自己的 copying 函数，编译器仿佛被被冒犯到似的，当你的实现代码几乎必然出错时却不告诉你**。

一个例子

```c++
class Data
{
public:
	Data(){}
	~Data(){}
private:
};

void logCall(const std::string& funcName) {
	std::cout << funcName << std::endl;
}

class Customers
{
public:
	Customers(const Customers& rhs);
	~Customers(){}
	Customers& operator=(const Customers& rhs);

private:
	std::string name;
	Data lastTransaction;
};

Customers::Customers(const Customers& rhs):name(rhs.name)
{
	logCall("Customer copy constructor");
}

Customers& Customers::operator=(const Customers& rhs)
{
	logCall("Customer copy assignment constructor");
	name = rhs.name;
	return *this;
}
```

此时只复制了 name，没有复制 lastTransaction。大部分编译器不会对该代码做出反应（警告）。



**当发生继承时，可能回造成非法操作。**

```c++
class Customers{};  //同上

class PriorityCustomer: public Customers 
{
public:
	PriorityCustomer(const PriorityCustomer& rhs):priority(rhs.priority){
		logCall("PriorityCustomer copy constructor");
	}

	PriorityCustomer& operator=(const PriorityCustomer& rhs) {
		logCall("PriorityCustomer copy assignment constructor");
		priority = rhs.priority;
		return *this;
	}
private:
	int priority;
};
```

PriorityCustomer 的copying 函数看起来好像复制了 PriorityCustomer 内的每一样东西，但是 PriorityCustomer 继承的 Customer 成员变量没有复制。**由于 Customer 只有copying 构造函数而没有默认构造函数，PriorityCustomer 会去调用他的默认构造函数，从而导致编译不通过。**

正确写法如下。

```c++
class PriorityCustomer: public Customers 
{
public:
	PriorityCustomer(const PriorityCustomer& rhs):Customers(rhs),priority(rhs.priority){
		logCall("PriorityCustomer copy constructor");
	}

	PriorityCustomer& operator=(const PriorityCustomer& rhs) {
		logCall("PriorityCustomer copy assignment constructor");
		Customers::operator=(rhs);
		priority = rhs.priority;
		return *this;
	}
private:
	int priority;
};
```



###  总结

- Copying 函数应该确保复制 “对象内的所有成员变量” 及 “所有 base class 成分”。
- 不要尝试以某个 copying 函数实现另一个 copying 函数。应该将共同技能放进第三个函数中，并由两个 copying 函数共同调用。



# 资源管理

所谓资源就是，一旦用了它，将来必须还给系统，否则将造成资源泄露。常见的资源有：动态分配内存、互斥锁（mutwx locks）、数据库连接、网络 sockets 等。

下面将介绍建立在构造函数、析构函数和 copying 函数的妓院管理方法。



## 条款13：以对象管理资源

**Use objects to manage resources**

- 为防止资源泄露，使用只能指针，它们能够在构造函数中获得资源并在析构函数中释放资源。

```c++
class Investment {

};

Investment* createInvestment();

void f()
{
	Investment* Pinv = createInvestment();
	delete Pinv;
}

void f()
{
	std::auto_ptr<Investment> pInv(createInvestment());
}
```



## 条款14：在资源管理类中小心 copying 行为

**Think carefuly about coppy behavior in resource-managing classes**

### 总结：

**资源取得时机便是初始化时机**（**Resource Acquisition Is Initializtion; RAII**）

- 复制 RAII 对象必须一并复制它所管理的资源，

## 条款15：在资源管理类中提供对原始数据资源的访问

**Provide access to raw resources in resource-managing classes.**

### 总结：

- APIs 往往要求访问原始资源（raw resources），所以每一个 RAII class 应该提供一个 “ 取得其所管理资源的方法”。
- 对原始资源的访问可能经由显示转换或隐士转换。一般而言显示转换比较安全，单隐士转换对客户比较方便。

```c++
class FontHandle{
};

FontHandle getFont() {}

void releaseFont(FontHandle fh) {}

class Font
{
public:
	explicit Font(FontHandle fh):f(fh){}  //获取资源
	~Font() {
		releaseFont(f);  //释放资源
	}
	FontHandle get() const { return f; }   //显示转换函数
	operator FontHandle() const { return f; } //隐士转换函数,不需要返回类型
protected:
private:
	FontHandle f;      //原始资源
};

void changeFontSize(FontHandle f, int newSize){}
```

## 条款16：成对使用 new 和 delete 时要采取相同形式

**use the same form in corresponding of new and delete. **

### 总结：

如果你在 new 表达式中使用 []，必须在相应的 delete 表达式中也使用 [] 。如果你在 new 表达式中不使用 [] ，一定不要在相应的 delete 表达式中是哦那个 []。

## 条款17：以独立语句将 newed 对象置入智能指针

**Store newed objects in smart pointers in standalone statements.**

- 以独立语句将 newed 对象存储于（置入）智能指针内。如果不这样做，一旦异常被抛出，有可能导致难以察觉的资源泄露。

# 设计与声明

## 条款18：让接口容易被正确使用，不易被误使用

**Make interfaces easy to use correctly and hard to use incorrectly**

C++在接口之海漂浮，function接口、class接口、template接口 ... ... ，每一种接口都是客户与你的代码互动的手段。它们才想要正确的使用你的接口。这种情况下如果他们对任何其中一个接口的用法不正确，你至少也得付一部分责任。**理想上，如果客户企图使用某个接口而却没有获得他所预期的行为，这个代码不该编译通过；入轨哦代码通过了编译，塔萨的作为就是客户想要的。**

与开发一个“容易被正确使用，不容易被误用”的接口，首先必须考虑客户可能做出是那么杨的错误。假设开发一个表现日期的class设计构造函数：

```c++
class Date {
public:
	Date(const int month,const int day,const int year):_month(month),_day(day),_year(year){}

private:
	int _month;
	int _day;
	int _year;
};

```

乍一看这个接口很合理，但他的客户很容易犯下至少两个错误。

​        1、错误的次序传递参数。

```c++
Date d（30，3，1995);
```

​        2、可能传递一个无效的月份或天数。

```c++
Date d（2，30，1995);
```

**许多客户错误可以导入新类型而获得预防。**带入简单的外覆类型来区别天数、月份和年月，然后与 Date 构造函数中使用这些类型。

```c++
struct Day {
	explicit Day(int d) :val(d) {}
	int val;
};

class Month {
public:
	explicit Month(int d) :val(d) {}
	int val;
};

struct Year {
	explicit Year(int y) :val(y) {}
	int val;
};

class Date {
public:
	Date(const Month& month,const Day& day,const Year& year):_month(month),_day(day),_year(year){}

private:
	Month _month;
	Day _day;
	Year _year;
};
Date d1(30, 3, 1995); //错误
Date d2(Day(30), Month(3), Year(1995));//错误
Date d3(Month(3), Day(30), Year(1995));
```

**一旦正确的类型就定位，限制其值有时候时通情达理的。**

```c++
class Month {
public:
	static Month Jan() { return Month(1); } //函数，返回有效月份
	static Month Feb() { return Month(2); }
	static Month Dec() { return Month(12); }
	
private:
	explicit Month(int m) :val(m) {}  //声明为私有，阻止生成新的月份
	int val;
};
```

**“除非有好理由，否则应该尽量令你的 types 的行为与内置类型一致”。**可以拿 int 做范本。



## 总结：

- 好的接口很容易被正确使用，不容易比误用。你应该在你的所有接口中努力达成这些性质。
- “促进正确使用”的办法包括接口的一致性，以及与内置类型的行为兼容。
- “阻止误用”的办法包括建立新类型、限制类型上的操作，束缚对象值，以及消除客户的资源管理责任。
- shared_ptr 支持定制型删除器。这颗防范DLL问题，可被用来自动解除互斥锁（mutex）等等。



## 条款19：设计 class 犹如设计 type

**Treat class design as type design.**

### 设计规范



## 条款20：宁以 pass-by-reference-to-const 替换 pass-by-value

**Prefer pass-by-reference-to-const to pass-by-value.**

缺省情况下C++以 by value 方式传递至函数。除非你另外指定，**否则函数参数都是以实际实参的复件为初值，而调用端所获得的亦是函数返回值的一个复件**。这些复件由对象的 copy 构造函数产出，这可能使得 **pass-by-value 成为昂贵的（费时的）**操作。

一个例子：

```c++
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
```

执行一下操作

```c++
bool validateStudent(const Student& s);
Student plato;
bool platoIsOk = validateStudent(plato);
```

调用 validateStudent(const Student& s) 函数发生了哪些过程？

对函数而言，copy 构造函数被调用一次，当函数结束返回 s 将被销毁，即参数的传递成本是“一次 Student Copy 构造函数调用，加上一次 Student 构造函数调用”。

表面上确实如此，但不完整。Student 有两个 string 对象，所以每次构造一个 Student 就会构造两个 string。此外还继承了 Person，所以每次构造一个 Student 就会构造一个 Person，每构造一次 Person 又会构造两次 string。

故总成本是“六次构造函数和六次析构函数”！







































