// +----------------------------------------------------------------------
// | Copyright (c) 2014.
// +----------------------------------------------------------------------
// | * Redistribution and use of this software in source and binary forms,
// |   with or without modification, are permitted provided that the following
// |   conditions are met:
// |
// | * Redistributions of source code must retain the above
// |   copyright notice, this list of conditions and the
// |   following disclaimer.
// |
// | * Redistributions in binary form must reproduce the above
// |   copyright notice, this list of conditions and the
// |   following disclaimer in the documentation and/or other
// |   materials provided with the distribution.
// |
// | * Neither the name of the ray team, nor the names of its
// |   contributors may be used to endorse or promote products
// |   derived from this software without specific prior
// |   written permission of the ray team.
// |
// | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// | "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// | LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// | A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// | OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// | SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// | LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// | DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// | THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// | (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// | OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// +----------------------------------------------------------------------

#ifndef _H_LIBSCRIPT_FUNCTION_H_
#define _H_LIBSCRIPT_FUNCTION_H_

#include <vector>
#include "libscript_table.h"
#include "libscript_pusher.h"

/// @addtogroup script
/// @{

_NAME_BEGIN

class EXPORT Function : public Value
{
public:
    Function(const Stack& stack);
    Function(const Value& value);
    Function(const Function& copy);
    ~Function();
    
    /// @brief Call this function and given arguments
    template<typename ... _Args>
    Function& call(_Args ... arg)
    {
        return operator()(arg ...);
    }

    template<typename ... _Args>
    Function& operator()(_Args ... arg)
    {
        std::tuple<_Args ...> tuple(arg ...);
        argReset();
        pushRef(T_Function);
        _ArgPusher<std::tuple<_Args ...>, sizeof...(_Args)>::push(_pusher, tuple);
        raw_call();
        return *this;
    }

    /// @brief Get the number of last results
    int resultCount() const { return _pusher.count(); }

    /// @brief Save last results to a new table
    Table getReuslt();

    /// @brief Get last result's value
    /// @note When to index a nonexistent position, will be return a nil
    /// (Index begin with 1)
    Value& operator[](int i);

private:
    template<typename _Tuple, std::size_t _ArgN>
    class _ArgPusher {
    public:
        static void push(Pusher& pusher, _Tuple& tuple)
        {
            _ArgPusher<_Tuple, _ArgN - 1>::push(pusher, tuple);
            pusher.push(std::get<_ArgN - 1>(tuple));
        }
    };

    template<typename _Tuple>
    class _ArgPusher<_Tuple, 0> {
    public:
        static void push(Pusher& pusher, _Tuple& tuple) { }
    };

    void argReset() { _pusher.reset(); _beform_stack_size = gettop(); }

    void raw_call();

private:
    int     _beform_stack_size;
    int     _resultNum;
    Pusher  _pusher;
    Value*  _nil;
    std::vector<Value*> _result;
};

/// @}

_NAME_END

#endif
