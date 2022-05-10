// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "MultipleQueriesClient.h"

#include "graphqlservice/internal/SortedMap.h"

#include <algorithm>
#include <array>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <utility>

using namespace std::literals;

namespace graphql::client {
namespace multiple {

const std::string& GetRequestText() noexcept
{
	static const auto s_request = R"gql(
		# Copyright (c) Microsoft Corporation. All rights reserved.
		# Licensed under the MIT License.
		
		query Appointments {
		  appointments {
		    edges {
		      node {
		        id
		        subject
		        when
		        isNow
		        __typename
		      }
		    }
		  }
		}
		
		query Tasks {
		  tasks {
		    edges {
		      node {
		        id
		        title
		        isComplete
		        __typename
		      }
		    }
		  }
		}
		
		query UnreadCounts {
		  unreadCounts {
		    edges {
		      node {
		        id
		        name
		        unreadCount
		        __typename
		      }
		    }
		  }
		}
		
		query Miscellaneous {
		  # Read a field with an enum type
		  testTaskState
		
		  # Try a field with a union type
		  anyType(ids: ["ZmFrZVRhc2tJZA=="]) {
		    __typename
		    ...on Node {
		      id
		    }
		    ...on Task {
		      id
		      title
		      isComplete
		    }
		    ...on Appointment {
		      id
		      subject
		      when
		      isNow
		    }
		  }
		
		  # Try a field with a C++ keyword
		  default
		}
		
		mutation CompleteTaskMutation($input: CompleteTaskInput = {id: "ZmFrZVRhc2tJZA==", isComplete: true, clientMutationId: "Hi There!"}, $skipClientMutationId: Boolean!) {
		  completedTask: completeTask(input: $input) {
		    completedTask: task {
		      completedTaskId: id
		      title
		      isComplete
		    }
		    clientMutationId @skip(if: $skipClientMutationId)
		  }
		}
	)gql"s;

	return s_request;
}

const peg::ast& GetRequestObject() noexcept
{
	static const auto s_request = []() noexcept {
		auto ast = peg::parseString(GetRequestText());

		// This has already been validated against the schema by clientgen.
		ast.validated = true;

		return ast;
	}();

	return s_request;
}

static const std::array<std::string_view, 4> s_namesTaskState = {
	R"gql(New)gql"sv,
	R"gql(Started)gql"sv,
	R"gql(Complete)gql"sv,
	R"gql(Unassigned)gql"sv
};

static const std::array<std::pair<std::string_view, TaskState>, 4> s_valuesTaskState = {
	std::make_pair(R"gql(New)gql"sv, TaskState::New),
	std::make_pair(R"gql(Started)gql"sv, TaskState::Started),
	std::make_pair(R"gql(Complete)gql"sv, TaskState::Complete),
	std::make_pair(R"gql(Unassigned)gql"sv, TaskState::Unassigned)
};

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
	: id { ModifiedVariable<response::IdType>::duplicate(other.id) }
	, testTaskState { ModifiedVariable<TaskState>::duplicate<TypeModifier::Nullable>(other.testTaskState) }
	, isComplete { ModifiedVariable<bool>::duplicate<TypeModifier::Nullable>(other.isComplete) }
	, clientMutationId { ModifiedVariable<std::string>::duplicate<TypeModifier::Nullable>(other.clientMutationId) }
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

} // namespace multiple

using namespace multiple;

template <>
query::Appointments::Response::appointments_AppointmentConnection::edges_AppointmentEdge::node_Appointment ModifiedResponse<query::Appointments::Response::appointments_AppointmentConnection::edges_AppointmentEdge::node_Appointment>::parse(response::Value&& response)
{
	query::Appointments::Response::appointments_AppointmentConnection::edges_AppointmentEdge::node_Appointment result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(id)js"sv)
			{
				result.id = ModifiedResponse<response::IdType>::parse(std::move(member.second));
				continue;
			}
			if (member.first == R"js(subject)js"sv)
			{
				result.subject = ModifiedResponse<std::string>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
			if (member.first == R"js(when)js"sv)
			{
				result.when = ModifiedResponse<response::Value>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
			if (member.first == R"js(isNow)js"sv)
			{
				result.isNow = ModifiedResponse<bool>::parse(std::move(member.second));
				continue;
			}
			if (member.first == R"js(__typename)js"sv)
			{
				result._typename = ModifiedResponse<std::string>::parse(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

template <>
query::Appointments::Response::appointments_AppointmentConnection::edges_AppointmentEdge ModifiedResponse<query::Appointments::Response::appointments_AppointmentConnection::edges_AppointmentEdge>::parse(response::Value&& response)
{
	query::Appointments::Response::appointments_AppointmentConnection::edges_AppointmentEdge result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(node)js"sv)
			{
				result.node = ModifiedResponse<query::Appointments::Response::appointments_AppointmentConnection::edges_AppointmentEdge::node_Appointment>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

template <>
query::Appointments::Response::appointments_AppointmentConnection ModifiedResponse<query::Appointments::Response::appointments_AppointmentConnection>::parse(response::Value&& response)
{
	query::Appointments::Response::appointments_AppointmentConnection result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(edges)js"sv)
			{
				result.edges = ModifiedResponse<query::Appointments::Response::appointments_AppointmentConnection::edges_AppointmentEdge>::parse<TypeModifier::Nullable, TypeModifier::List, TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

namespace query::Appointments {

const std::string& GetOperationName() noexcept
{
	static const auto s_name = R"gql(Appointments)gql"s;

	return s_name;
}

Response parseResponse(response::Value&& response)
{
	Response result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(appointments)js"sv)
			{
				result.appointments = ModifiedResponse<query::Appointments::Response::appointments_AppointmentConnection>::parse(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

} // namespace query::Appointments

template <>
query::Tasks::Response::tasks_TaskConnection::edges_TaskEdge::node_Task ModifiedResponse<query::Tasks::Response::tasks_TaskConnection::edges_TaskEdge::node_Task>::parse(response::Value&& response)
{
	query::Tasks::Response::tasks_TaskConnection::edges_TaskEdge::node_Task result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(id)js"sv)
			{
				result.id = ModifiedResponse<response::IdType>::parse(std::move(member.second));
				continue;
			}
			if (member.first == R"js(title)js"sv)
			{
				result.title = ModifiedResponse<std::string>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
			if (member.first == R"js(isComplete)js"sv)
			{
				result.isComplete = ModifiedResponse<bool>::parse(std::move(member.second));
				continue;
			}
			if (member.first == R"js(__typename)js"sv)
			{
				result._typename = ModifiedResponse<std::string>::parse(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

template <>
query::Tasks::Response::tasks_TaskConnection::edges_TaskEdge ModifiedResponse<query::Tasks::Response::tasks_TaskConnection::edges_TaskEdge>::parse(response::Value&& response)
{
	query::Tasks::Response::tasks_TaskConnection::edges_TaskEdge result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(node)js"sv)
			{
				result.node = ModifiedResponse<query::Tasks::Response::tasks_TaskConnection::edges_TaskEdge::node_Task>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

template <>
query::Tasks::Response::tasks_TaskConnection ModifiedResponse<query::Tasks::Response::tasks_TaskConnection>::parse(response::Value&& response)
{
	query::Tasks::Response::tasks_TaskConnection result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(edges)js"sv)
			{
				result.edges = ModifiedResponse<query::Tasks::Response::tasks_TaskConnection::edges_TaskEdge>::parse<TypeModifier::Nullable, TypeModifier::List, TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

namespace query::Tasks {

const std::string& GetOperationName() noexcept
{
	static const auto s_name = R"gql(Tasks)gql"s;

	return s_name;
}

Response parseResponse(response::Value&& response)
{
	Response result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(tasks)js"sv)
			{
				result.tasks = ModifiedResponse<query::Tasks::Response::tasks_TaskConnection>::parse(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

} // namespace query::Tasks

template <>
query::UnreadCounts::Response::unreadCounts_FolderConnection::edges_FolderEdge::node_Folder ModifiedResponse<query::UnreadCounts::Response::unreadCounts_FolderConnection::edges_FolderEdge::node_Folder>::parse(response::Value&& response)
{
	query::UnreadCounts::Response::unreadCounts_FolderConnection::edges_FolderEdge::node_Folder result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(id)js"sv)
			{
				result.id = ModifiedResponse<response::IdType>::parse(std::move(member.second));
				continue;
			}
			if (member.first == R"js(name)js"sv)
			{
				result.name = ModifiedResponse<std::string>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
			if (member.first == R"js(unreadCount)js"sv)
			{
				result.unreadCount = ModifiedResponse<int>::parse(std::move(member.second));
				continue;
			}
			if (member.first == R"js(__typename)js"sv)
			{
				result._typename = ModifiedResponse<std::string>::parse(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

template <>
query::UnreadCounts::Response::unreadCounts_FolderConnection::edges_FolderEdge ModifiedResponse<query::UnreadCounts::Response::unreadCounts_FolderConnection::edges_FolderEdge>::parse(response::Value&& response)
{
	query::UnreadCounts::Response::unreadCounts_FolderConnection::edges_FolderEdge result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(node)js"sv)
			{
				result.node = ModifiedResponse<query::UnreadCounts::Response::unreadCounts_FolderConnection::edges_FolderEdge::node_Folder>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

template <>
query::UnreadCounts::Response::unreadCounts_FolderConnection ModifiedResponse<query::UnreadCounts::Response::unreadCounts_FolderConnection>::parse(response::Value&& response)
{
	query::UnreadCounts::Response::unreadCounts_FolderConnection result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(edges)js"sv)
			{
				result.edges = ModifiedResponse<query::UnreadCounts::Response::unreadCounts_FolderConnection::edges_FolderEdge>::parse<TypeModifier::Nullable, TypeModifier::List, TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

namespace query::UnreadCounts {

const std::string& GetOperationName() noexcept
{
	static const auto s_name = R"gql(UnreadCounts)gql"s;

	return s_name;
}

Response parseResponse(response::Value&& response)
{
	Response result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(unreadCounts)js"sv)
			{
				result.unreadCounts = ModifiedResponse<query::UnreadCounts::Response::unreadCounts_FolderConnection>::parse(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

} // namespace query::UnreadCounts

template <>
TaskState ModifiedResponse<TaskState>::parse(response::Value&& value)
{
	if (!value.maybe_enum())
	{
		throw std::logic_error { R"ex(not a valid TaskState value)ex" };
	}

	const auto result = internal::sorted_map_lookup<internal::shorter_or_less>(
		s_valuesTaskState,
		std::string_view { value.get<std::string>() });

	if (!result)
	{
		throw std::logic_error { R"ex(not a valid TaskState value)ex" };
	}

	return *result;
}

template <>
query::Miscellaneous::Response::anyType_UnionType ModifiedResponse<query::Miscellaneous::Response::anyType_UnionType>::parse(response::Value&& response)
{
	query::Miscellaneous::Response::anyType_UnionType result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(__typename)js"sv)
			{
				result._typename = ModifiedResponse<std::string>::parse(std::move(member.second));
				continue;
			}
			if (member.first == R"js(id)js"sv)
			{
				result.id = ModifiedResponse<response::IdType>::parse(std::move(member.second));
				continue;
			}
			if (member.first == R"js(title)js"sv)
			{
				result.title = ModifiedResponse<std::string>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
			if (member.first == R"js(isComplete)js"sv)
			{
				result.isComplete = ModifiedResponse<bool>::parse(std::move(member.second));
				continue;
			}
			if (member.first == R"js(subject)js"sv)
			{
				result.subject = ModifiedResponse<std::string>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
			if (member.first == R"js(when)js"sv)
			{
				result.when = ModifiedResponse<response::Value>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
			if (member.first == R"js(isNow)js"sv)
			{
				result.isNow = ModifiedResponse<bool>::parse(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

namespace query::Miscellaneous {

const std::string& GetOperationName() noexcept
{
	static const auto s_name = R"gql(Miscellaneous)gql"s;

	return s_name;
}

Response parseResponse(response::Value&& response)
{
	Response result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(testTaskState)js"sv)
			{
				result.testTaskState = ModifiedResponse<TaskState>::parse(std::move(member.second));
				continue;
			}
			if (member.first == R"js(anyType)js"sv)
			{
				result.anyType = ModifiedResponse<query::Miscellaneous::Response::anyType_UnionType>::parse<TypeModifier::List, TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
			if (member.first == R"js(default)js"sv)
			{
				result.default_ = ModifiedResponse<std::string>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

} // namespace query::Miscellaneous

template <>
response::Value ModifiedVariable<TaskState>::serialize(TaskState&& value)
{
	response::Value result { response::Type::EnumValue };

	result.set<std::string>(std::string { s_namesTaskState[static_cast<size_t>(value)] });

	return result;
}

template <>
response::Value ModifiedVariable<CompleteTaskInput>::serialize(CompleteTaskInput&& inputValue)
{
	response::Value result { response::Type::Map };

	result.emplace_back(R"js(id)js"s, ModifiedVariable<response::IdType>::serialize(std::move(inputValue.id)));
	result.emplace_back(R"js(testTaskState)js"s, ModifiedVariable<TaskState>::serialize<TypeModifier::Nullable>(std::move(inputValue.testTaskState)));
	result.emplace_back(R"js(isComplete)js"s, ModifiedVariable<bool>::serialize<TypeModifier::Nullable>(std::move(inputValue.isComplete)));
	result.emplace_back(R"js(clientMutationId)js"s, ModifiedVariable<std::string>::serialize<TypeModifier::Nullable>(std::move(inputValue.clientMutationId)));

	return result;
}

template <>
mutation::CompleteTaskMutation::Response::completedTask_CompleteTaskPayload::completedTask_Task ModifiedResponse<mutation::CompleteTaskMutation::Response::completedTask_CompleteTaskPayload::completedTask_Task>::parse(response::Value&& response)
{
	mutation::CompleteTaskMutation::Response::completedTask_CompleteTaskPayload::completedTask_Task result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(completedTaskId)js"sv)
			{
				result.completedTaskId = ModifiedResponse<response::IdType>::parse(std::move(member.second));
				continue;
			}
			if (member.first == R"js(title)js"sv)
			{
				result.title = ModifiedResponse<std::string>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
			if (member.first == R"js(isComplete)js"sv)
			{
				result.isComplete = ModifiedResponse<bool>::parse(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

template <>
mutation::CompleteTaskMutation::Response::completedTask_CompleteTaskPayload ModifiedResponse<mutation::CompleteTaskMutation::Response::completedTask_CompleteTaskPayload>::parse(response::Value&& response)
{
	mutation::CompleteTaskMutation::Response::completedTask_CompleteTaskPayload result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(completedTask)js"sv)
			{
				result.completedTask = ModifiedResponse<mutation::CompleteTaskMutation::Response::completedTask_CompleteTaskPayload::completedTask_Task>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
			if (member.first == R"js(clientMutationId)js"sv)
			{
				result.clientMutationId = ModifiedResponse<std::string>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

namespace mutation::CompleteTaskMutation {

const std::string& GetOperationName() noexcept
{
	static const auto s_name = R"gql(CompleteTaskMutation)gql"s;

	return s_name;
}

response::Value serializeVariables(Variables&& variables)
{
	response::Value result { response::Type::Map };

	result.emplace_back(R"js(input)js"s, ModifiedVariable<CompleteTaskInput>::serialize<TypeModifier::Nullable>(std::move(variables.input)));
	result.emplace_back(R"js(skipClientMutationId)js"s, ModifiedVariable<bool>::serialize(std::move(variables.skipClientMutationId)));

	return result;
}

Response parseResponse(response::Value&& response)
{
	Response result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(completedTask)js"sv)
			{
				result.completedTask = ModifiedResponse<mutation::CompleteTaskMutation::Response::completedTask_CompleteTaskPayload>::parse(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

} // namespace mutation::CompleteTaskMutation
} // namespace graphql::client
