// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ifndef APPOINTMENTEDGEOBJECT_H
#define APPOINTMENTEDGEOBJECT_H

#include "TodaySchema.h"

namespace graphql::today::object {

class AppointmentEdge
	: public service::Object
{
protected:
	explicit AppointmentEdge();

public:
	virtual service::FieldResult<std::shared_ptr<Appointment>> getNode(service::FieldParams&& params) const;
	virtual service::FieldResult<response::Value> getCursor(service::FieldParams&& params) const;

private:
	std::future<service::ResolverResult> resolveNode(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveCursor(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
};

} /* namespace graphql::today::object */

#endif // APPOINTMENTEDGEOBJECT_H
