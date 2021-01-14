#include<iostream>
#include<string>
#include<list>
using namespace std;

//class SccessLevels {
//public:
//	int getReadOnly(){ return reasOnly; }
//	void setReadWrite(int value) { readWrite = value; }
//	int getReadWrite()const { return readWrite; }
//	void setWriteOnly(int value) { writeOnly = value; }
//
//private:
//	int noAccess;
//	int reasOnly;
//	int readWrite;
//	int writeOnly;
//};

class SpeedDataCollection {
public:
	SpeedDataCollection() :m_speed(0), m_averageSpeed(0) {}
	void addValue(int speed) { m_speed = speed; }     //添加一笔新数据
	void computeAverage() {
		m_averageSpeed = m_sum / m_count;
	}
	double averageSoFar() const { return m_averageSpeed; }  //返回平均速度

	//double averageSoFar()  { 
	//	m_averageSpeed = (m_averageSpeed+m_speed)/2;
	//}

private:
	double m_speed;
	double m_averageSpeed;
	static double m_sum;
	static int m_count;
};

int SpeedDataCollection::m_count = 0;
double SpeedDataCollection::m_sum = 0;

int main()
{
	return 0;	
}