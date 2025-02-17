#include <cstdio>
#include <utility>  // std::swap

class shared_countt {
public:
  shared_countt() noexcept
    : count_(1) {}

  void add_count() noexcept {
    ++count_;
  }

  long reduce_count() noexcept {
    return --count_;
  }

  long get_count() const noexcept {
    return count_;
  }

private:
  long count_;
};

template <typename T>
class smart_ptrr {
public:
  template <typename U>
  friend class smart_ptrr;

  explicit smart_ptrr(T* ptr = nullptr)
    : ptr_(ptr) {
    if (ptr) {
      shared_count_ =
          new shared_countt();
    }
  }

  ~smart_ptrr() {
    printf("~smart_ptr(): %p\n", this);
    if (ptr_ &&
        !shared_count_
        ->reduce_count()) {
      delete ptr_;
      delete shared_count_;
    }
  }

  template <typename U>
  smart_ptrr(const smart_ptrr<U>& other) noexcept {
    ptr_ = other.ptr_;
    if (ptr_) {
      other.shared_count_->add_count();
      shared_count_ = other.shared_count_;
    }
  }

  template <typename U>
  smart_ptrr(smart_ptrr<U>&& other) noexcept {
    ptr_ = other.ptr_;
    if (ptr_) {
      shared_count_ =
          other.shared_count_;
      other.ptr_ = nullptr;
    }
  }

  template <typename U>
  smart_ptrr(const smart_ptrr<U>& other,
             T* ptr) noexcept {
    ptr_ = ptr;
    if (ptr_) {
      other.shared_count_
          ->add_count();
      shared_count_ =
          other.shared_count_;
    }
  }

  smart_ptrr&
  operator=(smart_ptrr rhs) noexcept {
    rhs.swap(*this);
    return *this;
  }

  T* get() const noexcept {
    return ptr_;
  }

  long use_count() const noexcept {
    if (ptr_) {
      return shared_count_
          ->get_count();
    } else {
      return 0;
    }
  }

  void swap(smart_ptrr& rhs) noexcept {
    using std::swap;
    swap(ptr_, rhs.ptr_);
    swap(shared_count_,
         rhs.shared_count_);
  }

  T& operator*() const noexcept {
    return *ptr_;
  }

  T* operator->() const noexcept {
    return ptr_;
  }

  operator bool() const noexcept {
    return ptr_;
  }

private:
  T* ptr_;
  shared_countt* shared_count_;
};

template <typename T>
void swap(smart_ptrr<T>& lhs,
          smart_ptrr<T>& rhs) noexcept {
  lhs.swap(rhs);
}

template <typename T, typename U>
smart_ptrr<T> static_pointer_cast(
    const smart_ptrr<U>& other) noexcept {
  T* ptr = static_cast<T*>(other.get());
  return smart_ptrr<T>(other, ptr);
}

template <typename T, typename U>
smart_ptrr<T> reinterpret_pointer_cast(
    const smart_ptrr<U>& other) noexcept {
  T* ptr = reinterpret_cast<T*>(other.get());
  return smart_ptrr<T>(other, ptr);
}

template <typename T, typename U>
smart_ptrr<T> const_pointer_cast(
    const smart_ptrr<U>& other) noexcept {
  T* ptr = const_cast<T*>(other.get());
  return smart_ptrr<T>(other, ptr);
}

template <typename T, typename U>
smart_ptrr<T> dynamic_pointer_cast(
    const smart_ptrr<U>& other) noexcept {
  T* ptr = dynamic_cast<T*>(other.get());
  return smart_ptrr<T>(other, ptr);
}


class shape {
public:
  virtual ~shape() {}
};

class circle : public shape {
public:
  ~circle() { puts("~circle()"); }
};

int main() {
  smart_ptrr<circle> ptr1(new circle());
  printf("use count of ptr1 is %ld\n",
         ptr1.use_count());
  smart_ptrr<shape> ptr2;
  printf("use count of ptr2 was %ld\n",
         ptr2.use_count());
  ptr2 = ptr1;
  printf("use count of ptr2 is now %ld\n",
         ptr2.use_count());
  if (ptr1) {
    puts("ptr1 is not empty");
  }
}
