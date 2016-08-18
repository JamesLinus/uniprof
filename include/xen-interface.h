#ifndef __XEN_INTERFACE_H
#define __XEN_INTERFACE_H

#include <config.h>

#if defined(HYPERCALL_XENCALL) + defined(HYPERCALL_LIBXC) == 0
#error Define exactly one of HYPERCALL_LIBXC, HYPERCALL_XENCALL
#elif defined(HYPERCALL_XENCALL) + defined(HYPERCALL_LIBXC) > 1
#warning You defined more than one of HYPERCALL_LIBXC, HYPERCALL_XENCALL. This might lead to unexpected results.
#endif

#undef DBG
#ifdef DEBUG
#define DBG(string, args...) printf("[DBG %s:%s] "string, __FILE__, __func__, ##args)
#else
#define DBG(args...)
#endif /* DEBUG */

#if defined(HYPERCALL_XENCALL)
#include <xenctrl.h>
#include <xencall.h>
#include <xenforeignmemory.h>
#define HYPERCALL_NAME "libxencall"
typedef vcpu_guest_context_t vcpu_guest_context_transparent_t;
extern xencall_handle *callh;
extern xenforeignmemory_handle *fmemh;
#elif defined(HYPERCALL_LIBXC)
#define XC_WANT_COMPAT_MAP_FOREIGN_API
#include <xenctrl.h>
#define HYPERCALL_NAME "libxc"
typedef vcpu_guest_context_any_t vcpu_guest_context_transparent_t;
extern xc_interface *xc_handle;
#endif

#ifndef __maybe_unused
#define __maybe_unused __attribute__((unused))
#endif

#define PAGE_SHIFT XC_PAGE_SHIFT
#define PAGE_SIZE  XC_PAGE_SIZE
#define PAGE_MASK  XC_PAGE_MASK

// big enough for 32 bit and 64 bit
typedef uint64_t guest_word_t;

int xen_interface_open(void);
int xen_interface_close(void);
int get_word_size(int domid);
guest_word_t instruction_pointer(vcpu_guest_context_transparent_t *vc);
guest_word_t frame_pointer(vcpu_guest_context_transparent_t *vc);
int get_vcpu_context(int domid, int vcpu, vcpu_guest_context_transparent_t *vc);
void xen_map_domu_page(int domid, int vcpu, uint64_t addr, unsigned long *mfn, void **buf);
int pause_domain(int domid);
int unpause_domain(int domid);
int get_max_vcpu_id(int domid);

#endif /* __XEN_INTERFACE_H */