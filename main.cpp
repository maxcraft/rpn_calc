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

#include <charconv>
#include <cstdlib>
#include <stack>
#include <string_view>
#include <cmath>
#include <numbers>
#include <tuple>
#include <functional>
#include <cstdio>
#include <string>
#include <stdexcept>
#include <format>
#include <random>

#include "lexer.hpp"

namespace
{
    template< class T, class ...Args >
    std::tuple<T, Args...> do_read_args( std::stack< double > &mem )
    {
        if constexpr ( sizeof...( Args ) == 0 )
        {
            if( mem.size() < 1 ) throw std::runtime_error( "Not enought data" );

            auto val = mem.top();
            mem.pop();
            return std::make_tuple( val );
        }
        else
        {
            auto tail = do_read_args<Args...>( mem );

            if( mem.size() < 1 ) throw std::runtime_error( "Not enought data" );

            auto val = mem.top();
            mem.pop();
            return std::tuple_cat( std::make_tuple( val ), tail );
        }
    }


    template<class R, class... Args>
    void do_calc( std::stack< double > &mem, std::function< R(Args...)> func )
    {
        if constexpr ( sizeof...(Args) )
        {
            mem.push( std::apply( func, do_read_args< Args... >( mem ) ) );
        }
        else
        {
            mem.push( func() );
        }
    }

    template<class L>
    void do_calc( std::stack< double > &mem, L const &lambda )
    {
        std::function f = lambda;
        do_calc( mem, std::move( f ) );
    }

    void print_error( auto what, int index, auto token )
    {
        puts( std::format( "Error: {}\nToken {}: \'{}\'", what, index, token ).c_str() );
    }

    constexpr auto UNKNOWN_TOKEN = "Unknown token";

    void printHelp( lexer::Lexer &lexer )
    {
        std::puts( "Calculates the expression in Reverse Polish Notation. The expression comes as command line arguments." );
        std::puts( "For example: ./rpn_calc 1 2 3 4 5 - - - -" );
        std::puts( "-h or --help keywordrs will cause the calculator stop evaluating the expression and print this help." );
        std::puts( "variables x, y and z mentioned below have the following meanings: x = top, y = top-1, z = top-2" );
        std::puts( "Supported keywords:" );

        lexer.forEach([]( std::string const &keyword, std::string const &help ){ std::puts( std::format( " {:10}   -- {}", keyword, help ).c_str() ); } );
    }

    double rnd()
    {
        static std::random_device rd;
        static std::mt19937_64 gen(rd());
        return std::generate_canonical<double, -1u>(gen);
    }
}

int main (int argc, char *argv[]) {
    std::stack< double > mem;

    lexer::Lexer aLexer{};

    aLexer.addKeyword( "-h",[&aLexer](){ printHelp( aLexer ); std::exit( EXIT_SUCCESS ); }, "Prints this help and exits" );
    aLexer.addKeyword( "--help",[&aLexer](){ printHelp( aLexer ); std::exit( EXIT_SUCCESS ); }, "Prints this help and exits" );

    aLexer.addKeyword( "+", [&mem](){ do_calc(mem, []( double a, double b ) -> double { return a + b; } ); }, "Adds two operands" );
    aLexer.addKeyword( "-", [&mem](){ do_calc(mem, []( double a, double b ) -> double { return a - b; } ); }, "Subtructs y-x, where x was added after y (example: 15 3 -; y = 15, x = 3)" );
    aLexer.addKeyword( "*", [&mem](){ do_calc(mem, []( double a, double b ) -> double { return a * b; } ); }, "Multiplies two operands" );
    aLexer.addKeyword( "/", [&mem](){ do_calc(mem, []( double a, double b ) -> double { return a / b; } ); }, "Divides y/x, where x was added after y (example: 15 3 /; y = 15, x = 3)" );
    aLexer.addKeyword( "^", [&mem](){ do_calc(mem, []( double a, double b ) -> double { return std::pow( a, b ); } ); }, "Calculates y in power of x (y^x)" );
    aLexer.addKeyword( "abs", [&mem](){ do_calc(mem, []( double a ){ return std::abs( a ); } ); }, "Calculates absolute value of x" );
    aLexer.addKeyword( "sqrt", [&mem](){ do_calc(mem, []( double a ){ return std::sqrt( a ); } ); }, "Calculates square root" );
    aLexer.addKeyword( "cbrt", [&mem](){ do_calc(mem, []( double a ){ return std::cbrt( a ); } ); }, "Calculates cubic root" );
    aLexer.addKeyword( "sqr",  [&mem](){ do_calc(mem, []( double a ){ return a * a; } ); }, "Calculates x*x, or x^2" );
    aLexer.addKeyword( "cub",  [&mem](){ do_calc(mem, []( double a ){ return a * a * a; } ); }, "Calculates x*x*x, or x^3" );
    aLexer.addKeyword( "sin", [&mem](){ do_calc(mem, []( double a ){ return std::sin( a ); } ); }, "Calculates sin(x)" );
    aLexer.addKeyword( "cos", [&mem](){ do_calc(mem, []( double a ){ return std::cos( a ); } ); }, "Calculates cos(x)" );
    aLexer.addKeyword( "tan", [&mem](){ do_calc(mem, []( double a ){ return std::tan( a ); } ); }, "Calculates tan(x)" );
    aLexer.addKeyword( "atan", [&mem](){ do_calc(mem, []( double a ){ return std::atan( a ); } ); }, "Calculates atan(x)" );
    aLexer.addKeyword( "asin", [&mem](){ do_calc(mem, []( double a ){ return std::asin( a ); } ); }, "Calculates asin(x)" );
    aLexer.addKeyword( "acos", [&mem](){ do_calc(mem, []( double a ){ return std::acos( a ); } ); }, "Calculates acos(x)" );
    aLexer.addKeyword( "atan2", [&mem](){ do_calc(mem, []( double a, double b ){ return std::atan2( a, b ); } ); }, "Calculates atan(y,x)" );
    aLexer.addKeyword( "cosh", [&mem](){ do_calc(mem, []( double a ){ return std::cosh( a ); } ); }, "Calculates cosh(x)" );
    aLexer.addKeyword( "sinh", [&mem](){ do_calc(mem, []( double a ){ return std::sinh( a ); } ); }, "Calculates sinh(x)" );
    aLexer.addKeyword( "tanh", [&mem](){ do_calc(mem, []( double a ){ return std::tanh( a ); } ); }, "Calculates tanh(x)" );
    aLexer.addKeyword( "acosh", [&mem](){ do_calc(mem, []( double a ){ return std::acosh( a ); } ); }, "Calculates acosh(x)" );
    aLexer.addKeyword( "asinh", [&mem](){ do_calc(mem, []( double a ){ return std::asinh( a ); } ); }, "Calculates asinh(x)" );
    aLexer.addKeyword( "atanh", [&mem](){ do_calc(mem, []( double a ){ return std::atanh( a ); } ); }, "Calculates atanh(x)" );

    aLexer.addKeyword( "mod", [&mem](){ do_calc(mem, []( double a, double b ){ return std::fmod( a, b ); } ); }, "Calculates floating point remainder of division y/x" );
    aLexer.addKeyword( "remainder", [&mem](){ do_calc(mem, []( double a, double b ){ return std::remainder( a, b ); } ); }, "Calculates the IEEE remainder of the floating point division y/x" );
    aLexer.addKeyword( "fma", [&mem](){ do_calc(mem, []( double a, double b, double c ){ return std::fma( a, b,c ); } ); }, "Calculates z*y+x" );

    aLexer.addKeyword( "max", [&mem](){ do_calc(mem, []( double a, double b ){ return std::fmax( a, b ); } ); }, "Calculates max(x,y)" );
    aLexer.addKeyword( "min", [&mem](){ do_calc(mem, []( double a, double b ){ return std::fmin( a, b ); } ); }, "Calculates min(x,y)" );

    aLexer.addKeyword( "dim", [&mem](){ do_calc(mem, []( double a, double b ){ return std::fdim( a, b ); } ); }, "Calculates positive difference of two floating point values max(0,y-x)" );
    aLexer.addKeyword( "lerp", [&mem](){ do_calc(mem, []( double a, double b, double c ){ return std::lerp( a, b, c ); } ); }, "Calculates linear interpolation function" );

    aLexer.addKeyword( "exp", [&mem](){ do_calc(mem, []( double a ){ return std::exp( a ); } ); }, "Calculates e rised to power x" );
    aLexer.addKeyword( "exp2", [&mem](){ do_calc(mem, []( double a ){ return std::exp2( a ); } ); }, "Calculates 2^x" );
    aLexer.addKeyword( "expm1", [&mem](){ do_calc(mem, []( double a ){ return std::expm1( a ); } ); }, "Calculates (e^x)-1" );

    aLexer.addKeyword( "log", [&mem](){ do_calc(mem, []( double a ){ return std::log( a ); } ); }, "Calculates ln(x)" );
    aLexer.addKeyword( "ln", [&mem](){ do_calc(mem, []( double a ){ return std::log( a ); } ); }, "Calculates ln(x)" );
    aLexer.addKeyword( "log10", [&mem](){ do_calc(mem, []( double a ){ return std::log10( a ); } ); }, "Calculates log10(x)" );
    aLexer.addKeyword( "log2", [&mem](){ do_calc(mem, []( double a ){ return std::log2( a ); } ); }, "Calculates log2(x)" );
    aLexer.addKeyword( "log1p", [&mem](){ do_calc(mem, []( double a ){ return std::log1p( a ); } ); }, "Calculates ln(1+x)" );

    aLexer.addKeyword( "hypot", [&mem](){ do_calc(mem, []( double a, double b ){ return std::hypot( a, b ); } ); }, "Calculates sqrt(x*x+y*y)" );
    aLexer.addKeyword( "hypot3", [&mem](){ do_calc(mem, []( double a, double b, double c ){ return std::hypot( a, b, c ); } ); }, "Calculates sqrt(x*x+y*y+z*z)" );
    aLexer.addKeyword( "erf", [&mem](){ do_calc(mem, []( double a ){ return std::erf( a ); } ); }, "Calculates error function" );
    aLexer.addKeyword( "erfc", [&mem](){ do_calc(mem, []( double a ){ return std::erfc( a ); } ); }, "Calculates complementary error function" );
    aLexer.addKeyword( "tgamma", [&mem](){ do_calc(mem, []( double a ){ return std::tgamma( a ); } ); }, "Calculates gamma function" );
    aLexer.addKeyword( "lgamma", [&mem](){ do_calc(mem, []( double a ){ return std::lgamma( a ); } ); }, "Calculates natural logarithm of the gamma function" );
 
    aLexer.addKeyword( "ceil", [&mem](){ do_calc(mem, []( double a ){ return std::ceil( a ); } ); }, "Calculates nearest integer not less than the given value" );
    aLexer.addKeyword( "floor", [&mem](){ do_calc(mem, []( double a ){ return std::floor( a ); } ); }, "Calculates nearest integer not greater than the given value" );
    aLexer.addKeyword( "trunc", [&mem](){ do_calc(mem, []( double a ){ return std::trunc( a ); } ); }, "Calculates nearest integer not greater in magnitude than the given value" );
    aLexer.addKeyword( "round", [&mem](){ do_calc(mem, []( double a ){ return std::round( a ); } ); }, "Calculates nearest integer, rounding away from zero in halfway cases" );

    aLexer.addKeyword( "rnd", [&mem](){ do_calc(mem, [](){ return ::rnd(); } ); }, "Generate a random value from in range [0.0,1.0)" );
    
    // Constants
    aLexer.addKeyword( "e", [&mem](){ do_calc(mem, [](){ return std::numbers::e; } ); }, "The mathematical constant e" );
    aLexer.addKeyword( "log2e", [&mem](){ do_calc(mem, [](){ return std::numbers::log2e; } ); }, "log2(e) constant" );
    aLexer.addKeyword( "log10e", [&mem](){ do_calc(mem, [](){ return std::numbers::log10e; } ); }, "log10(e) constant" );
    aLexer.addKeyword( "pi", [&mem](){ do_calc(mem, [](){ return std::numbers::pi; } ); }, "The mathematical constant PI" );
    aLexer.addKeyword( "inv_pi", [&mem](){ do_calc(mem, [](){ return std::numbers::inv_pi; } ); }, "1/PI constant" );
    aLexer.addKeyword( "inv_sqrtpi", [&mem](){ do_calc(mem, [](){ return std::numbers::inv_sqrtpi; } ); }, "1/sqrt(PI) constant" );
    aLexer.addKeyword( "ln2", [&mem](){ do_calc(mem, [](){ return std::numbers::ln2; } ); }, "log(2) constant" );
    aLexer.addKeyword( "ln10", [&mem](){ do_calc(mem, [](){ return std::numbers::ln10; } ); }, "log(10) constant" );
    aLexer.addKeyword( "sqrt2", [&mem](){ do_calc(mem, [](){ return std::numbers::sqrt2; } ); }, "sqrt(2) constant" );
    aLexer.addKeyword( "sqrt3", [&mem](){ do_calc(mem, [](){ return std::numbers::sqrt3; } ); }, "sqrt(3) constant" );
    aLexer.addKeyword( "inv_sqrt3", [&mem](){ do_calc(mem, [](){ return std::numbers::inv_sqrt3; } ); }, "1/sqrt(3) constant" );
    aLexer.addKeyword( "egamma", [&mem](){ do_calc(mem, [](){ return std::numbers::egamma; } ); }, "The Euler–Mascheroni constant" );
    aLexer.addKeyword( "phi", [&mem](){ do_calc(mem, [](){ return std::numbers::phi; } ); }, "Golden ratio constant" );

    aLexer.addKeyword( "E", [&mem](){ do_calc(mem, [](){ return std::numbers::e; } ); }, "The mathematical constant e" );
    aLexer.addKeyword( "LOG2E", [&mem](){ do_calc(mem, [](){ return std::numbers::log2e; } ); }, "log2(e) constant" );
    aLexer.addKeyword( "LOG10E", [&mem](){ do_calc(mem, [](){ return std::numbers::log10e; } ); }, "log10(e) constant" );
    aLexer.addKeyword( "PI", [&mem](){ do_calc(mem, [](){ return std::numbers::pi; } ); }, "The mathematical constant PI" );
    aLexer.addKeyword( "INV_PI", [&mem](){ do_calc(mem, [](){ return std::numbers::inv_pi; } ); }, "1/PI constant" );
    aLexer.addKeyword( "INV_SQRTPI", [&mem](){ do_calc(mem, [](){ return std::numbers::inv_sqrtpi; } ); }, "1/sqrt(PI) constant" );
    aLexer.addKeyword( "LN2", [&mem](){ do_calc(mem, [](){ return std::numbers::ln2; } ); }, "log(2) constant" );
    aLexer.addKeyword( "LN10", [&mem](){ do_calc(mem, [](){ return std::numbers::ln10; } ); }, "log(10) constant" );
    aLexer.addKeyword( "SQRT2", [&mem](){ do_calc(mem, [](){ return std::numbers::sqrt2; } ); }, "sqrt(2) constant" );
    aLexer.addKeyword( "SQRT3", [&mem](){ do_calc(mem, [](){ return std::numbers::sqrt3; } ); }, "sqrt(2) constant" );
    aLexer.addKeyword( "INV_SQRT3", [&mem](){ do_calc(mem, [](){ return std::numbers::inv_sqrt3; } ); }, "1/sqrt(3) constant" );
    aLexer.addKeyword( "EGAMMA", [&mem](){ do_calc(mem, [](){ return std::numbers::egamma; } ); }, "The Euler–Mascheroni constant" );
    aLexer.addKeyword( "PHI", [&mem](){ do_calc(mem, [](){ return std::numbers::phi; } ); }, "Golden ratio constant" );


    for( int i = 1; i < argc; ++i )
    {
        try
        {
            std::string_view item { argv[ i ] };

            if( aLexer.processKeyword( item ) ) continue;

            double value{};
            size_t offset{ 0 };

            if( item.size() > 1  && item[ 0 ] == '+' && item[ 1 ] != '-'  ) offset = 1; // skip the leading '+' to make it parsed properly

            auto [ ptr, ec ] = std::from_chars( item.begin() + offset, item.end(), value );

            if( ec == std::errc() && *ptr == '\0' )
            {
                mem.push( value );
                continue;
            }

            print_error( UNKNOWN_TOKEN, i, argv[ i ] );
            return EXIT_FAILURE;
        }
        catch( const std::exception &ex )
        {
            print_error( ex.what(), i, argv[ i ] );
            return EXIT_FAILURE;
        }
    }

    std::puts( std::format( "{}", mem.top() ).c_str() );

    return EXIT_SUCCESS;
}
