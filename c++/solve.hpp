#pragma once

#include "common.hpp"
#include "component.hpp"

struct LrDecomposition {};

template<>
class component<LrSolver> {
	template<typename Schema>
	static auto build_types(saw::schema_factory<Schema> factory){
		return factory
			.template add_maybe<Matrix, "l">()
			.template add_maybe<Matrix, "u">()
		;
	}

	template<typename Schema>
	static saw::error_or<void> solve(saw::data<Schema>& val){
		auto& l = val.template get<"l">();
		auto& u = val.template get<"u">();

		return saw::void_t{};
	}
};
