// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "TodayObjects.h"

#include "graphqlservice/introspection/Introspection.h"

#include <algorithm>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

using namespace std::literals;

namespace graphql::today {
namespace object {

FolderConnection::FolderConnection()
	: service::Object({
		"FolderConnection"
	}, {
		{ R"gql(__typename)gql"sv, [this](service::ResolverParams&& params) { return resolve_typename(std::move(params)); } },
		{ R"gql(edges)gql"sv, [this](service::ResolverParams&& params) { return resolveEdges(std::move(params)); } },
		{ R"gql(pageInfo)gql"sv, [this](service::ResolverParams&& params) { return resolvePageInfo(std::move(params)); } }
	})
{
}

service::FieldResult<std::shared_ptr<PageInfo>> FolderConnection::getPageInfo(service::FieldParams&&) const
{
	throw std::runtime_error(R"ex(FolderConnection::getPageInfo is not implemented)ex");
}

std::future<service::ResolverResult> FolderConnection::resolvePageInfo(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto result = getPageInfo(service::FieldParams(params, std::move(params.fieldDirectives)));
	resolverLock.unlock();

	return service::ModifiedResult<PageInfo>::convert(std::move(result), std::move(params));
}

service::FieldResult<std::optional<std::vector<std::shared_ptr<FolderEdge>>>> FolderConnection::getEdges(service::FieldParams&&) const
{
	throw std::runtime_error(R"ex(FolderConnection::getEdges is not implemented)ex");
}

std::future<service::ResolverResult> FolderConnection::resolveEdges(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto result = getEdges(service::FieldParams(params, std::move(params.fieldDirectives)));
	resolverLock.unlock();

	return service::ModifiedResult<FolderEdge>::convert<service::TypeModifier::Nullable, service::TypeModifier::List, service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> FolderConnection::resolve_typename(service::ResolverParams&& params)
{
	return service::ModifiedResult<response::StringType>::convert(response::StringType{ R"gql(FolderConnection)gql" }, std::move(params));
}

} /* namespace object */

void AddFolderConnectionDetails(std::shared_ptr<schema::ObjectType> typeFolderConnection, const std::shared_ptr<schema::Schema>& schema)
{
	typeFolderConnection->AddFields({
		std::make_shared<schema::Field>(R"gql(pageInfo)gql"sv, R"md()md"sv, std::nullopt, std::vector<std::shared_ptr<schema::InputValue>>(), schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("PageInfo"))),
		std::make_shared<schema::Field>(R"gql(edges)gql"sv, R"md()md"sv, std::nullopt, std::vector<std::shared_ptr<schema::InputValue>>(), schema->WrapType(introspection::TypeKind::LIST, schema->LookupType("FolderEdge")))
	});
}

} /* namespace graphql::today */
