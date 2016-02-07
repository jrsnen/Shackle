
#include <cassert>
#include "boost/shared_ptr.hpp"

#include "macrotask.hh"

namespace Shackle
{

Macrotask::Macrotask():parts_()
{
    //ctor
}

Macrotask::Macrotask(boost::shared_ptr<Task> task):parts_()
{
    //ctor 2
    parts_.push_back(task);
}

Macrotask::~Macrotask()
{
    //dtor
}



void Macrotask::run()
{
    for (unsigned int task = 0; task < parts_.size(); ++task)
    {
        parts_.at(task)->run();
    }

}

void Macrotask::addTask(boost::shared_ptr<Task> task)
{
    assert(task != 0);
    parts_.push_back(task);
}

void Macrotask::clear()
{
    parts_.clear();
}
}
