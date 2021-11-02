// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef TASKOBJECT_H
#define TASKOBJECT_H

#include "TodaySchema.h"

namespace graphql::today::object {

class Task
	: public service::Object
	, public Node
{
protected:
	explicit Task();

public:
	virtual service::FieldResult<response::IdType> getId(service::FieldParams&& params) const override;
	virtual service::FieldResult<std::optional<response::StringType>> getTitle(service::FieldParams&& params) const;
	virtual service::FieldResult<response::BooleanType> getIsComplete(service::FieldParams&& params) const;

private:
	service::AwaitableResolver resolveId(service::ResolverParams&& params);
	service::AwaitableResolver resolveTitle(service::ResolverParams&& params);
	service::AwaitableResolver resolveIsComplete(service::ResolverParams&& params);

	service::AwaitableResolver resolve_typename(service::ResolverParams&& params);
};

} // namespace graphql::today::object

#endif // TASKOBJECT_H
