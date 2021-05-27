// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef MUTATECLIENT_H
#define MUTATECLIENT_H

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "graphqlservice/GraphQLParse.h"
#include "graphqlservice/GraphQLResponse.h"

#include "graphqlservice/internal/Version.h"

// Check if the library version is compatible with clientgen 3.6.0
static_assert(graphql::internal::MajorVersion == 3, "regenerate with clientgen: major version mismatch");
static_assert(graphql::internal::MinorVersion == 6, "regenerate with clientgen: minor version mismatch");

#include <optional>
#include <string>
#include <vector>

/// <summary>
/// Operation: mutation CompleteTaskMutation
/// </summary>
/// <code class="language-graphql">
/// # Copyright (c) Microsoft Corporation. All rights reserved.
/// # Licensed under the MIT License.
/// 
/// mutation CompleteTaskMutation($input: CompleteTaskInput! = {id: "ZmFrZVRhc2tJZA==", isComplete: true, clientMutationId: "Hi There!"}) {
///   completedTask: completeTask(input: $input) {
///     completedTask: task {
///       completedTaskId: id
///       title
///       isComplete
///     }
///     clientMutationId
///   }
/// }
/// </code>
namespace graphql::client::mutation::CompleteTaskMutation {

// Return the original text of the request document.
const std::string& GetRequestText() noexcept;

// Return a pre-parsed, pre-validated request object.
const peg::ast& GetRequestObject() noexcept;

enum class TaskState
{
	New,
	Started,
	Complete,
	Unassigned,
};

struct Variables
{
	struct CompleteTaskInput
	{
		response::IdType id {};
		std::optional<TaskState> testTaskState {};
		std::optional<response::BooleanType> isComplete {};
		std::optional<response::StringType> clientMutationId {};
	};

	CompleteTaskInput input {};
};

response::Value serializeVariables(Variables&& variables);

struct Response
{
	struct completedTask_CompleteTaskPayload
	{
		struct completedTask_Task
		{
			response::IdType completedTaskId {};
			std::optional<response::StringType> title {};
			response::BooleanType isComplete {};
		};

		std::optional<completedTask_Task> completedTask {};
		std::optional<response::StringType> clientMutationId {};
	};

	completedTask_CompleteTaskPayload completedTask {};
};

Response parseResponse(response::Value&& response);

} // namespace graphql::client::mutation::CompleteTaskMutation

#endif // MUTATECLIENT_H
