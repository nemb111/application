#pragma once

#include "receiver.h"

struct Sender {
    virtual void connect(Receiver* receiver);

    Receiver* m_receiver;
};