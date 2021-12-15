// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef HUMANOBJECT_H
#define HUMANOBJECT_H

#include "StarWarsSchema.h"

namespace graphql::learn::object {
namespace implements {

template <class I>
concept HumanIs = std::is_same_v<I, Character>;

} // namespace implements

namespace methods::HumanHas {

template <class TImpl>
concept getIdWithParams = requires (TImpl impl, service::FieldParams params) 
{
	{ service::AwaitableScalar<std::string> { impl.getId(std::move(params)) } };
};

template <class TImpl>
concept getId = requires (TImpl impl) 
{
	{ service::AwaitableScalar<std::string> { impl.getId() } };
};

template <class TImpl>
concept getNameWithParams = requires (TImpl impl, service::FieldParams params) 
{
	{ service::AwaitableScalar<std::optional<std::string>> { impl.getName(std::move(params)) } };
};

template <class TImpl>
concept getName = requires (TImpl impl) 
{
	{ service::AwaitableScalar<std::optional<std::string>> { impl.getName() } };
};

template <class TImpl>
concept getFriendsWithParams = requires (TImpl impl, service::FieldParams params) 
{
	{ service::AwaitableObject<std::optional<std::vector<std::shared_ptr<Character>>>> { impl.getFriends(std::move(params)) } };
};

template <class TImpl>
concept getFriends = requires (TImpl impl) 
{
	{ service::AwaitableObject<std::optional<std::vector<std::shared_ptr<Character>>>> { impl.getFriends() } };
};

template <class TImpl>
concept getAppearsInWithParams = requires (TImpl impl, service::FieldParams params) 
{
	{ service::AwaitableScalar<std::optional<std::vector<std::optional<Episode>>>> { impl.getAppearsIn(std::move(params)) } };
};

template <class TImpl>
concept getAppearsIn = requires (TImpl impl) 
{
	{ service::AwaitableScalar<std::optional<std::vector<std::optional<Episode>>>> { impl.getAppearsIn() } };
};

template <class TImpl>
concept getHomePlanetWithParams = requires (TImpl impl, service::FieldParams params) 
{
	{ service::AwaitableScalar<std::optional<std::string>> { impl.getHomePlanet(std::move(params)) } };
};

template <class TImpl>
concept getHomePlanet = requires (TImpl impl) 
{
	{ service::AwaitableScalar<std::optional<std::string>> { impl.getHomePlanet() } };
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

} // namespace methods::HumanHas

class Human
	: public service::Object
{
private:
	service::AwaitableResolver resolveId(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveName(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveFriends(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveAppearsIn(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveHomePlanet(service::ResolverParams&& params) const;

	service::AwaitableResolver resolve_typename(service::ResolverParams&& params) const;

	struct Concept
	{
		virtual ~Concept() = default;

		virtual void beginSelectionSet(const service::SelectionSetParams& params) const = 0;
		virtual void endSelectionSet(const service::SelectionSetParams& params) const = 0;

		virtual service::AwaitableScalar<std::string> getId(service::FieldParams&& params) const = 0;
		virtual service::AwaitableScalar<std::optional<std::string>> getName(service::FieldParams&& params) const = 0;
		virtual service::AwaitableObject<std::optional<std::vector<std::shared_ptr<Character>>>> getFriends(service::FieldParams&& params) const = 0;
		virtual service::AwaitableScalar<std::optional<std::vector<std::optional<Episode>>>> getAppearsIn(service::FieldParams&& params) const = 0;
		virtual service::AwaitableScalar<std::optional<std::string>> getHomePlanet(service::FieldParams&& params) const = 0;
	};

	template <class T>
	struct Model
		: Concept
	{
		Model(std::shared_ptr<T>&& pimpl) noexcept
			: _pimpl { std::move(pimpl) }
		{
		}

		service::AwaitableScalar<std::string> getId(service::FieldParams&& params) const final
		{
			if constexpr (methods::HumanHas::getIdWithParams<T>)
			{
				return { _pimpl->getId(std::move(params)) };
			}
			else
			{
				static_assert(methods::HumanHas::getId<T>, R"msg(Human::getId is not implemented)msg");
				return { _pimpl->getId() };
			}
		}

		service::AwaitableScalar<std::optional<std::string>> getName(service::FieldParams&& params) const final
		{
			if constexpr (methods::HumanHas::getNameWithParams<T>)
			{
				return { _pimpl->getName(std::move(params)) };
			}
			else
			{
				static_assert(methods::HumanHas::getName<T>, R"msg(Human::getName is not implemented)msg");
				return { _pimpl->getName() };
			}
		}

		service::AwaitableObject<std::optional<std::vector<std::shared_ptr<Character>>>> getFriends(service::FieldParams&& params) const final
		{
			if constexpr (methods::HumanHas::getFriendsWithParams<T>)
			{
				return { _pimpl->getFriends(std::move(params)) };
			}
			else
			{
				static_assert(methods::HumanHas::getFriends<T>, R"msg(Human::getFriends is not implemented)msg");
				return { _pimpl->getFriends() };
			}
		}

		service::AwaitableScalar<std::optional<std::vector<std::optional<Episode>>>> getAppearsIn(service::FieldParams&& params) const final
		{
			if constexpr (methods::HumanHas::getAppearsInWithParams<T>)
			{
				return { _pimpl->getAppearsIn(std::move(params)) };
			}
			else
			{
				static_assert(methods::HumanHas::getAppearsIn<T>, R"msg(Human::getAppearsIn is not implemented)msg");
				return { _pimpl->getAppearsIn() };
			}
		}

		service::AwaitableScalar<std::optional<std::string>> getHomePlanet(service::FieldParams&& params) const final
		{
			if constexpr (methods::HumanHas::getHomePlanetWithParams<T>)
			{
				return { _pimpl->getHomePlanet(std::move(params)) };
			}
			else
			{
				static_assert(methods::HumanHas::getHomePlanet<T>, R"msg(Human::getHomePlanet is not implemented)msg");
				return { _pimpl->getHomePlanet() };
			}
		}

		void beginSelectionSet(const service::SelectionSetParams& params) const final
		{
			if constexpr (methods::HumanHas::beginSelectionSet<T>)
			{
				_pimpl->beginSelectionSet(params);
			}
		}

		void endSelectionSet(const service::SelectionSetParams& params) const final
		{
			if constexpr (methods::HumanHas::endSelectionSet<T>)
			{
				_pimpl->endSelectionSet(params);
			}
		}

	private:
		const std::shared_ptr<T> _pimpl;
	};

	Human(std::unique_ptr<Concept>&& pimpl) noexcept;

	// Interfaces which this type implements
	friend Character;

	template <class I>
	static constexpr bool implements() noexcept
	{
		return implements::HumanIs<I>;
	}

	service::TypeNames getTypeNames() const noexcept;
	service::ResolverMap getResolvers() const noexcept;

	void beginSelectionSet(const service::SelectionSetParams& params) const final;
	void endSelectionSet(const service::SelectionSetParams& params) const final;

	const std::unique_ptr<Concept> _pimpl;

public:
	template <class T>
	Human(std::shared_ptr<T> pimpl) noexcept
		: Human { std::unique_ptr<Concept> { std::make_unique<Model<T>>(std::move(pimpl)) } }
	{
	}
};

} // namespace graphql::learn::object

#endif // HUMANOBJECT_H
