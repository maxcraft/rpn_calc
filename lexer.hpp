/*
 *  Copyright (c) 2026 Maksym Kravtsov.
 *
 *  This file is part of rpn_calc.
 *
 *  rpn_calc is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Foundation,
 *  either version 3 of the License, or (at your option) any later version.
 *
 *  rpn_calc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with rpn_calc.
 *  If not, see <https://www.gnu.org/licenses/>.
 * */

#pragma once
#include <functional>
#include <string_view>
#include <memory>
#include <string>

namespace lexer
{
    using Action = std::function< bool() >;
    struct State
    {
        static constexpr size_t stateNum = 128;
        Action m_action { nullptr };
        std::string m_help{};
        size_t m_states [ stateNum ] = { 0 };

        State() = default;
        State( Action &&action, std::string &&help = "" );
    };

    class Lexer
    {
        public:
            Lexer();
            void addKeyword( const std::string_view &keyword, std::function< void() > &&function, std::string help = "" );
            bool processKeyword( const std::string_view &keyword );
            
            using forEachCb = std::function< void( std::string const &, std::string const &) >;
            void forEach( forEachCb const &cb );

        private:
            std::vector< State > m_states;
    };
}
