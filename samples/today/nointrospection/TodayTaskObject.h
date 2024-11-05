// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef TODAY_TODAYTASKOBJECT_H
#define TODAY_TODAYTASKOBJECT_H

#include "TodaySchema.h"

namespace graphql::today::object {
namespace implements {

template <class I>
concept TaskIs = std::is_same_v<I, Node> || std::is_same_v<I, UnionType>;

} // namespace implements

namespace methods::TaskHas {

template <class TImpl>
concept getIdWithParams = requires (TImpl impl, service::FieldParams params)
{
	{ service::AwaitableScalar<response::IdType> { impl.getId(std::move(params)) } };
};

template <class TImpl>
concept getId = requires (TImpl impl)
{
	{ service::AwaitableScalar<response::IdType> { impl.getId() } };
};

template <class TImpl>
concept getTitleWithParams = requires (TImpl impl, service::FieldParams params)
{
	{ service::AwaitableScalar<std::optional<std::string>> { impl.getTitle(std::move(params)) } };
};

template <class TImpl>
concept getTitle = requires (TImpl impl)
{
	{ service::AwaitableScalar<std::optional<std::string>> { impl.getTitle() } };
};

template <class TImpl>
concept getIsCompleteWithParams = requires (TImpl impl, service::FieldParams params)
{
	{ service::AwaitableScalar<bool> { impl.getIsComplete(std::move(params)) } };
};

template <class TImpl>
concept getIsComplete = requires (TImpl impl)
{
	{ service::AwaitableScalar<bool> { impl.getIsComplete() } };
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

} // namespace methods::TaskHas

class [[nodiscard("unnecessary construction")]] Task final
	: public service::Object
{
private:
	[[nodiscard("unnecessary call")]] service::AwaitableResolver resolveId(service::ResolverParams&& params) const;
	[[nodiscard("unnecessary call")]] service::AwaitableResolver resolveTitle(service::ResolverParams&& params) const;
	[[nodiscard("unnecessary call")]] service::AwaitableResolver resolveIsComplete(service::ResolverParams&& params) const;

	[[nodiscard("unnecessary call")]] service::AwaitableResolver resolve_typename(service::ResolverParams&& params) const;

	struct [[nodiscard("unnecessary construction")]] Concept
	{
		virtual ~Concept() = default;

		virtual void beginSelectionSet(const service::SelectionSetParams& params) const = 0;
		virtual void endSelectionSet(const service::SelectionSetParams& params) const = 0;

		[[nodiscard("unnecessary call")]] virtual service::AwaitableScalar<response::IdType> getId(service::FieldParams&& params) const = 0;
		[[nodiscard("unnecessary call")]] virtual service::AwaitableScalar<std::optional<std::string>> getTitle(service::FieldParams&& params) const = 0;
		[[nodiscard("unnecessary call")]] virtual service::AwaitableScalar<bool> getIsComplete(service::FieldParams&& params) const = 0;
	};

	template <class T>
	struct [[nodiscard("unnecessary construction")]] Model final
		: Concept
	{
		explicit Model(std::shared_ptr<T> pimpl) noexcept
			: _pimpl { std::move(pimpl) }
		{
		}

		[[nodiscard("unnecessary call")]] service::AwaitableScalar<response::IdType> getId(service::FieldParams&& params) const override
		{
			if constexpr (methods::TaskHas::getIdWithParams<T>)
			{
				return { _pimpl->getId(std::move(params)) };
			}
			else if constexpr (methods::TaskHas::getId<T>)
			{
				return { _pimpl->getId() };
			}
			else
			{
				throw service::unimplemented_method(R"ex(Task::getId)ex");
			}
		}

		[[nodiscard("unnecessary call")]] service::AwaitableScalar<std::optional<std::string>> getTitle(service::FieldParams&& params) const override
		{
			if constexpr (methods::TaskHas::getTitleWithParams<T>)
			{
				return { _pimpl->getTitle(std::move(params)) };
			}
			else if constexpr (methods::TaskHas::getTitle<T>)
			{
				return { _pimpl->getTitle() };
			}
			else
			{
				throw service::unimplemented_method(R"ex(Task::getTitle)ex");
			}
		}

		[[nodiscard("unnecessary call")]] service::AwaitableScalar<bool> getIsComplete(service::FieldParams&& params) const override
		{
			if constexpr (methods::TaskHas::getIsCompleteWithParams<T>)
			{
				return { _pimpl->getIsComplete(std::move(params)) };
			}
			else if constexpr (methods::TaskHas::getIsComplete<T>)
			{
				return { _pimpl->getIsComplete() };
			}
			else
			{
				throw service::unimplemented_method(R"ex(Task::getIsComplete)ex");
			}
		}

		void beginSelectionSet(const service::SelectionSetParams& params) const override
		{
			if constexpr (methods::TaskHas::beginSelectionSet<T>)
			{
				_pimpl->beginSelectionSet(params);
			}
		}

		void endSelectionSet(const service::SelectionSetParams& params) const override
		{
			if constexpr (methods::TaskHas::endSelectionSet<T>)
			{
				_pimpl->endSelectionSet(params);
			}
		}

	private:
		const std::shared_ptr<T> _pimpl;
	};

	explicit Task(std::unique_ptr<const Concept> pimpl) noexcept;

	// Interfaces which this type implements
	friend Node;

	// Unions which include this type
	friend UnionType;

	template <class I>
	[[nodiscard("unnecessary call")]] static constexpr bool implements() noexcept
	{
		return implements::TaskIs<I>;
	}

	[[nodiscard("unnecessary call")]] service::TypeNames getTypeNames() const noexcept;
	[[nodiscard("unnecessary call")]] service::ResolverMap getResolvers() const noexcept;

	void beginSelectionSet(const service::SelectionSetParams& params) const override;
	void endSelectionSet(const service::SelectionSetParams& params) const override;

	const std::unique_ptr<const Concept> _pimpl;

public:
	template <class T>
	explicit Task(std::shared_ptr<T> pimpl) noexcept
		: Task { std::unique_ptr<const Concept> { std::make_unique<Model<T>>(std::move(pimpl)) } }
	{
	}

	[[nodiscard("unnecessary call")]] static constexpr std::string_view getObjectType() noexcept
	{
		return { R"gql(Task)gql" };
	}
};

} // namespace graphql::today::object

#endif // TODAY_TODAYTASKOBJECT_H