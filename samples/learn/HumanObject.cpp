// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "StarWarsObjects.h"

#include "graphqlservice/introspection/Introspection.h"

#include <algorithm>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

using namespace std::literals;

namespace graphql::learn {
namespace object {

Human::Human()
	: service::Object({
		"Character",
		"Human"
	}, {
		{ R"gql(id)gql"sv, [this](service::ResolverParams&& params) { return resolveId(std::move(params)); } },
		{ R"gql(name)gql"sv, [this](service::ResolverParams&& params) { return resolveName(std::move(params)); } },
		{ R"gql(friends)gql"sv, [this](service::ResolverParams&& params) { return resolveFriends(std::move(params)); } },
		{ R"gql(appearsIn)gql"sv, [this](service::ResolverParams&& params) { return resolveAppearsIn(std::move(params)); } },
		{ R"gql(__typename)gql"sv, [this](service::ResolverParams&& params) { return resolve_typename(std::move(params)); } },
		{ R"gql(homePlanet)gql"sv, [this](service::ResolverParams&& params) { return resolveHomePlanet(std::move(params)); } }
	})
{
}

service::FieldResult<response::StringType> Human::getId(service::FieldParams&&) const
{
	throw std::runtime_error(R"ex(Human::getId is not implemented)ex");
}

service::AwaitableResolver Human::resolveId(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getId(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<response::StringType>::convert(std::move(result), std::move(params));
}

service::FieldResult<std::optional<response::StringType>> Human::getName(service::FieldParams&&) const
{
	throw std::runtime_error(R"ex(Human::getName is not implemented)ex");
}

service::AwaitableResolver Human::resolveName(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getName(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<response::StringType>::convert<service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::FieldResult<std::optional<std::vector<std::shared_ptr<service::Object>>>> Human::getFriends(service::FieldParams&&) const
{
	throw std::runtime_error(R"ex(Human::getFriends is not implemented)ex");
}

service::AwaitableResolver Human::resolveFriends(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getFriends(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<service::Object>::convert<service::TypeModifier::Nullable, service::TypeModifier::List, service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::FieldResult<std::optional<std::vector<std::optional<Episode>>>> Human::getAppearsIn(service::FieldParams&&) const
{
	throw std::runtime_error(R"ex(Human::getAppearsIn is not implemented)ex");
}

service::AwaitableResolver Human::resolveAppearsIn(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getAppearsIn(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<Episode>::convert<service::TypeModifier::Nullable, service::TypeModifier::List, service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::FieldResult<std::optional<response::StringType>> Human::getHomePlanet(service::FieldParams&&) const
{
	throw std::runtime_error(R"ex(Human::getHomePlanet is not implemented)ex");
}

service::AwaitableResolver Human::resolveHomePlanet(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getHomePlanet(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<response::StringType>::convert<service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::AwaitableResolver Human::resolve_typename(service::ResolverParams&& params)
{
	return service::ModifiedResult<response::StringType>::convert(response::StringType{ R"gql(Human)gql" }, std::move(params));
}

} // namespace object

void AddHumanDetails(std::shared_ptr<schema::ObjectType> typeHuman, const std::shared_ptr<schema::Schema>& schema)
{
	typeHuman->AddInterfaces({
		std::static_pointer_cast<const schema::InterfaceType>(schema->LookupType(R"gql(Character)gql"sv))
	});
	typeHuman->AddFields({
		schema::Field::Make(R"gql(id)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("String"))),
		schema::Field::Make(R"gql(name)gql"sv, R"md()md"sv, std::nullopt, schema->LookupType("String")),
		schema::Field::Make(R"gql(friends)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::LIST, schema->LookupType("Character"))),
		schema::Field::Make(R"gql(appearsIn)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::LIST, schema->LookupType("Episode"))),
		schema::Field::Make(R"gql(homePlanet)gql"sv, R"md()md"sv, std::nullopt, schema->LookupType("String"))
	});
}

} // namespace graphql::learn
