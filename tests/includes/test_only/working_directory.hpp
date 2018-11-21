/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

Product name: redemption, a FLOSS RDP proxy
Copyright (C) Wallix 2010-2018
Author(s): Jonathan Poelen
*/

#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <unordered_set>


#define RED_CHECK_WORKSPACE(wd) RED_CHECK_EQUAL(wd.unmached_files(), "")

struct [[nodiscard]] WorkingDirectory
{
    WorkingDirectory(WorkingDirectory const&) = delete;

    WorkingDirectory(std::string_view dirname);

    [[nodiscard]] WorkingDirectory& create_subdirectory(std::string_view directory);

    /*
     * filename with '/' at back is a directory
     */
    /// @{
    [[nodiscard]] std::string add_file(std::string file);
    [[nodiscard]] WorkingDirectory& add_files(std::initializer_list<std::string> files);
    [[nodiscard]] WorkingDirectory& remove_files(std::initializer_list<std::string> files);
    /// @}

    std::string path_of(std::string_view path) const;

    std::string const& dirname() const noexcept;

    [[nodiscard]] std::string unmached_files();

    ~WorkingDirectory() noexcept(false);

private:
    WorkingDirectory(std::string_view dirname, std::string_view base);

    enum class Type : bool
    {
        File, Directory
    };

    struct Path
    {
        std::string name;
        std::unique_ptr<WorkingDirectory> child;
        Type type;
        mutable int counter_id;

        Path() noexcept;

        Path(std::string name, int counter_id) noexcept;

        Path(std::unique_ptr<WorkingDirectory>&& child, int counter_id) noexcept;

        bool operator == (Path const& other) const;
    };

    struct HashPath
    {
        std::size_t operator()(Path const& path) const;
    };

    std::unordered_set<Path, HashPath> paths;
    std::string directory;
    bool has_error = false;
    bool is_checked = true;
    int counter_id = 0;
};
