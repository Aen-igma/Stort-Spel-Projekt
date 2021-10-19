#pragma once
#include"PhysXWrap.h"
using namespace physx;

class PhysXService {
private:
    static PhysXWrap* mp_PhysXService;

public:
   
    PhysXService() {
        mp_PhysXService = nullptr;
    };

    virtual ~PhysXService() {
        delete mp_PhysXService; 
    };

    static PhysXWrap* GetInstance() {
        if (!mp_PhysXService)
            mp_PhysXService = new PhysXWrap;
        return mp_PhysXService;
    };

    static void SetInstance(PhysXWrap* instance) {
        mp_PhysXService = instance;
    };
};