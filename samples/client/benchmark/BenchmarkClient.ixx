// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

module;

#include "BenchmarkClient.h"

export module GraphQL.Benchmark.BenchmarkClient;

export namespace graphql::benchmark {

namespace client {

using client::GetRequestText;
using client::GetRequestObject;

namespace query::Query {

using graphql::benchmark::client::GetRequestText;
using graphql::benchmark::client::GetRequestObject;
using Query::GetOperationName;

using Query::Response;
using Query::parseResponse;

using Query::Traits;

} // namespace query::Query

} // namespace client
} // namespace graphql::benchmark
