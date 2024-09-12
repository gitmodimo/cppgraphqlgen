// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ifndef GRAPHQLVERSION_H
#define GRAPHQLVERSION_H

#include <cstddef>
#include <string_view>

namespace graphql::internal {

constexpr std::string_view FullVersion { "4.5.8" };

constexpr std::size_t MajorVersion = 4;
constexpr std::size_t MinorVersion = 5;
constexpr std::size_t PatchVersion = 8;

} // namespace graphql::internal

#endif // GRAPHQLVERSION_H
