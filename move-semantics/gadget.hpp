#ifndef GADGET_HPP
#define GADGET_HPP

#include <iostream>
#include <string>

namespace Helpers
{
    class Gadget
    {
        int id_;
        std::string name_;

    public:
        static int gen_id()
        {
            static int id_seed;
            return ++id_seed;
        }

        Gadget()
            : id_{gen_id()}
            , name_{std::string("Gadget#") + std::to_string(id_)}
        {
            std::cout << "Gadget(" << id_ << ", " << name_ << ")" << std::endl;
        }

        Gadget(int id, const std::string& name = "unknown")
            : id_{id}
            , name_{name}
        {
            std::cout << "Gadget(" << id_ << ", " << name_ << ")" << std::endl;
        }

        ~Gadget()
        {
            std::cout << "~Gadget(" << (name_.empty() ? "after-move" : name_) << ", " << id_ << ")" << std::endl;
        }

        Gadget(const Gadget& source)
            : id_{source.id_}
            , name_{source.name_}
        {
            std::cout << "Gadget(cc: " << id_ << ", " << name_ << ")" << std::endl;
        }

        Gadget& operator=(const Gadget& source)
        {
            if (this != &source)
            {
                id_ = source.id_;
                name_ = source.name_;

                std::cout << "Gadget::operator=(cpy: " << id_ << ", " << name_ << ")" << std::endl;
            }

            return *this;
        }

#ifdef ENABLE_MOVE_SEMANTICS

        Gadget(Gadget&& source) noexcept
            : id_{source.id_}
            , name_{std::move(source.name_)}
        {
            if (this != &source)
            {
                std::cout << "Gadget(mv: " << id_ << ", " << name_ << ")" << std::endl;
            }
        }

        Gadget& operator=(Gadget&& source)
        {
            if (this != &source)
            {
                id_ = source.id_;
                name_ = std::move(source.name_);

                std::cout << "Gadget::operator=(mv: " << id_ << ", " << name_ << ")" << std::endl;
            }

            return *this;
        }
#endif

        friend std::ostream& operator<<(std::ostream& out, const Gadget& g)
        {
            out << "Gadget(id: " << g.id() << ", name: " << g.name() << ")";
            return out;
        }

        int id() const
        {
            return id_;
        }

        std::string name() const
        {
            return name_;
        }

        void use() const
        {
            std::cout << "Using " << *this << "\n";
        }
    };

} // namespace Helpers

#endif