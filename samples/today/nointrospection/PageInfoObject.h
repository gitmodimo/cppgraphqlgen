// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef PAGEINFOOBJECT_H
#define PAGEINFOOBJECT_H

#include "TodaySchema.h"

namespace graphql::today::object {
namespace methods::PageInfoHas {

template <class TImpl>
concept getHasNextPageWithParams = requires (TImpl impl, service::FieldParams params)
{
	{ service::AwaitableScalar<bool> { impl.getHasNextPage(std::move(params)) } };
};

template <class TImpl>
concept getHasNextPage = requires (TImpl impl)
{
	{ service::AwaitableScalar<bool> { impl.getHasNextPage() } };
};

template <class TImpl>
concept getHasPreviousPageWithParams = requires (TImpl impl, service::FieldParams params)
{
	{ service::AwaitableScalar<bool> { impl.getHasPreviousPage(std::move(params)) } };
};

template <class TImpl>
concept getHasPreviousPage = requires (TImpl impl)
{
	{ service::AwaitableScalar<bool> { impl.getHasPreviousPage() } };
};

template <class TImpl>
concept beginSelectionSet = requires (TImpl impl, const service::SelectionSetParams params)
{
	{ impl.beginSelectionSet(params) };
};

template <class TImpl>
concept endSelectionSet = requires (TImpl impl, const service::SelectionSetParams params)
{
	{ impl.endSelectionSet(params) };
};

} // namespace methods::PageInfoHas

class [[nodiscard]] PageInfo final
	: public service::Object
{
private:
	[[nodiscard]] service::AwaitableResolver resolveHasNextPage(service::ResolverParams&& params) const;
	[[nodiscard]] service::AwaitableResolver resolveHasPreviousPage(service::ResolverParams&& params) const;

	[[nodiscard]] service::AwaitableResolver resolve_typename(service::ResolverParams&& params) const;

	struct [[nodiscard]] Concept
	{
		virtual ~Concept() = default;

		virtual void beginSelectionSet(const service::SelectionSetParams& params) const = 0;
		virtual void endSelectionSet(const service::SelectionSetParams& params) const = 0;

		[[nodiscard]] virtual service::AwaitableScalar<bool> getHasNextPage(service::FieldParams&& params) const = 0;
		[[nodiscard]] virtual service::AwaitableScalar<bool> getHasPreviousPage(service::FieldParams&& params) const = 0;
	};

	template <class T>
	struct [[nodiscard]] Model final
		: Concept
	{
		explicit Model(std::shared_ptr<T> pimpl) noexcept
			: _pimpl { std::move(pimpl) }
		{
		}

		[[nodiscard]] service::AwaitableScalar<bool> getHasNextPage(service::FieldParams&& params) const override
		{
			if constexpr (methods::PageInfoHas::getHasNextPageWithParams<T>)
			{
				return { _pimpl->getHasNextPage(std::move(params)) };
			}
			else if constexpr (methods::PageInfoHas::getHasNextPage<T>)
			{
				return { _pimpl->getHasNextPage() };
			}
			else
			{
				throw service::unimplemented_method(R"ex(PageInfo::getHasNextPage)ex");
			}
		}

		[[nodiscard]] service::AwaitableScalar<bool> getHasPreviousPage(service::FieldParams&& params) const override
		{
			if constexpr (methods::PageInfoHas::getHasPreviousPageWithParams<T>)
			{
				return { _pimpl->getHasPreviousPage(std::move(params)) };
			}
			else if constexpr (methods::PageInfoHas::getHasPreviousPage<T>)
			{
				return { _pimpl->getHasPreviousPage() };
			}
			else
			{
				throw service::unimplemented_method(R"ex(PageInfo::getHasPreviousPage)ex");
			}
		}

		void beginSelectionSet(const service::SelectionSetParams& params) const override
		{
			if constexpr (methods::PageInfoHas::beginSelectionSet<T>)
			{
				_pimpl->beginSelectionSet(params);
			}
		}

		void endSelectionSet(const service::SelectionSetParams& params) const override
		{
			if constexpr (methods::PageInfoHas::endSelectionSet<T>)
			{
				_pimpl->endSelectionSet(params);
			}
		}

	private:
		const std::shared_ptr<T> _pimpl;
	};

	explicit PageInfo(std::unique_ptr<const Concept> pimpl) noexcept;

	[[nodiscard]] service::TypeNames getTypeNames() const noexcept;
	[[nodiscard]] service::ResolverMap getResolvers() const noexcept;

	void beginSelectionSet(const service::SelectionSetParams& params) const override;
	void endSelectionSet(const service::SelectionSetParams& params) const override;

	const std::unique_ptr<const Concept> _pimpl;

public:
	template <class T>
	explicit PageInfo(std::shared_ptr<T> pimpl) noexcept
		: PageInfo { std::unique_ptr<const Concept> { std::make_unique<Model<T>>(std::move(pimpl)) } }
	{
	}

	[[nodiscard]] static constexpr std::string_view getObjectType() noexcept
	{
		return { R"gql(PageInfo)gql" };
	}
};

} // namespace graphql::today::object

#endif // PAGEINFOOBJECT_H
