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

private:
	template<uint64_t i>
	saw::error_or<void> apply_ele(saw::data<Schema>& dat){
		if constexpr ( i < sizeof...(T) ){
			auto& cmp_ele = std::get<i>(tuple);
			cmp_ele.template apply<Schema>(dat);
			
			auto eov = apply_ele<i+1u>(dat);
			if(eov.is_error()){
				return std::move(eov.get_error());
			}
		}
		return saw::void_t{};
	}

public:
	saw::data<Schema> generate_data(){
		auto builder = saw::build_schema<saw::schema::Struct<>>();
		auto next_builder = build_schema_ele<saw::schema::Struct<>, 0u>(builder);
		return saw::data<typename decltype(next_builder)::Schema>{};
	}

	saw::error_or<void> apply(saw::data<Schema>& dat){
		return apply_ele<0u>(dat);
	}
};
