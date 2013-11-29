#include <iostream>
#include <vector>
#include <algorithm>

#include "llvm_shadow_stack.hpp"


struct element
{
    void* pointer;
    bool marked;
};


static class GC
{
public:
    int32_t* alloc()
    {
        if ( elements_.size() >= 10 ) {
            // clean up garbages
            std::cout << "-- Triggered Garbage Collector!!" << std::endl;
            all_collect();
        }

        int32_t* const p = new int;
        element e{ p, false };

        elements_.emplace_back( e );

        return p;
    }

public:
    void all_collect()
    {
        for( auto& elm : elements_ )
            elm.marked = false;

        //
        std::cout << "!! Start to visit ShadowStack" << std::endl;
        visitGCRoots( [&]( void **Root, const void *Meta ) {
            std::cout << "-> Alived pointer: " << "(" << *Root << ")" << std::endl;

            auto const it = find_if( elements_.begin(), elements_.end(), [&]( element const& elm ) {
                return elm.pointer == *Root;
            } );
            if ( it != elements_.end() )
                it->marked = true;
        } );

        // prints
        show_object_status();

        //
        for( auto& elm : elements_ ) {
            if ( elm.marked == false ) {
                delete static_cast<int*>( elm.pointer );
                std::cout << "! Destructed( Addr: " << elm.pointer << " )" << std::endl;
            }
        }
        auto const it = remove_if( elements_.begin(), elements_.end(), []( element const& elm ) {
            return elm.marked == false;
        } );
        elements_.erase( it, elements_.end() );

        //
        std::cout << "!! Garbege Collected" << std::endl;
        show_object_status();

        std::cout << "-- Finished" << std::endl;
    }

private:
    void show_object_status() const
    {
        std::cout << "- Object Status" << std::endl;
        for( auto const& elm : elements_ ) {
            std::cout << "? Addr: " << elm.pointer << "(" << ( elm.marked ? "Alive" : "Dead" ) << ")" << std::endl;
        }
    }

private:
    std::vector<element> elements_;
} gc;


// called from test_function.ll
extern "C"
{
    int32_t* my_alloc()
    {
        return gc.alloc();
    }

    void put_int32( int32_t const* const ptr )
    {
        std::cout << "put_int32: " << std::dec << *ptr << " (Addr: " << std::hex << ptr << ")" << std::endl;
    }

    void put_int32_inner( int32_t const* const ptr, int const base_i )
    {
        std::cout << "from: " << base_i << " > ";
        put_int32( ptr );
    }

    void put_line()
    {
        std::cout << "==========" << std::endl;
    }
}
