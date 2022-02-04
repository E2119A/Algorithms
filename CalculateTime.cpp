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
    vector<int> elements;
    int esize = 2;
    clock_t time, time1;
    double rett;
    printf("The number of elements           Merge                           Quick                           Heap");
    printf("\n");
    for(int i=0;i<25; i++)
    {
        esize = esize*2;

        printf("\t%d        ",esize);

        elements.reserve(esize);
        elements.resize(esize,0);
        for_each(elements.begin(), elements.end(), [](int& element)
        {
            element = rand() % 100 - 50;
        });

        time=clock();
        mergeSort(elements.begin(),elements.end());
        time1=clock();
        printf("\t   %f seconds     ", (double)(time1 - time) / CLOCKS_PER_SEC);

        time=clock();
        quickSort(elements, 0, elements.size() - 1);
        time1=clock();
        printf("\t   %f seconds     ", (double)(time1 - time) / CLOCKS_PER_SEC);

        time=clock();
        heapSort(elements);
        time1=clock();
        printf("\t   %f seconds     ", (double)(time1 - time) / CLOCKS_PER_SEC);

        printf("\n");


    }

    return 0;
}
