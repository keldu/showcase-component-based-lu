#pragma once

#include <forstio/codec/data.hpp>

#include "common.hpp"
#include "component.hpp"

namespace cmpt {
struct LuDecomposition {};
}

template<>
class component<cmpt::LuDecomposition> {
public:
	template<typename Schema>
	static auto build_types(saw::schema_factory<Schema> factory){
		return factory
			.template add_maybe<schema::Matrix, "L">()
			.template add_maybe<schema::Matrix, "U">()
		;
	}

	template<typename Schema>
	static saw::error_or<void> solve(saw::data<Schema>& val){
		auto& l = val.template get<"L">();
		auto& u = val.template get<"U">();

		return saw::void_t{};
	}
};
