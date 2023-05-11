// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "QueryObject.h"
#include "MutationObject.h"
#include "SubscriptionObject.h"

#include "graphqlservice/internal/Schema.h"

#include "graphqlservice/introspection/IntrospectionSchema.h"

#include <algorithm>
#include <array>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

using namespace std::literals;

namespace graphql {
namespace service {

static const auto s_namesTaskState = today::getTaskStateNames();
static const auto s_valuesTaskState = today::getTaskStateValues();

template <>
today::TaskState Argument<today::TaskState>::convert(const response::Value& value)
{
	if (!value.maybe_enum())
	{
		throw service::schema_exception { { R"ex(not a valid TaskState value)ex" } };
	}

	const auto result = internal::sorted_map_lookup<internal::shorter_or_less>(
		s_valuesTaskState,
		std::string_view { value.get<std::string>() });

	if (!result)
	{
		throw service::schema_exception { { R"ex(not a valid TaskState value)ex" } };
	}

	return *result;
}

template <>
service::AwaitableResolver Result<today::TaskState>::convert(service::AwaitableScalar<today::TaskState> result, ResolverParams params)
{
	return ModifiedResult<today::TaskState>::resolve(std::move(result), std::move(params),
		[](today::TaskState value, const ResolverParams&)
		{
			response::Value result(response::Type::EnumValue);

			result.set<std::string>(std::string { s_namesTaskState[static_cast<size_t>(value)] });

			return result;
		});
}

template <>
void Result<today::TaskState>::validateScalar(const response::Value& value)
{
	if (!value.maybe_enum())
	{
		throw service::schema_exception { { R"ex(not a valid TaskState value)ex" } };
	}

	const auto [itr, itrEnd] = internal::sorted_map_equal_range<internal::shorter_or_less>(
		s_valuesTaskState.begin(),
		s_valuesTaskState.end(),
		std::string_view { value.get<std::string>() });

	if (itr == itrEnd)
	{
		throw service::schema_exception { { R"ex(not a valid TaskState value)ex" } };
	}
}

template <>
today::CompleteTaskInput Argument<today::CompleteTaskInput>::convert(const response::Value& value)
{
	const auto defaultValue = []()
	{
		response::Value values(response::Type::Map);
		response::Value entry;

		entry = response::Value(true);
		values.emplace_back("isComplete", std::move(entry));

		return values;
	}();

	auto valueId = service::ModifiedArgument<response::IdType>::require("id", value);
	auto valueTestTaskState = service::ModifiedArgument<today::TaskState>::require<service::TypeModifier::Nullable>("testTaskState", value);
	auto pairIsComplete = service::ModifiedArgument<bool>::find<service::TypeModifier::Nullable>("isComplete", value);
	auto valueIsComplete = (pairIsComplete.second
		? std::move(pairIsComplete.first)
		: service::ModifiedArgument<bool>::require<service::TypeModifier::Nullable>("isComplete", defaultValue));
	auto valueClientMutationId = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable>("clientMutationId", value);

	return today::CompleteTaskInput {
		std::move(valueId),
		std::move(valueTestTaskState),
		std::move(valueIsComplete),
		std::move(valueClientMutationId)
	};
}

template <>
today::ThirdNestedInput Argument<today::ThirdNestedInput>::convert(const response::Value& value)
{
	auto valueId = service::ModifiedArgument<response::IdType>::require("id", value);
	auto valueSecond = service::ModifiedArgument<today::SecondNestedInput>::require<service::TypeModifier::Nullable>("second", value);

	return today::ThirdNestedInput {
		std::move(valueId),
		std::move(valueSecond)
	};
}

template <>
today::FourthNestedInput Argument<today::FourthNestedInput>::convert(const response::Value& value)
{
	auto valueId = service::ModifiedArgument<response::IdType>::require("id", value);

	return today::FourthNestedInput {
		std::move(valueId)
	};
}

template <>
today::IncludeNullableSelfInput Argument<today::IncludeNullableSelfInput>::convert(const response::Value& value)
{
	auto valueSelf = service::ModifiedArgument<today::IncludeNullableSelfInput>::require<service::TypeModifier::Nullable>("self", value);

	return today::IncludeNullableSelfInput {
		std::move(valueSelf)
	};
}

template <>
today::IncludeNonNullableListSelfInput Argument<today::IncludeNonNullableListSelfInput>::convert(const response::Value& value)
{
	auto valueSelves = service::ModifiedArgument<today::IncludeNonNullableListSelfInput>::require<service::TypeModifier::List>("selves", value);

	return today::IncludeNonNullableListSelfInput {
		std::move(valueSelves)
	};
}

template <>
today::StringOperationFilterInput Argument<today::StringOperationFilterInput>::convert(const response::Value& value)
{
	auto valueAnd_ = service::ModifiedArgument<today::StringOperationFilterInput>::require<service::TypeModifier::Nullable, service::TypeModifier::List>("and", value);
	auto valueOr_ = service::ModifiedArgument<today::StringOperationFilterInput>::require<service::TypeModifier::Nullable, service::TypeModifier::List>("or", value);
	auto valueEqual = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable>("equal", value);
	auto valueNotEqual = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable>("notEqual", value);
	auto valueContains = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable>("contains", value);
	auto valueNotContains = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable>("notContains", value);
	auto valueIn = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable, service::TypeModifier::List>("in", value);
	auto valueNotIn = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable, service::TypeModifier::List>("notIn", value);
	auto valueStartsWith = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable>("startsWith", value);
	auto valueNotStartsWith = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable>("notStartsWith", value);
	auto valueEndsWith = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable>("endsWith", value);
	auto valueNotEndsWith = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable>("notEndsWith", value);

	return today::StringOperationFilterInput {
		std::move(valueAnd_),
		std::move(valueOr_),
		std::move(valueEqual),
		std::move(valueNotEqual),
		std::move(valueContains),
		std::move(valueNotContains),
		std::move(valueIn),
		std::move(valueNotIn),
		std::move(valueStartsWith),
		std::move(valueNotStartsWith),
		std::move(valueEndsWith),
		std::move(valueNotEndsWith)
	};
}

template <>
today::SecondNestedInput Argument<today::SecondNestedInput>::convert(const response::Value& value)
{
	auto valueId = service::ModifiedArgument<response::IdType>::require("id", value);
	auto valueThird = service::ModifiedArgument<today::ThirdNestedInput>::require("third", value);

	return today::SecondNestedInput {
		std::move(valueId),
		std::move(valueThird)
	};
}

template <>
today::ForwardDeclaredInput Argument<today::ForwardDeclaredInput>::convert(const response::Value& value)
{
	auto valueNullableSelf = service::ModifiedArgument<today::IncludeNullableSelfInput>::require<service::TypeModifier::Nullable>("nullableSelf", value);
	auto valueListSelves = service::ModifiedArgument<today::IncludeNonNullableListSelfInput>::require("listSelves", value);

	return today::ForwardDeclaredInput {
		std::move(valueNullableSelf),
		std::move(valueListSelves)
	};
}

template <>
today::FirstNestedInput Argument<today::FirstNestedInput>::convert(const response::Value& value)
{
	auto valueId = service::ModifiedArgument<response::IdType>::require("id", value);
	auto valueSecond = service::ModifiedArgument<today::SecondNestedInput>::require("second", value);
	auto valueThird = service::ModifiedArgument<today::ThirdNestedInput>::require("third", value);

	return today::FirstNestedInput {
		std::move(valueId),
		std::move(valueSecond),
		std::move(valueThird)
	};
}

} // namespace service

namespace today {

CompleteTaskInput::CompleteTaskInput() noexcept
{
}

CompleteTaskInput::CompleteTaskInput(
		response::IdType idArg,
		std::optional<TaskState> testTaskStateArg,
		std::optional<bool> isCompleteArg,
		std::optional<std::string> clientMutationIdArg) noexcept
	: id { std::move(idArg) }
	, testTaskState { std::move(testTaskStateArg) }
	, isComplete { std::move(isCompleteArg) }
	, clientMutationId { std::move(clientMutationIdArg) }
{
}

CompleteTaskInput::CompleteTaskInput(const CompleteTaskInput& other)
	: id { service::ModifiedArgument<response::IdType>::duplicate(other.id) }
	, testTaskState { service::ModifiedArgument<TaskState>::duplicate<service::TypeModifier::Nullable>(other.testTaskState) }
	, isComplete { service::ModifiedArgument<bool>::duplicate<service::TypeModifier::Nullable>(other.isComplete) }
	, clientMutationId { service::ModifiedArgument<std::string>::duplicate<service::TypeModifier::Nullable>(other.clientMutationId) }
{
}

CompleteTaskInput::CompleteTaskInput(CompleteTaskInput&& other) noexcept
	: id { std::move(other.id) }
	, testTaskState { std::move(other.testTaskState) }
	, isComplete { std::move(other.isComplete) }
	, clientMutationId { std::move(other.clientMutationId) }
{
}

CompleteTaskInput& CompleteTaskInput::operator=(const CompleteTaskInput& other)
{
	CompleteTaskInput value { other };

	std::swap(*this, value);

	return *this;
}

CompleteTaskInput& CompleteTaskInput::operator=(CompleteTaskInput&& other) noexcept
{
	id = std::move(other.id);
	testTaskState = std::move(other.testTaskState);
	isComplete = std::move(other.isComplete);
	clientMutationId = std::move(other.clientMutationId);

	return *this;
}

CompleteTaskInput::~CompleteTaskInput()
{
}

ThirdNestedInput::ThirdNestedInput() noexcept
{
}

ThirdNestedInput::ThirdNestedInput(
		response::IdType idArg,
		std::unique_ptr<SecondNestedInput> secondArg) noexcept
	: id { std::move(idArg) }
	, second { std::move(secondArg) }
{
}

ThirdNestedInput::ThirdNestedInput(const ThirdNestedInput& other)
	: id { service::ModifiedArgument<response::IdType>::duplicate(other.id) }
	, second { service::ModifiedArgument<SecondNestedInput>::duplicate<service::TypeModifier::Nullable>(other.second) }
{
}

ThirdNestedInput::ThirdNestedInput(ThirdNestedInput&& other) noexcept
	: id { std::move(other.id) }
	, second { std::move(other.second) }
{
}

ThirdNestedInput& ThirdNestedInput::operator=(const ThirdNestedInput& other)
{
	ThirdNestedInput value { other };

	std::swap(*this, value);

	return *this;
}

ThirdNestedInput& ThirdNestedInput::operator=(ThirdNestedInput&& other) noexcept
{
	id = std::move(other.id);
	second = std::move(other.second);

	return *this;
}

ThirdNestedInput::~ThirdNestedInput()
{
}

FourthNestedInput::FourthNestedInput() noexcept
{
}

FourthNestedInput::FourthNestedInput(
		response::IdType idArg) noexcept
	: id { std::move(idArg) }
{
}

FourthNestedInput::FourthNestedInput(const FourthNestedInput& other)
	: id { service::ModifiedArgument<response::IdType>::duplicate(other.id) }
{
}

FourthNestedInput::FourthNestedInput(FourthNestedInput&& other) noexcept
	: id { std::move(other.id) }
{
}

FourthNestedInput& FourthNestedInput::operator=(const FourthNestedInput& other)
{
	FourthNestedInput value { other };

	std::swap(*this, value);

	return *this;
}

FourthNestedInput& FourthNestedInput::operator=(FourthNestedInput&& other) noexcept
{
	id = std::move(other.id);

	return *this;
}

FourthNestedInput::~FourthNestedInput()
{
}

IncludeNullableSelfInput::IncludeNullableSelfInput() noexcept
{
}

IncludeNullableSelfInput::IncludeNullableSelfInput(
		std::unique_ptr<IncludeNullableSelfInput> selfArg) noexcept
	: self { std::move(selfArg) }
{
}

IncludeNullableSelfInput::IncludeNullableSelfInput(const IncludeNullableSelfInput& other)
	: self { service::ModifiedArgument<IncludeNullableSelfInput>::duplicate<service::TypeModifier::Nullable>(other.self) }
{
}

IncludeNullableSelfInput::IncludeNullableSelfInput(IncludeNullableSelfInput&& other) noexcept
	: self { std::move(other.self) }
{
}

IncludeNullableSelfInput& IncludeNullableSelfInput::operator=(const IncludeNullableSelfInput& other)
{
	IncludeNullableSelfInput value { other };

	std::swap(*this, value);

	return *this;
}

IncludeNullableSelfInput& IncludeNullableSelfInput::operator=(IncludeNullableSelfInput&& other) noexcept
{
	self = std::move(other.self);

	return *this;
}

IncludeNullableSelfInput::~IncludeNullableSelfInput()
{
}

IncludeNonNullableListSelfInput::IncludeNonNullableListSelfInput() noexcept
{
}

IncludeNonNullableListSelfInput::IncludeNonNullableListSelfInput(
		std::vector<IncludeNonNullableListSelfInput> selvesArg) noexcept
	: selves { std::move(selvesArg) }
{
}

IncludeNonNullableListSelfInput::IncludeNonNullableListSelfInput(const IncludeNonNullableListSelfInput& other)
	: selves { service::ModifiedArgument<IncludeNonNullableListSelfInput>::duplicate<service::TypeModifier::List>(other.selves) }
{
}

IncludeNonNullableListSelfInput::IncludeNonNullableListSelfInput(IncludeNonNullableListSelfInput&& other) noexcept
	: selves { std::move(other.selves) }
{
}

IncludeNonNullableListSelfInput& IncludeNonNullableListSelfInput::operator=(const IncludeNonNullableListSelfInput& other)
{
	IncludeNonNullableListSelfInput value { other };

	std::swap(*this, value);

	return *this;
}

IncludeNonNullableListSelfInput& IncludeNonNullableListSelfInput::operator=(IncludeNonNullableListSelfInput&& other) noexcept
{
	selves = std::move(other.selves);

	return *this;
}

IncludeNonNullableListSelfInput::~IncludeNonNullableListSelfInput()
{
}

StringOperationFilterInput::StringOperationFilterInput() noexcept
{
}

StringOperationFilterInput::StringOperationFilterInput(
		std::optional<std::vector<StringOperationFilterInput>> and_Arg,
		std::optional<std::vector<StringOperationFilterInput>> or_Arg,
		std::optional<std::string> equalArg,
		std::optional<std::string> notEqualArg,
		std::optional<std::string> containsArg,
		std::optional<std::string> notContainsArg,
		std::optional<std::vector<std::string>> inArg,
		std::optional<std::vector<std::string>> notInArg,
		std::optional<std::string> startsWithArg,
		std::optional<std::string> notStartsWithArg,
		std::optional<std::string> endsWithArg,
		std::optional<std::string> notEndsWithArg) noexcept
	: and_ { std::move(and_Arg) }
	, or_ { std::move(or_Arg) }
	, equal { std::move(equalArg) }
	, notEqual { std::move(notEqualArg) }
	, contains { std::move(containsArg) }
	, notContains { std::move(notContainsArg) }
	, in { std::move(inArg) }
	, notIn { std::move(notInArg) }
	, startsWith { std::move(startsWithArg) }
	, notStartsWith { std::move(notStartsWithArg) }
	, endsWith { std::move(endsWithArg) }
	, notEndsWith { std::move(notEndsWithArg) }
{
}

StringOperationFilterInput::StringOperationFilterInput(const StringOperationFilterInput& other)
	: and_ { service::ModifiedArgument<StringOperationFilterInput>::duplicate<service::TypeModifier::Nullable, service::TypeModifier::List>(other.and_) }
	, or_ { service::ModifiedArgument<StringOperationFilterInput>::duplicate<service::TypeModifier::Nullable, service::TypeModifier::List>(other.or_) }
	, equal { service::ModifiedArgument<std::string>::duplicate<service::TypeModifier::Nullable>(other.equal) }
	, notEqual { service::ModifiedArgument<std::string>::duplicate<service::TypeModifier::Nullable>(other.notEqual) }
	, contains { service::ModifiedArgument<std::string>::duplicate<service::TypeModifier::Nullable>(other.contains) }
	, notContains { service::ModifiedArgument<std::string>::duplicate<service::TypeModifier::Nullable>(other.notContains) }
	, in { service::ModifiedArgument<std::string>::duplicate<service::TypeModifier::Nullable, service::TypeModifier::List>(other.in) }
	, notIn { service::ModifiedArgument<std::string>::duplicate<service::TypeModifier::Nullable, service::TypeModifier::List>(other.notIn) }
	, startsWith { service::ModifiedArgument<std::string>::duplicate<service::TypeModifier::Nullable>(other.startsWith) }
	, notStartsWith { service::ModifiedArgument<std::string>::duplicate<service::TypeModifier::Nullable>(other.notStartsWith) }
	, endsWith { service::ModifiedArgument<std::string>::duplicate<service::TypeModifier::Nullable>(other.endsWith) }
	, notEndsWith { service::ModifiedArgument<std::string>::duplicate<service::TypeModifier::Nullable>(other.notEndsWith) }
{
}

StringOperationFilterInput::StringOperationFilterInput(StringOperationFilterInput&& other) noexcept
	: and_ { std::move(other.and_) }
	, or_ { std::move(other.or_) }
	, equal { std::move(other.equal) }
	, notEqual { std::move(other.notEqual) }
	, contains { std::move(other.contains) }
	, notContains { std::move(other.notContains) }
	, in { std::move(other.in) }
	, notIn { std::move(other.notIn) }
	, startsWith { std::move(other.startsWith) }
	, notStartsWith { std::move(other.notStartsWith) }
	, endsWith { std::move(other.endsWith) }
	, notEndsWith { std::move(other.notEndsWith) }
{
}

StringOperationFilterInput& StringOperationFilterInput::operator=(const StringOperationFilterInput& other)
{
	StringOperationFilterInput value { other };

	std::swap(*this, value);

	return *this;
}

StringOperationFilterInput& StringOperationFilterInput::operator=(StringOperationFilterInput&& other) noexcept
{
	and_ = std::move(other.and_);
	or_ = std::move(other.or_);
	equal = std::move(other.equal);
	notEqual = std::move(other.notEqual);
	contains = std::move(other.contains);
	notContains = std::move(other.notContains);
	in = std::move(other.in);
	notIn = std::move(other.notIn);
	startsWith = std::move(other.startsWith);
	notStartsWith = std::move(other.notStartsWith);
	endsWith = std::move(other.endsWith);
	notEndsWith = std::move(other.notEndsWith);

	return *this;
}

StringOperationFilterInput::~StringOperationFilterInput()
{
}

SecondNestedInput::SecondNestedInput() noexcept
{
}

SecondNestedInput::SecondNestedInput(
		response::IdType idArg,
		ThirdNestedInput thirdArg) noexcept
	: id { std::move(idArg) }
	, third { std::move(thirdArg) }
{
}

SecondNestedInput::SecondNestedInput(const SecondNestedInput& other)
	: id { service::ModifiedArgument<response::IdType>::duplicate(other.id) }
	, third { service::ModifiedArgument<ThirdNestedInput>::duplicate(other.third) }
{
}

SecondNestedInput::SecondNestedInput(SecondNestedInput&& other) noexcept
	: id { std::move(other.id) }
	, third { std::move(other.third) }
{
}

SecondNestedInput& SecondNestedInput::operator=(const SecondNestedInput& other)
{
	SecondNestedInput value { other };

	std::swap(*this, value);

	return *this;
}

SecondNestedInput& SecondNestedInput::operator=(SecondNestedInput&& other) noexcept
{
	id = std::move(other.id);
	third = std::move(other.third);

	return *this;
}

SecondNestedInput::~SecondNestedInput()
{
}

ForwardDeclaredInput::ForwardDeclaredInput() noexcept
{
}

ForwardDeclaredInput::ForwardDeclaredInput(
		std::unique_ptr<IncludeNullableSelfInput> nullableSelfArg,
		IncludeNonNullableListSelfInput listSelvesArg) noexcept
	: nullableSelf { std::move(nullableSelfArg) }
	, listSelves { std::move(listSelvesArg) }
{
}

ForwardDeclaredInput::ForwardDeclaredInput(const ForwardDeclaredInput& other)
	: nullableSelf { service::ModifiedArgument<IncludeNullableSelfInput>::duplicate<service::TypeModifier::Nullable>(other.nullableSelf) }
	, listSelves { service::ModifiedArgument<IncludeNonNullableListSelfInput>::duplicate(other.listSelves) }
{
}

ForwardDeclaredInput::ForwardDeclaredInput(ForwardDeclaredInput&& other) noexcept
	: nullableSelf { std::move(other.nullableSelf) }
	, listSelves { std::move(other.listSelves) }
{
}

ForwardDeclaredInput& ForwardDeclaredInput::operator=(const ForwardDeclaredInput& other)
{
	ForwardDeclaredInput value { other };

	std::swap(*this, value);

	return *this;
}

ForwardDeclaredInput& ForwardDeclaredInput::operator=(ForwardDeclaredInput&& other) noexcept
{
	nullableSelf = std::move(other.nullableSelf);
	listSelves = std::move(other.listSelves);

	return *this;
}

ForwardDeclaredInput::~ForwardDeclaredInput()
{
}

FirstNestedInput::FirstNestedInput() noexcept
{
}

FirstNestedInput::FirstNestedInput(
		response::IdType idArg,
		SecondNestedInput secondArg,
		ThirdNestedInput thirdArg) noexcept
	: id { std::move(idArg) }
	, second { std::move(secondArg) }
	, third { std::move(thirdArg) }
{
}

FirstNestedInput::FirstNestedInput(const FirstNestedInput& other)
	: id { service::ModifiedArgument<response::IdType>::duplicate(other.id) }
	, second { service::ModifiedArgument<SecondNestedInput>::duplicate(other.second) }
	, third { service::ModifiedArgument<ThirdNestedInput>::duplicate(other.third) }
{
}

FirstNestedInput::FirstNestedInput(FirstNestedInput&& other) noexcept
	: id { std::move(other.id) }
	, second { std::move(other.second) }
	, third { std::move(other.third) }
{
}

FirstNestedInput& FirstNestedInput::operator=(const FirstNestedInput& other)
{
	FirstNestedInput value { other };

	std::swap(*this, value);

	return *this;
}

FirstNestedInput& FirstNestedInput::operator=(FirstNestedInput&& other) noexcept
{
	id = std::move(other.id);
	second = std::move(other.second);
	third = std::move(other.third);

	return *this;
}

FirstNestedInput::~FirstNestedInput()
{
}

Operations::Operations(std::shared_ptr<object::Query> query, std::shared_ptr<object::Mutation> mutation, std::shared_ptr<object::Subscription> subscription)
	: service::Request({
		{ service::strQuery, query },
		{ service::strMutation, mutation },
		{ service::strSubscription, subscription }
	}, GetSchema())
	, _query(std::move(query))
	, _mutation(std::move(mutation))
	, _subscription(std::move(subscription))
{
}

void AddTypesToSchema(const std::shared_ptr<schema::Schema>& schema)
{
	schema->AddType(R"gql(ItemCursor)gql"sv, schema::ScalarType::Make(R"gql(ItemCursor)gql"sv, R"md()md", R"url()url"sv));
	schema->AddType(R"gql(DateTime)gql"sv, schema::ScalarType::Make(R"gql(DateTime)gql"sv, R"md()md", R"url()url"sv));
	auto typeTaskState = schema::EnumType::Make(R"gql(TaskState)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(TaskState)gql"sv, typeTaskState);
	auto typeCompleteTaskInput = schema::InputObjectType::Make(R"gql(CompleteTaskInput)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(CompleteTaskInput)gql"sv, typeCompleteTaskInput);
	auto typeThirdNestedInput = schema::InputObjectType::Make(R"gql(ThirdNestedInput)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(ThirdNestedInput)gql"sv, typeThirdNestedInput);
	auto typeFourthNestedInput = schema::InputObjectType::Make(R"gql(FourthNestedInput)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(FourthNestedInput)gql"sv, typeFourthNestedInput);
	auto typeIncludeNullableSelfInput = schema::InputObjectType::Make(R"gql(IncludeNullableSelfInput)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(IncludeNullableSelfInput)gql"sv, typeIncludeNullableSelfInput);
	auto typeIncludeNonNullableListSelfInput = schema::InputObjectType::Make(R"gql(IncludeNonNullableListSelfInput)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(IncludeNonNullableListSelfInput)gql"sv, typeIncludeNonNullableListSelfInput);
	auto typeStringOperationFilterInput = schema::InputObjectType::Make(R"gql(StringOperationFilterInput)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(StringOperationFilterInput)gql"sv, typeStringOperationFilterInput);
	auto typeSecondNestedInput = schema::InputObjectType::Make(R"gql(SecondNestedInput)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(SecondNestedInput)gql"sv, typeSecondNestedInput);
	auto typeForwardDeclaredInput = schema::InputObjectType::Make(R"gql(ForwardDeclaredInput)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(ForwardDeclaredInput)gql"sv, typeForwardDeclaredInput);
	auto typeFirstNestedInput = schema::InputObjectType::Make(R"gql(FirstNestedInput)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(FirstNestedInput)gql"sv, typeFirstNestedInput);
	auto typeNode = schema::InterfaceType::Make(R"gql(Node)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(Node)gql"sv, typeNode);
	auto typeUnionType = schema::UnionType::Make(R"gql(UnionType)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(UnionType)gql"sv, typeUnionType);
	auto typeQuery = schema::ObjectType::Make(R"gql(Query)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(Query)gql"sv, typeQuery);
	auto typePageInfo = schema::ObjectType::Make(R"gql(PageInfo)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(PageInfo)gql"sv, typePageInfo);
	auto typeAppointmentEdge = schema::ObjectType::Make(R"gql(AppointmentEdge)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(AppointmentEdge)gql"sv, typeAppointmentEdge);
	auto typeAppointmentConnection = schema::ObjectType::Make(R"gql(AppointmentConnection)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(AppointmentConnection)gql"sv, typeAppointmentConnection);
	auto typeTaskEdge = schema::ObjectType::Make(R"gql(TaskEdge)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(TaskEdge)gql"sv, typeTaskEdge);
	auto typeTaskConnection = schema::ObjectType::Make(R"gql(TaskConnection)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(TaskConnection)gql"sv, typeTaskConnection);
	auto typeFolderEdge = schema::ObjectType::Make(R"gql(FolderEdge)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(FolderEdge)gql"sv, typeFolderEdge);
	auto typeFolderConnection = schema::ObjectType::Make(R"gql(FolderConnection)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(FolderConnection)gql"sv, typeFolderConnection);
	auto typeCompleteTaskPayload = schema::ObjectType::Make(R"gql(CompleteTaskPayload)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(CompleteTaskPayload)gql"sv, typeCompleteTaskPayload);
	auto typeMutation = schema::ObjectType::Make(R"gql(Mutation)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(Mutation)gql"sv, typeMutation);
	auto typeSubscription = schema::ObjectType::Make(R"gql(Subscription)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(Subscription)gql"sv, typeSubscription);
	auto typeAppointment = schema::ObjectType::Make(R"gql(Appointment)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(Appointment)gql"sv, typeAppointment);
	auto typeTask = schema::ObjectType::Make(R"gql(Task)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(Task)gql"sv, typeTask);
	auto typeFolder = schema::ObjectType::Make(R"gql(Folder)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(Folder)gql"sv, typeFolder);
	auto typeNestedType = schema::ObjectType::Make(R"gql(NestedType)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(NestedType)gql"sv, typeNestedType);
	auto typeExpensive = schema::ObjectType::Make(R"gql(Expensive)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(Expensive)gql"sv, typeExpensive);

	typeTaskState->AddEnumValues({
		{ service::s_namesTaskState[static_cast<size_t>(today::TaskState::Unassigned)], R"md()md"sv, std::make_optional(R"md(Need to deprecate an [enum value](https://spec.graphql.org/October2021/#sec-Schema-Introspection.Deprecation))md"sv) },
		{ service::s_namesTaskState[static_cast<size_t>(today::TaskState::New)], R"md()md"sv, std::nullopt },
		{ service::s_namesTaskState[static_cast<size_t>(today::TaskState::Started)], R"md()md"sv, std::nullopt },
		{ service::s_namesTaskState[static_cast<size_t>(today::TaskState::Complete)], R"md()md"sv, std::nullopt }
	});

	typeCompleteTaskInput->AddInputValues({
		schema::InputValue::Make(R"gql(id)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(ID)gql"sv)), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(testTaskState)gql"sv, R"md()md"sv, schema->LookupType(R"gql(TaskState)gql"sv), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(isComplete)gql"sv, R"md()md"sv, schema->LookupType(R"gql(Boolean)gql"sv), R"gql(true)gql"sv),
		schema::InputValue::Make(R"gql(clientMutationId)gql"sv, R"md()md"sv, schema->LookupType(R"gql(String)gql"sv), R"gql()gql"sv)
	});
	typeThirdNestedInput->AddInputValues({
		schema::InputValue::Make(R"gql(id)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(ID)gql"sv)), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(second)gql"sv, R"md()md"sv, schema->LookupType(R"gql(SecondNestedInput)gql"sv), R"gql()gql"sv)
	});
	typeFourthNestedInput->AddInputValues({
		schema::InputValue::Make(R"gql(id)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(ID)gql"sv)), R"gql()gql"sv)
	});
	typeIncludeNullableSelfInput->AddInputValues({
		schema::InputValue::Make(R"gql(self)gql"sv, R"md()md"sv, schema->LookupType(R"gql(IncludeNullableSelfInput)gql"sv), R"gql()gql"sv)
	});
	typeIncludeNonNullableListSelfInput->AddInputValues({
		schema::InputValue::Make(R"gql(selves)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(IncludeNonNullableListSelfInput)gql"sv)))), R"gql()gql"sv)
	});
	typeStringOperationFilterInput->AddInputValues({
		schema::InputValue::Make(R"gql(and)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(StringOperationFilterInput)gql"sv))), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(or)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(StringOperationFilterInput)gql"sv))), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(equal)gql"sv, R"md()md"sv, schema->LookupType(R"gql(String)gql"sv), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(notEqual)gql"sv, R"md()md"sv, schema->LookupType(R"gql(String)gql"sv), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(contains)gql"sv, R"md()md"sv, schema->LookupType(R"gql(String)gql"sv), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(notContains)gql"sv, R"md()md"sv, schema->LookupType(R"gql(String)gql"sv), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(in)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv))), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(notIn)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv))), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(startsWith)gql"sv, R"md()md"sv, schema->LookupType(R"gql(String)gql"sv), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(notStartsWith)gql"sv, R"md()md"sv, schema->LookupType(R"gql(String)gql"sv), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(endsWith)gql"sv, R"md()md"sv, schema->LookupType(R"gql(String)gql"sv), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(notEndsWith)gql"sv, R"md()md"sv, schema->LookupType(R"gql(String)gql"sv), R"gql()gql"sv)
	});
	typeSecondNestedInput->AddInputValues({
		schema::InputValue::Make(R"gql(id)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(ID)gql"sv)), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(third)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(ThirdNestedInput)gql"sv)), R"gql()gql"sv)
	});
	typeForwardDeclaredInput->AddInputValues({
		schema::InputValue::Make(R"gql(nullableSelf)gql"sv, R"md()md"sv, schema->LookupType(R"gql(IncludeNullableSelfInput)gql"sv), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(listSelves)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(IncludeNonNullableListSelfInput)gql"sv)), R"gql()gql"sv)
	});
	typeFirstNestedInput->AddInputValues({
		schema::InputValue::Make(R"gql(id)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(ID)gql"sv)), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(second)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(SecondNestedInput)gql"sv)), R"gql()gql"sv),
		schema::InputValue::Make(R"gql(third)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(ThirdNestedInput)gql"sv)), R"gql()gql"sv)
	});

	AddNodeDetails(typeNode, schema);

	AddUnionTypeDetails(typeUnionType, schema);

	AddQueryDetails(typeQuery, schema);
	AddPageInfoDetails(typePageInfo, schema);
	AddAppointmentEdgeDetails(typeAppointmentEdge, schema);
	AddAppointmentConnectionDetails(typeAppointmentConnection, schema);
	AddTaskEdgeDetails(typeTaskEdge, schema);
	AddTaskConnectionDetails(typeTaskConnection, schema);
	AddFolderEdgeDetails(typeFolderEdge, schema);
	AddFolderConnectionDetails(typeFolderConnection, schema);
	AddCompleteTaskPayloadDetails(typeCompleteTaskPayload, schema);
	AddMutationDetails(typeMutation, schema);
	AddSubscriptionDetails(typeSubscription, schema);
	AddAppointmentDetails(typeAppointment, schema);
	AddTaskDetails(typeTask, schema);
	AddFolderDetails(typeFolder, schema);
	AddNestedTypeDetails(typeNestedType, schema);
	AddExpensiveDetails(typeExpensive, schema);

	schema->AddDirective(schema::Directive::Make(R"gql(id)gql"sv, R"md()md"sv, {
		introspection::DirectiveLocation::FIELD_DEFINITION
	}, {}, false));
	schema->AddDirective(schema::Directive::Make(R"gql(queryTag)gql"sv, R"md()md"sv, {
		introspection::DirectiveLocation::QUERY
	}, {
		schema::InputValue::Make(R"gql(query)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv)), R"gql()gql"sv)
	}, false));
	schema->AddDirective(schema::Directive::Make(R"gql(fieldTag)gql"sv, R"md()md"sv, {
		introspection::DirectiveLocation::FIELD
	}, {
		schema::InputValue::Make(R"gql(field)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv)), R"gql()gql"sv)
	}, false));
	schema->AddDirective(schema::Directive::Make(R"gql(fragmentDefinitionTag)gql"sv, R"md()md"sv, {
		introspection::DirectiveLocation::FRAGMENT_DEFINITION
	}, {
		schema::InputValue::Make(R"gql(fragmentDefinition)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv)), R"gql()gql"sv)
	}, false));
	schema->AddDirective(schema::Directive::Make(R"gql(fragmentSpreadTag)gql"sv, R"md()md"sv, {
		introspection::DirectiveLocation::FRAGMENT_SPREAD
	}, {
		schema::InputValue::Make(R"gql(fragmentSpread)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv)), R"gql()gql"sv)
	}, false));
	schema->AddDirective(schema::Directive::Make(R"gql(inlineFragmentTag)gql"sv, R"md()md"sv, {
		introspection::DirectiveLocation::INLINE_FRAGMENT
	}, {
		schema::InputValue::Make(R"gql(inlineFragment)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv)), R"gql()gql"sv)
	}, false));
	schema->AddDirective(schema::Directive::Make(R"gql(repeatableOnField)gql"sv, R"md()md"sv, {
		introspection::DirectiveLocation::FIELD
	}, {}, true));

	schema->AddQueryType(typeQuery);
	schema->AddMutationType(typeMutation);
	schema->AddSubscriptionType(typeSubscription);
}

std::shared_ptr<schema::Schema> GetSchema()
{
	static std::weak_ptr<schema::Schema> s_wpSchema;
	auto schema = s_wpSchema.lock();

	if (!schema)
	{
		schema = std::make_shared<schema::Schema>(true, R"md()md"sv);
		introspection::AddTypesToSchema(schema);
		AddTypesToSchema(schema);
		s_wpSchema = schema;
	}

	return schema;
}

} // namespace today
} // namespace graphql
