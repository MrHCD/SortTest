#include "SortControl.h"

void SortControl::init(unsigned count, unsigned *values)
{
    if (count == 0 || values == nullptr)
    {
        barCount = 0;
        barObjects = nullptr;
        return; // 如果没有柱状图，直接返回
    }

    // Allocate memory for the bar objects
    barCount = count;
    barObjects = new BarObject[barCount];

    for(unsigned i = 0 ; i< barCount; ++i)
    {
        barObjects[i].setValue(values[i]);
    }

}

void SortControl::initializeBars(unsigned win_width, unsigned win_height)
{
    if (barCount == 0 || barObjects == nullptr)
    {
        return; // 如果没有柱状图，直接返回
    }


    unsigned barWidth = win_width / 20; // Calculate width of each bar
    unsigned barSpacing = barWidth / 4 ; // Calculate space between bars

    int x = barSpacing; // Initial x position for the first bar

    // Initialize each bar object with the provided values
    for (unsigned i = 0; i < barCount; ++i)
    {
        unsigned height = (barObjects[i].getValue() * win_height) / 100; // Scale height based on window height
        unsigned width = barWidth; // Distribute width evenly
        unsigned y = win_height - height; // Calculate y position

        barObjects[i].setBarHeight(height)
                     .setBarWidth(width)
                     .setBarX(x)
                     .setBarY(y); 

        // Set the color based on the status
        if (barObjects[i].getStatus() == BarObject::Status::HIGHLIGHTED)
        {
            barObjects[i].setBarColor(BarObject::Color::RED);
        }
        else if (barObjects[i].getStatus() == BarObject::Status::SORTED)
        {
            barObjects[i].setBarColor(BarObject::Color::BULE);
        }
        else
        {
            barObjects[i].setBarColor(BarObject::Color::GRAY);
        }
        

        x += barWidth + barSpacing; // Update x position for the next bar
    }
}
    

void SortControl::updateBars(unsigned index, BarObject::Status status)
{
    if (index < barCount)
    {
        barObjects[index].setStatus(status);
    }
    else
    {
        // Handle error: index out of bounds
    }
}

void SortControl::cleanupBars()
{
    if(barObjects == nullptr)
        return; // If barObjects is already null, nothing to clean up

    delete[] barObjects; // Clean up the allocated memory for bar objects
    barObjects = nullptr; // Set pointer to nullptr after deletion
    barCount = 0; // Reset bar count
}

void SortControl::resetState()
{
    isSorted = true; // Reset sorted state
    execCount = 0; // Reset execution count
}

SortControl::SortControl()
    : barObjects(nullptr), barCount(0), isSorted(true), execCount(0)
{
}

SortControl::~SortControl()
{
}
