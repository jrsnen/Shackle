#ifndef STATEMANAGER_HH
#define STATEMANAGER_HH



#include <vector>


namespace Shackle
{
    class GameState;
    class InputManager;
    class Userinterface;
    class Scripter;
    class GraphicsEngine;
    class EngineController;
    class Worker;

class StateManager
{
    public:
        StateManager();
        ~StateManager();

        void pushState(GameState* state);

        //! \brief
        //! \return Tells whether state was iniated which means it might have taken longer than expected
        bool update(EngineController& controller, InputManager& input,
                    GraphicsEngine& graphics, Userinterface* gui,
                    Scripter& scripter, Worker& worker);

        void draw(float interpolation, GraphicsEngine& graphics,
                      Userinterface* gui, Worker& worker);

        bool statesLeft() const;


    private:

    void popState();

    bool iniated_;


    std::vector<GameState*> states_;

    StateManager(const StateManager& other);
    StateManager& operator=(const StateManager& other);
};


}
#endif // STATEMANAGER_HH
