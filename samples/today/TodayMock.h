// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ifndef TODAYMOCK_H
#define TODAYMOCK_H

#include "TodaySchema.h"

#include "TodayAppointmentEdgeObject.h"
#include "TodayAppointmentObject.h"
#include "TodayFolderEdgeObject.h"
#include "TodayFolderObject.h"
#include "TodayMutationObject.h"
#include "TodayNodeObject.h"
#include "TodayPageInfoObject.h"
#include "TodayQueryObject.h"
#include "TodaySubscriptionObject.h"
#include "TodayTaskEdgeObject.h"
#include "TodayTaskObject.h"

#include <atomic>
#include <memory>
#include <stack>

namespace graphql::today {

// These IDs are hard-coded in every test which uses TodayMock.
const response::IdType& getFakeAppointmentId() noexcept;
const response::IdType& getFakeTaskId() noexcept;
const response::IdType& getFakeFolderId() noexcept;

struct TodayMockService
{
	std::shared_ptr<Operations> service {};
	std::size_t getAppointmentsCount {};
	std::size_t getTasksCount {};
	std::size_t getUnreadCountsCount {};
};

std::shared_ptr<TodayMockService> mock_service() noexcept;

struct RequestState : service::RequestState
{
	RequestState(std::size_t id);

	const std::size_t requestId;

	std::size_t appointmentsRequestId = 0;
	std::size_t tasksRequestId = 0;
	std::size_t unreadCountsRequestId = 0;

	std::size_t loadAppointmentsCount = 0;
	std::size_t loadTasksCount = 0;
	std::size_t loadUnreadCountsCount = 0;
};

class Appointment;
class Task;
class Folder;
class Expensive;

class Query : public std::enable_shared_from_this<Query>
{
public:
	using appointmentsLoader = std::function<std::vector<std::shared_ptr<Appointment>>()>;
	using tasksLoader = std::function<std::vector<std::shared_ptr<Task>>()>;
	using unreadCountsLoader = std::function<std::vector<std::shared_ptr<Folder>>()>;

	explicit Query(appointmentsLoader&& getAppointments, tasksLoader&& getTasks,
		unreadCountsLoader&& getUnreadCounts);

	service::AwaitableObject<std::shared_ptr<object::Node>> getNode(
		service::FieldParams params, response::IdType id);
	std::future<std::shared_ptr<object::AppointmentConnection>> getAppointments(
		const service::FieldParams& params, std::optional<int> first,
		std::optional<response::Value>&& after, std::optional<int> last,
		std::optional<response::Value>&& before);
	std::future<std::shared_ptr<object::TaskConnection>> getTasks(
		const service::FieldParams& params, std::optional<int> first,
		std::optional<response::Value>&& after, std::optional<int> last,
		std::optional<response::Value>&& before);
	std::future<std::shared_ptr<object::FolderConnection>> getUnreadCounts(
		const service::FieldParams& params, std::optional<int> first,
		std::optional<response::Value>&& after, std::optional<int> last,
		std::optional<response::Value>&& before);
	std::vector<std::shared_ptr<object::Appointment>> getAppointmentsById(
		const service::FieldParams& params, const std::vector<response::IdType>& ids);
	std::vector<std::shared_ptr<object::Task>> getTasksById(
		const service::FieldParams& params, const std::vector<response::IdType>& ids);
	std::vector<std::shared_ptr<object::Folder>> getUnreadCountsById(
		const service::FieldParams& params, const std::vector<response::IdType>& ids);
	std::shared_ptr<object::NestedType> getNested(service::FieldParams&& params);
	std::vector<std::shared_ptr<object::Expensive>> getExpensive();
	TaskState getTestTaskState();
	std::vector<std::shared_ptr<object::UnionType>> getAnyType(
		const service::FieldParams& params, const std::vector<response::IdType>& ids);
	std::optional<std::string> getDefault() const noexcept;

private:
	std::shared_ptr<Appointment> findAppointment(
		const service::FieldParams& params, const response::IdType& id);
	std::shared_ptr<Task> findTask(const service::FieldParams& params, const response::IdType& id);
	std::shared_ptr<Folder> findUnreadCount(
		const service::FieldParams& params, const response::IdType& id);

	// Lazy load the fields in each query
	void loadAppointments(const std::shared_ptr<service::RequestState>& state);
	void loadTasks(const std::shared_ptr<service::RequestState>& state);
	void loadUnreadCounts(const std::shared_ptr<service::RequestState>& state);

	appointmentsLoader _getAppointments;
	tasksLoader _getTasks;
	unreadCountsLoader _getUnreadCounts;

	std::vector<std::shared_ptr<Appointment>> _appointments;
	std::vector<std::shared_ptr<Task>> _tasks;
	std::vector<std::shared_ptr<Folder>> _unreadCounts;
};

class PageInfo
{
public:
	explicit PageInfo(bool hasNextPage, bool hasPreviousPage);

	bool getHasNextPage() const noexcept;
	bool getHasPreviousPage() const noexcept;

private:
	const bool _hasNextPage;
	const bool _hasPreviousPage;
};

class Appointment
{
public:
	explicit Appointment(
		response::IdType&& id, std::string&& when, std::string&& subject, bool isNow);

	// EdgeConstraints accessor
	const response::IdType& id() const noexcept;

	service::AwaitableScalar<response::IdType> getId() const noexcept;
	std::shared_ptr<const response::Value> getWhen() const noexcept;
	std::shared_ptr<const response::Value> getSubject() const noexcept;
	bool getIsNow() const noexcept;
	std::optional<std::string> getForceError() const;

private:
	response::IdType _id;
	std::shared_ptr<const response::Value> _when;
	std::shared_ptr<const response::Value> _subject;
	bool _isNow;
};

class AppointmentEdge
{
public:
	explicit AppointmentEdge(std::shared_ptr<Appointment> appointment);

	std::shared_ptr<object::Appointment> getNode() const noexcept;
	service::AwaitableScalar<response::Value> getCursor() const;

private:
	std::shared_ptr<Appointment> _appointment;
};

class AppointmentConnection
{
public:
	explicit AppointmentConnection(bool hasNextPage, bool hasPreviousPage,
		std::vector<std::shared_ptr<Appointment>> appointments);

	std::shared_ptr<object::PageInfo> getPageInfo() const noexcept;
	std::optional<std::vector<std::shared_ptr<object::AppointmentEdge>>> getEdges() const noexcept;
	std::vector<double> getBigArray() const noexcept;

private:
	std::shared_ptr<PageInfo> _pageInfo;
	std::vector<std::shared_ptr<Appointment>> _appointments;
};

class Task
{
public:
	explicit Task(response::IdType&& id, std::string&& title, bool isComplete);

	// EdgeConstraints accessor
	const response::IdType& id() const;

	service::AwaitableScalar<response::IdType> getId() const noexcept;
	std::shared_ptr<const response::Value> getTitle() const noexcept;
	bool getIsComplete() const noexcept;

private:
	response::IdType _id;
	std::shared_ptr<const response::Value> _title;
	bool _isComplete;
};

class TaskEdge
{
public:
	explicit TaskEdge(std::shared_ptr<Task> task);

	std::shared_ptr<object::Task> getNode() const noexcept;
	service::AwaitableScalar<response::Value> getCursor() const noexcept;

private:
	std::shared_ptr<Task> _task;
};

class TaskConnection
{
public:
	explicit TaskConnection(
		bool hasNextPage, bool hasPreviousPage, std::vector<std::shared_ptr<Task>> tasks);

	std::shared_ptr<object::PageInfo> getPageInfo() const noexcept;
	std::optional<std::vector<std::shared_ptr<object::TaskEdge>>> getEdges() const noexcept;

private:
	std::shared_ptr<PageInfo> _pageInfo;
	std::vector<std::shared_ptr<Task>> _tasks;
};

class Folder
{
public:
	explicit Folder(response::IdType&& id, std::string&& name, int unreadCount);

	// EdgeConstraints accessor
	const response::IdType& id() const noexcept;

	service::AwaitableScalar<response::IdType> getId() const noexcept;
	std::shared_ptr<const response::Value> getName() const noexcept;
	int getUnreadCount() const noexcept;

private:
	response::IdType _id;
	std::shared_ptr<const response::Value> _name;
	int _unreadCount;
};

class FolderEdge
{
public:
	explicit FolderEdge(std::shared_ptr<Folder> folder);

	std::shared_ptr<object::Folder> getNode() const noexcept;
	service::AwaitableScalar<response::Value> getCursor() const noexcept;

private:
	std::shared_ptr<Folder> _folder;
};

class FolderConnection
{
public:
	explicit FolderConnection(
		bool hasNextPage, bool hasPreviousPage, std::vector<std::shared_ptr<Folder>> folders);

	std::shared_ptr<object::PageInfo> getPageInfo() const noexcept;
	std::optional<std::vector<std::shared_ptr<object::FolderEdge>>> getEdges() const noexcept;

private:
	std::shared_ptr<PageInfo> _pageInfo;
	std::vector<std::shared_ptr<Folder>> _folders;
};

class CompleteTaskPayload
{
public:
	explicit CompleteTaskPayload(
		std::shared_ptr<Task> task, std::optional<std::string>&& clientMutationId);

	std::shared_ptr<object::Task> getTask() const noexcept;
	const std::optional<std::string>& getClientMutationId() const noexcept;

private:
	std::shared_ptr<Task> _task;
	std::optional<std::string> _clientMutationId;
};

class Mutation
{
public:
	using completeTaskMutation =
		std::function<std::shared_ptr<CompleteTaskPayload>(CompleteTaskInput&&)>;

	explicit Mutation(completeTaskMutation&& mutateCompleteTask);

	static double getFloat() noexcept;

	std::shared_ptr<object::CompleteTaskPayload> applyCompleteTask(
		CompleteTaskInput&& input) noexcept;
	double applySetFloat(double valueArg) noexcept;

private:
	completeTaskMutation _mutateCompleteTask;
	static std::optional<double> _setFloat;
};

class Subscription
{
public:
	explicit Subscription() = default;

	std::shared_ptr<object::Appointment> getNextAppointmentChange() const;
	std::shared_ptr<object::Node> getNodeChange(const response::IdType&) const;
};

class NextAppointmentChange
{
public:
	using nextAppointmentChange =
		std::function<std::shared_ptr<Appointment>(const std::shared_ptr<service::RequestState>&)>;

	explicit NextAppointmentChange(nextAppointmentChange&& changeNextAppointment);

	static std::size_t getCount(service::ResolverContext resolverContext);

	std::shared_ptr<object::Appointment> getNextAppointmentChange(
		const service::FieldParams& params) const;
	std::shared_ptr<object::Node> getNodeChange(const response::IdType&) const;

private:
	nextAppointmentChange _changeNextAppointment;

	static std::size_t _notifySubscribeCount;
	static std::size_t _subscriptionCount;
	static std::size_t _notifyUnsubscribeCount;
};

class NodeChange
{
public:
	using nodeChange =
		std::function<std::shared_ptr<object::Node>(service::ResolverContext resolverContext,
			const std::shared_ptr<service::RequestState>&, response::IdType&&)>;

	explicit NodeChange(nodeChange&& changeNode);

	std::shared_ptr<object::Appointment> getNextAppointmentChange() const;
	std::shared_ptr<object::Node> getNodeChange(
		const service::FieldParams& params, response::IdType&& idArg) const;

private:
	nodeChange _changeNode;
};

struct CapturedParams
{
	// Copied in the constructor
	const service::Directives operationDirectives;
	const service::Directives fragmentDefinitionDirectives;
	const service::Directives fragmentSpreadDirectives;
	const service::Directives inlineFragmentDirectives;

	// Moved in the constructor
	const service::Directives fieldDirectives;
};

class NestedType
{
public:
	explicit NestedType(service::FieldParams&& params, int depth);

	int getDepth() const noexcept;
	std::shared_ptr<object::NestedType> getNested(service::FieldParams&& params) const noexcept;

	static std::stack<CapturedParams> getCapturedParams() noexcept;

private:
	static std::stack<CapturedParams> _capturedParams;

	// Initialized in the constructor
	const int depth;
};

class Expensive
{
public:
	static bool Reset() noexcept;

	explicit Expensive();
	~Expensive();

	std::future<int> getOrder(const service::FieldParams& params) const noexcept;

	static constexpr std::size_t count = 5;
	static std::mutex testMutex;

private:
	// Block async calls to getOrder until pendingExpensive == count
	static std::mutex pendingExpensiveMutex;
	static std::condition_variable pendingExpensiveCondition;
	static std::size_t pendingExpensive;

	// Number of instances
	static std::atomic<std::size_t> instances;

	// Initialized in the constructor
	const std::size_t order;
};

class EmptyOperations : public service::Request
{
public:
	explicit EmptyOperations();
};

} // namespace graphql::today

#endif // TODAYMOCK_H
