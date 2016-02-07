#ifndef TASK_HH
#define TASK_HH

namespace Shackle
{

    class Task
    {
    public:
        Task() {}
        virtual ~Task() {}

        virtual void run() = 0;
    };

}
#endif // TASK_HH
