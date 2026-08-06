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

#include "lexer.hpp"
#include <stdexcept>

namespace {
    void forEach( lexer::Lexer::forEachCb const &cb, std::vector< lexer::State > const &states, size_t stateId = 0, std::string const &prefix = "" )
    {
        auto *state = &states[ stateId ];

        if( state->m_action )
        {
            cb( prefix, state->m_help );
        }

        for( unsigned char i = 1; i < lexer::State::stateNum; ++i )
        {
            if( state->m_states[ i ] )
            {
                forEach( cb, states, state->m_states[ i ], prefix + static_cast< char >( i ) );
            }
        }
    }
}


namespace lexer {
    State::State( Action &&action, std::string &&help )
        : m_action( std::move( action ) )
        , m_help( std::move( help ) )
    {}

    Lexer::Lexer()
        : m_states( 1 )
        {}

    void Lexer::addKeyword( const std::string_view &keyword, std::function< void() > &&function, std::string help )
    {
        m_states.reserve( m_states.size() + keyword.size() );
        size_t currentStateId { 0 };
        State *currentState = &m_states[ currentStateId ];

        for( const char c : keyword )
        {
            if( !currentState->m_states[ c ] )
            {
                m_states.emplace_back();
                // emplace_back might move the whole array to a new location
                currentState = &m_states[ currentStateId ];
                currentState->m_states[ c ] = m_states.size() - 1;
            }

            currentStateId = currentState->m_states[ c ];
            currentState = &m_states[ currentStateId ];
        }

        currentState->m_action = [ function = std::move( function )](){ function(); return true; };
        currentState->m_help = std::move( help );
    }

    bool Lexer::processKeyword( const std::string_view &keyword )
    {
        State *currentState = &m_states[ 0 ];

        for( const char c : keyword )
        {
            if( !currentState->m_states[ c ] )
            {
                // Unrecognized keyword
                return false;
            }

            currentState = &m_states[ currentState->m_states[ c ] ];
        }

        return ( currentState->m_action ) ? currentState->m_action() : false;
    }

    void Lexer::forEach( forEachCb const &cb )
    {
        ::forEach( cb, m_states );
    }

}
