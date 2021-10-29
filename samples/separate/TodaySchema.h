// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef TODAYSCHEMA_H
#define TODAYSCHEMA_H

#include "graphqlservice/internal/Schema.h"

// Check if the library version is compatible with schemagen 4.0.0
static_assert(graphql::internal::MajorVersion == 4, "regenerate with schemagen: major version mismatch");
static_assert(graphql::internal::MinorVersion == 0, "regenerate with schemagen: minor version mismatch");

#include <memory>
#include <string>
#include <vector>

namespace graphql {
namespace today {

enum class TaskState
{
	New,
	Started,
	Complete,
	Unassigned
};

struct CompleteTaskInput
{
	response::IdType id;
	std::optional<TaskState> testTaskState;
	std::optional<response::BooleanType> isComplete;
	std::optional<response::StringType> clientMutationId;
};

struct ThirdNestedInput
{
	response::IdType id;
};

struct FourthNestedInput
{
	response::IdType id;
};

struct SecondNestedInput
{
	response::IdType id;
	ThirdNestedInput third;
};

struct FirstNestedInput
{
	response::IdType id;
	SecondNestedInput second;
	ThirdNestedInput third;
};

namespace object {

class Query;
class PageInfo;
class AppointmentEdge;
class AppointmentConnection;
class TaskEdge;
class TaskConnection;
class FolderEdge;
class FolderConnection;
class CompleteTaskPayload;
class Mutation;
class Subscription;
class Appointment;
class Task;
class Folder;
class NestedType;
class Expensive;

} // namespace object

struct Node
{
	virtual service::FieldResult<response::IdType> getId(service::FieldParams&& params) const = 0;
};

class Operations
	: public service::Request
{
public:
	explicit Operations(std::shared_ptr<object::Query> query, std::shared_ptr<object::Mutation> mutation, std::shared_ptr<object::Subscription> subscription);

private:
	std::shared_ptr<object::Query> _query;
	std::shared_ptr<object::Mutation> _mutation;
	std::shared_ptr<object::Subscription> _subscription;
};

void AddQueryDetails(std::shared_ptr<schema::ObjectType> typeQuery, const std::shared_ptr<schema::Schema>& schema);
void AddPageInfoDetails(std::shared_ptr<schema::ObjectType> typePageInfo, const std::shared_ptr<schema::Schema>& schema);
void AddAppointmentEdgeDetails(std::shared_ptr<schema::ObjectType> typeAppointmentEdge, const std::shared_ptr<schema::Schema>& schema);
void AddAppointmentConnectionDetails(std::shared_ptr<schema::ObjectType> typeAppointmentConnection, const std::shared_ptr<schema::Schema>& schema);
void AddTaskEdgeDetails(std::shared_ptr<schema::ObjectType> typeTaskEdge, const std::shared_ptr<schema::Schema>& schema);
void AddTaskConnectionDetails(std::shared_ptr<schema::ObjectType> typeTaskConnection, const std::shared_ptr<schema::Schema>& schema);
void AddFolderEdgeDetails(std::shared_ptr<schema::ObjectType> typeFolderEdge, const std::shared_ptr<schema::Schema>& schema);
void AddFolderConnectionDetails(std::shared_ptr<schema::ObjectType> typeFolderConnection, const std::shared_ptr<schema::Schema>& schema);
void AddCompleteTaskPayloadDetails(std::shared_ptr<schema::ObjectType> typeCompleteTaskPayload, const std::shared_ptr<schema::Schema>& schema);
void AddMutationDetails(std::shared_ptr<schema::ObjectType> typeMutation, const std::shared_ptr<schema::Schema>& schema);
void AddSubscriptionDetails(std::shared_ptr<schema::ObjectType> typeSubscription, const std::shared_ptr<schema::Schema>& schema);
void AddAppointmentDetails(std::shared_ptr<schema::ObjectType> typeAppointment, const std::shared_ptr<schema::Schema>& schema);
void AddTaskDetails(std::shared_ptr<schema::ObjectType> typeTask, const std::shared_ptr<schema::Schema>& schema);
void AddFolderDetails(std::shared_ptr<schema::ObjectType> typeFolder, const std::shared_ptr<schema::Schema>& schema);
void AddNestedTypeDetails(std::shared_ptr<schema::ObjectType> typeNestedType, const std::shared_ptr<schema::Schema>& schema);
void AddExpensiveDetails(std::shared_ptr<schema::ObjectType> typeExpensive, const std::shared_ptr<schema::Schema>& schema);

std::shared_ptr<schema::Schema> GetSchema();

} // namespace today
} // namespace graphql

#endif // TODAYSCHEMA_H
