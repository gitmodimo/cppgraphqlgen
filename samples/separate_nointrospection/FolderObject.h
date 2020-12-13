// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ifndef FOLDEROBJECT_H
#define FOLDEROBJECT_H

#include "TodaySchema.h"

namespace graphql::today::object {

class Folder
	: public service::Object
	, public Node
{
protected:
	explicit Folder();

public:
	virtual service::FieldResult<response::IdType> getId(service::FieldParams&& params) const override;
	virtual service::FieldResult<std::optional<response::StringType>> getName(service::FieldParams&& params) const;
	virtual service::FieldResult<response::IntType> getUnreadCount(service::FieldParams&& params) const;

private:
	std::future<service::ResolverResult> resolveId(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveName(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveUnreadCount(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
};

} /* namespace graphql::today::object */

#endif // FOLDEROBJECT_H
