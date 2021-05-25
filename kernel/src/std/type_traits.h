#pragma once

namespace std
{

    template<typename TYPE, TYPE VALUE>
    struct Constant
    {
        using Type = TYPE;
        inline static constexpr Type Value = VALUE;
    };


    template<bool VALUE>
    typedef Constant<bool, VALUE> neuron_bool;

    using neuron_true = neuron_bool<true>;
    using neuron_false = neuron_bool<false>;


    template<typename A, typename B>
    struct IsSame : public neuron_false
    {

    };
    template<typename A>
    struct IsSame<A, A> : public neuron_true
    {

    };


    template<bool VALUE, typename PASS>
    struct EnableIf
    {
        using Type = void;

        static_assert(VALUE, "Static assert thrown");
    };
    template<typename PASS>
    struct EnableIf<true, PASS>
    {
        using Type = PASS;
    };


    template<typename Base, typename Derived>
    struct IsBaseOf : public neuron_bool<__is_base_of(Base, Derived)>
    {

    };

}