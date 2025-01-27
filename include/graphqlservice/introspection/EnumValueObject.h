// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef ENUMVALUEOBJECT_H
#define ENUMVALUEOBJECT_H

#include "IntrospectionSchema.h"

namespace graphql::introspection::object {

class [[nodiscard]] EnumValue final
	: public service::Object
{
private:
	[[nodiscard]] service::AwaitableResolver resolveName(service::ResolverParams&& params) const;
	[[nodiscard]] service::AwaitableResolver resolveDescription(service::ResolverParams&& params) const;
	[[nodiscard]] service::AwaitableResolver resolveIsDeprecated(service::ResolverParams&& params) const;
	[[nodiscard]] service::AwaitableResolver resolveDeprecationReason(service::ResolverParams&& params) const;

	[[nodiscard]] service::AwaitableResolver resolve_typename(service::ResolverParams&& params) const;

	struct [[nodiscard]] Concept
	{
		virtual ~Concept() = default;

		[[nodiscard]] virtual service::AwaitableScalar<std::string> getName() const = 0;
		[[nodiscard]] virtual service::AwaitableScalar<std::optional<std::string>> getDescription() const = 0;
		[[nodiscard]] virtual service::AwaitableScalar<bool> getIsDeprecated() const = 0;
		[[nodiscard]] virtual service::AwaitableScalar<std::optional<std::string>> getDeprecationReason() const = 0;
	};

	template <class T>
	struct [[nodiscard]] Model
		: Concept
	{
		Model(std::shared_ptr<T>&& pimpl) noexcept
			: _pimpl { std::move(pimpl) }
		{
		}

		[[nodiscard]] service::AwaitableScalar<std::string> getName() const final
		{
			return { _pimpl->getName() };
		}

		[[nodiscard]] service::AwaitableScalar<std::optional<std::string>> getDescription() const final
		{
			return { _pimpl->getDescription() };
		}

		[[nodiscard]] service::AwaitableScalar<bool> getIsDeprecated() const final
		{
			return { _pimpl->getIsDeprecated() };
		}

		[[nodiscard]] service::AwaitableScalar<std::optional<std::string>> getDeprecationReason() const final
		{
			return { _pimpl->getDeprecationReason() };
		}

	private:
		const std::shared_ptr<T> _pimpl;
	};

	const std::unique_ptr<const Concept> _pimpl;

	[[nodiscard]] service::TypeNames getTypeNames() const noexcept;
	[[nodiscard]] service::ResolverMap getResolvers() const noexcept;

public:
	GRAPHQLSERVICE_EXPORT EnumValue(std::shared_ptr<introspection::EnumValue> pimpl) noexcept;
	GRAPHQLSERVICE_EXPORT ~EnumValue();
};

} // namespace graphql::introspection::object

#endif // ENUMVALUEOBJECT_H
