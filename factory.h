//ok
#pragma once

#include <memory>
#include <ostream>

#include "args.h"
#include "filters/filter.h"

std::unique_ptr<Filter> CreateFilter(const Filterinfo& spec);

void PrintFiltersHelp(std::ostream& out);