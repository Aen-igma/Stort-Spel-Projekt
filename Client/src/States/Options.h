#pragma once
#include "States.h"
#include<functional>

class Options : public State {
private:
    Aen::Entity* m_UI;

public:
    Options(Aen::Window& window);
    ~Options() override;

    // Inherited via State
    virtual void Update(const float& deltaTime) override;
    virtual void Initialize() override;
};
