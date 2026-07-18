#pragma once
#include <string>

namespace StacKSpy { namespace Core { namespace Models {
			struct StockInfo
			{
				std::string Code;
				std::string Name;

				StockInfo() = default;
				StockInfo(const std::string& code, const std::string& name);
			};
		}
	}
}