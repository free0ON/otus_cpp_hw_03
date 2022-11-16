#include "TAllocator.h"

/**
 * ==
 * @tparam T
 * @tparam U
 * @return
 */
template<typename T, typename U>
constexpr bool operator==(const TAllocator<T> &,
                          const TAllocator<U> &) noexcept
{
    return true;
}

/**
 * !=
 * @tparam T
 * @tparam U
 * @return
 */
template<typename T, typename U>
constexpr bool operator!=(const TAllocator<T> &,
                          const TAllocator<U> &) noexcept
{
    return false;
}


/**
*
*/
