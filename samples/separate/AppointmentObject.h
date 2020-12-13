// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ifndef APPOINTMENTOBJECT_H
#define APPOINTMENTOBJECT_H

#include "TodaySchema.h"

namespace graphql::today::object {

class Appointment
	: public service::Object
	, public Node
{
protected:
	explicit Appointment();

public:
	virtual service::FieldResult<response::IdType> getId(service::FieldParams&& params) const override;
	virtual service::FieldResult<std::optional<response::Value>> getWhen(service::FieldParams&& params) const;
	virtual service::FieldResult<std::optional<response::StringType>> getSubject(service::FieldParams&& params) const;
	virtual service::FieldResult<response::BooleanType> getIsNow(service::FieldParams&& params) const;
	virtual service::FieldResult<std::optional<response::StringType>> getForceError(service::FieldParams&& params) const;

private:
	std::future<service::ResolverResult> resolveId(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveWhen(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveSubject(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveIsNow(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveForceError(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
};

} /* namespace graphql::today::object */

#endif // APPOINTMENTOBJECT_H
