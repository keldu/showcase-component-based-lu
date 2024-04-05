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
	static saw::error_or<void> apply(saw::data<Schema>& val){
		auto& l = val.template get<"L">();
		auto& y = val.template get<"y">();
		auto& b = val.template get<"b">();

		const uint64_t n = y.get_dim_size(0u);
		if( ! (n == b.get_dim_size(0) ) ){
			return saw::make_error<saw::err::invalid_state>("Vectors need to be the same size");
		}
		if( ! (n == l.get_dim_size(0) && n == l.get_dim_size(1) ) ){
			return saw::make_error<saw::err::invalid_state>("Matrix needs to be square and both have to be the same size");
		}

		for(uint64_t i = 0; i < n; ++i) {
			if( l.at(i,i).get() == 0u ){
				return saw::make_error<saw::err::invalid_state>("Diagonal is zero");
			}
			y.at(i) = b.at(i);
			for(uint64_t j = 0; j < i; ++j){
				y.at(i) = y.at(i) - l.at(i,j) * y.at(j);
			}
			y.at(i) = y.at(i) / l.at(i,i);
		}

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
	static saw::error_or<void> apply(saw::data<Schema>& val){
		auto& u = val.template get<"U">();
		auto& y = val.template get<"y">();
		auto& x = val.template get<"x">();
		
		const uint64_t n = y.get_dim_size(0u);
		if( ! (n == x.get_dim_size(0) ) ){
			return saw::make_error<saw::err::invalid_state>("Vectors need to be the same size");
		}
		if( ! (n == u.get_dim_size(0) && n == u.get_dim_size(1) ) ){
			return saw::make_error<saw::err::invalid_state>("Matrix needs to be square and both have to be the same size");
		}

		for(uint64_t i = n; i > 0u; --i){
			if( u.at(i-1u,i-1u).get() == 0u ){
				return saw::make_error<saw::err::invalid_state>("Diagonal is zero");
			}
			x.at(i-1u) = y.at(i-1u);
			for(uint64_t j = i+1u; j <= n; ++j){
				x.at(i-1u) = x.at(i-1u) - u.at(i-1u,j-1u) * x.at(j-1u);
			}
			x.at(i-1u) = x.at(i-1u) / u.at(i-1u,i-1u);
		}

		return saw::void_t{};
	}
};
