// Copyright (c) the JPEG XL Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// File utilities for benchmarking and testing, but which are not needed for
// main jxl itself.

#ifndef TOOLS_BENCHMARK_BENCHMARK_FILE_IO_H_
#define TOOLS_BENCHMARK_BENCHMARK_FILE_IO_H_

#include <string>
#include <vector>

#include "lib/jxl/base/file_io.h"
#include "lib/jxl/base/status.h"

namespace jxl {

// Checks if the file exists, either as file or as directory
bool PathExists(const std::string& fname);

// Checks if the file exists and is a regular file.
bool IsRegularFile(const std::string& fname);

// Checks if the file exists and is a directory.
bool IsDirectory(const std::string& fname);

// Recursively makes dir, or successfully does nothing if it already exists.
Status MakeDir(const std::string& dirname);

// Deletes a single regular file.
Status DeleteFile(const std::string& fname);

// Returns value similar to unix basename, except it returns empty string if
// fname ends in '/'.
std::string FileBaseName(const std::string& fname);
// Returns value similar to unix dirname, except returns up to before the last
// slash if fname ends in '/'.
std::string FileDirName(const std::string& fname);

// Returns the part of the filename starting from the last dot, or empty
// string if there is no dot.
std::string FileExtension(const std::string& fname);

// Matches one or more files given glob pattern.
Status MatchFiles(const std::string& pattern, std::vector<std::string>* list);

std::string JoinPath(const std::string& first, const std::string& second);

}  // namespace jxl

#endif  // TOOLS_BENCHMARK_BENCHMARK_FILE_IO_H_
