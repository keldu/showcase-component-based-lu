#pragma once

#include <forstio/codec/data.hpp>

#include "common.hpp"
#include "component.hpp"

namespace cmpt {
struct ForwardSubstitution {};
struct BackwardSubstitution {};
}

template<>
class component<cmpt::ForwardSubstitution> {
public:
	template<typename Schema>
	static auto build_types(saw::schema_factory<Schema> factory){
		return factory
			.template add_maybe<schema::Matrix, "L">()
			.template add_maybe<schema::Vector, "y">()
			.template add_maybe<schema::Vector, "b">()
		;
	}

	template<typename Schema>
	static saw::error_or<void> solve(saw::data<Schema>& val){
		auto& l = val.template get<"L">();
		auto& y = val.template get<"y">();
		auto& b = val.template get<"b">();

		return saw::void_t{};
	}
};

template<>
class component<cmpt::BackwardSubstitution> {
public:
	template<typename Schema>
	static auto build_types(saw::schema_factory<Schema> factory){
		return factory
			.template add_maybe<schema::Matrix, "U">()
			.template add_maybe<schema::Vector, "y">()
			.template add_maybe<schema::Vector, "x">()
		;
	}

	template<typename Schema>
	static saw::error_or<void> solve(saw::data<Schema>& val){
		auto& u = val.template get<"U">();
		auto& y = val.template get<"y">();
		auto& x = val.template get<"x">();

		return saw::void_t{};
	}
};
