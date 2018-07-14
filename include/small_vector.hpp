#pragma once

#include <type_traits>
#include <vector>
#include <initializer_list>
#include <cstddef>
#include <algorithm>
#include <iterator>


namespace helene
{

template <class T, std::size_t StackBufferSize = sizeof(std::vector<T>)>
class small_vector
{
    static_assert(std::is_trivial<T>::value,
                  "small_vector is only compatible with trivial types");

public:
    typedef typename std::vector<T>::size_type size_type;
    typedef typename std::vector<T>::difference_type difference_type;
    typedef T* iterator;
    typedef const T* const_iterator;

private:
    static constexpr size_type at_least_size =
        std::max(StackBufferSize, sizeof(std::vector<T>*));
    static constexpr size_type max_stack_size_ = at_least_size / sizeof(T);

public:
    small_vector() : size_()
    {
        new(&storage_) T[max_stack_size_];
    }

    small_vector(std::initializer_list<T> init) : size_(init.size())
    {
        if(size_ > max_stack_size_)
        {
            new(&storage_) std::vector<T>(init);
        }
        else
        {
            new(&storage_) T[max_stack_size_];
            std::copy(
                init.begin(), init.end(), reinterpret_cast<T*>(&storage_));
        }
    }

    small_vector(size_type count) : size_(count)
    {
        if(size_ > max_stack_size_)
        {
            new(&storage_) std::vector<T>(count);
        }
        else
        {
            new(&storage_) T[max_stack_size_];
            std::fill_n(reinterpret_cast<T*>(&storage_), size_, T{});
        }
    }

    small_vector(size_type count, const T& value)
    {
        if(size_ > max_stack_size_)
        {
            new(&storage_) std::vector<T>(count, value);
        }
        else
        {
            new(&storage_) T[max_stack_size_];
            std::fill_n(reinterpret_cast<T*>(&storage_), size_, value);
        }
    }

    ~small_vector()
    {
        if(size_ > max_stack_size_)
        {
            reinterpret_cast<std::vector<T>*>(&storage_)->~vector();
        }
    }

public:
    void
    push_back(const T& value)
    {
        if(size_ > max_stack_size_)
        {
            reinterpret_cast<std::vector<T>*>(&storage_)->push_back(value);
        }
        else if(size_ == max_stack_size_)
        {
            T* buff = reinterpret_cast<T*>(&storage_);
            T temp[max_stack_size_];
            std::copy(buff, buff + size_, std::begin(temp));

            new(&storage_) std::vector<T>(std::begin(temp), std::end(temp));
            reinterpret_cast<std::vector<T>*>(&storage_)->push_back(value);
        }
        else
        {
            reinterpret_cast<T*>(&storage_)[size_] = value;
        }

        ++size_;
    }

    void
    pop_back()
    {
        if(size_ > max_stack_size_ + 1) // remain on heap
        {
            reinterpret_cast<std::vector<T>*>(&storage_)->pop_back();
            --size_;
        }
        else if(size_ == max_stack_size_ + 1) // transition to stack
        {
            // reinterpret_cast<std::vector<T>*>(&storage_)->pop_back();
            // not necessary, trivially destructible

            std::vector<T> temp(
                std::move(*(reinterpret_cast<std::vector<T>*>(&storage_))));
            reinterpret_cast<std::vector<T>*>(&storage_)->~vector();

            new(&storage_) T[max_stack_size_];
            std::copy(
                temp.begin(), temp.end() - 1, reinterpret_cast<T*>(&storage_));
            --size_;
        }
        else // remain on stack
        {
            --size_;
        }
    }

    T& operator[](size_type n)
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<std::vector<T>*>(&storage_)->operator[](n);
        }
        else
        {
            return reinterpret_cast<T*>(&storage_)[n];
        }
    }

    const T& operator[](size_type n) const
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<const std::vector<T>*>(&storage_)->
            operator[](n);
        }
        else
        {
            return reinterpret_cast<const T*>(&storage_)[n];
        }
    }

    T&
    front()
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<std::vector<T>*>(&storage_)->front();
        }
        else
        {
            return reinterpret_cast<T*>(&storage_)[0];
        }
    }

    const T&
    front() const
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<const std::vector<T>*>(&storage_)->front();
        }
        else
        {
            return reinterpret_cast<const T*>(&storage_)[0];
        }
    }


    T&
    back()
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<std::vector<T>*>(&storage_)->back();
        }
        else
        {
            return reinterpret_cast<T*>(&storage_)[size_ - 1];
        }
    }

    const T&
    back() const
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<const std::vector<T>*>(&storage_)->back();
        }
        else
        {
            return reinterpret_cast<const T*>(&storage_)[size_ - 1];
        }
    }

    iterator
    erase(iterator pos)
    {
        if(size_ > max_stack_size_ + 1)
        {
            std::vector<T>* ref = reinterpret_cast<std::vector<T>*>(&storage_);

            // convert to std::vector<T>::iterator
            typename std::vector<T>::iterator iter_pos =
                ref->begin() + (pos - ref->data());

            auto iter_out = ref->erase(iter_pos);
            --size_;

            // convert std::vector<T>::iterator to pointer
            difference_type diff = iter_out - ref->begin();


            return ref->data() + diff;
        }
        else if(size_ == max_stack_size_ + 1)
        {
            std::vector<T>* ref = reinterpret_cast<std::vector<T>*>(&storage_);

            // convert to std::vector<T>::iterator
            const auto diff = pos - ref->data();
            typename std::vector<T>::iterator iter_pos =
                ref->begin() + (pos - ref->data());


            auto iter_out = ref->erase(iter_pos);

            std::vector<T> temp(std::move(*ref));
            ref->~vector();

            new(&storage_) T[max_stack_size_];
            std::copy(
                temp.begin(), temp.end(), reinterpret_cast<T*>(&storage_));
            --size_;

            return reinterpret_cast<T*>(&storage_) + diff;
        }
        else
        {
            T* beg = reinterpret_cast<T*>(&storage_);
            std::copy(pos + 1, end(), pos);

            --size_;
            return pos;
        }
    }

    iterator
    erase(iterator first, iterator last)
    {
        const auto erase_size = std::distance(first, last);

        if(size_ > max_stack_size_ + erase_size)
        {
            std::vector<T>* ref = reinterpret_cast<std::vector<T>*>(&storage_);

            // convert to std::vector<T>::iterator
            auto first_iter = ref->begin() + (first - begin());
            auto last_iter = ref->begin() + (last - begin());

            auto iter_out = ref->erase(first_iter, last_iter);
            size_ -= erase_size;

            // convert back to simple pointer
            return begin() + (iter_out - ref->begin());
        }
        else if(size_ <= max_stack_size_)
        {
            T* buff_beg = reinterpret_cast<T*>(&storage_);
            if(last + 1 < end()) // if anything left past end of range to erase
            {
                std::copy(last + 1, end(), first);
            }
            size_ -= erase_size;
            return first;
        }
        else
        {
            std::vector<T>* ref = reinterpret_cast<std::vector<T>*>(&storage_);

            // convert to std::vector<T>::iterator
            auto first_iter = ref->begin() + (first - begin());
            auto last_iter = ref->begin() + (last - begin());

            // carry out erase on vector
            auto iter_out = ref->erase(first_iter, last_iter);
            const auto out_diff = iter_out - ref->begin();

            std::vector<T> temp(std::move(*ref));
            ref->~vector();

            new(&storage_) T[max_stack_size_];

            std::copy(
                temp.begin(), temp.end(), reinterpret_cast<T*>(&storage_));

            size_ -= erase_size;

            return begin() + out_diff;
        }
    }


    T*
    data()
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<std::vector<T>*>(&storage_)->data();
        }
        else
        {
            return reinterpret_cast<T*>(&storage_);
        }
    }

    const T*
    data() const
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<const std::vector<T>*>(&storage_)->data();
        }
        else
        {
            return reinterpret_cast<const T*>(&storage_);
        }
    }

    iterator
    begin()
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<std::vector<T>*>(&storage_)->data();
        }
        else
        {
            return reinterpret_cast<T*>(&storage_);
        }
    }

    iterator
    end()
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<std::vector<T>*>(&storage_)->data() + size_;
        }
        else
        {
            return reinterpret_cast<T*>(&storage_) + size_;
        }
    }

    const_iterator
    cbegin() const
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<const std::vector<T>*>(&storage_)->data();
        }
        else
        {
            return reinterpret_cast<const T*>(&storage_);
        }
    }

    const_iterator
    cend() const
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<const std::vector<T>*>(&storage_)->data() +
                   size_;
        }
        else
        {
            return reinterpret_cast<const T*>(&storage_) + size_;
        }
    }

    size_type
    size() const
    {
        return size_;
    }

    bool
    empty() const
    {
        return size_ == 0;
    }

    static constexpr size_type
    max_stack_size()
    {
        return max_stack_size_;
    }

    bool
    on_stack() const
    {
        return size_ <= max_stack_size_;
    }

private:
    std::aligned_storage_t<at_least_size> storage_;
    size_type size_;
};

} // namespace helene
