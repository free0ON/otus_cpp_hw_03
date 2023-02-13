#include "TAllocator.h"

/**
 * ==
 * @tparam T
 * @tparam U
 * @return
 */
template<typename T, typename U>
constexpr bool operator==(const TAllocator<T, 0> &,
                          const TAllocator<U, 0> &) noexcept
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
constexpr bool operator!=(const TAllocator<T, 0> &,
                          const TAllocator<U, 0> &) noexcept
{
    return false;
}


/**
*
*/
