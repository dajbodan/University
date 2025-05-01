#include "CString.hpp"

#include "AbstractType.hpp"
#include "CParser.hpp"
#include "AbstractSyntaxTree.hpp"
#include <memory>
#include "Controller.hpp"
#include "IPrintable.hpp"
#include "PlusFunction.hpp"
#include <iostream>


int main(void)
{
    std :: cout << "Begin\n";
    Controller controller(std :: make_shared<ConsoleLoader>(ConsoleLoader()));
    controller.startApplication();
    std :: cout << "End\n";
    return 0;
}
