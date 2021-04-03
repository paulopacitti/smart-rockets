# smart-rockets
smart-rockets implementation in C 🚀

# Multithreaded routines
## Quick Sort
- The quick sort implemented uses 4 threads, dividing the Population of rockets in 4 sections. Each one of the sections are sorted using quick sort, then, after the threads are joined, the main thread merges the 4 sections;
- Changing the number of threads breaks the sort function, a improved version of it will be developed;
- Implementation based on [GeeksForGeeks article](https://www.geeksforgeeks.org/merge-sort-using-multi-threading/);

## Building and Running
- `cd smart-rockets`
- `mkdir build`
- `cd build`
- `cmake ..`
- `make`
- `./smart-rockets-exec`