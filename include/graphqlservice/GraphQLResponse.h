// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ifndef GRAPHQLRESPONSE_H
#define GRAPHQLRESPONSE_H

// clang-format off
#ifdef GRAPHQL_DLLEXPORTS
	#ifdef IMPL_GRAPHQLRESPONSE_DLL
		#define GRAPHQLRESPONSE_EXPORT __declspec(dllexport)
	#else // !IMPL_GRAPHQLRESPONSE_DLL
		#define GRAPHQLRESPONSE_EXPORT __declspec(dllimport)
	#endif // !IMPL_GRAPHQLRESPONSE_DLL
#else // !GRAPHQL_DLLEXPORTS
	#define GRAPHQLRESPONSE_EXPORT
#endif // !GRAPHQL_DLLEXPORTS
// clang-format on

#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace graphql::response {

// GraphQL responses are not technically JSON-specific, although that is probably the most common
// way of representing them. These are the primitive types that may be represented in GraphQL, as
// of the [June 2018 spec](http://spec.graphql.org/June2018/#sec-Serialization-Format).
enum class Type : uint8_t
{
	Map,	   // JSON Object
	List,	   // JSON Array
	String,	   // JSON String
	Null,	   // JSON null
	Boolean,   // JSON true or false
	Int,	   // JSON Number
	Float,	   // JSON Number
	EnumValue, // JSON String
	Scalar,	   // JSON any type
	Custom, // User Defined type
};

struct Value;

using MapType = std::vector<std::pair<std::string, Value>>;
using ListType = std::vector<Value>;
using StringType = std::string;
using BooleanType = bool;
using IntType = int;
using FloatType = double;
using ScalarType = Value;
using IdType = std::vector<uint8_t>;

struct CustomTypeBase
{
	//virtual bool operator==(const CustomTypeBase& rhs) const=0;
	//virtual CustomTypeBase&& clone()const=0;
	virtual ~CustomTypeBase(){}
};

using CustomType = std::unique_ptr<CustomTypeBase>;

template <typename ValueType>
struct ValueTypeTraits
{
	// Set by r-value reference, get by const reference, and release by value. The only types
	// that actually support all 3 methods are StringType and ScalarType, everything else
	// overrides some subset of these types with a template specialization.
	using set_type = ValueType&&;
	using get_type = const ValueType&;
	using release_type = ValueType;
};

template <>
struct ValueTypeTraits<MapType>
{
	// Get by const reference and release by value.
	using get_type = const MapType&;
	using release_type = MapType;
};

template <>
struct ValueTypeTraits<ListType>
{
	// Get by const reference and release by value.
	using get_type = const ListType&;
	using release_type = ListType;
};

template <>
struct ValueTypeTraits<BooleanType>
{
	// Set and get by value.
	using set_type = BooleanType;
	using get_type = BooleanType;
};

template <>
struct ValueTypeTraits<IntType>
{
	// Set and get by value.
	using set_type = IntType;
	using get_type = IntType;
};

template <>
struct ValueTypeTraits<FloatType>
{
	// Set and get by value.
	using set_type = FloatType;
	using get_type = FloatType;
};

template <>
struct ValueTypeTraits<IdType>
{
	// ID values are represented as a Base64 String, so they require conversion.
	using set_type = const IdType&;
	using get_type = IdType;
	using release_type = IdType;
};

template <>
struct ValueTypeTraits<CustomType>
{
	using release_type = CustomType;
};

// Represent a discriminated union of GraphQL response value types.
struct Value
{
	GRAPHQLRESPONSE_EXPORT Value(Type type = Type::Null);
	GRAPHQLRESPONSE_EXPORT ~Value();

	GRAPHQLRESPONSE_EXPORT explicit Value(const char* value);
	GRAPHQLRESPONSE_EXPORT explicit Value(StringType&& value);
	GRAPHQLRESPONSE_EXPORT explicit Value(BooleanType value);
	GRAPHQLRESPONSE_EXPORT explicit Value(IntType value);
	GRAPHQLRESPONSE_EXPORT explicit Value(FloatType value);
	GRAPHQLRESPONSE_EXPORT explicit Value(const IdType& value);
	GRAPHQLRESPONSE_EXPORT explicit Value(std::unique_ptr<CustomTypeBase>&& value);

	GRAPHQLRESPONSE_EXPORT Value(Value&& other) noexcept;
	GRAPHQLRESPONSE_EXPORT explicit Value(const Value& other);

	GRAPHQLRESPONSE_EXPORT Value& operator=(Value&& rhs) noexcept;
	Value& operator=(const Value& rhs) = delete;

	// Comparison
	GRAPHQLRESPONSE_EXPORT bool operator==(const Value& rhs) const noexcept;
	GRAPHQLRESPONSE_EXPORT bool operator!=(const Value& rhs) const noexcept;

	// Check the Type
	GRAPHQLRESPONSE_EXPORT Type type() const noexcept;

	// JSON doesn't distinguish between Type::String and Type::EnumValue, so if this value comes
	// from JSON and it's a string we need to track the fact that it can be interpreted as either.
	GRAPHQLRESPONSE_EXPORT Value&& from_json() noexcept;
	GRAPHQLRESPONSE_EXPORT bool maybe_enum() const noexcept;

	// Valid for Type::Map or Type::List
	GRAPHQLRESPONSE_EXPORT void reserve(size_t count);
	GRAPHQLRESPONSE_EXPORT size_t size() const;

	// Valid for Type::Map
	GRAPHQLRESPONSE_EXPORT bool emplace_back(std::string&& name, Value&& value);
	GRAPHQLRESPONSE_EXPORT MapType::const_iterator find(std::string_view name) const;
	GRAPHQLRESPONSE_EXPORT MapType::const_iterator begin() const;
	GRAPHQLRESPONSE_EXPORT MapType::const_iterator end() const;
	GRAPHQLRESPONSE_EXPORT const Value& operator[](std::string_view name) const;

	// Valid for Type::List
	GRAPHQLRESPONSE_EXPORT void emplace_back(Value&& value);
	GRAPHQLRESPONSE_EXPORT const Value& operator[](size_t index) const;

	// Specialized for all single-value Types.
	template <typename ValueType>
	void set(typename std::enable_if_t<std::is_same_v<std::decay_t<ValueType>, ValueType>,
		typename ValueTypeTraits<ValueType>::set_type>
			value);

	// Specialized for all Types.
	template <typename ValueType>
	typename std::enable_if_t<std::is_same_v<std::decay_t<ValueType>, ValueType>,
		typename ValueTypeTraits<ValueType>::get_type>
	get() const;

	// Specialized for all Types which allocate extra memory.
	template <typename ValueType>
	typename ValueTypeTraits<ValueType>::release_type release();

	// Compatibility wrappers
	template <typename ReferenceType>
	[[deprecated("Use the unqualified Value::set<> specialization instead of specializing on the "
				 "r-value reference.")]] void
	set(typename std::enable_if_t<std::is_rvalue_reference_v<ReferenceType>, ReferenceType> value)
	{
		set<std::decay_t<ReferenceType>>(std::move(value));
	}

	template <typename ReferenceType>
	[[deprecated("Use the unqualified Value::get<> specialization instead of specializing on the "
				 "const reference.")]]
	typename std::enable_if_t<
		std::is_lvalue_reference_v<
			ReferenceType> && std::is_const_v<typename std::remove_reference_t<ReferenceType>>,
		ReferenceType>
	get() const
	{
		return get<std::decay_t<ReferenceType>>();
	}

private:
	// Type::Map
	struct MapData
	{
		bool operator==(const MapData& rhs) const;

		MapType map;
		std::vector<size_t> members;
	};

	// Type::String
	struct StringData
	{
		bool operator==(const StringData& rhs) const;

		StringType string;
		bool from_json = false;
	};

	// Type::Null
	struct NullData
	{
		bool operator==(const NullData& rhs) const;
	};

	// Type::EnumValue
	using EnumData = StringType;

	// Type::Scalar
	struct ScalarData
	{
		bool operator==(const ScalarData& rhs) const;

		std::unique_ptr<ScalarType> scalar;
	};

	using TypeData = std::variant<MapData, ListType, StringData, NullData, BooleanType, IntType,
		FloatType, EnumData, ScalarData, CustomType>;

	TypeData _data;
};

#ifdef GRAPHQL_DLLEXPORTS
// Export all of the specialized template methods
template <>
GRAPHQLRESPONSE_EXPORT void Value::set<StringType>(StringType&& value);
template <>
GRAPHQLRESPONSE_EXPORT void Value::set<BooleanType>(BooleanType value);
template <>
GRAPHQLRESPONSE_EXPORT void Value::set<IntType>(IntType value);
template <>
GRAPHQLRESPONSE_EXPORT void Value::set<FloatType>(FloatType value);
template <>
GRAPHQLRESPONSE_EXPORT void Value::set<ScalarType>(ScalarType&& value);
template <>
GRAPHQLRESPONSE_EXPORT void Value::set<IdType>(const IdType& value);
template <>
GRAPHQLRESPONSE_EXPORT const MapType& Value::get<MapType>() const;
template <>
GRAPHQLRESPONSE_EXPORT const ListType& Value::get<ListType>() const;
template <>
GRAPHQLRESPONSE_EXPORT const StringType& Value::get<StringType>() const;
template <>
GRAPHQLRESPONSE_EXPORT BooleanType Value::get<BooleanType>() const;
template <>
GRAPHQLRESPONSE_EXPORT IntType Value::get<IntType>() const;
template <>
GRAPHQLRESPONSE_EXPORT FloatType Value::get<FloatType>() const;
template <>
GRAPHQLRESPONSE_EXPORT const ScalarType& Value::get<ScalarType>() const;
template <>
GRAPHQLRESPONSE_EXPORT IdType Value::get<IdType>() const;
template <>
GRAPHQLRESPONSE_EXPORT MapType Value::release<MapType>();
template <>
GRAPHQLRESPONSE_EXPORT ListType Value::release<ListType>();
template <>
GRAPHQLRESPONSE_EXPORT StringType Value::release<StringType>();
template <>
GRAPHQLRESPONSE_EXPORT ScalarType Value::release<ScalarType>();
template <>
GRAPHQLRESPONSE_EXPORT IdType Value::release<IdType>();
#endif // GRAPHQL_DLLEXPORTS

} // namespace graphql::response

#endif // GRAPHQLRESPONSE_H
