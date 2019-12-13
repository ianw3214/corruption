#pragma once
#include "oasis.h"

class Title : public Oasis::IState
{
    virtual void Init() override;
    virtual void Close() override;

    virtual void OnEvent(const Oasis::Event& event) override;
    virtual void Update() override;
};