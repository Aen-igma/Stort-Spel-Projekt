#pragma once
#include"Physics.h"

namespace Aen {

    class CollisionFilter : public px::PxControllerFilterCallback {
        public:
        CollisionFilter();
        ~CollisionFilter() override;
        virtual bool filter(const px::PxController& a, const px::PxController& b) override;
    };

    class AEN_DECLSPEC PhysicsHandler {
        public:

        static PhysXWrap*& GetInstance() {
            return mp_PhysXService;
        };

        static void SetGravity(const Vec3f& force) {
            mp_PhysXService->SetGravity(force);
        }

        static void SetGravity(const float& x, const float& y, const float& z) {
            mp_PhysXService->SetGravity(x, y, z);
        }

        static px::PxControllerManager*& GetCManager() {
            return m_cManager;
        }

        private:
        PhysicsHandler() = default;
        ~PhysicsHandler() = default;

        static void Initialize(const int& toleranceLength, const int& toleranceSpeed) {
            mp_PhysXService = AEN_NEW PhysXWrap;
            mp_PhysXService->InitPhysics(toleranceLength, toleranceSpeed);
            m_cManager = PxCreateControllerManager(*mp_PhysXService->GetScene(), false);
        };

        static void Update(const float& deltaTime) {
            mp_PhysXService->RunPhysics(deltaTime);
        }

        static void Destroy() {
            if(mp_PhysXService)
                delete mp_PhysXService;

            /*if(m_cManager)
                m_cManager->release();*/
        }

        static PhysXWrap* mp_PhysXService;
        static px::PxControllerManager* m_cManager;
        friend class GameLoop;
    };
}