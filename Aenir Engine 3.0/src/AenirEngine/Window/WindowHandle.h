#pragma once
#include"Window.h"
#include<unordered_map>

namespace Aen {

	struct AEN_DECLSPEC WindowHandle {

		/*static std::unordered_map<int, Window*>& GetWindows() {
			static std::unordered_map<int, Window*> windows;
			return windows;
		}*/

		static Window*& GetWindows() {
			static Window* windows;
			return windows;
		}

		static bool HandleMsg() {
			/*static int id = -1;
			for(auto& i : GetWindows())
				id = (i.second->HandleMsg()) ? -1 : i.second->id;

			if(id > -1)
				GetWindows().erase(id);
			return (GetWindows().size() > 0);	*/
			
			if (!(GetWindows()->HandleMsg()))
			{
				return false;
			}
			return true;
		}
	};
}