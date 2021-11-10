// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "SubscribeClient.h"

#include <algorithm>
#include <array>
#include <stdexcept>
#include <sstream>
#include <string_view>

using namespace std::literals;

namespace graphql::client {

using namespace subscription::TestSubscription;

template <>
Response::nextAppointment_Appointment ModifiedResponse<Response::nextAppointment_Appointment>::parse(response::Value&& response)
{
	Response::nextAppointment_Appointment result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(nextAppointmentId)js"sv)
			{
				result.nextAppointmentId = ModifiedResponse<response::IdType>::parse(std::move(member.second));
				continue;
			}
			if (member.first == R"js(when)js"sv)
			{
				result.when = ModifiedResponse<response::Value>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
			if (member.first == R"js(subject)js"sv)
			{
				result.subject = ModifiedResponse<response::StringType>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
			if (member.first == R"js(isNow)js"sv)
			{
				result.isNow = ModifiedResponse<response::BooleanType>::parse(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

namespace subscription::TestSubscription {

const std::string& GetRequestText() noexcept
{
	static const auto s_request = R"gql(
		# Copyright (c) Microsoft Corporation. All rights reserved.
		# Licensed under the MIT License.
		
		subscription TestSubscription {
		  nextAppointment: nextAppointmentChange {
		    nextAppointmentId: id
		    when
		    subject
		    isNow
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

Response parseResponse(response::Value&& response)
{
	Response result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(nextAppointment)js"sv)
			{
				result.nextAppointment = ModifiedResponse<Response::nextAppointment_Appointment>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

} // namespace subscription::TestSubscription
} // namespace graphql::client