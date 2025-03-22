#pragma once

class BaseComponent
{
public:
    virtual ~BaseComponent() = default;
    virtual void Update(float deltaTime) {
        std::cout << "Base Component Updated!" << std::endl;
    };
};