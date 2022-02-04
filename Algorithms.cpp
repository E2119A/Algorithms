#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iterator>
#include <time.h>
using namespace std;

const int smallSize     = 30;
const int mediumSize    = 100;
const int bigSize       = 65536;
const string rawDataFile        = "RawData.txt";
const string bigRawDataFile     = "bigRawData.txt";
const string sortedFile         = "SortedData.txt";

bool writeInFile(const string filename, bool sorted, vector<int> &vec)
{
    ofstream myfile;
    myfile.open(filename);
    if(myfile.is_open())
    {
        if(!sorted)
        {
            myfile<<"Unsorted"<<endl;
            for(int i = 0; i < vec.size(); i ++)
            {
                myfile << vec[i] << " " ;
            }
        }
        else
        {
            myfile<<"Sorted"<<endl;
            for(int i = 0; i < vec.size(); i ++)
            {
                myfile << vec[i] << " " ;
            }
        }
        return true;
    }
    else
        return false;
}

void openFileInNotepad(const string filename)
{
    string systemCall = "notepad \"" + filename + "\"";
    system(systemCall.c_str()); //This can throw an error

}

void printVector(vector<int> &vec)
{
    for(int i = 0; i<vec.size(); i++)
    {
        cout<<vec[i]<<" ";
    }
    cout<<endl;
}

void fillVector(string line, vector<int> &vec)
{
    stringstream line_stream( line );
    int i =0;
    int number;
    while ( line_stream >> number )
    {
        vec.push_back(number);
        i++;
    }
}

bool readFromFile(string filename, vector<int> &vec, int lineNumber)
{
    ifstream myfile;
    myfile.open(filename);
    int currLine = 0;
    string line;
    while(currLine != lineNumber)
    {
        if(!getline(myfile, line))
            return false;
        currLine++;
    }
    if(!getline(myfile, line))//now in line we have line with number lineNumber
        return false;
    fillVector(line, vec);
    return true;
}

void mergeSort(vector<int>::iterator left, vector<int>::iterator right)
{

    const int vecSize = right - left;
    if(vecSize > 1)
    {
        auto new_right = left + vecSize/2;
        auto new_left = new_right;
        mergeSort(left, new_right);
        mergeSort(new_left, right);

        auto leftIter = left, rightIter = new_left;
        vector<int> tmp_vec;
        auto tmpIter = tmp_vec.begin();

        while (leftIter < new_right || rightIter < right)
        {
            if ((*leftIter) < (*rightIter))
            {
                tmp_vec.push_back(*leftIter);
                leftIter++;
            }
            else
            {
                tmp_vec.push_back(*rightIter);
                rightIter++;
            }

            if (leftIter == new_right)
            {
                tmp_vec.insert(tmp_vec.end(),rightIter, right);
                break;
            }
            if (rightIter == right)
            {
                tmp_vec.insert(tmp_vec.end(),leftIter, new_right);
                break;
            }
        }
        move(tmp_vec.begin(),tmp_vec.end(), left);
    }


}

int partialSort(vector<int>& A, int left, int right)
{
    int pivot = A[right];
    int i = (left - 1);

    for(int j = left; j <= right - 1; j++)
    {
        if(A[j] < pivot)
        {
            i++;
            swap(A[i], A[j]);
        }
    }
    swap(A[i+1],A[right]);
    return (i + 1);
}


void quickSort(vector<int>& A, int left, int right)
{
    if(left<right)
    {
        int pi = partialSort(A, left, right);
        quickSort(A, left, pi - 1);
        quickSort(A, pi + 1, right);
    }
}

void heapify(vector<int>& a, int n, int i)
{
    int largest = i;
    int l = 2*i + 1;
    int r = 2*i + 2;

    if ((l < n) && (a[l] > a[largest]))
        largest = l;

    if ((r < n) && (a[r] > a[largest]))
        largest = r;

    if (largest != i)
    {
        swap(a[i], a[largest]);

        heapify(a, n, largest);
    }
}

void heapSort(vector<int>& a)
{
    int n = a.size();
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(a, n, i);

    for (int i = n - 1; i >= 0; i--)
    {
        swap(a[0], a[i]);

        heapify(a, i, 0);
    }
}

int main()
{
    double a;
    string fileName;
    int currentSize;
    vector<int> elements;
    int command;
    int lineNumber = 0;
    string lineNumberS;
    clock_t start, finish;

    command = -1;//here command states for vector generation type
    while(command != 1 && command != 2 && command != 0)
    {
        cout<<"Print 1 for random data, and 2 for preset data (print 0 to quit program): "<<endl;
        cin>>command;
        if(command == 0)
            return 0;
        else if(command == 1)
        {
            command = -1;//here command states for size
            while(command != 1 && command != 2 && command != 3 && command != 0)
            {
                cout<<"Print 1 for smallSize data, 2 for mediumSize data and 3 for bigSize data (print 0 to quit program): "<<endl;
                cin>>command;
                switch(command)
                {
                case 0:
                    return 0;
                case 1:
                    currentSize = smallSize;
                    break;
                case 2:
                    currentSize = mediumSize;
                    break;
                case 3:
                    currentSize = bigSize;
                    break;
                }
            }
            elements.reserve(currentSize);
            elements.resize(currentSize,0);
            for_each(elements.begin(), elements.end(), [](int& element)
            {
                element = rand() % 100 - 50;
            });
            break;
        }
        else if(command == 2)
        {
            while(lineNumber <= 0)
            {
                cout<<"Print line natural number in manually created tests file: "<<endl;
                cin>>lineNumberS;
                try
                {
                    lineNumber = stoi(lineNumberS);
                }
                catch (invalid_argument const &e)
                {
                    cout<<"Not a number"<<endl;
                }
                catch (out_of_range const &e)
                {
                    cout<<"Out of range number"<<endl;
                }
                if(lineNumber <= 0)
                {
                    cout<<"Negative number or 0"<<endl;
                }
            }
            if(!readFromFile(rawDataFile, elements, lineNumber))
            {
                cout<<"Line not found."<<endl;
                return 2;
            }
            break;
        }
    }

    if(currentSize > mediumSize)    //For big size write data in file
    {

        if(!writeInFile(bigRawDataFile, false, elements))
        {
            cout << "Cannot open file " << bigRawDataFile << endl;
            return 1;
        }

    }
    else    //For small size write in console
    {
        printVector(elements);
    }

    command = -1;
    while(command != 1 && command != 2 && command != 3 && command != 0)
    {
        cout<<"Print command"<<endl;
        cout<<"1 - mergeSort"<<endl;
        cout<<"2 - quickSort"<<endl;
        cout<<"3 - heapSort"<<endl;
        cout<<"0 - Exit"<<endl;
        cin>>command;
        start = clock();
        switch(command)
        {
        case 0:
            return 0;
        case 1:
            mergeSort(elements.begin(),elements.end());
            break;
        case 2:
            quickSort(elements, 0, elements.size() - 1);
            break;
        case 3:
            heapSort(elements);
            break;
        }
        finish = clock(); ///============================================================

        printf("The time: %G seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
    }

    if(!writeInFile(sortedFile, true, elements))
    {
        cout<<"Cannot open file "<<sortedFile<<endl;
        return 1;
    }
    openFileInNotepad(sortedFile);
    return 0;
}
