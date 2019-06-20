#pragma once
#include "cell.h"

namespace spt {

template <std::size_t Dim = 3, typename Real = typename spt::vec<Dim>::real_type>
using polychoron = polytope<4, Dim, Real>;

} // namespace spt