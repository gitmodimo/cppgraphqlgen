// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

module;

#include "QueryClient.h"

export module GraphQL.Query.QueryClient;

export namespace graphql::query {

namespace client {

using client::GetRequestText;
using client::GetRequestObject;

} // namespace client

using query::TaskState;

namespace client {

namespace query::Query {

using graphql::query::client::GetRequestText;
using graphql::query::client::GetRequestObject;
using Query::GetOperationName;

using graphql::query::TaskState;

using Query::Response;
using Query::ResponseVisitor;
using Query::parseResponse;

using Query::Traits;

} // namespace query::Query

} // namespace client
} // namespace graphql::query
