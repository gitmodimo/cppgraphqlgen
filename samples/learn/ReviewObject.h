// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef REVIEWOBJECT_H
#define REVIEWOBJECT_H

#include "StarWarsSchema.h"

namespace graphql::learn::object {
namespace methods::ReviewMethod {

template <class TImpl>
concept WithParamsStars = requires (TImpl impl, service::FieldParams params) 
{
	{ service::FieldResult<response::IntType> { impl.getStars(std::move(params)) } };
};

template <class TImpl>
concept NoParamsStars = requires (TImpl impl) 
{
	{ service::FieldResult<response::IntType> { impl.getStars() } };
};

template <class TImpl>
concept WithParamsCommentary = requires (TImpl impl, service::FieldParams params) 
{
	{ service::FieldResult<std::optional<response::StringType>> { impl.getCommentary(std::move(params)) } };
};

template <class TImpl>
concept NoParamsCommentary = requires (TImpl impl) 
{
	{ service::FieldResult<std::optional<response::StringType>> { impl.getCommentary() } };
};

template <class TImpl>
concept HasBeginSelectionSet = requires (TImpl impl, const service::SelectionSetParams params) 
{
	{ impl.beginSelectionSet(params) };
};

template <class TImpl>
concept HasEndSelectionSet = requires (TImpl impl, const service::SelectionSetParams params) 
{
	{ impl.endSelectionSet(params) };
};

} // namespace methods::ReviewMethod

class Review
	: public service::Object
{
private:
	service::AwaitableResolver resolveStars(service::ResolverParams&& params);
	service::AwaitableResolver resolveCommentary(service::ResolverParams&& params);

	service::AwaitableResolver resolve_typename(service::ResolverParams&& params);

	struct Concept
	{
		virtual ~Concept() = default;

		virtual void beginSelectionSet(const service::SelectionSetParams& params) const = 0;
		virtual void endSelectionSet(const service::SelectionSetParams& params) const = 0;

		virtual service::FieldResult<response::IntType> getStars(service::FieldParams&& params) const = 0;
		virtual service::FieldResult<std::optional<response::StringType>> getCommentary(service::FieldParams&& params) const = 0;
	};

	template <class T>
	struct Model
		: Concept
	{
		Model(std::shared_ptr<T>&& pimpl) noexcept
			: _pimpl { std::move(pimpl) }
		{
		}

		service::FieldResult<response::IntType> getStars(service::FieldParams&& params) const final
		{
			if constexpr (methods::ReviewMethod::WithParamsStars<T>)
			{
				return { _pimpl->getStars(std::move(params)) };
			}
			else
			{
				static_assert(methods::ReviewMethod::NoParamsStars<T>, R"msg(Review::getStars is not implemented)msg");
				return { _pimpl->getStars() };
			}
		}

		service::FieldResult<std::optional<response::StringType>> getCommentary(service::FieldParams&& params) const final
		{
			if constexpr (methods::ReviewMethod::WithParamsCommentary<T>)
			{
				return { _pimpl->getCommentary(std::move(params)) };
			}
			else
			{
				static_assert(methods::ReviewMethod::NoParamsCommentary<T>, R"msg(Review::getCommentary is not implemented)msg");
				return { _pimpl->getCommentary() };
			}
		}

		void beginSelectionSet(const service::SelectionSetParams& params) const final
		{
			if constexpr (methods::ReviewMethod::HasBeginSelectionSet<T>)
			{
				_pimpl->beginSelectionSet(params);
			}
		}

		void endSelectionSet(const service::SelectionSetParams& params) const final
		{
			if constexpr (methods::ReviewMethod::HasEndSelectionSet<T>)
			{
				_pimpl->endSelectionSet(params);
			}
		}

	private:
		const std::shared_ptr<T> _pimpl;
	};

	Review(std::unique_ptr<Concept>&& pimpl);

	void beginSelectionSet(const service::SelectionSetParams& params) const final;
	void endSelectionSet(const service::SelectionSetParams& params) const final;

	const std::unique_ptr<Concept> _pimpl;

public:
	template <class T>
	Review(std::shared_ptr<T> pimpl)
		: Review { std::unique_ptr<Concept> { std::make_unique<Model<T>>(std::move(pimpl)) } }
	{
	}
};

} // namespace graphql::learn::object

#endif // REVIEWOBJECT_H
