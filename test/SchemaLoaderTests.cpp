// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <gtest/gtest.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace {

// Read an entire file into a string.
std::string readFile(const std::filesystem::path& path)
{
	std::ifstream ifs(path);
	std::ostringstream oss;
	oss << ifs.rdbuf();
	return oss.str();
}

// RAII wrapper for a temporary output directory.
class TempDir
{
public:
	TempDir()
	{
		_path = std::filesystem::temp_directory_path()
			/ ("schemagen_test_" + std::to_string(std::rand()));
		std::filesystem::create_directories(_path);
	}

	~TempDir()
	{
		std::filesystem::remove_all(_path);
	}

	[[nodiscard]] const std::filesystem::path& path() const noexcept
	{
		return _path;
	}

private:
	std::filesystem::path _path;
};

// Run schemagen and return its exit code.
int runSchemagen(const std::string& schemaFile, const std::string& prefix,
	const std::string& ns, const std::string& outputDir)
{
	std::string cmd = std::string(SCHEMAGEN_PATH)
		+ " --schema=" + schemaFile
		+ " --prefix=" + prefix
		+ " --namespace=" + ns
		+ " --header-dir=" + outputDir
		+ " --source-dir=" + outputDir
		+ " --stubs --no-introspection 2>&1";
	return std::system(cmd.c_str());
}

} // anonymous namespace

// Regression test: run schemagen on a schema where a builtin-returning field
// (String) takes a nullable input object argument (TestInput). The generated
// concept must use std::unique_ptr<TestInput>, not std::optional<TestInput>.
TEST(SchemaGenCase, NullableInputArgOnBuiltinReturnField)
{
	TempDir outDir;
	const std::string schemaFile =
		std::string(SCHEMA_TEST_DIR) + "/schema.nullable_input_builtin_return.graphql";

	int rc = runSchemagen(schemaFile, "Test", "test", outDir.path().string());
	ASSERT_EQ(rc, 0) << "schemagen should succeed";

	// Check the generated object header for the concept declaration.
	const auto headerContent = readFile(outDir.path() / "QueryObject.h");
	ASSERT_FALSE(headerContent.empty()) << "QueryObject.h should be generated";

	// The nullable input object argument should use unique_ptr in the concept.
	EXPECT_NE(headerContent.find("std::unique_ptr<TestInput>"), std::string::npos)
		<< "concept should use std::unique_ptr<TestInput> for nullable input object argument";
	EXPECT_EQ(headerContent.find("std::optional<TestInput>"), std::string::npos)
		<< "concept should NOT use std::optional<TestInput> for nullable input object argument";

	// Also check the source file for consistent usage.
	const auto sourceContent = readFile(outDir.path() / "QueryObject.cpp");
	ASSERT_FALSE(sourceContent.empty()) << "QueryObject.cpp should be generated";

	EXPECT_NE(sourceContent.find("unique_ptr<TestInput>"), std::string::npos)
		<< "resolver should use unique_ptr<TestInput>";
}

// Regression test: run schemagen on a schema where a builtin-returning field
// (String) takes a non-null input object argument with an empty default (TestInput! = {}).
// schemagen must not crash and the argument must be recognized as an input type.
TEST(SchemaGenCase, NonNullInputEmptyDefaultOnBuiltinReturnField)
{
	TempDir outDir;
	const std::string schemaFile =
		std::string(SCHEMA_TEST_DIR) + "/schema.nonnull_input_empty_default.graphql";

	int rc = runSchemagen(schemaFile, "Test", "test", outDir.path().string());
	ASSERT_EQ(rc, 0) << "schemagen should succeed without crashing";

	// Check the generated source for the argument conversion code.
	const auto sourceContent = readFile(outDir.path() / "QueryObject.cpp");
	ASSERT_FALSE(sourceContent.empty()) << "QueryObject.cpp should be generated";

	// For a non-null input object argument, the generated code should use
	// ModifiedArgument<test::TestInput> (not a builtin type).
	EXPECT_NE(sourceContent.find("ModifiedArgument<test::TestInput>"), std::string::npos)
		<< "resolver should use ModifiedArgument<test::TestInput>";
}
