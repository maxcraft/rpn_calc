#[[
  Copyright (c) 2026 Maksym Kravtsov.

  This file is part of rpn_calc.

  rpn_calc is free software: you can redistribute it and/or modify it under the terms
  of the GNU General Public License as published by the Free Software Foundation,
  either version 3 of the License, or (at your option) any later version.

  rpn_calc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with rpn_calc.
  If not, see <https://www.gnu.org/licenses/>.
]]

cmake_minimum_required(VERSION 4.0)

project( rpn_calc LANGUAGES CXX )

add_executable( rpn_calc lexer.cpp main.cpp )


