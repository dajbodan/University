#pragma once

#include <iostream>


/**
 * this represents interface. Class that inherits this interface, can load table.
 */
class TableLoader
{
public:
    virtual std :: pair<bool, std :: string> load() = 0;

    virtual ~TableLoader() { }
};