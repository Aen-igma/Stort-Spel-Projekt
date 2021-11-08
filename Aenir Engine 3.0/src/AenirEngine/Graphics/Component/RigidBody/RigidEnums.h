#pragma once
#include"../Component.h"
#include"../../Physics/PhysicsHandler.h"


namespace Aen {
	enum class CombineMode {
		AVERAGE = 0,
		MIN = 1,
		MULTIPLY = 2,
		MAX = 3
	};

	enum class MaterialFlag {
		DISABLE_FRICTION = 1 << 0, 
		DISABLE_STRONG_FRICTION = 1 << 1, 
		IMPROVED_PATCH_FRICTION = 1 << 2 
	};

	enum class DynamicGeometryType {
		SPHERE,
		CAPSULE,
		CUBE
	};

	enum class StaticGeometryType {
		SPHERE,
		CAPSULE,
		PLANE,
		CUBE
	};

	enum class ForceMode {
		FORCE,			
		IMPULSE,		
		VELOCITY_CHANGE,
		ACCELERATION	
	};
}