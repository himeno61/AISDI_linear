#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

#include "Vector.h"
#include "LinkedList.h"
#include <time.h>
#define REPEAT_TIMES 40


namespace
{


void int_Test1()
{
	int NUM_ELEMENTS = 1000;
	aisdi::Vector<int> vect ;
    aisdi::LinkedList<int> list ;
	clock_t time_1,time_2;
	//float tab [REPEAT_TIMES][2];
	std::ofstream outputFile;
	outputFile.open("test_times.txt");
	outputFile << "Numer\tWektor\t\tLista" <<std::endl;
    for (size_t j=0;j <REPEAT_TIMES;j++)
    {
		outputFile<<NUM_ELEMENTS<<"\t";
        time_1=clock();
        for (int i=0;i<NUM_ELEMENTS;i++)
        {
            vect.append(i);
        }
        time_2=clock();
        outputFile<<(float)(time_2-time_1)/CLOCKS_PER_SEC;
		outputFile<<"\t\t";
        time_1=clock();
        for (int i=0;i<NUM_ELEMENTS;i++)
        {
            list.append(i);
        }
        time_2=clock();
        outputFile<< (float)(time_2-time_1)/CLOCKS_PER_SEC<<std::endl;
        NUM_ELEMENTS = NUM_ELEMENTS*2;
    }
    //std::ofstream outputFile;
	//outputFile.open("test_times.txt");
	//outputFile << "Wektor 			Lista" <<std::endl;
	outputFile.close();
}

	
	
} // namespace

int main()
{
  int_Test1();
  return 0;
}
