#include "BarObject.h"

class SortControl
{
private:
    /* data */
    BarObject *barObjects; // Array of BarObject instances
    unsigned barCount; // Number of bars in the control

    bool isSorted; // Flag to indicate if the bars are sorted
    unsigned execCount; // Execution count for sorting operations

public:
    // Getters and Setters for bar properties
    SortControl& setBarObjects(BarObject *bars) { barObjects = bars; return *this; }
    BarObject* getBarObjects() const { return barObjects; }

    SortControl& setBarCount(unsigned count) { barCount = count; return *this; }
    unsigned getBarCount() const { return barCount; }

    SortControl& setIsSorted(bool sorted) { isSorted = sorted; return *this; }
    bool getIsSorted() const { return isSorted; }

    SortControl& addExecCount(unsigned count = 1) { execCount += count; return *this; }
    unsigned getExecCount() const { return execCount; }

    // Function to init the data of bar objects
    void init(unsigned count, unsigned* values);

    // Function to initialize the bar objects
    void initializeBars( unsigned win_width, unsigned win_height);

    // Function to update the bar properties
    void updateBars(unsigned index, BarObject::Status status);

    // Function to clean up the bar objects
    void cleanupBars(); 

    void resetState();

public:
    void* operator new(size_t size)
    {
        return ::operator new(size);
    }

    void operator delete(void *ptr)
    {
        ::operator delete(ptr);
    }

    SortControl(/* args */);
    ~SortControl();
};
