/*
 Name: Anthony Rutherford
 Email: adrutherford@crimson.ua.edu
 Course Section: Fall 2023 CS 201
 Homework #: 1
 Instructions to Compile: g++ heapsort_quicksort.cpp
 Instructions to Execute: ./a.exe 
*/
#include <iostream> 
#include <array>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <chrono>

using namespace std;

template< class RandomIt >
constexpr void swapVals(RandomIt a, RandomIt b) { //Take in itterators and swap their values
	auto hold = *a;
    *a = *b;
    *b = hold;
}

template <class RandomIt>
constexpr void heapify(RandomIt first, RandomIt last, int index) {
    auto size = std::distance(first, last);
    auto largestLocation = index;
    auto left = (2 * index) + 1;
    auto right = (2 * index) + 2;

    if (left < size && *(first + left) > *(first + largestLocation))  //if leftChild is greater than parent
        largestLocation = left;

    if (right < size && *(first + right) > *(first + largestLocation))  //if rightChild is greater than parent
        largestLocation = right;

    if (largestLocation != index) {                            //if parent is not greatest swap and reorder
        swapVals(first + index, first + largestLocation);
        heapify(first, last, largestLocation);
    }
}

template <class RandomIt>
constexpr void heapsort(RandomIt first, RandomIt last) {
    auto size = std::distance(first, last);

    for (auto i = size / 2 - 1; i >= 0; i--){      // Consturct maxHeap
        heapify(first, last, i);
    }

    for (auto i = size - 1; i > 0; i--) {      // Pull elements from heap then reorder
        swapVals(first, first + i);
        heapify(first, first + i, 0);
    }
}

template <class RandomIt>
constexpr RandomIt partition(RandomIt first, RandomIt last) {   //Method from youtube video animation (works)
    auto size = std::distance(first, last);
    auto pivotDist = size / 2;
    auto pivotValue = *(first + pivotDist); //Pivot equal to middle element

    swapVals((first + pivotDist), (last - 1)); // Move the pivot to the end
    auto pivot = first;

    for (auto j = first; j < last - 1; j++) {
        if (*j <= pivotValue) {           //Sorting values
            swapVals(pivot, j);
            pivot++;
        }
    }
    swapVals(pivot, (last - 1)); // Move the pivot back to its correct position
    return pivot;
}

template <class RandomIt>
constexpr void quicksort(RandomIt first, RandomIt last) {
    if (first < last) {
        auto pivot = partition(first, last);
        quicksort(first, pivot);       //recursive on first half
        quicksort(pivot + 1, last);    //recursive on back half
    }
}

int main(int argc, char *argv[]) {

	for (auto sort : std::array<void (*)(int *, int *), 3> {std::sort, heapsort, quicksort}) {
		std::chrono::duration<double> timetaken[3][3];
		std::cout << "Problem Size\tTime Taken (seconds)" << std::endl;
		std::cout << "\t\tCase 1\tCase 2\tCase 3" << std::endl;
		for (int size = 10; size <= 100000000; size *= 10) {
		
			int *a = new int[size];

			// repeat each case for three times 
			for (int i = 0; i < 3; i++) {
				std::generate(a, a+size, std::rand);

				/* Case 1: sorting random values */
				auto starttime = std::chrono::steady_clock::now();
				sort(a, a+size);
                auto endtime = std::chrono::steady_clock::now();
				timetaken[i][0] = endtime - starttime;

				/* Case 2: sorting data that is already sorted */
				starttime = std::chrono::steady_clock::now();
				sort(a, a+size);
				endtime = std::chrono::steady_clock::now();
				timetaken[i][1] = endtime - starttime;

				/* Case 3: sorting data that is in reverse sorted order */
				std::reverse(a, a+size);
				starttime = std::chrono::steady_clock::now();
				sort(a, a+size);
				endtime = std::chrono::steady_clock::now();
				timetaken[i][2] = endtime - starttime;
			}

			// print the average time of three runs
			std::cout << size << "\t\t";
			std::cout << (timetaken[0][0].count() + timetaken[1][0].count() + timetaken[2][0].count())/3.0 << "\t";
			std::cout << (timetaken[0][1].count() + timetaken[1][1].count() + timetaken[2][1].count())/3.0 << "\t";
			std::cout << (timetaken[0][2].count() + timetaken[1][2].count() + timetaken[2][2].count())/3.0 << std::endl;

			delete[] a;
		}
	}
}
