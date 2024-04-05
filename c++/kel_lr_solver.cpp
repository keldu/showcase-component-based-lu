#include "decompose.hpp"
#include "substitute.hpp"

#include <iostream>

void print_matrix(const saw::data<schema::Matrix>& m){
	const uint64_t a = m.get_dim_size(0);
	const uint64_t b = m.get_dim_size(1);

	std::cout<<"Matrix: "<<a<<" "<<b<<"\n";
	for(uint64_t i = 0u; i < b; ++i){
		for(uint64_t j = 0u; j < a; ++j){
			std::cout<<" "<<m.at(i,j).get();
		}
		std::cout<<"\n";
	}
	std::cout<<std::endl;
}

void print_vector(const saw::data<schema::Vector>& m){
	const uint64_t a = m.get_dim_size(0);

	std::cout<<"Vector: "<<a<<"\n";
	for(uint64_t i = 0u; i < a; ++i){
		std::cout<<" "<<m.at(i).get();
	}
	std::cout<<std::endl;
}

int main(int argc, char** argv){
	(void) argc;
	(void) argv;

	component_tuple<
		cmpt::LuDecomposition,
		cmpt::ForwardSubstitution,
		cmpt::BackwardSubstitution
	> comps;

	auto data = comps.generate_data();

	auto& u = data.template get<"U">();
	auto& l = data.template get<"L">();
	auto& b = data.template get<"b">();
	auto& x = data.template get<"x">();
	auto& y = data.template get<"y">();

	uint64_t n = 3;
	u = {{n,n}};
	l = {{n,n}};
	b = {{n}};
	x = {{n}};
	y = {{n}};

	u.at(0,0) =  2u;
	u.at(0,1) =  4u;
	u.at(0,2) =  5u;
	u.at(1,0) =  4u;
	u.at(1,1) =  9u;
	u.at(1,2) = 11u;
	u.at(2,0) =  2u;
	u.at(2,1) =  7u;
	u.at(2,2) = 11u;

	for(uint64_t i = 0; i < n; ++i){
		l.at(i,i) = 1u;
	}

	b.at(0) = 1u;
	b.at(1) = 3u;
	b.at(2) = 1u;

	auto eov = comps.apply(data);
	if(eov.is_error()){
		auto& err = eov.get_error();
		std::cerr<<"Error: "<<err.get_category();
		auto& msg = err.get_message();
		if( not msg.empty() ){
			std::cerr<<" - "<<msg;
		}
		std::cerr<<std::endl;
	}
	
	print_matrix(l);
	print_matrix(u);
	print_vector(b);
	print_vector(y);
	print_vector(x);

	return 0;
}
