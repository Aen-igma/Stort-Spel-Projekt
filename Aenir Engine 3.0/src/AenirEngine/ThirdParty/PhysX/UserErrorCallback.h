#pragma once

#include"foundation\PxErrorCallback.h"

class UserErrorCallback : public physx::PxErrorCallback
{
public:
    virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);
   
};