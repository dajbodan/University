#pragma once

#include "TableLoader.hpp"
#include <iostream>
#include <optional>

/**
 * this class load value for table from console
 */
class ConsoleLoader : public TableLoader
{
public :
    /**
     *
     * @return true and string when this method reads from input successfully. Otherwise false and dummy string
     */
    virtual std :: pair<bool, std :: string> load() override;
    virtual  ~ConsoleLoader() { }
private:

};