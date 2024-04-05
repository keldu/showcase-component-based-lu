#pragma once

#include <tuple>

#include "common.hpp"

template<typename T>
class component {
};

template<typename... T>
class component_tuple {
private:
	std::tuple<component<T>...> tuple;

	template<typename Schema, uint64_t i>
	static constexpr auto build_schema_ele(saw::schema_factory<Schema> builder){
		if constexpr ( i < sizeof...(T) ){
			auto next_builder = std::decay_t<decltype(std::get<i>(tuple))>::build_types(builder);
			return build_schema_ele<typename decltype(next_builder)::Schema, i+1u>(next_builder);
		} else {
			return builder;
		}
	}
public:
	using Schema = typename std::decay_t<decltype(build_schema_ele<saw::schema::Struct<>, 0u>({}))>::Schema;

	saw::data<Schema> generate_data(){
		auto builder = saw::build_schema<saw::schema::Struct<>>();
		auto next_builder = build_schema_ele<saw::schema::Struct<>, 0u>(builder);
		return saw::data<typename decltype(next_builder)::Schema>{};
	}

	saw::error_or<void> apply(saw::data<Schema>& data){

		return saw::void_t{};
	}
};
