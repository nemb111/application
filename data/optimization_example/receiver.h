#pragma once

struct Receiver {
    virtual void connect();

    bool isConnected = false;
};