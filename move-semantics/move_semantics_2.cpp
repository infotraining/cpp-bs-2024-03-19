#include "gadget.hpp"

#include <catch2/catch_test_macros.hpp>
#include <utility>

////////////////////////////////////////////////
// simplified implementation of unique_ptr - only moveable type

namespace Explain
{
    template <typename T>
    class unique_ptr
    {
    public:
        explicit unique_ptr(T* ptr)
            : ptr_{ptr}
        {
        }

        unique_ptr(const unique_ptr& other) = delete;
        unique_ptr& operator=(const unique_ptr& other) = delete;

        // move constructor
        unique_ptr(unique_ptr&& other)
            : ptr_{std::exchange(other.ptr_, nullptr)}
        {
        }

        // move assignment
        unique_ptr& operator=(unique_ptr&& other)
        {
            if (this != &other)
            {
                delete ptr_;

                // ptr_ = other.ptr_;
                // other.ptr_ = nullptr;
                ptr_ = std::exchange(other.ptr_, nullptr);
            }
            return *this;
        }

        ~unique_ptr()
        {
            delete ptr_;
        }

        operator bool() const
        {
            return ptr_ != nullptr;
        }

        T& operator*() const
        {
            return *ptr_;
        }

        T* operator->() const
        {
            return ptr_;
        }

        T* get() const
        {
            return ptr_;
        }

    private:
        T* ptr_;
    };
} // namespace Explain

Explain::unique_ptr<Helpers::Gadget> create_gadget(int id)
{
    using Helpers::Gadget;
    using namespace std::literals;
    return Explain::unique_ptr<Gadget>(new Gadget(id, "Gadget#"s + std::to_string(id)));
}

TEST_CASE("move semantics - unique_ptr")
{
    using Helpers::Gadget;

    {
        Explain::unique_ptr<Gadget> ptr_g(new Gadget(42, "ipad"));
        ptr_g->use();
        (*ptr_g).use();

        // Explain::unique_ptr<Gadget> other_ptr_g = ptr_g;
        // other_ptr_g->use();

        Explain::unique_ptr<Gadget> target_ptr_g = std::move(ptr_g);

        CHECK(ptr_g.get() == nullptr);
        if (target_ptr_g)
            target_ptr_g->use();
    }
    std::cout << "-------------------\n";
}

struct Tester
{
    std::vector<int> data;

    Tester(std::initializer_list<int> lst) : data{lst}
    {
    }

    Tester(const Tester&) = default;
    Tester& operator=(const Tester&) = default;
    Tester(Tester&&) = default;
    Tester& operator=(Tester&&) = default;

    ~Tester() { /*....*/ }
};

TEST_CASE("special functions")
{
    Tester a{1, 2, 3};
    Tester b = a; // cc
    Tester c = std::move(a); // mv
    CHECK(a.data.size() == 0);
    CHECK(c.data == std::vector{1, 2, 3});
}

TEST_CASE("move semantics")
{
    using Helpers::Gadget;

    std::vector<Explain::unique_ptr<Gadget>> gadgets;

    gadgets.push_back(create_gadget(42));
    gadgets.push_back(create_gadget(665));

    auto ptr_g = create_gadget(777);
    gadgets.push_back(std::move(ptr_g));

    for(const auto& ptr_g : gadgets)
    {
        if (ptr_g)
            ptr_g->use();
    }

    SECTION("primitive types - copy")
    {
        int x = 10;
        int* ptr = &x;

        int y = std::move(x); // copy
        int* other_ptr = std::move(ptr); // copy
    }

    SECTION("class/struct - maybe move")
    {
        std::vector<int> vec = {1, 2, 3};

        std::vector<int> target_vec = std::move(vec);
        CHECK(target_vec == std::vector{1, 2, 3});
        CHECK(vec.size() == 0);
    }
}

struct CompositeObject
{
    std::string id;
    std::shared_ptr<Helpers::Gadget> g;
    std::vector<int> data;

    CompositeObject(std::string id) : id{std::move(id)}
    {}

    // Rule of Zero
};