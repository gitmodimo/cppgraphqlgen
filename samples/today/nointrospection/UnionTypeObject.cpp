// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "UnionTypeObject.h"

#include "graphqlservice/internal/Schema.h"

#include "graphqlservice/introspection/IntrospectionSchema.h"

using namespace std::literals;

namespace graphql::today {
namespace object {

UnionType::UnionType(std::unique_ptr<const Concept>&& pimpl) noexcept
	: service::Object { pimpl->getTypeNames(), pimpl->getResolvers() }
	, _pimpl { std::move(pimpl) }
{
}

void UnionType::beginSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->beginSelectionSet(params);
}

void UnionType::endSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->endSelectionSet(params);
}

} // namespace object

void AddUnionTypeDetails(const std::shared_ptr<schema::UnionType>& typeUnionType, const std::shared_ptr<schema::Schema>& schema)
{
	typeUnionType->AddPossibleTypes({
		schema->LookupType(R"gql(Appointment)gql"sv),
		schema->LookupType(R"gql(Task)gql"sv),
		schema->LookupType(R"gql(Folder)gql"sv)
	});
}

} // namespace graphql::today
