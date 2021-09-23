#pragma once
#include"PhysXWrap.h"
using namespace physx;

class PhysXService {
private:
    static PhysXWrap* m_PhysXService;

public:
   
    PhysXService() {
        m_PhysXService = nullptr;
    };

    virtual ~PhysXService() {
        delete m_PhysXService; 
    };

    static PhysXWrap* GetInstance() {
        if (!m_PhysXService)
            m_PhysXService = new PhysXWrap;
        return m_PhysXService;
    };

    static void SetInstance(PhysXWrap* instance) {
        m_PhysXService = instance;
    };
};