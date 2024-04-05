#pragma once

#include <forstio/codec/schema_factory.hpp>

namespace schema {
using namespace saw::schema;
using Precision = Float32;

using Matrix = Array<Precision, 2>;
using Vector = Array<Precision, 1>;
}
