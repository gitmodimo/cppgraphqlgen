// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

module;

#include "ValidationSchema.h"

export module GraphQL.Validation.ValidationSchema;

export import GraphQL.Validation.SentientObject;
export import GraphQL.Validation.PetObject;
export import GraphQL.Validation.NodeObject;
export import GraphQL.Validation.ResourceObject;
export import GraphQL.Validation.CatOrDogObject;
export import GraphQL.Validation.DogOrHumanObject;
export import GraphQL.Validation.HumanOrAlienObject;
export import GraphQL.Validation.QueryObject;
export import GraphQL.Validation.DogObject;
export import GraphQL.Validation.AlienObject;
export import GraphQL.Validation.HumanObject;
export import GraphQL.Validation.CatObject;
export import GraphQL.Validation.MutationObject;
export import GraphQL.Validation.MutateDogResultObject;
export import GraphQL.Validation.SubscriptionObject;
export import GraphQL.Validation.MessageObject;
export import GraphQL.Validation.ArgumentsObject;

export namespace graphql::validation {

using validation::DogCommand;
using validation::getDogCommandNames;
using validation::getDogCommandValues;

using validation::CatCommand;
using validation::getCatCommandNames;
using validation::getCatCommandValues;

using validation::ComplexInput;

using validation::Operations;

using validation::AddSentientDetails;
using validation::AddPetDetails;
using validation::AddNodeDetails;
using validation::AddResourceDetails;
using validation::AddCatOrDogDetails;
using validation::AddDogOrHumanDetails;
using validation::AddHumanOrAlienDetails;
using validation::AddQueryDetails;
using validation::AddDogDetails;
using validation::AddAlienDetails;
using validation::AddHumanDetails;
using validation::AddCatDetails;
using validation::AddMutationDetails;
using validation::AddMutateDogResultDetails;
using validation::AddSubscriptionDetails;
using validation::AddMessageDetails;
using validation::AddArgumentsDetails;

using validation::GetSchema;

} // namespace graphql::validation