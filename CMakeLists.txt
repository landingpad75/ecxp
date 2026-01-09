cmake_minimum_required(VERSION 3.10)

project(EcliptixPlus)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_definitions( 
   -D STD_CONSOLE
   -D STD_ECLIPTIX
   -D STD_FILE
#   -D STD_GRAPHICS
   -D STD_MATH
)

add_library(EcliptixLex STATIC
    src/lexer/lexer.cpp
    src/lexer/stringify.cpp

    include/lexer/lexer.hpp
)

add_library(StandardLibrary STATIC
    src/std/console/ask.cpp
    src/std/console/error.cpp
    src/std/console/out.cpp
    src/std/console/outraw.cpp

    src/std/ecx/exit.cpp
    src/std/ecx/wait.cpp

    src/std/file/read.cpp
    src/std/file/write.cpp

    src/std/graphics/close.cpp
    src/std/graphics/integrity.cpp
    src/std/graphics/log.cpp
    src/std/graphics/loop.cpp
    src/std/graphics/rect.cpp
    src/std/graphics/rectround.cpp
    src/std/graphics/running.cpp
    src/std/graphics/showfps.cpp
    src/std/graphics/triangle.cpp

    src/std/math/abs.cpp
    src/std/math/cos.cpp
    src/std/math/log.cpp
    src/std/math/max.cpp
    src/std/math/min.cpp
    src/std/math/pow.cpp
    src/std/math/round.cpp
    src/std/math/sin.cpp
    src/std/math/sqrt.cpp

    src/std/common.cpp
)

add_library(EcliptixParse STATIC
    src/parser/parser.cpp
    src/parser/parserUtils.cpp
    src/parser/expressions/add.cpp
    src/parser/expressions/args.cpp
    src/parser/expressions/array.cpp
    src/parser/expressions/assign.cpp
    src/parser/expressions/call.cpp
    src/parser/expressions/dsn.cpp
    src/parser/expressions/expr.cpp
    src/parser/expressions/member.cpp
    src/parser/expressions/memberCall.cpp
    src/parser/expressions/mul.cpp
    src/parser/expressions/object.cpp
    src/parser/expressions/primary.cpp

    src/parser/statements/fn.cpp
    src/parser/statements/if.cpp
    src/parser/statements/stmt.cpp
    src/parser/statements/vars-fn.cpp
    src/parser/statements/when.cpp
    src/parser/statements/while.cpp

    include/parser/parser.hpp

    src/lexer/stringify.cpp
)

add_library(EcliptixOptimize STATIC
    src/optimizer/optimizer.cpp

    include/optimizer/optimizer.hpp
)

add_library(EcliptixInterpreter STATIC
    src/executor/expressions/array.cpp
    src/executor/expressions/assignment.cpp
    src/executor/expressions/binary.cpp
    src/executor/expressions/call.cpp
    src/executor/expressions/dsn.cpp
    src/executor/expressions/equality.cpp
    src/executor/expressions/ident.cpp
    src/executor/expressions/member.cpp
    src/executor/expressions/number.cpp
    src/executor/expressions/object.cpp
    src/executor/expressions/program.cpp
    src/executor/expressions/string.cpp

    src/executor/statements/function.cpp
    src/executor/statements/if.cpp
    src/executor/statements/variable.cpp
    src/executor/statements/when.cpp
    src/executor/statements/while.cpp

    src/executor/executor.cpp
    include/executor/executor.hpp
)

add_library(EcliptixUtilities STATIC
    src/utilities/config.cpp
    src/utilities/utils.cpp
    src/utilities/values.cpp

    include/utilities/env.hpp
    include/utilities/toml.hpp
    include/utilities/utils.hpp
)

add_executable(ecxp src/main.cpp)

include_directories(
    include
)

target_link_libraries(ecxp PRIVATE EcliptixLex EcliptixParse EcliptixOptimize EcliptixInterpreter StandardLibrary EcliptixUtilities)