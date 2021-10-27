#pragma once
#include"Physics.h"

namespace Aen {

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

        private:
        PhysicsHandler() = default;
        ~PhysicsHandler() = default;

        static void Initialize(const int& toleranceLength, const int& toleranceSpeed) {
            mp_PhysXService = AEN_NEW PhysXWrap;
            mp_PhysXService->InitPhysics(toleranceLength, toleranceSpeed);
        };

        static void Update(const float& deltaTime) {
            mp_PhysXService->RunPhysics(deltaTime);
        }

        static void Destroy() {
            if(mp_PhysXService) {
                delete mp_PhysXService;
            }
        }

        static PhysXWrap* mp_PhysXService;
        friend class GameLoop;
    };
}