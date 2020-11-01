#include <features.h>

#undef assert


/**
 * @ingroup assert
 * @par Description:
 * This API is used to insert diagnostics into programs, if expression is false, it will  output the text of the argument,
 *  the name of the source file, the source file line number, and the name of the enclosing function
 *
 * @attention
 * <ul>
 * <li>The assert() macro only prints an error message, it would not call abort,instead by locking the interrupt and entering dead cycle by doing while (1).</li>
 * </ul>
 *
 * @retval None.
 *
 * @par Dependency:
 * <ul><li>assert.h</li></ul>
 *
 * @see None
 *
 * @since Huawei LiteOS V100R001C00
 */
#ifdef NDEBUG
#define	assert(x) (void)0
#else
#define assert(x) ((void)((x) || (__assert_fail(#x, __FILE__, __LINE__, __func__),0)))
#endif

#ifdef __LITEOS__
#ifdef CONFIG_DEBUG
#define DEBUGASSERT(f) ((void)((f) || (__assert_fail(#f, __FILE__, __LINE__, __func__),0)))
#else
#define DEBUGASSERT(f)
#endif
#endif

#if __STDC_VERSION__ >= 201112L && !defined(__cplusplus)
#define static_assert _Static_assert
#endif

#ifdef __cplusplus
extern "C" {
#endif

_Noreturn void __assert_fail (const char *, const char *, int, const char *);

#ifdef __cplusplus
}
#endif
