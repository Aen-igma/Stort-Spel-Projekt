#pragma once
#include"../Graphics.h"

namespace Aen {

	class AEN_DECLSPEC Component {
		public:
		Component(const size_t& id);

		const size_t& GetId();

		protected:

		const size_t m_id;
	};
}
