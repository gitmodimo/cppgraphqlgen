// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef QUERYCLIENT_H
#define QUERYCLIENT_H

#include "graphqlservice/GraphQLClient.h"
#include "graphqlservice/GraphQLParse.h"
#include "graphqlservice/GraphQLResponse.h"

#include "graphqlservice/internal/Version.h"

#include <optional>
#include <string>
#include <vector>

// Check if the library version is compatible with clientgen 5.0.0
static_assert(graphql::internal::MajorVersion == 5, "regenerate with clientgen: major version mismatch");
static_assert(graphql::internal::MinorVersion == 0, "regenerate with clientgen: minor version mismatch");

namespace graphql::query {

/// # Operation: query (unnamed)
/// ```graphql
/// # Copyright (c) Microsoft Corporation. All rights reserved.
/// # Licensed under the MIT License.
/// 
/// query {
///   appointments {
///     edges {
///       node {
///         id
///         subject
///         when
///         isNow
///         __typename
///       }
///     }
///   }
///   tasks {
///     edges {
///       node {
///         id
///         title
///         isComplete
///         __typename
///       }
///     }
///   }
///   unreadCounts {
///     edges {
///       node {
///         id
///         name
///         unreadCount
///         __typename
///       }
///     }
///   }
/// 
///   # Read a field with an enum type
///   testTaskState
/// 
///   # Try a field with a union type
///   anyType(ids: ["ZmFrZVRhc2tJZA=="]) {
///     __typename
///     ...on Node {
///       id
///     }
///     ...on Task {
///       id
///       title
///       isComplete
///     }
///     ...on Appointment {
///       id
///       subject
///       when
///       isNow
///       array
///     }
///   }
/// 
///   # Try a field with a C++ keyword
///   default
/// }
/// ```
namespace client {

// Return the original text of the request document.
[[nodiscard("unnecessary call")]] const std::string& GetRequestText() noexcept;

// Return a pre-parsed, pre-validated request object.
[[nodiscard("unnecessary call")]] const peg::ast& GetRequestObject() noexcept;

} // namespace client

enum class [[nodiscard("unnecessary conversion")]] TaskState
{
	Unassigned,
	New,
	Started,
	Complete,
};

namespace client {

namespace query::Query {

using graphql::query::client::GetRequestText;
using graphql::query::client::GetRequestObject;

// Return the name of this operation in the shared request document.
[[nodiscard("unnecessary call")]] const std::string& GetOperationName() noexcept;

using graphql::query::TaskState;

struct [[nodiscard("unnecessary construction")]] Response
{
	struct [[nodiscard("unnecessary construction")]] appointments_AppointmentConnection
	{
		struct [[nodiscard("unnecessary construction")]] edges_AppointmentEdge
		{
			struct [[nodiscard("unnecessary construction")]] node_Appointment
			{
				response::IdType id {};
				std::optional<std::string> subject {};
				std::optional<response::Value> when {};
				bool isNow {};
				std::string _typename {};
			};

			std::optional<node_Appointment> node {};
		};

		std::optional<std::vector<std::optional<edges_AppointmentEdge>>> edges {};
	};

	struct [[nodiscard("unnecessary construction")]] tasks_TaskConnection
	{
		struct [[nodiscard("unnecessary construction")]] edges_TaskEdge
		{
			struct [[nodiscard("unnecessary construction")]] node_Task
			{
				response::IdType id {};
				std::optional<std::string> title {};
				bool isComplete {};
				std::string _typename {};
			};

			std::optional<node_Task> node {};
		};

		std::optional<std::vector<std::optional<edges_TaskEdge>>> edges {};
	};

	struct [[nodiscard("unnecessary construction")]] unreadCounts_FolderConnection
	{
		struct [[nodiscard("unnecessary construction")]] edges_FolderEdge
		{
			struct [[nodiscard("unnecessary construction")]] node_Folder
			{
				response::IdType id {};
				std::optional<std::string> name {};
				int unreadCount {};
				std::string _typename {};
			};

			std::optional<node_Folder> node {};
		};

		std::optional<std::vector<std::optional<edges_FolderEdge>>> edges {};
	};

	struct [[nodiscard("unnecessary construction")]] anyType_UnionType
	{
		std::string _typename {};
		response::IdType id {};
		std::optional<std::string> title {};
		bool isComplete {};
		std::optional<std::string> subject {};
		std::optional<response::Value> when {};
		bool isNow {};
		std::vector<response::IdType> array {};
	};

	appointments_AppointmentConnection appointments {};
	tasks_TaskConnection tasks {};
	unreadCounts_FolderConnection unreadCounts {};
	TaskState testTaskState {};
	std::vector<std::optional<anyType_UnionType>> anyType {};
	std::optional<std::string> default_ {};
};

class ResponseVisitor
	: public std::enable_shared_from_this<ResponseVisitor>
{
public:
	ResponseVisitor() noexcept;
	~ResponseVisitor();

	void add_value(std::shared_ptr<const response::Value>&&);
	void reserve(std::size_t count);
	void start_object();
	void add_member(std::string&& key);
	void end_object();
	void start_array();
	void end_array();
	void add_null();
	void add_string(std::string&& value);
	void add_enum(std::string&& value);
	void add_id(response::IdType&& value);
	void add_bool(bool value);
	void add_int(int value);
	void add_float(double value);
	void complete();

	Response response();

private:
	struct impl;

	std::unique_ptr<impl> _pimpl;
};

[[nodiscard("unnecessary conversion")]] Response parseResponse(response::Value&& response);

struct Traits
{
	[[nodiscard("unnecessary call")]] static const std::string& GetRequestText() noexcept;
	[[nodiscard("unnecessary call")]] static const peg::ast& GetRequestObject() noexcept;
	[[nodiscard("unnecessary call")]] static const std::string& GetOperationName() noexcept;

	using Response = Query::Response;
	using ResponseVisitor = Query::ResponseVisitor;

	[[nodiscard("unnecessary conversion")]] static Response parseResponse(response::Value&& response);
};

} // namespace query::Query
} // namespace client
} // namespace graphql::query

#endif // QUERYCLIENT_H
