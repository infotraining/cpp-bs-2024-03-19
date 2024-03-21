#include "gadget.hpp"

#include <catch2/catch_test_macros.hpp>
#include <deque>

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

using namespace Helpers;

////////////////////////////////////////////////////////
///  PERFECT FORWARDING

void have_fun(Gadget& g)
{
    puts(__PRETTY_FUNCTION__);
    g.use();
}

void have_fun(const Gadget& cg)
{
    puts(__PRETTY_FUNCTION__);
    cg.use();
}

void have_fun(Gadget&& g)
{
    puts(__PRETTY_FUNCTION__);
    g.use();
}

namespace WithoutPerfectForwarding
{
    void use(Gadget& g)
    {
        have_fun(g);
    }

    void use(const Gadget& g)
    {
        have_fun(g);
    }

    void use(Gadget&& g)
    {
        have_fun(std::move(g));
    }
} // namespace WithoutPerfectForwarding

namespace PerfectForwarding
{
    template <typename TGadget>
    void use(TGadget&& g)
    {
        have_fun(std::forward<TGadget>(g));

        // if constexpr(std::is_reference_v<TGadget>)
        //     have_fun(g);
        // else
        //     have_fun(std::move(g))
    }  

    namespace Cpp20
    {
        void use(auto&& g)
        {
            have_fun(std::forward<decltype(g)>(g));
        }
    }  

    namespace LValue
    {
        template <typename TGadget = Gadget&>
        void use(TGadget&& g) // Gadget& && g -> Gadget&
        {
            have_fun(g);
        }    
    }

    namespace RValue
    {
        template <typename TGadget = Gadget>
        void use(TGadget&& g) // Gadget&& g
        {
            have_fun(std::move(g));
        }    
    }

    void non_template_use(Gadget&& g)
    {
        have_fun(std::move(g));
    }
}

TEST_CASE("auto and && - universal reference")
{
    std::string text = "abc";
    
    auto&& item_1 = text; // string&
    auto&& item_2 = std::string("abc"); // string&&
}

TEST_CASE("using gadget")
{
    Gadget g{1, "g"};
    const Gadget cg{2, "const g"};

    using PerfectForwarding::use;
    using PerfectForwarding::non_template_use;

    //non_template_use(g);
    use(g);
    use(cg);
    use(Gadget{3, "temporary gadget"});
}