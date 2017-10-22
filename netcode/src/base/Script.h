#ifndef NC_SCRIPT_H
#define NC_SCRIPT_H

#include "netcode.h"

class Script {
public:
    Script();
    virtual void Start();
    virtual void Update();
    virtual void Destroy();

    bool IsRunnable()  { return isRunnable; }
    bool IsDestroyed() { return isDestroyed; }
protected:
    bool isRunnable;
    bool isDestroyed;
};

#endif // NC_SCRIPT_H