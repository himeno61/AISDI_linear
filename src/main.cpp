#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

#include "Vector.h"
#include "LinkedList.h"
#include <ctime>
#include <chrono>
#define REPEAT_TIMES 5


namespace
{
using timeValue = std::chrono::time_point<std::chrono::system_clock>;
using durationValue = std::chrono::duration<double>;

void int_Test_append()
{
	int NUM_ELEMENTS = 1000;
	std::ofstream outputFile;
	outputFile.open("test_times_append.txt");
	outputFile.setf(std::ios::scientific);
	outputFile << "Numer\tWektor\t\tLista" <<std::endl;
    for (size_t j=0;j <REPEAT_TIMES;j++)
    {
		aisdi::Vector<int>* vect = new aisdi::Vector<int>;
		aisdi::LinkedList<int>* list = new aisdi::LinkedList<int>;
		outputFile<<NUM_ELEMENTS<<"\t";
        timeValue start, stop;
        start = std::chrono::system_clock::now();
        for (int i=0;i<NUM_ELEMENTS;i++)
        {
            vect->append(i);
        }
        stop = std::chrono::system_clock::now();
        durationValue differ = stop - start;
        outputFile<<differ.count();
		outputFile<<"\t\t";
        start = std::chrono::system_clock::now();
        for (int i=0;i<NUM_ELEMENTS;i++)
        {
            list->append(i);
        }
        stop = std::chrono::system_clock::now();
		differ = stop - start;
        outputFile<<differ.count()<<std::endl;
        NUM_ELEMENTS = NUM_ELEMENTS*2;
        delete list;
        delete vect;
    }
  
	outputFile.close();
}


void int_Test_prepend()
{
	int NUM_ELEMENTS = 1000;
	std::ofstream outputFile;
	outputFile.open("test_times_prepend.txt");
	outputFile.setf(std::ios::scientific);
	outputFile << "Numer\tWektor\t\tLista" <<std::endl;
    for (size_t j=0;j <REPEAT_TIMES;j++)
    {
		aisdi::Vector<int>* vect = new aisdi::Vector<int>;
		aisdi::LinkedList<int>* list = new aisdi::LinkedList<int>;
		outputFile<<NUM_ELEMENTS<<"\t";
        timeValue start, stop;
        start = std::chrono::system_clock::now();
        for (int i=0;i<NUM_ELEMENTS;i++)
        {
            vect->prepend(i);
        }
        stop = std::chrono::system_clock::now();
        durationValue differ = stop - start;
        outputFile<<differ.count();
		outputFile<<"\t\t";
        start = std::chrono::system_clock::now();
        for (int i=0;i<NUM_ELEMENTS;i++)
        {
            list->prepend(i);
        }
        stop = std::chrono::system_clock::now();
		differ = stop - start;
        outputFile<<differ.count()<<std::endl;
        NUM_ELEMENTS = NUM_ELEMENTS*2;
        delete list;
        delete vect;
    }
  
	outputFile.close();
}

	
	
} // namespace

int main()
{
  int_Test_append();
  int_Test_prepend();
  return 0;
}
