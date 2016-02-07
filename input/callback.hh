#ifndef CALLBACK_HH
#define CALLBACK_HH


namespace Shackle
{

struct HighLevelInput;



class Callback
{
    public:
        Callback(){}
        virtual ~Callback() {}

        virtual void handle(HighLevelInput* input) = 0;

    private:


        Callback(const Callback& other);
        Callback& operator=(const Callback& other);
};


}

#endif // CALLBACK_HH
