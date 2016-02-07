#ifndef ENGINECONTROLLER_HH
#define ENGINECONTROLLER_HH


#include <cassert>
namespace Shackle
{

class EngineController
{
    public:
        EngineController():continue_(true), multiplier_(1){}
        ~EngineController(){}

        void quit()
        {
            continue_ = false;
        }

        void setSpeed(unsigned int multiplier)
        {
            assert(multiplier_ != 0 && " tried to make game never update");
            multiplier_ = multiplier_;
        }

        bool stillGoing() const
        {
            return continue_;
        }

        unsigned int speed() const
        {
            return multiplier_;
        }

        // add showfps


    private:

    bool continue_;

    unsigned int multiplier_;

    EngineController(const EngineController& other);
    EngineController& operator=(const EngineController& other);
};

}
#endif // ENGINECONTROLLER_HH
