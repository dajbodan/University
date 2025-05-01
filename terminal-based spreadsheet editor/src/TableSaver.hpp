#pragma once

#include "Table.hpp"

/**
 * this represents interface. Class that inherits this interface, can save table.
 */
class TableSaver
{
public:
    virtual bool save(const std :: shared_ptr<Table> & table) = 0;
};