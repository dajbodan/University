#pragma once

/**
 * this is interface that has one polymorphic method isCycleDetected()
 */
class ICycleDetectable
{
public:
    virtual bool isCycleDetected() = 0;
};