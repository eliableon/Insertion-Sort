// Insertion Sort.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


//Pre-Processor
#include <iostream>
#include <vector>


// Function to perform insertion sort
//setting up the function with a vector of integers as a parameter to sort the array.
void insertionSort(std::vector<int> & arr) {
	//Iterating through the array starting from the second element
    for (size_t i = 1; i < arr.size(); ++i) {
		//Storing the current element in a variable called key
        int key = arr[i];
		//Initializing a variable j to keep track of the index of the previous element
        int j = i - 1;

		//Moving elements of arr[0..i-1], that are greater than key, to one position ahead of their current position
        while (j >= 0 && arr[j] > key) {
			//Shifting the elements to the right to make space for the key
            arr[j + 1] = arr[j];
			//Decrementing j to continue checking the previous elements
            j = j - 1;
        }
		//Inserting the key at its correct position in the sorted part of the array
        arr[j + 1] = key;
    }
}

int main() {
    std::cout << "Hello World!\n";
	//Setting the array with some random numbers
    std::vector<int> data = { 12, 11, 13, 5, 6,2,7,8,5 };

    std::cout << "Given Array: ";
    for (int num : data) {
        std::cout << num << " ";
    }
    //Calls the function to sort the array using the insertion sort algorithm.
    insertionSort(data);

    std::cout << "\nSorted array: ";
    for (int num : data) {
        std::cout << num << " ";
    }
    
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
