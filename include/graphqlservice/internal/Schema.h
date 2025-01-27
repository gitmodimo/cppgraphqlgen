// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ifndef GRAPHQLSCHEMA_H
#define GRAPHQLSCHEMA_H

#include "graphqlservice/GraphQLService.h"

#include <shared_mutex>

namespace graphql {
namespace introspection {

enum class [[nodiscard]] TypeKind;
enum class [[nodiscard]] DirectiveLocation;

} // namespace introspection

namespace schema {

class Schema;
class Directive;
class BaseType;
class ScalarType;
class ObjectType;
class InterfaceType;
class UnionType;
class EnumType;
class InputObjectType;
class WrapperType;
class Field;
class InputValue;
class EnumValue;

class [[nodiscard]] Schema : public std::enable_shared_from_this<Schema>
{
public:
	GRAPHQLSERVICE_EXPORT explicit Schema(
		bool noIntrospection = false, std::string_view description = "");

	GRAPHQLSERVICE_EXPORT void AddQueryType(std::shared_ptr<ObjectType> query);
	GRAPHQLSERVICE_EXPORT void AddMutationType(std::shared_ptr<ObjectType> mutation);
	GRAPHQLSERVICE_EXPORT void AddSubscriptionType(std::shared_ptr<ObjectType> subscription);
	GRAPHQLSERVICE_EXPORT void AddType(std::string_view name, std::shared_ptr<BaseType> type);
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::shared_ptr<const BaseType>& LookupType(
		std::string_view name) const;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::shared_ptr<const BaseType> WrapType(
		introspection::TypeKind kind, std::shared_ptr<const BaseType> ofType);
	GRAPHQLSERVICE_EXPORT void AddDirective(std::shared_ptr<Directive> directive);

	// Accessors
	GRAPHQLSERVICE_EXPORT [[nodiscard]] bool supportsIntrospection() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view description() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::vector<
		std::pair<std::string_view, std::shared_ptr<const BaseType>>>&
	types() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::shared_ptr<const ObjectType>& queryType()
		const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::shared_ptr<const ObjectType>& mutationType()
		const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::shared_ptr<const ObjectType>& subscriptionType()
		const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::vector<std::shared_ptr<const Directive>>&
	directives() const noexcept;

private:
	const bool _noIntrospection = false;
	const std::string_view _description;

	std::shared_ptr<const ObjectType> _query;
	std::shared_ptr<const ObjectType> _mutation;
	std::shared_ptr<const ObjectType> _subscription;
	internal::string_view_map<size_t> _typeMap;
	std::vector<std::pair<std::string_view, std::shared_ptr<const BaseType>>> _types;
	std::vector<std::shared_ptr<const Directive>> _directives;
	std::shared_mutex _nonNullWrappersMutex;
	internal::sorted_map<std::shared_ptr<const BaseType>, std::shared_ptr<const BaseType>>
		_nonNullWrappers;
	std::shared_mutex _listWrappersMutex;
	internal::sorted_map<std::shared_ptr<const BaseType>, std::shared_ptr<const BaseType>>
		_listWrappers;
};

class [[nodiscard]] BaseType : public std::enable_shared_from_this<BaseType>
{
public:
	GRAPHQLSERVICE_EXPORT virtual ~BaseType() = default;

	// Accessors
	GRAPHQLSERVICE_EXPORT [[nodiscard]] introspection::TypeKind kind() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] virtual std::string_view name() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view description() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] virtual const std::vector<std::shared_ptr<const Field>>&
	fields() const noexcept;
	GRAPHQLSERVICE_EXPORT
	[[nodiscard]] virtual const std::vector<std::shared_ptr<const InterfaceType>>& interfaces()
		const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] virtual const std::vector<std::weak_ptr<const BaseType>>&
	possibleTypes() const noexcept;
	GRAPHQLSERVICE_EXPORT
	[[nodiscard]] virtual const std::vector<std::shared_ptr<const EnumValue>>& enumValues()
		const noexcept;
	GRAPHQLSERVICE_EXPORT
	[[nodiscard]] virtual const std::vector<std::shared_ptr<const InputValue>>& inputFields()
		const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] virtual const std::weak_ptr<const BaseType>& ofType()
		const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] virtual std::string_view specifiedByURL() const noexcept;

protected:
	BaseType(introspection::TypeKind kind, std::string_view description);

private:
	const introspection::TypeKind _kind;
	const std::string_view _description;
};

class [[nodiscard]] ScalarType : public BaseType
{
private:
	// Use a private constructor parameter type to enable std::make_shared inside of the static Make
	// method without exposing the constructor as part of the public interface.
	struct init;

public:
	explicit ScalarType(init&& params);

	GRAPHQLSERVICE_EXPORT [[nodiscard]] static std::shared_ptr<ScalarType> Make(
		std::string_view name, std::string_view description, std::string_view specifiedByURL);

	// Accessors
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view name() const noexcept final;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view specifiedByURL() const noexcept final;

private:
	const std::string_view _name;
	const std::string_view _specifiedByURL;
};

class [[nodiscard]] ObjectType : public BaseType
{
private:
	// Use a private constructor parameter type to enable std::make_shared inside of the static Make
	// method without exposing the constructor as part of the public interface.
	struct init;

public:
	explicit ObjectType(init&& params);

	GRAPHQLSERVICE_EXPORT [[nodiscard]] static std::shared_ptr<ObjectType> Make(
		std::string_view name, std::string_view description);

	GRAPHQLSERVICE_EXPORT void AddInterfaces(
		std::vector<std::shared_ptr<const InterfaceType>>&& interfaces);
	GRAPHQLSERVICE_EXPORT void AddFields(std::vector<std::shared_ptr<const Field>>&& fields);

	// Accessors
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view name() const noexcept final;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::vector<std::shared_ptr<const Field>>& fields()
		const noexcept final;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::vector<std::shared_ptr<const InterfaceType>>&
	interfaces() const noexcept final;

private:
	const std::string_view _name;

	std::vector<std::shared_ptr<const InterfaceType>> _interfaces;
	std::vector<std::shared_ptr<const Field>> _fields;
};

class [[nodiscard]] InterfaceType : public BaseType
{
private:
	// Use a private constructor parameter type to enable std::make_shared inside of the static Make
	// method without exposing the constructor as part of the public interface.
	struct init;

public:
	explicit InterfaceType(init&& params);

	GRAPHQLSERVICE_EXPORT [[nodiscard]] static std::shared_ptr<InterfaceType> Make(
		std::string_view name, std::string_view description);

	GRAPHQLSERVICE_EXPORT void AddPossibleType(std::weak_ptr<BaseType> possibleType);
	GRAPHQLSERVICE_EXPORT void AddInterfaces(
		std::vector<std::shared_ptr<const InterfaceType>>&& interfaces);
	GRAPHQLSERVICE_EXPORT void AddFields(std::vector<std::shared_ptr<const Field>>&& fields);

	// Accessors
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view name() const noexcept final;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::vector<std::shared_ptr<const Field>>& fields()
		const noexcept final;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::vector<std::weak_ptr<const BaseType>>&
	possibleTypes() const noexcept final;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::vector<std::shared_ptr<const InterfaceType>>&
	interfaces() const noexcept final;

private:
	const std::string_view _name;

	std::vector<std::shared_ptr<const InterfaceType>> _interfaces;
	std::vector<std::shared_ptr<const Field>> _fields;
	std::vector<std::weak_ptr<const BaseType>> _possibleTypes;
};

class [[nodiscard]] UnionType : public BaseType
{
private:
	// Use a private constructor parameter type to enable std::make_shared inside of the static Make
	// method without exposing the constructor as part of the public interface.
	struct init;

public:
	explicit UnionType(init&& params);

	GRAPHQLSERVICE_EXPORT [[nodiscard]] static std::shared_ptr<UnionType> Make(
		std::string_view name, std::string_view description);

	GRAPHQLSERVICE_EXPORT void AddPossibleTypes(
		std::vector<std::weak_ptr<const BaseType>>&& possibleTypes);

	// Accessors
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view name() const noexcept final;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::vector<std::weak_ptr<const BaseType>>&
	possibleTypes() const noexcept final;

private:
	const std::string_view _name;

	std::vector<std::weak_ptr<const BaseType>> _possibleTypes;
};

struct [[nodiscard]] EnumValueType
{
	std::string_view value;
	std::string_view description;
	std::optional<std::string_view> deprecationReason;
};

class [[nodiscard]] EnumType : public BaseType
{
private:
	// Use a private constructor parameter type to enable std::make_shared inside of the static Make
	// method without exposing the constructor as part of the public interface.
	struct init;

public:
	explicit EnumType(init&& params);

	GRAPHQLSERVICE_EXPORT [[nodiscard]] static std::shared_ptr<EnumType> Make(
		std::string_view name, std::string_view description);

	GRAPHQLSERVICE_EXPORT void AddEnumValues(std::vector<EnumValueType>&& enumValues);

	// Accessors
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view name() const noexcept final;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::vector<std::shared_ptr<const EnumValue>>&
	enumValues() const noexcept final;

private:
	const std::string_view _name;

	std::vector<std::shared_ptr<const EnumValue>> _enumValues;
};

class [[nodiscard]] InputObjectType : public BaseType
{
private:
	// Use a private constructor parameter type to enable std::make_shared inside of the static Make
	// method without exposing the constructor as part of the public interface.
	struct init;

public:
	explicit InputObjectType(init&& params);

	GRAPHQLSERVICE_EXPORT [[nodiscard]] static std::shared_ptr<InputObjectType> Make(
		std::string_view name, std::string_view description);

	GRAPHQLSERVICE_EXPORT void AddInputValues(
		std::vector<std::shared_ptr<const InputValue>>&& inputValues);

	// Accessors
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view name() const noexcept final;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::vector<std::shared_ptr<const InputValue>>&
	inputFields() const noexcept final;

private:
	const std::string_view _name;

	std::vector<std::shared_ptr<const InputValue>> _inputValues;
};

class [[nodiscard]] WrapperType : public BaseType
{
private:
	// Use a private constructor parameter type to enable std::make_shared inside of the static Make
	// method without exposing the constructor as part of the public interface.
	struct init;

public:
	explicit WrapperType(init&& params);

	GRAPHQLSERVICE_EXPORT [[nodiscard]] static std::shared_ptr<WrapperType> Make(
		introspection::TypeKind kind, std::weak_ptr<const BaseType> ofType);

	// Accessors
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::weak_ptr<const BaseType>& ofType()
		const noexcept final;

private:
	const std::weak_ptr<const BaseType> _ofType;
};

class [[nodiscard]] Field : public std::enable_shared_from_this<Field>
{
private:
	// Use a private constructor parameter type to enable std::make_shared inside of the static Make
	// method without exposing the constructor as part of the public interface.
	struct init;

public:
	explicit Field(init&& params);

	GRAPHQLSERVICE_EXPORT [[nodiscard]] static std::shared_ptr<Field> Make(std::string_view name,
		std::string_view description, std::optional<std::string_view> deprecationReason,
		std::weak_ptr<const BaseType> type,
		std::vector<std::shared_ptr<const InputValue>>&& args = {});

	// Accessors
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view name() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view description() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::vector<std::shared_ptr<const InputValue>>& args()
		const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::weak_ptr<const BaseType>& type() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::optional<std::string_view>& deprecationReason()
		const noexcept;

private:
	const std::string_view _name;
	const std::string_view _description;
	const std::optional<std::string_view> _deprecationReason;
	const std::weak_ptr<const BaseType> _type;
	const std::vector<std::shared_ptr<const InputValue>> _args;
};

class [[nodiscard]] InputValue : public std::enable_shared_from_this<InputValue>
{
private:
	// Use a private constructor parameter type to enable std::make_shared inside of the static Make
	// method without exposing the constructor as part of the public interface.
	struct init;

public:
	explicit InputValue(init&& params);

	GRAPHQLSERVICE_EXPORT [[nodiscard]] static std::shared_ptr<InputValue> Make(
		std::string_view name, std::string_view description, std::weak_ptr<const BaseType> type,
		std::string_view defaultValue);

	// Accessors
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view name() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view description() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::weak_ptr<const BaseType>& type() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view defaultValue() const noexcept;

private:
	const std::string_view _name;
	const std::string_view _description;
	const std::weak_ptr<const BaseType> _type;
	const std::string_view _defaultValue;
};

class [[nodiscard]] EnumValue : public std::enable_shared_from_this<EnumValue>
{
private:
	// Use a private constructor parameter type to enable std::make_shared inside of the static Make
	// method without exposing the constructor as part of the public interface.
	struct init;

public:
	explicit EnumValue(init&& params);

	GRAPHQLSERVICE_EXPORT [[nodiscard]] static std::shared_ptr<EnumValue> Make(
		std::string_view name, std::string_view description,
		std::optional<std::string_view> deprecationReason);

	// Accessors
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view name() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view description() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::optional<std::string_view>& deprecationReason()
		const noexcept;

private:
	const std::string_view _name;
	const std::string_view _description;
	const std::optional<std::string_view> _deprecationReason;
};

class [[nodiscard]] Directive : public std::enable_shared_from_this<Directive>
{
private:
	// Use a private constructor parameter type to enable std::make_shared inside of the static Make
	// method without exposing the constructor as part of the public interface.
	struct init;

public:
	explicit Directive(init&& params);

	GRAPHQLSERVICE_EXPORT [[nodiscard]] static std::shared_ptr<Directive> Make(
		std::string_view name, std::string_view description,
		std::vector<introspection::DirectiveLocation>&& locations,
		std::vector<std::shared_ptr<const InputValue>>&& args, bool isRepeatable);

	// Accessors
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view name() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] std::string_view description() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::vector<introspection::DirectiveLocation>&
	locations() const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] const std::vector<std::shared_ptr<const InputValue>>& args()
		const noexcept;
	GRAPHQLSERVICE_EXPORT [[nodiscard]] bool isRepeatable() const noexcept;

private:
	const std::string_view _name;
	const std::string_view _description;
	const std::vector<introspection::DirectiveLocation> _locations;
	const std::vector<std::shared_ptr<const InputValue>> _args;
	const bool _isRepeatable;
};

} // namespace schema
} // namespace graphql

#endif // GRAPHQLSCHEMA_H
