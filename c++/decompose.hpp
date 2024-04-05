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

	/**
	 * maybe set up initialization function to ensure correct behaviour?
	 */

	/**
	 * Do a step. In this case we don't really have steps?
	 */
	template<typename Schema>
	static saw::error_or<void> apply(saw::data<Schema>& val){
		auto& l = val.template get<"L">();
		auto& u = val.template get<"U">();

		auto l_d = l.get_dims();
		auto u_d = u.get_dims();

		/**
		 * Check if square and check if l and u are the same squares
		 */
		if ( ! ((l_d.at({0}) == l_d.at({1})) && (u_d.at({0}) == u_d.at({1})) && (l_d.at({0}) == u_d.at({0})) ) ){
			return saw::make_error<saw::err::invalid_state>("Matrix needs to be square and both have to be the same size");
		}
		uint64_t n = l_d.at({0}).get();

		for(uint64_t i = 0; (i+1u) < n; ++i){
			for(uint64_t k = i+1u; k < n; ++k){
				if(u.at(i,i).get() == 0u){
					return saw::make_error<saw::err::invalid_state>("Diagonal is zero");
				}
				l.at(k,i) = u.at(k,i) / u.at(i,i);
				for(uint64_t j = i; j < n; ++j){
					u.at(k,j) = u.at(k,j) - l.at(k,i) * u.at(i,j);
				}
			}
		}

		return saw::void_t{};
	}
};
