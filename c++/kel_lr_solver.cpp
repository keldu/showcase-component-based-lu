#include "decompose.hpp"
#include "substitute.hpp"


int main(int argc, char** argv){
	(void) argc;
	(void) argv;

	component_tuple<
		cmpt::LuDecomposition,
		cmpt::ForwardSubstitution,
		cmpt::BackwardSubstitution
	> comps;

	auto data = comps.generate_data();

	return 0;
}
