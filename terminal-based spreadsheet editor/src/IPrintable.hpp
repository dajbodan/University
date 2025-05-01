#pragma once

#include <iostream>

/**
 * this is interface that has one polymorphic method print().
 */
class IPrintable
{
public:
    /**
     * it prints into ostream, to output
     * @param os output stream
     * @return
     */
    virtual std :: ostream & print(std :: ostream & os) = 0;
};